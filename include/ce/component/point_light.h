#pragma once
#include "ce/component/light.h"

class PointLight : public ALight
{
public:
    /**
     * @brief Data is a struct that is used for transfering data to
     * the shaders.
     */
    struct Data
    {
        float position[4];
        float color[4];
        float intensity = 1.0f;
    };
protected:
    struct Data data;

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
    virtual void SetUniform() override;
};