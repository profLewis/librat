librat 🐀
------------
`librat` (🐁 actually called `ratlib` in the code) is a ray tracing library built for radiative trabnsfer simulations for remote sensing studies.

📲Installation
============

To set this up, in a unix shell type:

    ./configure
    make clean all test

The configure script sets variables from [`config.in`](config.in):

    set CCS = ("gcc" "cc")
    set CFLAGSS = ("-I." "-fPIC")
    set DYNS = ("ld -G" "libtool  -L.. -lc -dynamic -undefined dynamic_lookup")
    set MAKES = ("gmake" "make")
    set OPTS = ("-g")
    # specify a temporary directory
    set TMP = /tmp
    
These are the options considered in configure. For example, we first try a compiler `gcc` and if that fails, try `cc`. We try ther dynamic linker `ld -G` and if that fails, `libtool  -L.. -lc -dynamic -undefined dynamic_lookup` etc. Note that the configutaion will settle on the first option in the list that passes some internal tests (follow through the script [`configure`](configure)).

The configuation the sets e.g. (settings on `OS X x86_64`):

    ARCH = x86_64
    OBJ = objects.${ARCH}

    BPMS = /Users/plewis/librat
    LIB = ${BPMS}/lib
    BIN = ${BPMS}/bin

    CFLAGS = -I.
    MAKE = make
    WG = -L -R --create-dirs -o
    WGET = /usr/bin/curl
    CC = gcc
    DYN = libtool -L.. -lc -dynamic -undefined dynamic_lookup
    OPT = -g

in the file [`src/makefile`](src/makefile). Note that [`src/makefile`](src/makefile) is generate from the template [`src/Makefile.in`](src/Makefile.in).

⏳ Compilation
===========

To compile and test the library (after running [`./configure`)](configure), type:

    make clean all test
    
as above. In fact, if you forget to run [`./configure`)](configure), this will run it for you, via the file [`makefile`](makefile). The command `make clean` clears out any previously compiled code. Then `make all` compiles the library `${BPMS}/lib/${ARCH}/libratlib.so` (so, you refer to this as `-L ${BPMS}/lib/${ARCH} -lratlib`. It also compiles a `C` language interface to the library, `${BPMS}/bin/${ARCH}/start`. `make test` runs a test on the `start` code, comparing a simple ray tracing outout to a reference one.

To sum up, after compilation, you will have generated files in

* [`bin/${ARCH}`](bin/x86_64) : executable -- `start`
* [`lib/${ARCH}`](lib/x86_64) : shared object library -- `libratlib.so`

Notice that these links ⛓ (`bin/x86_64`, `lib/x86_64`)  don't exist on the github repository: they are user-generated!

