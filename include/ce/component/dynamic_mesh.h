#pragma once
#include <vector>
#include <mutex>
#include "ce/component/visual_mesh.h"
#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"

namespace CrossEngine
{
    class DynamicMesh : public VisualMesh
    {
    private:
        std::vector<Triangle*> triangles;
        bool triangles_dirty = true;
        std::mutex triangles_mutex;

        void SetTrianglesDirty(bool p_dirty);
    protected:
        
        /**
         * @brief Draw the mesh.
         * 
         */
        virtual void Draw(Window* p_context) override;
    public:

        /**
         * @brief Construct a new mesh.
         * 
         * @param p_context The context to load the mesh in.
         */
        explicit DynamicMesh(const std::string& p_component_name = "dynamic mesh");

        /**
         * @brief Construct a new mesh.
         * 
         * @param p_triangles The triangles of this mesh.
         * @param p_context The context to load the mesh in.
         */
        explicit DynamicMesh(std::vector<Triangle*>&& p_triangles, const std::string& p_component_name = "dynamic mesh");

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
         * @brief Get the triangles.
         * 
         * @return const std::vector<Triangle*>& The triangles of the mesh.
         */
        virtual const std::vector<Triangle*>& GetTriangles() override { return triangles; };

        /**
         * @brief Get the vertex count of this mesh.
         * 
         * @return size_t The vertex count of this mesh.
         */
        virtual size_t GetVertexCount() const override { return triangles.size() * 3; }

        /**
         * @brief Get the triangles of this mesh.
         * 
         * @return const std::vector<Triangle*>& The triangles of this mesh.
         */
        const std::vector<Triangle*>& GetTriangles() const { return triangles; }

        /**
         * @brief Load the triangles.
         * 
         * @param p_triangles The triangles to load.
         */
        virtual void LoadTriangles(std::vector<Triangle*>&& p_triangles) override;

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
}