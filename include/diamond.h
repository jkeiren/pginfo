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

#ifndef DIAMOND_H
#define DIAMOND_H

#include "cpplogging/logger.h"

struct diamond_count_t
{
  size_t all;
  size_t even;
  size_t odd;

  diamond_count_t()
    : all(0), even(0), odd(0)
  {}
};

// Count the number of 2-diamonds in the graph.
template <typename Graph>
diamond_count_t diamond_count(const Graph& g)
{
  cpplog(cpplogging::verbose) << "Counting number of 2-diamonds in the graph" << std::endl;
  diamond_count_t result;

  typename boost::graph_traits< Graph >::vertex_iterator i, end;
  typename boost::graph_traits< Graph >::adjacency_iterator ai,aj,aend;

  for (boost::tie(i, end) = boost::vertices(g); i != end; ++i)
  {
    typename Graph::vertex_descriptor u = *i;
    cpplog(cpplogging::debug) << "Vertex u = " << u << std::endl;

    boost::tie(ai, aend) = boost::adjacent_vertices(u, g);
    if (ai == aend)
    {
      cpplog(cpplogging::debug) << "Vertex " << u << " has no successors" << std::endl;
      continue;
    }

    for(; ai != aend; ++ai)
    {
      {
        typename boost::graph_traits< Graph >::adjacency_iterator ai_ = ai;
        if(++ai_ == aend)
        {
          continue; // workaround to loop to aend - 1
        }
      }
      typename Graph::vertex_descriptor v = *ai;

      cpplog(cpplogging::debug) << "  Vertex v = " << v << std::endl;

      aj = ai;
      ++aj;
      if(aj == aend)
      {
        cpplog(cpplogging::debug) << "    w = v; skipping" << std::endl;
      }

      for(; aj != aend; ++aj)
      {
        typename Graph::vertex_descriptor w = *aj;
        cpplog(cpplogging::debug) << "      Checking diamond property for " << u << " with " << v << " and " << w << std::endl;

        typename boost::graph_traits< Graph >::adjacency_iterator vi, wi, vend, wend;
        boost::tie(vi, vend) = boost::adjacent_vertices(v, g);
        boost::tie(wi, wend) = boost::adjacent_vertices(w, g);
        while(vi != vend && wi != wend)
        {
          if(boost::vertex(*vi,g) < boost::vertex(*wi,g))
            ++wi;
          else if(boost::vertex(*wi, g) < boost::vertex(*vi, g))
            ++vi;
          else
          {
            cpplog(cpplogging::debug) << "      Diamond completed by " << *wi << std::endl;
            ++result.all;
            if(g[u].player == even && g[v].player == even && g[w].player == even)
            {
              ++result.even;
            }
            else if(g[u].player == odd && g[v].player == odd && g[w].player == odd)
            {
              ++result.odd;
            }
            ++vi; ++wi;
          }
        }
      }
      cpplog(cpplogging::debug) << "      " << result.all << " diamonds after this iteration" << std::endl;
    }

  }
  return result;
}

#endif // DIAMOND_H
