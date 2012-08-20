#ifndef DFS_INFO_H
#define DFS_INFO_H

#include <stack>
#include "graph.h"

namespace graph
{

template <typename graph_t>
class dfs_algorithm
{

protected:
  typedef graph::VertexIndex VertexIndex;

  const graph_t& m_graph;
  std::stack<VertexIndex> m_stack;
  std::vector<size_t> m_visited;//(m_graph.size(), 0);

  virtual
  void push(const VertexIndex v)
  {
    m_stack.push(v);
  }

  virtual
  VertexIndex pop()
  {
    VertexIndex ret = m_stack.top();
    m_stack.pop();
    return ret;
  }

  virtual
  void old_node_seen(const VertexIndex current_node, const VertexIndex old_node)
  {}

  virtual
  void new_node_seen(const VertexIndex current_node, const VertexIndex new_node)
  {}

  virtual
  void start_iteration(const size_t n)
  {}

public:
  dfs_algorithm(const graph_t& g)
    : m_graph(g),
      m_visited(m_graph.size(), std::numeric_limits<VertexIndex>::max())
  {}

  void dfs(const VertexIndex start_node = 0)
  {
    size_t visited = 0;

    push(start_node);

    while(!m_stack.empty())
    {
      start_iteration(++visited);

      // dfs
      VertexIndex vi = pop();
      m_visited[vi] = visited;

      typename graph_t::vertex_t v = m_graph.vertex(vi);
      for(std::set<VertexIndex>::const_iterator i = v.out.begin(); i != v.out.end(); ++i)
      {
        if(m_visited[*i] == std::numeric_limits<VertexIndex>::max())
        {
          push(*i);
          new_node_seen(vi, *i);
        }
        else
        {
          old_node_seen(vi, *i);
        }
      }
    }
  }

};

template <typename graph_t>
class dfs_info : dfs_algorithm<graph_t>
{
protected:
  typedef dfs_algorithm<graph_t> super;
  typedef typename super::VertexIndex VertexIndex;

  bool m_details;
  size_t m_max_stack_size;
  std::map<size_t, size_t> m_stack_size_per_visited_node;
  using super::m_stack;

  virtual void start_iteration(const size_t visited)
  {
    m_stack_size_per_visited_node[visited] = m_stack.size();
    m_max_stack_size = (std::max)(m_max_stack_size, m_stack.size());
  }

public:
  dfs_info(graph_t& g, bool details = false)
    : super(g),
      m_details(details),
      m_max_stack_size(0)
  {
    super::dfs();
  }

  size_t maximum_stack_size() const
  {
    return m_max_stack_size;
  }

  void yaml(YAML::Emitter& out) const
  {
    out << YAML::BeginMap
        << YAML::Key << "Maximum stack size"
        << YAML::Value << maximum_stack_size();
    if(m_details)
    {
      out << YAML::Key << "Stack size per visited node"
          << YAML::Value << m_stack_size_per_visited_node;
    }
    out << YAML::EndMap;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const dfs_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // DFS_INFO_H
