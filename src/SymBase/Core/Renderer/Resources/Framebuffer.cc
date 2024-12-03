#include "Framebuffer.hh"
#include "Application.hh"
#include "Utils.hh"

namespace sym_base
{
  Framebuffer::Framebuffer(const FramebufferParams& params) : m_width{ 0 }, m_height{ 0 }
  {
    glGenFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    m_msaa.m_enabled = params.m_multisampling;

    if (m_msaa.m_enabled) { glGenFramebuffers(1, &m_msaa.m_framebuffer_id); }
  }

  Framebuffer::~Framebuffer()
  {
    glDeleteRenderbuffers(1, &m_render_buffer_id);
    glDeleteFramebuffers(1, &m_renderer_id);
    if (m_msaa.m_enabled)
    {
      glDeleteFramebuffers(1, &m_msaa.m_framebuffer_id);
      glDeleteTextures(1, &m_msaa.m_color_buffer_id);
    }
  }

  void Framebuffer::bind() const
  {
    if (m_msaa.m_enabled) { glBindFramebuffer(GL_FRAMEBUFFER, m_msaa.m_framebuffer_id); }
    else { glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id); }
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
  }

  void Framebuffer::unbind() const
  {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Framebuffer::blit() const
  {
    ASSERT(m_msaa.m_enabled == true, "Multisampling must be enabled in order to perform blit operation");
    ASSERT(m_msaa.m_color_buffer_id != 0, "Create multisampled buffer before calling blit");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaa.m_framebuffer_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_renderer_id);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
    glBindFramebuffer(GL_FRAMEBUFFER, m_msaa.m_framebuffer_id);
    glGenRenderbuffers(1, &m_render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
    if (m_msaa.m_enabled)
    {
      auto samples = Application::get().get_params().m_samples;
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, m_width, m_height);
    }
    else { glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  void Framebuffer::create_multisampled_buffer()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_msaa.m_framebuffer_id);
    glGenTextures(1, &m_msaa.m_color_buffer_id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaa.m_color_buffer_id);
    auto samples = Application::get().get_params().m_samples;
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, m_width, m_height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           m_msaa.m_color_buffer_id,
                           0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
} // namespace sym_base