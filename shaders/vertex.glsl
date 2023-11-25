#version 330 core

layout (location = 0) in vec4 pos;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * pos;
}