#pragma once
#include "ce/defs.hpp"

namespace CrossEngine
{
    class AShader
    {
    protected:

        std::string shader_path;
        unsigned int shader_id;
    public:
        /**
         * @brief Constructor for AShader.
         * 
         * @param p_shader_path The path to the shader file.
         */
        AShader(const std::string& p_shader_path) noexcept
            : shader_path(p_shader_path) {}

        /**
         * @brief Destructor for AShader.
         */
        virtual ~AShader();

        /**
         * @brief Get the shader id.
         * 
         * @return unsigned int The shader id.
         */
        FORCE_INLINE unsigned int GetShaderId() const { return shader_id; }

        /**
         * @brief Compile the shader.
         */
        virtual void Compile();

        /**
         * @brief Get the shader type.
         * 
         * @return unsigned int The shader type. 
         */
        virtual unsigned int GetShaderType() const = 0;
    };
}