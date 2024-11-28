#ifndef SYM_BASE_GUILAYER_HH
#define SYM_BASE_GUILAYER_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class GuiLayer : public Layer
  {
   public:
    GuiLayer()  = default;
    ~GuiLayer() = default;

    void update(float dt) override
    {
      m_fps_manager.m_frames_count++;
      m_fps_manager.m_current_time += dt;
      if (m_fps_manager.m_current_time >= m_fps_manager.m_display_time)
      {
        m_fps_manager.m_current_fps  = 1.f / dt;
        m_fps_manager.m_current_time = 0;
      }
    }

    void imgui_update(float dt) override
    {
      ImGui::Begin(DockWinId::s_settings.c_str());
      {
        ImGui::Text("FPS: %.1f", m_fps_manager.m_current_fps);
        ImGui::Text("Simulation time: %.1f", Clock::now());
        ImGui::Spacing();
      }
      ImGui::End();
    }

   private:
    struct
    {
      int m_frames_count         = 0;
      float m_current_fps        = 0;
      float m_current_time       = 0;
      const float m_display_time = .25f;
    } m_fps_manager;
  };
} // namespace sym

#endif // SYM_BASE_GUILAYER_HH
