#include "ce/math/math_type_base.hpp"
#include <utility>
#include <stdexcept>

namespace Math
{
    /**
     * @brief An interval object.
     * 
     * @tparam T The type of the interval elements.
     */
    template<typename T>
    class Interval : public MathTypeBase {
    private:
        T min_val;
        T max_val;

    public:

        /**
         * @brief Construct a new Interval object
         * 
         * @param p_min The minimum of the interval.
         * @param p_max The maximum of the interval.
         */
        Interval(const T& p_min, const T& p_max)
            : min_val(p_min), max_val(p_max) {}

        /**
         * @brief Get the minimum of the interval
         * 
         * @return const T& The minimum of the interval. 
         */
        FORCE_INLINE const T& GetMin() { return min_val; }

        /**
         * @brief Get the maximum of the interval
         * 
         * @return const T& The maximum of the interval. 
         */
        FORCE_INLINE const T& GetMax() { return max_val; }
        
        /**
         * @brief Test weather the interval contains a value.
         * 
         * @tparam T1 The type of the value to check.
         * @param p_value The value to test on.
         * @return true The interval contains the value.
         * @return false The interval does not contain the value.
         */
        template<typename T1>
        FORCE_INLINE bool Contains(T1&& p_value)
        {
            return (std::forward(p_value) >= min_val) && (std::forward(p_value) <= max_val);
        }

        /**
         * @brief Test weather the interval surrounds a value.
         * 
         * @tparam T1 The type of the value to check.
         * @param p_value The value to test on.
         * @return true The interval surrounds the value.
         * @return false The interval does not surrounds the value.
         */
        template<typename T1>
        FORCE_INLINE bool Surrounds(T1&& p_value)
        {
            return (std::forward(p_value) > min_val) && (std::forward(p_value) < max_val);
        }

        /**
         * @brief Get the size of the interval.
         * 
         * @return The size of the interval.
         */
        FORCE_INLINE auto Size()
        {
            return max_val - min_val;
        }

        /**
         * @brief Set the minimum of the interval
         * 
         * @param p_min The minimum to be set to.
         */
        FORCE_INLINE void SetMin(const T& p_min)
        {
            min_val = p_min;
        }

        /**
         * @brief Set the minimum of the interval
         * 
         * @param p_min The minimum to be set to.
         */
        FORCE_INLINE void SetMax(const T& p_max)
        {
            max_val = p_max;
        }

        /**
         * @brief Is the interval empty.
         * 
         * @return true The interval is empty.
         * @return false The interval is not empty.
         */
        FORCE_INLINE bool IsEmpty()
        {
            return max_val < min_val;
        }

        /**
         * @brief Find the union of this interval and another interval. 
         * @warning The result will be a interval with the minimum of 
         * this and the argument to the maximum. So if you have two
         * intervals that don't intersect, the result interval may contains
         * part that both intervals don't contain.
         * 
         * @param p_other The other interval.
         * @return Interval<T> The union interval.
         */
        FORCE_INLINE Interval<T> Union(const Interval<T>& p_other)
        {
            return Interval<T>(std::min(min_val, p_other.min_val), std::max(max_val, p_other.max_val));
        }

        /**
         * @brief Find the intersect of this interval and another interval.
         * 
         * @param p_other The other interval.
         * @return Interval<T> The intersect interval.
         */
        FORCE_INLINE Interval<T> Intersect(const Interval<T>& p_other)
        {
            return Interval<T>(std::max(min_val, p_other.min_val), std::min(max_val, p_other.max_val));
        }

        /**
         * @brief Returns the clamp of a value. This function won't change the
         * original value.
         * 
         * @param p_value The value to be clamped
         * @return T The clamped value.
         */
        FORCE_INLINE T Clamped(const T& p_value)
        {
            if (p_value < min_val) return min_val;
            if (p_value > max_val) return max_val;
            return p_value;
        }

        /**
         * @brief Clamp a value. This function will change the original value 
         * been passed in as the argument.
         * 
         * @param p_value The value to be clamped.
         * @return T& The reference of the value.
         */
        FORCE_INLINE T& Clamp(T& p_value)
        {
            if (p_value < min_val) p_value = min_val;
            else if (p_value > max_val) p_value = max_val;
            return p_value;
        }
    };
}