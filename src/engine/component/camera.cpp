#include "ce/component/camera.h"

Camera::Camera(Window* p_context)
    : Component(p_context)
{
}

Math::Mat4 Camera::GetViewMatrix() const
{
    //todo
    return Math::View(GetPosition(), GetRotation());
}