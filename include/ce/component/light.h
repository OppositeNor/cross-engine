#pragma once
#include "ce/component/component.h"
class Window;
class ALight : public Component
{
public:
    ALight(Window* p_context);
    virtual ~ALight();

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
};