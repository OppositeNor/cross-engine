#include "ce/texture/static_texture.h"
#include "ce/resource/resource.h"
#include "ce/graphics/window.h"
#include "ce/graphics/renderer/renderer.h"
#include "ce/game/game.h"

namespace CrossEngine
{
    StaticTexture::StaticTexture(const TextureConfig& p_config)
        : ATexture(p_config)
    {

    }

    StaticTexture::StaticTexture(const std::string& p_path, const TextureConfig& p_config)
        : ATexture(p_config)
    {
        LoadTexture(p_path);
    }

    StaticTexture::~StaticTexture()
    {
        if (!Game::IsInitialized())
            return;
        for (auto& i : texture_ids)
        {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                Graphics::DeleteTexture(i.second, i.first);
        }
    }

    void StaticTexture::LoadTexture(const std::string& p_path)
    {
        data = std::unique_ptr<ubyte_t[]>(Resource::LoadTextureImage(p_path, nullptr, 0, width, height, channels));
    };

    void StaticTexture::LoadTexture(ubyte_t*&& p_data, size_t p_width, size_t p_height, size_t p_channels)
    {
        width = p_width;
        height = p_height;
        channels = p_channels;
        data = std::unique_ptr<ubyte_t[]>(p_data);
        p_data = nullptr;
    }

    void StaticTexture::LoadTexture(const ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels)
    {
        width = p_width;
        height = p_height;
        channels = p_channels;
        auto size = width * height * channels;
        data = std::unique_ptr<ubyte_t[]>(new ubyte_t[size]);
        memcpy(data.get(), p_data, size);
    }

    void StaticTexture::BindTexture(Window* p_context, const std::string& p_uniform_name)
    {
        if (!texture_ids.contains(p_context))
        {
            unsigned int texture = Graphics::GenerateTexture(p_context);
            Graphics::SetTexture(texture, width, height, channels, data.get(), config.mipmap);
            Graphics::ConfigTexture(texture, config);
            texture_ids[p_context] = texture;
        }
        p_context->GetRenderer()->GetShaderProgram()->SetSampler2DUniform(p_uniform_name, texture_ids[p_context]);
    }
}