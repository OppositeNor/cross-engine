#pragma once
#include "ce/component/component.h"

class Camera : public Component
{

public:

    Camera(Window* p_context);

    Mat4 GetViewMatrix() const;
};
