#include "ce/graphics/texture/static_texture.h"
#include "ce/resource/resource.h"
#include <glad/glad.h>

void StaticTexture::CreateTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    switch (config.repeat_mode_h)
    {
    case TextureRepeatMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        break;
    case TextureRepeatMode::MIRRORED_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        break;
    case TextureRepeatMode::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case TextureRepeatMode::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        break;
    }
    switch (config.repeat_mode_v)
    {
    case TextureRepeatMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case TextureRepeatMode::MIRRORED_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    case TextureRepeatMode::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case TextureRepeatMode::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        break;
    }
    if (config.mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        switch (config.filter_mode_min)
        {
        case TextureFilterMode::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case TextureFilterMode::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        }
    
    }
    switch (config.filter_mode_mag)
    {
    case TextureFilterMode::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case TextureFilterMode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }
}

StaticTexture::StaticTexture(const std::string& p_path, const TextureConfig& p_config)
    : ATexture(p_config)
{
    LoadTexture(p_path);
}

StaticTexture::~StaticTexture()
{
    if (texture != 0)
        glDeleteTextures(1, &texture);
}

void StaticTexture::LoadTexture(const std::string& p_path)
{
    if (texture == 0)
        CreateTexture();
    auto data = std::unique_ptr<ubyte_t[]>(Resource::LoadTextureImage(p_path, nullptr, 0, width, height, channels));
    glBindTexture(GL_TEXTURE_2D, texture);
    switch (channels)
    {
    case 1:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.get());
        break;
    case 3:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        break;
    case 4:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
        break;
    default:
        throw std::runtime_error("Unsupported number of channels.");
    }
    if (config.mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
};

void StaticTexture::LoadTexture(ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels)
{
    if (texture == 0)
        CreateTexture();
    width = p_width;
    height = p_height;
    channels = p_channels;
    glBindTexture(GL_TEXTURE_2D, texture);
    switch (channels)
    {
    case 1:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, p_data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;
    case 3:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, p_data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);
        break;
    default:
        throw std::runtime_error("Unsupported number of channels.");
    }
    if (config.mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
}

void StaticTexture::BindTexture() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}