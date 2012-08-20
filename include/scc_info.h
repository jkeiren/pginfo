#ifndef SCC_INFO_H
#define SCC_INFO_H

#include <vector>

#include "graph.h"
#include "detail/scc.h"

namespace graph
{

template <typename graph_t>
class scc_info
{
  typedef graph::VertexIndex VertexIndex;

protected:

  graph_t m_graph;
  graph_t m_quotient_graph;
  std::vector<VertexIndex> m_sccs;

  void compute_sccs()
  {
    mCRL2log(verbose) << "Computing strongly connected components" << std::endl;
    using namespace graph;
    impl::tarjan_iterative(m_graph.vertices(), m_sccs, true);
    impl::guarantee_vertex0_in_scc0(m_graph.vertices(), m_sccs);
    m_quotient_graph = m_graph;
    impl::collapse(m_quotient_graph.vertices(), m_sccs);
  }

  std::map<VertexIndex, size_t> vertices_per_scc() const
  {
    mCRL2log(debug) << "Determining vertices per SCC" << std::endl;
    std::map<VertexIndex, size_t> result;
    for(std::vector<VertexIndex>::const_iterator i = m_sccs.begin(); i != m_sccs.end(); ++i)
    {
      increment(result, *i);
    }
    return result;
  }

  // Count trivial sccs (size 1)
  size_t trivial_sccs() const
  {
    mCRL2log(debug) << "Counting trivial SCCs" << std::endl;
    size_t result = 0;
    std::map<VertexIndex, size_t> vertices_per_scc_ = vertices_per_scc();

    for(std::map<VertexIndex, size_t>::const_iterator i = vertices_per_scc_.begin(); i != vertices_per_scc_.end(); ++i)
    {
      if(i->second == 1)
      {
        ++result;
      }
    }
    return result;
  }

  // Count terminal sccs
  size_t terminal_sccs() const
  {
    mCRL2log(debug) << "Counting terminal SCCs" << std::endl;
    size_t result = 0;
    for(typename std::vector<typename graph_t::vertex_t>::const_iterator i = m_quotient_graph.vertices().begin(); i != m_quotient_graph.vertices().end(); ++i)
    {
      if(i->out.empty())
      {
        ++result;
      }
    }
    return result;
  }

  size_t quotient_height_impl(const typename graph_t::vertex_t& v) const
  {
    if(v.out.empty())
    {
      return 0;
    }
    else
    {
      size_t max_height = 0;
      for(std::set<VertexIndex>::const_iterator i = v.out.begin(); i != v.out.end(); ++i)
      {
        max_height = (std::max)(max_height, quotient_height_impl(m_quotient_graph.vertex(*i)));
      }
      return 1 + max_height;
    }
  }

  // Compute quotient height
  size_t quotient_height() const
  {
    mCRL2log(debug) << "Determining quotient height" << std::endl;
    quotient_height_impl(m_quotient_graph.vertex(0));
  }

public:
  scc_info(const graph_t& g)
    : m_graph(g),
      m_sccs(m_graph.size())
  {
    compute_sccs();
  }

  void yaml(YAML::Emitter& out) const
  {
    out << YAML::BeginMap
        << YAML::Key << "Number of SCCs"
        << YAML::Value << m_quotient_graph.size()
        << YAML::Key << "Number of terminal SCCs"
        << YAML::Value << terminal_sccs()
        << YAML::Key << "Number of trivial SCCs"
        << YAML::Value << trivial_sccs()
        << YAML::Key << "Number of vertices per SCC"
        << YAML::Value << vertices_per_scc()
        << YAML::Key << "Quotient height"
        << YAML::Value << quotient_height()
        << YAML::EndMap;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const scc_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // SCC_INFO_H
