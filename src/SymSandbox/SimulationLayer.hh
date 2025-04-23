#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer() :
      m_pumaModel1("meshes/mesh1.puma", {.m_position = true, .m_normal = true}),
      m_pumaModel2("meshes/mesh2.puma", {.m_position = true, .m_normal = true}),
      m_pumaModel3("meshes/mesh3.puma", {.m_position = true, .m_normal = true}),
      m_pumaModel4("meshes/mesh4.puma", {.m_position = true, .m_normal = true}),
      m_pumaModel5("meshes/mesh5.puma", {.m_position = true, .m_normal = true}),
      m_pumaModel6("meshes/mesh6.puma", {.m_position = true, .m_normal = true}),
      m_robotShader("../assets/shaders/robot.glsl"),
      m_joint1Angle(0.0f),
      m_joint2Angle(0.0f),
      m_joint3Angle(0.0f),
      m_joint4Angle(0.0f),
      m_joint5Angle(0.0f)
    { }

    ~SimulationLayer() = default;

    void update(float dt) override
    {
      update_camera(dt);

      m_robotShader.bind();
      m_robotShader.upload_uniform_mat4("u_ViewProjection", Renderer::get_view_projection());
      m_robotShader.upload_uniform_float3("u_CameraPosition", Renderer::get_camera()->get_position());

      glm::mat4 joint1 = glm::mat4(1.0f);
      m_robotShader.upload_uniform_mat4("u_Model", joint1);
      Renderer::submit(m_pumaModel1);

      glm::mat4 joint2 = glm::rotate(glm::mat4(1.0f), m_joint1Angle, glm::vec3(0, 1, 0));
      m_robotShader.upload_uniform_mat4("u_Model", joint2);
      Renderer::submit(m_pumaModel2);

      glm::mat4 joint3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.27f, 0.0f));
      joint3 = glm::rotate(joint3, m_joint2Angle, glm::vec3(0, 0, 1));
      joint3 = glm::translate(joint3, glm::vec3(0.0f, -0.27f, 0.0f));
      joint3 = joint2 * joint3;
      m_robotShader.upload_uniform_mat4("u_Model", joint3);
      Renderer::submit(m_pumaModel3);

      glm::mat4 joint4 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.91f, 0.27f, 0.0f));
      joint4 = glm::rotate(joint4, m_joint3Angle, glm::vec3(0, 0, 1));
      joint4 = glm::translate(joint4, glm::vec3(0.91f, -0.27f, 0.0f));
      joint4 = joint3 * joint4;
      m_robotShader.upload_uniform_mat4("u_Model", joint4);
      Renderer::submit(m_pumaModel4);

      glm::mat4 joint5 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.72f, 0.27f, 0.0f));
      joint5 = glm::rotate(joint5, m_joint4Angle, glm::vec3(0, 0, 1));
      joint5 = glm::translate(joint5, glm::vec3(1.72f, -0.27f, 0.0f));
      joint5 = joint4 * joint5;
      m_robotShader.upload_uniform_mat4("u_Model", joint5);
      Renderer::submit(m_pumaModel5);

      glm::mat4 joint6 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.05f, 0.27f, 0.0f));
      joint6 = glm::rotate(joint6, m_joint5Angle, glm::vec3(0, 0, 1));
      joint6 = glm::translate(joint6, glm::vec3(2.05f, -0.27f, 0.0f));
      joint6 = joint5 * joint6;
      m_robotShader.upload_uniform_mat4("u_Model", joint6);
      Renderer::submit(m_pumaModel6);

      m_robotShader.unbind();
    }

    void update_camera(float dt)
    {
      // Camera controls are now handled in on_mouse_moved and on_mouse_scrolled
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin("Robot Controls");

      ImGui::SliderFloat("Joint 1 Angle", &m_joint1Angle, -glm::pi<float>(), glm::pi<float>());
      ImGui::SliderFloat("Joint 2 Angle", &m_joint2Angle, -glm::pi<float>(), glm::pi<float>());
      ImGui::SliderFloat("Joint 3 Angle", &m_joint3Angle, -glm::pi<float>(), glm::pi<float>());
      ImGui::SliderFloat("Joint 4 Angle", &m_joint4Angle, -glm::pi<float>(), glm::pi<float>());
      ImGui::SliderFloat("Joint 5 Angle", &m_joint5Angle, -glm::pi<float>(), glm::pi<float>());

      ImGui::End();
    }

   private:
    Model m_pumaModel1;
    Model m_pumaModel2;
    Model m_pumaModel3;
    Model m_pumaModel4;
    Model m_pumaModel5;
    Model m_pumaModel6;
    Shader m_robotShader;

    float m_joint1Angle;
    float m_joint2Angle;
    float m_joint3Angle;
    float m_joint4Angle;
    float m_joint5Angle;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
