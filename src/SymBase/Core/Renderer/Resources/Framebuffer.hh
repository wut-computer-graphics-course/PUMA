#ifndef SIMULATIONAPP_FRAMEBUFFER_HH
#define SIMULATIONAPP_FRAMEBUFFER_HH

#include "Texture.hh"

#include <memory>

namespace sym_base
{
  class Framebuffer
  {
   public:
    Framebuffer();
    ~Framebuffer();

    void bind() const;
    void unbind() const;

    void set_color_buffer(const std::shared_ptr<Texture>& texture);
    void create_render_buffer(/* TODO: add class for render buffer */);
    // TODO: set depth/stencil buffer

   private:
    uint32_t m_renderer_id;
    uint32_t m_render_buffer_id;
    uint32_t m_width;
    uint32_t m_height;
    std::shared_ptr<Texture> m_color_texture;
  };
} // namespace sym_base

#endif // SIMULATIONAPP_FRAMEBUFFER_HH
