#include "Framebuffer.hh"
#include "Utils.hh"

namespace sym_base
{
  Framebuffer::Framebuffer() : m_width{ 0 }, m_height{ 0 }
  {
    glGenFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
  }

  Framebuffer::~Framebuffer()
  {
    glDeleteRenderbuffers(1, &m_render_buffer_id);
    glDeleteFramebuffers(1, &m_renderer_id);
  }

  void Framebuffer::bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
  }

  void Framebuffer::unbind() const
  {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Framebuffer::set_color_buffer(const std::shared_ptr<Texture>& texture)
  {
    m_color_texture = texture;
    m_width         = texture->get_width();
    m_height        = texture->get_height();
    if (dynamic_cast<Texture2D*>(texture.get()))
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture->get_id(), 0);
    }
    else { throw std::runtime_error("Framebuffer can only have a 2D color texture"); }
  }

  void Framebuffer::create_render_buffer()
  {
    glGenRenderbuffers(1, &m_render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);
  }
} // namespace sym_base