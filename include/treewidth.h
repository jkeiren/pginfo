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
#include <boost/heap/fibonacci_heap.hpp>
#include "cpplogging/logger.h"

#include "graph_utilities.h"

/* According to Obdrzalek in ... 2006, p.40
 * "There is an easy way to generalise the concept of tree-width to directed
 *  graphs: For a directed graph G we put tw(G) = tw(G') where G' is obtained
 *  from G by forgetting the orientation of the edges (i.e. an edge {u,v} of G'
 *  can correspond to two edges (u,v) and (v,u) of G. We can therefore
 *  freely talk about tree-width and tree decompositions of directed graphs."
 */

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
/* \brief Eliminate a vertex v.
 *
 * v is eliminated by adding edges u -- w for all u,w s.t. u,w -- v, u != w.
 * Priority pq is updated accordingly.
 *
 * \pre v \not \in pq.
 * \pre v -- v \not \in edges(g).
 */
template <typename UndirectedGraph, class PriorityQueue, class HandleMap>
inline
void eliminate_vertex(typename boost::graph_traits<UndirectedGraph>::vertex_descriptor v,
                   UndirectedGraph& g,
                   PriorityQueue& pq,
                   HandleMap& handle)
{
  typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;
  typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aj, aend;
  typedef typename PriorityQueue::handle_type handle_t;

  boost::tie(ai, aend) = boost::adjacent_vertices(v, g);
  while(ai != aend)
  {
    vertex_t u = *ai;
    for (aj = ai; aj != aend; ++aj)
    {
      vertex_t w = *aj;
      if(u != v && u != w && v != w)
      {
        boost::add_edge(v, w, g);
        pq.decrease(handle[v]);
        pq.decrease(handle[w]);
      }
    }
    ai++;
    boost::remove_edge(v, u, g);
    // no pq.increase(handle[v]) since v is not in pq.
    pq.increase(handle[u]);
  }
}

