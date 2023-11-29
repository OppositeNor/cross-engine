#pragma once
#include "ce/component/component.h"
class Window;
class ALight : public Component
{
protected:
    Vec4 color;
    float intensity = 2.0f;
public:
    ALight(Window* p_context);
    ALight(const Vec4& p_color, Window* p_context);
    virtual ~ALight();

    /**
     * @brief Get the color of the light.
     * 
     * @return const Vec3& The color of the light.
     */
    FORCE_INLINE const Vec4& GetColor() const { return color; }
    /**
     * @brief Get the color of the light.
     * 
     * @return Vec3& The color of the light.
     */
    FORCE_INLINE Vec4& GetColor() { return color; }

    /**
     * @brief Get the name of the uniform variable.
     * 
     * @return std::string The name of the uniform variable.
     */
    virtual std::string UniformName() const = 0;

    /**
     * @brief Set the uniform of the light's data.
     * 
     */
    virtual void SetUniform(size_t p_index) = 0;

    /**
     * @brief Get the intensity of the light.
     * 
     * @return const float& The intensity of the light.
     */
    float& Intensity() { return intensity; }

    /**
     * @brief Get the intensity of the light.
     * 
     * @return const float& The intensity of the light.
     */
    const float& Intensity() const { return intensity; }
};