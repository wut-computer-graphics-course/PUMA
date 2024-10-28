#ifndef SYM_BASE_EVENT_HH
#define SYM_BASE_EVENT_HH

#define SET_BIT(b) (1 << (b))

namespace sym_base
{
  /// @brief Abstract interface representing event that is passed around in the engine.
  class Event
  {
   public:
    /// @brief A more specific subtype of and event that listeners can subscribe to.
    enum class EventSubtype
    {
      /* for window class */
      WindowClosed,
      WindowResized,
      /* for input */
      KeyPressed,
      KeyReleased,
      MouseButtonPressed,
      MouseButtonReleased,
      MouseMoved,
      MouseScrolled,
    };

    /// @brief A broad type of event that listeners can subscribe to.
    enum EventType
    {
      EventTypeWindow   = SET_BIT(0),
      EventTypeInput    = SET_BIT(1),
      EventTypeKeyboard = SET_BIT(2),
      EventTypeMouse    = SET_BIT(3),
      EventTypeGui      = SET_BIT(4),
      EventTypeLayer    = SET_BIT(5)
    };

    template<typename T> using EventHandler = std::function<bool(T&)>;

   public:
    /// @brief Flag indicating whether the event has already been handled.
    bool handled = false;

   public:
    /// @brief Returns event's subtype.
    /// @return Subtype of an event.
    virtual EventSubtype get_subtype() const = 0;
    /// @brief Returns event's type.
    /// @return Type of an event.
    virtual int get_type() const = 0;

    /// @brief Checks whether event's type is the same as the passed one.
    /// @param type Type of event to check against.
    /// @return True when type of event is same as the passed one. False otherwise.
    inline bool is_type(EventType type) { return get_type() & type; }

    /// @brief Tries to handle event with passed handler.
    /// @tparam T Type of event class.
    /// @param event Event to handle.
    /// @param handler Handler that will be used to try to handle event.
    /// @return True if event's subtype matches with handlers subtype. False otherwise.
    template<typename T> static bool try_handler(Event& event, EventHandler<T> handler)
    {
      if (event.get_type() == T::get_class_type() && event.get_subtype() == T::get_class_subtype())
      {
        event.handled = handler(*(T*)&event);
        return true;
      }

      return false;
    }
  };
} // namespace sym_base

#define EVENT_CLASS_SUBTYPE(subtype)                                                                          \
  static sym_base::Event::EventSubtype get_class_subtype() { return sym_base::Event::EventSubtype::subtype; } \
  virtual sym_base::Event::EventSubtype get_subtype() const override { return get_class_subtype(); }

#define EVENT_CLASS_TYPE(type)                                             \
  static int get_class_type() { return sym_base::Event::EventType::type; } \
  virtual int get_type() const override { return type; }

#endif // SYM_BASE_EVENT_HH
