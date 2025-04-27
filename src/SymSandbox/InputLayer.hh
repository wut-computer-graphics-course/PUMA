#ifndef SIMULATIONAPP_INPUTLAYER_HH
#define SIMULATIONAPP_INPUTLAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"

using namespace sym_base;

namespace sym
{
  class InputLayer : public Layer
  {
   public:
    InputLayer() = default;

    void update(float dt) { handle_keyboard_input(dt); }

    void handle_event(Event& e, float dt) override
    {
      EventDispatcher dispatcher(e);
      dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FOR_FUN(InputLayer::mouse_moved_event_handler, dt));
    }

   private:
    void handle_keyboard_input(float dt)
    {
      auto camera = static_cast<FpsCamera*>(Renderer::get_camera());
      if (Input::is_key_pressed(GLFW_KEY_W)) { camera->move(FORWARD, m_keyboard_sens * dt); }
      if (Input::is_key_pressed(GLFW_KEY_S)) { camera->move(BACKWARD, m_keyboard_sens * dt); }
      if (Input::is_key_pressed(GLFW_KEY_A)) { camera->move(LEFT, m_keyboard_sens * dt); }
      if (Input::is_key_pressed(GLFW_KEY_D)) { camera->move(RIGHT, m_keyboard_sens * dt); }
      if (Input::is_key_pressed(GLFW_KEY_E)) { camera->move(UP, m_keyboard_sens * dt); }
      if (Input::is_key_pressed(GLFW_KEY_Q)) { camera->move(DOWN, m_keyboard_sens * dt); }
    }

    bool mouse_moved_event_handler(sym_base::MouseMovedEvent& e, float dt)
    {
      auto camera        = static_cast<FpsCamera*>(Renderer::get_camera());
      bool input_handled = false;

      if (Input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
      {
        camera->look_at(e.get_dx(), -e.get_dy(), m_mouse_sens * dt);
        input_handled = true;
      }

      return input_handled;
    }

   private:
    float m_keyboard_sens = 5.f;
    float m_mouse_sens    = 4.f;
  };
} // namespace sym

#endif // SIMULATIONAPP_INPUTLAYER_HH
