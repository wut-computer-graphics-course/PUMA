#include "Buffer.hh"
#include "Utils.hh"

static uint32_t shared_data_type_size(sym_base::SharedDataType type);
namespace sym_base
{
  ////////////////////////////////////////////////// BufferElement //////////////////////////////////////////////////

  BufferElement::BufferElement(SharedDataType type, const std::string& name, bool normalized) :
      m_name{ name }, m_type{ type }, m_size{ shared_data_type_size(type) }, m_offset{ 0 }, m_normalized{ normalized }
  {
  }

  uint32_t BufferElement::get_component_count() const
  {
    switch (m_type)
    {
    case SharedDataType::None:
      return 0;
    case SharedDataType::Float:
      return 1;
    case SharedDataType::Float2:
      return 2;
    case SharedDataType::Float3:
      return 3;
    case SharedDataType::Float4:
      return 4;
    case SharedDataType::Mat3:
      return 3 * 3;
    case SharedDataType::Mat4:
      return 4 * 4;
    case SharedDataType::Int:
      return 1;
    case SharedDataType::Int2:
      return 2;
    case SharedDataType::Int3:
      return 3;
    case SharedDataType::Int4:
      return 4;
    case SharedDataType::Bool:
    default:
      return 1;
    }
  }

  GLenum BufferElement::get_gl_type() const
  {
    switch (m_type)
    {
    case SharedDataType::None:
      return GL_NONE;
    case SharedDataType::Float:
    case SharedDataType::Float2:
    case SharedDataType::Float3:
    case SharedDataType::Float4:
      return GL_FLOAT;
    case SharedDataType::Mat3:
      return GL_FLOAT_MAT3;
    case SharedDataType::Mat4:
      return GL_FLOAT_MAT4;
    case SharedDataType::Int:
    case SharedDataType::Int2:
    case SharedDataType::Int3:
    case SharedDataType::Int4:
      return GL_INT;
    case SharedDataType::Bool:
    default:
      return GL_BOOL;
    }
  }

  ////////////////////////////////////////////////// BufferLayout //////////////////////////////////////////////////

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements{ elements }
  {
    calc_offsets_and_stride();
  }

  void BufferLayout::calc_offsets_and_stride()
  {
    uint32_t offset = 0;
    m_stride        = 0;
    for (auto& element : m_elements)
    {
      element.m_offset = offset;
      offset += element.m_size;
      m_stride += element.m_size;
    }
  }

  ////////////////////////////////////////////////// VertexBuffer //////////////////////////////////////////////////

  VertexBuffer::VertexBuffer(void* vertices, uint32_t size, uint32_t instance_size, BufferType type) :
      m_type{ type }, m_count{ size / instance_size }
  {
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, (GLenum)m_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

  void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id); }

  void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  void VertexBuffer::send_data(uint32_t offset, uint32_t size, void* data)
  {
    ASSERT(m_type == BufferType::DYNAMIC, "Buffer needs to be DYNAMIC in order to send data");
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  ////////////////////////////////////////////////// IndexBuffer //////////////////////////////////////////////////

  IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_count{ count }
  {
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

  void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id); }

  void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
} // namespace sym_base

static uint32_t shared_data_type_size(sym_base::SharedDataType type)
{
  switch (type)
  {
  case sym_base::SharedDataType::None:
    return 0;
  case sym_base::SharedDataType::Float:
    return 4;
  case sym_base::SharedDataType::Float2:
    return 4 * 2;
  case sym_base::SharedDataType::Float3:
    return 4 * 3;
  case sym_base::SharedDataType::Float4:
    return 4 * 4;
  case sym_base::SharedDataType::Mat3:
    return 4 * 3 * 3;
  case sym_base::SharedDataType::Mat4:
    return 4 * 4 * 4;
  case sym_base::SharedDataType::Int:
    return 4;
  case sym_base::SharedDataType::Int2:
    return 4 * 2;
  case sym_base::SharedDataType::Int3:
    return 4 * 3;
  case sym_base::SharedDataType::Int4:
    return 4 * 4;
  case sym_base::SharedDataType::Bool:
  default:
    return 1;
  }
}