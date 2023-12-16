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
            glfwWindowHint(GLFW_SAMPLES, 4);

            initialized = true;
        }
        
    }
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

void Graphics::ClearGraphicsResourceQueue()
{
    std::lock_guard<std::mutex> lock(queue_freed_graphic_resources_mutex);
    for (auto& resource : queue_freed_graphic_resources)
    {
        if (glfwGetCurrentContext() != resource.context)
            glfwMakeContextCurrent((GLFWwindow*)resource.context);
        resource.destroy_func(1, &resource.id);
    }
    queue_freed_graphic_resources.clear();
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
    glfwDestroyWindow(static_cast<GLFWwindow*>(p_context));
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

unsigned int Graphics::GenerateTexture()
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    return texture_id;
}

void Graphics::DeleteTexture(unsigned int p_texture_id, const Window* p_context)
{
    RegisterQueueFree((void*)p_context->GetGLFWContext(), p_texture_id, glDeleteTextures);
}

void Graphics::ConfigTexture(unsigned int p_texture_id, const TextureConfig& p_config)
{
    glBindTexture(GL_TEXTURE_2D, p_texture_id);
    switch (p_config.repeat_mode_h)
    {
    case TextureRepeatMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        break;
    case TextureRepeatMode::MIRRORED_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        break;
    case TextureRepeatMode::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case TextureRepeatMode::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        break;
    }
    switch (p_config.repeat_mode_v)
    {
    case TextureRepeatMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case TextureRepeatMode::MIRRORED_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    case TextureRepeatMode::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case TextureRepeatMode::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        break;
    }
    if (p_config.mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        switch (p_config.filter_mode_min)
        {
        case TextureFilterMode::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case TextureFilterMode::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        }
    
    }
    switch (p_config.filter_mode_mag)
    {
    case TextureFilterMode::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case TextureFilterMode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::SetTexture(unsigned int p_texture_id, size_t p_width, size_t p_height, size_t p_channels, const ubyte_t* p_data, bool p_mipmap)
{
    glBindTexture(GL_TEXTURE_2D, p_texture_id);
    switch (p_channels)
    {
    case 1:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, p_width, p_height, 0, GL_RED, GL_UNSIGNED_BYTE, p_data);
        break;
    case 3:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, p_data);
        break;
    case 4:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);
        break;
    default:
        throw std::runtime_error("Unsupported number of channels.");
    }
    if (p_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::RegisterQueueFree(const void* p_context, unsigned int p_id, std::function<void(unsigned int, unsigned int*)> p_destroy_func)
{
    std::lock_guard<std::mutex> lock(queue_freed_graphic_resources_mutex);
    queue_freed_graphic_resources.push_back(GraphicsResource(p_context, p_id, p_destroy_func));
}

void Graphics::Update()
{
    if (initialized)
        ClearGraphicsResourceQueue();
}