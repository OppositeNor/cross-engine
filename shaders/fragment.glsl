#version 330 core

out vec4 FragColor;

#define MAX_POINT_LIGHTS 4
struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;
};

uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform int point_light_count;

uniform vec4 camera_position;

in vec4 frag_position;
in vec2 frag_texture_uv;
in mat4 frag_tbn;

uniform sampler2D ftexture;
uniform samplerCube skybox;

struct Material {
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
};

uniform vec4 scaler_albedo;
uniform float scaler_metallic;
uniform float scaler_roughness;
uniform float scaler_ao;

vec4 albedo;
vec4 normal;
float metallic;
float roughness;
float ao;

uniform Material material;

const float PI = 3.141592653589793;


float TRGGX(vec4 p_normal, vec4 p_half);
float SchlickGGX(vec4 p_normal, vec4 p_vec, float p_k);
float GSmith(vec4 p_normal, vec4 p_to_camera, vec4 p_to_light);
vec4 FresnelSchlick(vec4 p_half, vec4 p_to_camera, vec4 f0);

void main()
{
    vec4 to_camera = normalize(camera_position - frag_position);

    albedo = scaler_albedo * texture(material.albedo, frag_texture_uv);
    normal = texture(material.normal, frag_texture_uv) * 2.0 - 1.0;
    normal.w = 0.0;
    normal = frag_tbn * normal;
    normal = normalize(normal);
    metallic = scaler_metallic * texture(material.metallic, frag_texture_uv).r;
    roughness = scaler_roughness * texture(material.roughness, frag_texture_uv).r;
    ao = scaler_ao * texture(material.ao, frag_texture_uv).r;


    vec4 temp_color = vec4(0.03) * albedo;
    vec4 f0 = mix(vec4(0.04), albedo, metallic);
    for (int i = 0; i < point_light_count; ++i)
    {
        vec4 to_light = normalize(point_light[i].position - frag_position);
        vec4 half = normalize(to_camera + to_light);

        vec4 F = FresnelSchlick(half, to_camera, f0);

        // specular
        vec4 specular = TRGGX(normal, half) * F * GSmith(normal, to_camera, to_light) 
            / (4.0 * max(dot(normal, to_camera), 0.0) * max(dot(normal, to_light), 0.0) + 0.001);
        vec4 kD = (vec4(1.0) - F) * (1 - metallic);

        float d_to_light = length(point_light[i].position - frag_position);
        vec4 radiance = point_light[i].color * point_light[i].intensity / (d_to_light * d_to_light);
        temp_color += (kD * albedo / PI + specular) * radiance * max(dot(normal, to_light), 0.0);
    }

    temp_color.w = 1.0;
    FragColor = temp_color * texture(ftexture, frag_texture_uv);
}

float TRGGX(vec4 p_normal, vec4 p_half)
{
    float alpha2 = roughness * roughness;
    return alpha2 / (PI * pow(pow(max(dot(p_normal, p_half), 0.0), 2) * (alpha2 - 1) + 1, 2));
}

float SchlickGGX(vec4 p_normal, vec4 p_vec, float p_k)
{
    float ndotv = max(dot(p_normal, p_vec), 0.0);
    return ndotv / (ndotv * (1 - p_k) + p_k);
}

float GSmith(vec4 p_normal, vec4 p_to_camera, vec4 p_to_light)
{
    float k = roughness * roughness / 2;
    return SchlickGGX(p_normal, p_to_camera, k) * SchlickGGX(p_normal, p_to_light, k);
}

vec4 FresnelSchlick(vec4 p_half, vec4 p_to_camera, vec4 f0)
{
    return f0 + (1 - f0) * pow(clamp(1.0 - dot(p_half, p_to_camera), 0.0, 1.0), 5.0);
}