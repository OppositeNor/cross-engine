#include "ce/component/visual_mesh.h"
#include "ce/graphics/window.h"
#include "ce/geometry/triangle.h"
#include "ce/resource/resource.h"
#include "ce/texture/texture.h"
#include "ce/materials/material.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

VisualMesh::VisualMesh(Window* p_context)
    : Component(p_context)
{
    if (p_context->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("VisualMesh must be created on the same thread as the Window");
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
}

VisualMesh::~VisualMesh()
{
    GetContext()->FreeThreadResource(vbo);
    GetContext()->FreeThreadResource(vao);
}


VisualMesh::VisualMesh(VisualMesh&& p_other) noexcept
    : Component(std::move(p_other))
{
    vao = p_other.vao;
    p_other.vao = 0;
    vbo = p_other.vbo;
    p_other.vbo = 0;
}

void VisualMesh::Draw()
{
    Component::Draw();
    
    if (!IsVisible())
        return;
    if (GetContext()->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("Skybox must be drawn on the main thread.");
    
    glBindVertexArray(vao);
    GetContext()->GetShaderProgram()->SetUniform("model", GetSubspaceMatrix());
    if (material)
        material->SetUniform(GetContext()->GetShaderProgram());
    else
        GetContext()->GetDefaultMaterial()->SetUniform(GetContext()->GetShaderProgram());
    
    glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
}

void VisualMesh::UpdateVAO(const std::vector<Triangle*>& p_triangles)
{
    if (GetContext()->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("VAO must be updated on the same thread as the Window.");
    auto vertex_count = GetVertexCount();
    auto vertices = std::unique_ptr<float[]>(new float[vertex_count * Vertex::ARRAY_SIZE]);
    Resource::CreateModelVertexArray(p_triangles, vertices.get(), vertex_count * Vertex::ARRAY_SIZE);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * Vertex::ARRAY_SIZE * sizeof(float), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
}