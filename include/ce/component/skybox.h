#pragma once

#include "ce/component/component.h"
#include "ce/graphics/shader/shader_program.h"

class Skybox
    : public Component
{
    const static float vertices[108];

    // for skybox, the texture is hardcoded into the class.
    unsigned int vbo = 0;
    unsigned int vao = 0;
    unsigned int texture_cube = 0;
public:

    /**
     * @brief Construct a new Skybox object.
     * 
     * @param p_context The context of the skybox.
     * @param p_faces The path to the images of faces of the skybox.
     */
    Skybox(Window* p_context, const std::vector<std::string>& p_faces);
    
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
    void SetSkyboxTexture(const std::vector<std::string>& p_faces);

    /**
     * @brief Get the texture cube of the skybox.
     * 
     * @return unsigned int The texture cube of the skybox.
     */
    FORCE_INLINE unsigned int GetTextureCube() const noexcept { return texture_cube; }

    /**
     * @brief Draw the skybox.
     * 
     */
    void Draw() const;
};