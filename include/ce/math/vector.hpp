#pragma once
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cmath>
#include "ce/math/math_type_base.hpp"
#include "ce/defs.hpp"

/**
 * @brief Vector class.
 * 
 * @tparam T The type of the elements of the vector.
 * @tparam N The dimension of the vector.
 */
template <typename T, size_t N>
class Vector : public MathTypeBase
{
    T data[N];

public:
    inline static constexpr size_t DIMENSION = N;
    inline static const Vector<T, N> ZERO = Vector<T, N>();

    Vector() noexcept
    {
        static_assert(N > 0, "The vector dimension cannot be zero.");
        for (size_t i = 0; i < N; ++i)
            data[i] = T();
    }

    /**
     * @brief Construct or for Vector.
     * 
     * @param p_x The x component.
     * @param p_y The y component.
     * @param p_z The z component.
     * @param p_w The w component.
     */
    Vector(const T& p_x, const T& p_y, const T& p_z, const T& p_w)
        : data{p_x, p_y, p_z, p_w}
    {
        static_assert(N >= 4, "The vector dimension is less than 4");
    }

    /**
     * @brief Construct or for Vector.
     * 
     * @param p_x The x component.
     * @param p_y The y component.
     * @param p_z The z component.
     */
    Vector(const T& p_x, const T& p_y, const T& p_z)
        : data{p_x, p_y, p_z}
    {
        static_assert(N >= 3, "The vector dimension is less than 3");
    }

    /**
     * @brief Construct a new Vector object.
     * 
     * @param p_list The initializer list.
     * @throw std::invalid_argument The size of the initializer list does not match vector dimension.
     */
    Vector(const std::initializer_list<T>& p_list)
    {
        if (p_list.size() != N)
            throw std::invalid_argument("The size of the initializer list does not match vector dimension.");
        for (size_t i = 0; i < N; ++i)
            data[i] = *(p_list.begin() + i);
    }

    operator std::string() const
    {
        std::stringstream ss;
        ss << "Vector<" << typeid(T).name() << ", " << N << ">(";
        if constexpr (std::is_convertible_v<T, std::string>) {
            ss << (std::string)data[0];
            for (size_t i = 1; i < N; ++i)
                ss << ", " << (std::string)data[i];
        } else {
            ss << data[0];
            for (size_t i = 1; i < N; ++i)
                ss << ", " << data[i];
        }
        ss << ")";
        return ss.str();
    }


    FORCE_INLINE T& operator[](int p_index)
    {
        return data[p_index];
    }

    FORCE_INLINE const T& operator[](int p_index) const
    {
        return data[p_index];
    }

    /**
     * @brief Dot product of this vector and another vector.
     * 
     * @param p_other The other vector.
     * @return decltype(std::declval<T>() * std::declval<T>()) The result of the dot product.s
     */
    template<typename T1>
    FORCE_INLINE auto Dot(const Vector<T1, N>& p_other) const
        -> decltype(std::declval<T>() * std::declval<T1>())
    {
        auto result = decltype(std::declval<T>() * std::declval<T1>())();
        for (size_t i = 0; i < N; ++i)
            result += data[i] * p_other[i];
        return result;
    }

    /**
     * @brief Cross product of this vector and another vector.
     * 
     * @tparam T1 The type of the other vector.
     * @param p_other The other vector.
     * @return decltype(std::declval<T>() * std::declval<T1>()) The result of the cross product.
     */
    template<typename T1>
    auto Cross(const Vector<T1, N>& p_other) const
        -> Vector<decltype(std::declval<T>() * std::declval<T1>() - std::declval<T>() * std::declval<T1>()), N>
    {
        static_assert(N == 3 || N == 4, "The vector dimension is not valid for dot product.");
        if constexpr (N == 4)
        {
            if (data[3] != T() || p_other[3] != T1())
                throw std::invalid_argument("This four dimensional vector is not a direction. Cannot do cross product.");
        }
        using VectorResult = Vector<decltype(std::declval<T>() * std::declval<T1>()), N>;
        VectorResult result;
#ifndef CE_LHAND_COORD
        result[0] = data[1] * p_other[2] - data[2] * p_other[1];
        result[1] = data[2] * p_other[0] - data[0] * p_other[2];
        result[2] = data[0] * p_other[1] - data[1] * p_other[0];
#else
        result[0] = data[2] * p_other[1] - data[1] * p_other[2];
        result[1] = data[0] * p_other[2] - data[2] * p_other[0];
        result[2] = data[1] * p_other[0] - data[0] * p_other[1];
#endif
        return result;
    }

