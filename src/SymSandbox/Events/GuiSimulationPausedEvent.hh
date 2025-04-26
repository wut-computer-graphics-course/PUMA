#ifndef PUMA_GUISIMULATIONPAUSEDEVENT_HH
#define PUMA_GUISIMULATIONPAUSEDEVENT_HH

#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  class GuiSimulationPausedEvent : public Event
  {
   public:
    GuiSimulationPausedEvent() {}

    EVENT_CLASS_TYPE(Other)
    EVENT_CLASS_CATEGORY(EventCategoryLayer)
    EVENT_CLASS_NAME(GuiSimulationPausedEvent)
  };
} // namespace sym

#endif // PUMA_GUISIMULATIONPAUSEDEVENT_HH
