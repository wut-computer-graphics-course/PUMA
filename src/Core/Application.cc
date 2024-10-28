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