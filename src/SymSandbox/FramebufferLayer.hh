#ifndef SYM_BASE_FRAMEBUFFERLAYER_HH
#define SYM_BASE_FRAMEBUFFERLAYER_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"
#include "ShadowVolumeExampleLayer.hh"
#include "SimulationLayer.hh"

using namespace sym_base;

namespace sym
{
  class FramebufferLayer : public Layer
  {
   public:
    FramebufferLayer()
    {
      // framebuffer
      {
        m_framebuffer.m_texture = std::make_shared<Texture2D>(m_framebuffer.m_width, m_framebuffer.m_height);
        FramebufferParams params{ .m_multisampling = true };
        m_framebuffer.m_buffer = std::make_shared<Framebuffer>(params);
        m_framebuffer.m_buffer->set_color_buffer(m_framebuffer.m_texture);
        m_framebuffer.m_buffer->create_render_buffer();
        m_framebuffer.m_buffer->create_multisampled_buffer();
      }

      m_camera = std::make_shared<OrbitCamera>();
      m_camera->set_position({ 0, 5, 5 });

      // push_child_layer(new SimulationLayer());
      push_child_layer(new ShadowVolumeExampleLayer());
    }
    ~FramebufferLayer() = default;

    void update(float dt) override
    {
      auto& window           = Application::get().get_window();
      auto rendering_context = window.get_rendering_context();
      rendering_context->set_viewport(0, 0, m_framebuffer.m_width, m_framebuffer.m_height);

      auto aspect_ratio = m_framebuffer.m_win_width / m_framebuffer.m_win_height;
      m_camera->set_perspective(aspect_ratio);

      // clear viewport
      RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
      RenderCommand::clear(ClearBufferMask::COLOR_BUFFER_BIT);

      Renderer::begin_scene(nullptr, m_camera.get());
      {
        m_framebuffer.m_buffer->bind();
        {
          // clear framebuffer
          RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
          RenderCommand::clear(ClearBufferMask::DEPTH_BUFFER_BIT | ClearBufferMask::COLOR_BUFFER_BIT |
                               ClearBufferMask::STENCIL_BUFFER_BIT);
          // enable rendering features
          RenderCommand::depth_buffering(true);
          RenderCommand::anti_aliasing(true);
          RenderCommand::face_culling(true);
          RenderCommand::alpha_blending(true);
          // update children layers
          Layer::update(dt);
        }
        m_framebuffer.m_buffer->unbind();

        m_framebuffer.m_buffer->blit();
      }
      Renderer::end_scene();
    }

    void imgui_update(float dt)
    {
      Layer::imgui_update(dt);

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
      ImGui::Begin(DockWinId::s_content.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
      {
        ImVec2 win_size            = ImGui::GetWindowSize();
        m_framebuffer.m_win_width  = win_size.x;
        m_framebuffer.m_win_height = win_size.y;

        ImGui::Image((ImTextureID)(intptr_t)m_framebuffer.m_texture->get_id(), win_size, ImVec2(0, 1), ImVec2(1, 0));

        auto viewport_hovered = ImGui::IsWindowHovered();
        Application::get().get_imgui_layer().block_events(!viewport_hovered);
      }
      ImGui::End();
      ImGui::PopStyleVar();
    }

    struct
    {
      std::shared_ptr<Texture2D> m_texture;
      std::shared_ptr<Framebuffer> m_buffer;
      const uint32_t m_width  = 800;
      const uint32_t m_height = 600;
      float m_win_width       = 800;
      float m_win_height      = 600;
    } m_framebuffer;

    std::shared_ptr<OrbitCamera> m_camera;
  };
} // namespace sym

#endif // SYM_BASE_FRAMEBUFFERLAYER_HH
