#include "ce/component/dynamic_mesh.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "glad/glad.h"

void DynamicMesh::SetTrianglesDirty(bool p_dirty)
{
    std::lock_guard<std::mutex> lock(triangles_mutex);
    triangles_dirty = p_dirty;
}

DynamicMesh::DynamicMesh(Window* p_context)
    : VisualMesh(p_context)
{
    
}

DynamicMesh::DynamicMesh(std::vector<Triangle*>&& p_triangles, Window* p_context)
    : DynamicMesh(p_context)
{
    triangles = std::move(p_triangles);
}

DynamicMesh::DynamicMesh(const DynamicMesh& p_other)
    : VisualMesh(p_other)
{
    for (auto i : p_other.triangles)
        triangles.push_back(new Triangle(*i));
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

std::vector<Triangle*>& DynamicMesh::Triangles()
{
    triangles_dirty = true;
    return triangles;
}

void DynamicMesh::Update(float p_delta)
{
    VisualMesh::Update(p_delta);
    if (triangles_dirty)
    {
        std::lock_guard<std::mutex> lock(triangles_mutex);
        if (triangles_dirty)
        {
            triangles_dirty = false;
            UpdateVAO(triangles);
        }
    }
}

void DynamicMesh::LoadTriangles(std::vector<Triangle*>&& p_triangles)
{
    for (auto i : triangles)
        delete i;
    triangles = std::move(p_triangles);
    SetTrianglesDirty(true);
}

void DynamicMesh::LoadTriangles(const std::string& p_file)
{
    for (auto i : triangles)
        delete i;
    Resource::LoadTris(p_file, triangles);
    SetTrianglesDirty(true);
}

void DynamicMesh::LoadTrisWithNormal(const std::string& p_file)
{
    for (auto i : triangles)
        delete i;
    Resource::LoadTrisWithNormal(p_file, triangles);
    SetTrianglesDirty(true);
}