#include "yaml-cpp/yaml.h"

#include "cppcli/input_tool.h"
#include "cpplogging/logger.h"
#include "pg.h"
#include "pgsolver_io.h"
#include "utilities.h"

#include "bfs.h"
#include "degree.h"
#include "dfs.h"
#include "diameter.h"
#include "diamond.h"
#include "girth.h"
#include "neighbourhood.h"
#include "scc.h"

class pginfo : public tools::input_tool
{
private:
  std::ifstream m_ifstream; // dummy for opening file

protected:
  bool m_expensive;

public:
  pginfo()
    : tools::input_tool("pginfo",
                                        "Jeroen J.A. Keiren",
                                        "Provides various sorts of structural information about parity games.",
                                        "Structural properties that are described in the paper XXX"), // TODO
      m_expensive(false)
  {}

  void
  add_options(interface_description& desc)
  {
    tools::input_tool::add_options(desc);
    desc.
    add_option("expensive", "perform expensive checks");
  }

  void parse_options(const command_line_parser& parser)
  {
    tools::input_tool::parse_options(parser);
    m_expensive = parser.options.count("expensive");
  }

  bool run()
  {
    std::istream& is = open_input(input_filename(), m_ifstream);
    parity_game_t pg;
    parse_pgsolver(pg, is, timer());
    YAML::Emitter out;

    typedef typename boost::graph_traits<parity_game_t>::vertices_size_type vertex_size_t;

    out << YAML::BeginMap
        << YAML::Key << "Number of vertices"
        << YAML::Value << boost::num_vertices(pg)
        << YAML::Key << "Number of edges"
        << YAML::Value << boost::num_edges(pg)
        << YAML::Key << "Degree"
           << YAML::Value
           << YAML::BeginMap
           << YAML::Key << "min" << YAML::Value << min_degree(pg)
           << YAML::Key << "max" << YAML::Value << max_degree(pg)
           << YAML::Key << "avg" << YAML::Value << avg_degree(pg)
           << YAML::EndMap
        << YAML::Key << "In-degree"
           << YAML::Value
           << YAML::BeginMap
           << YAML::Key << "min" << YAML::Value << min_in_degree(pg)
           << YAML::Key << "max" << YAML::Value << max_in_degree(pg)
           << YAML::Key << "avg" << YAML::Value << avg_in_degree(pg)
           << YAML::EndMap
        << YAML::Key << "Out-degree"
           << YAML::Value
           << YAML::BeginMap
           << YAML::Key << "min" << YAML::Value << min_out_degree(pg)
           << YAML::Key << "max" << YAML::Value << max_out_degree(pg)
           << YAML::Key << "avg" << YAML::Value << avg_out_degree(pg)
           << YAML::EndMap;

    std::vector<vertex_size_t> levels(boost::num_vertices(pg), 0);
    vertex_size_t nlevels = bfs_levels(pg, 0, levels);
    std::map<vertex_size_t, vertex_size_t> bfs_back_level_edges;
    vertex_size_t nback_edges = back_level_edges(pg, 0, bfs_back_level_edges);
    out << YAML::Key << "BFS"
        << YAML::Value
        << YAML::BeginMap
          << YAML::Key << "Number of levels (BFS height)"
          << YAML::Value << nlevels
          << YAML::Key << "Nodes per level"
          << YAML::Value << bfs_nodes_per_level(levels)
          << YAML::Key << "Number of back level edges"
          << YAML::Value << nback_edges
          << YAML::Key << "Lengths of back level edges"
          << YAML::Value << bfs_back_level_edges
       << YAML::EndMap;

    out << YAML::Key << "DFS"
        << YAML::Value
        << YAML::BeginMap
        << YAML::Key << "Max stack"
        << YAML::Value << dfs_max_stacksize(pg)
        << YAML::EndMap;

    out << YAML::Key << "Diameter"
        << YAML::Value << diameter(pg);
    out << YAML::Key << "Girth"
        << YAML::Value << girth(pg);
    out << YAML::Key << "Diamonds"
        << YAML::Value << diamond_count(pg);

    out << YAML::Key << "Neighbourhood"
        << YAML::Value
        << YAML::BeginMap;
    for(size_t i = 1; i <= 5; ++i)
    {
      out << YAML::Key << i
          << YAML::Value
          << YAML::BeginMap
          << YAML::Key << "min" << YAML::Value << min_kneighbourhood(pg, i)
          << YAML::Key << "max" << YAML::Value << max_kneighbourhood(pg, i)
          << YAML::Key << "avg" << YAML::Value << avg_kneighbourhood(pg, i)
          << YAML::EndMap;
    }
    out << YAML::EndMap;

    std::vector<vertex_size_t> sccs (boost::num_vertices(pg), 0);
    vertex_size_t nsccs = boost::strong_components(pg, &sccs[0]);
    parity_game_t quotient = quotient_graph(pg, sccs, nsccs);
    out << YAML::Key << "SCC"
        << YAML::Value
        << YAML::BeginMap
        << YAML::Key << "SCCs" << YAML::Value << nsccs
        << YAML::Key << "Trivial SCCs" << YAML::Value << count_elements_occurring_exactly_n_times(sccs, 1)
        << YAML::Key << "Terminal SCCs" << YAML::Value << vertices_out_degree_n(quotient, 0)
        << YAML::Key << "Quotient height" << YAML::Value << bfs_levels(quotient)
        << YAML::EndMap;

    out << YAML::EndMap;

    std::cout << out.c_str() << std::endl;

    return true;
  }
};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
