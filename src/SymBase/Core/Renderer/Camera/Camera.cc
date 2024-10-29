#include "Camera.hh"

namespace sym_base
{
  void Camera::set_perspective(float fov, float aspect_ratio, float near, float far)
  {
    m_projection_mat = glm::perspective(fov, aspect_ratio, near, far);
  }
} // namespace sym_base