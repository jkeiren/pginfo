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
// Use the convention that changing the color of a vertex to gray
// means pushing on the stack, changing color to black is popping from the stack.
// The stack size at position k is the size of the stack once we finish
// processing the k'th vertex.
template<class Tag>
struct stack_push_recorder: public boost::default_dfs_visitor
{
  size_t& cur_stacksize;
  typedef Tag event_filter;

  stack_push_recorder(size_t& cur_stacksize_)
    : cur_stacksize(cur_stacksize_)
  {}

  template<typename Vertex, typename Graph>
  inline void operator()(Vertex, Graph&)
  {
    ++cur_stacksize;
  }
};

template <typename OutIter>
struct stack_pop_recorder: public boost::default_dfs_visitor
{
  size_t& m_cur_stacksize;
  OutIter m_stack_size_out;
  typedef boost::on_finish_vertex event_filter;

  stack_pop_recorder(size_t& cur_stacksize, OutIter stack_size_out)
    : m_cur_stacksize(cur_stacksize),
      m_stack_size_out(stack_size_out)
  {}

  template<typename Vertex, typename Graph>
  inline void operator()(Vertex, Graph&)
  {
    --m_cur_stacksize;
    *(m_stack_size_out)++ = m_cur_stacksize;
  }
};

template<class Tag>
inline stack_push_recorder<Tag>
record_stack_push(size_t& cur_stacksize, Tag)
{
  return stack_push_recorder<Tag>(cur_stacksize);
}

template<typename StackSizeMap>
inline stack_pop_recorder<StackSizeMap>
record_stack_pop(size_t& cur_stacksize, StackSizeMap stack_sizes)
{
  return stack_pop_recorder<StackSizeMap>(cur_stacksize, stack_sizes);
}

} // namespace detail

template<typename Graph>
inline
std::vector<typename boost::graph_traits<Graph>::vertices_size_type>
dfs_stack_sizes(const Graph& g, typename Graph::vertex_descriptor v = 0)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  vertex_size_t cur = 0;
  std::vector<vertex_size_t> stack_size_map(boost::num_vertices(g), 0);
  boost::depth_first_search(g,
      boost::visitor(boost::make_dfs_visitor(std::make_pair(
          detail::record_stack_push(cur, boost::on_discover_vertex()),
          detail::record_stack_pop(cur, &(stack_size_map[0]))
      ))).root_vertex(v));
  return stack_size_map;
}

#endif // DFS_INFO_H
