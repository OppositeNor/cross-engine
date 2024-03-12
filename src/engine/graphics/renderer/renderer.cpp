#include "ce/graphics/renderer/renderer.h"
#include "ce/graphics/shader/shader_program.h"
#include "glad/glad.h"

#include <algorithm>

namespace CrossEngine
{
    Renderer::Renderer(ShaderProgram*&& p_shader_program) noexcept
        : shader_program(p_shader_program)
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::AddRenderTask(const Task& p_task)
    {
        if (p_task.priority == 0)
            unprioritized_render_tasks.push_back(p_task);
        else
            render_tasks.push_back(p_task);
    }

    void Renderer::Refresh()
    {
        shader_program->Refresh();
        render_tasks.clear();
        unprioritized_render_tasks.clear();
    }

    void Renderer::Render()
    {
        glEnable(GL_CULL_FACE);
        shader_program->Use();
        for (auto& task : unprioritized_render_tasks)
            task.task();
        glDisable(GL_CULL_FACE);
        std::sort(render_tasks.begin(), render_tasks.end(),
            [](const Task& a, const Task& b) { return a > b; });
        for (auto& task : render_tasks)
        {
            task.task();
            auto error = glGetError();
            if (error != GL_NO_ERROR)
                throw std::runtime_error("OpenGL error: " + std::to_string(error));
        }
    }
}