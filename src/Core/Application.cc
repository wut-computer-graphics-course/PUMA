#include "Application.hh"
#include "Layers/LayerStack.hh"

namespace sym_base
{
  static float s_refresh_rate = 0;

  Application* Application::s_instance          = nullptr;
  EventCallbackFn Application::s_events_manager = nullptr;

  Application::Application(const ApplicationParams& params) : m_running{ true }
  {
    s_instance = this;

    // set refresh rate
    s_refresh_rate = params.m_refresh_rate;

    // create event manager
    s_events_manager = BIND_EVENT_FOR_FUN(Application::events_manager);

    // create (alloc and init) window
    m_window = std::unique_ptr<Window>(Window::create({
        params.m_title,
        params.m_width,
        params.m_height,
        params.m_vsync,
    }));
    m_window->set_event_callback_fn(s_events_manager);

    // create (alloc and init) layer stacks
    m_layer_stack = new LayerStack();

    m_imgui_layer = new ImGuiLayer();
    push_layer(m_imgui_layer);

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

  Application::~Application()
  {
    delete m_layer_stack;

    s_instance = nullptr;
  }

  void Application::run()
  {
    while (m_running)
    {
      m_timer.tick();
      if (m_timer.get_dt() < s_refresh_rate)
      {
        sleep(s_refresh_rate - m_timer.get_dt());
        continue;
      }
      else { m_timer.reset(); }

      // TODO: renderer
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      m_square_va->bind();
      m_square_shader->bind();
      glDrawElements(GL_TRIANGLES, m_square_va->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
      m_square_va->unbind();

      m_triangle_va->bind();
      m_triangle_shader->bind();
      glDrawElements(GL_TRIANGLES, m_triangle_va->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
      m_triangle_va->unbind();
      //

      this->update(m_timer.get_dt());

      m_imgui_layer->begin();
      {
        for (auto layer : *m_layer_stack)
        {
          layer->imgui_update(m_timer.get_dt());
        }
      }
      m_imgui_layer->end();

      m_window->update();
    }
  }

  void Application::update(float dt)
  {
    for (auto layer : *m_layer_stack)
    {
      layer->update(m_timer.get_dt());
    }
  }

  void Application::push_layer(Layer* layer)
  {
    m_layer_stack->push_layer(layer);
    layer->attach();
  }

  bool Application::on_window_closed(WindowClosedEvent& e)
  {
    m_running = false;
    return true;
  }
  void Application::events_manager(Event& e)
  {
    Event::try_handler<WindowClosedEvent>(e, BIND_EVENT_FOR_FUN(Application::on_window_closed));

    for (auto& iter : *m_layer_stack | std::views::reverse)
    {
      iter->handle_event(e, m_timer.get_dt());
      if (e.handled) { break; }
    }
  }
  void Application::sleep(float t) { std::this_thread::sleep_for(std::chrono::nanoseconds((int)(t * NS_IN_S))); }
} // namespace sym_base