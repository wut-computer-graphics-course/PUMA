#ifndef SYM_BASE_RENDERERAPI_HH
#define SYM_BASE_RENDERERAPI_HH

#include "pch.hh"

#include "VertexArray.hh"

namespace sym_base
{
  class RendererAPI
  {
   public:
    void set_clear_color(const glm::vec4& color);
    void clear();

    void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array);
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERERAPI_HH
