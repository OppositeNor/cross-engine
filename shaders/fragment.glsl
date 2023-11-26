#version 330 core

out vec4 FragColor;

struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;
};

uniform PointLight point_light;

in vec4 frag_position;
in vec4 frag_normal;

void main()
{
    vec4 to_light = (point_light.position - frag_position);
    vec4 temp_color = point_light.color * (point_light.intensity / pow(to_light.length(), 2)) 
        * max(0, dot(frag_normal, normalize(to_light)));
    FragColor = vec4(temp_color.x, temp_color.y, temp_color.z, 1);

    // FragColor = frag_normal;
}