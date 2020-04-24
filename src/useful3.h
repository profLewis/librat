/* Miscellaneous useful subroutines and declarations */

#ifndef USEFUL
#define USEFUL

#ifdef PFAT
#include <GL/gl.h>
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#include <sys/param.h>

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#define MMAX(X,Y) MAX(X,Y)
#define MMIN(X,Y) MIN(X,Y)


#define ABS(X) ((X)<0?(-(X)):(X))
#define AVERAGE(X,Y) ((X+Y)/2.0)

#define	PUTOUT fprintf(stdout," %s",argv[i])
#define OUTPUT(strng) fprintf(stderr," %s",strng)
#define DTOR(alpha)     ((alpha)*M_PI/180.0)
#define RTOD(alpha)     ((alpha)*180.0/M_PI)
#define	FATAL		1
#define	NON_FATAL 	0

/*
**	triplet (vector)
*/
typedef struct {
	double	x,y,z;
} triplet;
/*
**	pair (vector)
*/

/*
**	matrix3 (3x3 matrix structure)
*/
typedef struct {
	double	data[3][3];
} matrix3;


/*
**      matrix4 (4x4 matrix structure)
*/
typedef struct {
        double  data[4][4];
} matrix4;

/*
**	vector4 (4-D vector structure)
*/
typedef struct {
	double	v[4];
} vector4;
/*
**	image descriptor
*/
typedef struct {
	double	ox,oy,sx,sy,gx,gy;
	int	row,col;
} image_descriptor;
/*
**
*/
typedef struct {
	char	*b;
	int	*i;
} char_int;
typedef struct {
	char	**flag;
	char	*Type;
	char	*no_of_parameters;
}Data_type;
/*
**	pair (vector)
*/
typedef struct {
	double	x,y;
}pair;
/*
**	pair (vector)
*/
typedef struct {
	int	x,y;
} ipair;
typedef struct {
	pair	column[2];
} matrix_2D;
typedef struct {
	pair	vertex[3];
} triangle;




#endif
