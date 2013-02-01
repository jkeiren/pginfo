#include <gtest/gtest.h>

#include "cppcli/execution_timer.h"
#include "cpplogging/logger.h"
#include "pg.h"
#include "pgsolver_io.h"

#include <boost/graph/wavefront.hpp>

//#include "parsers/pgsolver.h"

#include "cases.h"

#include "degree.h"
#include "bfs.h"
#include "dfs.h"
#include "diameter.h"
#include "diamond.h"
#include "girth.h"
#include "neighbourhood.h"
#include "scc.h"
#include "entanglement.h"
#include "treewidth.h"
#include "alternation_depth.h"
#include "kellywidth.h"

template<typename ParityGame>
void load_graph(ParityGame& pg, const std::string& s)
{
  execution_timer timer;
  std::stringstream ss;
  ss << s;
  parse_pgsolver(pg, ss, timer);
}

TEST(GraphStats, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(4, boost::num_vertices(pg));
  EXPECT_EQ(5, boost::num_edges(pg));
  EXPECT_DOUBLE_EQ(5.0/4.0, avg_degree(pg));
  EXPECT_EQ(1, min_degree(pg));
  EXPECT_EQ(5, max_degree(pg));
  EXPECT_DOUBLE_EQ(5.0/4.0, avg_in_degree(pg));
  EXPECT_EQ(0, min_in_degree(pg));
  EXPECT_EQ(3, max_in_degree(pg));
  EXPECT_DOUBLE_EQ(5.0/4.0, avg_out_degree(pg));
  EXPECT_EQ(1, min_out_degree(pg));
  EXPECT_EQ(2, max_out_degree(pg));
}

TEST(GraphStats, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(75, boost::num_vertices(pg));
  EXPECT_EQ(93, boost::num_edges(pg));
  EXPECT_DOUBLE_EQ(93.0/75.0, avg_degree(pg));
  EXPECT_EQ(1, min_degree(pg));
  EXPECT_EQ(5, max_degree(pg));
  EXPECT_DOUBLE_EQ(1.24, avg_in_degree(pg));
  EXPECT_EQ(0, min_in_degree(pg));
  EXPECT_EQ(3, max_in_degree(pg));
  EXPECT_DOUBLE_EQ(1.24, avg_out_degree(pg));
  EXPECT_EQ(1, min_out_degree(pg));
  EXPECT_EQ(2, max_out_degree(pg));
}

TEST(GraphStats, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(132, boost::num_vertices(pg));
  EXPECT_EQ(162, boost::num_edges(pg));
  EXPECT_DOUBLE_EQ(162.0/132.0, avg_degree(pg));
  EXPECT_EQ(1, min_degree(pg));
  EXPECT_EQ(7, max_degree(pg));
  EXPECT_DOUBLE_EQ(162.0/132.0, avg_degree(pg));
  EXPECT_EQ(0, min_in_degree(pg));
  EXPECT_EQ(5, max_in_degree(pg));
  EXPECT_DOUBLE_EQ(162.0/132.0, avg_degree(pg));
  EXPECT_EQ(1, min_out_degree(pg));
  EXPECT_EQ(4, max_out_degree(pg));
}

TEST(GameStats, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(0, num_even_vertices(pg));
  EXPECT_EQ(4, num_odd_vertices(pg));
}

TEST(GameStats, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(0,num_even_vertices(pg));
  EXPECT_EQ(75,num_odd_vertices(pg));
}

TEST(GameStats, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(1,num_even_vertices(pg));
  EXPECT_EQ(131,num_odd_vertices(pg));
}

TEST(BFS, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(3, bfs_levels(pg));
  EXPECT_EQ(2, back_level_edges(pg));
}

TEST(BFS, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(21, bfs_levels(pg));
  EXPECT_EQ(14, back_level_edges(pg));
}

TEST(BFS, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(21, bfs_levels(pg));
  EXPECT_EQ(25, back_level_edges(pg));
}

TEST(DFS, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);;
  std::vector<boost::graph_traits<parity_game_t>::vertices_size_type> s = dfs_stack_sizes(pg);
  EXPECT_EQ(2, *std::max_element(s.begin(), s.end()));
}

TEST(DFS, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  std::vector<boost::graph_traits<parity_game_t>::vertices_size_type> s = dfs_stack_sizes(pg);
  //EXPECT_EQ(5, dfs_max_stacksize(pg)); previous implementation
  EXPECT_EQ(20, *std::max_element(s.begin(), s.end()));
}

TEST(DFS, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  std::vector<boost::graph_traits<parity_game_t>::vertices_size_type> s = dfs_stack_sizes(pg);
  //EXPECT_EQ(7, dfs_max_stacksize(pg)); previous implementation
  EXPECT_EQ(20, *std::max_element(s.begin(), s.end()));
}

