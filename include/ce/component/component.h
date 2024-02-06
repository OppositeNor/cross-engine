#pragma once
#include "ce/math/math.hpp"
#include <vector>
#include <mutex>

namespace CrossEngine
{
    class Window;

    /**
     * @brief A Component is anything that can existed in the game.
     */
    class Component
        : public std::enable_shared_from_this<Component>
    {
    private:
        inline static Math::Mat4 identity = Math::Mat4();

        using WPComponent = std::weak_ptr<Component>;
        WPComponent parent;
        std::vector<WPComponent> children;

        bool visible = true;

        bool activated = false;
        mutable std::mutex activation_mutex;
    protected:
        
        virtual void SetSubspaceMatrixDirty();
        void SetChildrenSubspaceMatrixDirty();
        void SetChildrenSubspaceMatrixInverseDirty();

        void Activate();
        bool IsActivated() const;
    public:
        Component();
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
        virtual const Math::Mat4& GetSubspaceMatrix() const;

        /**
         * @brief Get the inverse subspace matrix of this component.
         * 
         * @return Math::Mat4 The inverse subspace matrix of this component.
         */
        virtual const Math::Mat4& GetSubspaceMatrixInverse() const;

        /**
         * @brief Get the model matrix of this component.
         * 
         * @return const Math::Mat4& The model matrix of this component.
         */
        const Math::Mat4& GetModelMatrix();

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
        virtual void Draw(Window* p_context);
    };
}