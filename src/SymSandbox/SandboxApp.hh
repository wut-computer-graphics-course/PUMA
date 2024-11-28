#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"
#include "InputLayer.hh"
#include "MyLayer.hh"
#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      // setup simulation
      SimulationContext::s_camera = std::make_shared<OrbitCamera>();
      SimulationContext::s_camera->set_position({ 0, 0, 5 });

      // create application layers
      push_layer(new MyLayer());
      push_layer(new InputLayer());
      push_layer(new DockSpaceLayer());
    }

    ~SandboxApp() override {}

    virtual void update(float dt) override
    {
      if (Input::is_key_pressed(GLFW_KEY_ESCAPE)) { m_running = false; }
      Application::update(dt);
    }
  };
} // namespace sym

#endif // SYM_BASE_SANDBOXAPP_HH
