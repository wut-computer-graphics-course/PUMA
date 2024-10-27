#ifndef SYM_BASE_APPLICATION_HH
#define SYM_BASE_APPLICATION_HH

#include "pch.hh"

#include "Events/Events.hh"
#include "ImGui/ImGuiLayer.hh"
#include "Renderer/Buffer.hh"
#include "Renderer/Shader.hh"
#include "Renderer/VertexArray.hh"
#include "Timer.hh"
#include "Window.hh"

namespace sym_base
{
  class Layer;
  class LayerStack;

  struct ApplicationParams
  {
    std::string m_title;
    uint32_t m_width;
    uint32_t m_height;
    bool m_vsync;
    float m_refresh_rate;
  };

  class Application
  {
   public:
    Application(const ApplicationParams& params);
    virtual ~Application();

    void run();
    virtual void update(float dt);
    void push_layer(Layer* layer);

    inline static Application& get() { return *s_instance; }
    inline Window& get_window() { return *s_instance->m_window; }

   private:
    bool on_window_closed(WindowClosedEvent& e);

    void events_manager(Event& e);
    void sleep(float t);

   protected:
    Timer m_timer;
    bool m_running;

   private:
    static Application* s_instance;
    static EventCallbackFn s_events_manager;

    LayerStack* m_layer_stack;
    ImGuiLayer* m_imgui_layer;

    std::unique_ptr<Window> m_window;

    std::shared_ptr<VertexArray> m_triangle_va;
    std::shared_ptr<Shader> m_triangle_shader;

    std::shared_ptr<VertexArray> m_square_va;
    std::shared_ptr<Shader> m_square_shader;
  };
} // namespace sym_base

#endif // SYM_BASE_APPLICATION_HH
