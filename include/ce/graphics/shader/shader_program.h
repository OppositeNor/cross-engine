#pragma once
#include <memory>
#include <mutex>
#include "ce/graphics/shader/a_shader.h"
#include "ce/math/math.hpp"

class ShaderProgram
{
    unsigned int program_id;
    std::shared_ptr<AShader> vert_shader;
    std::shared_ptr<AShader> frag_shader;
    std::mutex compile_mutex;
    bool usable = false;
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
     * @brief Set the uniform for the shader.
     * 
     * @param p_name The name of the uniform.
     * @param p_mat4 The Mat4f to set the uniform to.
     */
    void SetUniform(const std::string& p_name, const Mat4& p_mat4);

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
     * @brief Destroy the Shader Program object.
     * 
     */
    virtual ~ShaderProgram();

};