TEST(Diameter, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(2, diameter(pg));
}

TEST(Diameter, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(30, diameter(pg));
}

TEST(Diameter, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(30, diameter(pg));
}

TEST(Diamond, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  diamond_count_t diamonds = diamond_count(pg);
  EXPECT_EQ(1, diamonds.all);
  EXPECT_EQ(0,diamonds.even);
  EXPECT_EQ(1,diamonds.odd);
}

TEST(Diamond, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  diamond_count_t diamonds = diamond_count(pg);
  EXPECT_EQ(8,diamonds.all);
  EXPECT_EQ(0,diamonds.even);
  EXPECT_EQ(8,diamonds.odd);
  /* Manually verified result, diamonds are:
   * 15 - 19,20 - 2
   * 17 - 22,23 - 3
   * 33 - 37,38 - 45
   * 34 - 39,40 - 46
   * 52 - 56,57 - 31
   * 54 - 59,60 - 32
   * 67 - 69,70 - 73
   * 68 - 71,72 - 74
   */
}

TEST(Diamond, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  diamond_count_t diamonds = diamond_count(pg);
  EXPECT_EQ(8,diamonds.all);
  EXPECT_EQ(0,diamonds.even);
  EXPECT_EQ(8,diamonds.odd);
  /* Manually verified result, diamonds are:
   * 21 - 25,26 - 2
   * 23 - 28,29 - 3
   * 45 - 49,50 - 57
   * 46 - 51,52 - 58
   * 68 - 72,73 - 74
   * 70 - 75,76 - 42
   * 87 - 89,90 - 93
   * 88 - 91,92 - 94
   */
}

TEST(Girth, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(2, girth(pg));
}

TEST(Girth, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(6, girth(pg));
}

TEST(Girth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(1, girth(pg));
}

TEST(SCC, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(2, sccs(pg));
  EXPECT_EQ(1, terminal_sccs(pg));
  EXPECT_EQ(1, trivial_sccs(pg));
  EXPECT_EQ(2, quotient_height(pg));
}

TEST(SCC, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(2, sccs(pg));
  EXPECT_EQ(1, terminal_sccs(pg));
  EXPECT_EQ(1, trivial_sccs(pg));
  EXPECT_EQ(2, quotient_height(pg));
}

TEST(SCC, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(23, sccs(pg));
  EXPECT_EQ(1, terminal_sccs(pg));
  EXPECT_EQ(18, trivial_sccs(pg));
  EXPECT_EQ(8, quotient_height(pg));
}

TEST(Neighbourhood, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_DOUBLE_EQ(avg_out_degree(pg), avg_kneighbourhood(pg, 1));
  EXPECT_EQ(2, max_kneighbourhood(pg, 1));
  EXPECT_EQ(1, min_kneighbourhood(pg, 1));
  EXPECT_DOUBLE_EQ(9.0/4.0, avg_kneighbourhood(pg, 2));
  EXPECT_EQ(3, max_kneighbourhood(pg, 2));
  EXPECT_EQ(2, min_kneighbourhood(pg, 2));
  EXPECT_DOUBLE_EQ(9.0/4.0, avg_kneighbourhood(pg, 3));
  EXPECT_EQ(3, max_kneighbourhood(pg, 3));
  EXPECT_EQ(2, min_kneighbourhood(pg, 3));
}

TEST(Neighbourhood, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_DOUBLE_EQ(avg_out_degree(pg), avg_kneighbourhood(pg, 1));
  EXPECT_EQ(2, max_kneighbourhood(pg, 1));
  EXPECT_EQ(1, min_kneighbourhood(pg, 1));
  EXPECT_NEAR(2.70666, avg_kneighbourhood(pg, 2), 0.00001);
  EXPECT_EQ(4, max_kneighbourhood(pg, 2));
  EXPECT_EQ(2, min_kneighbourhood(pg, 2));
  EXPECT_NEAR(4.54666, avg_kneighbourhood(pg, 3), 0.00001);
  EXPECT_EQ(8, max_kneighbourhood(pg, 3));
  EXPECT_EQ(3, min_kneighbourhood(pg, 3));
  EXPECT_NEAR(6.78667, avg_kneighbourhood(pg, 4), 0.00001);
  EXPECT_EQ(12, max_kneighbourhood(pg, 4));
  EXPECT_EQ(5, min_kneighbourhood(pg, 4));
  EXPECT_NEAR(9.50666, avg_kneighbourhood(pg, 5), 0.00001);
  EXPECT_EQ(16, max_kneighbourhood(pg, 5));
  EXPECT_EQ(6, min_kneighbourhood(pg, 5));
}

