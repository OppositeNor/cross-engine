#pragma once

#include <iostream>

#include "ce/defs.hpp"

namespace CrossEngine::Math
{

    class MathTypeBase {};
    
    /**
     * @brief A math type.
     * 
     * @tparam T The type to check.
     */
    template<typename T>
    concept MathType = std::is_base_of<MathTypeBase, std::remove_reference_t<T>>::value;

    /**
     * @brief A non-math type
     * 
     * @tparam T The type to check
     */
    template<typename T>
    concept NMathType = !std::is_base_of<MathTypeBase, std::remove_reference_t<T>>::value;

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
}