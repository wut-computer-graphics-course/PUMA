#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include <memory>

#include "Application.hh"
#include "GuiLayer.hh"
#include "Input/Input.hh"
#include "MyLayer.hh"
#include "Renderer/Camera/OrbitCamera.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      RenderCommand::enable_depth_buffering();
      RenderCommand::enable_anti_aliasing();
      RenderCommand::enable_face_culling();

      push_layer(new GuiLayer());
      push_layer(new MyLayer());
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
