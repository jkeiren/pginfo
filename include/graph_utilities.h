#ifndef GRAPH_UTILITIES_H
#define GRAPH_UTILITIES_H

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

namespace detail
{

/* \brief Functor that compares vertices in a graph by their out degree
 */
template <typename Graph>
struct outdegree_greater
{
  const Graph& m_g;
  typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;

  outdegree_greater(const Graph& g)
    : m_g(g)
  {}

  bool operator()(const vertex_t& v, const vertex_t& w) const
  {
    return boost::out_degree(v, m_g) > boost::out_degree(w, m_g);
  }
};

} // namespace detail

#endif // GRAPH_UTILITIES_H