    /**
     * @brief Cross product of v1 and v2.
     * 
     * @param p_vec1 v1.
     * @param p_vec2 v2.
     * @return Vec3 The result of the cross product.
     */
    FORCE_INLINE static Vector<float, 3> Cross(const Vector<float, 3>& p_vec1, const Vector<float, 3>& p_vec2);
    /**
     * @brief Cross product of v1 and v2.
     * 
     * @param p_vec1 v1.
     * @param p_vec2 v2.
     * @return Vec4 The result of the cross product.
     */
    FORCE_INLINE static Vector<float, 4> Cross(const Vector<float, 4>& p_vec1, const Vector<float, 4>& p_vec2);

    /**
     * @brief The length of the vector.
     * 
     * @return T The length of the vector.
     */
    auto Length() const
    {
        if constexpr (!has_times<T, T>)
            throw std::domain_error("The type of the elements is not able to get the length.");
        else
        {
            auto result = decltype(std::declval<T>() * std::declval<T>())();
            for (size_t i = 0; i < N; ++i)
                result += data[i] * data[i];
            return std::sqrt(result);
        }
    }
    
    /**
     * @brief Normalize this vector.
     * 
     * @return Vector<T, N>& The reference to this vector.
     */
    FORCE_INLINE Vector<T, N>& Normalize()
    {
        if (*this == ZERO)
            return *this;
        auto length = Length();
        for (size_t i = 0; i < N; ++i)
            data[i] /= length;
        return *this;
    }

    /**
     * @brief Get the normalized vector.
     * 
     * @return Vector<T, N> The normalized vector.
     */
    FORCE_INLINE Vector<T, N> Normalized() const
    {
        Vector<T, N> result = *this;
        return result.Normalize();
    }

    template <typename T1>
    FORCE_INLINE auto operator+(const Vector<T1, N>& p_other) const
        -> Vector<decltype(std::declval<T>() + std::declval<T1>()), N>
    {
        using result_val_type = decltype(std::declval<T>() + std::declval<T1>());
        Vector<result_val_type, N> result;
        for (size_t i = 0; i < N; ++i)
            result[i] = data[i] + p_other[i];
        return result;
    }

    template <typename T1>
    FORCE_INLINE Vector<T, N>& operator+=(const Vector<T1, N>& p_other)
    {
        for (size_t i = 0; i < N; ++i)
            data[i] += p_other[i];
        return *this;
    }

    template <typename T1>
    FORCE_INLINE auto operator-(const Vector<T1, N>& p_other) const
        -> Vector<decltype(std::declval<T>() - std::declval<T1>()), N>
    {
        using result_val_type = decltype(std::declval<T>() - std::declval<T1>());
        Vector<result_val_type, N> result;
        for (size_t i = 0; i < N; ++i)
            result[i] = data[i] - p_other[i];
        return result;
    }

    template <typename T1>
    FORCE_INLINE Vector<T, N>& operator -=(const Vector<T1, N>& p_other) const
    {
        for (size_t i = 0; i < N; ++i)
            data[i] -= p_other[i];
        return *this;
    }

    template <typename T1>
    FORCE_INLINE Vector<T, N> operator*= (T1&& p_scaler)
    {
        for (size_t i = 0; i < N; ++i)
            data[i] *= std::forward<T1>(p_scaler);
        return *this;
    }

    template <typename T1>
    FORCE_INLINE bool operator==(const Vector<T1, N>& p_other) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (!(data[i] == p_other[i]))
                return false;
        }
        return true;
    }

    template <typename T1>
    FORCE_INLINE bool operator!=(const Vector<T1, N>& p_other) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (data[i] != p_other[i])
                return true;
        }
        return false;
    }
};

template <size_t N>
using Vecf = Vector<float, N>;
using Vec2f = Vecf<2>;
using Vec3f = Vecf<3>;
using Vec4f = Vecf<4>;

template <size_t N>
using Vec = Vecf<N>;
using Vec2 = Vecf<2>;
using Vec3 = Vecf<3>;
using Vec4 = Vecf<4>;

template <size_t N>
using Veci = Vector<int, N>;
using Vec2i = Veci<2>;
using Vec3i = Veci<3>;
using Vec4i = Veci<4>;

template <size_t N>
using Vecs = Vector<size_t, N>;
using Vec2s = Vecs<2>;
using Vec3s = Vecs<3>;
using Vec4s = Vecs<4>;

FORCE_INLINE Vec4 Pos()
{
    return Vec4(0, 0, 0, 1);
}

FORCE_INLINE Vec4 Pos(float p_x, float p_y, float p_z)
{
    return Vec4(p_x, p_y, p_z, 1);
}