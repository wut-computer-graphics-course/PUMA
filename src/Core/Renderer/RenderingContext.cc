#include "RenderingContext.hh"

namespace sym_base
{
  RenderingContext::RenderingContext(GLFWwindow* window_handle) : m_window_handle{ window_handle } {}

  void RenderingContext::init()
  {
    glfwMakeContextCurrent(m_window_handle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status) { throw std::runtime_error("Failed to initialize GLAD"); }
  }

  void RenderingContext::swap_buffers() { glfwSwapBuffers(m_window_handle); }

} // namespace sym_base