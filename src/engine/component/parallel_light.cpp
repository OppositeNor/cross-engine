#include "ce/component/parallel_light.h"
#include "ce/graphics/window.h"

ParallelLight::ParallelLight()
    : ParallelLight(Math::Vec4(0, 0, 1, 0), Math::Vec4(1, 1, 1, 1), 1)
{

}

ParallelLight::ParallelLight(const Math::Vec4& p_direction, const Math::Vec4& p_color, float p_intensity)
    : direction(p_direction), color(p_color), intensity(p_intensity), ALight()
{
    direction = p_direction;
    color = p_color;
    intensity = p_intensity;
}

ParallelLight::~ParallelLight()
{

}

void ParallelLight::SetUniform(Window* p_context, size_t p_index)
{
    std::stringstream ss;
    ss << UniformName() << "[" << p_index << "]";
    p_context->GetShaderProgram()->SetUniform(ss.str() + ".direction", direction);
    p_context->GetShaderProgram()->SetUniform(ss.str() + ".color", color);
    p_context->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
}

void ParallelLight::Draw(Window* p_context)
{
    ALight::Draw(p_context);
    SetUniform(p_context, p_context->GetParallelLightNextIndex());
}