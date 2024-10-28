#ifndef SYM_BASE_RENDERCOMMAND_HH
#define SYM_BASE_RENDERCOMMAND_HH

#include "RendererAPI.hh"

namespace sym_base
{
  class RenderCommand
  {
   public:
    static void set_clear_color(const glm::vec4& color);
    static void set_draw_primitive(const DrawPrimitive& primitive);
    static void set_line_width(float width);

    static void enable_depth_buffering();
    static void enable_anti_aliasing();
    static void enable_face_culling();

    static void clear();

    static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array);

   private:
    static RendererAPI* m_renderer_api;
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERCOMMAND_HH
