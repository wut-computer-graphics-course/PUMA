#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"

using namespace sym_base;

static std::pair<std::vector<Vertex>, std::vector<uint32_t>> generate_cuboid(glm::vec3 dims, glm::vec3 color);

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      // shaders
      {
        m_robot_shader = ShaderSystem::acquire("shaders/robot.glsl");
        m_light_shader = ShaderSystem::acquire("shaders/light.glsl");
      }

      // light
      {
        const glm::vec3 size       = { .2f, .2f, .2f };
        auto&& [vertices, indices] = generate_cuboid(size, { 1, 1, 1 });

        m_light.m_model     = std::make_shared<Model>(vertices, indices, ModelParams{ .m_position = true });
        m_light.m_model_mat = glm::translate(glm::mat4(1), glm::vec3(5.f / 2 - size.x, 5.f - size.y, 5.f / 2 - size.z));
        m_light.m_color     = glm::vec3(1, 1, 1);
      }

      // robot
      {
        for (size_t i = 0; i < m_robot.m_joints.size(); i++)
        {
          m_robot.m_joints[i].m_model = std::make_shared<Model>(std::format("meshes/mesh{}.puma", i + 1),
                                                                ModelParams{ .m_position = true, .m_normal = true });
        }
      }
    }

    ~SimulationLayer() = default;

    void update(float dt) override
    {
      RenderCommand::depth_test(true);

      update_camera(dt);
      update_robot(dt);

      draw_robot(m_robot_shader);
      draw_lights();
    }

    void update_robot(float dt)
    {
      static std::array<glm::vec3, 6> translations = { glm::vec3(0, 2, 0),
                                                       glm::vec3(0, 0, 0),
                                                       glm::vec3(0.0f, 0.27f, 0.0f),
                                                       glm::vec3(-0.91f, 0.27f, 0.0f),
                                                       glm::vec3(-1.72f + 0.1f, 0.27f, 0.0f),
                                                       glm::vec3(-2.05f + 0.3f, 0.27f, 0.0f) };
      static std::array<glm::vec3, 6> axes         = { glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1),
                                                       glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1) };

      auto& joint0       = m_robot.m_joints[0];
      joint0.m_model_mat = glm::rotate(glm::mat4(1), joint0.m_angle, axes[0]);
      joint0.m_model_mat = glm::translate(joint0.m_model_mat, translations[0]);
      joint0.m_model_mat = glm::scale(joint0.m_model_mat, { 2, 2, 2 });

      for (size_t i = 1; i < m_robot.m_joints.size(); i++)
      {
        auto& joint       = m_robot.m_joints[i];
        joint.m_model_mat = glm::translate(glm::mat4(1), translations[i]);
        joint.m_model_mat = glm::rotate(joint.m_model_mat, joint.m_angle, axes[i]);
        joint.m_model_mat = glm::translate(joint.m_model_mat, -translations[i]);
        joint.m_model_mat = m_robot.m_joints[i - 1].m_model_mat * joint.m_model_mat;
      }
    }

    void draw_robot(Shader* shader)
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

    void update_camera(float dt)
    {
      static bool first_time = true;
      if (first_time)
      {
        auto camera = dynamic_cast<OrbitCamera*>(Renderer::get_camera());
        if (camera) { camera->look_at({ 0, 5.f / 2, 0 }); }
        first_time = false;
      }
    }

    virtual void imgui_update(float dt)
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
      }
      ImGui::End();
    }

   private:
    struct PumaJoint
    {
      std::shared_ptr<Model> m_model;
      float m_angle = 0;
      glm::mat4 m_model_mat;
    };

    Shader* m_robot_shader;
    Shader* m_light_shader;

    struct
    {
      std::shared_ptr<Model> m_model;
      glm::mat4 m_model_mat;
      glm::vec3 m_color;
    } m_light;

    struct
    {
      std::array<PumaJoint, 6> m_joints;
      glm::vec3 m_color = { 1, 1, 1 };
    } m_robot;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
