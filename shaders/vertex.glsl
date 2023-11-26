#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;

out vec4 frag_position;
out vec4 frag_normal;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * pos;
    frag_normal = normal;
    frag_position = model * pos;
}