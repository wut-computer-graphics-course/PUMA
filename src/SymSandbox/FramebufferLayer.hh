#ifndef SYM_BASE_FRAMEBUFFERLAYER_HH
#define SYM_BASE_FRAMEBUFFERLAYER_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"
#include "SimulationContext.hh"
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
        float vertices[] = {
          // Coords   // TexCoords
          -1.f, -1.f, 0.f, 0.f, //
          1.f,  -1.f, 1.f, 0.f, //
          1.f,  1.f,  1.f, 1.f, //
          -1.f, 1.f,  0.f, 1.f  //
        };

        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(float));

        BufferLayout layout = { { SharedDataType::Float2, "a_Position" }, { SharedDataType::Float2, "a_TexCoord" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_framebuffer.m_va = std::make_shared<VertexArray>();
        m_framebuffer.m_va->add_vertex_buffer(vertex_buffer);
        m_framebuffer.m_va->set_index_buffer(index_buffer);

        m_framebuffer.m_shader = std::make_shared<Shader>("shaders/framebuffer.glsl");

        m_framebuffer.m_texture = std::make_shared<Texture2D>(m_framebuffer.m_width, m_framebuffer.m_height);

        FramebufferParams params{ .m_multisampling = true };
        m_framebuffer.m_buffer = std::make_shared<Framebuffer>(params);
        m_framebuffer.m_buffer->set_color_buffer(m_framebuffer.m_texture);
        m_framebuffer.m_buffer->create_render_buffer();
        m_framebuffer.m_buffer->create_multisampled_buffer();
      }

      push_child_layer(new SimulationLayer());
    }
    ~FramebufferLayer() = default;

    void update(float dt) override
    {
      auto& window           = Application::get().get_window();
      auto rendering_context = window.get_rendering_context();

      auto& camera = SimulationContext::s_camera;
      camera->set_perspective(M_PI / 4, m_framebuffer.m_win_width / m_framebuffer.m_win_height, 1.f, 100.f);

      Renderer::begin_scene();
      {
        rendering_context->set_viewport(0, 0, m_framebuffer.m_width, m_framebuffer.m_height);

        m_framebuffer.m_buffer->bind();
        {
          // clear buffer
          RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
          RenderCommand::clear();
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
      }
      ImGui::End();
      ImGui::PopStyleVar();
    }

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      std::shared_ptr<Texture2D> m_texture;
      std::shared_ptr<Framebuffer> m_buffer;
      const uint32_t m_width  = 800;
      const uint32_t m_height = 600;
      float m_win_width       = 800;
      float m_win_height      = 600;
    } m_framebuffer;
  };
} // namespace sym

#endif // SYM_BASE_FRAMEBUFFERLAYER_HH
