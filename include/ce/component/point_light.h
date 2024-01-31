#pragma once
#include "ce/component/light.h"

/**
 * @brief Point light.
 * 
 */
class PointLight : public ALight
{
protected:
    Math::Vec4 color;
    float intensity;

    /**
     * @brief The maximum distance form the camera
     * that the light can be visualized.
     */
    float max_light_render_distance;
    float max_light_render_distance_sq;

public:

    /**
     * @brief Constructor for PointLight.
     * 
     */
    PointLight();

    /**
     * @brief Constructor for PointLight.
     * 
     * @param p_color The color of the light.
     * @param p_intensity The intensity of the light.
     */
    PointLight(const Math::Vec4& p_color, float p_intensity);

    /**
     * @brief Get the maximum distance that the light can be fisible to the camera.
     * 
     * @return float 
     */
    FORCE_INLINE float GetMaxLightRenderDistance() { return max_light_render_distance; }

    /**
     * @brief Set the maximum distance that the light can be fisible to the camera.
     * 
     * @param p_distance The distance to be set to.
     */
    void SetMaxLightRenderDistance(float p_distance);

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
    virtual void SetUniform(Window* p_context, size_t p_index) override;

    /**
     * @brief Draw the light.
     * 
     */
    virtual void Draw(Window* p_context) override;
};