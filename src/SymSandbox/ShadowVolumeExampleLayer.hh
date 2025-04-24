#ifndef PUMA_SHADOWVOLUMEEXAMPLELAYER_HH
#define PUMA_SHADOWVOLUMEEXAMPLELAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"

using namespace sym_base;

static std::pair<std::vector<Vertex>, std::vector<uint32_t>> generate_cuboid(glm::vec3 dims, glm::vec3 color);

namespace sym
{
  class ShadowVolumeExampleLayer : public Layer
  {
   public:
    ShadowVolumeExampleLayer()
    {
      // shaders
      {
        m_default_shader = ShaderSystem::acquire("shaders/default.glsl");
        m_phong_shader   = ShaderSystem::acquire("shaders/phong.glsl");
      }

      const float wall_size = 5;

      // walls
      {
        auto&& [vertices, indices] = generate_cuboid({ wall_size, wall_size, wall_size }, { 1, 0, 0 });
        for (auto& v : vertices)
        {
          v.m_normal *= -1;
        }

        m_walls.m_model     = std::make_shared<Model>(vertices,
                                                  indices,
                                                  ModelParams{ .m_position = true, .m_color = true, .m_normal = true });
        m_walls.m_model_mat = glm::translate(glm::mat4(1), glm::vec3(0, wall_size / 2, 0));
      }

      // light
      {
        const glm::vec3 size       = { .2f, .2f, .2f };
        auto&& [vertices, indices] = generate_cuboid(size, { 1, 1, 1 });

        m_light.m_model = std::make_shared<Model>(vertices, indices, ModelParams{ .m_position = true });
        m_light.m_model_mat =
            glm::translate(glm::mat4(1), glm::vec3(wall_size / 2 - size.x, wall_size - size.y, wall_size / 2 - size.z));
        m_light.m_color = glm::vec3(1, 1, 1);
      }

      // box1
      {
        const glm::vec3 size       = { 1.f, 2.f, .5f };
        auto&& [vertices, indices] = generate_cuboid(size, { 0, 1, 0 });

        m_box1.m_model     = std::make_shared<Model>(vertices,
                                                 indices,
                                                 ModelParams{ .m_position = true, .m_color = true, .m_normal = true });
        m_box1.m_model_mat = glm::rotate(glm::mat4(1), M_PI_4f, { 0, 1, 0 });
        m_box1.m_model_mat = glm::translate(m_box1.m_model_mat, glm::vec3(0, size.y / 2, -1));
      }
    }
    ~ShadowVolumeExampleLayer() = default;

    void update(float dt) override
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      // walls
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        RenderCommand::face_culling(Face::FRONT);
        m_phong_shader->bind();
        m_phong_shader->upload_uniform_mat4("u_ViewProjection", vp);
        m_phong_shader->upload_uniform_mat4("u_Model", m_walls.m_model_mat);
        m_phong_shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        m_phong_shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        m_phong_shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        Renderer::submit(*m_walls.m_model);
        m_phong_shader->unbind();
        RenderCommand::face_culling(false);
      }

      // light
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        m_default_shader->bind();
        m_default_shader->upload_uniform_mat4("u_ViewProjection", vp);
        m_default_shader->upload_uniform_mat4("u_Model", m_light.m_model_mat);
        m_default_shader->upload_uniform_float3("u_Color", m_light.m_color);
        Renderer::submit(*m_light.m_model);
        m_default_shader->unbind();
      }

      // box1
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        m_phong_shader->bind();
        m_phong_shader->upload_uniform_mat4("u_ViewProjection", vp);
        m_phong_shader->upload_uniform_mat4("u_Model", m_box1.m_model_mat);
        m_phong_shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        m_phong_shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        m_phong_shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        Renderer::submit(*m_box1.m_model);
        m_phong_shader->unbind();
      }
    }

    virtual void imgui_update(float dt) {}

   private:
    Shader* m_default_shader;
    Shader* m_phong_shader;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
    } m_walls;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      glm::vec3 m_color;
    } m_light;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
    } m_box1;
  };
} // namespace sym

static std::pair<std::vector<Vertex>, std::vector<uint32_t>> generate_cuboid(glm::vec3 dims, glm::vec3 color)
{
  std::vector<Vertex> vertices;
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
      vertices.push_back({ positions[face][i], color, normals[face] });
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

#endif // PUMA_SHADOWVOLUMEEXAMPLELAYER_HH
