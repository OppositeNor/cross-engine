#pragma once
#include "ce/component/component.h"

class Triangle;
class ATexture;
class AMaterial;
class VisualMesh : public Component
{
protected:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    void UpdateVAO(const std::vector<Triangle*>& p_triangles);

    std::shared_ptr<ATexture> texture;

    std::shared_ptr<AMaterial> material;
public:
    VisualMesh(Window* p_context);
    ~VisualMesh();

    VisualMesh(const VisualMesh& p_other) 
        : Component(p_other) {};

    VisualMesh(VisualMesh&& p_other) noexcept;

    /**
     * @brief Get the material of this component.
     * 
     * @return const std::shared_ptr<AMaterial>& The material of this component.
     */
    FORCE_INLINE const std::shared_ptr<AMaterial>& GetMaterial() const noexcept { return material; }

    /**
     * @brief Get the material of this component.
     * 
     * @return std::shared_ptr<AMaterial> The material of this component.
     */
    FORCE_INLINE std::shared_ptr<AMaterial> GetMaterial() { return material; }

    /**
     * @brief Set the material of this component.
     * 
     * @param p_material The material to be set to.
     */
    FORCE_INLINE void SetMaterial(std::shared_ptr<AMaterial> p_material) { material = p_material; } 

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
     * @brief Load the triangles.
     * 
     * @param p_triangles The triangles to load.
     */
    virtual void LoadTriangles(std::vector<Triangle*>&& p_triangles) = 0;

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
     * @brief Set the texture.
     * 
     * @param p_texture The texture to be set to.
     */
    FORCE_INLINE void SetTexture(std::shared_ptr<ATexture> p_texture) { texture = p_texture; }

    /**
     * @brief Get the texture.
     * 
     * @return std::shared_ptr<Texture> The texture.
     */
    FORCE_INLINE std::shared_ptr<ATexture> GetTexture() const { return texture; }

    /**
     * @brief Draw the mesh.
     * 
     */
    virtual void Draw() override;
};