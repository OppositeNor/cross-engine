#include "ce/graphics/shader/a_shader.h"
#include <glad/glad.h>
#include "ce/resource/resource.h"
#include "ce/memory/unique_ptr.hpp"

AShader::~AShader()
{
    if (shader_id != 0)
        glDeleteShader(shader_id);
}


void AShader::Compile()
{
    size_t size;
    auto shader_source = UniquePtr<char[]>(Resource::LoadFile(shader_path, size));
    shader_source[size] = '\0';
    auto p = shader_source.Get();
    glShaderSource(shader_id, 1, (const char**)&p, NULL);
    glCompileShader(shader_id);
    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        throw std::runtime_error("Failed to compile shader: " + std::string(info_log));
    }
}