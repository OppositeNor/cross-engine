#include "ce/texture/texture.h"
#include <glad/glad.h>

namespace CrossEngine
{
    ATexture::ATexture(const TextureConfig& p_config)
        : config(p_config)
    {
    }

    ATexture::~ATexture()
    {
    }
}