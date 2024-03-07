#pragma once

#include "ce/component/component.h"

namespace CrossEngine
{
    class Component3D : public Component
    {
    private:
        Math::Vec4 position;
        Math::Vec4 rotation;
        Math::Vec4 scale;

        mutable Math::Mat4 subspace_matrix;
        mutable bool subspace_matrix_dirty = true;
        mutable std::mutex subspace_matrix_mutex;

        mutable Math::Mat4 subspace_matrix_inverse;
        mutable bool subspace_matrix_inverse_dirty = true;
        mutable std::mutex subspace_matrix_inverse_mutex;
    protected:
        void SetSubspaceMatrixDirty() final;

        void UpdateSubspaceMatrix() const;

        void UpdateSubspaceMatrixInverse() const;

    public:
        Component3D(const std::string& p_component_name = "component 3d");

        Component3D(const Component3D& p_other);
        
        Component3D(Component3D&& p_other);

        /**
         * @brief Get the position of the component.
         * 
         * @return const Math::Vec3& The position of the component.
         */
        FORCE_INLINE const Math::Vec4& GetPosition() const { return position; }

        /**
         * @brief Get the reference of the position of the component.
         * 
         * @return Math::Vec3& The reference of the position of the component.
         */
        Math::Vec4& Position();

        /**
         * @brief Get the rotation of the component. The rotation
         * is ordered as pitch, yaw, roll.
         * 
         * @return const Math::Vec3& The rotation of the component.
         */
        FORCE_INLINE Math::Vec4 GetRotation() const { return rotation.Normalized(); }

        /**
         * @brief Get the rotation of the component. The rotation
         * is ordered as pitch, yaw, roll.
         * 
         * @return Math::Vec3& The rotation of the component.
         */
        Math::Vec4& Rotation();

        /**
         * @brief Set the rotation of the component in Euler angle.
         * The rotation is ordered as pitch, yaw, roll.
         * 
         * @param p_rotation The rotation of the component.
         */
        void SetRotationEuler(const Math::Vec4& p_rotation, EulerRotOrder p_order);

        /**
         * @brief Set the rotation of the component in Euler angle.
         * The rotation is ordered as pitch, yaw, roll.
         * 
         * @param p_rotation The rotation of the component.
         */
        void SetRotationEuler(const Math::Vec3& p_rotation, EulerRotOrder p_order);

        /**
         * @brief Get the rotation of the component in Euler angle.
         * The rotation is ordered as pitch, yaw, roll.
         * 
         * @return const Math::Vec3& The rotation of the component.
         */
        Math::Vec4 GetRotationEuler() const;

        
        /**
         * @brief Rotate the component in the direction.
         * 
         * @param p_axis The axis to rotate the component.
         * @param p_angle The angle to rotate the component.
         */
        void Rotate(Math::Vec4 p_axis, float p_angle);

        /**
         * @brief Set the rotation of the component in the direction.
         * 
         * @param p_axis The axis to rotate the component.
         * @param p_angle The angle to rotate the component.
         */
        void SetRotate(Math::Vec4 p_axis, float p_angle);

        /**
         * @brief Scale the component in the direction.
         * 
         * @param p_direction The direction to scale the component.
         * @param p_scale The scale to scale the component.
         */
        void Scale(Math::Vec4 p_direction, float p_scale);

        /**
         * @brief Get the scale of the component.
         * 
         * @return const Math::Vec3& The scale of the component.
         */
        FORCE_INLINE const Math::Vec4& GetScale() const { return scale; }

        /**
         * @brief Get the scale of the component.
         * 
         * @return Math::Vec3& The scale of the component.
         */
        Math::Vec4& Scale();

        /**
         * @brief Get the global position of this component.
         * 
         * @return const Math::Vec4& The global position of this component.
         */
        Math::Vec4 GetGlobalPosition() const;

        /**
         * @brief Get the global direction of this component.
         * @note The returned vector is not normalized. If you want the normalized vector, use GetGlobalDirectionNormalized.
         * @return Math::Vec4 The global direction of this component.
         */
        Math::Vec4 GetGlobalDirection() const;

        /**
         * @brief Get the normalized global direction of this component.
         * @note The returned vector is normalized. If you want the unnormalized vector, use GetGlobalDirection.
         * @return Math::Vec4 The global direction of this component.
         */
        Math::Vec4 GetGlobalDirectionNormalized() const;

        /**
         * @brief Set the position of this component.
         * 
         * @param p_position The new position of this component.
         */
        void SetGlobalPosition(const Math::Vec4& p_position);

        /**
         * @brief Move the component in the direction.
         * 
         * @param p_direction The direction to move the component.
         * @param p_distance The distance to move the component.
         */
        void Move(const Math::Vec4& p_direction, float p_distance);

        /**
         * @brief Get the subspace matrix of this component.
         * 
         * @return const Math::Mat4& The subspace matrix of this component.
         */
        const Math::Mat4& GetSubspaceMatrix() const final;

        /**
         * @brief Get the inverse subspace matrix of this component.
         * 
         * @return Math::Mat4 The inverse subspace matrix of this component.
         */
        const Math::Mat4& GetSubspaceMatrixInverse() const final;

        /**
         * @brief Get the front direction of the component.
         * 
         * @return Math::Vec4 The front direction of the component.
         */
        FORCE_INLINE Math::Vec4 GetFront() const { return (GetSubspaceMatrix() * Math::FRONT<4>).Normalize(); }

        /**
         * @brief Get the right direction of the component.
         * 
         * @return Math::Vec4 The right direction of the component.
         */
        Math::Vec4 GetUp() const { return (GetSubspaceMatrix() * Math::UP<4>).Normalize(); }

        /**
         * @brief Get the right direction of the component.
         * 
         * @return Math::Vec4 The right direction of the component.
         */
        Math::Vec4 GetRight() const { return (GetSubspaceMatrix() * Math::RIGHT<4>).Normalize(); };
        
        FORCE_INLINE Math::Vec4 GetDirection() const { return GetFront(); };
    };
}