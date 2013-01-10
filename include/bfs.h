#ifndef BFS_INFO_H
#define BFS_INFO_H

#include <boost/graph/breadth_first_search.hpp>
#include "utilities.h"

namespace detail
{

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

template<typename DistanceMap>
inline
std::map<size_t, size_t> bfs_nodes_per_level(const DistanceMap& d)
{
  std::map<size_t, size_t> l;
  for(auto i: d)
    increment(l, i);
  return l;
}

template<typename Graph, typename DistanceMap>
inline
typename boost::graph_traits<Graph>::vertices_size_type
bfs_levels(const Graph& g, typename Graph::vertex_descriptor v, DistanceMap& d)
{
  assert(d.size() == num_vertices(g));
  boost::breadth_first_search(g, v,
          boost::visitor(boost::make_bfs_visitor(
            boost::record_distances(&d[0], boost::on_tree_edge()))
      ));
  return *std::max_element(d.begin(), d.end()) + 1;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
bfs_levels(const Graph& g, typename Graph::vertex_descriptor v = 0)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> d(boost::num_vertices(g), 0);
  return bfs_levels(g, v, d);
}

template<typename Graph, typename BackEdgeMap>
inline
typename boost::graph_traits<Graph>::vertices_size_type
back_level_edges(const Graph& g, typename Graph::vertex_descriptor v, BackEdgeMap& m)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  vertex_size_t n = 0;
  std::vector<vertex_size_t> d(boost::num_vertices(g));
  boost::breadth_first_search(g, v,
                              boost::visitor(boost::make_bfs_visitor(std::make_pair(
                                boost::record_distances(&d[0], boost::on_tree_edge()),
                                detail::record_back_edges(n, m, &d[0]))
                          ))
      );
  return n;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
back_level_edges(const Graph& g, typename Graph::vertex_descriptor v = 0)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  std::map<vertex_size_t, vertex_size_t> m;
  return back_level_edges(g, v, m);
}

#endif // BFS_INFO_H
