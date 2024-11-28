#ifndef SYM_BASE_DOCKSPACELAYER_HH
#define SYM_BASE_DOCKSPACELAYER_HH

#include "SymBase.hh"

namespace sym
{
  struct DockWinId
  {
    static const std::string s_content;
    static const std::string s_settings;
  };

  inline const std::string DockWinId::s_content  = "Content";
  inline const std::string DockWinId::s_settings = "Settings";

  class DockSpaceLayer : public sym_base::Layer
  {
   public:
    DockSpaceLayer()  = default;
    ~DockSpaceLayer() = default;

    void imgui_update(float dt) override
    {
      static ImGuiDockNodeFlags dock_space_flags = ImGuiDockNodeFlags_PassthruCentralNode;
      ImGuiWindowFlags window_flags              = ImGuiWindowFlags_NoDocking;

      ImGuiViewport* viewport = ImGui::GetMainViewport();

      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);

      window_flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
      window_flags |= ImGuiWindowFlags_NoBackground;

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
      ImGui::Begin("Root", nullptr, window_flags);
      {
        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
          ImGuiID dock_space_id = ImGui::GetID("RootDockSpace");
          ImGui::DockSpace(dock_space_id, ImVec2(0.f, 0.f), dock_space_flags);

          static auto first_time = true;
          if (first_time)
          {
            first_time = false;
            // Clear out existing layout
            ImGui::DockBuilderRemoveNode(dock_space_id);
            // Add empty node
            ImGui::DockBuilderAddNode(dock_space_id, dock_space_flags | ImGuiDockNodeFlags_DockSpace);
            // Main node should cover entire window
            ImGui::DockBuilderSetNodeSize(dock_space_id, ImGui::GetWindowSize());
            // get id of main dock space area
            ImGuiID dock_space_main_id = dock_space_id;
            // Create a dock node for the right docked window
            ImGuiID left =
                ImGui::DockBuilderSplitNode(dock_space_main_id, ImGuiDir_Left, .25f, nullptr, &dock_space_main_id);

            ImGui::DockBuilderDockWindow(DockWinId::s_content.c_str(), dock_space_main_id);
            ImGui::DockBuilderDockWindow(DockWinId::s_settings.c_str(), left);
            ImGui::DockBuilderFinish(dock_space_id);
          }
        }
      }
      ImGui::End();
      ImGui::PopStyleVar(3);
    }
  };
} // namespace sym

#endif // SYM_BASE_DOCKSPACELAYER_HH
