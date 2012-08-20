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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <ostream>
#include <map>
#include <vector>

template <typename key_t, typename value_t>
void print_map(std::ostream& os, const std::map<key_t, value_t>& m, size_t indent = 0)
{
  os.width(3);

  for(typename std::map<key_t, value_t>::const_iterator i = m.begin(); i != m.end(); ++i)
  {
    os << std::string(indent, ' ') << i->first << ": " << i->second << std::endl;
  }
}

template <typename value_t>
void print_vector(std::ostream& os, const std::vector<value_t>& v)
{
  for(typename std::vector<value_t>::const_iterator i = v.begin(); i != v.end(); ++i)
  {
    if(i != v.begin())
    {
      os << ", ";
    }
    os << *i;
  }
}

template<typename key_t>
void increment(std::map<key_t, size_t>& m, const key_t& k)
{
  if(m.find(k) == m.end())
  {
    m[k] = 1;
  }
  else
  {
    ++m[k];
  }
}

#endif // UTILITIES_H
