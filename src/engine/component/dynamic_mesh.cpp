#include "ce/component/dynamic_mesh.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "glad/glad.h"
#include "ce/graphics/window.h"
#include "ce/component/camera.h"
#include "ce/graphics/renderer/renderer.h"

#include <algorithm>

namespace CrossEngine
{
    void DynamicMesh::SetTrianglesDirty(bool p_dirty)
    {
        std::lock_guard<std::mutex> lock(triangles_mutex);
        triangles_dirty = p_dirty;
    }

    DynamicMesh::DynamicMesh(const std::string& p_component_name)
        : VisualMesh(p_component_name)
    {
        
    }

    DynamicMesh::DynamicMesh(std::vector<Triangle*>&& p_triangles, const std::string& p_component_name)
        : DynamicMesh(p_component_name)
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
        SetTrianglesDirty(true);
        return triangles;
    }

    void DynamicMesh::Update(float p_delta)
    {
    }

    float DynamicMesh::GetPriority(Window* p_context) const
    {
        if (material->ShouldPrioritize())
        {
            std::lock_guard<std::mutex> lock(triangles_mutex);
            auto to_camera = p_context->GetUsingCamera()->GetGlobalPosition() - GetSubspaceMatrix() * triangles[0]->GetCenter();
            return to_camera.LengthSquared();
        }
        else
        {
            return VisualMesh::GetPriority(p_context);
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

    void DynamicMesh::Draw(Window* p_context)
    {
        VisualMesh::Draw(p_context);
        if (material->ShouldPrioritize())
        {
            std::lock_guard<std::mutex> lock(triangles_mutex);
            Math::Vec4 camera_pos = p_context->GetUsingCamera()->GetGlobalPosition();
            auto subspace_matrix = GetSubspaceMatrix();
            std::sort(triangles.begin(), triangles.end(), [this, &camera_pos, &subspace_matrix](const Triangle* p_a, const Triangle* p_b)
            {
                auto a_to_camera = camera_pos - subspace_matrix * p_a->GetCenter();
                auto b_to_camera = camera_pos - subspace_matrix * p_b->GetCenter();
                return a_to_camera.LengthSquared() > b_to_camera.LengthSquared();
            });
            UpdateVAO(triangles, GetVAO(p_context), GetVBO(p_context));
            triangles_dirty = false;
            return;
        }
        if (triangles_dirty)
        {
            std::lock_guard<std::mutex> lock(triangles_mutex);
            if (triangles_dirty)
            {
                UpdateVAO(triangles, GetVAO(p_context), GetVBO(p_context));
                triangles_dirty = false;
            }
        }
    }
}