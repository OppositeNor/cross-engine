#pragma once
#include <memory>
#include <mutex>
#include "ce/graphics/shader/a_shader.h"
#include "ce/math/math.hpp"
#include "ce/component/point_light.h"
#include <memory>

class ShaderProgram
{
    unsigned int program_id = 0;
    std::shared_ptr<AShader> vert_shader;
    std::shared_ptr<AShader> frag_shader;
    std::mutex compile_mutex;
    bool usable = false;
    mutable int sampler_count = 0;
public:

    /**
     * @brief Construct a new Shader Program object.
     * 
     * @param p_vert_shader_path The path to the vertex shader file.
     * @param p_frag_shader_path The path to the fragment shader file.
     */
    ShaderProgram(const std::string& p_vert_shader_path, const std::string& p_frag_shader_path);

    /**
     * @brief Construct a new Shader Program object.
     * 
     * @param p_vert_shader The vertex shader to be moved.
     * @param p_frag_shader The fragment shader to be moved.
     */
    ShaderProgram(AShader*&& p_vert_shader, AShader*&& p_frag_shader) noexcept;

    /**
     * @brief Construct a new Shader Program object.
     * 
     * @param p_vert_shader The vertex shader to be set to.
     * @param p_frag_shader The fragment shader to be set to.
     */
    ShaderProgram(const std::shared_ptr<AShader>& p_vert_shader, const std::shared_ptr<AShader>& p_frag_shader);

    /**
     * @brief ShaderProgram should not be copied.
     */
    ShaderProgram(const ShaderProgram&) = delete;

    /**
     * @brief Move constructor for ShaderProgram.
     */
    ShaderProgram(ShaderProgram&& p_other) noexcept;

    /**
     * @brief Destroy the Shader Program object.
     * 
     */
    virtual ~ShaderProgram();

    /**
     * @brief Set the uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_float The float to set the uniform to.
     */
    void SetUniform(const std::string& p_name, float p_float) const;

    /**
     * @brief Set the uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_data The integer to set the uniform to.
     */
    void SetUniform(const std::string& p_name, int p_int) const;

    /**
     * @brief Set the uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_mat4 The Math::Mat4f to set the uniform to.
     */
    void SetUniform(const std::string& p_name, const Math::Mat4& p_mat4) const;

    /**
     * @brief Set the uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_vec4 TheMath::Vec4f to set the uniform to.
     */
    void SetUniform(const std::string& p_name, const Math::Vec4& p_vec4) const;

    /**
     * @brief Set the sampler uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_texture_id The texture id to set the uniform to.
     */
    void SetSampler2DUniform(const std::string& p_name, unsigned int p_texture_id) const;

    /**
     * @brief Set the sampler uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_texture_id The texture id to set the uniform to.
     */
    void SetSamplerCubeUniform(const std::string& p_name, unsigned int p_texture_id) const;

    /**
     * @brief Compile the shader program.
     * 
     * @throw std::runtime_error If the shader program fails to link.
     * @throw std::runtime_error If the vertex shader fails to compile.
     * @throw std::runtime_error If the fragment shader fails to compile.
     */
    void Compile();

    /**
     * @brief Activate this shader program.
     * 
     */
    void Use();

    /**
     * @brief Refresh the shader program.
     * 
     */
    void Refresh();

    /**
     * @brief Returns whether or not this shader program is usable.
     * The shader program will be usable if it has been successfully
     * compiled.
     * 
     * @return true The shader program is usable.
     * @return false The shader program is not usable.
     */
    FORCE_INLINE bool IsUsable() const { return usable; }

    /**
     * @brief Get the vertex shader.
     * 
     * @return std::shared_ptr<AShader> The vertex shader.
     */
    FORCE_INLINE std::shared_ptr<AShader> GetVertShader() const { return vert_shader; }

    /**
     * @brief Get the fragment shader.
     * 
     * @return std::shared_ptr<AShader> The fragment shader.
     */
    FORCE_INLINE std::shared_ptr<AShader> GetFragShader() const { return frag_shader; }

    FORCE_INLINE int GetSamplerNewIndex() const { return sampler_count++; }
};