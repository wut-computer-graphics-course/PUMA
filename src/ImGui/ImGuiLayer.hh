#ifndef SYM_BASE_IMGUILAYER_HH
#define SYM_BASE_IMGUILAYER_HH

#include "Layers/Layer.hh"

namespace sym_base
{
  class ImGuiLayer : public Layer
  {
   public:
    ImGuiLayer()  = default;
    ~ImGuiLayer() = default;

    void attach() override;
    void detach() override;
    void handle_event(Event& event, float dt) override;

    void block_events(bool val) { m_block_events = val; }

    void begin();
    void end();

   private:
    bool m_block_events = true;
  };
} // namespace sym_base

#endif // SYM_BASE_IMGUILAYER_HH
