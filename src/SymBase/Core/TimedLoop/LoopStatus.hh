#ifndef SYM_BASE_STATUS_HH
#define SYM_BASE_STATUS_HH

namespace sym_base
{
  enum class LoopStatus
  {
    /// @brief loop is ready to run
    idle = 1,
    /// @brief loop is running
    running = 2,
    /// @brief loop will be break in next occasion.
    exiting = 3,
    /// @brief loop job should be reloaded
    reload = 4
  };
}

#endif // SYM_BASE_STATUS_HH
