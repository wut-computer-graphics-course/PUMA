#define type vertex
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inDirection;
layout(location = 2) in float inThickness;
layout(location = 3) in float inAge;
layout(location = 4) in float inLifetime;

out VS_OUT {
    vec3 position;
    vec3 direction;
    float thickness;
    float age;
    float lifetime;
} vs_out;

void main() {
    vs_out.position = inPosition;
    vs_out.direction = inDirection;
    vs_out.thickness = inThickness;
    vs_out.age = inAge;
    vs_out.lifetime = inLifetime;
}

#define type geometry
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPos;

in VS_OUT {
    vec3 position;
    vec3 direction;
    float thickness;
    float age;
    float lifetime;
} gs_in[];

out vec2 vUV;
out float vAge;
out float vLifetime;

void main() {
    vec3 pos = gs_in[0].position;
    vec3 dir = normalize(gs_in[0].direction);
    float thickness = gs_in[0].thickness;
    float age = gs_in[0].age;
    float lifetime = gs_in[0].lifetime;

    vec3 cam_dir = normalize(u_CameraPos - pos);
    vec3 side = normalize(cross(dir, cam_dir)) * thickness * 0.5;
    vec3 up = normalize(cross(side, dir)) * thickness * 0.5;

    vec3 corners[4] = {
        pos - side - up,
        pos + side - up,
        pos - side + up,
        pos + side + up
    };
    vec2 uvs[4] = { vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1) };

    for (int i = 0; i < 4; ++i) {
        gl_Position = u_ViewProjection * vec4(corners[i], 1.0);
        vUV = uvs[i];
        vAge = age;
        vLifetime = lifetime;
        EmitVertex();
    }
    EndPrimitive();
}

#define type fragment
#version 450

in vec2 vUV;
in float vAge;
in float vLifetime;
out vec4 fragColor;

void main() {
    float alpha = 1.0 - (vAge / vLifetime);
    fragColor = vec4(1.0, 0.8, 0.2, alpha);
}