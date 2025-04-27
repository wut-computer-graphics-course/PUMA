#ifndef PUMA_MIRROREXAMPLELAYER_HH
#define PUMA_MIRROREXAMPLELAYER_HH

#include "SymBase.hh"
#include "Utils.hh"
#include "symbase_pch.hh"

using namespace sym_base;

namespace sym
{
  class MirrorExampleLayer : public Layer
  {
   public:
    MirrorExampleLayer()
    {
      // shaders
      {
        m_light_shader = ShaderSystem::acquire("shaders/MirrorExample/light.glsl");
        m_phong_shader = ShaderSystem::acquire("shaders/MirrorExample/phong.glsl");
      }

      // walls
      {
        auto&& [vertices, indices] = generate_cuboid({ m_walls.m_size, m_walls.m_size, m_walls.m_size });
        for (auto& v : vertices)
        {
          v.m_normal *= -1;
        }

        m_walls.m_model =
            std::make_shared<Model>(vertices, indices, ModelParams{ .m_position = true, .m_normal = true });
        m_walls.m_model_mat = glm::translate(glm::mat4(1), glm::vec3(0, m_walls.m_size / 2, 0));
        m_walls.m_color     = glm::vec3(1, 1, 1);
      }

      // mirror
      {
        auto&& [vertices, indices] = generate_square({ m_mirror.m_size / 2, m_mirror.m_size });

        m_mirror.m_model =
            std::make_shared<Model>(vertices, indices, ModelParams{ .m_position = true, .m_normal = true });
        m_mirror.m_model_mat =
            glm::translate(glm::mat4(1), glm::vec3(0, m_mirror.m_size / 2, -m_walls.m_size / 2 + .01f));
        m_mirror.m_view_mat = glm::scale(m_mirror.m_model_mat, glm::vec3(1, -1, 1));
        // glm::scale(m_mirror.m_model_mat, glm::vec3(1, -1, 1)) * glm::inverse(m_mirror.m_model_mat);
        m_mirror.m_color = glm::vec3(1, 1, 0);
      }

      // light
      {
        const glm::vec3 size       = { .2f, .2f, .2f };
        auto&& [vertices, indices] = generate_cuboid(size);

        m_light.m_model     = std::make_shared<Model>(vertices, indices, ModelParams{ .m_position = true });
        m_light.m_model_mat = glm::translate(
            glm::mat4(1),
            glm::vec3(m_walls.m_size / 2 - size.x, m_walls.m_size - size.y, m_walls.m_size / 2 - size.z));
        m_light.m_color = glm::vec3(1, 1, 1);
      }

      // box1
      {
        const glm::vec3 size       = { 1.f, 2.f, .5f };
        auto&& [vertices, indices] = generate_cuboid(size);

        m_box1.m_model =
            std::make_shared<Model>(vertices,
                                    indices,
                                    ModelParams{ .m_position = true, .m_normal = true, .m_use_adjacency = true });
        m_box1.m_model_mat = glm::rotate(glm::mat4(1), M_PI_4f, { 0, 1, 0 });
        m_box1.m_model_mat = glm::translate(m_box1.m_model_mat, glm::vec3(0, size.y / 2, -1));
        m_box1.m_color     = glm::vec3(0, 1, 0);
      }

      // box2
      {
        const glm::vec3 size       = { .5f, .5f, .5f };
        auto&& [vertices, indices] = generate_cuboid(size);

        m_box2.m_model =
            std::make_shared<Model>(vertices,
                                    indices,
                                    ModelParams{ .m_position = true, .m_normal = true, .m_use_adjacency = true });
        m_box2.m_color       = glm::vec3(0, 0, 1);
        m_box2.m_translation = { .5f, 2.5f, .5f };
      }
    }
    ~MirrorExampleLayer() = default;

