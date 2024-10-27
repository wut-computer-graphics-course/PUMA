#include "Input.hh"
#include "Application.hh"

namespace sym_base
{
  bool Input::is_key_pressed(int keycode)
  {
    auto window = Application::get_window().get_handle();
    auto state  = glfwGetKey(window, keycode);

    return state == GLFW_PRESS;
  }
} // namespace sym_base