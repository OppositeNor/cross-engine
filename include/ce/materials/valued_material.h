#pragma once
#include "ce/materials/material.h"
#include "ce/math/math.hpp"

class PBRMaterial : public AMaterial
{
    Vec4 scaler_albedo;
    float scaler_roughness;
    float scaler_metallic;

public:
    /**
     * @brief Construct a new PBRMaterial object.
     */
    PBRMaterial(const Window* p_context);

    /**
     * @brief Construct a new PBRMaterial object.
     * 
     * @param p_albedo Albedo.
     * @param p_roughness Roughness.
     * @param p_metallic Metallic.
     * @param p_ao Ambient occlusion.
     */
    PBRMaterial(const Vec4& p_albedo, float p_roughness, float p_metallic, const Window* p_context);

    /**
     * @brief Get the albedo of the material.
     * 
     * @return const std::shared_ptr<ATexture>& The albedo of the material.
     */
    FORCE_INLINE const std::shared_ptr<ATexture>& GetAlbedo() const noexcept { return albedo; }

    /**
     * @brief Get the albedo of the material.
     *
     * @return std::shared_ptr<ATexture> The albedo of the material.
     */
    FORCE_INLINE std::shared_ptr<ATexture> Albedo() noexcept { return albedo; }

    /**
     * @brief Get the normal of the material.
     * 
     * @return std::shared_ptr<ATexture> The normal of the material.
     */
    FORCE_INLINE const std::shared_ptr<ATexture>& GetNormal() const noexcept { return normal; }

    /**
     * @brief Get the normal of the material.
     *
     * @return std::shared_ptr<ATexture> The normal of the material.
     */
    FORCE_INLINE std::shared_ptr<ATexture> Normal() noexcept { return normal; }

    /**
     * @brief Get the metallic of the material.
     * 
     * @return std::shared_ptr<ATexture> The metallic of the material.
     */
    FORCE_INLINE const std::shared_ptr<ATexture>& GetMetallic() const noexcept { return metallic; }

    /**
     * @brief Get the metallic of the material.
     *
     * @return std::shared_ptr<ATexture> The metallic of the material.
     */
    FORCE_INLINE std::shared_ptr<ATexture> Metallic() noexcept { return metallic; }

    /**
     * @brief Get the roughness of the material.
     * 
     * @return std::shared_ptr<ATexture> The roughness of the material.
     */
    FORCE_INLINE const std::shared_ptr<ATexture>& GetRoughness() const noexcept { return roughness; }

    /**
     * @brief Get the roughness of the material.
     *
     * @return std::shared_ptr<ATexture> The roughness of the material.
     */
    FORCE_INLINE std::shared_ptr<ATexture> Roughness() noexcept { return roughness; }

    /**
     * @brief Get the ambient occlusion of the material.
     * 
     * @return std::shared_ptr<ATexture> The ambient occlusion of the material.
     */
    FORCE_INLINE const std::shared_ptr<ATexture>& GetAO() const noexcept { return ao; }

    /**
     * @brief Get the ambient occlusion of the material.
     *
     * @return std::shared_ptr<ATexture> The ambient occlusion of the material.
     */
    FORCE_INLINE std::shared_ptr<ATexture> AO() noexcept { return ao; }

    /**
     * @brief Get the albedo of the material.
     * 
     * @return const Vec4& The albedo of the material.
     */
    FORCE_INLINE const Vec4& GetAlbedoScaler() const noexcept { return scaler_albedo; }

    /**
     * @brief Get the albedo of the material.
     * 
     * @return Vec4& The albedo of the material.
     */
    FORCE_INLINE Vec4& AlbedoScaler() noexcept { return scaler_albedo; }

    /**
     * @brief Get the roughness of the material.
     * 
     * @return float The roughness of the material.
     */
    FORCE_INLINE float GetRoughnessScaler() const noexcept { return scaler_roughness; }

    /**
     * @brief Get the roughness of the material.
     * 
     * @return float& The roughness of the material.
     */
    FORCE_INLINE float& RoughnessScaler() noexcept { return scaler_roughness; }

    /**
     * @brief Get the metallic of the material.
     * 
     * @return float The metallic of the material.
     */
    FORCE_INLINE float GetMetallicScaler() const noexcept { return scaler_metallic; }

    /**
     * @brief Get the metallic of the material.
     * 
     * @return float& The metallic of the material.
     */
    FORCE_INLINE float& MetallicScaler() noexcept { return scaler_metallic; }

    virtual void SetUniform(const ShaderProgram* p_shader_program) const override;
};