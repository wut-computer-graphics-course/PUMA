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
        m_ambient_shader       = ShaderSystem::acquire("shaders/ambient.glsl");
        m_light_shader         = ShaderSystem::acquire("shaders/light.glsl");
        m_phong_shader         = ShaderSystem::acquire("shaders/phong.glsl");
        m_shadow_volume_shader = ShaderSystem::acquire("shaders/shadow_volume.glsl");
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

        m_box1.m_model = std::make_shared<Model>(
            vertices,
            indices,
            ModelParams{ .m_position = true, .m_color = true, .m_normal = true, .m_use_adjacency = true });
        m_box1.m_model_mat = glm::rotate(glm::mat4(1), M_PI_4f, { 0, 1, 0 });
        m_box1.m_model_mat = glm::translate(m_box1.m_model_mat, glm::vec3(0, size.y / 2, -1));
      }

      // box2
      {
        const glm::vec3 size       = { .5f, .5f, .5f };
        auto&& [vertices, indices] = generate_cuboid(size, { 0, 0, 1 });

        m_box2.m_model = std::make_shared<Model>(
            vertices,
            indices,
            ModelParams{ .m_position = true, .m_color = true, .m_normal = true, .m_use_adjacency = true });
        m_box2.m_translation = { .5f, 2.5f, .5f };
      }
    }
    ~ShadowVolumeExampleLayer() = default;

    void update(float dt) override
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      draw_walls(m_ambient_shader);
      draw_boxes(m_ambient_shader);
      draw_lights();

      glEnable(GL_STENCIL_TEST);
      glEnable(GL_DEPTH_TEST);

      glStencilFunc(GL_ALWAYS, 0, 0xFF);
      glEnable(GL_DEPTH_CLAMP);

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);

      glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
      glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
      draw_boxes(m_shadow_volume_shader);

      glDisable(GL_DEPTH_CLAMP);

      RenderCommand::set_color_mask(true, true, true, true);

      glDepthFunc(GL_EQUAL);

      glStencilFunc(GL_EQUAL, 0x0, 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      draw_walls(m_phong_shader);
      draw_boxes(m_phong_shader);
      draw_lights();

      glDepthMask(GL_TRUE);
      glDepthFunc(GL_LEQUAL);

      glDisable(GL_STENCIL_TEST);

      m_box2.m_angle += dt / 2;
    }

    virtual void imgui_update(float dt) {}

   private:
    void draw_walls(Shader* shader)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      shader->bind();
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        RenderCommand::face_culling(Face::FRONT);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        // walls
        shader->upload_uniform_mat4("u_Model", m_walls.m_model_mat);
        Renderer::submit(*m_walls.m_model);
        RenderCommand::face_culling(false);
      }
      shader->unbind();
    }

    void draw_lights()
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      m_light_shader->bind();
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        m_light_shader->upload_uniform_mat4("u_ViewProjection", vp);
        m_light_shader->upload_uniform_float3("u_Color", m_light.m_color);
        // lights
        m_light_shader->upload_uniform_mat4("u_Model", m_light.m_model_mat);
        Renderer::submit(*m_light.m_model);
      }
      m_light_shader->unbind();
    }

    void draw_boxes(Shader* shader)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      shader->bind();
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES_ADJACENCY);
        RenderCommand::set_line_width(1);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        // box1
        shader->upload_uniform_mat4("u_Model", m_box1.m_model_mat);
        Renderer::submit(*m_box1.m_model);
        // box2
        shader->upload_uniform_mat4("u_Model", m_box2.get_model_mat());
        Renderer::submit(*m_box2.m_model);
      }
      shader->unbind();
    }

   private:
    Shader* m_ambient_shader;
    Shader* m_light_shader;
    Shader* m_phong_shader;
    Shader* m_shadow_volume_shader;

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

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::vec3 m_translation = { 0, 0, 0 };
      float m_angle           = M_PI_4f;
      float m_scale           = 1.f;

      glm::mat4 get_model_mat()
      {
        glm::quat rotation = glm::angleAxis(m_angle, glm::normalize(glm::vec3(1, 1, -1)));
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }
    } m_box2;
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
