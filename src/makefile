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
OBJ = objects.${ARCH}
LIBRARY = ${BPMS}/lib

CFLAGS += -I. -DMMAP  -DVERBOSE -DDOUBLEDEF -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64



all:
		${MAKE} "BPMS=${BPMS}" liberr libimage hiplib librand libvect liballoc allhips libmatrix libimage librat libratlib

sick:		
		@echo "stick your fingers down your throat"

lib:
		${MAKE} "BPMS=${BPMS}" all

liberr:		makefile ${OBJ}/error.o
		@echo "archiving error library"
		ar rcv ${LIBRARY}/liberr_${ARCH}.a ${OBJ}/error.o
		ranlib ${LIBRARY}/liberr_${ARCH}.a
		$(DYN) -o ${LIBRARY}/liberr_${ARCH}.so ${OBJ}/error.o

libimage:	makefile ${OBJ}/filelib.o ${OBJ}/hiplib.o ${OBJ}/envilib.o ${OBJ}/imagelib.o
		@echo "archiving image library"
		ar rcv ${LIBRARY}/libimage_${ARCH}.a ${OBJ}/filelib.o ${OBJ}/hiplib.o ${OBJ}/envilib.o ${OBJ}/imagelib.o
		$(DYN) -o ${LIBRARY}/libimage_${ARCH}.so ${OBJ}/filelib.o ${OBJ}/hiplib.o ${OBJ}/envilib.o ${OBJ}/imagelib.o 
		ranlib ${LIBRARY}/libimage_${ARCH}.a

libhipl:	${OBJ}/libhipl.o
		ar rcv ${LIBRARY}/libhipl_${ARCH}.a ${OBJ}/libhipl.o
		ranlib ${LIBRARY}/libhipl_${ARCH}.a
		$(DYN) -o ${LIBRARY}/libhipl_${ARCH}.so ${OBJ}/libhipl.o

librand:	makefile ${OBJ}/r250.o ${OBJ}/randlcg.o
		@echo "making random interface library"
		ar rcv ${LIBRARY}/librand_${ARCH}.a ${OBJ}/r250.o ${OBJ}/randlcg.o
		ranlib ${LIBRARY}/librand_${ARCH}.a
		$(DYN) -o ${LIBRARY}/librand_${ARCH}.so ${OBJ}/r250.o ${OBJ}/randlcg.o

libvect:	makefile ${OBJ}/4D_vectors.o ${OBJ}/vectors2.o ${OBJ}/2D_vectors.o ${OBJ}/2D_vectors_double.o ${OBJ}/3D_vectors_double.o
		@echo "archiving vector/matrix library"
		ar rcv ${LIBRARY}/libvect_${ARCH}.a ${OBJ}/2D_vectors_double.o ${OBJ}/4D_vectors.o ${OBJ}/3D_vectors_double.o ${OBJ}/vectors2.o ${OBJ}/2D_vectors.o
		ranlib ${LIBRARY}/libvect_${ARCH}.a
		$(DYN) -o ${LIBRARY}/libvect_${ARCH}.so ${OBJ}/2D_vectors_double.o ${OBJ}/4D_vectors.o ${OBJ}/3D_vectors_double.o ${OBJ}/vectors2.o ${OBJ}/2D_vectors.o


liballoc:	makefile ${OBJ}/allocate.o
		@echo "archiving memory allocation library"
		ar rcv ${LIBRARY}/liballoc_${ARCH}.a ${OBJ}/error.o ${OBJ}/allocate.o
		ranlib ${LIBRARY}/liballoc_${ARCH}.a
		$(DYN) -o ${LIBRARY}/liballoc_${ARCH}.so ${OBJ}/error.o ${OBJ}/allocate.o

libmatrix:	${OBJ}/matrix.o
		ar rcv ${LIBRARY}/libmatrix_${ARCH}.a ${OBJ}/matrix.o
		ranlib ${LIBRARY}/libmatrix_${ARCH}.a
		$(DYN) -o ${LIBRARY}/libmatrix_${ARCH}.so ${OBJ}/matrix.o


PRATOBJ = ${OBJ}/rpv.o ${OBJ}/cylinderTess.o ${OBJ}/sphereTess.o ${OBJ}/ratFront.o ${OBJ}/disk.o ${OBJ}/dummy.o ${OBJ}/dem.o ${OBJ}/bbox.o ${OBJ}/facet.o ${OBJ}/camera.o ${OBJ}/intersect_objects.o ${OBJ}/random.o ${OBJ}/plane.o ${OBJ}/reflectance.o ${OBJ}/volumeRayTrace.o ${OBJ}/sphere.o ${OBJ}/cylinder.o ${OBJ}/ellipse.o ${OBJ}/spheroid.o ${OBJ}/sky.o ${OBJ}/sensor_rsr.o ${OBJ}/files.o ${OBJ}/mtllib.o ${OBJ}/bilinear.o ${OBJ}/dumper.o
USEFUL = ${OBJ}/useful.o
WAVEFRONT = ${OBJ}/prat_wavefront_read.o
USRHIPL = ${OBJ}/images.o
USRLIBS = -lvect -lerr
CFLAGS += -L.. -DDOUBLEDEF -DMMAP  -DMATCHK -D${ARCH} -D_NO_NAG
NAG =

librat:		makefile ${OBJ}/fratP.o $(USRHIPL) $(PRATOBJ) $(USEFUL) $(WAVEFRONT) ${FORTRANSTUFF} ${OBJ}/matherr.o
		ar rcv ${LIBRARY}/librat_${ARCH}.a ${OBJ}/fratP.o $(USRHIPL) $(PRATOBJ) $(USEFUL) $(WAVEFRONT) ${FORTRANSTUFF} ${OBJ}/matherr.o
		ranlib ${LIBRARY}/librat_${ARCH}.a
		$(DYN) -o ${LIBRARY}/librat_${ARCH}.so ${OBJ}/fratP.o $(USRHIPL) $(PRATOBJ) $(USEFUL) $(WAVEFRONT) ${FORTRANSTUFF} ${OBJ}/matherr.o 


${OBJ}/%.o:	%.c
		mkdir -p ${OBJ}
		$(CC) -I. -DDOUBLEDEF -I/usr/5include -D${ARCH} ${CFLAGS} -I${LIBRARY} -o $@ -c $<


# object building

.c.o:		makefile hiplib.h  $<
		$(CC) ${CFLAGS} -D${SUNTYPE} -I. -c $< 

clean:
		rm -f *% *.o ${OBJ}/*.o libstart_${ARCH}.a


libratlib:	makefile
		@echo ""
		@echo "^Garchiving libratlib library"
		$(DYN) -o libratlib_${ARCH}.so hips/objects.x86_64/libhipl.o err/objects.${ARCH}/*.o alloc/objects.${ARCH}/*.o image/objects.${ARCH}/*.o vect/objects.${ARCH}/*.o  matrix/objects.${ARCH}/*.o r250c/objects.${ARCH}/*.o rat/objects.${ARCH}/*.o 





