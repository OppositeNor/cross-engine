#pragma once
#include <vector>
#include <memory>
#include "ce/utils/task.h"

namespace CrossEngine
{
    class ShaderProgram;
    class Renderer
    {
    private:
        std::vector<Task> render_tasks;
        std::vector<Task> unprioritized_render_tasks;
        std::unique_ptr<ShaderProgram> shader_program;
    public:
        Renderer(ShaderProgram*&& p_shader_program) noexcept;
        
        virtual ~Renderer();

        /**
         * @brief Get the Shader Program object.
         * 
         * @return const std::unique_ptr<ShaderProgram>& The shader program.
         */
        const std::unique_ptr<ShaderProgram>& GetShaderProgram() const { return shader_program; }

        /**
         * @brief Add a render task to the renderer.
         * 
         * @param p_task The render task to add.
         */
        void AddRenderTask(const Task& p_task);

        /**
         * @brief Refresh the renderer.
         */
        void Refresh();

        /**
         * @brief Render.
         * 
         * @param p_context The context to render in.
         */
        void Render();
    };
}