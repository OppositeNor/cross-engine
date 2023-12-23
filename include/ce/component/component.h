#pragma once
#include "ce/math/math.hpp"
#include <vector>
#include <mutex>

class Window;

/**
 * @brief A Component is anything that can existed in the game.
 */
class Component
    : public std::enable_shared_from_this<Component>
{
private:
    using WPComponent = std::weak_ptr<Component>;
    Window* context = nullptr;

     Math::Vec4 position;
     Math::Vec4 rotation;
     Math::Vec4 scale;
    void SetSubspaceMatrixDirty();

    mutable Math::Mat4 subspace_matrix;
    mutable bool subspace_matrix_dirty = true;
    mutable std::mutex subspace_matrix_mutex;
    void SetChildrenSubspaceMatrixDirty();

    mutable Math::Mat4 subspace_matrix_inverse;
    mutable bool subspace_matrix_inverse_dirty = true;
    mutable std::mutex subspace_matrix_inverse_mutex;
    void SetChildrenSubspaceMatrixInverseDirty();


    WPComponent parent;
    std::vector<WPComponent> children;

    void UpdateSubspaceMatrix() const;
    void UpdateSubspaceMatrixInverse() const;

    bool visible = true;

public:
    Component(Window* p_context);
    Component(const Component& p_other);
    Component(Component&& p_other) noexcept;
    virtual ~Component();
    
    /**
     * @brief Called every frame.
     * This function calles every neccessary function to update the component.
     * @note When overriding this method, make sure to call the base method.
     */
    virtual void Update(float p_delta);

    /**
     * @brief Called once when the component is added.
     * 
     */
    virtual void Ready() {}
    /**
     * @brief Called every frame.
     * 
     * @param p_delta The time in seconds since the last frame.
     */
    virtual void Process(float p_delta) {}

    /**
     * @brief Called once when the component is removed.
     */
    virtual void Leave() {}

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
     * @brief Move the component in the direction.
     * 
     * @param p_direction The direction to move the component.
     * @param p_distance The distance to move the component.
     */
    void Move(Math::Vec4 p_direction, float p_distance);

    /**
     * @brief Rotate the component in the direction.
     * 
     * @param p_direction The direction to rotate the component.
     * @param p_angle The angle to rotate the component.
     */
    void Rotate(Math::Vec4 p_direction, float p_angle);

    /**
     * @brief Scale the component in the direction.
     * 
     * @param p_direction The direction to scale the component.
     * @param p_scale The scale to scale the component.
     */
    void Scale(Math::Vec4 p_direction, float p_scale);

    Math::Vec4 GetDirection() const;

    /**
     * @brief Remove a child from this component.
     * 
     * @param p_child The child to be removed.
     */
    void RemoveChild(Component* p_child);

    /**
     * @brief Add a child to this component.
     * 
     * @param p_child The child to be added.
     */
    void AddChild(WPComponent p_child);

    /**
     * @brief Get the parent of this component.
     * 
     * @return WPComponent The parent of this component.
     */
    const WPComponent GetParent() const { return parent; }

    /**
     * @brief Get the children of this component.
     * 
     * @return const std::vector<Component*>& The children of this component.
     */
    const std::vector<WPComponent>& GetChildren() const { return children; }

    /**
     * @brief Get the subspace matrix of this component.
     * 
     * @return const Math::Mat4& The subspace matrix of this component.
     */
    const Math::Mat4& GetSubspaceMatrix() const;

    /**
     * @brief Get the inverse subspace matrix of this component.
     * 
     * @return Math::Mat4 The inverse subspace matrix of this component.
     */
    const Math::Mat4& GetSubspaceMatrixInverse() const;

    /**
     * @brief Get the model matrix of this component.
     * 
     * @return const Math::Mat4& The model matrix of this component.
     */
    const Math::Mat4& GetModelMatrix();

    /**
     * @brief Get the global position of this component.
     * 
     * @return const Math::Vec4& The global position of this component.
     */
    Math::Vec4 GetGlobalPosition() const;

    /**
     * @brief Set the position of this component.
     * 
     * @param p_position The new position of this component.
     */
    void SetGlobalPosition(const Math::Vec4& p_position);

    /**
     * @brief Get the context of this mesh.
     * 
     * @return Window* The context of this mesh.
     */
    Window* GetContext() { return context; }

    /**
     * @brief Get the context of this mesh.
     * 
     * @return const Window* The context of this mesh.
     */
    const Window* GetContext() const { return context; }

    /**
     * @brief Is the component visible.
     * 
     * @return true The component is visible.
     * @return false The component is not visible. 
     */
    FORCE_INLINE bool IsVisible() const { return visible; }

    /**
     * @brief Set the visibility of the component.
     * 
     * @param p_visible The visibility of the component.
     */
    FORCE_INLINE void SetVisible(bool p_visible) { visible = p_visible; }

    /**
     * @brief Draw the component.
     * The parent function must be called when overriding this function.
     */
    virtual void Draw();
};