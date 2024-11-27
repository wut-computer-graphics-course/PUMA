#ifndef SYM_BASE_BUFFER_HH
#define SYM_BASE_BUFFER_HH

#include <glad/glad.h>

namespace sym_base
{
  enum class SharedDataType
  {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
  };

  struct BufferElement
  {
    std::string m_name;
    SharedDataType m_type;
    uint32_t m_size;
    uint32_t m_offset;
    bool m_normalized;

    BufferElement(SharedDataType type, const std::string& name, bool normalized = false);

    uint32_t get_component_count() const;
    GLenum get_gl_type() const;
  };

  class BufferLayout
  {
   public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements);

    inline const std::vector<BufferElement>& get_elements() const { return m_elements; }
    inline uint32_t get_stride() const { return m_stride; }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

   private:
    void calc_offsets_and_stride();

   private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;
  };

  enum class BufferType
  {
    STATIC  = 0x88E4,
    DYNAMIC = 0x88E8
  };

  class VertexBuffer
  {
   public:
    VertexBuffer(void* vertices, uint32_t size, uint32_t instance_size, BufferType type = BufferType::STATIC);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void send_data(uint32_t offset, uint32_t size, void* data);

    inline const BufferLayout& get_layout() const { return m_layout; }
    inline void set_layout(const BufferLayout& layout) { m_layout = layout; };
    inline uint32_t get_count() const { return m_count; }

   private:
    uint32_t m_renderer_id;
    BufferLayout m_layout;
    BufferType m_type;
    uint32_t m_count;
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
