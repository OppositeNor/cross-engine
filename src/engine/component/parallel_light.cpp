#include "ce/component/parallel_light.h"
#include "ce/graphics/window.h"

ParallelLight::ParallelLight(Window* p_context)
    : ParallelLight(Math::Vec4(0, 0, 1, 0), Math::Vec4(1, 1, 1, 1), 1, p_context)
{

}

ParallelLight::ParallelLight(const Math::Vec4& p_direction, const Math::Vec4& p_color, float p_intensity, Window* p_context)
    : direction(p_direction), color(p_color), intensity(p_intensity), ALight(p_context)
{
    direction = p_direction;
    color = p_color;
    intensity = p_intensity;
}

ParallelLight::~ParallelLight()
{

}

void ParallelLight::SetUniform(size_t p_index)
{
    std::stringstream ss;
    ss << UniformName() << "[" << p_index << "]";
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".direction", direction);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".color", color);
    GetContext()->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
}

void ParallelLight::Draw()
{
    ALight::Draw();
    SetUniform(GetContext()->GetParallelLightNextIndex());
}