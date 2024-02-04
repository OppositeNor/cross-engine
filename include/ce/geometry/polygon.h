#pragma once
#include "ce/geometry/triangle.h"

#include <vector>

namespace CrossEngine
{
    class PolygonN : public AGeometry
    {
        std::vector<Vertex*> vertices;
    public:
        PolygonN();
        PolygonN(std::initializer_list<Vertex*>&& p_vertices);
        virtual ~PolygonN() override;

        /**
         * @brief Get the number of vertices in the PolygonN.
         * 
         * @return size_t The number of vertices.
         */
        FORCE_INLINE size_t GetVertexCount() const { return vertices.size(); }

        /**
         * @brief Get the vertex at index.
         * 
         * @param p_index The index of the vertex.
         * @return Vertex* The vertex.
         */
        FORCE_INLINE const Vertex* GetVertex(size_t p_index) const { return vertices[p_index]; }

        /**
         * @brief Add a vertex to the PolygonN.
         * 
         * @param p_vertex The vertex to be added.
         */
        void AddVertex(size_t p_index, Vertex* p_vertex);

        /**
         * @brief Triangulate the PolygonN.
         * 
         * @param p_triangles The triangles to be filled.
         */
        void Triangulate(std::vector<Triangle*>& p_triangles) const;
    };
}