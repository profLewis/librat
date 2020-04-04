# librat
librat

Installation
============

To set this up, in a unix shell type:

    ./configure
    make clean all test

The configure script sets variables from config.in:

    set CCS = ("gcc" "cc")
    set CFLAGSS = ("-I." "-fPIC")
    set DYNS = ("ld -G" "libtool  -L.. -lc -dynamic -undefined dynamic_lookup")
    set MAKES = ("gmake" "make")
    set OPTS = ("-g")
    # specify a temporary directory
    set TMP = /tmp
    
These are the options considered in configure. For example, we first try a compiler `gcc` and if that fails, try `cc`. We try ther dynamic linker `ld -G` and if that fails, `libtool  -L.. -lc -dynamic -undefined dynamic_lookup` etc. Note that the configutaion will settle on the first option in the list that passes some internal tests (follow through the script `configure`).
