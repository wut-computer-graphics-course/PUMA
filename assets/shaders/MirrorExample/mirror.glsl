#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out VS
{
    vec3 worldPos;
    vec3 normal;
} vs;

void main()
{
    vs.worldPos = (u_Model * vec4(a_Position, 1.0)).xyz;
    vs.normal = (u_Model * vec4(a_Normal, 0.0)).xyz;
    vs.normal = normalize(vs.normal);
    gl_Position = u_ViewProjection * vec4(vs.worldPos, 1.0);
}

#define type fragment
#version 450

layout (location = 0) out vec4 fragColor;

const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);
const float kd = 0.5, ks = 0.2f, m = 100.0f;

struct Light
{
    vec3 position;
    vec3 color;
};

uniform vec3 u_CameraPos;
uniform Light u_Light;
uniform vec3 u_Color;
uniform float u_Alpha;

in VS
{
    vec3 worldPos;
    vec3 normal;
} vs;

void main()
{
    vec3 viewDir = normalize(u_CameraPos - vs.worldPos);
    vec3 color = u_Color * ambientColor;

    vec3 lightPos = u_Light.position;
    vec3 lightColor = u_Light.color;
    vec3 lightDir = normalize(lightPos - vs.worldPos);
    vec3 halfVec = normalize(viewDir + lightDir);
    color += lightColor * u_Color * kd * clamp(dot(vs.normal, lightDir), 0.0, 1.0); // diffuse color
    float nh = dot(vs.normal, halfVec);
    nh = clamp(nh, 0.0, 1.0);
    nh = pow(nh, m);
    nh *= ks;
    color += lightColor * nh;

    fragColor = vec4(clamp(color, 0.0, 1.0), u_Alpha);
}