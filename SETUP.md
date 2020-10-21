
<p align="center">
  <a href="https://github.com/profLewis/librat">
    <img src="http://www2.geog.ucl.ac.uk/~plewis/images/millet/millet.gif" alt="Millet the movie" width="72" height="72">
  </a>
</p>
<h2 align="center">librat 🐀</h2>

## Setup

To set the library up on your computer, you need to configure and compile the code. As explained in more detail below, this involves typing:

    ./configure; make clean all test install env
    
at the shell prompt. Press the `<return>` key to execute the commands. The semicolon `;` allows us to put multiple commands on the same line. We could otherwise type them on seperate lines. 


<h4 align="center">📲Configuration</h4>



To configure this library on your computer, in a unix shell type:

    ./configure

Press the `<return>` key to execute the command. The configure script sets up the environment for your computer. The reason we type `./` in front of it, it to tell the unix interpreter that the script `configure` is in the **current working directory** which is indicated by `.`. It does this by testing that various options work on your particular setup. The range of options we have should work with most `*nix` setups. Specifically, [`configure`](configure)  sets variables from [`config.in`](config.in):

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


<h4 align="center">⏳ Compilation</h4>


To compile and test the library (after running [`./configure`)](configure), type:

    make clean all test
    
Press the `<return>` key to execute the command. In fact, if you forget to run [`./configure`)](configure), this will run it for you, via the file [`makefile`](makefile). The command `make clean` clears out any previously compiled code. Then `make all` compiles the library `${BPMS}/lib/${ARCH}/libratlib.so` (so, you refer to this as `-L ${BPMS}/lib/${ARCH} -lratlib`. It also compiles a `C` language interface to the library, `${BPMS}/bin/${ARCH}/RATstart`. `make test` runs a test on the `RATstart` code, comparing a simple ray tracing outout to a reference one.

To install the library:

    make install

You will have generated files in

* [`bin/${ARCH}`](bin/x86_64) : executable -- `RATstart`
* [`lib/${ARCH}`](lib/x86_64) : shared object library -- `libratlib.so`

Notice that these links ⛓ (`bin/x86_64`, `lib/x86_64`)  don't necessarily exist on the github repository: they are user-generated

Finally, `make env` runs the script [`bin/RATsetup.sh`](bin/RATsetup.sh) and sets appropriate default values for all required BPMS environment variables. These are set in:

* [`~/.bpms_profile`](~/.bpms_profile) : environment

which is sourced from your shell initialisation script (e.g. `~/.bashrc` or `~/.zshrc`).


