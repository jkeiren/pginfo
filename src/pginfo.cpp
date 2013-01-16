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
  size_t m_max_vertices_for_expensive;
  size_t m_neighbourhood_upto;
  typedef tools::input_output_tool super;

public:
  pginfo()
    : super("pginfo",
                                        "Jeroen J.A. Keiren",
                                        "Provides various sorts of structural information about parity games.",
                                        "Structural properties that are described in the paper XXX"), // TODO
      m_max_vertices_for_expensive(1000),
      m_neighbourhood_upto(3)
  {}

  void
  add_options(interface_description& desc)
  {
    super::add_options(desc);
    desc.
        add_option("max-vertices-for-expensive", make_mandatory_argument("NUM"),
                   "perform expensive checks only if the number of vertices is at most NUM", 'm').
        add_option("neighbourhood-upto", make_mandatory_argument("NUM"),
                   "compute all neighbourhoods up to and including the NUM-neighbourhood", 'n');;
  }

  void parse_options(const command_line_parser& parser)
  {
    super::parse_options(parser);
    if (parser.options.count("max-vertices-for-expensive"))
    {
      m_max_vertices_for_expensive = parser.option_argument_as< size_t > ("max-vertices-for-expensive");
    }
    if (parser.options.count("neighbourhood-upto"))
    {
      m_neighbourhood_upto = parser.option_argument_as< size_t > ("neighbourhood-upto");
    }
  }

  bool run()
  {
    std::istream& is = open_input(input_filename(), m_ifstream);
    std::ostream& os = open_output(output_filename(), m_ofstream);

    parity_game_t pg;
    parse_pgsolver(pg, is, timer());
    YAML::Emitter out;

    report(pg, out, m_max_vertices_for_expensive, m_neighbourhood_upto);

    os << out.c_str() << std::endl;

    return true;
  }
};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
