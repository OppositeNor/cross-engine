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
        render_tasks.push_back(p_task);
    }

    void Renderer::RemoveRenderTask(const Task& p_task)
    {
        for (auto it = render_tasks.begin(); it != render_tasks.end(); ++it)
        {
            if (it->task.target_type() == p_task.task.target_type())
            {
                render_tasks.erase(it);
                break;
            }
        }
    }

    void Renderer::Refresh()
    {
        shader_program->Refresh();
        render_tasks.clear();
    }

    void Renderer::Render()
    {
        std::sort(render_tasks.begin(), render_tasks.end(), [](const Task& a, const Task& b) { return a > b; });
        shader_program->Use();
        for (auto& task : render_tasks)
        {
            task.task();
            auto error = glGetError();
            if (error != GL_NO_ERROR)
                throw std::runtime_error("OpenGL error: " + std::to_string(error));
        }
    }
}