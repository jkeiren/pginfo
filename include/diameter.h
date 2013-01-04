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

#ifndef DIAMETER_H
#define DIAMETER_H

#include "pg.h"

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
bfs_depth(const Graph& g, typename Graph::vertex_descriptor v)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> d(num_vertices(g), 0);
  boost::breadth_first_search(g, v,
          boost::visitor(boost::make_bfs_visitor(
            boost::record_distances(&d[0], boost::on_tree_edge()))
      ));
  return *std::max_element(d.begin(), d.end());
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
diameter(const Graph& g)
{
  typename boost::graph_traits<Graph>::vertices_size_type result = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::max(result, bfs_depth(g, *i));
  }
  return result;
}

#endif // DIAMETER_H
