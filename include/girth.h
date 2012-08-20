#ifndef GIRTH_H
#define GIRTH_H

#include "bfs_info.h"

namespace graph
{

template <typename graph_t>
class girth_info : bfs_algorithm<graph_t>
{
protected:
  typedef bfs_algorithm<graph_t> super;
  typedef typename super::VertexIndex VertexIndex;
  typedef typename super::vertex_t vertex_t;

  size_t m_girth;
  using super::m_color;
  using super::m_level;
  using super::m_graph;
  using super::black;

  class done_exception : public std::exception
  {
    public:
      done_exception() : std::exception()
      {}
  };

  virtual
  void old_node_seen(const VertexIndex current_node, const VertexIndex old_node)
  {
    if(current_node == old_node) // self loop
    {
      m_girth = 1;
    }
    else if(m_color[old_node] == black) // cross-level edge
    {
      m_girth = (std::min)(m_girth, m_level[current_node] - m_level[old_node] + 1);
    }
    throw done_exception();
  }

  void compute_girth()
  {
    for(VertexIndex i = 0; i < m_graph.size(); ++i)
    {
      try
      {
        super::bfs(i);
      }
      catch(done_exception&)
      {}

      if(m_girth == 1)
      {
        break;
      }
    }
  }

public:
  girth_info(const graph_t& g)
    : super(g),
      m_girth(std::numeric_limits<size_t>::max())
  {
    compute_girth();
  }

  size_t girth() const
  {
    return m_girth;
  }

  void yaml(YAML::Emitter& out) const
  {
    out << girth();
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const girth_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // GIRTH_H
