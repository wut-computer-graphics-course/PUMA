#ifndef SYM_BASE_WINDOWEVENT_H
#define SYM_BASE_WINDOWEVENT_H

#include "Event.hh"

#include <cstdint>

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
    WindowResizedEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}
    /// @brief Returns new window width.
    /// @return New window width.
    inline uint32_t get_width() const { return m_width; }

    /// @brief Returns new window height.
    /// @return New window height.
    inline uint32_t get_height() const { return m_height; }
    EVENT_CLASS_TYPE(EventTypeWindow)
    EVENT_CLASS_SUBTYPE(WindowResized)

   private:
    uint32_t m_width;
    uint32_t m_height;
  };
} // namespace sym_base

#endif // SYM_BASE_WINDOWEVENT_H
