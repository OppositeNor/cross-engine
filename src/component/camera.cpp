#include "ce/component/camera.h"

Camera::Camera(Window* p_context)
    : Component(p_context)
{
}

Mat4 Camera::GetViewMatrix() const
{
    //todo
    return Mat4::View(GetPosition(), GetRotation());
}