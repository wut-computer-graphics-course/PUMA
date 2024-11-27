#ifndef SYM_BASE_UTILS_HH
#define SYM_BASE_UTILS_HH

#define BIND_EVENT_FOR_FUN(obj, ...) std::bind(&obj, this, std::placeholders::_1, ##__VA_ARGS__)

#include <iostream>

#define ASSERT(condition, ...)                                            \
  do                                                                      \
  {                                                                       \
    if (!(condition))                                                     \
    {                                                                     \
      std::cerr << "Assertion failed: " << #condition;                    \
      if (sizeof(__VA_ARGS__) > 1) { std::cerr << " - " << __VA_ARGS__; } \
      std::cerr << "\n";                                                  \
      std::cerr << "in file: " << __FILE__ << "\n";                       \
      std::cerr << "in function: " << __FUNCTION__ << "\n";               \
      std::cerr << "at line: " << __LINE__ << "\n";                       \
      std::abort();                                                       \
    }                                                                     \
  } while (0)

#define NS_IN_S  1000000000L
#define NS_IN_MS 1000000L
#define MS_IN_S  1000

#include <functional>

namespace sym_base
{
  class Event;

  using EventCallbackFn = std::function<void(Event&)>;
} // namespace sym_base

#endif // SYM_BASE_UTILS_HH
