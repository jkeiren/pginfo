// Author(s): Jeroen Keiren
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file /path/to/file.ext
/// \brief Description comes here

#ifndef REPORT_H
#define REPORT_H

#include "bfs.h"
#include "degree.h"
#include "dfs.h"
#include "diameter.h"
#include "diamond.h"
#include "girth.h"
#include "neighbourhood.h"
#include "scc.h"
#include "alternation_depth.h"
#include "treewidth.h"
#include "kellywidth.h"

struct report_options
{
  bool general_graph_info;
  bool bfs_info;
  bool dfs_info;
  bool diameter;
  bool girth;
  bool diamonds;
  bool neighbourhoods;
  size_t neighbourhoods_upto;
  bool treewidth_lowerbound;
  bool treewidth_upperbound;
  bool kellywidth_upperbound;
  bool sccs;
  bool alternation_depth_cks;
  bool alternation_depth;
  size_t max_vertices_for_expensive_checks;

  report_options(bool all=false)
    : general_graph_info(all),
      bfs_info(all),
      dfs_info(all),
      diameter(all),
      girth(all),
      diamonds(all),
      neighbourhoods(all),
      neighbourhoods_upto(3),
      treewidth_lowerbound(all),
      treewidth_upperbound(all),
      kellywidth_upperbound(all),
      alternation_depth_cks(all),
      alternation_depth(all),
      max_vertices_for_expensive_checks(std::numeric_limits<size_t>::max())
  {}
};

inline
void report(const parity_game_t& pg, YAML::Emitter& out, const report_options options = report_options())
{
  typedef typename boost::graph_traits<parity_game_t>::vertices_size_type vertex_size_t;

  out << YAML::BeginMap;

  if(options.general_graph_info)
  {
    out << YAML::Key << "Graph"
        << YAML::Value
        << YAML::BeginMap
          << YAML::Key << "Number of vertices"
          << YAML::Value << boost::num_vertices(pg)
          << YAML::Key << "Number of edges"
          << YAML::Value << boost::num_edges(pg)
          << YAML::Key << "Number of even vertices"
          << YAML::Value << num_even_vertices(pg)
          << YAML::Key << "Number of odd vertices"
          << YAML::Value << num_odd_vertices(pg)
          << YAML::Key << "Number of priorities"
          << YAML::Value << priorities(pg).size()
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
             << YAML::EndMap
        << YAML::EndMap;
  }

  if(options.bfs_info)
  {
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
          << YAML::Value << bfs_back_level_edges;


    if(boost::num_vertices(pg) <= options.max_vertices_for_expensive_checks)
    {
      std::vector<vertex_size_t> queue_sizes = bfs_queue_sizes(pg);
      out << YAML::Key << "Max queue"
          << YAML::Value << *std::max_element(queue_sizes.begin(), queue_sizes.end())
          << YAML::Key << "Queue sizes"
          << YAML::Value << queue_sizes;
    }
    else
    {
      out << YAML::Key << "Max queue"
          << YAML::Value << bfs_max_queue_size(pg);
    }

    out << YAML::EndMap;
  }

  if(options.dfs_info)
  {
    out << YAML::Key << "DFS"
        << YAML::Value
        << YAML::BeginMap;

    if(boost::num_vertices(pg) <= options.max_vertices_for_expensive_checks)
    {
      std::vector<vertex_size_t> stack_sizes = dfs_stack_sizes(pg);
      out << YAML::Key << "Max stack"
          << YAML::Value << *std::max_element(stack_sizes.begin(), stack_sizes.end())
          << YAML::Key << "Stack sizes"
          << YAML::Value << stack_sizes;
    }
    else
    {
      out << YAML::Key << "Max stack"
          << YAML::Value << dfs_max_stack_size(pg);
    }

    out << YAML::EndMap;

  }

  if(options.diameter)
  {
    out << YAML::Key << "Diameter"
        << YAML::Value << std::to_string(diameter(pg));
  }

  if(options.girth)
  {
    out << YAML::Key << "Girth"
        << YAML::Value << std::to_string(girth(pg));
  }

  if(options.diamonds)
  {
    diamond_count_t diamonds = diamond_count(pg);
    out << YAML::Key << "Diamonds"
        << YAML::Value
        << YAML::BeginMap
        << YAML::Key << "Total"
        << YAML::Value << diamonds.all
        << YAML::Key << "Even"
        << YAML::Value << diamonds.even
        << YAML::Key << "Odd"
        << YAML::Value << diamonds.odd
        << YAML::EndMap;
  }

  if(options.neighbourhoods)
  {
    std::vector<neighbourhood_result> neighbourhoods = accumulated_upto_kneighbourhood(pg, options.neighbourhoods_upto);
    out << YAML::Key << "Neighbourhood"
        << YAML::Value
        << YAML::BeginMap;
    for(size_t i = 1; i <= options.neighbourhoods_upto; ++i)
    {
      out << YAML::Key << i
          << YAML::Value
          << YAML::BeginMap
          << YAML::Key << "min" << YAML::Value << neighbourhoods[i].min
          << YAML::Key << "max" << YAML::Value << neighbourhoods[i].max
          << YAML::Key << "avg" << YAML::Value << static_cast<double>(neighbourhoods[i].sum)/static_cast<double>(boost::num_vertices(pg))
          << YAML::EndMap;
    }
    out << YAML::EndMap;
  }

  if(options.treewidth_lowerbound || options.treewidth_upperbound)
  {
    undirected_parity_game_t undirected_pg;
    boost::copy_graph(pg, undirected_pg);

    if(options.treewidth_lowerbound)
    {
      out << YAML::Key << "Treewidth (Lower bound)"
          << YAML::Value << minor_min_width(undirected_pg);
    }
    if(options.treewidth_upperbound)
    {
      out << YAML::Key << "Treewidth (Upper bound)"
          << YAML::Value << greedy_degree(undirected_pg);
    }
  }

  if(options.kellywidth_upperbound)
  {
    out << YAML::Key << "Kelly-width (Upper bound)"
        << YAML::Value << elimination_ordering(pg);
  }

  if(options.sccs)
  {
    cpplog(cpplogging::verbose) << "Computing SCCs" << std::endl;
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
  }

  if(options.alternation_depth_cks)
  {
    out << YAML::Key << "Alternation depth [CKS93]" << YAML::Value << alternation_depth(pg);
  }
  if(options.alternation_depth)
  {
    out << YAML::Key << "Alternation depth (priority ordering)" << YAML::Value << alternation_depth_priority_sorting(pg);
  }

  out << YAML::EndMap;
}

#endif // REPORT_H
