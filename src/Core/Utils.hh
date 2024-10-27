#ifndef SYM_BASE_UTILS_HH
#define SYM_BASE_UTILS_HH

#include "pch.hh"

#define BIND_EVENT_FOR_FUN(obj, ...) std::bind(&obj, this, std::placeholders::_1, ##__VA_ARGS__)

#define NS_IN_S  1000000000L
#define NS_IN_MS 1000000L

namespace sym_base
{
  class Event;

  using EventCallbackFn = std::function<void(Event&)>;
} // namespace sym_base

#endif // SYM_BASE_UTILS_HH
