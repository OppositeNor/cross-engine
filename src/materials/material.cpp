#include "ce/materials/material.h"
#include "ce/graphics/graphics.h"
#include "ce/graphics/texture/static_texture.h"

AMaterial::AMaterial(const Window* p_context)
    : context(p_context)
{
}

AMaterial::~AMaterial()
{
}