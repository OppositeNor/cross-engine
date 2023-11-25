#pragma once
#include "ce/component/component.h"
class Window;
class ALight : public Component
{
protected:
    Vec3 color;
public:
    ALight(Window* p_context);
    ALight(const Vec3& p_color, Window* p_context);
    virtual ~ALight();

    /**
     * @brief Get the color of the light.
     * 
     * @return const Vec3& The color of the light.
     */
    FORCE_INLINE const Vec3& GetColor() const { return color; }
    /**
     * @brief Get the color of the light.
     * 
     * @return Vec3& The color of the light.
     */
    FORCE_INLINE Vec3& GetColor() { return color; }

    /**
     * @brief Get the data of this light.
     * 
     * @return void* 
     */
    virtual void* GetData() = 0;

    /**
     * @brief Get the name of the uniform variable.
     * 
     * @return std::string The name of the uniform variable.
     */
    virtual std::string UniformName() = 0;
};