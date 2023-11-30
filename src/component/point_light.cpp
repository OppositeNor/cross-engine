#include "ce/component/point_light.h"
#include "ce/graphics/window.h"
#include <type_traits>
#include <sstream>

PointLight::PointLight(Window* p_context)
    : ALight(p_context)
{
}
PointLight::PointLight(const Vec4& p_color, Window* p_context)
    : PointLight(p_context)
{
    diffuse_color = p_color;
    specular_color = p_color;
}

void PointLight::SetUniform(size_t p_index)
{
    auto global_position = GetGlobalPosition();
    std::stringstream ss;
    ss << UniformName() << "[" << p_index << "]";
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".position", global_position);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".diffuse_color", diffuse_color);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".diffuse_intensity", diffuse_intensity);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".specular_color", specular_color);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".specular_intensity", specular_intensity);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".specular_power", specular_power);
}