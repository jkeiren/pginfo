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

#ifndef SCC_INFO_H
#define SCC_INFO_H

#include <boost/graph/strong_components.hpp>

template<typename Graph>
class scc_info
{
protected:
  Graph& m_g;
  Graph m_quotient_g;
  typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;

  std::vector<vertex_size_t> m_component;
  size_t m_num_components;

  // pre: sccs have been computed.
  void quotient()
  {
    /* Fix quotient graph such that vertex 0 is in component 0 */
    size_t comp_zero = m_component[0];
    for(auto i = m_component.begin(); i != m_component.end(); ++i)
    {
      if(*i == 0)
        *i = comp_zero;
      else if(*i == comp_zero)
        *i = 0;
    }

    for(size_t i = 0; i < m_num_components; ++i)
      boost::add_vertex(m_quotient_g);

    typename boost::graph_traits< Graph >::edge_iterator i, end;
    for(boost::tie(i, end) = boost::edges(m_g); i != end; ++i)
    {
      typename Graph::vertex_descriptor u = boost::source(*i,m_g), v = boost::target(*i,m_g);
      if(m_component[u] != m_component[v])
        boost::add_edge(m_component[u], m_component[v], m_quotient_g);
    }
  }

  void compute()
  {
    m_num_components = boost::strong_components(m_g, &m_component[0]);
    quotient();
  }

public:
  scc_info(Graph& g)
    : m_g(g),
      m_component(num_vertices(g)),
      m_num_components(0)
  {
    compute();
  }

  size_t sccs() const
  {
    return m_num_components;
  }

  size_t quotient_height() const
  {
    vertex_t v = *(boost::vertices(m_quotient_g).first);
    std::vector<vertex_size_t> d(boost::num_vertices(m_quotient_g), 0);
    boost::breadth_first_search(m_quotient_g, v,
            boost::visitor(boost::make_bfs_visitor(boost::record_distances(&d[0], boost::on_tree_edge()))
        ));
    return *std::max_element(d.begin(), d.end());
  }

  size_t trivial_sccs() const
  {
    assert(num_vertices(m_quotient_g) > 0);
    std::vector<vertex_size_t> sorted_components = m_component;
    std::sort(sorted_components.begin(), sorted_components.end());

    size_t result = 0;
    auto i = sorted_components.begin();
    size_t prev = *(i++);
    size_t count = 1;
    if(i == sorted_components.end())
      return 1;
    for(; i != sorted_components.end(); ++i)
    {
      if(prev == *i)
        ++count;
      else
      {
        if(count == 1)
          ++result;
        count = 1;
        prev = *i;
      }
    }
    if(count == 1) ++result;
    return result;
  }

  size_t terminal_sccs() const
  {
    size_t result = 0;
    typename boost::graph_traits< Graph >::vertex_iterator i, end;
    for(boost::tie(i, end) = boost::vertices(m_quotient_g); i != end; ++i)
    {
      auto e = boost::out_edges(*i, m_quotient_g);
      if(e.first == e.second)
        ++result;
    }
    return result;
  }

};

#endif // SCC_INFO_H
