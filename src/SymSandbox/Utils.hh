#ifndef PUMA_UTILS_HH
#define PUMA_UTILS_HH

#include "SymBase.hh"

namespace sym
{
  static std::pair<std::vector<sym_base::Vertex>, std::vector<uint32_t>> generate_cuboid(glm::vec3 dims)
  {
    std::vector<sym_base::Vertex> vertices;
    std::vector<uint32_t> indices;

    float a2 = dims.x / 2;
    float b2 = dims.y / 2;
    float c2 = dims.z / 2;

    glm::vec3 normals[] = {
      { 0, 0, 1 },  // front
      { 0, 0, -1 }, // back
      { -1, 0, 0 }, // left
      { 1, 0, 0 },  // right
      { 0, 1, 0 },  // top
      { 0, -1, 0 }  // bottom
    };

    glm::vec3 positions[6][4] = { // Front face
                                  { { -a2, -b2, c2 }, { a2, -b2, c2 }, { a2, b2, c2 }, { -a2, b2, c2 } },
                                  // Back face
                                  { { a2, -b2, -c2 }, { -a2, -b2, -c2 }, { -a2, b2, -c2 }, { a2, b2, -c2 } },
                                  // Left face
                                  { { -a2, -b2, -c2 }, { -a2, -b2, c2 }, { -a2, b2, c2 }, { -a2, b2, -c2 } },
                                  // Right face
                                  { { a2, -b2, c2 }, { a2, -b2, -c2 }, { a2, b2, -c2 }, { a2, b2, c2 } },
                                  // Top face
                                  { { -a2, b2, c2 }, { a2, b2, c2 }, { a2, b2, -c2 }, { -a2, b2, -c2 } },
                                  // Bottom face
                                  { { -a2, -b2, -c2 }, { a2, -b2, -c2 }, { a2, -b2, c2 }, { -a2, -b2, c2 } }
    };

    for (int face = 0; face < 6; ++face)
    {
      size_t base_idx = vertices.size();
      for (int i = 0; i < 4; ++i)
      {
        vertices.push_back({ positions[face][i], {}, normals[face] });
      }

      indices.push_back(base_idx);
      indices.push_back(base_idx + 1);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx + 3);
    }

    return { vertices, indices };
  }

  static std::pair<std::vector<sym_base::Vertex>, std::vector<uint32_t>> generate_square(glm::vec2 dims)
  {
    std::vector<sym_base::Vertex> vertices;
    std::vector<uint32_t> indices;

    float a2 = dims.x / 2;
    float b2 = dims.y / 2;

    glm::vec3 normals[] = {
      { 0, 0, 1 }, // front
    };

    glm::vec3 positions[1][4] = { // Front face
                                  { { -a2, -b2, 0 }, { a2, -b2, 0 }, { a2, b2, 0 }, { -a2, b2, 0 } }
    };

    for (int face = 0; face < 1; ++face)
    {
      size_t base_idx = vertices.size();
      for (int i = 0; i < 4; ++i)
      {
        vertices.push_back({ positions[face][i], {}, normals[face] });
      }

      indices.push_back(base_idx);
      indices.push_back(base_idx + 1);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx + 3);
    }

    return { vertices, indices };
  }

  static std::pair<std::vector<sym_base::Vertex>, std::vector<uint32_t>> generate_sphere(float radius,
                                                                                         int samples_u,
                                                                                         int samples_v)
  {
    std::vector<sym_base::Vertex> vertices;
    std::vector<uint32_t> indices;

    for (int i = 0; i <= samples_v; ++i)
    {
      float v_angle = M_PI * (float(i) / samples_v) - M_PI / 2.f;
      float xy      = radius * cosf(v_angle);
      float y       = radius * sinf(v_angle);

      for (int j = 0; j <= samples_u; ++j)
      {
        float u_angle = (2 * M_PI) * (float(j) / samples_u);
        float x       = xy * cosf(u_angle);
        float z       = xy * sinf(u_angle);

        glm::vec3 position = glm::vec3(x, y, z);
        glm::vec3 normal   = glm::normalize(position);

        vertices.push_back({ position, {}, normal });
      }
    }

    for (int i = 0; i < samples_v; ++i)
    {
      int k1 = i * (samples_u + 1);
      int k2 = k1 + samples_u + 1;

      for (int j = 0; j < samples_u; ++j, ++k1, ++k2)
      {
        if (i != 0)
        {
          indices.push_back(k1);
          indices.push_back(k2);
          indices.push_back(k1 + 1);
        }

        if (i != (samples_v - 1))
        {
          indices.push_back(k1 + 1);
          indices.push_back(k2);
          indices.push_back(k2 + 1);
        }
      }
    }

    return { vertices, indices };
  }

} // namespace sym

#endif // PUMA_UTILS_HH
