#include "RenderCommand.hh"

namespace sym_base
{
  RendererAPI* RenderCommand::m_renderer_api = new RendererAPI();

  void RenderCommand::set_clear_color(const glm::vec4& color) { m_renderer_api->set_clear_color(color); }

  void RenderCommand::set_draw_primitive(const DrawPrimitive& primitive)
  {
    m_renderer_api->set_draw_primitive(primitive);
  }

  void RenderCommand::set_line_width(float width) { m_renderer_api->set_line_width(width); }

  void RenderCommand::enable_depth_buffering() { m_renderer_api->enable_depth_buffering(); }

  void RenderCommand::enable_anti_aliasing() { m_renderer_api->enable_anti_aliasing(); }

  void RenderCommand::enable_face_culling() { m_renderer_api->enable_face_culling(); }

  void RenderCommand::clear() { m_renderer_api->clear(); }
  void RenderCommand::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array)
  {
    m_renderer_api->draw_indexed(vertex_array);
  }

} // namespace sym_base