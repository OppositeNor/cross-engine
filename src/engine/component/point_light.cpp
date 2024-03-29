#include "ce/component/point_light.h"
#include "ce/graphics/window.h"
#include "ce/component/camera.h"
#include "ce/graphics/renderer/renderer.h"
#include <type_traits>
#include <sstream>

namespace CrossEngine
{
    PointLight::PointLight(const std::string& p_component_name)
        : PointLight(Math::Pos(), 20, p_component_name)
    {
    }
    PointLight::PointLight(const Math::Vec4& p_color, float p_intensity, const std::string& p_component_name)
        : ALight(p_component_name)
    {
        color = p_color;
        intensity = p_intensity;
        SetMaxLightRenderDistance(1000);
    }

    void PointLight::SetMaxLightRenderDistance(float p_distance)
    {
        max_light_render_distance = p_distance;
        max_light_render_distance_sq = p_distance * p_distance;
    }

    void PointLight::SetUniform(Window* p_context, size_t p_index)
    {
        auto global_position = GetGlobalPosition();
        std::stringstream ss;
        ss << UniformName() << "[" << p_index << "]";
        p_context->GetRenderer()->GetShaderProgram()->SetUniform(ss.str() + ".position", global_position);
        p_context->GetRenderer()->GetShaderProgram()->SetUniform(ss.str() + ".color", color);
        p_context->GetRenderer()->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
    }

    void PointLight::Draw(Window* p_context)
    {
        ALight::Draw(p_context);
        float to_camera_sq = (GetPosition() - p_context->GetUsingCamera()->GetPosition()).LengthSquared();
        if (max_light_render_distance_sq > to_camera_sq)
            SetUniform(p_context, p_context->GetPointLightNextIndex());
    }
}