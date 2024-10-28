#ifndef SYM_BASE_WINDOW_HH
#define SYM_BASE_WINDOW_HH

#include "Renderer/RenderingContext.hh"
#include "Utils.hh"

namespace sym_base
{
  struct WindowData
  {
    const std::string m_title;
    uint32_t m_width;
    uint32_t m_height;
    bool m_vsync;

    EventCallbackFn m_event_callback;
  };

  class Window
  {
   public:
    static Window* create(const WindowData& data);

    ~Window();

    void update();

    inline uint32_t get_width() const { return m_data.m_width; }
    inline uint32_t get_height() const { return m_data.m_height; }
    inline GLFWwindow* get_handle() const { return m_window; }

    inline bool is_vsync() const { return m_data.m_vsync; }

    inline void set_event_callback_fn(EventCallbackFn& callback_fn) { m_data.m_event_callback = callback_fn; }

   private:
    Window(const WindowData& data);
    void init();

    void terminate();
    void set_vsync(bool enabled);
    void set_callbacks();

   private:
    WindowData m_data;
    GLFWwindow* m_window;
    RenderingContext* m_context;
  };
} // namespace sym_base

#endif // SYM_BASE_WINDOW_HH
