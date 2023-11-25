#pragma once
#include <thread>
#include <memory>
#include <windows.h>
#include <vector>
#include <map>
#include "ce/math/math.hpp"
#include "ce/event_listener/window_event_listener.h"
#include "ce/graphics/shader/shader_program.h"

class Window
{
protected:
    void* glfw_context = nullptr;
    static std::map<void*, Window*> context_window_finder;

    Vec2s window_size;
    std::string window_title = "";
    HWND hwnd = nullptr;
    std::unique_ptr<std::thread> window_thread;
    std::vector<IWindowEventListener*> window_event_listeners;
    bool is_closed = false;
    ShaderProgram* shader_program = nullptr;
    unsigned int vao = 0;
    

    void ThreadFunc();
    static void WindowResized(void* p_glfw_context, int p_width, int p_height);
    static void WindowFocused(void* p_glfw_context, int p_focused);
    void InitWindow();
    void UpdateWindowSize(const Vec2s& p_new_window_size);
public:

    /**
     * @brief Constructor for window.
     * 
     * @param p_size The size of the window.
     * @param p_title The title of the window.
     */
    Window(const Vec2s& p_size, const std::string& p_title);

    /**
     * @brief Constructor for window.
     * 
     * @param p_width The width of the window.
     * @param p_height The height of the window.
     * @param p_title The title of the window.
     */
    Window(size_t p_width, size_t p_height, const std::string& p_title);

    /**
     * @brief Constructor for window.
     * @details This constructor is used for creating a window that is 640x480 with no title.
     */
    Window();

    /**
     * @brief Destroy the Window object
     */
    virtual ~Window();

    /**
     * @brief Get the id of the window thread.
     * 
     * @return std::thread::id The id of the window thread. 
     */
    FORCE_INLINE std::thread::id GetThreadId() const noexcept { return window_thread->get_id(); }

    /**
     * @brief Get the window handle.
     * @return HWND The window handle.
     */
    FORCE_INLINE HWND GetWindowHWND() { return hwnd; }

    /**
     * @brief Add a window event listener.
     * 
     * @param listener The listener to add.
     */
    FORCE_INLINE void AddWindowEventListener(IWindowEventListener* listener) 
        { window_event_listeners.push_back(listener); }
    
    /**
     * @brief Remove a window listener.
     * 
     * @param listener The listener to be removed.
     */
    FORCE_INLINE void RemoveWindowEventListener(IWindowEventListener* listener)
    {
        for (auto it = window_event_listeners.begin(); it != window_event_listeners.end(); ++it)
        {
            if (*it == listener)
                window_event_listeners.erase(it);
        }
    }

    /**
     * @brief Get the size of the window.
     * 
     * @return const Vec2s& The size of the window.
     */
    const Vec2s& GetWindowSize() const noexcept { return window_size; }

    /**
     * @brief Is the window closed.
     * 
     * @return bool True if the window is closed, false otherwise. 
     */
    FORCE_INLINE bool IsClosed() const { return is_closed; }

    /**
     * @brief Called once when the window runs.
     * 
     */
    virtual void Ready() {}

    /**
     * @brief Callend once when the window is closed.
     */
    virtual void OnClose() {}

    /**
     * @brief Called every frame when the window runs.
     * 
     * @param p_delta The difference of time between two frames.
     */
    virtual void Process(float p_delta) {}

    /**
     * @brief Draw the window. This is called after the process function.
     */
    virtual void Draw() {}

    /**
     * @brief Get the shader program.
     * 
     * @return ShaderProgram* The shader program.
     */
    FORCE_INLINE ShaderProgram* GetShaderProgram() { return shader_program; }

    /**
     * @brief Get the shader program.
     * 
     * @return const ShaderProgram* The shader program.
     */
    FORCE_INLINE const ShaderProgram* GetShaderProgram() const { return shader_program; }
};