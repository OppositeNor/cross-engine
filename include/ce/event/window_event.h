#pragma once

#include "ce/event/event.h"
#include "ce/math/math.hpp"

class Window;
struct OnWindowCloseEvent : public AEvent
{
    Window* window;

    OnWindowCloseEvent(Window* p_window);
    virtual ~OnWindowCloseEvent() override {}
};

struct OnWindowFocusEvent : public AEvent
{
    Window* window;
    bool is_focused;

    OnWindowFocusEvent(Window* p_window, bool p_is_focused);
    virtual ~OnWindowFocusEvent() override {}
};

struct OnWindowResizeEvent : public AEvent
{
    Window* window;
    Math::Vec2s original;
    Math::Vec2s new_size;

    OnWindowResizeEvent(Window* p_window, const Math::Vec2s& p_original, const Math::Vec2s& p_new_size);
    virtual ~OnWindowResizeEvent() override {}
};

struct OnWindowMove : public AEvent
{
    Window* window;
    Math::Vec2s original;
    Math::Vec2s new_position;

    OnWindowMove(Window* p_window, const Math::Vec2s& p_original, const Math::Vec2s& p_new_position);
    virtual ~OnWindowMove() override {}
};

struct OnKeyEvent : public AEvent
{
    Window* window;
    int key;
    int scancode;
    int action;
    int mods;

    OnKeyEvent(Window* p_window, int p_key, int p_scancode, int p_action, int p_mods);
    virtual ~OnKeyEvent() override {}
};

struct OnMouseMoveEvent : public AEvent
{
    Window* window;
    Math::Vector<double, 2> pos;

    OnMouseMoveEvent(Window* p_window, const Math::Vector<double, 2>& p_pos);
    virtual ~OnMouseMoveEvent() override {}
};