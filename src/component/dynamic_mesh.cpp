#include "ce/component/dynamic_mesh.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "glad/glad.h"

DynamicMesh::DynamicMesh(Window* p_context)
    : VisualMesh(p_context)
{
    
}

DynamicMesh::DynamicMesh(std::vector<Triangle*>&& p_triangles, Window* p_context)
    : DynamicMesh(p_context)
{
    triangles = std::move(p_triangles);
}

DynamicMesh::DynamicMesh(const std::string& p_file, Window* p_context)
    : DynamicMesh(p_context)
{
    Resource::LoadTris(p_file, triangles);
}

DynamicMesh::DynamicMesh(DynamicMesh&& p_other) noexcept
    : VisualMesh(std::move(p_other))
{
    triangles = std::move(p_other.triangles);
}

DynamicMesh::~DynamicMesh()
{
    for (auto i : triangles)
        delete i;
}

std::vector<Triangle*>& DynamicMesh::GetTriangles()
{
    triangles_dirty = true;
    return triangles;
}

void DynamicMesh::Update(float p_delta)
{
    VisualMesh::Update(p_delta);
    if (triangles_dirty)
    {
        auto vertex_count = GetVertexCount();
        std::unique_ptr<float> vertices = std::unique_ptr<float>(new float[vertex_count * Vertex::ARRAY_SIZE]);
        Resource::CreateModelVertexArray(triangles, vertices.get(), vertex_count * Vertex::ARRAY_SIZE);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * Vertex::ARRAY_SIZE * sizeof(float), vertices.get(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::ARRAY_SIZE * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}

void DynamicMesh::LoadTriangles(const std::string& p_file)
{
    for (auto i : triangles)
        delete i;
    Resource::LoadTris(p_file, triangles);
}