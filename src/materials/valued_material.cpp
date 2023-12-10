#include "ce/materials/valued_material.h"
#include "ce/graphics/shader/shader_program.h"

ValuedMaterial::ValuedMaterial()
    : ValuedMaterial(Vec4(1.0, 1.0, 1.0, 1.0), 0.2, 0)
{

}

ValuedMaterial::ValuedMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic)
    : albedo(p_albedo), roughness(p_roughness), metallic(p_metallic), AMaterial()
{

}

void ValuedMaterial::SetUniform(const ShaderProgram* p_shader_program) const
{
    p_shader_program->SetUniform(GetUniformName() + ".albedo", albedo);
    p_shader_program->SetUniform(GetUniformName() + ".roughness", roughness);
    p_shader_program->SetUniform(GetUniformName() + ".metallic", metallic);
}