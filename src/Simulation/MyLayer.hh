#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "pch.hh"

#include "Layers/Layer.hh"
#include "Renderer/Buffer.hh"
#include "Renderer/Renderer.hh"
#include "Renderer/Shader.hh"
#include "Renderer/VertexArray.hh"

using namespace sym_base;

namespace sym
{
  class MyLayer : public Layer
  {
   public:
    MyLayer()
    {
      // triangle
      {
        float vertices[3 * (3 + 4)] = { -.5f, -.5f, 0.f, 1.f, 0.f, 0.f, 1.f, //
                                        .5f,  -.5f, 0.f, 0.f, 1.f, 0.f, 1.f, //
                                        0.f,  .5f,  0.f, 0.f, 0.f, 1.f, 1.f };
        auto vertex_buffer          = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

        BufferLayout layout = { { SharedDataType::Float3, "a_Position" }, { SharedDataType::Float4, "a_Color" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[3] = { 0, 1, 2 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, 3);

        m_triangle.m_va = std::make_shared<VertexArray>();
        m_triangle.m_va->add_vertex_buffer(vertex_buffer);
        m_triangle.m_va->set_index_buffer(index_buffer);

        m_triangle.m_shader = std::make_shared<Shader>("shaders/triangle.glsl");
      }

      // cube
      {
        float vertices[4 * 3 * 2] = { -.5f, -.5f, .5f,  //
                                      .5f,  -.5f, .5f,  //
                                      .5f,  .5f,  .5f,  //
                                      -.5f, .5f,  .5f,  //
                                      -.5f, -.5f, -.5f, //
                                      .5f,  -.5f, -.5f, //
                                      .5f,  .5f,  -.5f, //
                                      -.5f, .5f,  -.5f };
        auto vertex_buffer        = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

        BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 3, 7, 2, 6 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_cube.m_va = std::make_shared<VertexArray>();
        m_cube.m_va->add_vertex_buffer(vertex_buffer);
        m_cube.m_va->set_index_buffer(index_buffer);

        m_cube.m_shader = std::make_shared<Shader>("shaders/cube.glsl");
      }
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      Renderer::begin_scene();
      {
        m_cube.m_shader->bind();
        m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);
        m_cube.m_rotation *= glm::angleAxis(dt / 2, glm::vec3(1, 1, 0));
        auto model_mat = glm::translate(glm::mat4(1.f), m_cube.m_translation) * glm::mat4_cast(m_cube.m_rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_cube.m_scale));
        m_cube.m_shader->upload_uniform_mat4("u_ModelMat", model_mat);
        RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
        RenderCommand::set_line_width(2);
        Renderer::submit(m_cube.m_va);
        m_cube.m_va->unbind();

        m_triangle.m_shader->bind();
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        RenderCommand::set_line_width(1);
        Renderer::submit(m_triangle.m_va);
        m_triangle.m_va->unbind();
      }
      Renderer::end_scene();
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin("Settings");
      ImGui::ColorEdit3("Cube color", glm::value_ptr(m_cube.m_color));
      ImGui::End();
    }

   private:
    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
    } m_triangle;

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      glm::vec3 m_color       = { 0, 0, 0 };
      glm::vec3 m_translation = { 0, 0, 0 };
      glm::quat m_rotation    = { 1, 0, 0, 0 };
      float m_scale           = 1.1f;
    } m_cube;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
