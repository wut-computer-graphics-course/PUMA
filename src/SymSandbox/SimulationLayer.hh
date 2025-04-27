#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"

#include "Events/GuiSimulationPausedEvent.hh"
#include "Events/GuiSimulationResumedEvent.hh"
#include "Utils.hh"
#include <random>

#define SHADOW_VOLUME_PASS // Comment to skip rendering shadow volumes
#define MIRROR_PASS        // Comment to skip rendering mirror

using namespace sym_base;

static void inverse_kinematics(glm::vec3 pos, glm::vec3 normal, float& a1, float& a2, float& a3, float& a4, float& a5);

namespace sym
{
  static std::array<glm::vec3, 6> robot_translations = { glm::vec3(0, 2, 0),
                                                         glm::vec3(0, 0, 0),
                                                         glm::vec3(0.0f, 0.27f, 0.0f),
                                                         glm::vec3(-0.91f, 0.27f, 0.0f),
                                                         glm::vec3(0.0f, 0.27f, -0.26f),
                                                         glm::vec3(-2.05f + 0.3f, 0.27f, 0.0f) };
  static std::array<glm::vec3, 6> robot_axes         = { glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1),
                                                         glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) };

  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      // shaders
      {
        m_ambient_shader       = ShaderSystem::acquire("shaders/ambient.glsl");
        m_light_shader         = ShaderSystem::acquire("shaders/light.glsl");
        m_phong_shader         = ShaderSystem::acquire("shaders/phong.glsl");
        m_shadow_volume_shader = ShaderSystem::acquire("shaders/shadow_volume.glsl");
        m_mirror_shader        = ShaderSystem::acquire("shaders/mirror.glsl");
        m_particle_shader      = ShaderSystem::acquire("shaders/spark_billboard.glsl");
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

      // robot
      {
        ModelParams params{ .m_position = true, .m_normal = true };
#ifdef SHADOW_VOLUME_PASS
        params.m_use_adjacency = true;
#endif
        for (size_t i = 0; i < m_robot.m_joints.size(); i++)
        {
          m_robot.m_joints[i].m_model = std::make_shared<Model>(std::format("meshes/mesh{}.puma", i + 1), params);
        }
        m_robot.m_color = glm::vec3(1, 1, 1);
      }

      // mirror
      {
        auto&& [vertices, indices] = generate_cuboid({ m_mirror.m_size, m_mirror.m_size, 0.01f });

        ModelParams params{ .m_position = true, .m_normal = true };
#ifdef SHADOW_VOLUME_PASS
        params.m_use_adjacency = true;
#endif
        m_mirror.m_model = std::make_shared<Model>(vertices, indices, params);

        auto rotation          = glm::rotate(glm::mat4(1), M_PI_2f, glm::vec3(0, 1, 0));
        auto robot_head_offset = 2 * robot_translations[5].x; // Scaling robot required multiplying by 2
        auto translation       = glm::translate(glm::mat4(1), glm::vec3(robot_head_offset, m_mirror.m_size / 2, .5f));
        m_mirror.m_model_mat   = translation * rotation;
        m_mirror.m_view_mat =
            glm::scale(m_mirror.m_model_mat, glm::vec3(1, 1, -1)) * glm::inverse(m_mirror.m_model_mat);
        m_mirror.m_color = glm::vec3(1, 1, 0);

        auto&& [back_vertices, back_indices] = generate_square({ m_mirror.m_size, m_mirror.m_size });
        m_mirror_back.m_model                = std::make_shared<Model>(back_vertices, back_indices, params);
        rotation                             = glm::rotate(glm::mat4(1), -M_PI_2f, glm::vec3(0, 1, 0));
        translation                          = glm::translate(translation, glm::vec3(-.01f, 0, 0));
        m_mirror_back.m_model_mat            = translation * rotation;
      }

      // spark
      {
        m_spark_model = std::make_shared<Model>(ModelParams{});
        std::vector<SparkParticle> spark_vertices(m_max_sparks);
        auto instance_buffer = std::make_shared<VertexBuffer>(spark_vertices.data(),
                                                              spark_vertices.size() * sizeof(glm::vec3),
                                                              sizeof(glm::vec3));
        instance_buffer->set_layout({ { SharedDataType::Float3, "inPosition", 0 },
                                      { SharedDataType::Float3, "inDirection", 0 },
                                      { SharedDataType::Float, "inThickness", 0 },
                                      { SharedDataType::Float, "inAge", 0 },
                                      { SharedDataType::Float, "inLifetime", 0 } });
        m_spark_model->get_va().add_vertex_buffer(instance_buffer);
      }
    }

    ~SimulationLayer() = default;

    void update(float dt) override
    {
      // ------------
      // Update scene
      // ------------
      if (m_update)
      {
        update_camera(dt);
        update_robot(dt);
      }
      // ------------

      // Spark particle system update
      if (m_sparks_enabled)
      {
        const float spawn_rate = 2000.0f; // particles per second
        m_spark_accum += dt * spawn_rate;
        glm::vec3 eff_pos = glm::vec3(m_robot.m_joints[5].m_model_mat[3]);
        std::uniform_real_distribution<float> dir_dist(-1.0f, 1.0f);
        std::uniform_real_distribution<float> thickness_dist(0.05f, 0.2f);
        std::uniform_real_distribution<float> lifetime_dist(0.5f, 1.5f);
        while (m_spark_accum >= 1.0f)
        {
          m_spark_accum -= 1.0f;
          glm::vec3 dir   = glm::normalize(glm::vec3(dir_dist(m_rng), dir_dist(m_rng), dir_dist(m_rng)));
          float thickness = thickness_dist(m_rng);
          float lifetime  = lifetime_dist(m_rng);
          m_sparks.push_back({ eff_pos, dir, thickness, 0.0f, lifetime });
        }
      }
      // Update and remove dead sparks
      for (auto& p : m_sparks)
      {
        p.position += p.direction * dt * 5.0f;
        p.age += dt;
      }
      while (!m_sparks.empty() && m_sparks.front().age > m_sparks.front().lifetime)
      {
        m_sparks.pop_front();
      }

      /* ==========================================================================================================  */
      /* ============================================= SHADOW VOLUMES =============================================  */
      /* ==========================================================================================================  */

#ifdef SHADOW_VOLUME_PASS
      // -------------------------------------------------
      // Ambient pass to make sure z-buffer contains data
      // -------------------------------------------------
      RenderCommand::depth_test(true);
      draw_walls(m_ambient_shader);
      draw_robot(m_ambient_shader);
      draw_mirror(m_ambient_shader);
      draw_lights();
      // -------------------------------------------------

      // -------------------------------------------------------------------
      // Create shadow volumes of objects and render into the stencil buffer
      // -------------------------------------------------------------------
      RenderCommand::set_color_mask(false, false, false, false);
      RenderCommand::stencil_test(true);
      RenderCommand::set_stencil_func(CompFunc::ALWAYS, 0, 0xFF);
      RenderCommand::depth_clamp(true);
      RenderCommand::depth_mask(false);
      RenderCommand::set_stencil_op_per_face(Face::BACK, StencilAct::KEEP, StencilAct::INCR_WRAP, StencilAct::KEEP);
      RenderCommand::set_stencil_op_per_face(Face::FRONT, StencilAct::KEEP, StencilAct::DECR_WRAP, StencilAct::KEEP);
      RenderCommand::face_culling(false);
      draw_robot(m_shadow_volume_shader);
      draw_mirror(m_shadow_volume_shader);
      RenderCommand::face_culling(true);
      RenderCommand::depth_clamp(false);
      // -------------------------------------------------------------------

      // ------------------------------------------------------------------------------------
      // Render the scene again, using lightning and the stencil buffer as a mask for shadows
      // ------------------------------------------------------------------------------------
      RenderCommand::set_color_mask(true, true, true, true);
      RenderCommand::set_depth_func(CompFunc::EQUAL);
      RenderCommand::set_stencil_func(CompFunc::EQUAL, 0x0, 0xFF);
      RenderCommand::set_stencil_op(StencilAct::KEEP, StencilAct::KEEP, StencilAct::KEEP);
#else
      RenderCommand::depth_test(true);
#endif

      draw_mirror(m_mirror_shader);
      draw_walls(m_phong_shader);
#ifndef MIRROR_PASS
      draw_robot(m_phong_shader);
#endif

#ifdef SHADOW_VOLUME_PASS
      RenderCommand::depth_mask(true);
      RenderCommand::set_depth_func(CompFunc::LESS);
      RenderCommand::stencil_test(false);
#endif

      /* ==========================================================================================================  */
      /* ================================================= MIRROR =================================================  */
      /* ==========================================================================================================  */

#ifdef MIRROR_PASS
      RenderCommand::clear(ClearBufferMask::DEPTH_BUFFER_BIT | ClearBufferMask::STENCIL_BUFFER_BIT);

      RenderCommand::depth_test(true);
      RenderCommand::set_color_mask(false, false, false, false);
      draw_mirror_back(m_mirror_shader);
      RenderCommand::stencil_test(true);
      RenderCommand::depth_mask(false);
      RenderCommand::set_stencil_op(StencilAct::KEEP, StencilAct::KEEP, StencilAct::REPLACE);
      RenderCommand::set_stencil_func(CompFunc::ALWAYS, 1, 0xFF);

      draw_mirror(m_ambient_shader);

      RenderCommand::set_color_mask(true, true, true, true);
      RenderCommand::depth_mask(true);
      RenderCommand::set_depth_func(CompFunc::LESS);
      RenderCommand::set_stencil_func(CompFunc::EQUAL, 1, 0xFF);

      glFrontFace(GL_CW); // TODO:
      draw_robot(m_phong_shader, m_mirror.m_view_mat);
      glFrontFace(GL_CCW); // TODO:

      RenderCommand::stencil_test(false);
#endif

      // --------------------------
      // Render rest of the scene
      // --------------------------
      draw_mirror(m_mirror_shader);
      draw_robot(m_phong_shader);
      draw_sparks(m_particle_shader);
      draw_lights();
      //  -------------------------
    }

    void imgui_update(float dt) override
    {
      ImGui::Begin(DockWinId::s_settings.c_str());
      {
        static const float padding = 5, rounding = 5, thickness = 1.5f;
        auto draw_list  = ImGui::GetWindowDrawList();
        auto item_min   = ImGui::GetCursorScreenPos();
        auto avail_size = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("Robot data", ImVec2(avail_size.x, 0), ImGuiChildFlags_AutoResizeY);
        {
          ImGui::ColorEdit3("Color", glm::value_ptr(m_robot.m_color));
          for (size_t i = 0; i < m_robot.m_joints.size(); i++)
          {
            auto label = std::format("Joint {} angle", i + 1);
            ImGui::SliderFloat(label.c_str(), &m_robot.m_joints[i].m_angle, -glm::pi<float>(), glm::pi<float>());
          }
        }
        ImGui::EndChild();
        auto item_max = ImGui::GetItemRectMax();
        item_min.x -= padding;
        item_min.y -= padding;
        item_max.x += padding;
        item_max.y += padding;
        draw_list->AddRect(item_min, item_max, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, thickness);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::Separator();
        ImGui::Text("Sparks");
        ImGui::Checkbox("Enabled", &m_sparks_enabled);
      }
      ImGui::End();
    }

    void handle_event(Event& event, float dt) override
    {
      EventDispatcher dispatcher(event);
      dispatcher.dispatch_with_name_check<GuiSimulationPausedEvent>(
          [this](auto& e) { return this->gui_simulation_paused_event_handler(e); });
      dispatcher.dispatch_with_name_check<GuiSimulationResumedEvent>(
          [this](auto& e) { return this->gui_simulation_resumed_event_handler(e); });
    }

   private:
    void update_robot(float dt)
    {
      auto& joint0       = m_robot.m_joints[0];
      joint0.m_model_mat = glm::rotate(glm::mat4(1), joint0.m_angle, robot_axes[0]);
      joint0.m_model_mat = glm::translate(joint0.m_model_mat, robot_translations[0]);
      joint0.m_model_mat = glm::scale(joint0.m_model_mat, { 2, 2, 2 });

      for (size_t i = 1; i < m_robot.m_joints.size(); i++)
      {
        auto& joint       = m_robot.m_joints[i];
        joint.m_model_mat = glm::translate(glm::mat4(1), robot_translations[i]);
        joint.m_model_mat = glm::rotate(joint.m_model_mat, joint.m_angle, robot_axes[i]);
        joint.m_model_mat = glm::translate(joint.m_model_mat, -robot_translations[i]);
        joint.m_model_mat = m_robot.m_joints[i - 1].m_model_mat * joint.m_model_mat;
      }

      // -------------------------------------------------------------------------------------------
      // ---------------------------------------- ANIMATION ----------------------------------------
      // -------------------------------------------------------------------------------------------

      float& a1 = m_robot.m_joints[1].m_angle;
      float& a2 = m_robot.m_joints[2].m_angle;
      float& a3 = m_robot.m_joints[3].m_angle;
      float& a4 = m_robot.m_joints[4].m_angle;
      float& a5 = m_robot.m_joints[5].m_angle;

      static const float r = .015f;
      static float a       = 0;
      a += 2 * dt;

      static glm::vec3 pos = robot_translations[5];
      pos.y += std::cos(a) * r;
      pos.z += std::sin(a) * r;

      inverse_kinematics(pos, glm::vec3(1, 0, 0), a1, a2, a3, a4, a5);

      // -------------------------------------------------------------------------------------------
      // -------------------------------------------------------------------------------------------
      // -------------------------------------------------------------------------------------------
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
        RenderCommand::face_culling(true);
      }
      shader->unbind();
    }

    void draw_robot(Shader* shader, glm::mat4 camera_transform = glm::mat4(1))
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view() * camera_transform;

      shader->bind();
      {
#ifdef SHADOW_VOLUME_PASS
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES_ADJACENCY);
#else
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
#endif
        RenderCommand::set_line_width(1);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        shader->upload_uniform_float3("u_Color", m_robot.m_color);
        for (auto& joint : m_robot.m_joints)
        {
          shader->upload_uniform_mat4("u_Model", joint.m_model_mat);
          Renderer::submit(*joint.m_model);
        }
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

    void draw_mirror(Shader* shader)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      shader->bind();
      {
#ifdef SHADOW_VOLUME_PASS
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES_ADJACENCY);
#else
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
#endif
        RenderCommand::set_line_width(1);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        shader->upload_uniform_float3("u_Color", m_mirror.m_color);
        if (shader == m_mirror_shader) { shader->upload_uniform_float("u_Alpha", m_mirror.m_alpha); }
        // mirror
        shader->upload_uniform_mat4("u_Model", m_mirror.m_model_mat);
        Renderer::submit(*m_mirror.m_model);
      }
      shader->unbind();
    }

    void draw_sparks(Shader* shader)
    {
      if (m_sparks.empty()) return;
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      std::vector<SparkVertex> vertices;
      vertices.reserve(m_sparks.size());
      for (const auto& p : m_sparks)
      {
        vertices.push_back({ p.position, p.direction, p.thickness, p.age, p.lifetime });
      }

      m_spark_model->swap_vertices(vertices.size() * sizeof(SparkVertex), vertices.data(), 0);
      shader->bind();
      shader->upload_uniform_mat4("u_ViewProjection", vp);
      shader->upload_uniform_float3("u_CameraPos", camera->get_position());
      RenderCommand::set_draw_primitive(DrawPrimitive::POINTS);
      RenderCommand::depth_mask(false);
      Renderer::submit(*m_spark_model, vertices.size());
      RenderCommand::depth_mask(true);
      shader->unbind();
    }

    void draw_mirror_back(Shader* shader)
    {
      auto camera = Renderer::get_camera();
      auto vp     = camera->get_projection() * camera->get_view();

      shader->bind();
      {
#ifdef SHADOW_VOLUME_PASS
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES_ADJACENCY);
#else
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
#endif
        RenderCommand::set_line_width(1);
        shader->upload_uniform_mat4("u_ViewProjection", vp);
        shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        shader->upload_uniform_float3("u_Light.position", m_light.m_model_mat[3]);
        shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        shader->upload_uniform_float3("u_Color", m_mirror.m_color);
        if (shader == m_mirror_shader) { shader->upload_uniform_float("u_Alpha", m_mirror.m_alpha); }
        // mirror
        shader->upload_uniform_mat4("u_Model", m_mirror_back.m_model_mat);
        Renderer::submit(*m_mirror_back.m_model);
      }
      shader->unbind();
    }

    void update_camera(float dt)
    {
      static bool first_time = true;
      if (first_time)
      {
        auto camera = dynamic_cast<FpsCamera*>(Renderer::get_camera());
        if (camera) { camera->look_at({ 0, 5.f / 2, 0 }); }
        first_time = false;
      }
    }

    bool gui_simulation_paused_event_handler(GuiSimulationPausedEvent& e)
    {
      m_update = false;
      return true;
    }

    bool gui_simulation_resumed_event_handler(GuiSimulationResumedEvent& e)
    {
      m_update = true;
      return true;
    }

   private:
    struct PumaJoint
    {
      std::shared_ptr<Model> m_model;
      float m_angle = 0;
      glm::mat4 m_model_mat;
    };

    bool m_update = true;

    Shader* m_ambient_shader;
    Shader* m_light_shader;
    Shader* m_phong_shader;
    Shader* m_shadow_volume_shader;
    Shader* m_mirror_shader;
    Shader* m_particle_shader;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      const float m_size = 10;
      glm::vec3 m_color;
    } m_walls;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      glm::vec3 m_color;
    } m_light;

    struct
    {
      std::array<PumaJoint, 6> m_joints;
      glm::vec3 m_color;
    } m_robot;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      glm::mat4 m_view_mat;
      const float m_size  = 4;
      const float m_alpha = .5f;
      glm::vec3 m_color;
    } m_mirror;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
    } m_mirror_back;

    struct SparkVertex
    {
      glm::vec3 position;
      glm::vec3 direction;
      float thickness;
      float age;
      float lifetime;
    };

    // Spark particle system data
    struct SparkParticle
    {
      glm::vec3 position;
      glm::vec3 direction;
      float thickness;
      float age;
      float lifetime;
    };
    std::deque<SparkParticle> m_sparks;
    float m_spark_accum   = 0.0f;
    bool m_sparks_enabled = true;
    std::mt19937 m_rng{ std::random_device{}() };
    size_t m_max_sparks = 100;
    std::shared_ptr<Model> m_spark_model;
  };
} // namespace sym

