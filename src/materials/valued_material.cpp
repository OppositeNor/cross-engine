#include "ce/materials/valued_material.h"
#include "ce/graphics/shader/shader_program.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "ce/graphics/texture/static_texture.h"
#include "ce/graphics/window.h"

PBRMaterial::PBRMaterial(const Window* p_context)
    : PBRMaterial(Vec4(1.0, 1.0, 1.0, 1.0), 0.2, 0.5, p_context)
{
}

PBRMaterial::PBRMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic, const Window* p_context)
    : scaler_albedo(p_albedo), scaler_roughness(p_roughness), scaler_metallic(p_metallic), AMaterial(p_context)
{
    albedo = p_context->GetDefaultAlbedo();
    normal = p_context->GetDefaultNormal();
    metallic = p_context->GetDefaultMetallic();
    roughness = p_context->GetDefaultRoughness();
    ao = p_context->GetDefaultAO();
    
}

void PBRMaterial::SetUniform(const ShaderProgram* p_shader_program) const
{
    albedo->BindTexture(p_shader_program, GetUniformName() + ".albedo");
    p_shader_program->SetUniform("scaler_albedo", scaler_albedo);
    normal->BindTexture(p_shader_program, GetUniformName() + ".normal");
    metallic->BindTexture(p_shader_program, GetUniformName() + ".metallic");
    p_shader_program->SetUniform("scaler_metallic", scaler_metallic);
    roughness->BindTexture(p_shader_program, GetUniformName() + ".roughness");
    p_shader_program->SetUniform("scaler_roughness", scaler_roughness);
    ao->BindTexture(p_shader_program, GetUniformName() + ".ao");

}