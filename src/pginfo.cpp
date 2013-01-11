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
  bool m_expensive;
  typedef tools::input_output_tool super;

public:
  pginfo()
    : super("pginfo",
                                        "Jeroen J.A. Keiren",
                                        "Provides various sorts of structural information about parity games.",
                                        "Structural properties that are described in the paper XXX"), // TODO
      m_expensive(false)
  {}

  void
  add_options(interface_description& desc)
  {
    super::add_options(desc);
    desc.
    add_option("expensive", "perform expensive checks");
  }

  void parse_options(const command_line_parser& parser)
  {
    super::parse_options(parser);
    m_expensive = parser.options.count("expensive");
  }

  bool run()
  {
    std::istream& is = open_input(input_filename(), m_ifstream);
    std::ostream& os = open_output(output_filename(), m_ofstream);

    parity_game_t pg;
    parse_pgsolver(pg, is, timer());
    YAML::Emitter out;

    report(pg, out);

    os << out.c_str() << std::endl;

    return true;
  }
};

int main(int argc, char** argv)
{
  return std::auto_ptr<pginfo>(new pginfo())->execute(argc, argv);
}
