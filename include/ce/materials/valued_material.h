#pragma once
#include "ce/materials/material.h"
#include "ce/math/math.hpp"

class ValuedMaterial : public AMaterial
{
    Vec4 albedo;
    float roughness;
    float metallic;
public:
    /**
     * @brief Construct a new Valued Material object.
     */
    ValuedMaterial();

    /**
     * @brief Construct a new Valued Material object.
     * 
     * @param p_albedo Albedo.
     * @param p_roughness Roughness.
     * @param p_metallic Metallic.
     * @param p_ao Ambient occlusion.
     */
    ValuedMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic);

    virtual std::string GetUniformName() const override { return "material"; };

    virtual void SetUniform(const ShaderProgram* p_shader_program) const override;
};