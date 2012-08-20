#include "yaml-cpp/yaml.h"

#include "mcrl2/utilities/input_tool.h"
#include "mcrl2/utilities/logger.h"
#include "pg.h"
#include "parsers/pgsolver.h"

#include "bfs_info.h"
#include "dfs_info.h"
#include "diameter.h"
#include "girth.h"
#include "scc_info.h"
#include "graph_statistics.h"
#include "diamond.h"
#include "neighbourhood.h"
#include "alternation_depth.h"


class pginfo : public mcrl2::utilities::tools::input_tool
{
private:
  std::auto_ptr<std::ifstream> m_ifstream;

protected:
  bool m_expensive;

public:
  pginfo()
    : mcrl2::utilities::tools::input_tool("pginfo",
                                        "Jeroen J.A. Keiren",
                                        "Provides various sorts of structural information about parity games.",
                                        "Structural properties that are described in the paper XXX"), // TODO
      m_expensive(false)
  {}

  void
  add_options(mcrl2::utilities::interface_description& desc)
  {
    mcrl2::utilities::tools::input_tool::add_options(desc);
    desc.
    add_option("expensive", "perform expensive checks");
  }

  void parse_options(const mcrl2::utilities::command_line_parser& parser)
  {
    mcrl2::utilities::tools::input_tool::parse_options(parser);
    m_expensive = parser.options.count("expensive");
  }

  bool run()
  {
    std::istream& instream = graph::open_input(m_input_filename, m_ifstream);
    mCRL2log(mcrl2::log::verbose)
      << "Computing statistics of parity game." << std::endl;

    typedef graph::KripkeStructure<graph::Vertex<graph::pg::Label> > graph_t;
    graph_t pg;
    graph::load(pg, instream, timer());

    using namespace graph;
    using namespace graph::pg;

    YAML::Emitter out;
    out << YAML::BeginMap
        << YAML::Key << "Graph information"
        << YAML::Value << pg_info(pg)
        << YAML::Key << "SCC information"
        << YAML::Value << scc_info<graph_t>(pg)
        << YAML::Key << "BFS information"
        << YAML::Value << bfs_info<graph_t>(pg)
        << YAML::Key << "DFS information"
        << YAML::Value << dfs_info<graph_t>(pg);
    if(m_expensive)
    {
      out << YAML::Key << "Diameter"
          << YAML::Value << diameter_info<graph_t>(pg)
          << YAML::Key << "Girth"
          << YAML::Value << girth_info<graph_t>(pg);
    }
    out << YAML::Key << "Number of diamonds"
        << YAML::Value << diamond_count(pg)
        << YAML::Key << "Neighbourhoods"
        << YAML::Value << neighbourhood<graph_t>(pg,4)
        << YAML::Key << "Alternation depth(s)"
        << YAML::Value << alternation_depth<graph_t>(pg)
        << YAML::EndMap;

    std::cout << out.c_str() << std::endl;
  }

};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
