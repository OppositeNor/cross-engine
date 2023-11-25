#pragma once
#include "ce/math/matrix.hpp"
#include "ce/math/vector.hpp"

struct MatrixTag{};

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

template <typename Tm, typename Tv, size_t M, size_t N>
auto operator*(const Matrix<Tm, M, N>& p_matrix, const Vector<Tv, N>& p_vector)
    -> Vector<decltype(std::declval<Tm>() * std::declval<Tv>()), M>
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
    -> Vector<decltype(std::declval<Tm>() * std::declval<Tv>()), N>
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
    -> Matrix<decltype(std::declval<T1>() * std::declval<T2>()), M1, N1>
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

template <typename Tv1, typename Tv2, size_t N>
auto operator* (const Vector<Tv1, N>& p_vec1, const Vector<Tv2, N>& p_vec2)
    -> decltype(std::declval<Tv1>() * std::declval<Tv2>(), Vector<decltype(std::declval<Tv1>() * std::declval<Tv2>()), N>())
{
    auto result = Vector<decltype(std::declval<Tv1>() * std::declval<Tv2>()), N>();
    for (size_t i = 0; i < N; ++i)
        result[i] = p_vec1[i] * p_vec2[i];
    return result;
}

template<>
FORCE_INLINE Vec3 Vec3::Cross(const Vec3& p_vec1, const Vec3& p_vec2)
{
    return Vec3(p_vec1[2] * p_vec2[1] - p_vec1[1] * p_vec2[2],
                p_vec1[0] * p_vec2[2] - p_vec1[2] * p_vec2[0],
                p_vec1[1] * p_vec2[0] - p_vec1[0] * p_vec2[1]);
}

template<>
FORCE_INLINE Vec4 Vec4::Cross(const Vec4& p_vec1, const Vec4& p_vec2)
{
    return Vec4(p_vec1[2] * p_vec2[1] - p_vec1[1] * p_vec2[2],
                p_vec1[0] * p_vec2[2] - p_vec1[2] * p_vec2[0],
                p_vec1[1] * p_vec2[0] - p_vec1[0] * p_vec2[1],
                0);
}

template<>
FORCE_INLINE Mat4 Mat4::Trans(float p_x, float p_y, float p_z) noexcept
{
    return Mat4({
        1.0, 0.0, 0.0, p_x,
        0.0, 1.0, 0.0, p_y,
        0.0, 0.0, 1.0, p_z,
        0.0, 0.0, 0.0, 1.0
    });
}

