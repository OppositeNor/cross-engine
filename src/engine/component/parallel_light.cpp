#include "ce/component/parallel_light.h"
#include "ce/graphics/window.h"

namespace CrossEngine
{
    ParallelLight::ParallelLight(const std::string& p_component_name)
        : ParallelLight(Math::Vec4(0, 0, 1, 0), Math::Vec4(1, 1, 1, 1), Math::Vec4(0.05, 0.06, 0.08, 1), 1, p_component_name)
    {

    }

    ParallelLight::ParallelLight(const Math::Vec4& p_direction, const Math::Vec4& p_color, const Math::Vec4& p_ambient, float p_intensity,
        const std::string& p_component_name)
        : direction(p_direction), color(p_color), ambient(p_ambient), intensity(p_intensity), ALight(p_component_name)
    {
    }

    ParallelLight::ParallelLight(const Math::Vec4& p_direction, const Math::Vec4& p_color, float p_intensity,
        const std::string& p_component_name)
        : ParallelLight(p_direction, p_color, Math::Vec4(0.05, 0.06, 0.08, 1), p_intensity, p_component_name)
    {
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
        p_context->GetShaderProgram()->SetUniform(ss.str() + ".ambient", ambient);
        p_context->GetShaderProgram()->SetUniform(ss.str() + ".intensity", intensity);
    }

    void ParallelLight::Draw(Window* p_context)
    {
        ALight::Draw(p_context);
        SetUniform(p_context, p_context->GetParallelLightNextIndex());
    }
}