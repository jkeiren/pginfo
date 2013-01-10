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

#ifndef DFS_INFO_H
#define DFS_INFO_H

#include <boost/graph/depth_first_search.hpp>

namespace detail
{
template<class Tag>
struct stack_push_recorder: public boost::default_dfs_visitor
{
  size_t& max_stacksize;
  size_t& cur_stacksize;
  typedef Tag event_filter;

  stack_push_recorder(size_t& max_stacksize_, size_t& cur_stacksize_)
    : max_stacksize(max_stacksize_), cur_stacksize(cur_stacksize_)
  {}

  template<typename Vertex, typename Graph>
  inline void operator()(Vertex, Graph&)
  {
    ++cur_stacksize;
    max_stacksize = std::max(max_stacksize, cur_stacksize);
  }
};

struct stack_pop_recorder: public boost::default_dfs_visitor
{
  size_t& cur_stacksize;
  typedef boost::on_finish_vertex event_filter;

  stack_pop_recorder(size_t& cur_stacksize_)
    : cur_stacksize(cur_stacksize_)
  {}

  template<typename Vertex, typename Graph>
  inline void operator()(Vertex, Graph&)
  {
    --cur_stacksize;
  }
};

template<class Tag>
inline stack_push_recorder<Tag>
record_stack_push(size_t& max_stacksize, size_t& cur_stacksize, Tag)
{
  return stack_push_recorder<Tag>(max_stacksize, cur_stacksize);
}

inline stack_pop_recorder
record_stack_pop(size_t& cur_stacksize)
{
  return stack_pop_recorder(cur_stacksize);
}

} // namespace detail

template<typename Graph>
inline
size_t dfs_max_stacksize(const Graph& g, typename Graph::vertex_descriptor v = 0)
{
  size_t max = 0, cur = 0;
  boost::depth_first_search(g,
      boost::visitor(boost::make_dfs_visitor(std::make_pair(
          detail::record_stack_push(max, cur, boost::on_discover_vertex()),
          detail::record_stack_pop(cur))
      )).root_vertex(v));
  return max;
}

#endif // DFS_INFO_H
