#
#
#
#       Makefile HIPS programs
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
LIBRARY = ${BPMS}/lib

CFLAGS = -I.
MAKE = make
WG = -L -R --create-dirs -o
WGET = /usr/bin/curl
CC = gcc
DYN = libtool -L.. -lc -dynamic -undefined dynamic_lookup
OPT = -g

CFLAGS += -I. -DMMAP  -DVERBOSE -DDOUBLEDEF -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS += -DMATCHK -D${ARCH} -D_NO_NAG

PRATOBJ = ${OBJ}/rpv.o ${OBJ}/cylinderTess.o ${OBJ}/sphereTess.o ${OBJ}/ratFront.o ${OBJ}/disk.o ${OBJ}/dummy.o ${OBJ}/dem.o ${OBJ}/bbox.o ${OBJ}/facet.o ${OBJ}/camera.o ${OBJ}/intersect_objects.o ${OBJ}/random.o ${OBJ}/plane.o ${OBJ}/reflectance.o ${OBJ}/volumeRayTrace.o ${OBJ}/sphere.o ${OBJ}/cylinder.o ${OBJ}/ellipse.o ${OBJ}/spheroid.o ${OBJ}/sky.o ${OBJ}/sensor_rsr.o ${OBJ}/files.o ${OBJ}/mtllib.o ${OBJ}/bilinear.o ${OBJ}/dumper.o ${OBJ}/useful.o ${OBJ}/prat_wavefront_read.o ${OBJ}/images.o

ANCOBJS = ${OBJ}/error.o ${OBJ}/filelib.o ${OBJ}/hiplib.o ${OBJ}/envilib.o ${OBJ}/imagelib.o ${OBJ}/libhipl.o ${OBJ}/r250.o ${OBJ}/randlcg.o ${OBJ}/4D_vectors.o ${OBJ}/vectors2.o ${OBJ}/2D_vectors.o ${OBJ}/2D_vectors_double.o ${OBJ}/3D_vectors_double.o  ${OBJ}/allocate.o ${OBJ}/matrix.o ${OBJ}/fratP.o ${OBJ}/matherr.o

STARTOBJS = ${OBJ}/${START}.o


all:		${LIBRAT}_${ARCH}.so ${START}

sick:		
		@echo "stick your fingers down your throat"

lib:		all


${LIBRAT}_${ARCH}.so:	${PRATOBJ} ${ANCOBJS} 
		$(DYN) -o ${BPMS}/lib/${LIBRAT}_${ARCH}.so ${PRATOBJ} ${ANCOBJS}


${START}:	${STARTOBJS} ${LIBRAT}_${ARCH}.so
		@mkdir -p ${BPMS}/bin/${ARCH}
		$(CC) $(CFLAGS) -L${BPMS}/lib -L. -L${LIBRARY} -o ${BPMS}/bin/${ARCH}/$@  ${STARTOBJS} -lratlib_${ARCH} -lm -lc

# object building

${OBJ}/%.o:	%.c
		@mkdir -p ${OBJ}
		$(CC) -I. -DDOUBLEDEF -I/usr/5include -D${ARCH} ${CFLAGS} -I${LIBRARY} -o $@ -c $<

clean:
		rm -f *% *.o ${PRATOBJ} ${ANCOBJS} ${STARTOBJS} ${BPMS}/lib/*_${ARCH}.so ${BPMS}/lib/*_${ARCH}.a






