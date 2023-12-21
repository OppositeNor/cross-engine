#include "ce/texture/static_texture.h"
#include "ce/resource/resource.h"
#include "ce/graphics/shader/shader_program.h"

void StaticTexture::CreateTexture()
{
    texture = Graphics::GenerateTexture(context);
    Graphics::ConfigTexture(texture, config);
}

StaticTexture::StaticTexture(const Window* p_context, const TextureConfig& p_config)
    : context(p_context), ATexture(p_config)
{

}

StaticTexture::StaticTexture(const Window* p_context, const std::string& p_path, const TextureConfig& p_config)
    : context(p_context), ATexture(p_config)
{
    LoadTexture(p_path);
}

StaticTexture::~StaticTexture()
{
    if (texture != 0)
        Graphics::DeleteTexture(texture, context);
}

void StaticTexture::LoadTexture(const std::string& p_path)
{
    if (texture == 0)
        CreateTexture();
    auto data = std::unique_ptr<ubyte_t[]>(Resource::LoadTextureImage(p_path, nullptr, 0, width, height, channels));
    LoadTexture(data.get(), width, height, channels);
};

void StaticTexture::LoadTexture(const ubyte_t* p_data, size_t p_width, size_t p_height, size_t p_channels)
{
    if (texture == 0)
        CreateTexture();
    width = p_width;
    height = p_height;
    channels = p_channels;
    
    Graphics::SetTexture(texture, width, height, channels, p_data, config.mipmap);
}

void StaticTexture::BindTexture(const ShaderProgram* shader_program, const std::string& p_uniform_name) const
{
    shader_program->SetSampler2DUniform(p_uniform_name, texture);
}