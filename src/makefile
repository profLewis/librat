#
#
#
#       Makefile librat
#
#       Lewis, RSU, UCL Geography, 1993
#
#
#
#       build executables 

START = start 
LIBRAT = libratlib

ARCH = x86_64
BPMS = /Users/plewis/librat
LIB = ${BPMS}/lib
BIN = ${BPMS}/bin

CFLAGS = -O
CC = clang
DYN = libtool -dynamic
OPT = -fPIC
OS = x86_64
ASYSTEM=Darwin
####LIBEXT = so

CFLAGS += -I. -DVERBOSE -DDOUBLEDEF -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS += -DMATCHK -D${ARCH} -D_NO_NAG

PRATOBJ = rpv.o cylinderTess.o sphereTess.o ratFront.o disk.o dummy.o dem.o bbox.o facet.o camera.o intersect_objects.o random.o plane.o reflectance.o volumeRayTrace.o sphere.o cylinder.o ellipse.o spheroid.o sky.o sensor_rsr.o files.o mtllib.o bilinear.o dumper.o useful.o prat_wavefront_read.o images.o start.o

ANCOBJS = error.o filelib.o hiplib.o envilib.o imagelib.o libhipl.o r250.o randlcg.o 4D_vectors.o vectors2.o 2D_vectors.o 2D_vectors_double.o 3D_vectors_double.o  allocate.o matrix.o fratP.o matherr.o

STARTOBJS = ${START}.o


ifeq ($(MSYSTEM),MINGW64)
  LIBEXT=dll
else
  LIBEXT=so
endif

ifeq ($(ImageOS),win19)
  LIBEXT=dll
else
  LIBEXT=so
endif

ifeq ($(ASYSTEM),Darwin)
  UND=-undefined dynamic_lookup
else
  UND=
endif

all:		${LIBRAT}.${LIBEXT}
		


sick:
		@echo "stick your fingers down your throat"

## windows shared object
## g++ -shared -o example_dll.dll example_dll.o -Wl,--out-implib,libexample_dll.a
##
## sensible (unix-like) shared objects
## $(DYN) -o ${LIBRAT}.so ${PRATOBJ} ${ANCOBJS}



#ImageOS=macos1015, ubuntu18, win19
# -Wl,--out-implib,${LIBRAT}.a
# -undefined dynamic_lookup
ifeq ($(LIBEXT),dll)
  CMD=ld -o ${LIBRAT}.${LIBEXT} ${PRATOBJ} ${ANCOBJS} -lmsvcrt -lm
else
  CMD=$(CC) ${UND} -o ${LIBRAT}.${LIBEXT} ${PRATOBJ} ${ANCOBJS} -lc -lm
endif


${LIBRAT}.${LIBEXT}:	${PRATOBJ} ${ANCOBJS}
		$(CMD)

${START}:	${STARTOBJS} ${LIBRAT}.${LIBEXT}
		@mkdir -p ${BIN}/${ARCH}
		$(CC) $(CFLAGS) ${OPT} -L. -o ${BIN}/${ARCH}/$@  ${STARTOBJS} -lratlib -lm -lc

# object building

%.o:		%.c
		$(CC) ${OPT} -I. -DDOUBLEDEF  -D${ARCH} ${CFLAGS} -o $@ -c $<

clean:
		rm -f *% *.o ${PRATOBJ} ${ANCOBJS} ${STARTOBJS} ${LIBRAT}.$(LIBEXT)




