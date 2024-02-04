#include "ce/component/skybox.h"
#include "ce/graphics/window.h"
#include "ce/resource/resource.h"
#include "ce/graphics/graphics.h"
#include "ce/game/game.h"

#include <glad/glad.h>

namespace CrossEngine
{
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

    void Skybox::SetSkyboxTexture(const std::vector<std::string>& p_faces, unsigned int p_texture_id)
    {
        if (p_faces.size() != 6)
            throw std::runtime_error("Skybox must have 6 faces.");
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, p_texture_id);
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

    Skybox::Skybox(const std::vector<std::string>& p_faces)
        : faces(p_faces)
    {
        Scale() = Math::Vec4(10.0f, 10.0f, 10.0f);
    }

    void Skybox::SetupSkybox(unsigned int p_vao, unsigned int p_vbo, unsigned int p_texture_id)
    {
        glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
        glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), vertices, GL_STATIC_DRAW);
        
        glBindVertexArray(p_vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        SetSkyboxTexture(faces, p_texture_id);
    }

    Skybox::~Skybox()
    {
        if (!Game::IsInitialized())
            return;
        for (auto& i : vbos)
        {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                i.first->FreeThreadResource(i.second);
        }
        for (auto& i : vaos)
        {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                i.first->FreeThreadResource(i.second);
        }
        for (auto& i : texture_cube_ids)
        {
            if (Game::GetInstance()->IsContextAvailable(i.first))
                i.first->FreeThreadResource(i.second);
        }
    }

    void Skybox::Draw(Window* p_context)
    {
        if (!vbos.contains(p_context))
        {
            unsigned int vbo, vao, texture_id;
            
            glGenBuffers(1, &vbo);
            p_context->RegisterThreadResource(vbo, glDeleteBuffers);
            glGenVertexArrays(1, &vao);
            p_context->RegisterThreadResource(vao, glDeleteVertexArrays);
            glGenTextures(1, &texture_id);
            p_context->RegisterThreadResource(texture_id, glDeleteTextures);
            SetupSkybox(vao, vbo, texture_id);
            vbos[p_context] = vbo;
            vaos[p_context] = vao;
            texture_cube_ids[p_context] = texture_id;
        }
        glDepthMask(GL_FALSE);
        p_context->GetSkyboxShaderProgram()->SetUniform("model", GetSubspaceMatrix());
        glBindVertexArray(vaos[p_context]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cube_ids[p_context]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }
}