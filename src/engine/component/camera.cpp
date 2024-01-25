#include "ce/component/camera.h"
#include "ce/graphics/window.h"

Camera::Camera()
    : Component()
{
}

Math::Mat4 Camera::GetViewMatrix() const
{
    return Math::View(GetGlobalPosition(), GetRotation());
}

void Camera::LookAt(const Math::Vec4& p_target)
{
    //todo: buggy
    Math::Vec4 to_target = (p_target - GetGlobalPosition()).Normalize();
    auto front = GetFront();
    auto axis = Math::Cross(front, to_target).Normalize();
    auto dot = Math::Dot(front, to_target);
    if (abs(dot - 1) < 0.0001f)
        return;
    while (dot > 1)
        dot -= Math::PI;
    while (dot < -Math::PI)
        dot += Math::PI;
    Rotate(axis, std::acos(dot));
    std::cout << dot << std::endl;
}