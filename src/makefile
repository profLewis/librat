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
CC = gcc
DYN = libtool -dynamic
OPT = -fPIC
OS = x86_64
####LIBEXT = so

CFLAGS += -I. -DVERBOSE -DDOUBLEDEF -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS += -DMATCHK -D${ARCH} -D_NO_NAG

PRATOBJ = rpv.o cylinderTess.o sphereTess.o ratFront.o disk.o dummy.o dem.o bbox.o facet.o camera.o intersect_objects.o random.o plane.o reflectance.o volumeRayTrace.o sphere.o cylinder.o ellipse.o spheroid.o sky.o sensor_rsr.o files.o mtllib.o bilinear.o dumper.o useful.o prat_wavefront_read.o images.o start.o

ANCOBJS = error.o filelib.o hiplib.o envilib.o imagelib.o libhipl.o r250.o randlcg.o 4D_vectors.o vectors2.o 2D_vectors.o 2D_vectors_double.o 3D_vectors_double.o  allocate.o matrix.o fratP.o matherr.o

STARTOBJS = ${START}.o


ifeq ($(MSYSTEM),'MINGW64')
  LIBEXT='dll'
  LIBC='-lmsvcrt -lm'
else
  LIBEXT='so'
  LIBC='-lc -lm'
endif

ifeq ($(ImageOS),'win19')
  LIBEXT='dll'
else
  LIBEXT='so'
endif


all:		${LIBRAT}.${LIBEXT}
		


sick:
		@echo "stick your fingers down your throat"

## windows shared object
## g++ -shared -o example_dll.dll example_dll.o -Wl,--out-implib,libexample_dll.a
##
## sensible (unix-like) shared objects
## $(DYN) -o ${LIBRAT}.so ${PRATOBJ} ${ANCOBJS}
${LIBRAT}.${LIBEXT}:	${PRATOBJ} ${ANCOBJS} 



#ImageOS=macos1015, ubuntu18, win19

ifeq ($(LIBEXT),'dll')
	@echo "$(LIBEXT) indicates windows ... uh oh : ${OS} : $(ImageOS)"
	$(DYN) -o ${LIBRAT}.${LIBEXT} --shared -Wl,--out-implib,${LIBRAT}.a ${PRATOBJ} ${ANCOBJS} -lmsvcrt -lm
else
	@echo "making on OS $(OS) $(ImageOS)"
	$(DYN) -undefined dynamic_lookup -o ${LIBRAT}.${LIBEXT} ${PRATOBJ} ${ANCOBJS} -lc -lm
endif

${START}:	${STARTOBJS} ${LIBRAT}.${LIBEXT}
		@mkdir -p ${BIN}/${ARCH}
		$(CC) $(CFLAGS) ${OPT} -L. -o ${BIN}/${ARCH}/$@  ${STARTOBJS} -lratlib -lm -lc

# object building

%.o:		%.c
		$(CC) ${OPT} -I. -DDOUBLEDEF  -D${ARCH} ${CFLAGS} -o $@ -c $<

clean:
		rm -f *% *.o ${PRATOBJ} ${ANCOBJS} ${STARTOBJS} ${LIBRAT}.so




