#include <gtest/gtest.h>
#include "execution_timer.h"
#include "logger.h"

#include "parsers/pgsolver.h"
#include "cases.h"

#include "graph_statistics.h"
#include "alternation_depth.h"
#include "bfs_info.h"
#include "dfs_info.h"
#include "diameter.h"
#include "diamond.h"
#include "girth.h"
//#include "neighbourhood.h" // TODO add test
#include "scc_info.h"

typedef graph::KripkeStructure<graph::Vertex<graph::pg::Label> > graph_t;

void load_graph(graph_t& pg, const std::string& s)
{
  execution_timer timer;
  std::stringstream ss;
  ss << s;
  graph::load(pg, ss, timer);
}

TEST(GraphStats, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::graph_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.num_vertices(), 75);
  EXPECT_EQ(algorithm.num_edges(), 93);
  EXPECT_LE(algorithm.average_degree(), 0.806453);
  EXPECT_GE(algorithm.average_degree(), 0.806451);
}

TEST(GraphStats, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::graph_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.num_vertices(), 132);
  EXPECT_EQ(algorithm.num_edges(), 162);
  EXPECT_LE(algorithm.average_degree(), 0.814816);
  EXPECT_GE(algorithm.average_degree(), 0.814814);
}

TEST(GameStats, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::pg::pg_info algorithm(pg);
  EXPECT_EQ(algorithm.num_even_vertices(), 0);
  EXPECT_EQ(algorithm.num_odd_vertices(), 75);
}

TEST(GameStats, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::pg::pg_info algorithm(pg);
  EXPECT_EQ(algorithm.num_even_vertices(), 1);
  EXPECT_EQ(algorithm.num_odd_vertices(), 131);
}

TEST(AlternationDepth, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(graph::alternation_depth<graph_t>(pg).get_alternation_depth(),0);
}

TEST(AlternationDepth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(graph::alternation_depth<graph_t>(pg).get_alternation_depth(),2);
}

TEST(BFS, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::bfs_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.get_levels(),20);
  EXPECT_EQ(algorithm.back_level_edges(), 14);
}

TEST(BFS, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::bfs_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.get_levels(),20);
  EXPECT_EQ(algorithm.back_level_edges(), 25);
}

TEST(DFS, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::dfs_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.maximum_stack_size(),5);
}

TEST(DFS, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::dfs_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.maximum_stack_size(), 7);
}

TEST(Diameter, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::diameter_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.diameter(), 30);
}

TEST(Diameter, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::diameter_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.diameter(), 30);
}

TEST(Diamond, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  EXPECT_EQ(diamond_count(pg), 8);
}

TEST(Diamond, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  EXPECT_EQ(diamond_count(pg), 12);
}

TEST(Girth, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::girth_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.girth(), 6);
}

TEST(Girth, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::girth_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.girth(), 1);
}

TEST(SCC, ABP_NODEADLOCK)
{
  graph_t pg;
  load_graph(pg, ABP_NODEADLOCK);
  graph::scc_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.sccs(), 2);
  EXPECT_EQ(algorithm.terminal_sccs(), 1);
  EXPECT_EQ(algorithm.trivial_sccs(), 1);
  EXPECT_EQ(algorithm.quotient_height(), 1);
}

TEST(SCC, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR)
{
  graph_t pg;
  load_graph(pg, ABP_READ_THEN_EVENTUALLY_SEND_IF_FAIR);
  graph::scc_info<graph_t> algorithm(pg);
  EXPECT_EQ(algorithm.sccs(), 23);
  EXPECT_EQ(algorithm.terminal_sccs(), 1);
  EXPECT_EQ(algorithm.trivial_sccs(), 18);
  EXPECT_EQ(algorithm.quotient_height(), 7);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
