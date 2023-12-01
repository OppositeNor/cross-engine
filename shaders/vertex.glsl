#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;

out vec4 frag_position;
out vec4 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * pos;
    frag_normal = mat4(transpose(inverse(mat3(model)))) * normal;
    frag_position = model * pos;
}