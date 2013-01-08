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

#ifndef ENTANGLEMENT_H
#define ENTANGLEMENT_H

#include <limits>
#include <utility>
#include <set>
#include "cpplogging/logger.h"

namespace detail
{

// Combine the two steps into a single function
// this allows for easier change to a manual stack with a while loop
template <typename Graph, typename CopContainer>
inline
bool entanglement_step(const Graph& g,
                       typename Graph::vertex_descriptor v,
                       CopContainer& d,
                       typename boost::graph_traits<Graph>::vertices_size_type k,
                       std::set<std::pair<typename Graph::vertex_descriptor, CopContainer > >& visited,
                       std::string indent,
                       bool cop_turn = true)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  if(cop_turn)
  {
    // result is true iff there is a step for the cops such that they
    // can capture the robber.
    assert(d.find(v) == d.end()); // robber cannot move to cop position
    bool result = false;
    for(typename CopContainer::iterator i = d.begin(); i != d.end() && !result; ++i)
    {
      cpplog(cpplogging::debug) << indent << "Cop step " << *i << " to " << v << std::endl;
      d.erase(i);
      d.insert(v);
      std::pair<typename Graph::vertex_descriptor, CopContainer > config = std::make_pair(v,d);
      if(visited.find(config) == visited.end())
      {
        visited.insert(config);
        result |= entanglement_step(g, v, d, k, visited, indent + "  ", false);
        visited.erase(config);
      }
    }
    cpplog(cpplogging::debug) << indent << "Cop step " << "pass" << std::endl;
    std::pair<typename Graph::vertex_descriptor, CopContainer > config = std::make_pair(v,d);
    if(visited.find(config) == visited.end())
    {
      visited.insert(config);
      result |= entanglement_step(g, v, d, k, visited, indent + "  ", false);
      visited.erase(config);
    }
    //result |= entanglement_step(g, v, d, k, visited, indent + "  ", !cop_turn); // no change in cop position
    return result;
  }
  else // robber's turn
  {
    typename boost::graph_traits< Graph >::adjacency_iterator ai,aend;
    // result is true iff for all steps of the robber, he can be captured
    // by the cops.
    bool caught = true;

    // while we have not found a vertex to evade capture, try next.
    for(boost::tie(ai, aend) = boost::adjacent_vertices(v, g); ai != aend && caught; ++ai)
    {
      if(d.find(*ai) == d.end())
      {
        // found successor vertex not covered by cop.
        // so we are not captured immediately.
        // we can evade capture by the cops if we can close a cycle of
        // configurations
        std::pair<typename Graph::vertex_descriptor, CopContainer > config = std::make_pair(*ai,d);
        if(visited.find(config) == visited.end())
        {
          visited.insert(config);
          cpplog(cpplogging::debug) << indent << "Robber step " << v << " to " << *ai << std::endl;
          caught &= entanglement_step(g, *ai, d, k, visited, indent + "  ", true);
          visited.erase(config);
        }
        else
        {
          cpplog(cpplogging::debug) << indent << "Cycle closed, robber can evade cops" << std::endl;
          caught = false;
        }
      }
      else
      {
        cpplog(cpplogging::debug) << indent << "Robber step " << v << " to " << *ai << " is blocked by cop" << std::endl;
      }
    }
    return caught;
  }
}

}

template <typename Graph>
inline
bool entanglement(const Graph& g, typename boost::graph_traits<Graph>::vertices_size_type k)
{
  typedef typename Graph::vertex_descriptor vertex_descriptor;
  vertex_descriptor v = *(boost::vertices(g).first); // current robber position
  cpplog(cpplogging::verbose) << "Computing entanglement starting with robber at " << v << " and " << k << " cops" << std::endl;
  std::string indent = "";

  typedef std::multiset<vertex_descriptor> cop_container_t;
  cop_container_t d;
  for(size_t i = 0; i < k; ++i)
  {
    d.insert(std::numeric_limits<vertex_descriptor>::max()); // cop positions; max() means outside the graph
  }
  typename std::set<std::pair<vertex_descriptor, cop_container_t > > visited;

  return detail::entanglement_step(g, v, d, k, visited, indent);
}

#endif // ENTANGLEMENT_H
