#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 i_Offset;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
    gl_Position = u_MVP * vec4(a_Position + i_Offset, 1.0);

    v_TexCoord = a_TexCoord;
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}