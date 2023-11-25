#include "ce/graphics/graphics.h"
#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ce/defs.hpp"

void Graphics::InitGraphics()
{
    if (!initialized)
    {
        std::lock_guard<std::mutex> lock(init_mutex);
        if (!initialized)
        {
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW.");
            
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            initialized = true;
        }
    }
}

void* Graphics::CreateGLFWContext(size_t p_width, size_t p_height, const std::string& p_title, void* p_shared)
{
    if (!initialized)
        throw std::runtime_error("Graphics not initialized.");
    GLFWwindow* result = glfwCreateWindow(p_width, p_height, p_title.c_str(), NULL, (GLFWwindow*)p_shared);
    if (!result)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    return reinterpret_cast<void*>(result);
}

void Graphics::DestroyGLFWContex(void* p_context)
{
    glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(p_context));
    glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(p_context));
}

unsigned int Graphics::GenerateVBO(unsigned int p_vao, float* p_vertices, size_t p_size)
{
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(p_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, p_size, p_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return vbo;
}

void Graphics::TerminateGraphics()
{
    if (initialized)
    {
        std::lock_guard<std::mutex> lock(init_mutex);
        if (initialized)
        {
            glfwTerminate();
            initialized = false;
        }
    }
}