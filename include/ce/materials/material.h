#pragma once
#include "ce/defs.hpp"
#include <memory>
#include <mutex>

namespace CrossEngine
{
    class ShaderProgram;
    class ATexture;
    class Window;
    class Triangle;
    class AMaterial
    {
    protected:
        std::shared_ptr<ATexture> albedo;
        std::shared_ptr<ATexture> normal;
        std::shared_ptr<ATexture> metallic;
        std::shared_ptr<ATexture> roughness;
        std::shared_ptr<ATexture> ao;
        
        const bool should_prioritize;
    public:
        /**
         * @brief Constructor of AMaterial.
         * 
         * @param p_should_prioritize Should prioritize.
         */
        AMaterial(bool p_should_prioritize = true);

        /**
         * @brief Destructor of AMaterial.
         */
        virtual ~AMaterial();

        /**
         * @brief Should the material be prioritized.
         * 
         * @return true The material should be prioritized.
         * @return false The material should not be prioritized.
         */
        FORCE_INLINE bool ShouldPrioritize() const noexcept { return should_prioritize; }

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