#pragma once
#include "ce/component/light.h"

class Window;
class ParallelLight : public ALight
{
private:
    Math::Vec4 direction;
    Math::Vec4 color;
    float intensity;
    
public:
    ParallelLight();
    ParallelLight(const Math::Vec4& p_direction, const Math::Vec4& p_color, float p_intensity);
    
    /**
     * @brief Get the direction of the light.
     * 
     * @return const Math::Vec4& The direction of the light.
     */
    FORCE_INLINE const Math::Vec4& GetDirection() const noexcept { return direction; }

    /**
     * @brief Get the direction of the light.
     * 
     * @return Math::Vec4& The direction of the light.
     */
    FORCE_INLINE Math::Vec4& Direction() noexcept { return direction; }

    /**
     * @brief Get the color of the light.
     * 
     * @return const Math::Vec4& The color of the light
     */
    FORCE_INLINE const Math::Vec4& GetColor() const noexcept { return color; }

    /**
     * @brief Get the color of the light.
     * 
     * @return Math::Vec4& The color of the light
     */
    FORCE_INLINE Math::Vec4& Color() noexcept { return color; }

    /**
     * @brief Get the intensity of the light
     * 
     * @return float The intensity of the light. 
     */
    FORCE_INLINE float GetIntensity() const noexcept { return intensity; }

    /**
     * @brief Get the intensity of the light
     * 
     * @return float& The intensity of the light. 
     */
    FORCE_INLINE float& Intensity() noexcept { return intensity; }

    virtual ~ParallelLight();

    /**
     * @brief Get the name of the uniform variable.
     * 
     * @return std::string The name of the uniform variable.
     */
    virtual std::string UniformName() const override { return "parallel_light"; }

    /**
     * @brief Set the uniform of the light's data.
     * 
     */
    virtual void SetUniform(Window* p_context, size_t p_index) override;

    /**
     * @brief Draw the light.
     * 
     */
    virtual void Draw(Window* p_context) override;

};