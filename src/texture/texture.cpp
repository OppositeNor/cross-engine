#include "ce/texture/texture.h"
#include <glad/glad.h>

ATexture::ATexture(const TextureConfig& p_config)
    : config(p_config)
{
}

ATexture::~ATexture()
{
}