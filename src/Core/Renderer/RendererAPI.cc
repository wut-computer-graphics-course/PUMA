#include "RendererAPI.hh"

namespace sym_base
{
  void RendererAPI::set_clear_color(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

  void RendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

  void RendererAPI::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array)
  {
    glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
  }
} // namespace sym_base