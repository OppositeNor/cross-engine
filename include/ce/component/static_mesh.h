#pragma once
#include "ce/component/visual_mesh.h"

class Window;
class StaticMesh : public VisualMesh
{
    size_t vertex_count = 0;
public:

    StaticMesh(Window* p_context);

    /**
     * @brief Construct a new mesh.
     * 
     * @param p_triangles The triangles of this mesh.
     * @param p_context The context to load the mesh in.
     */
    StaticMesh(std::vector<Triangle*>&& p_triangles, Window* p_context);

    virtual size_t GetVertexCount() const override { return vertex_count;}

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

};