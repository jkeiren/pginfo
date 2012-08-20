#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include "graph.h"

namespace graph
{

template<typename graph_t>
class neighbourhood : bfs_algorithm<graph_t>
{
protected:
  typedef bfs_algorithm<graph_t> super;
  typedef typename super::vertex_t vertex_t;
  typedef typename super::VertexIndex VertexIndex;

  size_t m_k; // Set in kneighbourhood
  size_t m_k_neighbourhood; // Belongs to m_k
  size_t m_edges_in_k_neighbourhood; // Belongs to m_k
  using super::m_level;
  using super::m_graph;

  std::map<VertexIndex, size_t> m_k_neighbourhood_per_vertex;
  std::map<VertexIndex, size_t> m_k_edges_in_neighbourhood_per_vertex;

  class done_exception : public std::exception
  {
    public:
      done_exception() : std::exception()
      {}
  };

  virtual
  void start_level(const size_t level, const size_t num_vertices)
  {
    if(level <= m_k)
    {
      m_k_neighbourhood += num_vertices;
    }
    else
    {
      throw done_exception();
    }
  }

  virtual
  void old_node_seen(const VertexIndex current_node, const VertexIndex old_node)
  {
    if(m_level[old_node] <= m_k)
    {
      ++m_edges_in_k_neighbourhood;
    }
  }

  virtual
  void new_node_seen(const VertexIndex current_node, const VertexIndex new_node)
  {
    if(m_level[new_node] <= m_k)
    {
      ++m_edges_in_k_neighbourhood;
    }
  }

  void kneighbourhood(VertexIndex v)
  {
    // k neighbourhood is the number of vertices reachable from v in k steps.
    // corresponds to the first k BFS levels, starting in v.
    m_k_neighbourhood = 0;
    m_edges_in_k_neighbourhood = 0;
    try
    {
      bfs(v);
    }
    catch (done_exception&)
    {}
  }

  void kneighbourhood()
  {
    for(VertexIndex i = 0; i < m_graph.size(); ++i)
    {
      kneighbourhood(i);
      m_k_neighbourhood_per_vertex[i] = m_k_neighbourhood;
      m_k_edges_in_neighbourhood_per_vertex[i] = m_edges_in_k_neighbourhood;
    }
  }

public:
  neighbourhood(const graph_t& g, size_t k)
    : super(g),
      m_k(k)
  {
    kneighbourhood();
  }

  void yaml(YAML::Emitter& out) const
  {
    std::stringstream key1, key2;
    key1 << "Sizes of " << m_k << "-neighbourhood per vertex";
    key2 << "Edges in " << m_k << "-neighbourhood per vertex";
    out << YAML::BeginMap
        << YAML::Key << key1.str()
        << YAML::Value << m_k_neighbourhood_per_vertex
        << YAML::Key << key2.str()
        << YAML::Value << m_k_edges_in_neighbourhood_per_vertex
        << YAML::EndMap;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const neighbourhood<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // NEIGHBOURHOOD_H
