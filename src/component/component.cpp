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

    if (p_other.parent != nullptr)
        p_other.parent->AddChild(this);
    for (auto& child : p_other.children)
        children.push_back(new Component(*child));
    
}

Component::Component(Component&& p_other) noexcept
{
    position = p_other.position;
    rotation = p_other.rotation;
    scale = p_other.scale;
    SetSubspaceMatrixDirty();

    if (p_other.parent != nullptr)
        p_other.parent->AddChild(this);
    p_other.parent = nullptr;
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
        child->Update(p_delta);
}

void Component::RemoveChild(Component* p_child)
{
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it == p_child)
        {
            children.erase(it);
            break;
        }
    }
    p_child->parent = nullptr;
}

void Component::AddChild(Component* p_child)
{
    for (auto i : children)
    {
        if (i == p_child)
            return;
    }
    children.push_back(p_child);
    if (p_child->parent != nullptr)
        p_child->parent->RemoveChild(p_child);
    p_child->SetSubspaceMatrixDirty();
    p_child->parent = this;
}

const Mat4& Component::GetSubspaceMatrix() const
{
    if (subspace_matrix_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_mutex);
        if (subspace_matrix_dirty)
            UpdateSubspaceMatrix();
    }
    return subspace_matrix;
}

const Mat4& Component::GetSubspaceMatrixInverse() const
{
    if (subspace_matrix_inverse_dirty)
    {
        std::lock_guard<std::mutex> lock(subspace_matrix_inverse_mutex);
        if (subspace_matrix_inverse_dirty)
            UpdateSubspaceMatrixInverse();
    }
    return subspace_matrix_inverse;
}

Vec4& Component::Position()
{
    SetSubspaceMatrixDirty();
    return position;
}

Vec4& Component::Rotation()
{
    rotation.Normalize();
    SetSubspaceMatrixDirty();
    return rotation;
}

Vec4& Component::Scale()
{
    SetSubspaceMatrixDirty();
    return scale;
}

void Component::Move(Vec4 p_direction, float p_distance)
{
    p_direction.Normalize();
    Position() += p_direction * p_distance;
}

void Component::Rotate(Vec4 p_direction, float p_angle)
{
    p_direction.Normalize();
    float half_angle = p_angle / 2.0f;
    float sin_half_angle = std::sin(half_angle);
    Vec4 quat;
    quat[3] = std::cos(half_angle);
    quat[0] = p_direction[0] * sin_half_angle;
    quat[1] = p_direction[1] * sin_half_angle;
    quat[2] = p_direction[2] * sin_half_angle;
    Rotation() = QuatProd(quat, rotation);
}

void Component::Scale(Vec4 p_direction, float p_scale)
{
    p_direction.Normalize();
    Scale() *= p_direction * p_scale;
}

Vec4 Component::GetDirection() const
{
    return (Mat4::RotQuaternion(rotation) * Vec4(0, 0, 1, 0)).Normalized();
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
        if (!child->subspace_matrix_dirty)
        {
            std::lock_guard<std::mutex> lock(child->subspace_matrix_mutex);
            if (!child->subspace_matrix_dirty)
            {
                child->subspace_matrix_dirty = true;
                child->SetChildrenSubspaceMatrixDirty();
            }
        }
        child->SetChildrenSubspaceMatrixDirty();
    }
}

void Component::SetChildrenSubspaceMatrixInverseDirty()
{
    for (auto& child : children)
    {
        if (!child->subspace_matrix_inverse_dirty)
        {
            std::lock_guard<std::mutex> lock(child->subspace_matrix_inverse_mutex);
            if (!child->subspace_matrix_inverse_dirty)
            {
                child->subspace_matrix_inverse_dirty = true;
                child->SetChildrenSubspaceMatrixInverseDirty();
            }
        }
        child->SetChildrenSubspaceMatrixInverseDirty();
    }

}

void Component::UpdateSubspaceMatrix() const
{
    if (parent == nullptr)
        subspace_matrix = Mat4::Model(position, rotation, scale);
    else
        subspace_matrix = parent->GetSubspaceMatrix() * Mat4::Model(position, rotation, scale);
    subspace_matrix_dirty = false;
}

void Component::UpdateSubspaceMatrixInverse() const
{
    if (parent == nullptr)
        subspace_matrix_inverse = Mat4::ModelInv(position, rotation, scale);
    else
        subspace_matrix_inverse = Mat4::ModelInv(position, rotation, scale) * parent->GetSubspaceMatrixInverse();
    subspace_matrix_inverse_dirty = false;
}

const Mat4& Component::GetModelMatrix()
{
    return GetSubspaceMatrix();
}

Vec4 Component::GetGlobalPosition() const
{
    return GetSubspaceMatrix() * Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Component::SetGlobalPosition(const Vec4& p_position)
{
    if (parent == nullptr)
        Position() = p_position;
    else
        Position() = parent->GetSubspaceMatrixInverse() * p_position;
}

void Component::SetRotationEuler(const Vec4& p_rotation, EulerRotOrder p_order)
{
    Rotation() = EulerToQuat(p_rotation, p_order);
}

void Component::SetRotationEuler(const Vec3& p_rotation, EulerRotOrder p_order)
{
    Rotation() = EulerToQuat(p_rotation, p_order);
}

Vec4 Component::GetRotationEuler() const
{
    //todo
    throw std::runtime_error("Not implemented");
    return Vec4();
}

void Component::Draw()
{
    if (!visible)
        return;
    for (auto& child : children)
        child->Draw();
}