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
