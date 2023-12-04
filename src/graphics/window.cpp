#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/managers/input_manager.h"
#include "ce/managers/event_manager.h"
#include "ce/event/window_event.h"
#include "ce/game/game.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

std::map<void*, Window*> Window::context_window_finder;

Window::Window(const Vec2s& p_size, const std::string& p_title)
    : window_title(p_title), window_size(p_size)
{
    window_thread = std::make_unique<std::thread>(&Window::ThreadFunc, this);
}

Window::Window(size_t p_width, size_t p_height, const std::string& p_title)
    : Window({p_width, p_height}, p_title)
{}

Window::Window()
    : Window(640, 480, "")
{}

Window::~Window()
{
    is_closed = true;
    if (window_thread->joinable())
        window_thread->join();
}

void Window::InitWindow()
{
    Graphics::InitGraphics();
    glfw_context = Graphics::CreateGLFWContext(window_size[0], window_size[1], window_title.c_str(), nullptr);
    context_window_finder[glfw_context] = this;
    glfwMakeContextCurrent((GLFWwindow*)(glfw_context));
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glfwSetFramebufferSizeCallback((GLFWwindow*)(glfw_context), (GLFWframebuffersizefun)(WindowResized));
    glfwSetWindowFocusCallback((GLFWwindow*)(glfw_context), (GLFWwindowfocusfun)(WindowFocused));
    
    hwnd = glfwGetWin32Window((GLFWwindow*)(glfw_context));
    glViewport(0, 0, window_size[0], window_size[1]);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glfwSetKeyCallback((GLFWwindow*)(glfw_context), (GLFWkeyfun)(OnKey));
    
    shader_program = new ShaderProgram(Resource::GetExeDirectory() + "/shaders/vertex.glsl", 
                                       Resource::GetExeDirectory() + "/shaders/fragment.glsl");
    shader_program->Compile();
}

void Window::UpdateWindowSize(const Vec2s& p_new_window_size)
{
    glViewport(0, 0, p_new_window_size[0], p_new_window_size[1]);
    
    window_size = p_new_window_size;
}

void Window::ThreadFunc()
{
    try {
        InitWindow();
        Ready();
        float frame_start;
        float delta = 0.01;
        while (!glfwWindowShouldClose((GLFWwindow*)glfw_context) && !is_closed)
        {
            frame_start = glfwGetTime();
            glfwSwapBuffers((GLFWwindow*)glfw_context);
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Process(delta);
            Draw();
            Game::GetInstance()->UpdateInput(this);
            delta = glfwGetTime() - frame_start;
        }
        is_closed = true;
        OnClose();
        Game::GetInstance()->DispatchEvent(std::make_shared<OnWindowCloseEvent>(this));
        Graphics::DestroyGLFWContex(glfw_context);
        delete shader_program;
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