#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/managers/input_manager.h"
#include "ce/managers/event_manager.h"
#include "ce/event/window_event.h"
#include "ce/game/game.h"
#include "ce/materials/valued_material.h"
#include "ce/texture/static_texture.h"
#include "ce/component/camera.h"
#include "ce/component/skybox.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include "GLFW/glfw3native.h"
#endif

std::map<void*, Window*> Window::context_window_finder;

std::shared_ptr<Component> Window::GetBaseComponent()
{
    return Game::GetInstance()->GetBaseComponent();
}

Window::Window(const Math::Vec2s& p_size, const std::string& p_title)
    : window_title(p_title), window_size(p_size)
{
    window_thread = std::unique_ptr<std::thread>(new std::thread(&Window::ThreadFunc, this));
}

Window::Window(size_t p_width, size_t p_height, const std::string& p_title)
    : Window({p_width, p_height}, p_title)
{}

Window::Window(const Math::Vec2s& p_size, const std::string& p_title, bool p_fullscreen, bool p_resizable)
    : Window(p_size, p_title)
{
    is_fullscreen = p_fullscreen;
    is_resizable = p_resizable;
}
    

Window::Window(size_t p_width, size_t p_height, const std::string& p_title, bool p_fullscreen, bool p_resizable)
    : Window({p_width, p_height}, p_title)
{
    is_fullscreen = p_fullscreen;
    is_resizable = p_resizable;
}

Window::Window()
    : Window(640, 480, "")
{}

Window::~Window()
{
    Close();
}

void Window::SetWindowTitle(const std::string& p_title)
{
    window_title = p_title;
    glfwSetWindowTitle(static_cast<GLFWwindow*>(glfw_context), p_title.c_str());
}

void Window::UpdateThreadResource()
{
    for (size_t i = 0; i < queued_thread_resources.size(); ++i)
    {
        if (queued_thread_resources[i].is_queue_freed)
        {
            queued_thread_resources[i].destroy_func(1, &queued_thread_resources[i].id);
            queued_thread_resources.erase(queued_thread_resources.begin() + i);
            --i;
        }
    }
}

void Window::ClearResource()
{
    std::lock_guard<std::mutex> lock(queued_thread_resources_mutex);
    for (auto& resource : queued_thread_resources)
        resource.destroy_func(1, &resource.id);
    queued_thread_resources.clear();
}

void Window::RegisterThreadResource(unsigned int p_id, ReleaseFunction p_destroy_func) const
{
    std::lock_guard<std::mutex> lock(queued_thread_resources_mutex);
    queued_thread_resources.push_back(ThreadResource(p_id, p_destroy_func));
}

void Window::FreeThreadResource(unsigned int p_id) const
{
    std::lock_guard<std::mutex> lock(queued_thread_resources_mutex);
    for (auto& resource : queued_thread_resources)
    {
        if (resource.id == p_id)
        {
            resource.is_queue_freed = true;
            return;
        }
    }
}

void Window::SetClearColor(const Math::Vec4& p_clear_color)
{
    if (std::this_thread::get_id() != window_thread->get_id())
        throw std::runtime_error("Cannot set clear color from another thread.");
    clear_color = p_clear_color;
    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
}

