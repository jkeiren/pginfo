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

#ifndef GIRTH_H
#define GIRTH_H

#include <limits>
#include <boost/graph/breadth_first_search.hpp>

namespace detail
{
template<typename T>
void update_girth(size_t& /*girth*/, ptrdiff_t /*x*/)
{
  // skip
}

template<>
void update_girth<boost::on_black_target>(size_t& girth, ptrdiff_t x)
{
  assert(x >= 1);
  girth = std::min(girth, static_cast<size_t>(x));
}

template<typename DistanceMap, class Tag>
struct girth_recorder: public boost::default_bfs_visitor
{
  const DistanceMap& d;
  size_t& girth;

  girth_recorder(const DistanceMap& d_, size_t& girth_)
    : d(d_), girth(girth_)
  {}

  typedef Tag event_filter;

  template<typename Edge, typename Graph>
  void operator()(Edge e, Graph& g)
  {
    typename Graph::vertex_descriptor u = boost::source(e,g), v = boost::target(e,g);

    if(u == v)
      girth = 1;
    else
      update_girth<Tag>(girth, d[u] - d[v] + 1);
  }
};

template<typename DistanceMap, class Tag>
girth_recorder<DistanceMap, Tag>
record_girth(const DistanceMap& d, size_t& girth, Tag)
{
  return girth_recorder<DistanceMap, Tag>(d, girth);
}

}

template <typename Graph>
size_t girth(const Graph& g)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;

  size_t result = std::numeric_limits<size_t>::max();

  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    std::vector<vertex_size_t> d(num_vertices(g), 0);
    boost::breadth_first_search(g, *i,
      boost::visitor(boost::make_bfs_visitor(std::make_pair(
        boost::record_distances(&d[0], boost::on_tree_edge()), std::make_pair(
        detail::record_girth(d, result, boost::on_black_target()),
        detail::record_girth(d, result, boost::on_gray_target()))))
      ));

  }
  return result;
}

#endif // GIRTH_H
