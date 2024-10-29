#ifndef SYM_BASE_GUILAYER_HH
#define SYM_BASE_GUILAYER_HH

#include "SymBase.hh"
#include "pch.hh"

namespace sym
{
  class GuiLayer : public sym_base::Layer
  {
   public:
    GuiLayer()  = default;
    ~GuiLayer() = default;

    void imgui_update(float dt) override { ImGui::ShowDemoWindow(); }
  };
} // namespace sym

#endif // SYM_BASE_GUILAYER_HH
