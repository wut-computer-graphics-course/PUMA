#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

uniform vec3 u_Color;

void main()
{
    color = vec4(u_Color, 1.0);
}