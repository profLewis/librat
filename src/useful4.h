/* Miscellaneous useful subroutines and declarations */

#include "define_float.h"

#ifndef USEFUL
#define USEFUL

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define ABS(X) ((X)<0?(-(X)):(X))
#define AVERAGE(X,Y) ((X+Y)/2.0)

	extern DOUBLE drand48();
	extern void srand48();

	extern INT find_switch();
	extern INT find_string();
#endif
#define	PUTOUT fprintf(stdout," %s",argv[i])
#define DTOR(alpha)     ((alpha)*M_PI/180.0)
#define RTOD(alpha)     ((alpha)*180.0/M_PI)
#define ZERO_MOD {0.0,0.0,0.0,0.0}
#define	FATAL		1
#define	NON_FATAL 	0

struct	touch_point {
	FLOAT tpx,tpz;
};
typedef struct {
        FLOAT   r1,r2;          	/* outer/inner radius */
        FLOAT   d1,l1;            	/* leaf,stem lengths*/
        FLOAT   alpha1;           	/* leaf inclination */
        FLOAT   alpha2;          	/* wrap around angle */
        FLOAT   rotate;         	/* rotation angle */
        INT     no_of_lines;    	/* no_of_lines of representation */
        INT     no_of_steps;    	/* no of poINTs per line */
	INT	nos,nosl;		/* stem params */
} Wheat_params;
typedef struct {
/* absolute parameters */
	FLOAT	r1;			/* stem radius */
	FLOAT	r2;			/* stem length */
	FLOAT	a_leaf_width;		/* leaf width at l = 0 */
	FLOAT	a_leaf_length;		/* leaf length */	
	FLOAT	leaf_wrap;		/* leaf wrap */	
	FLOAT	leaf_sheath_length;	/* leaf sheath length */
	FLOAT	stem_node_radius;	/* stem node radius */
/* functional representations */
	FLOAT	f_leaf_inclination[5];	/* leaf inclination parameters */	
	FLOAT	f_leaf_width[5];	/* leaf width parameters */	
	FLOAT	f_leaf_curvature[5];	/* leaf curvature parameters */	
	FLOAT	f_leaf_azimuth[5];	/* leaf azimuth parameters */	
	FLOAT	f_leaf_twist[5];	/* leaf twist parameters */
/* representation modes */	
	char	mode_leaf_inclination;	/* leaf inclination parameters */	
	char	mode_leaf_width;	/* leaf width parameters */	
	char	mode_leaf_curvature;	/* leaf curvature parameters */	
	char	mode_leaf_azimuth;	/* leaf azimuth parameters */	
	char	mode_leaf_twist;	/* leaf twist parameters */
/* no of discrete steps for representation */
        INT     no_of_lines;    	/* no_of_lines of representation */
        INT     no_of_steps;    	/* no of poINTs per line */
	INT	nos,nosl;		/* stem params */
} plant_params;
/*
**	triplet (vector)
*/
typedef struct {
	FLOAT	x,y,z;
} triplet;
/*
**	pair (vector)
*/
typedef struct {
	FLOAT	x,y;
} pair;
typedef struct {
	DOUBLE	x,y,z;
} Triplet;
/*
**	pair (vector)
*/
typedef struct {
	DOUBLE	x,y;
} Pair;
typedef struct {
	DOUBLE	data[3][3];
} Matrix3;
/*
**	pair (vector)
*/
typedef struct {
	INT	x,y;
} ipair;
/*
**	pair (matrix)
*/
typedef struct {
	pair	column[2];
} matrix_2D;
typedef struct {
	Pair	column[2];
} Matrix_2D;
/*
**	triangle (2D)
*/
typedef struct {
	pair	vertex[3];
} triangle;

typedef struct {
	Pair	vertex[3];
} Triangle;
/*
**	matrix3 (3x3 matrix structure)
*/
typedef struct {
	FLOAT	data[3][3];
} matrix3;
/*
**	image descriptor
*/
typedef struct {
	FLOAT	ox,oy,sx,sy,gx,gy;
	INT	row,col;
} image_descriptor;
/*
**
*/
typedef struct {
	char	*b;
	INT	*i;
} char_int;

typedef triplet TRIPLET;
typedef ipair IPAIR;
typedef pair PAIR;
typedef Matrix3 MATRIX3;
