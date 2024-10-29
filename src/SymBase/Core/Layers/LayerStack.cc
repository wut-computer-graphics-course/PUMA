#include "LayerStack.hh"

namespace sym_base
{
  LayerStack::LayerStack() { m_head_of_normal_layers = m_layers.begin(); }

  LayerStack::~LayerStack()
  {
    for (Layer* layer : m_layers)
    {
      layer->detach();
      delete layer;
    }
  }

  void LayerStack::push_layer(Layer* layer)
  {
    m_head_of_normal_layers = m_layers.emplace(m_head_of_normal_layers, layer);
  }

  void LayerStack::pop_layer(Layer* layer)
  {
    auto iter = std::find(m_layers.begin(), m_layers.end(), layer);
    if (iter != m_layers.end())
    {
      layer->detach();
      m_layers.erase(iter);
      m_head_of_normal_layers--;
    }
  }
} // namespace sym_base
