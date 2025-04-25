#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPosition;

out vec3 v_Normal;
out vec3 v_FragPosition;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPosition;

uniform vec3 u_CameraPosition;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_CameraPosition - v_FragPosition);
    float ndv = max(dot(normal, viewDir), 0.2);
    color = vec4(vec3(1.0) * ndv, 1.0);
} 