#pragma once
#include <thread>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif
#include <vector>
#include <map>
#include "ce/math/math.hpp"
#include "ce/graphics/shader/shader_program.h"
#include "ce/event/i_event_listener.h"
class InputManager;
class ATexture;
class Component;
class Camera;
class Window : public IEventListener
{
protected:
    void* glfw_context = nullptr;
    static std::map<void*, Window*> context_window_finder;

    Vec2s window_size;
    std::string window_title = "";

    std::shared_ptr<ATexture> default_texture;
#ifdef _WIN32
    HWND hwnd = nullptr;
#endif
    std::unique_ptr<std::thread> window_thread;
    bool is_closed = false;
    bool should_close = false;
    ShaderProgram* shader_program = nullptr;
    unsigned int vao = 0;
    
    Vec4 clear_color = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

    void ThreadFunc();
    static void WindowResized(void* p_glfw_context, int p_width, int p_height);
    static void WindowFocused(void* p_glfw_context, int p_focused);
    void InitWindow();
    void UpdateWindowSize(const Vec2s& p_new_window_size);

    std::shared_ptr<Component> base_component;
    std::shared_ptr<Camera> using_camera;

    Mat4 proj_matrix;

private:
    static void OnKey(void* p_glfw_context, int p_key, int p_scancode, int p_action, int p_mods);

public:

    /**
     * @brief Get the GLFW context.
     * 
     * @return void* The GLFW context.
     */
    FORCE_INLINE void* GetGLFWContext() noexcept { return glfw_context; }

    /**
     * @brief Get the GLFW context.
     * 
     * @return void* The GLFW context.
     */
    FORCE_INLINE const void* GetGLFWContext() const noexcept { return glfw_context; }


    /**
     * @brief Get the base component.
     * 
     * @return Component* The base component.
     */
    FORCE_INLINE std::shared_ptr<Component> GetBaseComponent() { return base_component; }

    /**
     * @brief Get the base component.
     *
     * @return const Component* The base component.
     */
    FORCE_INLINE const std::shared_ptr<Component> GetBaseComponent() const { return base_component; }

    /**
     * @brief Get the camera that is being used.
     * 
     * @return std::shared_ptr<Camera> The camera that is being used.
     */
    FORCE_INLINE std::shared_ptr<Camera> GetUsingCamera() { return using_camera; }

    /**
     * @brief Get the camera that is being used.
     * 
     * @return const std::shared_ptr<Camera>& The camera that is being used.
     */
    FORCE_INLINE const std::shared_ptr<Camera>& GetUsingCamera() const { return using_camera; }

    /**
     * @brief Set the camera that is being used.
     * 
     * @param p_camera The camera that is being used.
     */
    FORCE_INLINE void SetUsingCamera(std::shared_ptr<Camera> p_camera) { using_camera = p_camera; }

    /**
     * @brief Get the projection matrix.
     * 
     * @return const Mat4& The projection matrix.
     */
    FORCE_INLINE const Mat4& GetProjMatrix() const { return proj_matrix; }

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

    FORCE_INLINE const std::shared_ptr<ATexture>& GetDefaultTexture() const noexcept { return default_texture; }

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

#ifdef _WIN32
    /**
     * @brief Get the window handle.
     * @return HWND The window handle.
     */
    FORCE_INLINE HWND GetWindowHWND() { return hwnd; }
#endif

    /**
     * @brief Get the window's clear color.
     * 
     * @return Vec4 The clear color.
     */
    FORCE_INLINE Vec4 GetClearColor() const noexcept { return clear_color; }

    /**
     * @brief Set the window's clear color.
     * 
     * @param p_clear_color The clear color.
     */
    void SetClearColor(const Vec4& p_clear_color);

    /**
     * @brief Get the size of the window.
     * 
     * @return const Vec2s& The size of the window.
     */
    const Vec2s& GetWindowSize() const noexcept { return window_size; }

    /**
     * @brief Is the window closed.
     * 
     * @return true if the window is closed
     * @return false if the window is not closed 
     */
    FORCE_INLINE bool IsClosed() const { return is_closed; }

    /**
     * @brief Should the window be closed.
     * 
     * @detials When the window should close, it is not closed yet. It hasn't finish the closing process.
     * 
     * @return true if the window should be closed
     * @return false if the window should not be closed
     */
    FORCE_INLINE bool ShouldClose() const { return should_close; }

    /**
     * @brief Close the window.
     * 
     */
    FORCE_INLINE void Close() { 
        should_close = true; Join(); }

    /**
     * @brief Join the window thread.
     * 
     */
    FORCE_INLINE void Join() 
    { if (window_thread->joinable())
        window_thread->join(); }

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
    virtual void Draw();

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

    /**
     * @brief Called when an event is dispatched.
     * 
     * @param p_event The event that was dispatched.
     */
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override {}
};