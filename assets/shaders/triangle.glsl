#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);

    v_Color = a_Color;
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}