#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"

#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer() : m_pumaModel1("meshes/mesh1.puma", {.m_position = true, .m_normal = true})
    {

    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto camera = Renderer::get_camera();

      Renderer::submit(m_pumaModel1);
      // cube
      // {
      //   m_cube.m_shader->bind();
      //   m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);
      //   m_cube.m_rotation *= glm::angleAxis(dt / 2, glm::vec3(1, 1, 0));
      //   auto mvp = camera->get_projection() * camera->get_view() * m_cube.get_model_mat();
      //   m_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
      //   RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
      //   RenderCommand::set_line_width(2);
      //   Renderer::submit(*m_cube.m_shader, *m_cube.m_va, m_cube.get_model_mat());
      //   m_cube.m_va->unbind();
      // }
      // // square
      // {
      //   m_square.m_shader->bind();
      //   auto mvp = camera->get_projection() * camera->get_view();
      //   m_square.m_shader->upload_uniform_mat4("u_MVP", mvp);
      //   m_square.m_texture->bind(0);
      //   m_square.m_shader->upload_uniform_int("u_Texture", 0);
      //   RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
      //   RenderCommand::set_line_width(1);
      //   Renderer::submit(*m_square.m_shader, *m_square.m_va, glm::mat4(1));
      //   m_square.m_va->unbind();
      // }
    }

    virtual void imgui_update(float dt)
    {
      // note: this should be in gui layer
    }

   private:
    struct Vertex
    {
      glm::vec3 m_position;
      glm::vec3 m_color;
      glm::vec2 m_tex_coord;
    };

    Model m_pumaModel1;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
