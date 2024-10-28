#include "Input.hh"
#include "Application.hh"

namespace sym_base
{
  bool Input::is_key_pressed(int keycode)
  {
    auto& app    = Application::get();
    auto* window = static_cast<GLFWwindow*>(app.get_window().get_handle());
    auto state   = glfwGetKey(window, keycode);

    return state == GLFW_PRESS;
  }

  bool Input::is_mouse_button_pressed(int button)
  {
    auto& app    = Application::get();
    auto* window = static_cast<GLFWwindow*>(app.get_window().get_handle());
    auto state   = glfwGetMouseButton(window, button);

    return state == GLFW_PRESS;
  }
} // namespace sym_base