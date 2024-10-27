#include "Window.hh"
#include "Events/Events.hh"

static void glfw_error_callback(int error, const char* description) { throw std::runtime_error(description); }

namespace sym_base
{
  static bool s_glfw_initialized = false;

  Window* Window::create(const WindowData& data) { return new Window(data); }

  Window::~Window() { terminate(); }

  void Window::terminate() { glfwDestroyWindow(m_window); }

  void Window::update()
  {
    glfwPollEvents();
    glfwSwapBuffers(m_window);
  }

  Window::Window(const WindowData& data) : m_data{ data } { init(); }

  void Window::init()
  {
    if (!s_glfw_initialized)
    {
      int success = glfwInit();
      if (success == GLFW_FALSE) { throw std::runtime_error("Failed to initialize GLFW"); }
      s_glfw_initialized = true;
    }

    glfwSetErrorCallback(glfw_error_callback);
    m_window = glfwCreateWindow((int)m_data.m_width, (int)m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
    if (m_window == NULL) { throw std::runtime_error("Failed to create window instance"); }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &m_data);
    set_vsync(true);
    set_callbacks();
  }

  void Window::set_vsync(bool enabled)
  {
    if (enabled) { glfwSwapInterval(1); }
    else { glfwSwapInterval(0); }

    m_data.m_vsync = enabled;
  }

  void Window::set_callbacks()
  {
    /* set callbacks for glfw events */
    glfwSetWindowCloseCallback(m_window,
                               [](GLFWwindow* window)
                               {
                                 auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                 WindowClosedEvent event;
                                 data->m_event_callback(event);
                               });
  }
} // namespace sym_base