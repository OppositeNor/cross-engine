#include "ce/materials/material.h"
#include "ce/graphics/graphics.h"
#include "ce/texture/static_texture.h"

namespace CrossEngine
{
    AMaterial::AMaterial(bool p_should_prioritize)
        : should_prioritize(p_should_prioritize)
    {
    }

    AMaterial::~AMaterial()
    {
    }
}