template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
greedy_degree_destructive(UndirectedGraph& g)
{
  typedef typename boost::graph_traits<UndirectedGraph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;

  // We do not count selfloops, first remove so we can use out_degree to
  // determine the neighbourhood.
  remove_selfloops(g);

  // Build priority queue, sorted by outdegree, and record the handles.
  typedef outdegree_greater<UndirectedGraph> cmp_t;
  typedef boost::heap::fibonacci_heap<vertex_t, boost::heap::compare<cmp_t> > priority_queue_t;
  typedef typename priority_queue_t::handle_type handle_t;
  cmp_t cmp(g);
  priority_queue_t pq(cmp);
  std::vector<handle_t> handles;

  typename boost::graph_traits<UndirectedGraph>::vertex_iterator i, end;
  for(boost::tie(i,end) = boost::vertices(g); i != end; ++i)
  {
    handles.push_back(pq.push(*i));
  }

  // Compute the upperbound according to the greedy degree algorithm.
  vertex_size_t upperbound = std::numeric_limits<vertex_size_t>::min();

  while(!pq.empty())
  {
    vertex_t u = pq.top();
    pq.pop();
    upperbound = std::max(upperbound, boost::out_degree(u, g));
    eliminate_vertex(u, g, pq, handles);
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

/* \brief Contract the edge u -- v.
 *
 * The vertex in {u,v} with least outdegree is removed from g. Suppose this is u,
 * then all edges incident to u, that do not introduce a selfloop when moved to
 * v are added to v, and the vertex u is removed from g.
 */
template <typename UndirectedGraph, class PriorityQueue, class HandleMap>
inline
void contract_edge(typename boost::graph_traits<UndirectedGraph>::vertex_descriptor u,
                   typename boost::graph_traits<UndirectedGraph>::vertex_descriptor v,
                   UndirectedGraph& g,
                   PriorityQueue& pq,
                   HandleMap& handle)
{
  // ensure u is the vertex with largest number of neighbours
  cpplog(cpplogging::debug, "contract_edge") << "u = " << u << ", v = " << v << " out-degrees " << boost::out_degree(u,g) << ", " << boost::out_degree(v,g) << std::endl;
  if(boost::out_degree(u, g) < boost::out_degree(v, g))
  {
    std::swap(u,v);
    cpplog(cpplogging::debug, "contract_edge") << "swapping such that u = " << u << ", v = " << v << " out-degrees " << boost::out_degree(u,g) << ", " << boost::out_degree(v,g) << std::endl;
  }

  cpplog(cpplogging::debug, "contract_edge") << "removing " << u << "--" << v << std::endl;
  boost::remove_edge(u, v, g);
  pq.increase(handle[u]);
  pq.increase(handle[v]);

  // add all edges of v to u
  // need while loop here to prevent invalidation of ai iterator before
  // incrementing.
  typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aend;
  boost::tie(ai, aend) = boost::adjacent_vertices(v, g);
  while(ai != aend)
  {
    typename boost::graph_traits<UndirectedGraph>::vertex_descriptor w = *(ai++);
    assert(w != v);
    if(w != u)
    {
      cpplog(cpplogging::debug, "contract_edge") << "adding " << u << "--" << *ai << std::endl;
      boost::add_edge(u, w, g);
      pq.decrease(handle[u]);
      pq.decrease(handle[w]);
    }
    cpplog(cpplogging::debug, "contract_edge") << "removing " << v << "--" << *ai << std::endl;
    boost::remove_edge(v, w, g);
    pq.increase(handle[w]);
    pq.increase(handle[v]);
  }

  assert(boost::out_degree(v,g) == 0);
  assert(boost::num_edges(g) == 0 || boost::out_degree(u,g) > 0);
  assert(boost::num_edges(g) == 0 || pq.top() == v);
  cpplog(cpplogging::debug) << "Removing " << v << " from the queue" << std::endl;
  pq.pop();
}

/* \brief Determine lowerbound on treewidth through Minor-Min-Width algorithm.
 *
 * Pseudo code:
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
template <typename UndirectedGraph>
inline
typename boost::graph_traits<UndirectedGraph>::vertices_size_type
minor_min_width_destructive(UndirectedGraph& g)
{
  typedef typename boost::graph_traits<UndirectedGraph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;

  cpplog(cpplogging::debug) << "Computing Minor-Min-Width" << std::endl;
  cpplog(cpplogging::debug) << "Removing self loops" << std::endl;

  remove_selfloops(g);

  // build priority queue and record handles
  typedef outdegree_greater<UndirectedGraph> cmp_t;
  typedef boost::heap::fibonacci_heap<vertex_t, boost::heap::compare<cmp_t> > priority_queue_t;
  typedef typename priority_queue_t::handle_type handle_t;
  cmp_t cmp(g);
  priority_queue_t pq(cmp);
  std::vector<handle_t> handles;

  typename boost::graph_traits<UndirectedGraph>::vertex_iterator i, end;
  for(boost::tie(i,end) = boost::vertices(g); i != end; ++i)
  {
    handles.push_back(pq.push(*i));
  }

  // Determine lowerbound according to minor min-width strategy
  cpplog(cpplogging::debug) << "init: lowerbound = 0" << std::endl;
  vertex_size_t lowerbound = 0;

  while(!pq.empty())
  {
    if(boost::num_edges(g) == 0)
      return lowerbound;

    vertex_t u = pq.top();
    vertex_size_t degree_u = boost::out_degree(u, g);
    assert(degree_u > 0);
    cpplog(cpplogging::debug) << "top element of queue: " << u << " with out-degree " << degree_u << std::endl;

    lowerbound = std::max(degree_u, lowerbound);
    cpplog(cpplogging::debug) << "lowerbound = " << lowerbound << std::endl;

    vertex_t v;
    vertex_size_t degree_v = std::numeric_limits<vertex_size_t>::max();
    typename boost::graph_traits<UndirectedGraph>::adjacency_iterator ai, aend;
    for (boost::tie(ai, aend) = boost::adjacent_vertices(u, g); ai != aend; ++ai)
    {
      vertex_size_t n = boost::out_degree(*ai, g);
      if(n < degree_v && n > 0)
      {
        degree_v = n;
        v = *ai;
      }
    }
    cpplog(cpplogging::debug) << "neighbour with minimal outdegree: " << v << " with out-degree " << degree_v << std::endl;
    contract_edge(v, u, g, pq, handles);
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
