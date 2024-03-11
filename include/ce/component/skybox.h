#pragma once

#include "ce/component/component3D.h"
#include "ce/graphics/shader/shader_program.h"
#include <map>

namespace CrossEngine
{
    class Skybox
        : public Component3D
    {
    private:
        const static float vertices[108];

        std::map<Window*, unsigned int> vbos;
        std::map<Window*, unsigned int> vaos;
        std::map<Window*, unsigned int> texture_cube_ids;
        mutable std::shared_mutex context_resource_mutex;

        void SetupSkybox(unsigned int p_vao, unsigned int p_vbo, unsigned int p_texture_id);

        std::vector<std::string> faces;
    protected:

        /**
         * @brief Draw the skybox.
         * 
         */
        virtual void Draw(Window* p_context);
    public:

        /**
         * @brief Construct a new Skybox object.
         * 
         * @param p_context The context of the skybox.
         * @param p_faces The path to the images of faces of the skybox.
         */
        Skybox(const std::vector<std::string>& p_faces, const std::string& p_component_name = "skybox");
        
        /**
         * @brief Destroy the Skybox object
         * 
         */
        virtual ~Skybox() override;

        /**
         * @brief Set the texture of the skybox.
         * 
         * @param p_faces The pathes to the images of faces of the skybox.
         */
        void SetSkyboxTexture(const std::vector<std::string>& p_faces, unsigned int p_texture_id);

        /**
         * @brief Get the texture cube of the skybox.
         * 
         * @return std::map<Window*, unsigned int> The texture cube of the skybox.
         */
        FORCE_INLINE std::map<Window*, unsigned int> GetTextureCubeIDs() noexcept { return texture_cube_ids; }

        /**
         * @brief Get the texture cube of the skybox.
         * 
         * @return std::map<Window*, unsigned int> The texture cube of the skybox.
         */
        FORCE_INLINE const std::map<Window*, unsigned int>& GetTextureCubeIDs() const noexcept { return texture_cube_ids; }

        virtual bool RegisterDraw(Window* p_context) override;
    };
}