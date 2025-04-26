#ifndef PUMA_GUISIMULATIONRESUMEDEVENT_HH
#define PUMA_GUISIMULATIONRESUMEDEVENT_HH

#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  class GuiSimulationResumedEvent : public Event
  {
   public:
    GuiSimulationResumedEvent() {}

    EVENT_CLASS_TYPE(Other)
    EVENT_CLASS_CATEGORY(EventCategoryLayer)
    EVENT_CLASS_NAME(GuiSimulationResumedEvent)
  };
} // namespace sym

#endif // PUMA_GUISIMULATIONRESUMEDEVENT_HH
