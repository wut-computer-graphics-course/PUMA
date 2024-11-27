#ifndef SYM_BASE_CLOCK_HH
#define SYM_BASE_CLOCK_HH

namespace sym_base
{
  class Clock
  {
   public:
    static void start();
    static void reset();
    static void pause();
    static void resume();
    static std::chrono::high_resolution_clock::time_point chrono_now();
    static float now();
    static bool running();
  };
} // namespace sym_base

#endif // SYM_BASE_CLOCK_HH
