// Author(s): Jeroen Keiren
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file kellywidth.h
/// \brief Description comes here

#ifndef KELLYWIDTH_H
#define KELLYWIDTH_H

namespace detail
{
/* \brief Eliminate a vertex v.
 *
 * v is eliminated by adding edges u > w for all u,w s.t. u -> v -> w, u != w.
 * Priority pq is updated accordingly.
 *
 * \pre v \not \in pq.
 */
template <typename DirectedGraph, class PriorityQueue, class HandleMap>
inline
void eliminate_vertex_directed(typename boost::graph_traits<DirectedGraph>::vertex_descriptor v,
                   DirectedGraph& g,
                   PriorityQueue& pq,
                   HandleMap& handle)
{
  typedef typename boost::graph_traits<DirectedGraph>::vertex_descriptor vertex_t;
  typename boost::graph_traits<DirectedGraph>::in_edge_iterator ii, iend;
  typename boost::graph_traits<DirectedGraph>::out_edge_iterator oi, oend;

  boost::tie(ii, iend) = boost::in_edges(v, g);
  while(ii != iend)
  {
    vertex_t u = boost::source(*ii,g);
    for (boost::tie(oi, oend) = boost::out_edges(v, g); oi != oend; ++oi)
    {
      vertex_t w = boost::target(*oi,g);
      if(u != v && w != v && u != w)
      {
        boost::add_edge(u, w, g);
        pq.decrease(handle[u]);
      }
    }
    ii++;
    boost::remove_edge(u, v, g);
    // no pq.increase(handle[v]) since v is not in pq.
    if(u != v)
      pq.increase(handle[u]);
  }
  boost::clear_vertex(v, g);
  boost::tie(oi, oend) = boost::out_edges(v, g);
  assert(oi == oend);
}

/* \brief Compute the width of a directed elimination ordering.
 *
 * Kelly-width and the directed elimination ordering is described in
 * P. Hunter and S. Kreutzer. Digraph measures: Kelly decompositions, games
 * and orderings. In Theoretical Computer Science 399 (2008) pp. 206-219.
 *
 * - A directed elimination ordering < is a linear ordering on vertices(g)
 * - Given an elimination ordering < := (v_0, ..., v_{n-1}) of g, we define
 *   + g^<_0 = g
 *   + g^<_{i+1} is obtained from g^<_i by deleting v_i and adding new edges
 *     (if necessary) (u,v) if (u, v_i), (v_i, v) in edges(g^<_i) and u != v
 *   g^<_i is the directed elimination graph at step i.
 * - The width of an elimination ordering is the maximum over all i of the
 *   out-degree of v_i in G^<_i.
 *
 * In this implementation we dynamically build an elimination ordering by,
 * at each step in the algorithm, removing a vertex with the smallest out-degree.
 * Note that this still just gives an upperbound on the Kelly-width.
 */
template <typename DirectedGraph>
inline
typename boost::graph_traits<DirectedGraph>::vertices_size_type
elimination_ordering_destructive(DirectedGraph& g)
{
  typedef typename boost::graph_traits<DirectedGraph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<DirectedGraph>::vertex_descriptor vertex_t;

  // Build priority queue, sorted by outdegree, and record the handles.
  typedef outdegree_greater<DirectedGraph> cmp_t;
  typedef boost::heap::fibonacci_heap<vertex_t, boost::heap::compare<cmp_t> > priority_queue_t;
  typedef typename priority_queue_t::handle_type handle_t;
  cmp_t cmp(g);
  priority_queue_t pq(cmp);
  std::vector<handle_t> handles;

  typename boost::graph_traits<DirectedGraph>::vertex_iterator i, end;
  for(boost::tie(i,end) = boost::vertices(g); i != end; ++i)
  {
    handles.push_back(pq.push(*i));
  }

  // Compute the upperbound according to the dynamically computed elimination
  // ordering. Upperbound is maximum of the outdegrees of the verticese we
  // remove.
  vertex_size_t upperbound = std::numeric_limits<vertex_size_t>::min();

  while(!pq.empty())
  {
    vertex_t u = pq.top();
    if(pq.size() == 1)
      std::cerr << "HIER" << std::endl;
    pq.pop();
    upperbound = std::max(upperbound, boost::out_degree(u, g));
    eliminate_vertex_directed(u, g, pq, handles);
  }
  return upperbound;

}
}

template <typename DirectedGraph>
inline
typename boost::graph_traits<DirectedGraph>::vertices_size_type
elimination_ordering(const DirectedGraph& g)
{
  DirectedGraph destructable_g;
  boost::copy_graph(g, destructable_g);
  return detail::elimination_ordering_destructive(destructable_g);
}

#endif // KELLYWIDTH_H
