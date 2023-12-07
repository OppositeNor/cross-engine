#include "ce/component/visual_mesh.h"
#include "ce/graphics/window.h"
#include "ce/geometry/triangle.h"
#include "ce/resource/resource.h"
#include "ce/graphics/texture/texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
std::mutex test_mutex;

VisualMesh::VisualMesh(Window* p_context)
    : Component(p_context)
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
}

VisualMesh::~VisualMesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
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
    glBindVertexArray(vao);
    GetContext()->GetShaderProgram()->SetUniform("model", GetSubspaceMatrix());
    if (texture)
        texture->BindTexture();
    else
        GetContext()->GetDefaultTexture()->BindTexture();
    glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
}

void VisualMesh::UpdateVAO(const std::vector<Triangle*>& p_triangles)
{
    auto vertex_count = GetVertexCount();
    std::unique_ptr<float[]> vertices = std::unique_ptr<float[]>(new float[vertex_count * Vertex::ARRAY_SIZE]);
    Resource::CreateModelVertexArray(p_triangles, vertices.get(), vertex_count * Vertex::ARRAY_SIZE);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * Vertex::ARRAY_SIZE * sizeof(float), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}