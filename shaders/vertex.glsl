#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texture_uv;
layout (location = 3) in vec4 tangent;

out vec4 frag_position;
out vec2 frag_texture_uv;
out mat4 frag_tbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * pos;
    vec3 T = normalize(model * tangent).xyz;
    vec3 N = normalize(mat4(transpose(inverse(mat3(model)))) * normal).xyz;
    vec4 B = vec4(cross(T, N), 0);
    frag_tbn = mat4(vec4(T, 0), B, vec4(N, 0), vec4(0.0, 0.0, 0.0, 1.0));

    frag_position = model * pos;
    frag_texture_uv = texture_uv;
}