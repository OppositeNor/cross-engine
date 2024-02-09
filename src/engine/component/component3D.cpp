#include "ce/component/component3D.h"

namespace CrossEngine
{
    void Component3D::SetSubspaceMatrixDirty()
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

    void Component3D::UpdateSubspaceMatrix() const
    {
        if (GetParent().expired())
            subspace_matrix = Math::Model(position, rotation, scale);
        else
            subspace_matrix = GetParent().lock()->GetSubspaceMatrix() * Math::Model(position, rotation, scale);
        subspace_matrix_dirty = false;
    }

    void Component3D::UpdateSubspaceMatrixInverse() const
    {
        if (GetParent().expired())
            subspace_matrix_inverse = Math::ModelInv(position, rotation, scale);
        else
            subspace_matrix_inverse = Math::ModelInv(position, rotation, scale) * GetParent().lock()->GetSubspaceMatrixInverse();
        subspace_matrix_inverse_dirty = false;
    }

    Component3D::Component3D(const std::string& p_component_name)
        : position(0.0f, 0.0f, 0.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f, 1.0f, 1.0f), Component(p_component_name)
    {

    }

    Component3D::Component3D(const Component3D& p_other)
        : Component(p_other)
    {
        position = p_other.position;
        rotation = p_other.rotation;
        scale = p_other.scale;
        
    }

    Component3D::Component3D(Component3D&& p_other)
        : Component(std::move(p_other))
    {
        position = p_other.position;
        rotation = p_other.rotation;
        scale = p_other.scale;
    }
    
    Math::Vec4& Component3D::Position()
    {
        SetSubspaceMatrixDirty();
        return position;
    }

    Math::Vec4& Component3D::Rotation()
    {
        rotation.Normalize();
        SetSubspaceMatrixDirty();
        return rotation;
    }

    void Component3D::SetRotationEuler(const Math::Vec4& p_rotation, EulerRotOrder p_order)
    {
        Rotation() = EulerToQuat(p_rotation, p_order);
    }

    void Component3D::SetRotationEuler(const Math::Vec3& p_rotation, EulerRotOrder p_order)
    {
        Rotation() = EulerToQuat(p_rotation, p_order);
    }

    Math::Vec4 Component3D::GetRotationEuler() const
    {
        //todo
        throw std::runtime_error("Not implemented");
        return Math::Vec4();
    }

    void Component3D::Rotate(Math::Vec4 p_axis, float p_angle)
    {
        p_axis.Normalize();
        float half_angle = p_angle / 2.0f;
        float sin_half_angle = std::sin(half_angle);
        Math::Vec4 quat;
        quat[3] = std::cos(half_angle);
        quat[0] = p_axis[0] * sin_half_angle;
        quat[1] = p_axis[1] * sin_half_angle;
        quat[2] = p_axis[2] * sin_half_angle;
        Rotation() = QuatProd(quat, rotation);
    }

    void Component3D::SetRotate(Math::Vec4 p_axis, float p_angle)
    {
        p_axis.Normalize();
        float half_angle = p_angle / 2.0f;
        float sin_half_angle = std::sin(half_angle);
        Math::Vec4 quat;
        quat[3] = std::cos(half_angle);
        quat[0] = p_axis[0] * sin_half_angle;
        quat[1] = p_axis[1] * sin_half_angle;
        quat[2] = p_axis[2] * sin_half_angle;
        Rotation() = quat;
    }

    void Component3D::Scale(Math::Vec4 p_direction, float p_scale)
    {
        p_direction.Normalize();
        Scale() *= p_direction * p_scale;
    }

    Math::Vec4& Component3D::Scale()
    {
        SetSubspaceMatrixDirty();
        return scale;
    }

    Math::Vec4 Component3D::GetGlobalPosition() const
    {
        return GetSubspaceMatrix() * Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void Component3D::SetGlobalPosition(const Math::Vec4& p_position)
    {
        if (GetParent().expired())
            Position() = p_position;
        else
            Position() = GetParent().lock()->GetSubspaceMatrixInverse() * p_position;
    }

    void Component3D::Move(const Math::Vec4& p_direction, float p_distance)
    {
        Position() += p_direction.Normalized() * p_distance;
    }
    
    const Math::Mat4& Component3D::GetSubspaceMatrix() const
    {
        if (subspace_matrix_dirty)
        {
            std::lock_guard<std::mutex> lock(subspace_matrix_mutex);
            if (subspace_matrix_dirty)
                UpdateSubspaceMatrix();
        }
        return subspace_matrix;
    }

    const Math::Mat4& Component3D::GetSubspaceMatrixInverse() const
    {
        if (subspace_matrix_inverse_dirty)
        {
            std::lock_guard<std::mutex> lock(subspace_matrix_inverse_mutex);
            if (subspace_matrix_inverse_dirty)
                UpdateSubspaceMatrixInverse();
        }
        return subspace_matrix_inverse;
    }
}