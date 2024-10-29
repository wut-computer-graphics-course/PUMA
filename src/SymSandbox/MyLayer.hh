#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "pch.hh"

using namespace sym_base;

namespace sym
{
  class MyLayer : public Layer
  {
   public:
    MyLayer()
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

        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
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
        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 3, 7, 2, 6 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_cube.m_va = std::make_shared<VertexArray>();
        m_cube.m_va->add_vertex_buffer(vertex_buffer);
        m_cube.m_va->set_index_buffer(index_buffer);

        m_cube.m_shader = std::make_shared<Shader>("shaders/cube.glsl");
      }

      m_camera = std::make_shared<OrbitCamera>();
      m_camera->set_position({ 0, 0, 5 });
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      auto& window = Application::get().get_window();
      m_camera->set_perspective(M_PI / 4, window.get_width() / (float)window.get_height(), 1.f, 100.f);

      Renderer::begin_scene();
      {
        // cube
        {
          m_cube.m_shader->bind();
          m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);
          m_cube.m_rotation *= glm::angleAxis(dt / 2, glm::vec3(1, 1, 0));
          auto mvp = m_camera->get_projection() * m_camera->get_view() * m_cube.get_model_mat();
          m_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
          RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
          RenderCommand::set_line_width(2);
          Renderer::submit(m_cube.m_va);
          m_cube.m_va->unbind();
        }

        // square
        {
          m_square.m_shader->bind();
          auto mvp = m_camera->get_projection() * m_camera->get_view();
          m_square.m_shader->upload_uniform_mat4("u_MVP", mvp);
          m_square.m_texture->bind(0);
          m_square.m_shader->upload_uniform_int("u_Texture", 0);
          RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
          RenderCommand::set_line_width(1);
          Renderer::submit(m_square.m_va);
          m_square.m_va->unbind();
        }
      }
      Renderer::end_scene();
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin("Settings");
      ImGui::ColorEdit3("Cube color", glm::value_ptr(m_cube.m_color));
      ImGui::End();

      ImGui::SetNextWindowSize(ImVec2(800, 600));
      ImGui::Begin("Simulation window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

      ImGui::End();
    }

    void handle_event(Event& event, float dt) override
    {
      Event::try_handler<MouseMovedEvent>(event, BIND_EVENT_FOR_FUN(MyLayer::mouse_moved_event_handler, dt));
    }

   private:
    bool mouse_moved_event_handler(MouseMovedEvent& event, float dt)
    {
      if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
      {
        m_camera->rotate(event.get_dx(), event.get_dy(), m_sensitivity * dt);
      }
      if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT))
      {
        m_camera->zoom(event.get_dy(), m_sensitivity * dt);
      }

      return false;
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

    std::shared_ptr<OrbitCamera> m_camera;
    float m_sensitivity = 1.f;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
