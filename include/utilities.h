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

#include <istream>
#include <fstream>

#ifndef UTILITIES_H
#define UTILITIES_H

// for debugging
template <typename T>
std::string print(const std::multiset<T>& s)
{
  std::stringstream ss;
  for(typename std::multiset<T>::const_iterator i = s.begin(); i != s.end(); ++ i)
  {
    if(i != s.begin()) ss << ", ";
    ss << *i;
  }
  return ss.str();
}

template<typename Container>
size_t count_elements_occurring_exactly_n_times(Container c, size_t n = 1)
{
  std::sort(c.begin(), c.end());

  size_t result = 0;
  auto i = c.begin();
  size_t prev = *(i++);
  size_t count = 1;
  if(i == c.end())
    return 1;
  for(; i != c.end(); ++i)
  {
    if(prev == *i)
      ++count;
    else
    {
      if(count == 1)
        ++result;
      count = 1;
      prev = *i;
    }
  }
  if(count == n) ++result;
  return result;
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

inline
std::istream&
open_input(std::string& input_filename, std::ifstream& ifs)
{
  if(input_filename.empty())
  {
    return std::cin;
  }
  else
  {
    ifs.open(input_filename.c_str());
    return ifs;
  }
}

#endif // UTILITIES_H
