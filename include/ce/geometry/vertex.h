#pragma once
#include "ce/math/math.hpp"

namespace CrossEngine
{
    /**
     * @brief Vertex class.
     * @details A vertex is a point in 3D space, and it is connected to other vertices.
     * A vertex can only be connected to two other vertices, which are the previous and the
     * next. The "next" of the previous vertex is the current vertex, and the "previous" of
     * the next vertex is the current vertex.
     */
    class Vertex
    {
        Math::Vec4 position;
        Math::Vec4 normal;
        Math::Vec2 uv;


        Vertex* prev = nullptr;
        Vertex* next = nullptr;
    public:
        
        /**
         * @brief The count of elements in the vertex array.
         */
        static constexpr size_t ARRAY_SIZE = 11;

        /**
         * @brief Constructor for Vertex.
         */
        Vertex();

        /**
         * @brief Constructor for Vertex.
         * 
         * @param p_position The position of the vertex.
         * @param p_normal The normal of the vertex.
         * @param p_uv The uv of the vertex.
         */
        Vertex(const Math::Vec4& p_position, const Math::Vec4& p_normal, const Math::Vec2& p_uv);

        /**
         * @brief Constructor for Vertex.
         * 
         * @param p_position The position of the vertex.
         */
        Vertex(const Math::Vec4& p_position);

        /**
         * @brief Copy constructor for Vertex.
         * 
         * @param p_other The other vertex.
         */
        Vertex(const Vertex& p_other);

        /**
         * @brief Move constructor for Vertex.
         * 
         * @param p_other The other vertex.
         */
        Vertex(Vertex&& p_other) noexcept;

        /**
         * @brief Get the normal of the vertex.
         * 
         * @return const Math::Vec4& The normal of the vertex.
         */
        FORCE_INLINE const Math::Vec4& GetNormal() const noexcept { return normal; }

        /**
         * @brief Get the normal of the vertex.
         * 
         * @return Math::Vec4& The normal of the vertex.
         */
        FORCE_INLINE Math::Vec4& Normal() noexcept { return normal; }

        /**
         * @brief Set the normal of the vertex.
         * 
         * @param p_normal The normal of the vertex.
         */
        FORCE_INLINE void SetNormal(const Math::Vec4& p_normal) noexcept { normal = p_normal; }

        virtual ~Vertex();

        /**
         * @brief Get the position of the vertex.
         * 
         * @return const Math::Vec4& The position of the vertex.
         */
        FORCE_INLINE const Math::Vec4& GetPosition() const noexcept { return position; }

        /**
         * @brief Get the position of the vertex.
         * 
         * @return const Math::Vec4& The position of the vertex.
         */
        FORCE_INLINE Math::Vec4& Position() noexcept { return position; }
        /**
         * @brief Set the position of the vertex.
         * 
         * @param p_position The position of the vertex.
         */
        FORCE_INLINE void SetPosition(const Math::Vec4& p_position) noexcept { position = p_position; }

        /**
         * @brief Get the uv of the vertex.
         * 
         * @return const Math::Vec2& The uv of the vertex.
         */
        FORCE_INLINE const Math::Vec2& GetUV() const noexcept { return uv; }

        /**
         * @brief Get the uv of the vertex.
         * 
         * @return Math::Vec2& The uv of the vertex.
         */
        FORCE_INLINE Math::Vec2& UV() noexcept { return uv; }

        /**
         * @brief Set the uv of the vertex.
         * 
         * @param p_uv The uv of the vertex.
         */
        FORCE_INLINE void SetUV(const Math::Vec2& p_uv) noexcept { uv = p_uv; }

        /**
         * @brief Get the previous vertex.
         * 
         * @return Vertex* The previous vertex.
         */
        Vertex* GetPrev() noexcept { return prev; }
        /**
         * @brief Get the next vertex.
         * 
         * @return Vertex* The next vertex.
         */
        Vertex* GetNext() noexcept { return next; }

        /**
         * @brief Get the previous vertex.
         * 
         * @return const Vertex* The previous vertex.
         */
        const Vertex* GetPrev() const noexcept { return prev; }
        /**
         * @brief Get the next vertex.
         * 
         * @return Vertex* The next vertex.
         */
        const Vertex* GetNext() const noexcept { return next; }

        /**
         * @brief Insert a vertex before this vertex.
         * 
         * @param p_vertex The vertex to be inserted.
         */
        void InsertPrev(Vertex* p_vertex);
        /**
         * @brief Insert a vertex after this vertex.
         * 
         * @param p_vertex The vertex to be inserted.
         */
        void InsertNext(Vertex* p_vertex);

        /**
         * @brief Connect a vertex to the previous of the current vertex.
         * @note The original previous vertex will be disconnected and the memory
         * will not be freed.
         * @param p_vertex The vertex to be connected.
         */
        void ConnectPrev(Vertex* p_vertex);

        /**
         * @brief Connect a vertex to the next of the current vertex.
         * @note The original next vertex will be disconnected and the memory
         * will not be freed.
         * @param p_vertex The vertex to be connected.
         */
        void ConnectNext(Vertex* p_vertex);

        /**
         * @brief Remove the previous vertex.
         * @return Vertex* The removed vertex. The memory will not be freed
         * when you call this function.
         */
        Vertex* RemovePrev();
        /**
         * @brief Remove the next vertex.
         * @return Vertex* The removed vertex. The memory will not be freed
         * when you call this function.
         */
        Vertex* RemoveNext();

        /**
         * @brief Reset the normal of the vertex to the induced normal.
         * 
         */
        FORCE_INLINE void ResetNormal() { normal = GetInducedNormal(); }

        
        /**
         * @brief Get the vertex array.
         * 
         * @param p_buff The buffer to store the vertex array.
         * @param p_buff_size The size of the buffer.
         * @return float* The pointer to the buffer.
         */
        FORCE_INLINE float* GetArray(float* p_buff, size_t p_buff_size) const { return GetArray(p_buff, p_buff_size, GetTangent()); }

        /**
         * @brief Get the vertex array.
         * 
         * @param p_buff The buffer to store the vertex array.
         * @param p_buff_size The size of the buffer.
         * @param p_tangent The tangent of the vertex.
         * @return float* The pointer to the buffer.
         */
        float* GetArray(float* p_buff, size_t p_buff_size, const Math::Vec4& p_tangent) const;

        /**
         * @brief Get the normal based on the previous and the next vertex.
         * 
         * @return Math::Vec4 The normal.
         */
        Math::Vec4 GetInducedNormal() const;

        /**
         * @brief Is the vertex a ear.
         * 
         * @return true The vertex is a ear.
         * @return false The vertex is not a ear.
         */
        bool IsEar() const;

        /**
         * @brief Get the tangent of the vertex.
         * @todo Buffer the tangent.
         * @return Math::Vec4 The tangent of the vertex.
         */
        Math::Vec4 GetTangent() const;
    };
}