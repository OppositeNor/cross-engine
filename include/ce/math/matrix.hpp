#pragma once
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "ce/defs.hpp"
#include "ce/math/math_type_base.hpp"
#include "ce/math/vector.hpp"

namespace CrossEngine::Math
{
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
    private:
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

        template <typename T1>
        FORCE_INLINE bool operator ==(const Matrix<T1, M, N>& p_other) const
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                if (!(data[i] == p_other(i)))
                    return false;
            }
            return true;
        }

        template <typename T1>
        FORCE_INLINE bool operator !=(const Matrix<T1, M, N>& p_other) const
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                if (data[i] != p_other(i))
                    return true;
            }
            return false;
        }

        template <typename T1>
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

        template <typename T1>
        FORCE_INLINE Matrix<T, M, N>& operator +=(const Matrix<T1, M, N>& p_other)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                this->data[i] += p_other(i);
            }
            return *this;
        }

        template <typename T1>
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

        template <typename T1>
        FORCE_INLINE Matrix<T, M, N>& operator -=(const Matrix<T1, M, N>& p_other)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                this->data[i] -= p_other(i);
            }
            return *this;
        }

        template <typename T1>
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

        template <typename T1>
        FORCE_INLINE auto operator*=(T1&& p_scaler)
            -> std::enable_if_t<!std::is_base_of_v<MathTypeBase, std::remove_reference_t<T1>>, 
                decltype(std::declval<T>() * std::declval<T1>(), *this)>
        {
            for (size_t i = 0; i < SIZE; ++i)
                data[i] *= std::forward<T1>(p_scaler);
            return *this;
        }

        template <typename T1>
        FORCE_INLINE auto operator /=(T1&& p_scaler)
            -> std::enable_if_t<!std::is_base_of_v<MathTypeBase, std::remove_reference_t<T1>>, 
                decltype(std::declval<T>() / std::declval<T1>(), *this)>
        {
            for (size_t i = 0; i < SIZE; ++i)
                data[i] /= std::forward<T1>(p_scaler);
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
        
    };


    template <size_t N>
    using Mat = Matrix<float, N, N>;
    using Mat3 = Mat<3>;
    using Mat4 = Mat<4>;
}
