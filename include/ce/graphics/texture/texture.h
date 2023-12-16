#pragma once
#include "ce/defs.hpp"
#include "ce/graphics/graphics.h"

class ShaderProgram;
class Window;
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
    virtual void LoadTexture(const ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels) = 0;

    /**
     * @brief Bind the texture.
     * 
     */
    virtual void BindTexture(const ShaderProgram* shader_program, const std::string& p_uniform_name) const = 0;
};