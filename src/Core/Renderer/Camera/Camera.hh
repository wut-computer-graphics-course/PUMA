#ifndef SYM_BASE_CAMERA_HH
#define SYM_BASE_CAMERA_HH

namespace sym_base
{
  class Camera
  {
   public:
    ~Camera() = default;

    inline glm::mat4 get_projection() { return m_projection_mat; }

    void set_perspective(float fov, float aspect_ratio, float near, float far);

   protected:
    glm::mat4 m_projection_mat{ 1.f };
  };
} // namespace sym_base

#endif // SYM_BASE_CAMERA_HH
