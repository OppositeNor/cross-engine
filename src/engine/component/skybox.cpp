#include "ce/component/skybox.h"
#include "ce/graphics/window.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"

#include <glad/glad.h>

const float Skybox::vertices[108] = {
    // top
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    
    // bottom
    -1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,

    // left
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    
    // right
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,

    // front
    -1.0f, -1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f, 
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

    // back
    -1.0f, -1.0f, -1.0f, 
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f
};

void Skybox::SetSkyboxTexture(const std::vector<std::string>& p_faces)
{
    if (p_faces.size() != 6)
        throw std::runtime_error("Skybox must have 6 faces.");
    if (GetContext()->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("Skybox must be created on the main thread.");
    if (texture_cube == 0)
    {
        glGenTextures(1, &texture_cube);
        GetContext()->RegisterThreadResource(texture_cube, glDeleteTextures);
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cube);
    size_t img_width, img_height, img_channels;
    Resource::GetImageSize(p_faces[0], img_width, img_height, img_channels);
    size_t buffer_size = img_width * img_height * img_channels;
    std::unique_ptr<byte[]> buffer = 
        std::unique_ptr<byte[]>(new byte[buffer_size]);
    std::vector<unsigned char> image_data;
    for (size_t i = 0; i < p_faces.size(); ++i)
    {
        Resource::LoadTextureImage(p_faces[i], buffer.get(), buffer_size, img_width, img_height, img_channels);
        switch (img_channels)
        {
        case 1:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RED, img_width, img_height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer.get());
            break;
        case 3:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.get());
            break;
        case 4:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
            break;
        default:
            throw std::runtime_error("Invalid image format.");
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Skybox::Skybox(Window* p_context, const std::vector<std::string>& p_faces)
    : Component(p_context)
{
    Scale() =Math::Vec4(10.0f, 10.0f, 10.0f);
    if (p_context->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("Skybox must be created on the main thread.");
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), vertices, GL_STATIC_DRAW);
    GetContext()->RegisterThreadResource(vbo, glDeleteBuffers);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    GetContext()->RegisterThreadResource(vao, glDeleteVertexArrays);

    glGenTextures(1, &texture_cube);
    GetContext()->RegisterThreadResource(texture_cube, glDeleteTextures);
    SetSkyboxTexture(p_faces);

}

Skybox::~Skybox()
{
    GetContext()->FreeThreadResource(vbo);
    GetContext()->FreeThreadResource(vbo);
    GetContext()->FreeThreadResource(texture_cube);
    vbo = 0;
    vao = 0;
    texture_cube = 0;
}

void Skybox::Draw() const
{
    if (GetContext()->GetThreadId() != std::this_thread::get_id())
        throw std::runtime_error("Skybox must be drawn on the main thread.");
    glDepthMask(GL_FALSE);
    GetContext()->GetSkyboxShaderProgram()->SetUniform("model", GetSubspaceMatrix());
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cube);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}