#ifndef BFS_INFO_H
#define BFS_INFO_H

#include <deque>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/stlemitter.h>

#include "cpplogging/logger.h"
#include "utilities.h"

namespace graph
{

using cpplogging::verbose;
using cpplogging::status;
using cpplogging::debug;

template <typename graph_t>
class bfs_info;

template <typename graph_t>
class bfs_algorithm
{
protected:
  typedef graph::VertexIndex VertexIndex;
  typedef typename graph_t::vertex_t vertex_t;

  typedef enum
  {
    white,
    grey,
    black
  } color_t;

  const graph_t& m_graph;
  std::deque<VertexIndex> m_queue;
  std::vector<size_t> m_level; // The level at which the node is visited.
  std::vector<VertexIndex> m_parent;
  std::vector<color_t> m_color; // Color

  virtual
  void initialise()
  {
    m_queue.clear();
    m_level = std::vector<size_t>(m_graph.size(), std::numeric_limits<size_t>::max()), // The level at which the node is visited.
    m_parent = std::vector<size_t>(m_graph.size(), std::numeric_limits<VertexIndex>::max()),
    m_color = std::vector<color_t>(m_graph.size(), white);
  }

  virtual
  void push(const VertexIndex v)
  {
    m_queue.push_back(v);
  }

  virtual
  VertexIndex pop()
  {
    VertexIndex ret = m_queue.front();
    m_queue.pop_front();
    return ret;
  }

  virtual
  void old_node_seen(const VertexIndex current_node, const VertexIndex old_node)
  {}

  virtual
  void new_node_seen(const VertexIndex current_node, const VertexIndex new_node)
  {}

  virtual
  void start_level(const size_t level, const size_t num_vertices)
  {}

  virtual
  void start_iteration(const size_t n)
  {}

public:

  void bfs(size_t start_node = 0)
  {
    log(verbose, "BFS") << "Starting search at vertex " << start_node << std::endl;

    initialise();

    m_color[start_node] = grey;
    m_level[start_node] = 0;
    push(start_node);

    size_t visited = 0;
    size_t todo_this_level = 1;
    size_t todo_next_level = 0;
    start_level(0, 1);

    while(!m_queue.empty())
    {
      start_iteration(++visited);

      VertexIndex ui = pop();
      --todo_this_level;
      vertex_t u = m_graph.vertex(ui);

      for(std::set<VertexIndex>::const_iterator i = u.out.begin(); i != u.out.end(); ++i)
      {
        if(m_color[*i] == white)
        {
          // never seen this node before
          m_color[*i] = grey;
          m_level[*i] = m_level[ui] + 1;
          m_parent[*i] = ui;
          push(*i);
          ++todo_next_level;
          new_node_seen(ui, *i);
        }
        else // color is grey or black
        {
          old_node_seen(ui, *i);
        }
      }

      m_color[ui] = black;

      if(todo_this_level == 0)
      {
        start_level(m_level[ui] + 1, todo_next_level);
        todo_this_level = todo_next_level;
        todo_next_level = 0;
      }
    }
  }

  bfs_algorithm(const graph_t& g)
    : m_graph(g)
  {}
};

template <typename graph_t>
class bfs_info : public bfs_algorithm<graph_t>
{
protected:

  typedef bfs_algorithm<graph_t> super;
  typedef typename super::VertexIndex VertexIndex;
  typedef typename super::vertex_t vertex_t;

  using super::m_level;
  using super::m_queue;

  bool m_details;
  size_t m_back_level_edges;
  std::map<size_t, size_t> m_back_level_edges_per_length;

  virtual
  void old_node_seen(const VertexIndex current_node, const VertexIndex old_node)
  {
    // Record back level edge
    // note: take care of nodes on the next level with multiple edges...
    if(m_level[current_node] >= m_level[old_node])
    {
      size_t length = m_level[current_node] - m_level[old_node];
      ++m_back_level_edges;
      increment(m_back_level_edges_per_length, length);
    }
  }

  virtual
  void new_node_seen()
  {}

  std::map<size_t, size_t> m_nodes_per_level;
  size_t m_levels;

  virtual
  void start_level(const size_t level, const size_t num_vertices)
  {
    if(num_vertices != 0) // if it is 0, we are done! Do not record empty level at end of execution
    {
      m_levels = level;
      m_nodes_per_level[level] = num_vertices;
    }
  }

  size_t m_max_queue_size;
  std::map<size_t, size_t> m_queue_size_per_visited_node;

  virtual
  void start_iteration(const size_t n)
  {
    m_max_queue_size = (std::max)(m_max_queue_size, m_queue.size());
    m_queue_size_per_visited_node[n] = m_queue.size();
  }

public:
  bfs_info(graph_t& g, bool details=false)
    : super(g),
      m_details(details),
      m_levels(0),
      m_back_level_edges(0),
      m_max_queue_size(0)
  {
    super::bfs();
  }

  size_t get_levels() const
  {
    return m_levels;
  }

  size_t nodes_on_level(size_t i) const
  {
    return *(m_nodes_per_level.find(i));
  }

  size_t back_level_edges() const
  {
    return m_back_level_edges;
  }

  size_t back_level_edges_of_length(size_t i) const
  {
    return *(m_back_level_edges_per_length.find(i));
  }

  void yaml(YAML::Emitter& out) const
  {
    out << YAML::BeginMap
        << YAML::Key << "Number of levels (BFS height)"
        << YAML::Value << m_levels
        << YAML::Key << "Nodes per level"
        << YAML::Value << m_nodes_per_level
        << YAML::Key << "Number of back level edges"
        << YAML::Value << m_back_level_edges
        << YAML::Key << "Lengths of back level edges"
        << YAML::Value << m_back_level_edges_per_length;
    if(m_details)
    {
      out << YAML::Key << "Queue size per visited node"
          << YAML::Value << m_queue_size_per_visited_node;
    }
    out << YAML::EndMap;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const bfs_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // BFS_INFO_H
