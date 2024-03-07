#include "ce/materials/pbr_material.h"
#include "ce/graphics/shader/shader_program.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "ce/texture/static_texture.h"
#include "ce/graphics/window.h"
#include "ce/graphics/renderer/renderer.h"

namespace CrossEngine
{
    PBRMaterial::PBRMaterial()
        : PBRMaterial(Math::Vec4(1.0, 1.0, 1.0, 1.0), 0.2, 0.2)
    {
    }

    PBRMaterial::PBRMaterial(const Math::Vec4& p_albedo, float p_roughness, float p_metallic)
        : scaler_albedo(p_albedo), scaler_roughness(p_roughness), scaler_metallic(p_metallic), AMaterial()
    {
        albedo = Graphics::GetDefaultAlbedo();
        normal = Graphics::GetDefaultNormal();
        metallic = Graphics::GetDefaultMetallic();
        roughness = Graphics::GetDefaultRoughness();
        ao = Graphics::GetDefaultAO();
        
    }

    void PBRMaterial::SetUniform(Window* p_context) const
    {
        albedo->BindTexture(p_context, GetUniformName() + ".albedo");
        p_context->GetRenderer()->GetShaderProgram()->SetUniform("scaler_albedo", scaler_albedo);
        normal->BindTexture(p_context, GetUniformName() + ".normal");
        metallic->BindTexture(p_context, GetUniformName() + ".metallic");
        p_context->GetRenderer()->GetShaderProgram()->SetUniform("scaler_metallic", scaler_metallic);
        roughness->BindTexture(p_context, GetUniformName() + ".roughness");
        p_context->GetRenderer()->GetShaderProgram()->SetUniform("scaler_roughness", scaler_roughness);
        ao->BindTexture(p_context, GetUniformName() + ".ao");

    }
}