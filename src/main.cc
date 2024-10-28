#include "Application.hh"
#include "Input/Input.hh"
#include "Simulation/GuiLayer.hh"
#include "Simulation/MyLayer.hh"

using namespace sym_base;
using namespace sym;

class SimulationApp : public Application
{
 public:
  SimulationApp(const ApplicationParams& params) : Application(params)
  {
    RenderCommand::enable_depth_buffering();
    RenderCommand::enable_anti_aliasing();
    RenderCommand::enable_face_culling();

    push_layer(new GuiLayer());
    push_layer(new MyLayer());
  }

  ~SimulationApp() override {}

  virtual void update(float dt) override
  {
    if (Input::is_key_pressed(GLFW_KEY_ESCAPE)) { m_running = false; }
    Application::update(dt);
  }
};

int main()
{
  SimulationApp app({ .m_title        = "Sym base",
                      .m_width        = 1280,
                      .m_height       = 720,
                      .m_samples      = 4,
                      .m_vsync        = true,
                      .m_refresh_rate = 1 / 60.f,
                      .m_clear_color  = { .1f, .1f, .1f, .1f } });
  {
    app.run();
  }

  return EXIT_SUCCESS;
}