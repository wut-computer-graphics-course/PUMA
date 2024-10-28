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

        m_triangle_va = std::make_shared<VertexArray>();
        m_triangle_va->add_vertex_buffer(vertex_buffer);
        m_triangle_va->set_index_buffer(index_buffer);

        m_triangle_shader = std::make_shared<Shader>("shaders/triangle.glsl");
      }

      // square
      {
        float vertices[4 * 3] = { -.5f, -.5f, 0.f, //
                                  .5f,  -.5f, 0.f, //
                                  .5f,  .5f,  0.f, //
                                  -.5f, .5f,  0.f };
        auto vertex_buffer    = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

        BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, 6);

        m_square_va = std::make_shared<VertexArray>();
        m_square_va->add_vertex_buffer(vertex_buffer);
        m_square_va->set_index_buffer(index_buffer);

        m_square_shader = std::make_shared<Shader>("shaders/square.glsl");
      }
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
      RenderCommand::clear();

      Renderer::begin_scene();
      {
        m_square_shader->bind();
        m_square_shader->upload_uniform_float3("u_Color", m_square_color);
        Renderer::submit(m_square_va);
        m_square_va->unbind();

        m_triangle_shader->bind();
        Renderer::submit(m_triangle_va);
        m_triangle_va->unbind();
      }
      Renderer::end_scene();
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin("Settings");
      ImGui::ColorEdit3("Square color", glm::value_ptr(m_square_color));
      ImGui::End();
    }

   private:
    std::shared_ptr<VertexArray> m_triangle_va;
    std::shared_ptr<Shader> m_triangle_shader;

    std::shared_ptr<VertexArray> m_square_va;
    std::shared_ptr<Shader> m_square_shader;
    glm::vec3 m_square_color = { 0, 0, 0 };
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
