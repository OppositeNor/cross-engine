#pragma once
#include "ce/component/component.h"

class VisualMesh : public Component
{
protected:
    unsigned int vao = 0;
    unsigned int vbo = 0;
public:
    VisualMesh(Window* p_context);
    ~VisualMesh();

    VisualMesh(const VisualMesh& p_other) 
        : Component(p_other) {};

    VisualMesh(VisualMesh&& p_other) noexcept;

    /**
     * @brief Get the Vertex Array Object of this mesh.
     * 
     * @return unsigned int The Vertex Array Object of this mesh.
     */
    FORCE_INLINE unsigned int GetVAO() const { return vao; }

    /**
     * @brief Get the Vertex Buffer Object of this mesh.
     * 
     * @return unsigned int The Vertex Buffer Object of this mesh.
     */
    FORCE_INLINE unsigned int GetVBO() const { return vbo; }

    /**
     * @brief Get the vertex count of this mesh.
     * 
     * @return size_t The vertex count of this mesh.
     */
    virtual size_t GetVertexCount() const = 0;

    /**
     * @brief Load the triangles from a file.
     * 
     * @param p_file The file to load the triangles from.
     */
    virtual void LoadTriangles(const std::string& p_file) = 0;

    /**
     * @brief Load the triangles from a file.
     * 
     * @param p_file The file to load the triangles from.
     */
    virtual void LoadTrisWithNormal(const std::string& p_file) = 0;

    /**
     * @brief Draw the mesh.
     * 
     */
    virtual void Draw() override;
};