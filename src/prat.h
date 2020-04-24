/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

#ifndef PRAT_H
#define PRAT_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#ifndef __SUNPRO_C
#include <strings.h>
#endif
#include "hipl_format.h"
#ifdef __SUNPRO_C
#include <unistd.h>
#endif
#ifndef SEEK_SET
#define SEEK_SET        0       /* Set file pointer to "offset" */
#define SEEK_CUR        1       /* Set file pointer to current plus "offset" */
#define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include "r250.h"

#include "error.h"
#include "useful3.h"
#include "vectors2.h"
#include "4D_vectors.h"
#include "2D_vectors.h"
#include "wavefront.h"
#include "allocate.h"
#include "prat_objects.h"
#include "sensor_rsr.h"
#include "reflectance_functions.h"
#include "bags.h"
#include "reflectance.h"
#include "mtllib.h"

#include "camera.h"
#include "globals.h"
#include "element_types.h"
#include "vector_macros.h"
#include "sky.h"
#include "rat.h"
#include "ratFront.h"
#include "libhipl.h"
#include "imagelib.h"
#include "allocate.h"
#include "plane.h"
#include "disk.h"
#include "facet.h"
#include "bbox.h"
#include "spheroid.h"
#include "sphere.h"
#include "bilinear.h"
#include "ellipse.h"
#include "cylinder.h"
#include "dem.h"
#include "images.h"
#include "cylinderTess.h"

#include "volumeRayTrace.h"

#include "intersect_objects.h"

#include <stdio.h>

#define EXPONENTIAL(X) ((X)<(-300)?(0):(exp(X)))
#define FSIGN(a) ((a<0.0)?(-1):(1))
#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif
#define pi M_PI
#define	PI_OVER_2 M_PI_2
#define	PI_2 (2.*M_PI)

#define DIFFUSE 2

/* BBox tols. */
#define TTOL 0.001
#define BOXTOL 0.00001
#define RAY_TOL 0.01
#define UNITY_TOL  0.001

#ifdef __SUNPRO_C
	char *strchr();
#endif
#define	IP	1
#define	OP	2

#define	OLD	0
#define	NEW	1

int is_double(char *buffer,double *ptr); 


int writeInfo(PixelStats *pixelStats,FILE **datafile,FlagBag *flagbag,MaterialBag *materialbag,WavebandBag *wavebandbag);

/* if you pass (NULL,NULL to volumeRayTrace it resets the internal (static) stack counter */
#define RESET_VOLUMETRIC_STACKCOUNTER volumeRayTrace(NULL,NULL,NULL,NULL,0.,NULL)
#include "fratP.h"

#endif

