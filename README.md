pginfo
======

Tool to collect structural information from parity games.

Usage
-----

    pginfo [OPTION]... [INFILE [OUTFILE]]
    
where INFILE contains the parity game from which information needs to be collected in PGSolver format, and OUTFILE is the name of a file to which the structural information will be written in YAML format.

Some of the structural information is hard to compute (quadratic complexity or worse). The following options are provided to skip expensive computations for large inputs:

* `-mNUM,--max-vertices-for-expensive=NUM` expensive computations are only performed if the input has at most `NUM` vertices.
* `-nNUM,--neighbourhood-upto=NUM` the sizes of the neighbourhoods are computed upto and including the `NUM`-neighbourhood.