TEST(Neighbourhood, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  // The first one is expected to fail, since a vertex is not in its own k-neighbourhood.
  // Hence, for vertices with self-loops the outdegree and the 1-neighbourhood
  // are different.
  //EXPECT_DOUBLE_EQ(average_outdegree(pg), avg_kneighbourhood(pg, 1));
  EXPECT_NEAR(1.21969, avg_kneighbourhood(pg, 1), 0.00001);
  EXPECT_EQ(4, max_kneighbourhood(pg, 1));
  EXPECT_EQ(0, min_kneighbourhood(pg, 1));
  EXPECT_NEAR(2.67424, avg_kneighbourhood(pg, 2), 0.00001);
  EXPECT_EQ(8, max_kneighbourhood(pg, 2));
  EXPECT_EQ(0, min_kneighbourhood(pg, 2));
  EXPECT_NEAR(4.46212, avg_kneighbourhood(pg, 3), 0.00001);
  EXPECT_EQ(14, max_kneighbourhood(pg, 3));
  EXPECT_EQ(0, min_kneighbourhood(pg, 3));
  EXPECT_NEAR(6.65909, avg_kneighbourhood(pg, 4), 0.00001);
  EXPECT_EQ(22, max_kneighbourhood(pg, 4));
  EXPECT_EQ(0, min_kneighbourhood(pg, 4));
  EXPECT_NEAR(9.38636, avg_kneighbourhood(pg, 5), 0.00001);
  EXPECT_EQ(30, max_kneighbourhood(pg, 5));
  EXPECT_EQ(0, min_kneighbourhood(pg, 5));
}

TEST(Wavefront, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_NEAR(2.0, boost::aver_wavefront(pg), 0.00001);
  EXPECT_NEAR(2.12132, boost::rms_wavefront(pg), 0.00001);
  EXPECT_EQ(3, boost::max_wavefront(pg));
}

TEST(Wavefront, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_NEAR(5.30666, boost::aver_wavefront(pg), 0.00001);
  EXPECT_NEAR(5.55457, boost::rms_wavefront(pg), 0.00001);
  EXPECT_EQ(9, boost::max_wavefront(pg));
}

TEST(Wavefront, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_NEAR(14.85606, boost::aver_wavefront(pg), 0.00001);
  EXPECT_NEAR(16.05317, boost::rms_wavefront(pg), 0.00001);
  EXPECT_EQ(25, boost::max_wavefront(pg));
}

TEST(Entanglement, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_FALSE(entanglement(pg, 0));
  for(size_t i = 1; i <= 4; ++i) // only have 4 vertices, so no use of going beyond 4
    EXPECT_TRUE(entanglement(pg, i));
}

TEST(Entanglement, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_FALSE(entanglement(pg, 0));
  //The following two are still too slow
  //EXPECT_FALSE(entanglement(pg, 1));
  //EXPECT_TRUE(entanglement(pg, 2));
}

TEST(Treewidth, BUFFER_NODEADLOCK)
{
  undirected_parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(1, minor_min_width(pg));
  EXPECT_EQ(1, greedy_degree(pg));
  //EXPECT_EQ(1, treewidth(pg));
}

TEST(Treewidth, ABP_NODEADLOCK)
{
  undirected_parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(2, minor_min_width(pg));
  EXPECT_EQ(2, greedy_degree(pg));
  //EXPECT_EQ(2, treewidth(pg));
}

TEST(Treewidth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  undirected_parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(2, minor_min_width(pg));
  EXPECT_EQ(2, greedy_degree(pg));
  //EXPECT_EQ(2, treewidth(pg));
}

TEST(Kellywidth, BUFFER_NODEADLOCK)
{
  undirected_parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(1, elimination_ordering(pg));
}

TEST(Kellywidth, ABP_NODEADLOCK)
{
  undirected_parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(2, elimination_ordering(pg));
}

TEST(Kellywidth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  undirected_parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(2, elimination_ordering(pg));
}

TEST(AlternationDepth, BUFFER_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, BUFFER_NODEADLOCK);
  EXPECT_EQ(1, alternation_depth(pg));
  EXPECT_EQ(1, alternation_depth_priority_sorting(pg));
}

TEST(AlternationDepth, ABP_NODEADLOCK)
{
  parity_game_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(1, alternation_depth(pg));
  EXPECT_EQ(1, alternation_depth_priority_sorting(pg));
}

TEST(AlternationDepth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  parity_game_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(2,alternation_depth(pg));
  EXPECT_EQ(2,alternation_depth_priority_sorting(pg));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  //cpplogging::logger::set_reporting_level(cpplogging::debug);
  return RUN_ALL_TESTS();
}
