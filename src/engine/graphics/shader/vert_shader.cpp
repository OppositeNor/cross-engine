#include "ce/graphics/shader/vert_shader.h"
#include <glad/glad.h>

namespace CrossEngine
{
    VertShader::VertShader(const std::string& p_shader_path)
        : AShader(p_shader_path)
    {
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    }

    unsigned int VertShader::GetShaderType() const
    {
        return GL_VERTEX_SHADER;
    }
}