void Window::HideAndLockCursor()
{
    if (std::this_thread::get_id() != window_thread->get_id())
        throw std::runtime_error("Cannot hide and lock cursor from another thread.");
    glfwSetInputMode((GLFWwindow*)glfw_context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::ShowAndUnlockCursor()
{
    if (std::this_thread::get_id() != window_thread->get_id())
        throw std::runtime_error("Cannot show and unlock cursor from another thread.");
    glfwSetInputMode((GLFWwindow*)glfw_context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::InitWindow()
{
    Graphics::InitGraphics();
    if (window_size[0] == 0)
        window_size = Graphics::GetScreenSize();
    glfw_context = Graphics::CreateGLFWContext(
        window_size[0], window_size[1], 
        window_title.c_str(), 
        is_fullscreen, is_resizable, nullptr);

    context_window_finder[glfw_context] = this;
    glfwMakeContextCurrent((GLFWwindow*)(glfw_context));
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glfwSetFramebufferSizeCallback((GLFWwindow*)(glfw_context), (GLFWframebuffersizefun)(WindowResized));
    glfwSetWindowFocusCallback((GLFWwindow*)(glfw_context), (GLFWwindowfocusfun)(WindowFocused));

#ifdef _WIN32
    hwnd = glfwGetWin32Window((GLFWwindow*)(glfw_context));
#endif
    glViewport(0, 0, window_size[0], window_size[1]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    SetClearColor(Math::Vec4(0.2f, 0.2f, 0.2f, 1.0f));

    glfwSetKeyCallback((GLFWwindow*)(glfw_context), (GLFWkeyfun)(OnKey));
    glfwSetCursorPosCallback((GLFWwindow*)(glfw_context), (GLFWcursorposfun)(OnMouseMove));
    glfwSetMouseButtonCallback((GLFWwindow*)(glfw_context), (GLFWmousebuttonfun)(OnMouseButton));
    
    shader_program = new ShaderProgram(Resource::GetExeDirectory() + "/shaders/vertex.glsl", 
                                       Resource::GetExeDirectory() + "/shaders/fragment.glsl");
    shader_program->Compile();
    skybox_shader_program = new ShaderProgram(Resource::GetExeDirectory() + "/shaders/skybox_vertex.glsl", 
                                              Resource::GetExeDirectory() + "/shaders/skybox_fragment.glsl");
    skybox_shader_program->Compile();

    float aspect_ratio = (float)window_size[0] / (float)window_size[1];
    proj_matrix = Math::ProjPersp(
        0.2f * aspect_ratio, -0.2f * aspect_ratio, 0.2f, -0.2f, 0.5f, 1000.0f);

    skybox = new Skybox({Resource::GetExeDirectory() + "/textures/skybox/default/right.jpg",
                        Resource::GetExeDirectory() + "/textures/skybox/default/left.jpg",
                        Resource::GetExeDirectory() + "/textures/skybox/default/top.jpg",
                        Resource::GetExeDirectory() + "/textures/skybox/default/bottom.jpg",
                        Resource::GetExeDirectory() + "/textures/skybox/default/front.jpg",
                        Resource::GetExeDirectory() + "/textures/skybox/default/back.jpg"});
    
}

void Window::UpdateWindowSize(const Math::Vec2s& p_new_window_size)
{
    glViewport(0, 0, p_new_window_size[0], p_new_window_size[1]);
    
    window_size = p_new_window_size;
}

void Window::ThreadFunc()
{
    try {
        InitWindow();
        Game::GetInstance()->SetContextAvailable(this);
        Ready();
        float frame_start;
        float delta = 0.01;
        while (!glfwWindowShouldClose((GLFWwindow*)glfw_context) && !should_close)
        {
            frame_start = glfwGetTime();
            glfwSwapBuffers((GLFWwindow*)glfw_context);
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Process(delta);
            Draw();
            shader_program->Refresh();
            auto error = glGetError();
            if (error != GL_NO_ERROR)
                throw std::runtime_error("OpenGL error: " + std::to_string(error));
            Game::GetInstance()->UpdateInput(this);
            UpdateThreadResource();
            point_light_count = 0;
            parallel_light_count = 0;
            delta = glfwGetTime() - frame_start;
        }
        Game::GetInstance()->SetContextUnAvailable(this);
        OnClose();
        Game::GetInstance()->DispatchEvent(std::make_shared<OnWindowCloseEvent>(this));
        delete shader_program;
        delete skybox_shader_program;
        ClearResource();
        Graphics::DestroyGLFWContex(glfw_context);
        is_closed = true;
    }
    catch (const std::exception& e){
        std::cout << "Application throwed an error: " << e.what() << std::endl;
        throw std::runtime_error("Application throwed an error: " + std::string(e.what()));
    }
}

void Window::WindowResized(void* p_glfw_context, int p_width, int p_height)
{
    Window* window = context_window_finder[p_glfw_context];
    window->UpdateWindowSize({(size_t)p_width, (size_t)p_height});
}

void Window::WindowFocused(void* p_glfw_context, int p_focused)
{
    Window* window = context_window_finder[p_glfw_context];
    Game::GetInstance()->DispatchEvent(std::make_shared<OnWindowFocusEvent>(window, p_focused));
}

void Window::OnKey(void* p_glfw_context, int p_key, int p_scancode, int p_action, int p_mods)
{
    Window* window = context_window_finder[p_glfw_context];
    Game::GetInstance()->DispatchEvent(std::make_shared<OnKeyEvent>(window, p_key, p_scancode, p_action, p_mods));
}

void Window::OnMouseButton(void* p_glfw_context, int p_key, int p_action, int p_mods)
{
    Window* window = context_window_finder[p_glfw_context];
    Game::GetInstance()->DispatchEvent(std::make_shared<OnKeyEvent>(window, p_key, 0, p_action, p_mods));
}

void Window::OnMouseMove(void* p_glfw_context, double p_x, double p_y)
{
    Window* window = context_window_finder[p_glfw_context];
    Game::GetInstance()->DispatchEvent(std::make_shared<OnMouseMoveEvent>(window, Math::Vector<double, 2>(p_x, p_y)));
}

void Window::Draw()
{
    if (skybox != nullptr)
    {
        skybox_shader_program->Use();
        skybox_shader_program->SetUniform("proj", proj_matrix);
        if (using_camera == nullptr)
            skybox_shader_program->SetUniform("view", Math::Mat4());
        else
            skybox_shader_program->SetUniform("view", using_camera->GetViewMatrix());
        skybox->Draw(this);
    }
    shader_program->Use();
    shader_program->SetUniform("proj", proj_matrix);
    if (using_camera == nullptr)
    {
        shader_program->SetUniform("view", Math::Mat4());
        shader_program->SetUniform("camera_position", Math::Vec4());
    }
    else
    {
        shader_program->SetUniform("view", using_camera->GetViewMatrix());
        shader_program->SetUniform("camera_position", using_camera->GetGlobalPosition());
    }
    if (skybox != nullptr)
        shader_program->SetSamplerCubeUniform("skybox", skybox->GetTextureCubeIDs()[this]);
    Game::GetInstance()->GetBaseComponent()->Draw(this);
    GetShaderProgram()->SetUniform("point_light_count", GetPointLightCount());
    GetShaderProgram()->SetUniform("parallel_light_count", GetParallelLightCount());
}