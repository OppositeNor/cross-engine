#include "ce/component/component.h"
#include "ce/graphics/window.h"


Component::Component(Window* p_context)
    : position(0.0f, 0.0f, 0.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f, 1.0f, 1.0f),
        context(p_context)
{

}

Component::Component(const Component& p_other)
{
    position = p_other.position;
    rotation = p_other.rotation;
    scale = p_other.scale;
    SetSubspaceMatrixDirty();

    if (!p_other.parent.expired())
        p_other.parent.lock()->AddChild(shared_from_this());
    
}

Component::Component(Component&& p_other) noexcept
{
    position = p_other.position;
    rotation = p_other.rotation;
    scale = p_other.scale;
    SetSubspaceMatrixDirty();

    if (!p_other.parent.expired())
        p_other.parent.lock()->AddChild(shared_from_this());
    p_other.parent.reset();
    children = std::move(p_other.children);

    context = p_other.context;
    p_other.context = nullptr;
}

Component::~Component()
{

}

void Component::Update(float p_delta)
{
    Process(p_delta);
    for (auto& child : children)
    {
        if (child.expired())
            continue;
        child.lock()->Update(p_delta);
    }
}

void Component::RemoveChild(Component* p_child)
{
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if ((*it).lock().get() == p_child)
        {
            children.erase(it);
            break;
        }
    }
    p_child->parent.reset();
}

void Component::AddChild(WPComponent p_child)
{
    for (auto i : children)
    {
        if (i.lock().get() == p_child.lock().get())
            return;
    }
    children.push_back(p_child);
    auto child = p_child.lock();
    if (!child->parent.expired())
        child->parent.lock()->RemoveChild(child.get());
    child->SetSubspaceMatrixDirty();
    child->parent = shared_from_this();
    child->Ready();
}

const Math::Mat4& Component::GetSubspaceMatrix() const
{
    if (subspace_matrix_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_mutex);
        if (subspace_matrix_dirty)
            UpdateSubspaceMatrix();
    }
    return subspace_matrix;
}

const Math::Mat4& Component::GetSubspaceMatrixInverse() const
{
    if (subspace_matrix_inverse_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_inverse_mutex);
        if (subspace_matrix_inverse_dirty)
            UpdateSubspaceMatrixInverse();
    }
    return subspace_matrix_inverse;
}

Math::Vec4& Component::Position()
{
    SetSubspaceMatrixDirty();
    return position;
}

Math::Vec4& Component::Rotation()
{
    rotation.Normalize();
    SetSubspaceMatrixDirty();
    return rotation;
}

Math::Vec4& Component::Scale()
{
    SetSubspaceMatrixDirty();
    return scale;
}

void Component::Move(Math::Vec4 p_direction, float p_distance)
{
    p_direction.Normalize();
    Position() += p_direction * p_distance;
}

void Component::Rotate(Math::Vec4 p_direction, float p_angle)
{
    p_direction.Normalize();
    float half_angle = p_angle / 2.0f;
    float sin_half_angle = std::sin(half_angle);
    Math::Vec4 quat;
    quat[3] = std::cos(half_angle);
    quat[0] = p_direction[0] * sin_half_angle;
    quat[1] = p_direction[1] * sin_half_angle;
    quat[2] = p_direction[2] * sin_half_angle;
    Rotation() = QuatProd(quat, rotation);
}

void Component::Scale(Math::Vec4 p_direction, float p_scale)
{
    p_direction.Normalize();
    Scale() *= p_direction * p_scale;
}

Math::Vec4 Component::GetDirection() const
{
    return (Math::RotQuaternion(rotation) *Math::Vec4(0, 0, 1, 0)).Normalized();
}

void Component::SetSubspaceMatrixDirty()
{
    if (!subspace_matrix_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_mutex);
        if (!subspace_matrix_dirty)
        {
            subspace_matrix_dirty = true;
            SetChildrenSubspaceMatrixDirty();
        }
    }
    if (!subspace_matrix_inverse_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_inverse_mutex);
        if (!subspace_matrix_inverse_dirty)
        {
            subspace_matrix_inverse_dirty = true;
            SetChildrenSubspaceMatrixInverseDirty();
        }
    }
}

void Component::SetChildrenSubspaceMatrixDirty()
{
    for (auto& child : children)
    {
        auto temp_child = child.lock();
        if (!temp_child->subspace_matrix_dirty)
        {
            std::lock_guard<std::mutex> lock(temp_child->subspace_matrix_mutex);
            if (!temp_child->subspace_matrix_dirty)
            {
                temp_child->subspace_matrix_dirty = true;
                temp_child->SetChildrenSubspaceMatrixDirty();
            }
        }
        temp_child->SetChildrenSubspaceMatrixDirty();
    }
}

void Component::SetChildrenSubspaceMatrixInverseDirty()
{
    for (auto& child : children)
    {
        auto temp_child = child.lock();
        if (!temp_child->subspace_matrix_inverse_dirty)
        {
            std::lock_guard<std::mutex> lock(temp_child->subspace_matrix_inverse_mutex);
            if (!temp_child->subspace_matrix_inverse_dirty)
            {
                temp_child->subspace_matrix_inverse_dirty = true;
                temp_child->SetChildrenSubspaceMatrixInverseDirty();
            }
        }
        temp_child->SetChildrenSubspaceMatrixInverseDirty();
    }

}

void Component::UpdateSubspaceMatrix() const
{
    if (parent.expired())
        subspace_matrix = Math::Model(position, rotation, scale);
    else
        subspace_matrix = parent.lock()->GetSubspaceMatrix() * Math::Model(position, rotation, scale);
    subspace_matrix_dirty = false;
}

void Component::UpdateSubspaceMatrixInverse() const
{
    if (parent.expired())
        subspace_matrix_inverse = Math::ModelInv(position, rotation, scale);
    else
        subspace_matrix_inverse = Math::ModelInv(position, rotation, scale) * parent.lock()->GetSubspaceMatrixInverse();
    subspace_matrix_inverse_dirty = false;
}

const Math::Mat4& Component::GetModelMatrix()
{
    return GetSubspaceMatrix();
}

Math::Vec4 Component::GetGlobalPosition() const
{
    return GetSubspaceMatrix() *Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Component::SetGlobalPosition(const Math::Vec4& p_position)
{
    if (parent.expired())
        Position() = p_position;
    else
        Position() = parent.lock()->GetSubspaceMatrixInverse() * p_position;
}

void Component::SetRotationEuler(const Math::Vec4& p_rotation, EulerRotOrder p_order)
{
    Rotation() = EulerToQuat(p_rotation, p_order);
}

void Component::SetRotationEuler(const Math::Vec3& p_rotation, EulerRotOrder p_order)
{
    Rotation() = EulerToQuat(p_rotation, p_order);
}

Math::Vec4 Component::GetRotationEuler() const
{
    //todo
    throw std::runtime_error("Not implemented");
    return Math::Vec4();
}

void Component::Draw()
{
    if (!visible)
        return;
    for (auto& child : children)
    {
        if (child.expired())
            continue;
        child.lock()->Draw();
    }
}