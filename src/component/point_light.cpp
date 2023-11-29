#include "ce/component/point_light.h"
#include "ce/graphics/window.h"
#include <type_traits>
#include <sstream>

PointLight::PointLight(Window* p_context)
    : ALight(p_context)
{
}

PointLight::PointLight(const Vec4& p_color, Window* p_context)
    : ALight(p_color, p_context)
{
}


void PointLight::SetUniform(size_t p_index)
{
    auto global_position = GetGlobalPosition();
    data.position[0] = global_position[0];
    data.position[1] = global_position[1];
    data.position[2] = global_position[2];
    data.position[3] = 1.0f;
    data.color[0] = color[0];
    data.color[1] = color[1];
    data.color[2] = color[2];
    data.color[3] = color[3];
    data.intensity = intensity;
    std::stringstream ss;
    ss << UniformName() << "[" << p_index << "]";
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".position", global_position);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".color", color);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
}