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

template<typename Graph>
typename boost::graph_traits<Graph>::vertices_size_type
kneighbourhood(typename Graph::vertex_descriptor v, const Graph& g, const size_t k)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> d(num_vertices(g));
  boost::breadth_first_search(g, v,
          boost::visitor(boost::make_bfs_visitor(boost::record_distances(&d[0], boost::on_tree_edge()))
      ));
  return std::count_if(d.begin(), d.end(), [&k](vertex_size_t x) { return 0 < x && x <= k; });
}

template<typename Graph>
typename boost::graph_traits<Graph>::vertices_size_type
neighbourhood(typename Graph::vertex_descriptor v, const Graph& g)
{
  return kneighbourhood(v, g, 1);
}

template<typename Graph>
double avg_kneighbourhood(Graph& g, const size_t k)
{
  typename boost::graph_traits<Graph>::vertices_size_type sum = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    sum += kneighbourhood(*i, g, k);
  }
  return static_cast<double>(sum)/static_cast<double>(boost::num_vertices(g));
}

template<typename Graph>
typename boost::graph_traits<Graph>::vertices_size_type
max_kneighbourhood(Graph& g, const size_t k)
{
  typename boost::graph_traits<Graph>::vertices_size_type result = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::max(result, kneighbourhood(*i, g, k));
  }
  return result;
}

template<typename Graph>
typename boost::graph_traits<Graph>::vertices_size_type
min_kneighbourhood(Graph& g, const size_t k)
{
  typename boost::graph_traits<Graph>::vertices_size_type result = std::numeric_limits<size_t>::max();
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::min(result, kneighbourhood(*i, g, k));
  }
  return result;
}

#endif // NEIGHBOURHOOD_H
