#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static std::shared_ptr<OrbitCamera> s_camera;
  };

  inline std::shared_ptr<OrbitCamera> SimulationContext::s_camera = nullptr;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
