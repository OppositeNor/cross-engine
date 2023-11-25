#include "ce/component/visual_mesh.h"
#include "glad/glad.h"

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