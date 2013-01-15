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

#include "cpplogging/logger.h"
#include "pg.h"
#include "bfs.h"

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
diameter(const Graph& g)
{
  cpplog(cpplogging::verbose) << "Computing diameter" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type result = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::max(result, bfs_levels(g, *i) - 1);
  }
  return result;
}

#endif // DIAMETER_H
