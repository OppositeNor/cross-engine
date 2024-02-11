#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 frag_position;

void main()
{
    frag_position = model * vec4(pos, 1.0);
    gl_Position = proj * view * frag_position;
}