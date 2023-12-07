#pragma once
#include "ce/defs.hpp"


enum class TextureRepeatMode
{
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum class TextureFilterMode
{
    NEAREST,
    LINEAR
};

struct TextureConfig
{
    TextureConfig() = default;
    TextureConfig(
        bool p_mipmap, 
        TextureRepeatMode p_repeat_mode_h, 
        TextureRepeatMode p_repeat_mode_v, 
        TextureFilterMode p_filter_mode_min, 
        TextureFilterMode p_filter_mode_mag)
            : mipmap(p_mipmap), 
            repeat_mode_h(p_repeat_mode_h), 
            repeat_mode_v(p_repeat_mode_v), 
            filter_mode_min(p_filter_mode_min), 
            filter_mode_mag(p_filter_mode_mag) { }

    TextureRepeatMode repeat_mode_h = TextureRepeatMode::REPEAT;
    TextureRepeatMode repeat_mode_v = TextureRepeatMode::REPEAT;
    TextureFilterMode filter_mode_min = TextureFilterMode::LINEAR;
    TextureFilterMode filter_mode_mag = TextureFilterMode::LINEAR;
    bool mipmap = true;
};
class ATexture
{
protected:
    size_t width = 0;
    size_t height = 0;
    size_t channels = 0;
    TextureConfig config;

public:
    ATexture(const TextureConfig& p_config = TextureConfig());
    virtual ~ATexture();
    
    /**
     * @brief Get the width of the texture.
     * 
     * @return size_t The width of the texture.
     */
    FORCE_INLINE size_t GetWidth() const { return width; }

    /**
     * @brief Get the height of the texture.
     * 
     * @return size_t The height of the texture.
     */
    FORCE_INLINE size_t GetHeight() const { return height; }

    /**
     * @brief Get the number of channels of the texture.
     * 
     * @return size_t The number of channels of the texture.
     */
    FORCE_INLINE size_t GetChannels() const { return channels; }

    /**
     * @brief Load a texture from a file.
     * 
     * @param p_path The path to the texture file.
     */
    virtual void LoadTexture(const std::string& p_path) = 0;

    /**
     * @brief Load a texture.
     * 
     * @param p_data The texture data.
     * @param p_width The width of the texture.
     * @param p_height The height of the texture.
     * @param p_channels The channels of the texture.
     */
    virtual void LoadTexture(ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels) = 0;

    /**
     * @brief Bind the texture.
     * 
     */
    virtual void BindTexture() const = 0;
};