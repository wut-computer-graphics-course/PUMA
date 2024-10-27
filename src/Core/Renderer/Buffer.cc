#include "Buffer.hh"

namespace sym_base
{
  ////////////////////////////////////////////////// VertexBuffer //////////////////////////////////////////////////

  VertexBuffer::VertexBuffer(void* vertices, uint32_t size)
  {
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

  void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id); }

  void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  ////////////////////////////////////////////////// IndexBuffer //////////////////////////////////////////////////

  IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_count{ count }
  {
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

  void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id); }

  void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
} // namespace sym_base