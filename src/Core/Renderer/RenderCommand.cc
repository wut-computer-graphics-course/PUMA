#include "RenderCommand.hh"

namespace sym_base
{
  RendererAPI* RenderCommand::m_renderer_api = new RendererAPI();

  void RenderCommand::set_clear_color(const glm::vec4& color) { m_renderer_api->set_clear_color(color); }
  void RenderCommand::clear() { m_renderer_api->clear(); }

  void RenderCommand::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array)
  {
    m_renderer_api->draw_indexed(vertex_array);
  }

} // namespace sym_base