    void update(float dt) override
    {
      // ------------
      // Update scene
      // ------------
      static bool first_time = true;
      if (first_time)
      {
        auto camera = dynamic_cast<OrbitCamera*>(Renderer::get_camera());
        if (camera) { camera->look_at({ 0, m_walls.m_size / 2, 0 }); }
        first_time = false;
      }
      m_box2.m_angle += dt / 2;
      // ------------

      RenderCommand::depth_test(true);

      RenderCommand::set_color_mask(false, false, false, false);
      RenderCommand::stencil_test(true);
      RenderCommand::depth_mask(false);
      RenderCommand::set_stencil_op(StencilAct::KEEP, StencilAct::KEEP, StencilAct::REPLACE);
      RenderCommand::set_stencil_func(CompFunc::ALWAYS, 1, 0xFF);

      draw_mirror(m_phong_shader);

      RenderCommand::set_color_mask(true, true, true, true);
      RenderCommand::depth_mask(true);
      RenderCommand::set_depth_func(CompFunc::LESS);
      RenderCommand::set_stencil_func(CompFunc::EQUAL, 1, 0xFF);

      glFrontFace(GL_CW);
      draw_boxes(m_phong_shader, m_mirror.m_view_mat);
      glFrontFace(GL_CCW);

      //      RenderCommand::set_color_mask(false, false, false, false);
      //      RenderCommand::stencil_test(true);
      //      //      glClearStencil(0);
      //      //      RenderCommand::depth_test(true);
      //      //      RenderCommand::depth_mask(false);
      //      RenderCommand::set_stencil_op(StencilAct::KEEP, StencilAct::KEEP, StencilAct::REPLACE);
      //      RenderCommand::set_stencil_func(CompFunc::ALWAYS, 1, 0xFF);
      //      //      RenderCommand::stencil_mask(0xFF);
      //
      //      draw_mirror(m_phong_shader);

      //      RenderCommand::set_color_mask(true, true, true, true);
      //      RenderCommand::depth_mask(true);

      // Only draw where stencil equals 1 (the mirror area)
      //      glStencilFunc(GL_EQUAL, 1, 0xFF);
      //      RenderCommand::stencil_mask(0x00);
      //      RenderCommand::depth_test(false);

      //      RenderCommand::stencil_mask(0xFF);
      // glStencilFunc(GL_ALWAYS, 1, 0xFF);
      // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      // RenderCommand::depth_test(true);
      //      RenderCommand::set_depth_func(CompFunc::LEQUAL);
      RenderCommand::stencil_test(false);

      // draw_mirror(m_phong_shader);
      // draw_walls(m_phong_shader);
      draw_boxes(m_phong_shader, glm::mat4(1));
      draw_lights();

      //      RenderCommand::depth_mask(true);
      //      RenderCommand::set_depth_func(CompFunc::LEQUAL);
      //      RenderCommand::stencil_test(false);
    }

    virtual void imgui_update(float dt) {}

   private:
    void draw_mirror(Shader* shader)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      shader->bind();
      {
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        shader->upload_uniform_float3("u_Color", m_mirror.m_color);
        // mirror
        shader->upload_uniform_mat4("u_Model", m_mirror.m_model_mat);
        Renderer::submit(*m_mirror.m_model);
      }
      shader->unbind();
    }

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
        shader->upload_uniform_float3("u_Color", m_walls.m_color);
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

    void draw_boxes(Shader* shader, glm::mat4 m)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view() * m;

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
        shader->upload_uniform_float3("u_Color", m_box1.m_color);
        Renderer::submit(*m_box1.m_model);
        // box2
        shader->upload_uniform_mat4("u_Model", m_box2.get_model_mat());
        shader->upload_uniform_float3("u_Color", m_box2.m_color);
        Renderer::submit(*m_box2.m_model);
      }
      shader->unbind();
    }

   private:
    Shader* m_light_shader;
    Shader* m_phong_shader;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      const float m_size = 5;
      glm::vec3 m_color;
    } m_walls;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      glm::mat4 m_view_mat;
      glm::mat4 m_projection_mat;
      const float m_size = 4;
      glm::vec3 m_color;
    } m_mirror;

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
      glm::vec3 m_color;
    } m_box1;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::vec3 m_translation = { 0, 0, 0 };
      glm::vec3 m_color;
      float m_angle = M_PI_4f;
      float m_scale = 1.f;

      glm::mat4 get_model_mat()
      {
        glm::quat rotation = glm::angleAxis(m_angle, glm::normalize(glm::vec3(1, 1, -1)));
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }
    } m_box2;
  };
} // namespace sym

#endif // PUMA_MIRROREXAMPLELAYER_HH
