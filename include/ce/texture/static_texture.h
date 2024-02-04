#pragma once
#include "ce/texture/texture.h"
#include <map>

namespace CrossEngine
{
    class ShaderProgram;
    class Window;
    class StaticTexture : public ATexture
    {
    private:
        std::unique_ptr<ubyte_t[]> data;
        std::map<Window*, unsigned int> texture_ids;

    public:

        StaticTexture(const TextureConfig& p_config = TextureConfig());

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
        virtual void LoadTexture(ubyte_t*&& p_data, size_t p_width, size_t p_height, size_t p_channels) override;
        
        /**
         * @brief Load a texture from a file.
         * 
         * @param p_path The path to the texture file.
         */
        virtual void LoadTexture(const ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels) override;

        /**
         * @brief Bind the texture.
         */
        virtual void BindTexture(Window* p_context, const std::string& p_uniform_name) override;

        virtual ~StaticTexture() override;
    };
}