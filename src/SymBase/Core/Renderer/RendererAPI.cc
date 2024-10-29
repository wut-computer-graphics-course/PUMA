#include "RendererAPI.hh"

namespace sym_base
{
  static DrawPrimitive current_draw_primitive = DrawPrimitive::TRIANGLES;

  void RendererAPI::set_clear_color(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

  void RendererAPI::set_draw_primitive(const DrawPrimitive& primitive) { current_draw_primitive = primitive; }

  void RendererAPI::set_line_width(float width) { glLineWidth(width); }

  void RendererAPI::enable_depth_buffering()
  {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }

  void RendererAPI::enable_anti_aliasing() { glEnable(GL_MULTISAMPLE); }

  void RendererAPI::enable_face_culling()
  {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  }

  void RendererAPI::enable_alpha_blending()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void RendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

  void RendererAPI::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array)
  {
    glDrawElements(static_cast<GLenum>(current_draw_primitive),
                   vertex_array->get_index_buffer()->get_count(),
                   GL_UNSIGNED_INT,
                   nullptr);
  }
} // namespace sym_base