#ifndef SIMULATIONAPP_INPUTLAYER_HH
#define SIMULATIONAPP_INPUTLAYER_HH

#include "SimulationContext.hh"
#include "SymBase.hh"
#include "pch.hh"

using namespace sym_base;

namespace sym
{
  class InputLayer : public Layer
  {
   public:
    InputLayer() = default;

    void update(float dt) { handle_keyboard_input(dt); }

   private:
    void handle_keyboard_input(float dt)
    {
      float zoom         = 0;
      glm::vec2 rotation = { 0, 0 };
      if (Input::is_key_pressed(GLFW_KEY_W)) { rotation.y -= m_keyboard_sens; }
      if (Input::is_key_pressed(GLFW_KEY_S)) { rotation.y += m_keyboard_sens; }
      if (Input::is_key_pressed(GLFW_KEY_A)) { rotation.x -= m_keyboard_sens; }
      if (Input::is_key_pressed(GLFW_KEY_D)) { rotation.x += m_keyboard_sens; }
      if (Input::is_key_pressed(GLFW_KEY_Q)) { zoom += m_keyboard_sens; }
      if (Input::is_key_pressed(GLFW_KEY_E)) { zoom -= m_keyboard_sens; }

      SimulationContext::s_camera->zoom(zoom, dt);
      SimulationContext::s_camera->rotate(rotation.x, rotation.y, dt);
    }

   private:
    float m_keyboard_sens = 5.f;
  };
} // namespace sym

#endif // SIMULATIONAPP_INPUTLAYER_HH
