#pragma once
#include "ce/math/matrix.hpp"
#include "ce/math/vector.hpp"

namespace Math
{
    struct MatrixTag{};

    template<int N>
    inline const Vec<N> UP = Vec<N>(0, 1);

    template<int N>
    inline const Vec<N> RIGHT = Vec<N>(1, 0);

    template<int N>
    inline const Vec<N> FRONT = Vec<N>(0, 0, 1);

    template<int N>
    inline const Vec<N> BACK = Vec<N>(0, 0, -1);
    
    template<typename T, int N>
    inline static const Vector<T, N> ZERO = Vector<T, N>();

    template <typename Tm, size_t M, size_t N, typename Ts>
    auto operator*(const Matrix<Tm, M, N>& p_mat, Ts&& p_scaler)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Matrix<decltype(std::declval<Tm>() * std::declval<Ts>()), M, N>>
    {
        using result_val_type = decltype(std::declval<Tm>() * std::declval<Ts>());
        constexpr size_t SIZE = M * N;
        Matrix<result_val_type, M, N> result;
        for (size_t i = 0; i < SIZE; ++i)
            result(i) = p_mat(i) * std::forward<Ts>(p_scaler);
        return result;
    }

    template <typename Tm, size_t M, size_t N, typename Ts>
    auto operator*(Ts&& p_scaler, const Matrix<Tm, M, N>& p_mat)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Matrix<decltype(std::declval<Ts>() * std::declval<Tm>()), M, N>>
    {
        using result_val_type = decltype(std::declval<Ts>() * std::declval<Tm>());
        constexpr size_t SIZE = M * N;
        Matrix<result_val_type, M, N> result;
        for (size_t i = 0; i < SIZE; ++i)
            result(i) = std::forward<Ts>(p_scaler) * p_mat(i);
        return result;
    }

    template <typename Tm, size_t M, size_t N, typename Ts>
    auto operator/(const Matrix<Tm, M, N>& p_mat, Ts&& p_scaler)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Matrix<decltype(std::declval<Tm>() / std::declval<Ts>()), M, N>>
    {
        using result_val_type = decltype(std::declval<Tm>() / std::declval<Ts>());
        constexpr size_t SIZE = M * N;
        Matrix<result_val_type, M, N> result;
        for (size_t i = 0; i < SIZE; ++i)
            result(i) = p_mat(i) / std::forward<Ts>(p_scaler);
        return result;
    }

    template <typename Tm, typename Tv, size_t M, size_t N>
    auto operator*(const Matrix<Tm, M, N>& p_matrix, const Vector<Tv, N>& p_vector)
    {
        Vector<decltype(std::declval<Tm>() * std::declval<Tv>()), M> result;
        for (size_t i = 0; i < M; ++i)
        {
            result[i] = 0;
            for (size_t j = 0; j < N; ++j)
            {
                result[i] += p_matrix[i][j] * p_vector[j];
            }
        }
        return result;
    }

    template <typename Tm, typename Tv, size_t M, size_t N>
    auto operator*(const Vector<Tv, M>& p_vector, const Matrix<Tm, M, N>& p_matrix)
    {
        Vector<decltype(std::declval<Tm>() * std::declval<Tv>()), N> result;
        for (size_t i = 0; i < N; ++i)
        {
            result[i] = 0;
            for (size_t j = 0; j < M; ++j)
            {
                result[i] += p_vector[j] * p_matrix[j][i];
            }
        }
        return result;
    }

    template <typename T1, typename T2, size_t M1, size_t N, size_t N1>
    auto operator*(const Matrix<T1, M1, N>& p_mat1, const Matrix<T2, N, N1>& p_mat2)
    {
        using result_val_type = decltype(std::declval<T1>() * std::declval<T2>());
        auto result = Matrix<result_val_type, M1, N1>();
        
        for (size_t i = 0; i < M1; ++i)
        {
            for (size_t j = 0; j < N1; ++j)
            {
                result[i][j] = result_val_type();
                for (size_t k = 0; k < N; ++k)
                {
                    result[i][j] += p_mat1[i][k] * p_mat2[k][j];
                }
            }
        }
        return result;
    }

