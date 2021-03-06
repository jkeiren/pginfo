// Author(s): Jeroen Keiren
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file alternation_depth.h
/// \brief Implementation of alternation depth as described in
///        R. Cleaveland, M. Klein and B. Steffen, "Faster Model Checking
///        for the Modal Mu-Calculus", Proc. CAV 1993.
///
/// Note that the original definition of alternation depth was for modal
/// equation systems. We here translated the definition to parity games,
/// assuming that the relative order between vertices in the game is the same
/// as the relative order between equations in the underlying equation system.

#ifndef ALTERNATION_DEPTH_H
#define ALTERNATION_DEPTH_H

#include <boost/graph/strong_components.hpp>
#include <boost/graph/topological_sort.hpp>
#include "cpplogging/logger.h"

template<typename ComponentMap, typename NestingMap>
inline
void
nesting_depth(const parity_game_t& g, ComponentMap cm, NestingMap nm)
{
  cpplog(cpplogging::debug) << "Computing nesting depth" << std::endl;
  typename boost::graph_traits<parity_game_t>::vertex_iterator i,end;
  for(boost::tie(i, end) = boost::vertices(g); i != end; ++i)
  {
    nm[*i] = 1;
    typename boost::graph_traits<parity_game_t>::in_edge_iterator ai,aend;
    for(boost::tie(ai, aend) = boost::in_edges(*i,g); ai != aend; ++ai)
    {
      if(cm[boost::source(*ai,g)] != cm[*i] || boost::source(*ai,g) >= *i)
        continue;

      if((g[*i].prio % 2) == (g[boost::source(*ai,g)].prio % 2))
        nm[*i] = std::max(nm[*i], nm[boost::source(*ai,g)]);
      else
        nm[*i] = std::max(nm[*i], 1+nm[boost::source(*ai,g)]);
    }
  }
}

template<typename ComponentMap, typename NestingMap>
inline
void
nesting_depth_priority_sorting(const parity_game_t& g, ComponentMap cm, NestingMap nm)
{
  cpplog(cpplogging::debug) << "Computing nesting depth using priority sorting" << std::endl;
  typedef typename boost::graph_traits<parity_game_t>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<parity_game_t>::vertex_descriptor vertex_t;

  std::vector<vertex_t> order;
  order.reserve(boost::num_vertices(g));

  typename boost::graph_traits<parity_game_t>::vertex_iterator i,end;
  for(boost::tie(i, end) = boost::vertices(g); i != end; ++i)
    order.push_back(*i);
  assert(order.size() == boost::num_vertices(g));

  std::sort(order.begin(), order.end(), [&g](const vertex_t& x, const vertex_t& y){return g[boost::vertex(x, g)].prio < g[boost::vertex(y, g)].prio;});

  for(auto i: order)
  {
    nm[i] = 1;
    typename boost::graph_traits<parity_game_t>::in_edge_iterator ai,aend;
    for(boost::tie(ai, aend) = boost::in_edges(i,g); ai != aend; ++ai)
    {
      if(cm[boost::source(*ai,g)] != cm[i] || g[boost::source(*ai,g)].prio >= g[i].prio)
        continue;

      if((g[i].prio % 2) == (g[boost::source(*ai,g)].prio % 2))
        nm[i] = std::max(nm[i], nm[boost::source(*ai,g)]);
      else
        nm[i] = std::max(nm[i], 1+nm[boost::source(*ai,g)]);
    }
  }
}


inline
typename boost::graph_traits<parity_game_t>::vertices_size_type
alternation_depth(const parity_game_t& g)
{
  cpplog(cpplogging::verbose) << "Computing alternation depth" << std::endl;
  typedef typename boost::graph_traits<parity_game_t>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> components (boost::num_vertices(g), 0);
  boost::strong_components(g, &components[0]);

  std::vector<vertex_size_t> nesting_depths(boost::num_vertices(g), 1);
  nesting_depth(g, components, &nesting_depths[0]);

  return *std::max_element(nesting_depths.begin(), nesting_depths.end());
}

inline
typename boost::graph_traits<parity_game_t>::vertices_size_type
alternation_depth_priority_sorting(const parity_game_t& g)
{
  cpplog(cpplogging::verbose) << "Computing alternation depth with priority sorting" << std::endl;
  typedef typename boost::graph_traits<parity_game_t>::vertices_size_type vertex_size_t;
  std::vector<vertex_size_t> components (boost::num_vertices(g), 0);
  boost::strong_components(g, &components[0]);

  std::vector<vertex_size_t> nesting_depths(boost::num_vertices(g), 1);
  nesting_depth_priority_sorting(g, components, &nesting_depths[0]);

  return *std::max_element(nesting_depths.begin(), nesting_depths.end());
}

#endif // ALTERNATION_DEPTH_H
