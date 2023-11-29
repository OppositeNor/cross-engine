#version 330 core

out vec4 FragColor;

#define MAX_POINT_LIGHTS 16
struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;
};

uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform int point_light_count;
uniform vec4 ambient_color;
uniform float ambient_intensity;

uniform vec4 camera_position;

in vec4 frag_position;
in vec4 frag_normal;

void main()
{
    vec4 to_light;
    vec4 temp_color = ambient_color * ambient_intensity;
    for (int i = 0; i < point_light_count; ++i)
    {
        to_light = (point_light[i].position - frag_position);
        temp_color += point_light[i].color * (point_light[i].intensity / pow(to_light.length(), 2)) 
            * max(0, dot(normalize(to_light), frag_normal));
    }
    FragColor = vec4(temp_color.x, temp_color.y, temp_color.z, 1);
}