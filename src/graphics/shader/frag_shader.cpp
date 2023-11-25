#include "ce/graphics/shader/frag_shader.h"
#include <glad/glad.h>

FragShader::FragShader(const std::string& p_shader_path)
    : AShader(p_shader_path)
{
    shader_id = glCreateShader(GL_FRAGMENT_SHADER);
}

unsigned int FragShader::GetShaderType() const
{
    return GL_FRAGMENT_SHADER;
}