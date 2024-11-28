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
    m_context->swap_buffers();
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

    if (m_data.m_samples) { glfwWindowHint(GLFW_SAMPLES, m_data.m_samples); }

    m_window = glfwCreateWindow((int)m_data.m_width, (int)m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) { throw std::runtime_error("Failed to create window instance"); }

    m_context = new RenderingContext(m_window);
    m_context->init();

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
    glfwSetErrorCallback(glfw_error_callback);
    glfwSetKeyCallback(m_window,
                       [](GLFWwindow* window, int key, int scancode, int action, int mods)
                       {
                         auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                         switch (action)
                         {
                         case GLFW_PRESS:
                         {
                           KeyPressedEvent event(key, false);
                           data->m_event_callback(event);
                           break;
                         }
                         case GLFW_RELEASE:
                         {
                           KeyReleasedEvent event(key);
                           data->m_event_callback(event);
                           break;
                         }
                         }
                       });
    glfwSetMouseButtonCallback(m_window,
                               [](GLFWwindow* window, int button, int action, int mods)
                               {
                                 auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                                 switch (action)
                                 {
                                 case GLFW_PRESS:
                                 {
                                   MouseButtonPressedEvent event(button);
                                   data->m_event_callback(event);
                                   break;
                                 }
                                 case GLFW_RELEASE:
                                 {
                                   MouseButtonReleasedEvent event(button);
                                   data->m_event_callback(event);
                                   break;
                                 }
                                 }
                               });
    glfwSetFramebufferSizeCallback(m_window,
                                   [](GLFWwindow* window, int width, int height)
                                   {
                                     auto* app_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
                                     app_window->m_data.m_width  = width;
                                     app_window->m_data.m_height = height;
                                     WindowResizedEvent event{ (uint32_t)width, (uint32_t)height };
                                     app_window->m_data.m_event_callback(event);

                                     app_window->m_context->set_viewport(0, 0, width, height);
                                   });
    glfwSetScrollCallback(m_window,
                          [](GLFWwindow* window, double x_offset, double y_offset)
                          {
                            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                            MouseScrolledEvent event((float)x_offset, (float)y_offset);
                            data->m_event_callback(event);
                          });
    glfwSetCursorPosCallback(m_window,
                             [](GLFWwindow* window, double x_pos, double y_pos)
                             {
                               WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                               MouseMovedEvent event((float)x_pos, (float)y_pos);
                               data->m_event_callback(event);
                             });
  }
} // namespace sym_base