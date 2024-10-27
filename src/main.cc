#include "Application.hh"
#include "Input/Input.hh"

using namespace sym_base;

class SimulationApp : public Application
{
 public:
  SimulationApp(const ApplicationParams& params) : Application(params) {}

  ~SimulationApp() override {}

  virtual void update(float dt) override
  {
    if (Input::is_key_pressed(GLFW_KEY_ESCAPE)) { m_running = false; }
  }
};

int main()
{
  SimulationApp app({
      .m_title        = "Sym base",
      .m_width        = 1280,
      .m_height       = 720,
      .m_vsync        = true,
      .m_refresh_rate = 1 / 60.f,
  });
  {
    app.run();
  }

  return EXIT_SUCCESS;
}