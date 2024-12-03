#ifndef SYM_BASE_APPLICATION_HH
#define SYM_BASE_APPLICATION_HH

#include "Events/Events.hh"
#include "ImGui/ImGuiLayer.hh"
#include "TimedLoop/TimedLoop.hh"
#include "Timer.hh"
#include "Utils.hh"
#include "Window.hh"

#include <glm/glm.hpp>
#include <memory>
#include <thread>

namespace sym_base
{
  class Layer;
  class LayerStack;

  struct ApplicationParams
  {
    std::string m_title;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_samples;
    bool m_vsync;
    float m_refresh_rate;
    glm::vec4 m_clear_color;
  };

  class Application
  {
   public:
    Application(const ApplicationParams& params);
    virtual ~Application();

    void run();

    inline static Application& get() { return *s_instance; }
    inline Window& get_window() { return *s_instance->m_window; }
    inline const ApplicationParams& get_params() const { return m_params; }

    virtual void end_simulation();
    virtual void reset_simulation();
    virtual void pause_simulation();
    virtual void resume_simulation();
    virtual void start_simulation();

   protected:
    void create_simulation(float period, const std::function<void(void)>& func, LoopStatus status);
    virtual void update(float dt);
    void push_layer(Layer* layer);

   protected:
    Timer m_timer;
    bool m_running;

    std::unique_ptr<TimedLoop> m_simulation_loop;
    std::unique_ptr<std::thread> m_simulation_thread;

   private:
    bool on_window_closed(WindowClosedEvent& e);

    void events_manager(Event& e);
    void sleep(float t);

   private:
    static Application* s_instance;
    static EventCallbackFn s_events_manager;

    ApplicationParams m_params;

    LayerStack* m_layer_stack;
    ImGuiLayer* m_imgui_layer;

    std::unique_ptr<Window> m_window;
  };
} // namespace sym_base

#endif // SYM_BASE_APPLICATION_HH
