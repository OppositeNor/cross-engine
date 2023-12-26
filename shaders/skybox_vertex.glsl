#version 330 core

layout (location = 0) in vec3 vert_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 texture_pos;

void main()
{
    texture_pos = vert_pos;
    vec4 pos = view * model * vec4(vert_pos, 0.0);
    pos.w = 1.0;
    pos = proj * pos;
    gl_Position = pos;
}