static void inverse_kinematics(glm::vec3 pos, glm::vec3 normal, float& a1, float& a2, float& a3, float& a4, float& a5)
{
  float l1 = .91f, l2 = .81f, l3 = .33f, dy = .27f, dz = .26f;

  glm::vec3 normalized_normal = glm::normalize(normal);
  glm::vec3 pos1              = pos + normalized_normal * l3;

  float e = sqrtf(pos1.z * pos1.z + pos1.x * pos1.x - dz * dz);
  a1      = atan2f(pos1.z, -pos1.x) + atan2f(dz, e);

  glm::vec3 pos2(e, pos1.y - dy, 0.0f);
  a3 = -acosf(std::min(1.0f, (pos2.x * pos2.x + pos2.y * pos2.y - l1 * l1 - l2 * l2) / (2.0f * l1 * l2)));

  float k       = l1 + l2 * cosf(a3);
  float l_prime = l2 * sinf(a3);
  a2            = -atan2f(pos2.y, sqrtf(pos2.x * pos2.x + pos2.z * pos2.z)) - atan2f(l_prime, k);

  glm::mat4 rotate_y_neg_a1 = glm::rotate(glm::mat4(1.0f), -a1, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec4 normal1_vec4 =
      rotate_y_neg_a1 * glm::vec4(normalized_normal.x, normalized_normal.y, normalized_normal.z, 0.0f);
  glm::vec3 normal1(normal1_vec4.x, normal1_vec4.y, normal1_vec4.z);

  glm::mat4 rotate_z_neg_a2_plus_a3 = glm::rotate(glm::mat4(1.0f), -(a2 + a3), glm::vec3(0.0f, 0.0f, 1.0f));
  normal1_vec4                      = rotate_z_neg_a2_plus_a3 * glm::vec4(normal1.x, normal1.y, normal1.z, 0.0f);
  normal1                           = glm::vec3(normal1_vec4.x, normal1_vec4.y, normal1_vec4.z);

  a5 = acosf(normal1.x);
  a4 = atan2f(normal1.z, normal1.y);
}

#endif // SYM_BASE_MYLAYER_HH
