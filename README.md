# pginfo

Tool to collect structural information from parity games.

## Installation

### Prerequisites

Compiling `pginfo` requires the following:

* A recent C++ compiler, supporting C++11 features, e.g. GCC 4.8 or newer, or Clang 3.5 or newer.
* [Boost](http://www.boost.org) (especially graph, algorith, shared_ptr, type_traits)
* [YAML-CPP](http://code.google.com/p/yaml-cpp/)
* [CMake](http://www.cmake.org) (2.8 or newer)

### Compiling

For compilation, assume that the source code has been checked out in /path/to/src. We will build the tool in /path/to/build, assuming that the build directory already exists.

First the build is configured using the following command:
  
    cd /path/to/build
    cmake /path/to/src -DCMAKE_INSTALL_PREFIX=<prefix>
    make
    make install
    
where you should substitute the prefix where you want to install the tool for `<prefix>`.

If you installed YAML-CPP in a non-standard path, it may not be detected by the CMake script, in that case you can append `-DYAMLCPP_DIR=/path/to/yaml` to the CMake command.

#### Advanced options

The tool uses a command line library and a logging library internally. By default, the CMake script tries to look for a pre-installed version of those libaries, and automatically falls back to a version which is included in the sourcetree if no such version can be found. Should you want to use your pre-installed version nevertheless, you can add `-DCPPCLI_DIR=/path/to/cppcli` or `-DCPPLOGGING_DIR=/path/to/cpplogging`, respectively, to the CMake line in the instructions above.

## Usage

    pginfo [OPTIONS]... [INFILE [OUTFILE]]
    
where INFILE contains the parity game from which information needs to be collected in PGSolver format, and OUTFILE is the name of a file to which the structural information will be written in YAML format. OPTIONS is a sequence options, containing for example the measures that need to be computed (multiple measures can be computed in one call to the executable).

### Options

The time it takes to compute the measures can be recorded and written to a file in YAML format:

* `--timings[=FILE]`     append timing measurements to `FILE`. Measurements are written to standard error if no `FILE` is provided

The measures that can be computed are controlled by the following options:

* `--all` compute all statistics about the graph. Overrules all other options
* `--ad`     compute alternation-depth using a sorting of priorities
* `--ad-cks` compute alternation-depth using the algorithm from [CKS93]
* `--bfs` compute information from BFS on the graph
* `--dfs` compute information from DFS on the graph
* `--diameter`           compute the diameter of the graph
* `--diamonds`           compute the number of diamonds in the graph
* `--girth`              compute the girth of the graph
* `--graph`              compute general information about the graph
* `--kellywidth-ub`      compute upperbound on Kelly-width
* `--sccs`               compute strongly connected components
* `--treewidth-lb`       compute lowerbound on treewidth
* `--treewidth-ub`       compute upperbound on treewidth

Some of the structural information is hard to compute (quadratic complexity or worse). The following options are provided to skip expensive computations for large inputs:

* `--max-for-expensive=NUM` for BFS and DFS do not records queue or stack sizes if the number of vertices exceeds `NUM`
* `--neighbourhoods=NUM` compute the sizes of the neighbourhoods up to and including `NUM`

