#ifndef DIAMETER_H
#define DIAMETER_H

#include <yaml-cpp/emitter.h>

namespace graph
{

template <typename graph_t>
class diameter_info
{
  typedef graph::VertexIndex VertexIndex;
  typedef size_t weight_t;

protected:
  graph_t& m_graph;

  std::vector<size_t> m_min_distance;
  std::set< std::pair<weight_t, VertexIndex> > m_vertex_queue;
  size_t m_diameter;

  void initialise_single_source(VertexIndex s)
  {
    m_min_distance = std::vector<size_t>(m_graph.size(), std::numeric_limits<size_t>::max());
    m_min_distance[s] = 0;
  }

  void relax(VertexIndex u, VertexIndex v)
  {
    if(m_min_distance[v] > m_min_distance[u] + 1)
    {
      m_vertex_queue.erase(std::make_pair(m_min_distance[v], v));
      m_min_distance[v] = m_min_distance[u] + 1;
      m_vertex_queue.insert(std::make_pair(m_min_distance[v], v));
    }
  }

  void dijkstra(VertexIndex s)
  {
    mCRL2log(debug, "Dijkstra") << "Running SSSP with source " << s << " out of " << m_graph.size() << std::endl;
    initialise_single_source(s);
    m_vertex_queue.insert(std::make_pair(m_min_distance[s], s));

    while(!m_vertex_queue.empty())
    {
      VertexIndex u = m_vertex_queue.begin()->second;
      m_vertex_queue.erase(m_vertex_queue.begin());

      // Optimisation, record on the fly.
      if(m_min_distance[u] != std::numeric_limits<size_t>::max())
      {
        m_diameter = (std::max)(m_diameter, m_min_distance[u]);
      }

      typename graph_t::vertex_t v = m_graph.vertex(u);
      for(std::set<VertexIndex>::const_iterator i = v.out.begin(); i != v.out.end(); ++i)
      {
        relax(u,*i);
      }
    }
  }

  // Run single source shortest path from every vertex, and update diameter
  // accordingly.
  void compute_diameter()
  {
    mCRL2log(debug) << "Computing diameter using Dijkstra's SSSP algorithm from all sources" << std::endl;
    for(VertexIndex i = 0; i < m_graph.size(); ++i)
    {
      dijkstra(i);
    }
  }

public:
  diameter_info(graph_t& g)
    : m_graph(g),
      m_diameter(0)
  {
    compute_diameter();
  }

  size_t diameter() const
  {
    return m_diameter;
  }

  void yaml(YAML::Emitter& out) const
  {
    out << diameter();
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const diameter_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // DIAMETER_H
