#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texture_uv;

out vec4 frag_position;
out vec4 frag_normal;
out vec2 frag_texture_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * pos;
    frag_normal = mat4(transpose(inverse(mat3(model)))) * normal;
    frag_position = model * pos;
    frag_texture_uv = texture_uv;
}