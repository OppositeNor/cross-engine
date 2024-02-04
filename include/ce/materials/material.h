#pragma once
#include "ce/defs.hpp"
#include <memory>
#include <mutex>

namespace CrossEngine
{
    class ShaderProgram;
    class ATexture;
    class Window;
    class AMaterial
    {
    protected:
        std::shared_ptr<ATexture> albedo;
        std::shared_ptr<ATexture> normal;
        std::shared_ptr<ATexture> metallic;
        std::shared_ptr<ATexture> roughness;
        std::shared_ptr<ATexture> ao;
    public:
        /**
         * @brief Constructor of AMaterial.
         * 
         * @param p_context The context of the material.
         */
        AMaterial();

        /**
         * @brief Destructor of AMaterial.
         */
        virtual ~AMaterial();

        /**
         * @brief Get the name of the uniform.
         * 
         * @return std::string The name of the uniform. 
         */
        FORCE_INLINE std::string GetUniformName() const noexcept { return "material"; };

        /**
         * @brief Set the uniform of the material.
         * 
         * @param p_context The context to set the uniform on.
         * @param p_shader_program The shader program to set the uniform.
         */
        virtual void SetUniform(Window* p_context) const = 0;
    };
}