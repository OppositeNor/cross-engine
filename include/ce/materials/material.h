#pragma once
#include "ce/defs.hpp"

class ShaderProgram;
class AMaterial
{
public:
    AMaterial() {}
    virtual ~AMaterial() {}

    virtual std::string GetUniformName() const = 0;

    virtual void SetUniform(const ShaderProgram* p_shader_program) const = 0;
};