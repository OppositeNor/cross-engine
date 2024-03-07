#pragma once
#include <string>
#include <stdexcept>

#define CE_DECL_HAS_METHOD(method, bool_var_name)       \
    template <typename dT, typename = void>             \
    inline static constexpr bool bool_var_name = false;        \
    template <typename dT>                              \
    inline static constexpr bool bool_var_name<dT, std::void_t<decltype(std::declval<dT>().method)>> = true;

#if __GNUC__
    #define FORCE_INLINE inline __attribute__((always_inline))
#elif defined _MSC_VER
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline
#endif

namespace CrossEngine
{
    using byte_t = char;
    using ubyte_t = unsigned char;
    using int32_t = int;

    using real_t = float;
}