#ifndef SYM_BASE_RENDERERAPI_HH
#define SYM_BASE_RENDERERAPI_HH

#include "VertexArray.hh"

namespace sym_base
{
  enum class DrawPrimitive
  {
    POINTS         = 0x0000,
    LINES          = 0x0001,
    LINE_LOOP      = 0x0002,
    LINE_STRIP     = 0x0003,
    TRIANGLES      = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN   = 0x0006
  };

  class RendererAPI
  {
   public:
    void set_clear_color(const glm::vec4& color);
    void set_draw_primitive(const DrawPrimitive& primitive);
    void set_line_width(float width);

    void enable_depth_buffering();
    void enable_anti_aliasing();
    void enable_face_culling();

    void clear();

    void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array);
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERERAPI_HH
