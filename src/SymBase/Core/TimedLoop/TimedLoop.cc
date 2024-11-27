#include "TimedLoop.hh"
#include "Utils.hh"

#include "pch.hh"

namespace sym_base
{
  TimedLoop::TimedLoop(int period_in_ms, std::function<void(void)> func, LoopStatus status) : m_status{ status }
  {
    m_on_tick = std::move(func);
    m_period  = period_in_ms * NS_IN_MS; // convert to nanoseconds
  }

  void TimedLoop::go()
  {
    while (m_status != LoopStatus::exiting)
    {
      m_timer.tick();
      if (m_timer.get_dt_ns() < m_period)
      {
        // wait that many ns
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_period - m_timer.get_dt_ns()));
        continue;
      }
      else { m_timer.reset(); }

      if (m_status == LoopStatus::running) { m_on_tick(); }
    }
  }

  void TimedLoop::go(uint32_t loops)
  {
    for (uint32_t loop = 0; loop < loops; ++loop)
    {
      if (m_status != LoopStatus::running) break;

      m_timer.tick();
      if (m_timer.get_dt_ns() < m_period)
      {
        // wait that many ns
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_period - m_timer.get_dt_ns()));
        continue;
      }
      else { m_timer.reset(); }

      m_on_tick();
    }
  }
  void TimedLoop::set_status(LoopStatus status) { m_status = status; }
} // namespace sym_base