#ifndef _INTERPRET2_H
#define _INTERPRET2_H

#define	MAXANGLE	40
#define	MAXSCALE	100
#define	TWO_PI	 6.2831853
#define	STACK_SIZE 80
#define	LEFT		2
#define	TOP		42
#define	RIGHT		510
#define	BOTTOM		340

#define SQUARE 0
#define BZP 1
#define BZP_TRIANGLE 2

#include "define_float.h"

struct TURTLE {
	triplet	location;
	triplet	direction[3];
};


struct PIXEL {
	short int	h,v;
};

typedef struct PIXEL PIXEL;

struct BOX {
	double xmin,xmax;
	double ymin,ymax;
};

typedef struct BOX BOX;
typedef struct TURTLE TURTLE;

typedef struct{
	int	patchFormat;
	int	relative;
	int	debug;
	int	triangulate;
	int	normal_flag;
	int	local_coords_flag;
	int	diana;
	int	noPrint;
}FlagBag;
/* bag of flags for plant form output options */

#endif /* _INTERPRET2_H */
