#pragma once
#include <mutex>
#include "ce/defs.hpp"
#include "ce/math/math.hpp"
#include "ce/materials/material.h"
#include <vector>
#include <mutex>

namespace CrossEngine
{
    class Window;
    class Triangle;
    class TextureConfig;
    class AMaterial;
    class ATexture;
    class Graphics
    {
    private:
        Graphics() = delete;
        inline static bool initialized = false;
        inline static std::mutex init_mutex;

        static std::shared_ptr<ATexture> default_albedo;
        static std::shared_ptr<ATexture> default_normal;
        static std::shared_ptr<ATexture> default_metallic;
        static std::shared_ptr<ATexture> default_roughness;
        static std::shared_ptr<ATexture> default_ao;
        static std::shared_ptr<AMaterial> default_material;

    public:

        inline static const ubyte_t WHITE_IMAGE[] = {
            0xFF, 0xFF, 0xFF, 0xFF
        };

        FORCE_INLINE static std::shared_ptr<ATexture> GetDefaultAlbedo() noexcept { return default_albedo; }
        FORCE_INLINE static std::shared_ptr<ATexture> GetDefaultNormal() noexcept { return default_normal; }
        FORCE_INLINE static std::shared_ptr<ATexture> GetDefaultMetallic() noexcept { return default_metallic; }
        FORCE_INLINE static std::shared_ptr<ATexture> GetDefaultRoughness() noexcept { return default_roughness; }
        FORCE_INLINE static std::shared_ptr<ATexture> GetDefaultAO() noexcept { return default_ao; }
        FORCE_INLINE static std::shared_ptr<AMaterial> GetDefaultMaterial() noexcept { return default_material; }

        /**
         * @brief Initialize graphics.
         * @throw std::runtime_error Failed to initialize GLFW.
         */
        static void InitGraphics();
        /**
         * @brief Terminate graphics.
         */
        static void TerminateGraphics();
        /**
         * @brief Create glfw window context.
         * 
         * @param p_width The width of the window.
         * @param p_height The height of the window.
         * @param p_title The title of the window.
         * @param p_shared The context that this context share resource with.
         * 
         * @throw std::runtime_error Failed to create GLFW window.
         * @throw std::runtime_error Graphics not initialized.
         * @return void* The created window context.
         */
        static void* CreateGLFWContext(size_t p_width, size_t p_height, const std::string& p_title, void* p_shared);
        
        /**
         * @brief Create glfw window context.
         * 
         * @param p_width The width of the window.
         * @param p_height The height of the window.
         * @param p_title The title of the window.
         * @param p_fullscreen Is the window fullscreen.
         * @param p_resizable Is the window resizable.
         * @param p_shared The context that this context share resource with.
         * 
         * @throw std::runtime_error Failed to create GLFW window.
         * @throw std::runtime_error Graphics not initialized.
         * @return void* THe created window context.
         */
        static void* CreateGLFWContext(size_t p_width, size_t p_height, const std::string& p_title, bool p_fullscreen, bool p_resizable, void* p_shared);

        /**
         * @brief Destroy the window context.
         * 
         * @param p_context The context to be destroyed.
         */
        static void DestroyGLFWContex(void* p_context);

        /**
         * @brief Generate VBO from vertices.
         * 
         * @param p_vao The VAO to generate VBO from.
         * @param p_vertices The vertices to generate VBO from.
         * @param p_size The size of the vertices.
         * @return unsigned int The ID of the VBO.
         */
        static unsigned int GenerateVBO(unsigned int p_vao, float* p_vertices, size_t p_size, const Window* p_context);

        /**
         * @brief Generate a texture.
         * 
         * @return unsigned int The ID of the texture.
         */
        static unsigned int GenerateTexture(const Window* p_context);

        /**
         * @brief Delete a texture.
         * 
         * @param p_texture_id The texture ID to be deleted
         * @param p_context The context that the texture is in.
         */
        static void DeleteTexture(unsigned int p_texture_id, const Window* p_context);

        /**
         * @brief Configure a texture.
         * 
         * @param p_texture_id The texture ID to be configured.
         * @param p_config The configuration of the texture.
         */
        static void ConfigTexture(unsigned int p_texture_id, const TextureConfig& p_config);

        /**
         * @brief Set the texture data.
         * 
         * @param p_texture_id The texture ID to be set.
         * @param p_width The width of the texture.
         * @param p_height The height of the texture.
         * @param p_channels The channels of the texture.
         * @param p_data The data of the texture.
         */
        static void SetTexture(unsigned int p_texture_id, size_t p_width, size_t p_height, size_t p_channels, const ubyte_t* p_data, bool p_mipmap);

        /**
         * @brief Get the size of the screen.
         * 
         * @return Math::Vec2s The size of the screen.
         */
        static Math::Vec2s GetScreenSize();

        /**
         * @brief Called every frame in the main thread.
         * 
         */
        static void Update();
    };


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
}