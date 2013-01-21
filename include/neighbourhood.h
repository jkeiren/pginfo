// Author(s): Jeroen Keiren
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file /path/to/file.ext
/// \brief Description comes here

#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include <algorithm>
#include <limits>
#include "cpplogging/logger.h"

namespace detail
{

/**
 * \brief Exception class for errors raised by the command-line parser.
 **/
class bound_reached_exception : public std::exception
{
public:
  bound_reached_exception() throw()
    : std::exception()
  {}
  virtual const char* what() const throw()
  {
    return "Bound reached";
  }
  virtual ~bound_reached_exception() throw()
  {}
};

template <class DistanceMap, class Tag>
struct bounded_distance_recorder
  : public boost::base_visitor<bounded_distance_recorder<DistanceMap, Tag> >
{
  typedef Tag event_filter;
  DistanceMap m_distance;
  const size_t m_bound;

  bounded_distance_recorder(DistanceMap pa, const size_t bound)
    : m_distance(pa), m_bound(bound) { }

  template <class Edge, class Graph>
  void operator()(Edge e, const Graph& g) {
    typename boost::graph_traits<Graph>::vertex_descriptor
      u = source(e, g), v = target(e, g);
    if(boost::get(m_distance, u) + 1 > m_bound)
      throw bound_reached_exception();
    boost::put(m_distance, v, boost::get(m_distance, u) + 1);
  }
};

template <class DistanceMap, class Tag>
bounded_distance_recorder<DistanceMap, Tag>
bounded_record_distances(DistanceMap pa, const size_t bound, Tag) {
  return bounded_distance_recorder<DistanceMap, Tag> (pa, bound);
}

} // namespace detail

template<typename Graph>
inline
std::vector<typename boost::graph_traits<Graph>::vertices_size_type>
upto_kneighbourhood(typename Graph::vertex_descriptor v, const Graph& g, const size_t k)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> result(k+1,0);
  if(k == 0) return result;
  std::vector<vertex_size_t> d(boost::num_vertices(g), std::numeric_limits<vertex_size_t>::max());
  d[v] = 0;
  try
  {
    boost::breadth_first_search(g, v,
            boost::visitor(boost::make_bfs_visitor(detail::bounded_record_distances(&d[0], k, boost::on_tree_edge()))
        ));
  }
  catch (detail::bound_reached_exception&)
  {};
  for(size_t i = 0; i <= k; ++i)
  {
    result[i] = std::count_if(d.begin(), d.end(), [&i](vertex_size_t x) { return 0 < x && x <= i; });
  }
return result;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
kneighbourhood(typename Graph::vertex_descriptor v, const Graph& g, const size_t k)
{
  return upto_kneighbourhood(v,g,k)[k];
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
neighbourhood(typename Graph::vertex_descriptor v, const Graph& g)
{
  return upto_kneighbourhood(v, g, 1)[1];
}

struct neighbourhood_result
{
  size_t min;
  size_t max;
  size_t sum;

  neighbourhood_result()
    : min(std::numeric_limits<size_t>::max()), max(0), sum(0)
  {}
};

template<typename Graph>
inline
std::vector<neighbourhood_result>
accumulated_upto_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing accumulated neighbourhood information" << std::endl;
  std::vector<neighbourhood_result> result(k+1, neighbourhood_result());
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    if((*i % 1000) == 0)
    {
      cpplog(cpplogging::status) << "Processed " << *i << " vertices" << std::endl;
    }
    std::vector<typename boost::graph_traits<Graph>::vertices_size_type> tmp = upto_kneighbourhood(*i, g, k);
    for(size_t i = 0; i <= k; ++i)
    {
      result[i].sum += tmp[i];
      result[i].min = std::min(result[i].min, tmp[i]);
      result[i].max = std::max(result[i].max, tmp[i]);
    }
  }
  cpplog(cpplogging::verbose) << "done" << std::endl;
  return result;
}


template<typename Graph>
inline
double avg_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing average " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type sum = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    sum += kneighbourhood(*i, g, k);
  }
  return static_cast<double>(sum)/static_cast<double>(boost::num_vertices(g));
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
max_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing maximum " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type result = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::max(result, kneighbourhood(*i, g, k));
  }
  return result;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
min_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing minimum " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type result = std::numeric_limits<size_t>::max();
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::min(result, kneighbourhood(*i, g, k));
  }
  return result;
}

#endif // NEIGHBOURHOOD_H
