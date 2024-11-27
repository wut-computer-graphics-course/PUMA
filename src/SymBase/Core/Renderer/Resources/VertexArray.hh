#ifndef SYM_BASE_VERTEXARRAY_HH
#define SYM_BASE_VERTEXARRAY_HH

#include "Buffer.hh"

namespace sym_base
{
  class VertexArray
  {
   public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer);
    void set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer);

    inline const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const { return m_vertex_buffers; }
    inline const std::shared_ptr<VertexBuffer>& get_vertex_buffer(uint32_t idx) const { return m_vertex_buffers[idx]; }
    inline const std::shared_ptr<IndexBuffer>& get_index_buffer() const { return m_index_buffer; }

   private:
    uint32_t m_renderer_id;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
    std::shared_ptr<IndexBuffer> m_index_buffer;
  };
} // namespace sym_base

#endif // SYM_BASE_VERTEXARRAY_HH
