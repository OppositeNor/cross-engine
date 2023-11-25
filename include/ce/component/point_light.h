#pragma once
#include "ce/component/light.h"

class PointLight : public ALight
{
protected:
    /**
     * @brief Data is a struct that is used for transfering data to
     * the shaders.
     */
    struct Data
    {
        float position[3];
        float color[3];
    }data;
public:

    /**
     * @brief Constructor for PointLight.
     * 
     * @param p_context 
     */
    PointLight(Window* p_context);

    /**
     * @brief Get the data of this light.
     * 
     * @return const Data& The data of this light.
     */
    virtual void* GetData() override;

    /**
     * @brief Get the name of the uniform variable.
     * 
     * @return std::string The name of the uniform variable.
     */
    virtual std::string UniformName() override { return "point_lights"; }
};