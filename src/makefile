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
OBJ = objects.${ARCH}

BPMS = /Users/plewis/librat
LIB = ${BPMS}/lib
BIN = ${BPMS}/bin

CFLAGS = -O
CC = gcc
DYN = gcc -dy -lc -lm -undefined dynamic_lookup
OPT = -fPIC

CFLAGS += -I. -DVERBOSE -DDOUBLEDEF -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS += -DMATCHK -D${ARCH} -D_NO_NAG

PRATOBJ = rpv.o cylinderTess.o sphereTess.o ratFront.o disk.o dummy.o dem.o bbox.o facet.o camera.o intersect_objects.o random.o plane.o reflectance.o volumeRayTrace.o sphere.o cylinder.o ellipse.o spheroid.o sky.o sensor_rsr.o files.o mtllib.o bilinear.o dumper.o useful.o prat_wavefront_read.o images.o start.o

ANCOBJS = error.o filelib.o hiplib.o envilib.o imagelib.o libhipl.o r250.o randlcg.o 4D_vectors.o vectors2.o 2D_vectors.o 2D_vectors_double.o 3D_vectors_double.o  allocate.o matrix.o fratP.o matherr.o

STARTOBJS = ${START}.o

all:		${LIBRAT}.dll 


sick:
		@echo "stick your fingers down your throat"

## windows shared object
## g++ -shared -o example_dll.dll example_dll.o -Wl,--out-implib,libexample_dll.a
##
${LIBRAT}.dll:	${PRATOBJ} ${ANCOBJS} 
		$(DYN) -o ${LIBRAT}.dll -Wl,--out-implib,${LIBRAT}.a ${PRATOBJ} ${ANCOBJS}

##
## sensible (unix-like) shared objects
##
${LIBRAT}.so:	${PRATOBJ} ${ANCOBJS} 
		$(DYN) -o ${LIBRAT}.so ${PRATOBJ} ${ANCOBJS}


${START}:	${STARTOBJS} ${LIBRAT}.so
		@mkdir -p ${BIN}/${ARCH}
		$(CC) $(CFLAGS) ${OPT} -L. -o ${BIN}/${ARCH}/$@  ${STARTOBJS} -lratlib -lm -lc

# object building


%.o:		%.c
		$(CC) ${OPT} -I. -DDOUBLEDEF  -D${ARCH} ${CFLAGS} -o $@ -c $<

clean:
		rm -f *% *.o ${PRATOBJ} ${ANCOBJS} ${STARTOBJS} ${LIBRAT}.so




