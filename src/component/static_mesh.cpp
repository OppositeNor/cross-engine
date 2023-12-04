#include "ce/component/static_mesh.h"
#include "ce/geometry/triangle.h"
#include "ce/resource/resource.h"
#include <glad/glad.h>
StaticMesh::StaticMesh(Window* p_context)
    : VisualMesh(p_context)
{
}


StaticMesh::StaticMesh(std::vector<Triangle*>&& p_triangles, Window* p_context)
    : VisualMesh(p_context)
{
    vertex_count = p_triangles.size() * 3;
    UpdateVAO(p_triangles);
    for (auto& i : p_triangles)
    {
        delete i;
        i = nullptr;
    }
}

void StaticMesh::LoadTriangles(const std::string& p_file)
{
    std::vector<Triangle*> triangles;
    Resource::LoadTris(p_file, triangles);
    vertex_count = triangles.size() * 3;
    UpdateVAO(triangles);
    for (auto i : triangles)
        delete i;
}

void StaticMesh::LoadTrisWithNormal(const std::string& p_file)
{
    std::vector<Triangle*> triangles;
    Resource::LoadTrisWithNormal(p_file, triangles);
    vertex_count = triangles.size() * 3;
    UpdateVAO(triangles);
    for (auto i : triangles)
        delete i;
}