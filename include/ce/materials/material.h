#pragma once
#include "ce/defs.hpp"
#include <memory>

class ShaderProgram;
class ATexture;
class Window;
class AMaterial
{
protected:
    std::unique_ptr<ATexture> albedo;
    std::unique_ptr<ATexture> normal;
    std::unique_ptr<ATexture> metallic;
    std::unique_ptr<ATexture> roughness;
    std::unique_ptr<ATexture> ao;

    const Window* context;
public:
    AMaterial(const Window* p_context);
    virtual ~AMaterial();

    FORCE_INLINE std::string GetUniformName() const noexcept { return "material"; };

    virtual void SetUniform(const ShaderProgram* p_shader_program) const = 0;
};