#pragma once
#include <vector>
#include <mutex>
#include "ce/component/visual_mesh.h"
#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"

class DynamicMesh : public VisualMesh
{
    std::vector<Triangle*> triangles;
    bool triangles_dirty = true;
    std::mutex triangles_mutex;
public:

    /**
     * @brief Construct a new mesh.
     * 
     * @param p_context The context to load the mesh in.
     */
    explicit DynamicMesh(Window* p_context);

    /**
     * @brief Construct a new mesh.
     * 
     * @param p_triangles The triangles of this mesh.
     * @param p_context The context to load the mesh in.
     */
    DynamicMesh(std::vector<Triangle*>&& p_triangles, Window* p_context);

    /**
     * @brief Copy constructor for DynamicMesh.
     */
    DynamicMesh(const DynamicMesh& p_other);

    /**
     * @brief Move constructor for DynamicMesh.
     * 
     * @param p_other The mesh to move from.
     */
    DynamicMesh(DynamicMesh&& p_other) noexcept;

    /**
     * @brief Destroy the mesh.
     */
    ~DynamicMesh();

    
    /**
     * @brief Called every frame.
     * This function calles every neccessary function to update the component.
     * @note When overriding this method, make sure to call the base method.
     */
    virtual void Update(float p_delta) override;

    /**
     * @brief Get the triangles of this mesh.
     * 
     * @return std::vector<Triangle*>& The triangles of this mesh.
     */
    std::vector<Triangle*>& Triangles();

    /**
     * @brief Get the triangles of this mesh.
     * 
     * @return const std::vector<Triangle*>& The triangles of this mesh.
     */
    const std::vector<Triangle*>& GetTriangles() const { return triangles; }

    /**
     * @brief Load the triangles from a file.
     * 
     * @param p_file The file to load the triangles from.
     */
    virtual void LoadTriangles(const std::string& p_file) override;

    /**
     * @brief Load the triangles from a file.
     * 
     * @param p_file The file to load the triangles from.
     */
    virtual void LoadTrisWithNormal(const std::string& p_file) override;

    /**
     * @brief Get the vertex count of this mesh.
     * 
     * @return size_t The vertex count of this mesh.
     */
    virtual size_t GetVertexCount() const override { return triangles.size() * 3; }
};