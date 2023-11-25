#pragma once

#include "ce/graphics/shader/a_shader.h"

class FragShader : public AShader
{
public:
    /**
     * @brief Construct a new Frag Shader object
     * 
     * @param p_shader_path The path to the shader file.
     */
    FragShader(const std::string& p_shader_path);

    /**
     * @brief Get the shader type.
     * 
     * @return unsigned int The shader type. 
     */
    virtual unsigned int GetShaderType() const override;
};

