#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(a_Position, 1.0);
}

#define type geometry
#version 450
layout (triangles_adjacency) in; // 6 vertices
layout (triangle_strip, max_vertices = 18) out;

struct Light
{
    vec3 position;
    vec3 color;
};

uniform mat4 u_ViewProjection;
uniform Light u_Light;

float EPSILON = 0.01;

void ExtrudeEdge(vec3 startVertex, vec3 endVertex)
{
    // Start vertex. Original and projected to infinity
    vec3 lightDir = normalize(startVertex - u_Light.position);
    gl_Position = u_ViewProjection * vec4((startVertex + lightDir * EPSILON), 1.0);
    EmitVertex();
    gl_Position = u_ViewProjection * vec4(lightDir, 0.0);
    EmitVertex();

    // End vertex. Original and projected to infinity
    lightDir = normalize(endVertex - u_Light.position);
    gl_Position = u_ViewProjection * vec4((endVertex + lightDir * EPSILON), 1.0);
    EmitVertex();
    gl_Position = u_ViewProjection * vec4(lightDir, 0.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 lightDirs[3];  // Direction towards light in each vertex of a triangle
    vec3 normals[3];    // Normal in each vertex of a triangle

    // Vertices as vec3
    vec3 vertPos[6];
    for (int i = 0; i < 6; i++) {
        vertPos[i] = gl_in[i].gl_Position.xyz;
    }

    // Edges (first three belong to main triangle)
    vec3 edge[6];
    edge[0] = vertPos[2] - vertPos[0];
    edge[1] = vertPos[4] - vertPos[2];
    edge[2] = vertPos[0] - vertPos[4];
    edge[3] = vertPos[1] - vertPos[0];
    edge[4] = vertPos[3] - vertPos[2];
    edge[5] = vertPos[5] - vertPos[4];

    vec3 mainNormal = cross(edge[0], edge[1]); // normal of main triangle
    vec3 lightPos = u_Light.position;
    vec3 lightDir = normalize(vertPos[0] - lightPos);

    // if main triangle not facing light, ignore (do nothing)
    if (dot(mainNormal, lightDir) > 0) return;

    // If it does, check its edges and extrude if needed
    for (int i = 0; i < 3; i++) {
        // Compute indices of start, end and neighbor vertice
        int start = i * 2;
        int neighbor = i * 2 + 1;
        int end = (i * 2 + 2) % 6;

        // compute normal of neighbor triangle (same in each vertex)
        vec3 normal = cross(edge[i + 3], edge[i]);

        // compute light direction in each vertex pos
        lightDirs[0] = normalize(vertPos[start] - lightPos);
        lightDirs[1] = normalize(vertPos[neighbor] - lightPos);
        lightDirs[2] = normalize(vertPos[end] - lightPos);

        ExtrudeEdge(vertPos[start], vertPos[end]);
    }

    // Render front cap
    for (int i = 0; i < 3; i++) {
        lightDir = normalize(vertPos[2 * i] - lightPos);
        gl_Position = u_ViewProjection * vec4((vertPos[2 * i] + lightDir * EPSILON), 1.0);
        EmitVertex();
    }
    EndPrimitive();

    // Render back cap
    int backCapIdx[3] = int[](0, 4, 2);
    for (int i = 0; i < 3; i++) {
        lightDir = normalize(vertPos[backCapIdx[i]] - lightPos);
        gl_Position = u_ViewProjection * vec4(lightDir, 0.0);
        EmitVertex();
    }
    EndPrimitive();
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

void main()
{
    color = vec4(0.0, 1.0, 0.0, 1.0);
}