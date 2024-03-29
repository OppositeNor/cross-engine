#include "ce/graphics/shader/shader_program.h"
#include "ce/graphics/shader/vert_shader.h"
#include "ce/graphics/shader/frag_shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CrossEngine
{
    ShaderProgram::ShaderProgram(const std::string& p_vert_shader_path, const std::string& p_frag_shader_path)
        : vert_shader(new VertShader(p_vert_shader_path)), frag_shader(new FragShader(p_frag_shader_path))
    {
    }

    ShaderProgram::ShaderProgram(AShader*&& p_vert_shader, AShader*&& p_frag_shader) noexcept
        : vert_shader(p_vert_shader), frag_shader(p_frag_shader)
    {
        p_vert_shader = nullptr;
        p_frag_shader = nullptr;
    }

    ShaderProgram::ShaderProgram(const std::shared_ptr<AShader>& p_vert_shader, const std::shared_ptr<AShader>& p_frag_shader)
        : vert_shader(p_vert_shader), frag_shader(p_frag_shader)
    {}



    ShaderProgram::ShaderProgram(ShaderProgram&& p_other) noexcept
    {
        program_id = p_other.program_id;
        vert_shader = std::move(p_other.vert_shader);
        frag_shader = std::move(p_other.frag_shader);
        p_other.program_id = 0;
    }

    void ShaderProgram::SetUniform(const std::string& p_name, float p_float) const
    {
        if (usable)
        {
            int location = glGetUniformLocation(program_id, p_name.c_str());
            if (location == -1)
                return;
            glUniform1f(location, p_float);
        }
    }

    void ShaderProgram::SetUniform(const std::string& p_name, int p_int) const
    {
        if (usable)
        {
            int location = glGetUniformLocation(program_id, p_name.c_str());
            if (location == -1)
                return;
            glUniform1i(location, p_int);
        }
    }

    void ShaderProgram::SetUniform(const std::string& p_name, const Math::Mat4& p_mat4) const
    {
        if (usable)
        {
            int location = glGetUniformLocation(program_id, p_name.c_str());
            if (location == -1)
                return;
            glUniformMatrix4fv(location, 1, GL_TRUE, p_mat4.GetRaw());
        }
    }

    void ShaderProgram::SetUniform(const std::string& p_name, const Math::Vec4& p_vec4) const
    {
        if (usable)
        {
            int location = glGetUniformLocation(program_id, p_name.c_str());
            if (location == -1)
                return;
            glUniform4fv(location, 1, p_vec4.GetRaw());
        }
    }

    void ShaderProgram::SetSampler2DUniform(const std::string& p_name, unsigned int p_texture_id) const
    {
        if (usable)
        {
            int index = GetSamplerNewIndex();
            SetUniform(p_name, index);
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, p_texture_id);
        }
    }

    void ShaderProgram::SetSamplerCubeUniform(const std::string& p_name, unsigned int p_texture_id) const
    {
        if (usable)
        {
            int index = GetSamplerNewIndex();
            SetUniform(p_name, index);
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_CUBE_MAP, p_texture_id);
        }

    }

    ShaderProgram::~ShaderProgram()
    {
        if (program_id != 0)
            glDeleteProgram(program_id);
    }

    void ShaderProgram::Compile()
    {
        if (!usable)
        {
            std::lock_guard<std::mutex> lock(compile_mutex);
            if (!usable)
            {
                if (program_id != 0)
                    glDeleteProgram(program_id);

                program_id = glCreateProgram();
                vert_shader->Compile();
                frag_shader->Compile();
                

                glAttachShader(program_id, vert_shader->GetShaderId());
                glAttachShader(program_id, frag_shader->GetShaderId());

                glLinkProgram(program_id);

                int success;
                char info_log[512];
                glGetProgramiv(program_id, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(program_id, 512, NULL, info_log);
                    throw std::runtime_error("Failed to link shader program: " + std::string(info_log));
                }
                usable = true;
            }
        }
    }

    void ShaderProgram::Use()
    {
        if (usable)
            glUseProgram(program_id);
    }

    void ShaderProgram::Refresh()
    {
        sampler_count = 0;
    }
}