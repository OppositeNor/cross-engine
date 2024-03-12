#pragma once
#include "ce/geometry/a_geometry.h"
#include "ce/geometry/vertex.h"


namespace CrossEngine
{
    /**
     * @brief The triangle geometry class.
     */
    class Triangle : public AGeometry
    {
        
        Vertex* vertices[3];

    public:

        static constexpr size_t TRIANGLE_ARRAY_SIZE = 3 * (Vertex::ARRAY_SIZE);
        
        /**
         * @brief Constructor.
         */
        Triangle();

        /**
         * @brief Copy constructor.
         * 
         * @param p_other The other triangle.
         */
        Triangle(const Triangle& p_other);

        /**
         * @brief Construct a new Triangle object.
         * 
         * @param p_v1 The first vertex.
         * @param p_v2 The second vertex.
         * @param p_v3 The third vertex.
         */
        Triangle(const Vertex& p_v1, const Vertex& p_v2, const Vertex& p_v3);

        /**
         * @brief Construct a new Triangle object.
         * 
         * @param p_v1 The first vertex.
         * @param p_v2 The second vertex.
         * @param p_v3 The third vertex.
         */
        Triangle(Vertex*&& p_v1, Vertex*&& p_v2, Vertex*&& p_v3) noexcept;

        /**
         * @brief Move constructor.
         * 
         * @param p_other The other triangle.
         */
        Triangle(Triangle&& p_other) noexcept;

        /**
         * @brief Construct a new Triangle object.
         * 
         * @param p_v1 The position of the first vertex.
         * @param p_v2 The position of the second vertex.
         * @param p_v3 The position of the third vertex.
         */
        Triangle(const Math::Vec4& p_v1, const Math::Vec4& p_v2, const Math::Vec4& p_v3);
        virtual ~Triangle();

        /**
         * @brief Get vertex from index.
         * 
         * @param i The index of the vertex to get.
         * @return Vertex* The vertex.
         */
        FORCE_INLINE Vertex* GetVertex(int i) {return vertices[i];}

        /**
         * @brief Get vertex from index.
         * 
         * @param i The index of the vertex to get.
         * @return const Vertex* The vertex.
         */
        FORCE_INLINE const Vertex* GetVertex(int i) const {return vertices[i];}

        FORCE_INLINE Vertex* operator[] (int i) {return vertices[i];}
        FORCE_INLINE const Vertex* operator[] (int i) const {return vertices[i];}

        /**
         * @brief Get the array of vertices.
         * 
         * @param p_buff The buffer to store the vertices in.
         * @param p_buff_size The size of the buffer.
         */
        float* GetVertexArray(float* p_buff, size_t p_buff_size) const;

        /**
         * @brief Get the normal of the triangle.
         * 
         * @return Math::Vec4 The normal of the triangle.
         */
        FORCE_INLINE Math::Vec4 GetNormal() const { return vertices[0]->GetInducedNormal(); }

        /**
         * @brief Get the tangent of a vertex.
         * 
         * @return Math::Vec4 The tangent of the vertex.
         */
        Math::Vec4 GetTangent() const;

        /**
         * @brief Get the center of the triangle.
         * 
         * @return Math::Vec4 The center of the triangle.
         */
        Math::Vec4 GetCenter() const;
        
        /**
         * @brief Get the closest vertex to a point.
         * 
         * @param p_point The point to get the closest vertex to.
         * @return Math::Vec4 The closest vertex to the point.
         */
        const Vertex* GetClosest(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point) const;

        /**
         * @brief Get the closest global position to a point.
         * 
         * @param p_point The point to get the closest position to.
         * @return Math::Vec4 The closest position to the point.
         */
        Math::Vec4 GetClosestPosition(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point) const;
        
        /**
         * @brief Get the least depth to a point according to a direction.
         * 
         * @param p_subspace_matrix The subspace matrix.
         * @param p_point The point of interest.
         * @param p_dir The direction.
         * @return float The least depth.
         */
        float GetLeastDepth(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point, Math::Vec4 p_dir) const;
    };
}