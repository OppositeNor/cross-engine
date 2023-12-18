#include "ce/materials/valued_material.h"
#include "ce/graphics/shader/shader_program.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "ce/graphics/texture/static_texture.h"

const ubyte_t ValuedMaterial::WHITE_IMAGE[4] = {
    255, 255, 255, 255
};

ValuedMaterial::ValuedMaterial(const Window* p_context)
    : ValuedMaterial(Vec4(1.0, 1.0, 1.0, 1.0), 0.2, 0, p_context)
{
}

ValuedMaterial::ValuedMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic, const Window* p_context)
    : scaler_albedo(p_albedo), scaler_roughness(p_roughness), scaler_metallic(p_metallic), AMaterial(p_context)
{
    albedo = std::make_unique<StaticTexture>(context);
    normal = std::make_unique<StaticTexture>(context);
    metallic = std::make_unique<StaticTexture>(context);
    roughness = std::make_unique<StaticTexture>(context);
    ao = std::make_unique<StaticTexture>(context);

    // albedo
    albedo->LoadTexture(WHITE_IMAGE, 2, 2, 1);

    // normal
    normal->LoadTexture(Resource::GetExeDirectory() + "/textures/default_normal.png");
    
    // metallic
    metallic->LoadTexture(WHITE_IMAGE, 2, 2, 1);

    // roughness
    roughness->LoadTexture(WHITE_IMAGE, 2, 2, 1);

    // ao
    ao->LoadTexture(WHITE_IMAGE, 2, 2, 1);
}

void ValuedMaterial::SetUniform(const ShaderProgram* p_shader_program) const
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