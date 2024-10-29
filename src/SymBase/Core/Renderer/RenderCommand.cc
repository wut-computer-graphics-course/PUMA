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

  void RenderCommand::depth_buffering(bool on) { m_renderer_api->depth_buffering(on); }

  void RenderCommand::anti_aliasing(bool on) { m_renderer_api->anti_aliasing(on); }

  void RenderCommand::face_culling(bool on) { m_renderer_api->face_culling(on); }

  void RenderCommand::alpha_blending(bool on) { m_renderer_api->alpha_blending(on); }

  void RenderCommand::clear() { m_renderer_api->clear(); }

  void RenderCommand::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array)
  {
    m_renderer_api->draw_indexed(vertex_array);
  }

} // namespace sym_base