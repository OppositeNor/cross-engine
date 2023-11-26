#include "ce/handlers/input_handler.h"
#include "ce/graphics/window.h"

InputHandler::InputHandler(Window* p_context)
    : context(p_context)
{
    context->AddWindowEventListener(this);
}


void InputHandler::SetInputStatePressed(int p_key)
{
    auto key = input_map.find(p_key);
    if (key == input_map.end())
        return;
    auto key_list = key->second;
    for (auto iter = key_list.begin(); iter != key_list.end(); ++iter)
    {
        input_state_map[*iter] = InputState::JustPressed;
    }
}

void InputHandler::SetInputStateReleased(int p_key)
{
    auto key = input_map.find(p_key);
    if (key == input_map.end())
        return;
    auto key_list = key->second;
    for (auto iter = key_list.begin(); iter != key_list.end(); ++iter)
    {
        input_state_map[*iter] = InputState::JustReleased;
    }
}

InputHandler::InputState InputHandler::GetInputState(const std::string& p_key) const
{
    return input_state_map.at(p_key);
}

void InputHandler::AddInput(const std::string& input_name, int p_key)
{
    input_map[p_key].push_back(input_name);
    input_state_map[input_name] = InputState::Released;
}

void InputHandler::ClearInput()
{
    input_map.clear();
    input_state_map.clear();
}

void InputHandler::UpdateInput()
{
    for (auto iter = input_state_map.begin(); iter != input_state_map.end(); ++iter)
    {
        if (iter->second == InputState::JustPressed)
            iter->second = InputState::Pressed;
        else if (iter->second == InputState::JustReleased)
            iter->second = InputState::Released;
    }
}

bool InputHandler::IsInputPressed(const std::string& p_key) const
{
    return input_state_map.at(p_key) & InputState::Pressed;
}

bool InputHandler::IsInputReleased(const std::string& p_key) const
{
    return input_state_map.at(p_key) & InputState::Released;
}

bool InputHandler::IsInputJustPressed(const std::string& p_key) const
{
    return input_state_map.at(p_key) == InputState::JustPressed;
}

bool InputHandler::IsInputJustReleased(const std::string& p_key) const
{
    return input_state_map.at(p_key) == InputState::JustReleased;
}

void InputHandler::OnKey(Window* p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
    switch (p_action)
    {
    case RELEASE:
        SetInputStateReleased(p_key);
        break;
    case PRESS:
        SetInputStatePressed(p_key);
        break;
    default:
        break;
    }
}