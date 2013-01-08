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

#ifndef PGBGL_H
#define PGBGL_H

#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

/**
 * @brief Enumerated constants to indicate parity game players.
 */
enum player_t
{
	even = 0,///< even
	odd  = 1 ///< odd
};

typedef size_t priority_t; ///< Type of vertex priorities.

/**
 * @brief Parity game label
 */
struct pg_label_t
{
  //size_t vertex_index;
  size_t node_id; ///< The identity of the vertex
	priority_t prio; ///< The vertex priority
	player_t player; ///< The owner of the vertex
	/// @brief Comparison to make pg_label_t a valid mapping index.
	bool operator<(const pg_label_t& other) const
	{
		return (prio < other.prio)
			or (prio == other.prio and player < other.player);
	}
	/// @brief Equality comparison.
	bool operator==(const pg_label_t& other) const
	{
		return (prio == other.prio)
		   and (player == other.player);
	}
};

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS, pg_label_t > parity_game_t;

struct is_even_vtx
{
  parity_game_t& m_pg;
  is_even_vtx(parity_game_t& pg)
    : m_pg(pg)
  {}

  bool operator()(parity_game_t::vertex_descriptor v)
  {
    return m_pg[v].player == even;
  }
};

inline
size_t num_even_vertices(parity_game_t& pg)
{
  boost::graph_traits< parity_game_t >::vertex_iterator i, end;
  boost::tie(i, end) = vertices(pg);
  return std::count_if(i, end, is_even_vtx(pg));
}

inline
size_t num_odd_vertices(parity_game_t& pg)
{
  return boost::num_vertices(pg) - num_even_vertices(pg);
}
#endif // PGBGL_H
