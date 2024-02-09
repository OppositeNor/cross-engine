#include "ce/component/component.h"
#include "ce/graphics/window.h"


namespace CrossEngine
{
    Component::Component(const std::string& p_component_name)
        : component_name(p_component_name)
    {
        
    }

    Component::Component(const Component& p_other)
    {
        SetSubspaceMatrixDirty();
        component_name = p_other.component_name;
        if (!p_other.parent.expired())
            p_other.parent.lock()->AddChild(shared_from_this());
        
    }

    Component::Component(Component&& p_other) noexcept
    {
        SetSubspaceMatrixDirty();
        component_name = p_other.component_name;
        if (!p_other.parent.expired())
            p_other.parent.lock()->AddChild(shared_from_this());
        p_other.parent.reset();
        children = std::move(p_other.children);
    }

    Component::~Component()
    {

    }

    void Component::Update(float p_delta)
    {
        Activate();
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
        for (auto& i : children)
        {
            if (i.lock().get() == p_child.lock().get())
                return;
        }
        if (activated)
        {
            p_child.lock()->Activate();
        }
        children.push_back(p_child);
        auto child = p_child.lock();
        if (!child->parent.expired())
            child->parent.lock()->RemoveChild(child.get());
        child->SetSubspaceMatrixDirty();
        child->parent = shared_from_this();
    }

    std::shared_ptr<Component> Component::GetChild(const std::string& p_child_name)
    {
        for (auto& i : children)
        {
            auto shared = i.lock();
            if (shared->GetName() == p_child_name)
                return shared;
        }
        return nullptr;
    }

    const Math::Mat4& Component::GetSubspaceMatrix() const
    {
        return identity;
    }

    const Math::Mat4& Component::GetSubspaceMatrixInverse() const
    {
        return identity;
    }

    void Component::SetSubspaceMatrixDirty()
    {
        SetChildrenSubspaceMatrixDirty();
        SetChildrenSubspaceMatrixInverseDirty();
    }

    void Component::SetChildrenSubspaceMatrixDirty()
    {
        for (auto& child : children)
        {
            child.lock()->SetSubspaceMatrixDirty();
        }
    }

    void Component::Activate()
    {
        if (!activated)
        {
            std::lock_guard<std::mutex> lock(activation_mutex);
            if (!activated)
            {
                activated = true;
                Ready();
            }
        }
    }
    bool Component::IsActivated() const
    {
        std::lock_guard<std::mutex> lock(activation_mutex);
        return activated;
    } 

    void Component::SetChildrenSubspaceMatrixInverseDirty()
    {
        for (auto& child : children)
        {
            child.lock()->SetSubspaceMatrixDirty();
        }
    }

    const Math::Mat4& Component::GetModelMatrix()
    {
        return GetSubspaceMatrix();
    }

    void Component::Draw(Window* p_context)
    {
        if (!visible)
            return;
        for (auto& child : children)
        {
            if (child.expired())
                continue;
            child.lock()->Draw(p_context);
        }
    }
}