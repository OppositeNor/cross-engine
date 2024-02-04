#include "ce/managers/input_manager.h"
#include "ce/graphics/window.h"
#include "ce/event/window_event.h"

namespace CrossEngine
{
    InputManager::InputManager()
    {
    }


    void InputManager::SetInputStatePressed(Window* p_context, Input p_key)
    {
        std::unique_lock<std::shared_mutex> lock(key_state_mutex);
        auto key = input_map.find(p_key);
        if (key == input_map.end())
            return;
        auto key_list = key->second;
        for (auto iter = key_list.begin(); iter != key_list.end(); ++iter)
        {
            context_map[p_context][*iter] = InputState::JustPressed;
        }
    }

    void InputManager::SetInputStateReleased(Window* p_context, Input p_key)
    {
        std::unique_lock lock(key_state_mutex);
        auto key = input_map.find(p_key);
        if (key == input_map.end())
            return;
        auto key_list = key->second;
        for (auto iter = key_list.begin(); iter != key_list.end(); ++iter)
        {
            context_map[p_context][*iter] = InputState::JustReleased;
        }
    }

    InputManager::InputState InputManager::GetInputState(const Window* p_context, const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        if (!context_map.contains(p_context) ||
            !context_map.at(p_context).contains(p_key))
            return InputState::Released;
        return context_map.at(p_context).at(p_key);
    }

    InputManager::InputState InputManager::GetInputState(const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        for (auto& i : context_map)
        {
            if (i.second.contains(p_key))
            {
                auto key_state = i.second.at(p_key);
                if (key_state != InputState::Released)
                    return key_state;
            }
        }
        return InputState::Released;
    }

    void InputManager::AddInput(const std::string& input_name, Input p_key)
    {
        std::unique_lock lock(key_state_mutex);
        input_map[p_key].push_back(input_name);
    }

    void InputManager::ClearInput()
    {
        std::unique_lock lock(key_state_mutex);
        input_map.clear();
    }

    void InputManager::UpdateInput(const Window* p_context)
    {
        std::unique_lock lock(key_state_mutex);
        for (auto iter = context_map[p_context].begin(); iter != context_map[p_context].end(); ++iter)
        {
            if (iter->second == InputState::JustPressed)
                iter->second = InputState::Pressed;
            else if (iter->second == InputState::JustReleased)
                iter->second = InputState::Released;
        }
    }

    bool InputManager::IsInputPressed(const Window* p_context, const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        return context_map.at(p_context).at(p_key) & InputState::Pressed;
    }

    bool InputManager::IsInputReleased(const Window* p_context, const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        return context_map.at(p_context).at(p_key) & InputState::Released;
    }

    bool InputManager::IsInputJustPressed(const Window* p_context, const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        return context_map.at(p_context).at(p_key) == InputState::JustPressed;
    }

    bool InputManager::IsInputJustReleased(const Window* p_context, const std::string& p_key) const
    {
        std::shared_lock lock(key_state_mutex);
        return context_map.at(p_context).at(p_key) == InputState::JustReleased;
    }

    void InputManager::OnEvent(std::shared_ptr<AEvent> p_event)
    {
        if (p_event->GetEventType() != EventType::OnKey)
            return;
        auto event = std::static_pointer_cast<OnKeyEvent>(p_event);
        switch ((InputAction)event->action)
        {
        case InputAction::RELEASE:
            SetInputStateReleased(event->window, (Input)(event->key));
            break;
        case InputAction::PRESS:
            SetInputStatePressed(event->window, (Input)(event->key));
            break;
        default:
            break;
        }
    }
}