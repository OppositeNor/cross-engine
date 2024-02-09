#include "ce/component/camera.h"
#include "ce/graphics/window.h"

namespace CrossEngine
{
    Camera::Camera(const std::string& p_component_name)
        : Component3D(p_component_name)
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
        Rotate(axis, std::acos(dot));
    }
}