#include "Clock.hh"

#include "pch.hh"

namespace sym_base
{
  using ClockType = std::chrono::high_resolution_clock;
  using TimePoint = ClockType::time_point;
  using Duration  = std::chrono::duration<float>;

  TimePoint start_time;
  TimePoint pause_time;
  Duration paused_duration;
  bool is_running = false;
  bool is_paused  = false;

  void Clock::start()
  {
    start_time      = chrono_now();
    paused_duration = Duration::zero();
    is_running      = true;
    is_paused       = false;
  }

  void Clock::reset()
  {
    is_running = false;
    is_paused  = false;
  }

  void Clock::pause()
  {
    if (is_running && !is_paused)
    {
      pause_time = ClockType::now();
      is_paused  = true;
    }
  }

  void Clock::resume()
  {
    if (is_running && is_paused)
    {
      paused_duration += ClockType::now() - pause_time;
      is_paused = false;
    }
  }

  std::chrono::high_resolution_clock::time_point Clock::chrono_now() { return ClockType::now(); }

  float Clock::now()
  {
    if (is_running)
    {
      if (is_paused) { return std::chrono::duration_cast<Duration>(pause_time - start_time - paused_duration).count(); }
      return std::chrono::duration_cast<Duration>(ClockType::now() - start_time - paused_duration).count();
    }
    return 0.f;
  }

  bool Clock::running() { return is_running; }
} // namespace sym_base