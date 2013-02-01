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

#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include <algorithm>
#include <limits>
#include "cpplogging/logger.h"
#include "cpplogging/progress_meter.h"

template<typename Graph>
inline
std::vector<typename boost::graph_traits<Graph>::vertices_size_type>
upto_kneighbourhood(typename Graph::vertex_descriptor v, const Graph& g, const size_t k)
{
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_size_t;
  typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
  std::set<vertex_t> visited;
  std::vector<std::pair<vertex_t, size_t> > queue;
  queue.push_back(std::make_pair(v, 0));
  visited.insert(v);
  size_t current = 0; // current position in the queue
  std::vector<vertex_size_t> result(k+1,0);
  if(k == 0) return result;

  while(current < queue.size())
  {
    if(queue[current].second < k)
    {
      typename boost::graph_traits< Graph >::adjacency_iterator ai,aend;
      for(boost::tie(ai, aend) = boost::adjacent_vertices(queue[current].first, g); ai != aend; ++ai)
      {
        if(visited.find(*ai) == visited.end())
        {
          visited.insert(*ai);
          queue.push_back(std::make_pair(*ai, queue[current].second + 1));
        }
      }
    }
    ++current;
  }

  for(size_t i = 0; i <= k; ++i)
  {
    result[i] = std::count_if(queue.begin(), queue.end(), [&i](std::pair<vertex_size_t, size_t> x) { return 0 < x.second && x.second <= i; });
  }
  return result;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
kneighbourhood(typename Graph::vertex_descriptor v, const Graph& g, const size_t k)
{
  return upto_kneighbourhood(v,g,k)[k];
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
neighbourhood(typename Graph::vertex_descriptor v, const Graph& g)
{
  return upto_kneighbourhood(v, g, 1)[1];
}

struct neighbourhood_result
{
  size_t min;
  size_t max;
  size_t sum;

  neighbourhood_result()
    : min(std::numeric_limits<size_t>::max()), max(0), sum(0)
  {}
};

template<typename Graph>
inline
std::vector<neighbourhood_result>
accumulated_upto_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing accumulated neighbourhood information" << std::endl;
  cpplogging::progress_meter progress(boost::num_vertices(g));

  std::vector<neighbourhood_result> result(k+1, neighbourhood_result());
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    progress.step();

    std::vector<typename boost::graph_traits<Graph>::vertices_size_type> tmp = upto_kneighbourhood(*i, g, k);
    for(size_t i = 0; i <= k; ++i)
    {
      result[i].sum += tmp[i];
      result[i].min = std::min(result[i].min, tmp[i]);
      result[i].max = std::max(result[i].max, tmp[i]);
    }
  }

  return result;
}


template<typename Graph>
inline
double avg_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing average " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type sum = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    sum += kneighbourhood(*i, g, k);
  }
  return static_cast<double>(sum)/static_cast<double>(boost::num_vertices(g));
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
max_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing maximum " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type result = 0;
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::max(result, kneighbourhood(*i, g, k));
  }
  return result;
}

template<typename Graph>
inline
typename boost::graph_traits<Graph>::vertices_size_type
min_kneighbourhood(Graph& g, const size_t k)
{
  cpplog(cpplogging::verbose) << "Computing minimum " << k << " neighbourhood" << std::endl;
  typename boost::graph_traits<Graph>::vertices_size_type result = std::numeric_limits<size_t>::max();
  typename boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = vertices(g); i != end; ++i)
  {
    result = std::min(result, kneighbourhood(*i, g, k));
  }
  return result;
}

#endif // NEIGHBOURHOOD_H