template<>
FORCE_INLINE Mat4 Mat4::Trans(const Vec4& p_transform) noexcept
{
    return Trans(p_transform[0], p_transform[1], p_transform[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::Trans(const Vec3& p_transform) noexcept
{
    return Trans(p_transform[0], p_transform[1], p_transform[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::Scale(float p_x, float p_y, float p_z) noexcept
{
    return Mat4({
        p_x, 0.0, 0.0, 0.0,
        0.0, p_y, 0.0, 0.0,
        0.0, 0.0, p_z, 0.0,
        0.0, 0.0, 0.0, 1.0
    });
}

template<>
FORCE_INLINE Mat4 Mat4::Scale(const Vec4& p_scale) noexcept
{
    return Scale(p_scale[0], p_scale[1], p_scale[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::Scale(const Vec3& p_scale) noexcept
{
    return Scale(p_scale[0], p_scale[1], p_scale[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::Pitch(float p_angle) noexcept
{
    return Mat4({
        1.0, 0.0, 0.0, 0.0,
        0.0, std::cos(p_angle), std::sin(p_angle), 0.0,
        0.0, -std::sin(p_angle), std::cos(p_angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    });
}

template<>
FORCE_INLINE Mat4 Mat4::Roll(float p_angle) noexcept
{
    return Mat4({
        std::cos(p_angle), std::sin(p_angle), 0.0, 0.0,
        -std::sin(p_angle), std::cos(p_angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    });
}

template<>
FORCE_INLINE Mat4 Mat4::Yaw(float p_angle) noexcept
{
    return Mat4({
        std::cos(p_angle), 0.0, -std::sin(p_angle), 0.0,
        0.0, 1.0, 0.0, 0.0,
        std::sin(p_angle), 0.0, std::cos(p_angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    });
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularPRY(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Yaw(p_yaw) * Roll(p_roll) * Pitch(p_pitch);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularPRY(const Vec4& p_rot) noexcept
{
    return Yaw(p_rot[2]) * Roll(p_rot[1]) * Pitch(p_rot[0]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularPYR(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Roll(p_roll) * Yaw(p_yaw) * Pitch(p_pitch);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularPYR(const Vec4& p_rot) noexcept
{
    return Roll(p_rot[1]) * Yaw(p_rot[2]) * Pitch(p_rot[0]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularRPY(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Roll(p_roll) * Pitch(p_pitch) * Yaw(p_yaw);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularRPY(const Vec4& p_rot) noexcept
{
    return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularRYP(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Pitch(p_pitch) * Yaw(p_yaw) * Roll(p_roll);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularRYP(const Vec4& p_rot) noexcept
{
    return Pitch(p_rot[0]) * Yaw(p_rot[2]) * Roll(p_rot[1]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularYPR(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Roll(p_roll) * Pitch(p_pitch) * Yaw(p_yaw);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularYPR(const Vec4& p_rot) noexcept
{
    return Roll(p_rot[1]) * Pitch(p_rot[0]) * Yaw(p_rot[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularYRP(float p_pitch, float p_roll, float p_yaw) noexcept
{
    return Pitch(p_pitch) * Roll(p_roll) * Yaw(p_yaw);
}

template<>
FORCE_INLINE Mat4 Mat4::RotEularYRP(const Vec4& p_rot) noexcept
{
    return Pitch(p_rot[0]) * Roll(p_rot[1]) * Yaw(p_rot[2]);
}

template<>
FORCE_INLINE Mat4 Mat4::RotQuaternion(Vec4 p_quat) noexcept
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

template<>
FORCE_INLINE Mat4 Mat4::Model(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept
{
    return Trans(p_translation) * RotQuaternion(p_rotation) * Scale(p_scale);
}

template<>
FORCE_INLINE Mat4 Mat4::Model(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale) noexcept
{
    return Trans(p_translation) * Roll(p_rotation[1]) * Yaw(p_rotation[2]) * Pitch(p_rotation[0]) * Scale(p_scale);
}

template<>
FORCE_INLINE Mat4 Mat4::ModelInv(const Vec4& p_translation, const Vec4& p_rotation, const Vec4& p_scale) noexcept
{
    return Scale(-1 * p_scale) * RotQuaternion(Vec4{-p_rotation[0], -p_rotation[1], -p_rotation[2], p_rotation[3]}) 
        * Trans(-1 * p_translation);
}

template<>
FORCE_INLINE Mat4 Mat4::ModelInv(const Vec3& p_translation, const Vec3& p_rotation, const Vec3& p_scale) noexcept
{
    return Trans(p_translation) * Roll(p_rotation[1]) * Yaw(p_rotation[2]) * Pitch(p_rotation[0]) * Scale(p_scale);
}

template<>
FORCE_INLINE Mat4 Mat4::View(const Vec4& p_translation, const Vec4& p_rotation) noexcept
{
    return Trans(p_translation) * Roll(-p_rotation[1]) * Yaw(-p_rotation[2]) * Pitch(-p_rotation[0]);
}

template<>
FORCE_INLINE Mat4 Mat4::ProjOrtho(
        float p_right, 
        float p_left, 
        float p_top, 
        float p_bottom,
        float p_near,
        float p_far)
{
    return Mat4::Scale(2 / (p_right - p_left), 2 / (p_top - p_bottom), 2 / (p_far - p_near))
         * Mat4::Trans(-(p_right + p_left) / 2, -(p_top + p_bottom) / 2, -(p_near + p_far) / 2);
}

template<>
FORCE_INLINE Mat4 Mat4::ProjPersp(
        float p_right, 
        float p_left, 
        float p_top, 
        float p_bottom,
        float p_near,
        float p_far)
{
    return Mat4::ProjOrtho(p_right, p_left, p_top, p_bottom, p_near, p_far) * 
        Mat4({p_near, 0, 0, 0,
              0, p_near, 0, 0,
              0, 0, p_near + p_far, -p_near * p_far,
              0, 0, 1.0f, 0});
}

template <typename Tr, typename T1, typename T2>
FORCE_INLINE auto Lerp(Tr&& p_ratio, T1&& p_start, T2&& p_end)
    -> decltype(std::declval<std::remove_reference_t<T1>>()
         + std::declval<std::remove_reference_t<Tr>>() * 
         (std::declval<std::remove_reference_t<T2>>() - 
         std::declval<std::remove_reference_t<T1>>()))
{
    return std::forward<T1>(p_start) + std::forward<Tr>(p_ratio) 
        * (std::forward<T2>(p_end) - std::forward<T1>(p_start));
}