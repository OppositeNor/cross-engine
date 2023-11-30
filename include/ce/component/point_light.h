#pragma once
#include "ce/component/light.h"

class PointLight : public ALight
{
public:

protected:
    Vec4 diffuse_color;
    float diffuse_intensity = 10.0f;
    Vec4 specular_color;
    float specular_intensity = 100.0f;
    float specular_power = 100.0f;

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
     * @param p_context 
     */
    PointLight(const Vec4& p_color, Window* p_context);

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
};