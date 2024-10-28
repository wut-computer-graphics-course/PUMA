#ifndef SYM_BASE_RENDERER_HH
#define SYM_BASE_RENDERER_HH

#include "pch.hh"

#include "RenderCommand.hh"
#include "VertexArray.hh"

namespace sym_base
{
  class Renderer
  {
   public:
    static void begin_scene();
    static void end_scene();

    static void submit(const std::shared_ptr<VertexArray>& vertex_array);
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERER_HH
