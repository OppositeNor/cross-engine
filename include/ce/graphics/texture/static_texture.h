#pragma once
#include "ce/graphics/texture/texture.h"

class StaticTexture : public ATexture
{
private:
    unsigned int texture = 0;

    void CreateTexture();
public:

    StaticTexture(const std::string& p_path, const TextureConfig& p_config = TextureConfig());

    /**
     * @brief Get the number of channels of the texture.
     * 
     * @return size_t The number of channels of the texture.
     */
    virtual void LoadTexture(const std::string& p_path) override;
    
    /**
     * @brief Load a texture from a file.
     * 
     * @param p_path The path to the texture file.
     */
    virtual void LoadTexture(ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels) override;

    /**
     * @brief Bind the texture.
     */
    virtual void BindTexture() const override;

    virtual ~StaticTexture() override;
};