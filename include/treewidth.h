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

#ifndef TREEWIDTH_H
#define TREEWIDTH_H

#include <limits>
#include <boost/graph/copy.hpp>
#include "cpplogging/logger.h"

#include "neighbourhood.h"

/* According to Obdrzalek in ... 2006, p.40
 * "There is an easy way to generalise the concept of tree-width to directed
 *  graphs: For a directed graph G we put tw(G) = tw(G') where G' is obtained
 *  from G by forgetting the orientation of the edges (i.e. an edge {u,v} of G'
 *  can correspond to two edges (u,v) and (v,u) of G. We can therefore
 *  freely talk about tree-width and tree decompositions of directed graphs."
 */

template <typename Graph>
void remove_selfloops(Graph& g)
{
  // add all edges of v to u
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for(boost::tie(i, end) = boost::vertices(g); i != end; ++i)
  {
    if(boost::edge(*i, *i, g).second)
    {
      boost::remove_edge(*i, *i, g);
    }
  }
}


/* Known algorithms for computing upperbound on treewidth:
 * - LexBFS
 * - MaximumCardinalitySearch
 * - MaximumCardinalitySearchMinimal
 * as well as variations of the above started from each vertex.
 *
 * All of these algorithms return permutations that must be transformed
 * into a tree decomposition to get an upperbound on treewidth.
 *
 * - GreedyDegree --
 * - GreedyFillIn -- (TACO 2006 always at least as good as the others)
 */

namespace detail
{
template <typename UndirectedGraph>
inline
void eliminate_vertex(typename boost::graph_traits<UndirectedGraph>::vertex_descriptor v,
                   UndirectedGraph& g)
{
  // add all edges of v to u
  typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aj, aend;

  for (boost::tie(ai, aend) = boost::adjacent_vertices(v, g); ai != aend; ++ai)
  {
    for (aj = ai; aj != aend; ++aj)
    {
      if(/* *ai != v && *aj != v && */ *ai != *aj)
      {
        boost::add_edge(*ai, *aj, g); // adds *ai -> *aj and vice versa, due to undirectedness.
      }
    }
    // remove the edge from v to *ai (O(E/V) operation)
    boost::remove_edge(v, *ai, g);
  }

  /* We want to remove all outgoing edges from the vertex, and then remove
   * the vertex. Since especially removing the vertex is an inefficient operation,
   * we refrain from doing so, and we just assume a vertex with no outgoing
   * edges is irrelevant.
   */
  //boost::clear_vertex(v,g);
  //boost::remove_vertex(v,g);
}

/* TODO: We should use a priority queue here for efficiently keeping the
 * vertices with the least number of outgoing edges. Problem here is that the
 * algorithm requires updates to arbitrary vertices, meaning we have to resort
 * to an extra level of indirection if we want to prevent linear-time searches.
 * Could maybe be implemented using the Fibonacci heap in boost::heap.
 */
template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
greedy_degree_destructive(UndirectedGraph& g)
{
  typedef typename boost::graph_traits<UndirectedGraph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;

  remove_selfloops(g);

  vertex_size_t upperbound = std::numeric_limits<vertex_size_t>::min();

  while(boost::num_edges(g) > 0)
  {
    vertex_size_t min = boost::num_vertices(g);
    vertex_t u;
    typename boost::graph_traits<UndirectedGraph>::vertex_iterator i, end;
    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
      vertex_size_t n = boost::out_degree(*i, g);
      if(n < min && n > 0)
      {
        u = *i;
        min = n;
      }
    }

    upperbound = std::max(upperbound, min);
    eliminate_vertex(u, g);
  }
  return upperbound;
}
}

template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
greedy_degree(UndirectedGraph g)
{
  UndirectedGraph destructable_g;
  boost::copy_graph(g, destructable_g);
  return detail::greedy_degree_destructive(destructable_g);
}

/* Known algorithms for computing lowerbound on treewidth:
 * - MinDegree
 * - MinorMinWidth *  ---
 * - Ramachandramurthi
 * - MaximumMinimumDegree *
 * - MaximumMinimumDegreePlusMinD
 * - MaximumMinimumDegreePlusMaxD
 * - MaximumMinimumDegreePlusLeastC *
 * the algorithms marked with * can be started from each vertex.
 */

namespace detail
{


template <typename UndirectedGraph>
inline
void contract_edge(typename boost::graph_traits<UndirectedGraph>::vertex_descriptor u,
                   typename boost::graph_traits<UndirectedGraph>::vertex_descriptor v,
                   UndirectedGraph& g)
{
  // ensure u is the vertex with largest number of neighbours
  if(boost::out_degree(u, g) < boost::out_degree(v, g))
    std::swap(u,v);

  boost::remove_edge(u, v, g);

  // add all edges of v to u
  typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aend;
  for (boost::tie(ai, aend) = boost::adjacent_vertices(v, g); ai != aend; ++ai)
  {
    assert(*ai != v);
    if(*ai != u)
      boost::add_edge(u, *ai, g);
    boost::remove_edge(v, *ai, g);
  }

  // clear_vertex is buggy, and does not correctly remove all edges, hence
  // we manually remove the edges above.
  // boost::clear_vertex(v, g);

#ifndef NDEBUG
  typename boost::graph_traits<UndirectedGraph>::edge_iterator i, end;
  for (boost::tie(i, end) = boost::edges(g); i != end; ++i)
  {
    assert(boost::source(*i, g) != v);
    assert(boost::target(*i, g) != v);
  }
#endif

  boost::remove_vertex(v, g);
}

template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
minor_min_width_destructive(UndirectedGraph& g)
{
  /*
       * 1. lb=0;
       * 2. Repeat
       *   (a) Contract the edge between a minimum degree
       *       vertex v and u 2 N(v) such that the degree of
       *       u is minimum in N(v) to form a new graph G0.
       *   (b) lb = MAX(lb; degree_G(V))
       *   (c) Set G to G'
       * 3. Until no vertices remain in g.
       * 4. return lb
       */
  typedef typename boost::graph_traits<UndirectedGraph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;

  remove_selfloops(g);

  vertex_size_t lowerbound = 0;
  while(boost::num_edges(g) > 0)
  {
    // find vertex with minimum degree
    vertex_size_t min = std::numeric_limits<vertex_size_t>::max();
    typename boost::graph_traits<UndirectedGraph>::vertex_iterator i, end;
    vertex_t u;
    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
      vertex_size_t n = boost::out_degree(*i, g);
      if(n < min && n > 0)
      {
        u = *i;
        min = n;
      }
    }

    if(boost::num_edges(g) == 0)
      return lowerbound;

    lowerbound = std::max(min, lowerbound);

    // Find the neighbour with the lowest degree
    min = std::numeric_limits<vertex_size_t>::max();
    vertex_t v;
    typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aend;
    for (boost::tie(ai, aend) = boost::adjacent_vertices(u, g); ai != aend; ++ai)
    {
      vertex_size_t n = boost::out_degree(*ai, g);
      if(n < min && n > 0)
      {
        min = n;
        v = *ai;
      }
    }

    contract_edge(u, v, g);
  }

  return lowerbound;
}
} // namespace detail

template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
minor_min_width(const UndirectedGraph& g)
{
  UndirectedGraph destructable_g;
  boost::copy_graph(g, destructable_g);
  return detail::minor_min_width_destructive(destructable_g);
}

/* Exact algorithms known for computing treewidth:
 * - QuickBB
 * - TreewidthDP
 */


#endif // TREEWIDTH_H
