#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      BufferLayout layout = { { SharedDataType::Float3, "a_Position" },
                              { SharedDataType::Float3, "a_Color" },
                              { SharedDataType::Float2, "a_TexCoord" } };

      // square
      {
        Vertex vertices[4] = { { { -.5f, -.5f, 0.f }, {}, { 0.f, 0.f } },
                               { { .5f, -.5f, 0.f }, {}, { 1.f, 0.f } },
                               { { .5f, .5f, 0.f }, {}, { 1.f, 1.f } },
                               { { -.5f, .5f, 0.f }, {}, { 0.f, 1.f } } };

        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(Vertex));
        vertex_buffer->set_layout(layout);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_square.m_va = std::make_shared<VertexArray>();
        m_square.m_va->add_vertex_buffer(vertex_buffer);
        m_square.m_va->set_index_buffer(index_buffer);

        m_square.m_shader = std::make_shared<Shader>("shaders/square.glsl");

        m_square.m_texture = std::make_shared<Texture2D>("textures/leaf.png");
      }

      // cube
      {
        Vertex vertices[8] = {
          { { -.5f, -.5f, .5f } },  { { .5f, -.5f, .5f } },  { { .5f, .5f, .5f } },  { { -.5f, .5f, .5f } },
          { { -.5f, -.5f, -.5f } }, { { .5f, -.5f, -.5f } }, { { .5f, .5f, -.5f } }, { { -.5f, .5f, -.5f } },
        };
        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(Vertex));
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 3, 7, 2, 6 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_cube.m_va = std::make_shared<VertexArray>();
        m_cube.m_va->add_vertex_buffer(vertex_buffer);
        m_cube.m_va->set_index_buffer(index_buffer);

        m_cube.m_shader = std::make_shared<Shader>("shaders/cube.glsl");
      }

      // framebuffer
      {
        float vertices[] = {
          // Coords   // TexCoords
          -1.f, -1.f, 0.f, 0.f, //
          1.f,  -1.f, 1.f, 0.f, //
          1.f,  1.f,  1.f, 1.f, //
          -1.f, 1.f,  0.f, 1.f  //
        };

        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(float));

        layout = { { SharedDataType::Float2, "a_Position" }, { SharedDataType::Float2, "a_TexCoord" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_framebuffer.m_va = std::make_shared<VertexArray>();
        m_framebuffer.m_va->add_vertex_buffer(vertex_buffer);
        m_framebuffer.m_va->set_index_buffer(index_buffer);

        m_framebuffer.m_shader = std::make_shared<Shader>("shaders/framebuffer.glsl");

        m_framebuffer.m_texture = std::make_shared<Texture2D>(m_framebuffer.m_tex_width, m_framebuffer.m_tex_height);

        m_framebuffer.m_buffer = std::make_shared<Framebuffer>();
        m_framebuffer.m_buffer->set_color_buffer(m_framebuffer.m_texture);
        m_framebuffer.m_buffer->create_render_buffer();
      }
    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto& camera = SimulationContext::s_camera;
      camera->set_perspective(M_PI / 4, m_framebuffer.m_win_width / m_framebuffer.m_win_height, 1.f, 100.f);
      auto& window           = Application::get().get_window();
      auto rendering_context = window.get_rendering_context();

      Renderer::begin_scene();
      {
        rendering_context->set_viewport(0, 0, m_framebuffer.m_tex_width, m_framebuffer.m_tex_height);

        // render onto framebuffer
        m_framebuffer.m_buffer->bind();
        {
          // clear buffer
          RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
          RenderCommand::clear();
          // enable rendering features
          RenderCommand::depth_buffering(true);
          RenderCommand::anti_aliasing(true);
          RenderCommand::face_culling(true);
          RenderCommand::alpha_blending(true);
          // render objects
          // cube
          {
            m_cube.m_shader->bind();
            m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);
            m_cube.m_rotation *= glm::angleAxis(dt / 2, glm::vec3(1, 1, 0));
            auto mvp = camera->get_projection() * camera->get_view() * m_cube.get_model_mat();
            m_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
            RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
            RenderCommand::set_line_width(2);
            Renderer::submit(m_cube.m_va);
            m_cube.m_va->unbind();
          }
          // square
          {
            m_square.m_shader->bind();
            auto mvp = camera->get_projection() * camera->get_view();
            m_square.m_shader->upload_uniform_mat4("u_MVP", mvp);
            m_square.m_texture->bind(0);
            m_square.m_shader->upload_uniform_int("u_Texture", 0);
            RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
            RenderCommand::set_line_width(1);
            Renderer::submit(m_square.m_va);
            m_square.m_va->unbind();
          }
        }
        m_framebuffer.m_buffer->unbind();
      }
      Renderer::end_scene();
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin(DockWinId::s_settings.c_str());
      {
        ImGui::ColorEdit3("Cube color", glm::value_ptr(m_cube.m_color));
      }
      ImGui::End();

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
      ImGui::Begin(DockWinId::s_content.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
      {
        ImVec2 win_size            = ImGui::GetWindowSize();
        m_framebuffer.m_win_width  = win_size.x;
        m_framebuffer.m_win_height = win_size.y;

        ImGui::Image((ImTextureID)(intptr_t)m_framebuffer.m_texture->get_id(), win_size, ImVec2(0, 1), ImVec2(1, 0));
      }
      ImGui::End();
      ImGui::PopStyleVar();
    }

   private:
    struct Vertex
    {
      glm::vec3 m_position;
      glm::vec3 m_color;
      glm::vec2 m_tex_coord;
    };

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      std::shared_ptr<Texture2D> m_texture;
    } m_square;

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      std::shared_ptr<Texture2D> m_texture;
      std::shared_ptr<Framebuffer> m_buffer;
      const uint32_t m_tex_width  = 800;
      const uint32_t m_tex_height = 600;
      float m_win_width           = 800;
      float m_win_height          = 600;
    } m_framebuffer;

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      glm::vec3 m_color       = { 0, 0, 0 };
      glm::vec3 m_translation = { 0, 0, 0 };
      glm::quat m_rotation    = { 1, 0, 0, 0 };
      float m_scale           = 1.1f;

      glm::mat4 get_model_mat()
      {
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(m_rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }
    } m_cube;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
