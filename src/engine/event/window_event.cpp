#include "ce/event/window_event.h"


namespace CrossEngine
{
    OnWindowCloseEvent::OnWindowCloseEvent(Window* p_window) 
        : AEvent(EventType::OnWindowClose), window(p_window)
    {
    }

    OnWindowFocusEvent::OnWindowFocusEvent(Window* p_window, bool p_is_focused)
        : AEvent(EventType::OnWindowFocus), window(p_window), is_focused(p_is_focused)
    {
    }

    OnWindowResizeEvent::OnWindowResizeEvent(Window* p_window, const Math::Vec2s& p_original, const Math::Vec2s& p_new_size)
        : AEvent(EventType::OnWindowResize), window(p_window), original(p_original), new_size(p_new_size)
    {
    }

    OnWindowMove::OnWindowMove(Window* p_window, const Math::Vec2s& p_original, const Math::Vec2s& p_new_position)
        : AEvent(EventType::OnWindowMove), window(p_window), original(p_original), new_position(p_new_position)
    {
    }

    OnKeyEvent::OnKeyEvent(Window* p_window, int p_key, int p_scancode, int p_action, int p_mods)
        : AEvent(EventType::OnKey), window(p_window), key(p_key), scancode(p_scancode), action(p_action), mods(p_mods)
    {
    }

    OnMouseMoveEvent::OnMouseMoveEvent(Window* p_window, const Math::Vector<double, 2>& p_pos)
        : AEvent(EventType::OnMouseMove), window(p_window), pos(p_pos)
    {
    }
}