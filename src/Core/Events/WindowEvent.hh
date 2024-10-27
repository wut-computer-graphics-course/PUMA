#ifndef SYM_BASE_WINDOWEVENT_H
#define SYM_BASE_WINDOWEVENT_H

#include "Event.hh"

namespace sym_base
{
  /// @brief Event representing closing of window.
  class WindowClosedEvent : public Event
  {
   public:
    /// @brief Default constructor.
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(EventTypeWindow)
    EVENT_CLASS_SUBTYPE(WindowClosed)
  };

  /// @brief Event representing resize of window.
  class WindowResizedEvent : public Event
  {
   public:
    /// @brief Constructor setting new width and height of window.
    /// @param width New window width.
    /// @param height New window height.
    WindowResizedEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}
    /// @brief Returns new window width.
    /// @return New window width.
    inline unsigned int get_width() const { return m_width; }

    /// @brief Returns new window height.
    /// @return New window height.
    inline unsigned int get_height() const { return m_height; }
    EVENT_CLASS_TYPE(EventTypeWindow)
    EVENT_CLASS_SUBTYPE(WindowResized)

   private:
    unsigned int m_width;
    unsigned int m_height;
  };
} // namespace sym_base

#endif // SYM_BASE_WINDOWEVENT_H