    template <typename Tv, size_t N, typename Ts>
    auto operator*(const Vector<Tv, N>& p_vec, Ts&& p_scaler)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Vector<decltype(std::declval<Tv>() * std::declval<Ts>()), N>>
    {
        auto result = Vector<decltype(std::declval<Tv>() * std::declval<Ts>()), N>();
        for (size_t i = 0; i < N; ++i)
            result[i] = p_vec[i] * std::forward<Ts>(p_scaler);
        return result;
    }

    template <typename Tv, size_t N, typename Ts>
    auto operator*(Ts&& p_scaler, const Vector<Tv, N>& p_vec)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Vector<decltype(std::declval<Ts>() * std::declval<Tv>()), N>>
    {
        auto result = Vector<decltype(std::declval<Ts>() * std::declval<Tv>()), N>();
        for (size_t i = 0; i < N; ++i)
            result[i] = std::forward<Ts>(p_scaler) * p_vec[i];
        return result;
    }

    template <typename Tv, size_t N, typename Ts>
    auto operator/(const Vector<Tv, N>& p_vec, Ts&& p_scaler)
        -> std::enable_if_t<!std::is_base_of<MathTypeBase, 
            std::remove_reference_t<Ts>>::value, 
            Vector<decltype(std::declval<Tv>() / std::declval<Ts>()), N>>
    {
        auto result = Vector<decltype(std::declval<Tv>() / std::declval<Ts>()), N>();
        for (size_t i = 0; i < N; ++i)
            result[i] = p_vec[i] / std::forward<Ts>(p_scaler);
        return result;
    }

    template <typename Tv1, typename Tv2, size_t N>
    auto operator* (const Vector<Tv1, N>& p_vec1, const Vector<Tv2, N>& p_vec2)
        -> decltype(std::declval<Tv1>() * std::declval<Tv2>(), Vector<decltype(std::declval<Tv1>() * std::declval<Tv2>()), N>())
    {
        auto result = Vector<decltype(std::declval<Tv1>() * std::declval<Tv2>()), N>();
        for (size_t i = 0; i < N; ++i)
            result[i] = p_vec1[i] * p_vec2[i];
        return result;
    }

    template <typename T, size_t M, size_t N>
    auto operator <<(std::ostream& p_os, const Matrix<T, M, N>& p_mat)
        -> std::enable_if_t<std::convertible_to<Matrix<T, M, N>, std::string>, std::ostream&>
    {
        return p_os << (std::string)p_mat;
    }

    template <typename T, size_t N>
    auto operator <<(std::ostream& p_os, const Vector<T, N>& p_vec)
        -> std::enable_if_t<std::convertible_to<Vector<T, N>, std::string>, std::ostream&>
    {
        return p_os << (std::string)p_vec;
    }

    template<typename T, size_t N>
    FORCE_INLINE auto Dot(const Vector<T, N> &p_vec1, const Vector<T, N> &p_vec2)
    {
        return p_vec1.Dot(p_vec2);
    }

    /**
     * @brief Cross product of v1 and v2.
     * 
     * @param p_vec1 v1.
     * @param p_vec2 v2.
     * @return Vec3 The result of the cross product.
     */
    FORCE_INLINE Vec3 Cross(const Vec3& p_vec1, const Vec3& p_vec2)
    {
        return Vec3(p_vec1[2] * p_vec2[1] - p_vec1[1] * p_vec2[2],
                    p_vec1[0] * p_vec2[2] - p_vec1[2] * p_vec2[0],
                    p_vec1[1] * p_vec2[0] - p_vec1[0] * p_vec2[1]);
    }

    /**
     * @brief Cross product of v1 and v2.
     * 
     * @param p_vec1 v1.
     * @param p_vec2 v2.
     * @return Vec4 The result of the cross product.
     */
    FORCE_INLINE Vec4 Cross(const Vec4& p_vec1, const Vec4& p_vec2)
    {
        return Vec4(p_vec1[2] * p_vec2[1] - p_vec1[1] * p_vec2[2],
                    p_vec1[0] * p_vec2[2] - p_vec1[2] * p_vec2[0],
                    p_vec1[1] * p_vec2[0] - p_vec1[0] * p_vec2[1],
                    0);
    }

