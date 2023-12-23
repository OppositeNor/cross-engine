#include "ce/component/point_light.h"
#include "ce/graphics/window.h"
#include <type_traits>
#include <sstream>

PointLight::PointLight(Window* p_context)
    : PointLight(Math::Pos(), 20, p_context)
{
}
PointLight::PointLight(const Math::Vec4& p_color, float p_intensity, Window* p_context)
    : ALight(p_context)
{
    color = p_color;
    intensity = p_intensity;
}

void PointLight::SetUniform(size_t p_index)
{
    auto global_position = GetGlobalPosition();
    std::stringstream ss;
    ss << UniformName() << "[" << p_index << "]";
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".position", global_position);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".color", color);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
}

void PointLight::Draw()
{
    ALight::Draw();
    SetUniform(GetContext()->GetPointLightNextIndex());
}