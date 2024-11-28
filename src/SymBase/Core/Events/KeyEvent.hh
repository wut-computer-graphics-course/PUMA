#ifndef SYM_BASE_KEYEVENT_H
#define SYM_BASE_KEYEVENT_H

#include "Event.hh"

namespace sym_base
{
  /// @brief Represents a general event sent out by a keyboard.
  class KeyEvent : public Event
  {
   protected:
    int m_key_code;

    KeyEvent(int code) : m_key_code(code) {}

   public:
    /// @brief Returns code of the key that the event is about.
    /// @return Integer code of a keyboard key.
    inline int get_code() const { return m_key_code; }

    EVENT_CLASS_TYPE(EventTypeInput | EventTypeKeyboard)
  };

  /// @brief Event representing press event of a keyboard key.
  class KeyPressedEvent : public KeyEvent
  {
   private:
    bool m_is_repeated;

   public:
    /// @brief Constructor sets key code and bool informing if it is the initial press event.
    /// @param code Integer code of a keyboard key.
    /// @param is_repeated True if this is not the initial keypress (event repeated). False otherwise.
    KeyPressedEvent(int code, bool is_repeated) : KeyEvent(code), m_is_repeated(is_repeated) {}

    /// @brief Checks if this is repeated press event.
    /// @return True if this is not the initial keypress (event repeated). False otherwise.
    inline bool is_repeated() const { return m_is_repeated; }

    EVENT_CLASS_SUBTYPE(KeyPressed)
  };

  /// @brief Event representing release event of a keyboard key.
  class KeyReleasedEvent : public KeyEvent
  {
   public:
    /// @brief Constructor sets key code.
    /// @param code Integer code of a keyboard key.
    KeyReleasedEvent(int code) : KeyEvent(code) {}

    EVENT_CLASS_SUBTYPE(KeyReleased)
  };
} // namespace esp

#endif // SYM_BASE_KEYEVENT_H
