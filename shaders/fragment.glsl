#version 330 core

out vec4 FragColor;

#define MAX_POINT_LIGHTS 4
struct PointLight {
    vec4 position;
    vec4 diffuse_color;
    float diffuse_intensity;
    vec4 specular_color;
    float specular_power;
    float specular_intensity;
};

uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform int point_light_count;
uniform vec4 ambient_color;
uniform float ambient_intensity;

uniform vec4 camera_position;

in vec4 frag_position;
in vec4 frag_normal;
in vec2 frag_texture_uv;

uniform sampler2D ftexture;

uniform samplerCube skybox;

void main()
{
    vec4 to_light;
    vec4 temp_color;
    vec4 temp_normal = normalize(frag_normal);
    temp_color = ambient_color * ambient_intensity;
    temp_color += texture(skybox, vec3(reflect(normalize(frag_position - camera_position), temp_normal)));
    for (int i = 0; i < point_light_count; ++i)
    {
        to_light = point_light[i].position - frag_position;
        temp_color += point_light[i].diffuse_color * (point_light[i].diffuse_intensity / pow(length(to_light), 2)) 
            * max(0, dot(normalize(to_light), temp_normal));
        temp_color += point_light[i].specular_color * (point_light[i].specular_intensity / pow(length(to_light), 2)) 
            * pow(max(0, dot(normalize(normalize(to_light) + normalize(camera_position - frag_position)), temp_normal)), 
            point_light[i].specular_power);
    }
    temp_color.w = 1.0;
    FragColor = temp_color * texture(ftexture, frag_texture_uv);
}