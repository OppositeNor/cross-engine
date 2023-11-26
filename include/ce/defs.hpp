#pragma once
#include <string>
#include <stdexcept>

#define CE_DECL_HAS_METHOD(method, bool_var_name)       \
    template <typename dT, typename = void>             \
    inline static constexpr bool bool_var_name = false;        \
    template <typename dT>                              \
    inline static constexpr bool bool_var_name<dT, std::void_t<decltype(std::declval<dT>().method)>> = true;

#define CE_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

using byte_t = char;
using ubyte_t = unsigned char;
using int32_t = int;

#if __GNUC__
    #define FORCE_INLINE inline __attribute__((always_inline))
#elif defined _MSC_VER
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline
#endif

template <typename dT1, typename dT2, typename = void>
inline constexpr bool has_times = false;
template <typename dT1, typename dT2>
inline constexpr bool has_times<dT1, dT2, std::void_t<decltype(std::declval<dT1>() * std::declval<dT2>())>> = true;

enum class EulerRotOrder
{
    PRY = 1,
    PYR = 2,
    RPY = 3,
    RYP = -2,
    YPR = -3,
    YRP = -1
};

template <typename T>
FORCE_INLINE EulerRotOrder operator *(EulerRotOrder p_order, T&& p_value)
{
    return static_cast<EulerRotOrder>(static_cast<int>(p_order) * p_value);
}

template <typename T>
FORCE_INLINE EulerRotOrder operator *(T&& p_value, EulerRotOrder p_order)
{
    return static_cast<EulerRotOrder>(static_cast<int>(p_order) * p_value);
}