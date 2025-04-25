#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out VS
{
    vec3 color;
} vs;

void main()
{
    vs.color = a_Color;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);

in VS
{
    vec3 color;
} vs;

void main()
{
    color = vec4(ambientColor * vs.color, 1.0);
}