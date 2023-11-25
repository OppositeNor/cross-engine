#pragma once
#include <mutex>
#include "ce/defs.hpp"

class Window;
class Triangle;
class Graphics
{
    Graphics() = delete;
    inline static bool initialized = false;
    inline static std::mutex init_mutex;
public:
    /**
     * @brief Initialize graphics.
     * @throw std::runtime_error Failed to initialize GLFW.
     */
    static void InitGraphics();
    /**
     * @brief Terminate graphics.
     */
    static void TerminateGraphics();
    /**
     * @brief Create glfw window context.
     * 
     * @param p_width The width of the window.
     * @param p_height The height of the window.
     * @param p_title The title of the window.
     * @param p_shared The context that this context share resource with.
     * 
     * @throw std::runtime_error Failed to create GLFW window.
     * @throw std::runtime_error Graphics not initialized.
     * @return void* The created window context.
     */
    static void* CreateGLFWContext(size_t p_width, size_t p_height, const std::string& p_title, void* p_shared);

    /**
     * @brief Destroy the window context.
     * 
     * @param p_context The context to be destroyed.
     */
    static void DestroyGLFWContex(void* p_context);

    /**
     * @brief Generate VBO from vertices.
     * 
     * @param p_vao The VAO to generate VBO from.
     * @param p_vertices The vertices to generate VBO from.
     * @param p_size The size of the vertices.
     * @return unsigned int The ID of the VBO.
     */
    static unsigned int GenerateVBO(unsigned int p_vao, float* p_vertices, size_t p_size);
};