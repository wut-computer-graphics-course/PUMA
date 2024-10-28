#ifndef SYM_BASE_RENDERCOMMAND_HH
#define SYM_BASE_RENDERCOMMAND_HH

#include "RendererAPI.hh"

namespace sym_base
{
  class RenderCommand
  {
   public:
    static void set_clear_color(const glm::vec4& color);
    static void clear();

    static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array);

   private:
    static RendererAPI* m_renderer_api;
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERCOMMAND_HH
