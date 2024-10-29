#include "SymSandbox/SandboxApp.hh"

int main()
{
  sym::SandboxApp app({ .m_title        = "Sym base",
                        .m_width        = 1280,
                        .m_height       = 720,
                        .m_samples      = 4,
                        .m_vsync        = true,
                        .m_refresh_rate = 1 / 60.f,
                        .m_clear_color  = { 1.f, 0.f, 1.f, .1f } });
  {
    app.run();
  }

  return EXIT_SUCCESS;
}