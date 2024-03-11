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
        mutable std::shared_mutex context_resource_mutex;
        void UpdateVAO(const std::vector<Triangle*>& p_triangles, unsigned int p_vao, unsigned int p_vbo);

        std::shared_ptr<AMaterial> material;

        /**
         * @brief Draw the mesh.
         * 
         */
        virtual void Draw(Window* p_context);
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
         * @brief Register the current mesh to the draw list.
         * 
         * @param p_context The context to register the mesh to.
         */
        virtual bool RegisterDraw(Window* p_context) override;

        /**
         * @brief Get the priority of drawing this mesh
         * 
         * @param p_context The context to get the priority for.
         * @return float The priority of drawing this mesh
         */
        virtual float GetPriority(Window* p_context) const { return 0.0f; }

        /**
         * @brief Return the vao corresponding to the context
         * 
         * @return unsigned int the corresponding vao
         */
        unsigned int GetVAO(Window* p_context) const;

        /**
         * @brief Return the vbo corresponding to the context
         * 
         * @return unsigned int the corresponding vbo
         */
        unsigned int GetVBO(Window* p_context) const;

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

        virtual const std::vector<Triangle*>& GetTriangles() = 0;
    };
}