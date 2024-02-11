#version 330 core

in vec4 frag_position;
out vec4 color;

void main()
{
    color = vec4(frag_position.z, frag_position.z, frag_position.z, 1.0);
}
