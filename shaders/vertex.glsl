#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_uv;
layout (location = 3) in vec3 tangent;

out vec4 frag_position;
out vec2 frag_texture_uv;
out mat4 frag_tbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    frag_position = model * vec4(pos, 1.0);
    gl_Position = proj * view * frag_position;
    vec3 N = normalize(mat4(transpose(inverse(mat3(model)))) * vec4(normal, 0)).xyz;
    vec3 T = normalize(model * vec4(tangent, 0)).xyz;
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    frag_tbn = mat4(vec4(T, 0), vec4(B, 0), vec4(N, 0), vec4(0.0, 0.0, 0.0, 1.0));

    frag_texture_uv = texture_uv;
}