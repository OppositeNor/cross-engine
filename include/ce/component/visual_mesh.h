#pragma once
#include "ce/component/component3D.h"
#include <map>

namespace CrossEngine
{
    class Triangle;
    class AMaterial;
    class VisualMesh : public Component3D
    {
    protected:
        std::map<Window*, unsigned int> vaos;
        std::map<Window*, unsigned int> vbos;
        void UpdateVAO(const std::vector<Triangle*>& p_triangles, unsigned int p_vao, unsigned int p_vbo);

        std::shared_ptr<AMaterial> material;
    public:
        VisualMesh(const std::string& p_component_name = "visual mesh");
        ~VisualMesh();

        VisualMesh(const VisualMesh& p_other) 
            : Component3D(p_other) {};

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
         * @brief Return the map of the context correspond to it's vao.
         * 
         * @return std::map<const Window*, unsigned int> The map of the vaos.
         */
        FORCE_INLINE std::map<Window*, unsigned int> GetVAOs() const { return vaos; }

        /**
         * @brief Return the map of the context correspond to it's vbo.
         * 
         * @return std::map<Window*, unsigned int> The map of the vbos.
         */
        FORCE_INLINE std::map<Window*, unsigned int> GetVBOs() const { return vbos; }

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
         * @brief Draw the mesh.
         * 
         */
        virtual void Draw(Window* p_context) override;

        virtual const std::vector<Triangle*>& GetTriangles() = 0;
    };
}