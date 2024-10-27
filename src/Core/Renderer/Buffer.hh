#ifndef SYM_BASE_BUFFER_HH
#define SYM_BASE_BUFFER_HH

#include "pch.hh"

namespace sym_base
{
  class VertexBuffer
  {
   public:
    VertexBuffer(void* vertices, uint32_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

   private:
    uint32_t m_renderer_id;
  };

  class IndexBuffer
  {
   public:
    IndexBuffer(uint32_t* vertices, uint32_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline uint32_t get_count() const { return m_count; }

   private:
    uint32_t m_renderer_id;
    uint32_t m_count;
  };
} // namespace sym_base

#endif // SYM_BASE_BUFFER_HH
