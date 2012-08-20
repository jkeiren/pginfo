/// \author Jeroen Keiren
/// \file graph_statistics.h
/// \brief Description comes here

#ifndef GRAPH_STATISTICS_H
#define GRAPH_STATISTICS_H

#include <map>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits>

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/stlemitter.h>

#include "mcrl2/utilities/logger.h"
#include "utilities.h"

namespace graph
{

using mcrl2::log::verbose;
using mcrl2::log::status;
using mcrl2::log::debug;

template <typename graph_t>
class graph_info
{
protected:
  const graph_t& m_graph;
  typedef std::map<size_t, size_t> degree_distribution_t;

public:
  graph_info(const graph_t& g)
    : m_graph(g)
  {}

  size_t num_vertices() const
  {
    return m_graph.size();
  }

  size_t num_edges() const
  {
    return m_graph.num_edges();
  }

  double average_degree() const
  {
    assert(num_edges() > 0);
    double v = num_vertices();
    double e = num_edges();
    return v/e;
  }

  degree_distribution_t out_degrees() const
  {
    // map out degree to number of vertices with this outdegree
    degree_distribution_t result;

    for(typename graph_t::vertices_t::const_iterator i = m_graph.vertices().begin(); i != m_graph.vertices().end(); ++i)
    {
      increment(result, i->out.size());
    }
    return result;
  }

  degree_distribution_t in_degrees() const
  {
    // map out degree to number of vertices with this outdegree
    degree_distribution_t result;

    for(typename graph_t::vertices_t::const_iterator i = m_graph.vertices().begin(); i != m_graph.vertices().end(); ++i)
    {
      increment(result, i->in.size());
    }
    return result;
  }

protected:
  void yaml_values(YAML::Emitter& out) const
  {
    out << YAML::Key << "Number of vertices"
        << YAML::Value << num_vertices()
        << YAML::Key << "Number of edges"
        << YAML::Value << num_edges()
        << YAML::Key << "Average degree"
        << YAML::Value << average_degree()
        << YAML::Key << "In-degrees"
        << YAML::Value << in_degrees()
        << YAML::Key << "Out-degrees"
        << YAML::Value << out_degrees();
  }

public:

  void yaml(YAML::Emitter& out)
  {
    out << YAML::BeginMap;
    yaml_values(out);
    out << YAML::EndMap;
  }

};

namespace pg
{

class pg_info : graph_info<graph::KripkeStructure<graph::Vertex<graph::pg::Label> > >
{
protected:
  typedef graph::KripkeStructure<graph::Vertex<graph::pg::Label> > graph_t;
  typedef graph::Vertex<graph::pg::DivLabel> vertex_t;
  typedef graph_info<graph_t> super;

  using super::m_graph;

  std::map<Player, size_t> m_nodes_per_player;
  std::map<Priority, size_t> m_nodes_per_priority;

  void populate_vertex_statistics()
  {
    m_nodes_per_player[even] = 0;
    m_nodes_per_player[odd] = 0;
    for(VertexIndex i = 0; i < m_graph.size() ; ++i)
    {
      increment(m_nodes_per_player, m_graph.vertex(i).label.player);
      increment(m_nodes_per_priority, m_graph.vertex(i).label.prio);
    }
  }

public:
  pg_info(const graph_t& g)
    : super(g)
  {
    populate_vertex_statistics();
  }

  size_t num_even_vertices() const
  {
    return m_nodes_per_player.find(even)->second;
  }

  size_t num_odd_vertices() const
  {
    return m_nodes_per_player.find(odd)->second;
  }

protected:
  void yaml_values(YAML::Emitter &out) const
  {
    super::yaml_values(out);
    out << YAML::Key << "Vertex count per player"
        << YAML::Value
        << YAML::BeginMap
        << YAML::Key << "Even"
        << YAML::Value << num_even_vertices()
        << YAML::Key << "Odd"
        << YAML::Value << num_odd_vertices()
        << YAML::EndMap
        << YAML::Key << "Vertex count per priority"
        << YAML::Value << m_nodes_per_priority;
  }

public:
  void yaml(YAML::Emitter& out) const
  {
    out << YAML::BeginMap;
    yaml_values(out);
    out << YAML::EndMap;
  }


};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const graph_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const pg_info& info)
{
  info.yaml(out);
  return out;
}

} // namespace pg

} // namespace graph

#endif // GRAPH_STATISTICS_H
