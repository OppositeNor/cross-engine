#pragma once
#include "ce/graphics/texture/texture.h"

class ShaderProgram;
class Window;
class StaticTexture : public ATexture
{
private:
    unsigned int texture = 0;
    void CreateTexture();
    const Window* context = nullptr;

public:

    StaticTexture(const Window* p_context, const TextureConfig& p_config = TextureConfig());

    StaticTexture(const Window* p_context, const std::string& p_path, const TextureConfig& p_config = TextureConfig());

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
    virtual void LoadTexture(const ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels) override;

    /**
     * @brief Bind the texture.
     */
    virtual void BindTexture(const ShaderProgram* shader_program, const std::string& p_uniform_name) const override;

    virtual ~StaticTexture() override;
};