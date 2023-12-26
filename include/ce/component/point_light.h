#pragma once
#include "ce/component/light.h"

class PointLight : public ALight
{
public:

protected:
     Math::Vec4 color;
    float intensity;

public:

    /**
     * @brief Constructor for PointLight.
     * 
     * @param p_context 
     */
    PointLight(Window* p_context);

    /**
     * @brief Constructor for PointLight.
     * 
     * @param p_color The color of the light.
     * @param p_intensity The intensity of the light.
     * @param p_context The context of this light.
     */
    PointLight(const Math::Vec4& p_color, float p_intensity, Window* p_context);

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

    /**
     * @brief Get the name of the uniform variable.
     * 
     * @return std::string The name of the uniform variable.
     */
    virtual std::string UniformName() const override { return "point_light"; }

    /**
     * @brief Set the uniform of the light's data.
     * 
     */
    virtual void SetUniform(size_t p_index) override;

    /**
     * @brief Draw the light.
     * 
     */
    virtual void Draw() override;
};