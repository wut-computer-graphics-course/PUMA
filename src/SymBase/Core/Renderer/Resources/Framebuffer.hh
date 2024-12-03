#ifndef SIMULATIONAPP_FRAMEBUFFER_HH
#define SIMULATIONAPP_FRAMEBUFFER_HH

#include "Texture.hh"

#include <memory>

namespace sym_base
{
  struct FramebufferParams
  {
    bool m_multisampling = false;
  };

  class Framebuffer
  {
   public:
    Framebuffer(const FramebufferParams& params);
    ~Framebuffer();

    void bind() const;
    void unbind() const;
    void blit() const;

    void set_color_buffer(const std::shared_ptr<Texture>& texture);
    void create_render_buffer(/* TODO: add class for render buffer */);
    void create_multisampled_buffer(/* TODO: add class for anti aliasing buffer */);
    // TODO: set depth/stencil buffer

   private:
    uint32_t m_renderer_id;
    uint32_t m_render_buffer_id;
    uint32_t m_width;
    uint32_t m_height;
    std::shared_ptr<Texture> m_color_texture;

    struct
    {
      bool m_enabled;
      uint32_t m_framebuffer_id;
      uint32_t m_color_buffer_id;
    } m_msaa;
  };
} // namespace sym_base

#endif // SIMULATIONAPP_FRAMEBUFFER_HH
