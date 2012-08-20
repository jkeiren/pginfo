#ifndef DIAMETER_H
#define DIAMETER_H

namespace graph
{

template <typename graph_t>
class diameter_info
{
  typedef graph::VertexIndex VertexIndex;
  typedef size_t weight_t;

protected:
  graph_t& m_graph;

  std::vector<size_t> m_min_distance;
  std::set< std::pair<weight_t, VertexIndex> > m_vertex_queue;
  size_t m_diameter;

  void initialise_single_source(VertexIndex s)
  {
    m_min_distance = std::vector<size_t>(m_graph.size(), std::numeric_limits<size_t>::max());
    m_min_distance[s] = 0;
  }

  void relax(VertexIndex u, VertexIndex v)
  {
    if(m_min_distance[v] > m_min_distance[u] + 1)
    {
      m_vertex_queue.erase(std::make_pair(m_min_distance[v], v));
      m_min_distance[v] = m_min_distance[u] + 1;
      m_vertex_queue.insert(std::make_pair(m_min_distance[v], v));
    }
  }

  void dijkstra(VertexIndex s)
  {
    mCRL2log(debug, "Dijkstra") << "Running SSSP with source " << s << " out of " << m_graph.size() << std::endl;
    initialise_single_source(s);
    m_vertex_queue.insert(std::make_pair(m_min_distance[s], s));

    while(!m_vertex_queue.empty())
    {
      VertexIndex u = m_vertex_queue.begin()->second;
      m_vertex_queue.erase(m_vertex_queue.begin());

      // Optimisation, record on the fly.
      if(m_min_distance[u] != std::numeric_limits<size_t>::max())
      {
        m_diameter = (std::max)(m_diameter, m_min_distance[u]);
      }

      typename graph_t::vertex_t v = m_graph.vertex(u);
      for(std::set<VertexIndex>::const_iterator i = v.out.begin(); i != v.out.end(); ++i)
      {
        relax(u,*i);
      }
    }
  }

#if 0 // too inefficient for, even, 1394-fin nodeadlock.
  std::vector<std::vector<size_t> > m_weights;
  //m_weights(m_graph.size(), std::vector<size_t>(m_graph.size(), std::numeric_limits<size_t>::max())),
  void initialise_weights()
  {
    mCRL2log(debug, "Floyd-Warsnall") << "Initialising weight matrix" << std::endl;
    m_weights = std::vector<std::map<size_t, size_t> >(m_graph.size());

    for(size_t i = 0; i < m_graph.size(); ++i)
    {
      mCRL2log(debug, "Floyd-Warsnall") << "  source " << i << std::endl;
      m_weights[i][i] = 0;
      typename graph_t::vertex_t v = m_graph.vertex(i);
      for(std::set<VertexIndex>::const_iterator j = v.out.begin(); j != v.out.end(); ++j)
      {
        m_weights[i][*j] = 1;
      }
    }
  }

  // all pairs shortest path using Floyd-Warshall
  void floyd_warshall()
  {
    std::vector<std::vector<size_t> > m_next(m_weights);

    mCRL2log(debug, "Floyd-Warshall") << "Running Floyd-Warshall on graph of size " << m_graph.size() << std::endl;
    for(size_t k = 0; k < m_graph.size(); ++k)
    {
      mCRL2log(debug, "Floyd-Warshall") << "  step " << k << std::endl;
      for (size_t i = 0; i < m_graph.size(); ++i)
      {
        for (size_t j = 0; j < m_graph.size(); ++j)
        {
          m_next[i][j] = (std::min)(m_weights[i][j], m_weights[i][k] + m_weights[k][j]);
        }
      }
      m_weights.swap(m_next);
    }
  }

  void diameter_fw()
  {
    mCRL2log(debug) << "Using Floyd-Warshall for ASSP computation" << std::endl;
    initialise_weights();
    floyd_warshall();

    mCRL2log(debug) << "Done computing all-source shortest paths using Floyd-Warshall" << std::endl
              << "determining diameter" << std::endl;

    m_diameter = 0;
    for(size_t i = 0; i < m_graph.size(); ++i)
    {
      for(size_t j = 0; j < m_graph.size(); ++j)
      {
        if(m_weights[i][j] != std::numeric_limits<size_t>::max())
        {
          m_diameter = (std::max)(m_diameter, m_weights[i][j]);
        }
      }
    }
  }
#endif

  // Run single source shortest path from every vertex, and update diameter
  // accordingly.
  void diameter_dijkstra()
  {
    mCRL2log(debug) << "Using Dijkstra's SSSP algorithm from all sources" << std::endl;
    for(VertexIndex i = 0; i < m_graph.size(); ++i)
    {
      dijkstra(i);
    }
  }

  void diameter()
  {
    mCRL2log(verbose) << "Computing diameter" << std::endl;
    diameter_dijkstra();
  }

public:
  diameter_info(graph_t& g)
    : m_graph(g),
      m_diameter(0)
  {
    diameter();
  }

  void yaml(YAML::Emitter& out) const
  {
    out << m_diameter;
  }
};

template <typename graph_t>
YAML::Emitter& operator<<(YAML::Emitter& out, const diameter_info<graph_t>& info)
{
  info.yaml(out);
  return out;
}

} // namespace graph

#endif // DIAMETER_H
