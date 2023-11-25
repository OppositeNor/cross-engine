#include "ce/math/math.hpp"

class Window;
class IWindowEventListener
{
public:
    /**
     * @brief Called when the window is closed.
     * 
     * @param p_window The window that was closed.
     */
    virtual void OnWindowClose(Window* p_window) = 0;
    /**
     * @brief Called when the window focused is changed.
     * 
     * @param p_window The window that the focused is changed.
     */
    virtual void OnWindowFocus(Window* window, bool is_focused) = 0;
    /**
     * @brief Called when the window is resized.
     * 
     * @param p_window The window that was resized.
     * @param p_original The original size of the window.
     * @param p_new The new size of the window.
     */
    virtual void OnWindowResize(Window* p_window, const Vec2s& p_original, const Vec2s& p_new) = 0;
    /**
     * @brief Called when the window is moved.
     * 
     * @param p_window The window that is moved.
     * @param p_original The original position of the window.
     * @param p_new The new position of the window.
     */
    virtual void OnWindowMove(Window* p_window, const Vec2s& p_original, const Vec2s& p_new) = 0;
};