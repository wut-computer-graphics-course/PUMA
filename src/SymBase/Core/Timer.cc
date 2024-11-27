#include "Timer.hh"
#include "Clock.hh"

namespace sym_base
{
  Timer::Timer() : m_delta_time{ 0 } { reset(); }

  void Timer::tick()
  {
    auto current_time = Clock::chrono_now();
    auto duration =
        std::chrono::duration<uint64_t, std::chrono::nanoseconds::period>(current_time - m_start_time).count();
    m_delta_time = (float)duration / NS_IN_S;
  }

  void Timer::reset() { m_start_time = Clock::chrono_now(); }
} // namespace sym_base