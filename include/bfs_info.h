#ifndef BFS_INFO_H
#define BFS_INFO_H

#include <boost/graph/breadth_first_search.hpp>

template<typename key_t>
void increment(std::map<key_t, size_t>& m, const key_t& k)
{
  if(m.find(k) == m.end())
  {
    m[k] = 1;
  }
  else
  {
    ++m[k];
  }
}

namespace detail
{
template <typename T>
struct is_equal
{
  T x;
  is_equal(const T& x_)
    : x(x_)
  {}

  bool operator()(const T& other)
  {
    return other == x;
  }
};

template<typename BackEdgeMap, typename DistanceMap>
class back_edge_recorder: public boost::default_bfs_visitor
{
protected:
  size_t& m_back_edges;
  BackEdgeMap& m_back_level_edges_per_length;
  const DistanceMap& m_distance;

public:
  back_edge_recorder(size_t& c, BackEdgeMap& m, const DistanceMap& d)
    : m_back_edges(c), m_back_level_edges_per_length(m), m_distance(d)
  {}

  typedef boost::on_non_tree_edge event_filter;

  template<typename Edge, typename Graph>
  void operator()(Edge e, const Graph& g) {
    typename boost::graph_traits<Graph>::vertex_descriptor
      u = source(e, g), v = target(e, g);
    if(m_distance[u] >= m_distance[v])
    {
      size_t delta = m_distance[u] - m_distance[v];
      ++m_back_edges;
      increment(m_back_level_edges_per_length, delta);
    }
  }
};

template< typename BackEdgeMap, typename DistanceMap >
back_edge_recorder<BackEdgeMap, DistanceMap>
record_back_edges(size_t& n, BackEdgeMap& m, const DistanceMap& d)
{
  return back_edge_recorder<BackEdgeMap, DistanceMap>(n, m, d);
}

} // namespace detail

template<typename Graph>
class bfs_info
{
protected:
  const Graph& m_g;
  typedef typename Graph::vertex_descriptor vertex_t;
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;

  std::vector<vertex_size_t> m_distances;
  size_t m_back_edges;
  std::map<vertex_size_t, vertex_size_t> m_back_edge_map;
  std::map<size_t, size_t> m_nodes_per_level;

  void compute()
  {
    vertex_t v = *(boost::vertices(m_g).first);

    boost::breadth_first_search(m_g, v,
            boost::visitor(boost::make_bfs_visitor(std::make_pair(
              boost::record_distances(&m_distances[0], boost::on_tree_edge()),
              detail::record_back_edges(m_back_edges, m_back_edge_map, &m_distances[0]))
        ))
        );

    for(auto i: m_distances)
    {
      increment(m_nodes_per_level, i);
    }
  }

public:
  bfs_info(const Graph& g)
    : m_g(g),
      m_distances(num_vertices(g), 0),
      m_back_edges(0)
  {
    cpplog(cpplogging::debug) << "Computing BFS info" << std::endl;
    compute();
  }

  size_t get_levels() const
  {
    return *std::max_element(m_distances.begin(), m_distances.end()) + 1;
  }

  size_t nodes_on_level(const size_t i) const
  {
    std::map<size_t, size_t>::const_iterator it = m_nodes_per_level.find(i);
    if(it == m_nodes_per_level.end())
      return 0;
    else
      return it->second;
  }

  size_t back_level_edges() const
  {
    return m_back_edges;
  }

  size_t back_edges_of_length(const size_t i) const
  {
    std::map<size_t, size_t>::const_iterator it = m_back_edge_map.find(i);
    if(it == m_back_edge_map.end())
      return 0;
    else
      return it->second;
  }

  const std::map<size_t, size_t>& back_edge_map() const
  {
    return m_back_edge_map;
  }

  const std::map<size_t, size_t>& nodes_per_level() const
  {
    return m_nodes_per_level;
  }

};

#endif // BFS_INFO_H
