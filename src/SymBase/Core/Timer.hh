#ifndef SYM_BASE_TIMER_HH
#define SYM_BASE_TIMER_HH

#include <chrono>

namespace sym_base
{
  class Timer
  {
   public:
    Timer();
    ~Timer() = default;

    /// @brief Updates previous and current's tick time.
    void tick();
    /// @brief Resets tick time.
    void reset();
    /// @brief Returns delta time between ticks in seconds.
    /// @return Delta time between ticks.
    inline float get_dt() { return m_delta_time; }

   private:
    std::chrono::high_resolution_clock::time_point m_start_time;
    float m_delta_time;
  };
} // namespace sym_base

#endif // SYM_BASE_TIMER_HH
