#pragma once
#include "ce/component/component.h"

class Camera : public Component
{

public:

    Camera(Window* p_context);

     Math::Mat4 GetViewMatrix() const;

    /**
     * @brief Rotate the camera to look at a target.
     * 
     * @param p_target The target to look at.
     */
    void LookAt(const Math::Vec4& p_target);
};
