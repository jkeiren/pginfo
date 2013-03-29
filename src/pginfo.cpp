#include "yaml-cpp/yaml.h"

#include "cppcli/input_output_tool.h"
#include "cpplogging/logger.h"
#include "pg.h"
#include "pgsolver_io.h"
#include "utilities.h"
#include "report.h"

class pginfo : public tools::input_output_tool
{
private:
  std::ifstream m_ifstream; // dummy for opening file
  std::ofstream m_ofstream;

protected:
  report_options m_options;
  typedef tools::input_output_tool super;

public:
  pginfo()
    : super("pginfo",
                                        "Jeroen J.A. Keiren",
                                        "Provides various sorts of structural information about parity games.",
                                        "Structural properties that are described in the paper XXX"), // TODO
      m_options(false)
  {}

  void
  add_options(interface_description& desc)
  {
    super::add_options(desc);
    desc.
        add_option("all", "compute all statistics about the graph. Overrules all other options").
        add_option("graph", "compute general information about the graph").
        add_option("bfs", "compute information from BFS on the graph").
        add_option("dfs", "compute information from DFS on the graph").
        add_option("diameter", "compute the diameter of the graph").
        add_option("girth", "compute the girth of the graph").
        add_option("diamonds", "compute the number of diamonds in the graph").
        add_option("neighbourhoods", make_mandatory_argument<size_t>("NUM"),
                   "compute the sizes of the neighbourhoods up to and including NUM").
        add_option("treewidth-lb", "compute lowerbound on treewidth").
        add_option("treewidth-ub", "compute upperbound on treewidth").
        add_option("kellywidth-ub", "compute upperbound on Kelly-width").
        add_option("sccs", "compute strongly connected components").
        add_option("ad-cks", "compute alternation-depth using the algorithm from [CKS93]").
        add_option("ad", "compute alternation-depth using a sorting of priorities").
        add_option("max-for-expensive", make_mandatory_argument<size_t>("NUM"),
                    "for BFS and DFS do not records queue or stack sizes if the "
                    "number of vertices exceeds NUM");
  }

  void parse_options(const command_line_parser& parser)
  {
    super::parse_options(parser);
    if (parser.options.count("all"))
    {
      m_options = report_options(true);
    }
    else
    {
      m_options.general_graph_info = parser.options.count("graph");
      m_options.bfs_info = parser.options.count("bfs");
      m_options.dfs_info = parser.options.count("dfs");
      m_options.diameter = parser.options.count("diameter");
      m_options.girth = parser.options.count("girth");
      m_options.diamonds = parser.options.count("diamonds");
      m_options.neighbourhoods = parser.options.count("neighbourhoods");
      if(m_options.neighbourhoods)
        m_options.neighbourhoods_upto = parser.option_argument_as<size_t>("neighbourhoods");
      m_options.treewidth_lowerbound = parser.options.count("treewidth-lb");
      m_options.treewidth_upperbound = parser.options.count("treewidth-ub");
      m_options.kellywidth_upperbound = parser.options.count("kellywidth-ub");
      m_options.sccs = parser.options.count("sccs");
      m_options.alternation_depth_cks = parser.options.count("ad-cks");
      m_options.alternation_depth = parser.options.count("ad");
    }
    if(parser.options.count("max-for-expensive"))
    {
      m_options.max_vertices_for_expensive_checks = parser.option_argument_as<size_t>("max-for-expensive");
    }
  }

  bool run()
  {
    std::istream& is = open_input(input_filename(), m_ifstream);
    std::ostream& os = open_output(output_filename(), m_ofstream);

    parity_game_t pg;
    parse_pgsolver(pg, is, timer());
    YAML::Emitter out;

    report(pg, out, m_options);

    os << out.c_str() << std::endl;

    return true;
  }
};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
