#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "symbase_pch.hh"

#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer() : m_pumaModel1("meshes/mesh1.puma", {.m_position = true, .m_normal = true})
    {

    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto camera = Renderer::get_camera();

      Renderer::submit(m_pumaModel1);
    }

    virtual void imgui_update(float dt)
    {
      // note: this should be in gui layer
    }

   private:
    struct Vertex
    {
      glm::vec3 m_position;
      glm::vec3 m_color;
      glm::vec2 m_tex_coord;
    };

    Model m_pumaModel1;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
