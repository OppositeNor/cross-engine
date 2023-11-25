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

#define TO_CONST(p_val, T) (const_cast<const decltype(std::remove_reference_t(p_val))>(p_val))