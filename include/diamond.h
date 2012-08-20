#ifndef DIAMOND_H
#define DIAMOND_H

#include <set>
#include <algorithm>
#include "graph.h"

#include "mcrl2/utilities/logger.h"
#include "mcrl2/utilities/progress_meter.h"
#include "utilities.h"

namespace graph
{

using mcrl2::log::verbose;
using mcrl2::log::status;
using mcrl2::log::debug;

/*
   @brief Count the number of 2-diamonds in the graph.
 */
template <typename graph_t>
size_t diamond_count(graph_t& g)
{
  mCRL2log(verbose) << "Computing the number of diamonds in the graph" << std::endl;
  mcrl2::utilities::progress_meter progress(g.size());
  size_t result = 0;
  for(size_t ui = 0; ui < g.size(); ++ui)
  {
    progress.step();
    std::vector<graph::VertexIndex> u_successors(g.vertex(ui).out.begin(), g.vertex(ui).out.end());

    mCRL2log(debug) << "    " << u_successors.size() << " successors" << std::endl;
    // no combinations to be considered
    if(u_successors.size() <= 1)
    {
      continue;
    }

    for(size_t first = 0; first < u_successors.size() - 1; ++first)
    {
      for(size_t second = 1; second < u_successors.size(); ++second)
      {
        graph::VertexIndex v(u_successors[first]);
        graph::VertexIndex w(u_successors[second]);
        std::set<graph::VertexIndex>::const_iterator vi = g.vertex(v).out.begin();
        std::set<graph::VertexIndex>::const_iterator wi = g.vertex(w).out.begin();
        mCRL2log(debug) << "      Checking diamond property for " << ui << " with " << v << " and " << w << std::endl;
        while(vi != g.vertex(v).out.end() && wi != g.vertex(w).out.end())
        {
          if(*vi < *wi)
          {
            ++vi;
          }
          else if (*wi < *vi)
          {
            ++ wi;
          }
          else
          {
            mCRL2log(debug) << "      Diamond completed by " << *wi << std::endl;
            ++result; ++vi; ++wi;
          }
        }
      }
    }
    mCRL2log(debug) << "    " << result << " diamonds after this iteration" << std::endl;
  }
  mCRL2log(verbose) << std::endl;

  return result;
}

} // namespace graph

#endif // DIAMOND_H
