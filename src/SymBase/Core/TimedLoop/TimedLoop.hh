#ifndef SYM_BASE_TIMEDLOOP_HH
#define SYM_BASE_TIMEDLOOP_HH

#include "LoopStatus.hh"
#include "Timer.hh"

#include <functional>

namespace sym_base
{
  /// @brief Simulation of real-time synchronized loop
  class TimedLoop
  {
   public:
    /// @brief Constructor
    /// @param period_in_ms loop m_period in milliseconds
    /// @param func function that should be called in loop
    /// @param status reference to controlling status
    TimedLoop(int period_in_ms, std::function<void(void)> func, LoopStatus status);
    /// @brief start infinite loop
    void go();
    /// @brief start loop for specific cycle numbers
    /// @param loops how many cycles should be done
    void go(uint32_t loops);
    /// @breif changes loop status
    /// @param status new status
    void set_status(LoopStatus status);

   private:
    /// @brief loop function
    std::function<void(void)> m_on_tick;
    Timer m_timer;
    uint64_t m_period;
    LoopStatus m_status;
  };
} // namespace sym_base

#endif // SYM_BASE_TIMEDLOOP_HH
