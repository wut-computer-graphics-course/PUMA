#ifndef SYM_BASE_ORBITCAMERA_HH
#define SYM_BASE_ORBITCAMERA_HH

#include "Camera.hh"

namespace sym_base
{
  class OrbitCamera : public Camera
  {
   public:
    OrbitCamera(glm::vec3 target   = { 0, 0, 0 },
                float min_distance = 0.f,
                float max_distance = FLT_MAX,
                float distance     = 0.f);

    glm::vec3 get_position();
    glm::mat4 get_view();

    void rotate(float dx, float dy, float dt);
    void zoom(float dd, float dt);

    void set_position(glm::vec3 position);

   private:
    void set_distance_range(float min_distance, float max_distance);
    void clamp_distance();

   private:
    float m_angle_x;
    float m_angle_y;

    float m_distance;
    float m_min_distance;
    float m_max_distance;

    glm::vec3 m_target;
  };
} // namespace sym_base

#endif // SYM_BASE_ORBITCAMERA_HH
