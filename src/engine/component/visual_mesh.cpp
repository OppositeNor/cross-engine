#include "ce/component/visual_mesh.h"
#include "ce/graphics/window.h"
#include "ce/graphics/renderer/renderer.h"
#include "ce/geometry/triangle.h"
#include "ce/resource/resource.h"
#include "ce/texture/texture.h"
#include "ce/materials/material.h"
#include "ce/game/game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CrossEngine
{
    VisualMesh::VisualMesh(const std::string& p_component_name)
        : Component3D(p_component_name)
    {
        material = Graphics::GetDefaultMaterial();
    }

    VisualMesh::~VisualMesh()
    {
        if (!Game::IsInitialized())
            return;
        for (auto& i : vbos) {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                i.first->FreeThreadResource(i.second);
        }
        for (auto& i : vaos) {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                i.first->FreeThreadResource(i.second);
        }
    }


    VisualMesh::VisualMesh(VisualMesh&& p_other) noexcept
        : Component3D(std::move(p_other))
    {
        vaos = std::move(p_other.vaos);
        vbos = std::move(p_other.vbos);
        material = p_other.material;
    }

    unsigned int VisualMesh::GetVAO(Window* p_context) const
    {
        std::shared_lock<std::shared_mutex> context_resource_mutex;
        return vaos.at(p_context);
    }

    unsigned int VisualMesh::GetVBO(Window* p_context) const
    {
        std::shared_lock<std::shared_mutex> context_resource_mutex;
        return vbos.at(p_context);
    }

    bool VisualMesh::RegisterDraw(Window* p_context)
    {
        if (Component3D::RegisterDraw(p_context))
        {
            p_context->GetRenderer()->AddRenderTask(Task([this, p_context](){Draw(p_context);}, GetPriority(p_context)));
            return true;
        }
        return false;
    }

    void VisualMesh::Draw(Window* p_context)
    {
        if (!IsVisible())
            return;
        
        bool should_add_context_resource = false;

        {
            std::shared_lock<std::shared_mutex> lock(context_resource_mutex);
            if (!vaos.contains(p_context))
            {
                should_add_context_resource = true;
            }
        }
        
        if (should_add_context_resource)
        {
            std::unique_lock<std::shared_mutex> lock(context_resource_mutex);
            unsigned int vao, vbo;
            glGenBuffers(1, &vbo);
            p_context->RegisterThreadResource(vbo, glDeleteBuffers);
            glGenVertexArrays(1, &vao);
            p_context->RegisterThreadResource(vao, glDeleteVertexArrays);
            UpdateVAO(GetTriangles(), vao, vbo);
            vaos[p_context] = vao;
            vbos[p_context] = vbo;
        }

        if (p_context->GetThreadId() != std::this_thread::get_id())
            throw std::runtime_error("Skybox must be drawn on the main thread.");
        
        {
            std::shared_lock<std::shared_mutex> lock(context_resource_mutex);
            glBindVertexArray(vaos[p_context]);
        }
        
        p_context->GetRenderer()->GetShaderProgram()->SetUniform("model", GetSubspaceMatrix());
        material->SetUniform(p_context);
        
        glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
    }

    void VisualMesh::UpdateVAO(const std::vector<Triangle*>& p_triangles, unsigned int p_vao, unsigned int p_vbo)
    {
        auto vertex_count = GetVertexCount();
        auto vertices = std::unique_ptr<float[]>(new float[vertex_count * Vertex::ARRAY_SIZE]);
        Resource::CreateModelVertexArray(p_triangles, vertices.get(), vertex_count * Vertex::ARRAY_SIZE);
        glBindVertexArray(p_vao);
        glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * Vertex::ARRAY_SIZE * sizeof(float), vertices.get(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glBindVertexArray(0);
    }
}