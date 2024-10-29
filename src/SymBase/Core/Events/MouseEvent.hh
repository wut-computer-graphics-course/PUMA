#ifndef ESPERT_CORE_MOUSEEVENT_H
#define ESPERT_CORE_MOUSEEVENT_H

#include "Event.hh"

namespace sym_base
{
  /// @brief Event representing movement of a mouse.
  class MouseMovedEvent : public Event
  {
   public:
    /// @brief Constructor setting position of mouse.
    /// @param x Position of the mouse in x axis.
    /// @param y Position of the mouse in y axis.
    MouseMovedEvent(float x, float y) : m_x(x), m_y(y) {}

    ~MouseMovedEvent()
    {
      s_last_x = m_x;
      s_last_y = m_y;
    }

    /// @brief Returns position of the mouse in x axis.
    /// @return Position of the mouse in x axis.
    inline float get_x() const { return m_x; }
    /// @brief Returns position of the mouse in y axis.
    /// @return Position of the mouse in y axis.
    inline float get_y() const { return m_y; }

    inline float get_dx() const { return m_x - s_last_x; }
    inline float get_dy() const { return m_y - s_last_y; }

    EVENT_CLASS_TYPE(EventTypeInput | EventTypeMouse)
    EVENT_CLASS_SUBTYPE(MouseMoved)

   private:
    float m_x;
    float m_y;

    static float s_last_x;
    static float s_last_y;
  };

  inline float MouseMovedEvent::s_last_x = 0.f;
  inline float MouseMovedEvent::s_last_y = 0.f;

  /// @brief Event representing movement of mouse's scroll wheel.
  class MouseScrolledEvent : public Event
  {
   public:
    /// @brief Constructor setting offset of the mouse's scroll wheel.
    /// @param o_x Offset in x axis.
    /// @param o_y Offset in y axis.
    MouseScrolledEvent(float o_x, float o_y) : m_offset_x(o_x), m_offset_y(o_y) {}

    /// @brief Returns mouse wheel's offset in x axis.
    /// @return Mouse wheel's offset in x axis.
    inline float get_offset_x() const { return m_offset_x; }
    /// @brief Returns mouse wheel's offset in y axis.
    /// @return Mouse wheel's offset in y axis.
    inline float get_offset_y() const { return m_offset_y; }

    EVENT_CLASS_TYPE(EventTypeInput | EventTypeMouse)
    EVENT_CLASS_SUBTYPE(MouseScrolled)

   private:
    float m_offset_x;
    float m_offset_y;
  };

  /// @brief Event representing event concerning a mouse button.
  class MouseButtonEvent : public Event
  {
   public:
    /// @brief Returns mouse button's integer code.
    /// @return Mouse button's integer code.
    inline int get_button_code() const { return m_button_code; }

    EVENT_CLASS_TYPE(EventTypeInput | EventTypeMouse)

   protected:
    MouseButtonEvent(int code) : m_button_code(code) {}

    int m_button_code;
  };

  /// @brief Event representing mouse button press.
  class MouseButtonPressedEvent : public MouseButtonEvent
  {
   public:
    /// @brief Constructor setting mouse button code.
    /// @param code Integer code of a mouse button.
    MouseButtonPressedEvent(int code) : MouseButtonEvent(code) {}

    EVENT_CLASS_SUBTYPE(MouseButtonPressed)
  };

  /// @brief Event representing release of mouse button.
  class MouseButtonReleasedEvent : public MouseButtonEvent
  {
   public:
    /// @brief Constructor setting mouse button code.
    /// @param code Integer code of a mouse button.
    MouseButtonReleasedEvent(int code) : MouseButtonEvent(code) {}

    EVENT_CLASS_SUBTYPE(MouseButtonReleased)
  };
} // namespace sym_base

#endif // ESPERT_CORE_MOUSEEVENT_H
