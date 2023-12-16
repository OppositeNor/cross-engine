#pragma once
#include "ce/materials/material.h"
#include "ce/math/math.hpp"

class ValuedMaterial : public AMaterial
{
    Vec4 scaler_albedo;
    float scaler_roughness;
    float scaler_metallic;

    static const ubyte_t WHITE_IMAGE[4];

public:
    /**
     * @brief Construct a new Valued Material object.
     */
    ValuedMaterial(const Window* p_context);

    /**
     * @brief Construct a new Valued Material object.
     * 
     * @param p_albedo Albedo.
     * @param p_roughness Roughness.
     * @param p_metallic Metallic.
     * @param p_ao Ambient occlusion.
     */
    ValuedMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic, const Window* p_context);

    /**
     * @brief Get the albedo of the material.
     * 
     * @return const Vec4& The albedo of the material.
     */
    FORCE_INLINE const Vec4& GetAlbedo() const noexcept { return scaler_albedo; }

    /**
     * @brief Get the albedo of the material.
     * 
     * @return Vec4& The albedo of the material.
     */
    FORCE_INLINE Vec4& Albedo() noexcept { return scaler_albedo; }

    /**
     * @brief Get the roughness of the material.
     * 
     * @return float The roughness of the material.
     */
    FORCE_INLINE float GetRoughness() const noexcept { return scaler_roughness; }

    /**
     * @brief Get the roughness of the material.
     * 
     * @return float& The roughness of the material.
     */
    FORCE_INLINE float& Roughness() noexcept { return scaler_roughness; }

    /**
     * @brief Get the metallic of the material.
     * 
     * @return float The metallic of the material.
     */
    FORCE_INLINE float GetMetallic() const noexcept { return scaler_metallic; }

    /**
     * @brief Get the metallic of the material.
     * 
     * @return float& The metallic of the material.
     */
    FORCE_INLINE float& Metallic() noexcept { return scaler_metallic; }

    virtual void SetUniform(const ShaderProgram* p_shader_program) const override;
};