    /**
     * @brief Get the translate matrix.
     * 
     * @param p_x The x displacement.
     * @param p_y The y displacement.
     * @param p_z The z displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE Mat4 Trans(float p_x, float p_y, float p_z) noexcept
    {
        return Mat4({
            1.0, 0.0, 0.0, p_x,
            0.0, 1.0, 0.0, p_y,
            0.0, 0.0, 1.0, p_z,
            0.0, 0.0, 0.0, 1.0
        });
    }

    /**
     * @brief Get the translate matrix.
     * 
     * @param p_transform The transformation displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE Mat4 Trans(const Vec4& p_transform) noexcept
    {
        return Trans(p_transform[0], p_transform[1], p_transform[2]);
    }

    /**
     * @brief Get the translate matrix.
     * 
     * @param p_transform The transformation displacement.
     * @return Matrix<float, 4, 4> The transform matrix.
     */
    FORCE_INLINE Mat4 Trans(const Vec3& p_transform) noexcept
    {
        return Trans(p_transform[0], p_transform[1], p_transform[2]);
    }

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_x The scale of x.
     * @param p_y The scale of y.
     * @param p_z The scale of z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE Mat4 Scale(float p_x, float p_y, float p_z) noexcept
    {
        return Mat4({
            p_x, 0.0, 0.0, 0.0,
            0.0, p_y, 0.0, 0.0,
            0.0, 0.0, p_z, 0.0,
            0.0, 0.0, 0.0, 1.0
        });
    }

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_scale The scale of x, y, z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE Mat4 Scale(const Vec4& p_scale) noexcept
    {
        return Scale(p_scale[0], p_scale[1], p_scale[2]);
    }

    /**
     * @brief Get the scale matrix.
     * 
     * @param p_scale The scale of x, y, z.
     * @return Matrix<float, 4, 4> The scale matrix.
     */
    FORCE_INLINE Mat4 Scale(const Vec3& p_scale) noexcept
    {
        return Scale(p_scale[0], p_scale[1], p_scale[2]);
    }

