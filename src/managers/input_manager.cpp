#include "ce/managers/input_manager.h"
#include "ce/graphics/window.h"

InputManager::InputManager()
{
}


void InputManager::SetInputStatePressed(Window* p_context, int p_key)
{
    
    auto key = input_map.find(p_key);
    if (key == input_map.end())
        return;
    auto key_list = key->second;
    for (auto iter = key_list.begin(); iter != key_list.end(); ++iter)
    {
        context_map[p_context][*iter] = InputState::JustPressed;
    }
}

void InputManager::SetInputStateReleased(Window* p_context, int p_key)
{
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
    if (!context_map.contains(p_context) ||
        !context_map.at(p_context).contains(p_key))
        return InputState::Released;
    return context_map.at(p_context).at(p_key);
}

void InputManager::AddInput(const std::string& input_name, int p_key)
{
    input_map[p_key].push_back(input_name);
}

void InputManager::ClearInput()
{
    input_map.clear();
}

void InputManager::UpdateInput(const Window* p_context)
{
    for (auto iter = context_map[p_context].begin(); iter != context_map[p_context].end(); ++iter)
    {
        if (iter->second == InputState::JustPressed)
            iter->second = InputState::Pressed;
        else if (iter->second == InputState::JustReleased)
            iter->second = InputState::Released;
    }
}

bool InputManager::IsInputPressed(Window* p_context, const std::string& p_key) const
{
    return context_map.at(p_context).at(p_key) & InputState::Pressed;
}

bool InputManager::IsInputReleased(Window* p_context, const std::string& p_key) const
{
    return context_map.at(p_context).at(p_key) & InputState::Released;
}

bool InputManager::IsInputJustPressed(Window* p_context, const std::string& p_key) const
{
    return context_map.at(p_context).at(p_key) == InputState::JustPressed;
}

bool InputManager::IsInputJustReleased(Window* p_context, const std::string& p_key) const
{
    return context_map.at(p_context).at(p_key) == InputState::JustReleased;
}

void InputManager::OnKey(Window* p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
    switch (p_action)
    {
    case RELEASE:
        SetInputStateReleased(p_window, p_key);
        break;
    case PRESS:
        SetInputStatePressed(p_window, p_key);
        break;
    default:
        break;
    }
}