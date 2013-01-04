#include "yaml-cpp/yaml.h"

#include "cppcli/input_tool.h"
#include "cpplogging/logger.h"
#include "pg.h"
#include "pgsolver_io.h"
#include "utilities.h"

#include "graph_info.h"
#include "bfs_info.h"
#include "diameter.h"

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

    bfs_info<parity_game_t> bfs_inf(pg);

    out << YAML::BeginMap
        << YAML::Key << "Number of vertices"
        << YAML::Value << boost::num_vertices(pg)
        << YAML::Key << "Number of edges"
        << YAML::Value << boost::num_edges(pg)
        << YAML::Key << "Average degree"
        << YAML::Value << avg_degree(pg)
        << YAML::Key << "Number of levels (BFS height)"
        << YAML::Value << bfs_inf.get_levels()
        << YAML::Key << "Nodes per level"
        << YAML::Value << bfs_inf.nodes_per_level()
        << YAML::Key << "Number of back level edges"
        << YAML::Value << bfs_inf.back_level_edges()
        << YAML::Key << "Lengths of back level edges"
        << YAML::Value << bfs_inf.back_edge_map();

    out << YAML::Key << "Diameter"
        << YAML::Value << diameter(pg);

    out << YAML::EndMap;

    std::cout << out.c_str() << std::endl;

    return true;
  }
};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
