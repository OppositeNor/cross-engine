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

#define MAX_PARALLEL_LIGHTS 4
struct ParallelLight {
    vec4 direction;
    vec4 color;
    vec4 ambient;
    float intensity;
};

uniform ParallelLight parallel_light[MAX_PARALLEL_LIGHTS];
uniform int parallel_light_count;

uniform vec4 camera_position;

in vec4 frag_position;
in vec2 frag_texture_uv;
in mat4 frag_tbn;

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

vec4 albedo;
vec4 normal;
float metallic;
float roughness;
float ao;

uniform Material material;

const float PI = 3.141592653589793;

vec4 ShadeColor(vec4 p_to_camera, vec4 p_to_light, float p_d_to_light, vec4 p_normal, vec4 p_color, float p_intensity);

float TRGGX(vec4 p_normal, vec4 p_half);
float SchlickGGX(float p_dot_norm_vec, float p_k);
float GSmith(float p_dot_normal_cam, float p_dot_normal_light);
vec4 FresnelSchlick(vec4 p_half, vec4 p_to_camera, vec4 f0);

vec4 f0;
void main()
{
    vec4 to_camera = normalize(camera_position - frag_position);

    albedo = scaler_albedo * texture(material.albedo, frag_texture_uv);
    normal = texture(material.normal, frag_texture_uv) * 2.0 - vec4(1.0);
    normal.w = 0.0;
    normal = normalize(frag_tbn * normal);
    metallic = scaler_metallic * texture(material.metallic, frag_texture_uv).r;
    roughness = scaler_roughness * texture(material.roughness, frag_texture_uv).r;
    ao = texture(material.ao, frag_texture_uv).r;


    f0 = mix(vec4(0.04), albedo, metallic);
    vec4 temp_color;
    for (int i = 0; i < max(point_light_count, MAX_POINT_LIGHTS); ++i)
    {
        vec4 to_light = normalize(point_light[i].position - frag_position);
        temp_color += ShadeColor(to_camera, to_light, length(point_light[i].position - frag_position), 
            normal, point_light[i].color, point_light[i].intensity);
    }
    for (int i = 0; i < max(parallel_light_count, MAX_PARALLEL_LIGHTS); ++i)
    {
        temp_color += ShadeColor(to_camera, normalize(-1 * parallel_light[i].direction), 1, normal, 
            parallel_light[i].color, parallel_light[i].intensity);
        temp_color += parallel_light[i].ambient * albedo;
    }
    temp_color *= ao;

    temp_color[3] = albedo[3];
    FragColor = temp_color;
}

vec4 ShadeColor(vec4 p_to_camera, vec4 p_to_light, float p_d_to_light, vec4 p_normal, vec4 p_color, float p_intensity)
{
    vec4 half_vec = normalize(p_to_camera + p_to_light);

    vec4 F = FresnelSchlick(half_vec, p_to_camera, f0);

    float dot_normal_light = max(dot(p_normal, p_to_light), 0.0);
    float dot_normal_cam = max(dot(p_normal, p_to_camera), 0.0);
    vec4 specular = TRGGX(p_normal, half_vec) * F * GSmith(dot_normal_cam, dot_normal_light)
        / (4.0 * dot_normal_cam * dot_normal_light + 0.001);
    vec4 kD = (vec4(1.0) - F) * (1 - metallic);

    vec4 irradiance = p_color * p_intensity / (p_d_to_light * p_d_to_light);
    return (kD * albedo / PI + specular) * irradiance * dot_normal_light;
}

float TRGGX(vec4 p_normal, vec4 p_half)
{
    float alpha2 = roughness * roughness;
    return alpha2 / (PI * pow(pow(max(dot(p_normal, p_half), 0.0), 2) * (alpha2 - 1) + 1, 2));
}

float SchlickGGX(float p_dot_norm_vec, float p_k)
{
    return p_dot_norm_vec / (p_dot_norm_vec * (1 - p_k) + p_k);
}

float GSmith(float p_dot_normal_cam, float p_dot_normal_light)
{
    float k = roughness * roughness / 2;
    return SchlickGGX(p_dot_normal_cam, k) * SchlickGGX(p_dot_normal_light, k);
}

vec4 FresnelSchlick(vec4 p_half, vec4 p_to_camera, vec4 f0)
{
    return f0 + (1 - f0) * pow(clamp(1.0 - dot(p_half, p_to_camera), 0.0, 1.0), 5.0);
}