    /**
     * @brief Get the pitch rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 Pitch(float p_angle) noexcept
    {
        return Mat4({
            1.0, 0.0, 0.0, 0.0,
            0.0, std::cos(p_angle), std::sin(p_angle), 0.0,
            0.0, -std::sin(p_angle), std::cos(p_angle), 0.0,
            0.0, 0.0, 0.0, 1.0
        });
    }

    /**
     * @brief Get the roll rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 Roll(float p_angle) noexcept
    {
        return Mat4({
            std::cos(p_angle), std::sin(p_angle), 0.0, 0.0,
            -std::sin(p_angle), std::cos(p_angle), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        });
    }

    /**
     * @brief Get the yaw rotation matrix.
     * 
     * @param p_angle The angle of rotation.
     * @return Matrix<float, 4, 4> The rotation matrix. 
     */
    FORCE_INLINE Mat4 Yaw(float p_angle) noexcept
    {
        return Mat4({
            std::cos(p_angle), 0.0, -std::sin(p_angle), 0.0,
            0.0, 1.0, 0.0, 0.0,
            std::sin(p_angle), 0.0, std::cos(p_angle), 0.0,
            0.0, 0.0, 0.0, 1.0
        });
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, roll, yaw.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPRY(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Yaw(p_yaw) * Roll(p_roll) * Pitch(p_pitch);
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, roll, yaw.
     * 
     * @param p_rot The rotation angles, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPRY(const Vec4& p_rot) noexcept
    {
        return Yaw(p_rot[2]) * Roll(p_rot[1]) * Pitch(p_rot[0]);
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, roll, yaw.
     * 
     * @param p_rot The rotation angles, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPRY(const Vec3& p_rot) noexcept
    {
        return Yaw(p_rot[2]) * Roll(p_rot[1]) * Pitch(p_rot[0]);
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, yaw, roll.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPYR(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Roll(p_roll) * Yaw(p_yaw) * Pitch(p_pitch);
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, yaw, roll.
     * 
     * @param p_rot The rotation angles, ordered by pitch, yaw, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPYR(const Vec4& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Yaw(p_rot[2]) * Pitch(p_rot[0]);
    }

    /**
     * @brief Get the rotation matrix with the order of pitch, yaw, roll.
     * 
     * @param p_rot The rotation angles, ordered by pitch, yaw, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularPYR(const Vec3& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Yaw(p_rot[2]) * Pitch(p_rot[0]);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, pitch, yaw.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRPY(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Roll(p_roll) * Pitch(p_pitch) * Yaw(p_yaw);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, pitch, yaw.
     * 
     * @param p_rot The rotation angles, ordered by roll, pitch, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRPY(const Vec4& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, pitch, yaw.
     * 
     * @param p_rot The rotation angles, ordered by roll, pitch, yaw.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRPY(const Vec3& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, yaw, pitch.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRYP(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Pitch(p_pitch) * Yaw(p_yaw) * Roll(p_roll);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, yaw, pitch.
     * 
     * @param p_rot The rotation angles, ordered by roll, yaw, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRYP(const Vec4& p_rot) noexcept
    {
        return Pitch(p_rot[0]) * Yaw(p_rot[2]) * Roll(p_rot[1]);
    }

    /**
     * @brief Get the rotation matrix with the order of roll, yaw, pitch.
     * 
     * @param p_rot The rotation angles, ordered by roll, yaw, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularRYP(const Vec3& p_rot) noexcept
    {
        return Pitch(p_rot[0]) * Yaw(p_rot[2]) * Roll(p_rot[1]);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, pitch, roll.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYPR(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Roll(p_roll) * Pitch(p_pitch) * Yaw(p_yaw);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, pitch, roll.
     * 
     * @param p_rot The rotation angles, ordered by yaw, pitch, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYPR(const Vec4& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, pitch, roll.
     * 
     * @param p_rot The rotation angles, ordered by yaw, pitch, roll.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYPR(const Vec3& p_rot) noexcept
    {
        return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYRP(float p_pitch, float p_roll, float p_yaw) noexcept
    {
        return Pitch(p_pitch) * Roll(p_roll) * Yaw(p_yaw);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_rot The rotation angles, ordered by yaw, roll, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYRP(const Vec4& p_rot) noexcept
    {
        return Pitch(p_rot[0]) * Roll(p_rot[1]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_rot The rotation angles, ordered by yaw, roll, pitch.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEularYRP(const Vec3& p_rot) noexcept
    {
        return Pitch(p_rot[0]) * Roll(p_rot[1]) * Yaw(p_rot[2]);
    }

    /**
     * @brief Get the rotation matrix.
     * 
     * @param p_pitch The pitch angle.
     * @param p_roll The roll angle.
     * @param p_yaw The yaw angle.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEular(float p_pitch, float p_roll, float p_yaw, EulerRotOrder p_order)
    {
        switch (p_order)
        {
        case EulerRotOrder::PRY:
            return RotEularPRY(p_pitch, p_roll, p_yaw);
        case EulerRotOrder::PYR:
            return RotEularPYR(p_pitch, p_roll, p_yaw);
        case EulerRotOrder::RPY:
            return RotEularRPY(p_pitch, p_roll, p_yaw);
        case EulerRotOrder::RYP:
            return RotEularRYP(p_pitch, p_roll, p_yaw);
        case EulerRotOrder::YPR:
            return RotEularYPR(p_pitch, p_roll, p_yaw);
        case EulerRotOrder::YRP:
            return RotEularYRP(p_pitch, p_roll, p_yaw);
        default:
            throw std::invalid_argument("Invalid rotation order.");
        }
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_rot The rotation angles.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEular(const Vec4& p_rot, EulerRotOrder p_order)
    {
        switch (p_order)
        {
        case EulerRotOrder::PRY:
            return RotEularPRY(p_rot);
        case EulerRotOrder::PYR:
            return RotEularPYR(p_rot);
        case EulerRotOrder::RPY:
            return RotEularRPY(p_rot);
        case EulerRotOrder::RYP:
            return RotEularRYP(p_rot);
        case EulerRotOrder::YPR:
            return RotEularYPR(p_rot);
        case EulerRotOrder::YRP:
            return RotEularYRP(p_rot);
        default:
            throw std::invalid_argument("Invalid rotation order.");
        }
    }

    /**
     * @brief Get the rotation matrix with the order of yaw, roll, pitch.
     * 
     * @param p_rot The rotation angles.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The rotation matrix.
     */
    FORCE_INLINE Mat4 RotEular(const Vec3& p_rot, EulerRotOrder p_order)
    {
        switch (p_order)
        {
        case EulerRotOrder::PRY:
            return RotEularPRY(p_rot);
        case EulerRotOrder::PYR:
            return RotEularPYR(p_rot);
        case EulerRotOrder::RPY:
            return RotEularRPY(p_rot);
        case EulerRotOrder::RYP:
            return RotEularRYP(p_rot);
        case EulerRotOrder::YPR:
            return RotEularYPR(p_rot);
        case EulerRotOrder::YRP:
            return RotEularYRP(p_rot);
        default:
            throw std::invalid_argument("Invalid rotation order.");
        }
    }

    /**
     * @brief Get the rotation matrix with the quaternion values.
     * 
     * @param p_quat The quaternion values. The first three elements should be the 
     * imaginary vector, and the last value should be the real space.
     * @return Matrix<float, 4, 4> The rotation matrix. 
     */
    FORCE_INLINE Mat4 RotQuaternion(Vec4 p_quat) noexcept
    {
        p_quat.Normalize();
        auto result = Mat4({
            p_quat[3], p_quat[2], -p_quat[1], -p_quat[0],
            -p_quat[2], p_quat[3], p_quat[0], -p_quat[1],
            p_quat[1], -p_quat[0], p_quat[3], -p_quat[2],
            p_quat[0], p_quat[1], p_quat[2], p_quat[3]
        }) * Mat4({
            p_quat[3], p_quat[2], -p_quat[1], p_quat[0],
            -p_quat[2], p_quat[3], p_quat[0], p_quat[1],
            p_quat[1], -p_quat[0], p_quat[3], p_quat[2],
            -p_quat[0], -p_quat[1], -p_quat[2], p_quat[3],
        });
        return result;
    }

    /**
     * @brief Get the model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The quaternion rotation of the transform.
     * @param p_scale The scale of the transform.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 Model(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept
    {
        return Trans(p_translation) * RotQuaternion(p_rotation) * Scale(p_scale);
    }

    /**
     * @brief Get the model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The Eular rotation of the transform.
     * @param p_scale The scale of the transform.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 Model(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale,EulerRotOrder p_order)
    {
        return Trans(p_translation) * RotEular(p_rotation, p_order) * Scale(p_scale);
    }

    /**
     * @brief Get the model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The Eular rotation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 Model(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale, EulerRotOrder p_order)
    {
        return Trans(p_translation) * RotEular(p_rotation, p_order) * Scale(p_scale);
    }

    /**
     * @brief Get the inverse model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_rotation The quaternion rotation of the transform.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 ModelInv(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept
    {
        return Scale(-1 * p_scale) * RotQuaternion(Vec4{-p_rotation[0], -p_rotation[1], -p_rotation[2], p_rotation[3]}) 
            * Trans(-1 * p_translation);
    }

    /**
     * @brief Get the inverse model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The Euler rotation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 ModelInv(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale, EulerRotOrder p_order)
    {
        return Scale(-1 * p_scale) * RotEular(-1 * p_rotation, -1 * p_order) * Trans(-1 * p_translation);
    }

    /**
     * @brief Get the inverse model matrix with the order of scale, rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The Euler rotation of the transform.
     * @param p_scale The scale of the transform.
     * @param p_order The order of the eular rotation.
     * @return Matrix<float, 4, 4> The model matrix.
     */
    FORCE_INLINE Mat4 ModelInv(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale, EulerRotOrder p_order)
    {
        return Scale(-1 * p_scale) * RotEular(-1 * p_rotation, -1 * p_order) * Trans(-1 * p_translation);
    }

    /**
     * @brief Get the view matrix with the order of rotation, translation.
     * 
     * @param p_translation The translation of the transform.
     * @param p_rotation The rotation of the transform, ordered by pitch, roll, yaw.
     * @return Matrix<float, 4, 4> The view matrix.
     */
    FORCE_INLINE Mat4 View(const Vec4& p_translation, const Vec4& p_rotation) noexcept
    {
        return RotQuaternion(Vec4(-p_rotation[0], -p_rotation[1], -p_rotation[2], p_rotation[3])) * Trans(-1 * p_translation);
    }

    FORCE_INLINE Mat4 ProjOrtho(
            float p_right, 
            float p_left, 
            float p_top, 
            float p_bottom,
            float p_near,
            float p_far)
    {
        return Scale(2 / (p_right - p_left), 2 / (p_top - p_bottom), 2 / (p_far - p_near))
            * Trans(-(p_right + p_left) / 2, -(p_top + p_bottom) / 2, -(p_near + p_far) / 2);
    }

    FORCE_INLINE Mat4 ProjPersp(
            float p_right, 
            float p_left, 
            float p_top, 
            float p_bottom,
            float p_near,
            float p_far)
    {
        return ProjOrtho(p_right, p_left, p_top, p_bottom, p_near, p_far) * 
            Mat4({p_near, 0, 0, 0,
                0, p_near, 0, 0,
                0, 0, p_near + p_far, -p_near * p_far,
                0, 0, 1.0f, 0});
    }

    template <typename Tr, typename T1, typename T2>
    FORCE_INLINE auto Lerp(Tr&& p_ratio, T1&& p_start, T2&& p_end)
    {
        return std::forward<T1>(p_start) + std::forward<Tr>(p_ratio) 
            * (std::forward<T2>(p_end) - std::forward<T1>(p_start));
    }

    /**
     * @brief Product of quaternions. The order of the quaternions is from right to left.
     * The real part of the quaternion is the last element.
     * 
     * @param p_quat1 The first quaternion.
     * @param p_quats The rest of the quaternions.
     * @return Vec4 The product of the quaternions. 
     */
    template <typename ...Args>
    FORCE_INLINE Vec4 QuatProd(const Vec4& p_quat1, Args&&... p_quats)
    {
        return Mat4({
            p_quat1[3], p_quat1[2], -p_quat1[1], p_quat1[0],
            -p_quat1[2], p_quat1[3], p_quat1[0], p_quat1[1],
            p_quat1[1], -p_quat1[0], p_quat1[3], p_quat1[2],
            -p_quat1[0], -p_quat1[1], -p_quat1[2], p_quat1[3],
        }) * QuatProd(std::forward<Args>(p_quats)...);
    }

    /**
     * @brief Product of quaternions. The order of the quaternions is from right to left.
     * The real part of the quaternion is the last element.
     * 
     * @param p_quat1 The first quaternion.
     */
    FORCE_INLINE Vec4 QuatProd(const Vec4& p_quat1)
    {
        return p_quat1;
    }

    /**
     * @brief Convert Euler angles to quaternion.
     * 
     * @param p_rotation The Euler angles.
     * @param p_order The rotation order.
     * @return Vec4 The result quaternion.
     */
    FORCE_INLINE Vec4 EulerToQuat(const Vec4& p_rotation, EulerRotOrder p_order)
    {
            switch (p_order)
        {
        case EulerRotOrder::PRY:
            return QuatProd(
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)})
            );
            break;
        case EulerRotOrder::PYR:
            return QuatProd(
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)})
            );
            break;
        case EulerRotOrder::RPY:
            return QuatProd(
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)})
            );
            break;
        case EulerRotOrder::RYP:
            return QuatProd(
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)})
            );
            break;
        case EulerRotOrder::YPR:
            return QuatProd(
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)})
            );
            break;
        case EulerRotOrder::YRP:
            return QuatProd(
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)})
            );
            break;
        default:
            throw std::invalid_argument("Invalid rotation order.");
        }
    }

    /**
     * @brief Convert Euler angles to quaternion.
     * 
     * @param p_rotation The Euler angles.
     * @param p_order The rotation order.
     * @return Vec4 The result quaternion.
     */
    FORCE_INLINE Vec4 EulerToQuat(const Vec3& p_rotation, EulerRotOrder p_order)
    {
            switch (p_order)
        {
        case EulerRotOrder::PRY:
            return QuatProd(
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)})
            );
            break;
        case EulerRotOrder::PYR:
            return QuatProd(
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)})
            );
            break;
        case EulerRotOrder::RPY:
            return QuatProd(
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)})
            );
            break;
        case EulerRotOrder::RYP:
            return QuatProd(
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)})
            );
            break;
        case EulerRotOrder::YPR:
            return QuatProd(
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)})
            );
            break;
        case EulerRotOrder::YRP:
            return QuatProd(
                Vec4({0.0f, 0.0f, std::sin(p_rotation[2] / 2), std::cos(p_rotation[2] / 2)}),
                Vec4({0.0f, std::sin(p_rotation[1] / 2), 0.0f, std::cos(p_rotation[1] / 2)}),
                Vec4({std::sin(p_rotation[0] / 2), 0.0f, 0.0f, std::cos(p_rotation[0] / 2)})
            );
            break;
        default:
            throw std::invalid_argument("Invalid rotation order.");
        }
    }

    FORCE_INLINE Mat4 LookAt(const Vec4& p_from, const Vec4& p_target)
    {
        auto forward = (p_target - p_from).Normalize();
        auto right = forward.Cross(UP<4>).Normalize();
        auto up = forward.Cross(right).Normalize();
        return Mat4({
            right[0], right[1], right[2], -right.Dot(p_from),
            up[0], up[1], up[2], -up.Dot(p_from),
            forward[0], forward[1], forward[2], -forward.Dot(p_from),
            0, 0, 0, 1
        });
    }

    /**
     * @brief Reflect the vector across a normal vector.
     * 
     * @tparam T The type of the vectors.
     * @tparam N The dimension of the vectors.
     * @param p_vec The vector to get reflected.
     * @param p_norm The normal vector.
     * @return The reflected vector.
     */
    template<typename T, size_t N>
    FORCE_INLINE auto Reflect(const Vector<T, N>& p_vec, const Vector<T, N>& p_norm)
    {
        auto norm = p_norm.Normalized();
        return p_vec - 2 * Dot(p_vec, norm) * norm;
    }

    /**
     * @brief Refract the vector across a normal vector.
     * 
     * @tparam T The type of the vectors.
     * @tparam N The dimension of the vectors.
     * @param p_vec The vector to get refracted.
     * @param p_norm The normal vector.
     * @param p_eta The refractive index of the material that the vector is going inward.
     * @param p_eta_prime The refractive index of the material that the vector is going outward.
     * @return The refracted vector.
     */
    template<typename T, size_t N>
    FORCE_INLINE auto Refract(const Vector<T, N>& p_vec, const Vector<T, N>& p_norm, double p_eta, double p_eta_prime)
    {
        return Refract(p_vec, p_norm, p_eta / p_eta_prime);
    }

    /**
     * @brief Refract the vector across a normal vector.
     * 
     * @tparam T The type of the vectors.
     * @tparam N The dimension of the vectors.
     * @param p_vec The vector to get refracted.
     * @param p_norm The normal vector.
     * @param p_eta_fraction The fraction of the refractive index going inward 
     * over the refractive index going outward.
     * @return The refracted vector.
     */
    template<typename T, size_t N>
    FORCE_INLINE auto Refract(const Vector<T, N>& p_vec, const Vector<T, N>& p_norm, double p_eta_fraction)
    {
        auto perp = p_eta_fraction * (p_vec + Math::Dot(p_vec, p_norm) * p_norm);
        auto parallel = -sqrt(abs(1.0 - perp.LengthSquared())) * p_norm;
        return perp + parallel;
    }

    constexpr double PI = 3.14159265358979323846;
}