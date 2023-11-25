#pragma once
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "ce/defs.hpp"
#include "ce/math/math_type_base.hpp"
#include "ce/math/vector.hpp"

/**
 * @brief A MxN matrix.
 * 
 * @tparam T The type of the matrix.
 * @tparam tM The number of rows.
 * @tparam tN The number of columns.
 */
template <typename T, size_t tM, size_t tN>
class Matrix : public MathTypeBase
{
    union{
        T data[tM * tN];
        T data2[tM][tN];
    };

public:
    static constexpr size_t M = tM;
    static constexpr size_t N = tN;
    static constexpr size_t SIZE = M * N;

    static float global_buffer[SIZE];

    Matrix()
    {
        static_assert(M > 0 && N > 0, "The matrix dimension cannot be zero.");
        if constexpr (std::is_arithmetic_v<T>)
        {
            for (size_t i = 0; i < M; ++i)
            {
                for (size_t j = 0; j < N; ++j)
                {
                    data2[i][j] = i == j ? (T)1 : (T)0;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < SIZE; ++i)
                data[i] = T();
        }
    }

    Matrix(const std::initializer_list<T>& p_list)
    {
        if (p_list.size() != SIZE)
            throw std::invalid_argument("The size of the initializer list does not match matrix dimension.");

        for (size_t i = 0; i < SIZE; ++i)
            data[i] = *(p_list.begin() + i);
    }

    FORCE_INLINE T* operator [](size_t i)
    {
        return data2[i];
    }

    FORCE_INLINE const T* operator [](size_t p_i) const
    {
        return data2[p_i];
    }

    FORCE_INLINE T& operator()(size_t p_i)
    {
        return data[p_i];
    }

    FORCE_INLINE const T& operator()(size_t p_i) const
    {
        return data[p_i];
    }

    FORCE_INLINE T& operator [](size_t p_i, size_t p_j)
    {
        return data2[p_i][p_j];
    }

    FORCE_INLINE const T& operator [](size_t p_i, size_t p_j) const
    {
        return data2[p_i][p_j];
    }

    Matrix<T, N, M> Transpose() const
    {
        Matrix<T, N, M> result;
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                result[j][i] = data2[i][j];
            }
        }
        return result;
    }

    template<typename T1>
    FORCE_INLINE bool operator ==(const Matrix<T1, M, N>& p_other) const
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            if (!(data[i] == p_other(i)))
                return false;
        }
        return true;
    }

    template<typename T1>
    FORCE_INLINE bool operator !=(const Matrix<T1, M, N>& p_other) const
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            if (data[i] != p_other(i))
                return true;
        }
        return false;
    }

    template<typename T1>
    FORCE_INLINE auto operator +(const Matrix<T1, M, N>& p_other) const
        -> Matrix<decltype(std::declval<T>() + std::declval<T1>()), M, N>
    {
        auto result = Matrix<decltype(std::declval<T>() + std::declval<T1>()), M, N>();
        for (size_t i = 0; i < SIZE; ++i)
        {
            result(i) = data[i] + p_other(i);
        }
        return result;
    }

    template<typename T1>
    FORCE_INLINE Matrix<T, M, N>& operator +=(const Matrix<T1, M, N>& p_other)
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            this->data[i] += p_other(i);
        }
        return *this;
    }

    template<typename T1>
    FORCE_INLINE auto operator -(const Matrix<T1, M, N>& p_other) const
        -> Matrix<decltype(std::declval<T>() - std::declval<T1>()), M, N>
    {
        auto result = Matrix<decltype(std::declval<T>() - std::declval<T1>()), M, N>();
        for (size_t i = 0; i < SIZE; ++i)
        {
            result(i) = data[i] - p_other(i);
        }
        return result;
    }

    template<typename T1>
    FORCE_INLINE Matrix<T, M, N>& operator -=(const Matrix<T1, M, N>& p_other)
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            this->data[i] -= p_other(i);
        }
        return *this;
    }

    template<typename T1>
    FORCE_INLINE Matrix<T, M, N>& operator *=(const Matrix<T1, N, N>& p_other)
    {
        using result_val_type = decltype(std::declval<T>() * std::declval<T1>());
        auto result = Matrix<result_val_type, M, N>();
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                result[i][j] = result_val_type();
                for (size_t k = 0; k < N; ++k)
                {
                    result[i][j] += data2[i][k] * p_other[k][j];
                }
            }
        }
        *this = result;
        return *this;
    }

    template<typename T1>
    FORCE_INLINE auto operator*=(T1&& p_scaler)
        -> std::enable_if_t<!std::is_base_of_v<MathTypeBase, std::remove_reference_t<T1>>, 
            decltype(std::declval<T>() * std::declval<T1>(), *this)>
    {
        for (size_t i = 0; i < SIZE; ++i)
            data[i] *= std::forward<T1>(p_scaler);
        return *this;
    }

    operator std::string() const
    {
        std::stringstream ss;
        ss << "Matrix<" << typeid(T).name() << ", " << M << ", " << N << ">(";
        if constexpr (std::is_convertible_v<T, std::string>) {
            for (size_t i = 0; i < M; ++i)
            {
                ss << "(";
                for (size_t j = 0; j < N; ++j)
                {
                    ss << (std::string)data2[i][j];
                    if (j != N - 1)
                        ss << ", ";
                }
                ss << ")";
                if (i != M - 1)
                    ss << ", ";
            }
            ss << ")";
            return ss.str();
        } else {
            for (size_t i = 0; i < M; ++i)
            {
                ss << "(";
                for (size_t j = 0; j < N; ++j)
                {
                    ss << data2[i][j];
                    if (j != N - 1)
                        ss << ", ";
                }
                ss << ")";
                if (i != M - 1)
                    ss << ", ";
            }
            ss << ")";
            return ss.str();
        }
    }

    FORCE_INLINE const T* GetRaw() const
        { return data; }
    
    /**
     * @brief Get the translate matrix.
     * 
     * @param p_x The x displacement.
     * @param p_y The y displacement.
     * @param p_z The z displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Trans(float p_x, float p_y, float p_z) noexcept;
    
    /**
     * @brief Get the translate matrix.
     * 
     * @param p_transform The transformation displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Trans(const Vec4& p_transform) noexcept;

    /**
     * @brief Get the translate matrix.
     * 
     * @param p_transform The transformation displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Trans(const Vector<float, 3>& p_transform) noexcept;

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_x The scale of x.
     * @param p_y The scale of y.
     * @param p_z The scale of z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Scale(float p_x, float p_y, float p_z) noexcept;

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_scale The scale of x, y, z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Scale(const Vec4& p_scale) noexcept;

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_scale The scale of x, y, z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Scale(const Vector<float, 3>& p_scale) noexcept;
    
    /**
     * @brief Get the pitch rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Pitch(float p_angle) noexcept;

    /**
     * @brief Get the roll rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Roll(float p_angle) noexcept;

    /**
     * @brief Get the yaw rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix. 
     */
    FORCE_INLINE static Matrix<float, 4, 4> Yaw(float p_angle) noexcept;

    /**
     * @brief Get the rotation matrix with the order of pitch, roll, yaw.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularPRY(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of pitch, roll, yaw.
     * 
     * @param p_rot The rotation angles, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularPRY(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the order of pitch, yaw, roll.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularPYR(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of pitch, yaw, roll.
     * 
     * @param p_rot The rotation angles, ordered by pitch, yaw, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularPYR(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the order of roll, pitch, yaw.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularRPY(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of roll, pitch, yaw.
     * 
     * @param p_rot The rotation angles, ordered by roll, pitch, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularRPY(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the order of roll, yaw, pitch.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularRYP(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of roll, yaw, pitch.
     * 
     * @param p_rot The rotation angles, ordered by roll, yaw, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularRYP(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the order of yaw, pitch, roll.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularYPR(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of yaw, pitch, roll.
     * 
     * @param p_rot The rotation angles, ordered by yaw, pitch, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularYPR(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularYRP(float p_pitch, float p_roll, float p_yaw) noexcept;

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_rot The rotation angles, ordered by yaw, roll, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotEularYRP(const Vec4& p_rot) noexcept;

    /**
     * @brief Get the rotation matrix with the quaternion values.
     * 
     * @param p_quat The quaternion values. The first three elements should be the 
     * imaginary vector, and the last value should be the real space.
     * @return Matrix<float, 4, 4> The rotation matrix. 
     */
    FORCE_INLINE static Matrix<float, 4, 4> RotQuaternion(Vec4 p_quat) noexcept;

    /**
     * @brief Get the model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Model(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept;

    /**
     * @brief Get the model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> Model(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale) noexcept;

    /**
     * @brief Get the inverse model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> ModelInv(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept;

    /**
     * @brief Get the inverse model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> ModelInv(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale) noexcept;

    /**
     * @brief Get the view matrix with the order of rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The view matrix.
     */
    FORCE_INLINE static Matrix<float, 4, 4> View(const Vec4& p_translation, const Vec4& p_rotation) noexcept;

    FORCE_INLINE static Matrix<float, 4, 4> ProjOrtho(
        float p_right, 
        float p_left, 
        float p_top, 
        float p_bottom,
        float p_near,
        float p_far);
    
    FORCE_INLINE static Matrix<float, 4, 4> ProjPersp(
        float p_right, 
        float p_left, 
        float p_top, 
        float p_bottom,
        float p_near,
        float p_far);
};


template <size_t N>
using Mat = Matrix<float, N, N>;
using Mat4 = Mat<4>;