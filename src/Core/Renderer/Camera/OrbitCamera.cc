#include "OrbitCamera.hh"

namespace sym_base
{
  OrbitCamera::OrbitCamera(glm::vec3 target, float min_distance, float max_distance, float distance) :
      m_angle_x{ 0 }, m_angle_y{ 0 }, m_target{ target }, m_distance{ distance }
  {
    set_distance_range(min_distance, max_distance);
  }

  glm::vec3 OrbitCamera::get_position()
  {
    float x = m_target.x + m_distance * glm::cos(m_angle_y) * glm::sin(m_angle_x);
    float y = m_target.y + m_distance * glm::sin(m_angle_y);
    float z = m_target.z + m_distance * glm::cos(m_angle_y) * glm::cos(m_angle_x);

    return glm::vec3(x, -y, z);
  }

  glm::mat4 OrbitCamera::get_view()
  {
    auto view = glm::mat4{ 1.f };
    view      = glm::translate(view, { 0, 0, -m_distance });
    view      = glm::rotate(view, -m_angle_y, { 1, 0, 0 });
    view      = glm::rotate(view, -m_angle_x, { 0, 1, 0 });
    view      = glm::translate(view, -m_target);

    return view;
  }

  void OrbitCamera::rotate(float dx, float dy, float dt)
  {
    m_angle_x = glm::mod(m_angle_x + dx * dt, glm::two_pi<float>());
    m_angle_y = glm::mod(m_angle_y + dy * dt, glm::two_pi<float>());
  }

  void OrbitCamera::zoom(float dd, float dt)
  {
    m_distance += dd * dt;
    clamp_distance();
  }

  void OrbitCamera::set_position(glm::vec3 position)
  {
    auto offset = position - m_target;

    m_distance = glm::length(offset);
    clamp_distance();

    m_angle_y = glm::asin(-offset.y / m_distance);
    m_angle_x = glm::atan(offset.x, offset.z);
  }

  void OrbitCamera::set_distance_range(float min_distance, float max_distance)
  {
    if (max_distance < min_distance) { max_distance = min_distance; }
    m_min_distance = min_distance;
    m_max_distance = max_distance;

    clamp_distance();
  }

  void OrbitCamera::clamp_distance()
  {
    if (m_distance < m_min_distance) { m_distance = m_min_distance; }
    else if (m_distance > m_max_distance) { m_distance = m_max_distance; }
  }
} // namespace sym_base
