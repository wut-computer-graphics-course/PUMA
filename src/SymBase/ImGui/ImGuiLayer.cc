#include "ImGuiLayer.hh"
#include "Application.hh"

namespace sym_base
{
  void ImGuiLayer::attach()
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // ImGui context configuration
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //    io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    auto& app    = Application::get();
    auto* window = static_cast<GLFWwindow*>(app.get_window().get_handle());

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
  }

  void ImGuiLayer::detach()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::handle_event(Event& event, float dt)
  {
    if (m_block_events)
    {
      ImGuiIO& io = ImGui::GetIO();
      event.handled |= event.is_type(Event::EventTypeMouse) & io.WantCaptureMouse;
      event.handled |= event.is_type(Event::EventTypeKeyboard) & io.WantCaptureKeyboard;
    }
  }

  void ImGuiLayer::begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::end()
  {
    ImGuiIO& io      = ImGui::GetIO();
    Application& app = Application::get();
    io.DisplaySize   = ImVec2((float)app.get_window().get_width(), (float)app.get_window().get_height());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //    {
    //      GLFWwindow* backup_current_context = glfwGetCurrentContext();
    //      ImGui::UpdatePlatformWindows();
    //      ImGui::RenderPlatformWindowsDefault();
    //      glfwMakeContextCurrent(backup_current_context);
    //    }
  }
} // namespace sym_base