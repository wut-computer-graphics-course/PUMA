#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

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

        std::string vertex_src = R"(
      #version 330 core

      layout(location = 0) in vec3 a_Position;
      layout(location = 1) in vec4 a_Color;

      out vec4 v_Color;

      void main()
      {
        gl_Position = vec4(a_Position, 1.0);

        v_Color = a_Color;
      })";

        std::string fragment_src = R"(
      #version 330 core

      layout(location = 0) out vec4 color;

      in vec4 v_Color;

      void main()
      {
        color = v_Color;
      })";

        m_triangle_shader = std::make_shared<Shader>(vertex_src, fragment_src);
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

        std::string vertex_src = R"(
      #version 330 core

      layout(location = 0) in vec3 a_Position;

      void main()
      {
        gl_Position = vec4(a_Position, 1.0);
      })";

        std::string fragment_src = R"(
      #version 330 core

      layout(location = 0) out vec4 color;

      void main()
      {
        color = vec4(1.0, 1.0, 1.0, 1.0);
      })";

        m_square_shader = std::make_shared<Shader>(vertex_src, fragment_src);
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
        Renderer::submit(m_square_va);
        m_square_va->unbind();

        m_triangle_shader->bind();
        Renderer::submit(m_triangle_va);
        m_triangle_va->unbind();
      }
      Renderer::end_scene();
    }

   private:
    std::shared_ptr<VertexArray> m_triangle_va;
    std::shared_ptr<Shader> m_triangle_shader;

    std::shared_ptr<VertexArray> m_square_va;
    std::shared_ptr<Shader> m_square_shader;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
