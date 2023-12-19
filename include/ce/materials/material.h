#pragma once
#include "ce/defs.hpp"
#include <memory>
#include <mutex>

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

    const Window* context;
public:
    AMaterial(const Window* p_context);
    virtual ~AMaterial();

    FORCE_INLINE std::string GetUniformName() const noexcept { return "material"; };

    virtual void SetUniform(const ShaderProgram* p_shader_program) const = 0;
};