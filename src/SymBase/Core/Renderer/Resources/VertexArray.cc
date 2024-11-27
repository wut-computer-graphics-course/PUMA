#include "VertexArray.hh"
#include "Utils.hh"

namespace sym_base
{
  VertexArray::VertexArray() { glCreateVertexArrays(1, &m_renderer_id); }

  VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_renderer_id); }

  void VertexArray::bind() const
  {
    glBindVertexArray(m_renderer_id);
    for (auto& buffer : m_vertex_buffers)
    {
      buffer->bind();
    }
    if (m_index_buffer) { m_index_buffer->bind(); }
  }

  void VertexArray::unbind() const
  {
    for (auto& buffer : m_vertex_buffers)
    {
      buffer->unbind();
    }
    m_index_buffer->unbind();
    glBindVertexArray(0);
  }

  void VertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
  {
    ASSERT(!vertex_buffer->get_layout().get_elements().empty(), "Vertex buffer must have non empty layout");

    glBindVertexArray(m_renderer_id);
    vertex_buffer->bind();

    uint32_t idx       = 0;
    const auto& layout = vertex_buffer->get_layout();
    for (const auto& element : layout)
    {
      glEnableVertexAttribArray(idx);
      glVertexAttribPointer(idx,
                            element.get_component_count(),
                            element.get_gl_type(),
                            element.m_normalized,
                            layout.get_stride(),
                            (const void*)(element.m_offset));
      idx++;
    }

    m_vertex_buffers.push_back(vertex_buffer);

    vertex_buffer->unbind();
    glBindVertexArray(0);
  }

  void VertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer)
  {
    glBindVertexArray(m_renderer_id);
    index_buffer->bind();

    m_index_buffer = index_buffer;

    index_buffer->unbind();
    glBindVertexArray(0);
  }
} // namespace sym_base