#ifndef ALTERNATION_DEPTH_H
#define ALTERNATION_DEPTH_H

//#include "bfs_info.h"
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <yaml-cpp/emitter.h>

#include "pg.h"
#include "utilities.h"

namespace graph
{

template <typename T>
struct cmp_second
{
  bool operator()(const typename T::value_type& t1, const typename T::value_type& t2)
  {
    return t1.second < t2.second;
  }
};

template <typename graph_t>
class alternation_depth// : bfs_algorithm<graph_t>
{
protected:
  //typedef bfs_algorithm<graph_t> super;
  //typedef VertexIndex VertexIndex;
  typedef typename graph_t::vertex_t vertex_t;
  graph_t m_graph;

  typedef enum
  {
    white,
    grey,
    black
  } color_t;

  std::vector<color_t> m_color;
  std::vector<size_t> m_depth;
  std::map<VertexIndex, size_t> m_alternation_depth;

  void visit(VertexIndex i)
  {
    m_color[i] = grey;
    m_depth = std::vector<size_t>(m_graph.size(), 0);

    vertex_t u = m_graph.vertex(i);
    size_t result = 0;

    for(std::set<VertexIndex>::const_iterator j = u.out.begin(); j != u.out.end(); ++j)
    {
      vertex_t v = m_graph.vertex(*j);
      if(v.label.prio <= u.label.prio)
      {
        if(m_color[*j] == grey)
        {
          // loop on same level
          continue;
        }


        // fresh
        if(m_color[*j] == white)
        {
          visit(*j);
        }
        size_t res = m_depth[*j];

        // Compute result, if there is an alternation
        if(v.label.prio % 2 != u.label.prio % 2)
        {
          ++res;
        }
        m_depth[i] = (std::max)(result, res);
      }
    }
    m_color[i] = black;
  }

  size_t compute_alternation_depth(VertexIndex i)
  {
    m_color = std::vector<color_t>(m_graph.size(), white);
    visit(i);
    return m_depth[i];
  }

  void compute_alternation_depth()
  {
    for(VertexIndex i = 0; i < m_graph.size(); ++i)
    {
      m_alternation_depth[i] = compute_alternation_depth(i);
    }
  }


public:
  alternation_depth(const graph_t& g):m_graph(g)
  {
    compute_alternation_depth();
  }

  std::size_t get_alternation_depth() const
  {
    std::map<VertexIndex, size_t>::const_iterator m = std::max_element(m_alternation_depth.begin(), m_alternation_depth.end(), cmp_second<std::map<VertexIndex, size_t> >() );
    return m->second;
  }

  void yaml(YAML::Emitter& out) const
  {
    out << YAML::BeginMap
        << YAML::Key << "Max"
        << YAML::Value << get_alternation_depth()
        << YAML::Key << "Per node"
        << YAML::Value << m_alternation_depth
        << YAML::EndMap;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const alternation_depth<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // ALTERNATION_DEPTH_H
