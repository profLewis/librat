/*% cc -g sphere.c -o sphere -lm
 *
 * sphere - generate a triangle mesh approximating a sphere by
 *  recursive subdivision. First approximation is an octahedron;
 *  each level of refinement increases the number of triangles by
 *  a factor of 4.
 * Level 3 (128 triangles) is a good tradeoff if gouraud
 *  shading is used to render the database.
 *
 * Usage: sphere [level] [-p] [-c]
 *	level is an integer >= 1 setting the recursion level (default 1).
 *	-p causes generation of a PPHIGS format ASCII archive
 *	    instead of the default generic output format.
 *	-c causes triangles to be generated with vertices in counterclockwise
 *	    order as viewed from the outside in a RHS coordinate system.
 *	    The default is clockwise order.
 *
 *  The subroutines print_object() and print_triangle() should
 *  be changed to generate whatever the desired database format is.
 *
 * Jon Leech (leech@cs.unc.edu) 3/24/89
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    double  x, y, z;
} sphere_point;

typedef struct {
    sphere_point     pt[3];	/* Vertices of triangle */
    double    area;	/* Unused; might be used for adaptive subdivision */
} sphere_triangle;

typedef struct {
    int       npoly;	/* # of triangles in object */
    sphere_triangle *poly;	/* Triangles */
} sphere_object;

/*  Six equidistant points lying on the unit sphere */
#define XPLUS {  1,  0,  0 }    /*  X */
#define XMIN  { -1,  0,  0 }    /* -X */
#define YPLUS {  0,  1,  0 }    /*  Y */
#define YMIN  {  0, -1,  0 }    /* -Y */
#define ZPLUS {  0,  0,  1 }    /*  Z */
#define ZMIN  {  0,  0, -1 }    /* -Z */

/* Vertices of a unit octahedron */
sphere_triangle sphere_octahedron[] = {
    { { XPLUS, ZPLUS, YPLUS }, 0.0 },
    { { YPLUS, ZPLUS, XMIN  }, 0.0 },
    { { XMIN , ZPLUS, YMIN  }, 0.0 },
    { { YMIN , ZPLUS, XPLUS }, 0.0 },
    { { XPLUS, YPLUS, ZMIN  }, 0.0 },
    { { YPLUS, XMIN , ZMIN  }, 0.0 },
    { { XMIN , YMIN , ZMIN  }, 0.0 },
    { { YMIN , XPLUS, ZMIN  }, 0.0 }
};


/* A unit octahedron */
sphere_object sphere_oct = {
    sizeof(sphere_octahedron) / sizeof(sphere_octahedron[0]),
    &sphere_octahedron[0]
};

/* Forward declarations */
sphere_point *sphere_normalize();
sphere_point *sphere_midpoint();
double *sphere_print_object();
void sphere_print_triangle();
void sphere_pphigs_header();
void sphere_pphigs_trailer();



double *sphereTesselate(int maxlevel,int *n){
  sphere_triangle *oldt = NULL, *newt = NULL;
  sphere_point a, b, c;
  sphere_object *old=NULL, *new=NULL;
  int     ccwflag = 0, i, level;		/* Current subdivision level */
  double *out=NULL;
  void *v_allocate();
  static double *triangles=NULL;
  static int npoint=0;
  static sphere_object *objs=NULL;
  static int _maxlevel=0;

  int npol = 6;

  if(maxlevel > _maxlevel || objs == NULL){
    if(objs){
      for(i=0;i<_maxlevel;i++){
        objs[i].npoly = 0;
        free(objs[i].poly);
        objs[i].poly=NULL;
      }
      free(objs);
      objs = NULL;
    }
    _maxlevel = maxlevel;    
    objs = (sphere_object *)v_allocate(maxlevel,sizeof(sphere_object));
    for(i=0;i<maxlevel;i++){
      objs[i].npoly = npol;
      npol *= 4;
      objs[i].poly = (sphere_triangle *)v_allocate(npol,sizeof(sphere_triangle));
    }
  }

  *n = objs[maxlevel-1].npoly;
 
  /* load up first one */
  objs[0].npoly = sphere_oct.npoly;
  for(i=0;i<objs[0].npoly;i++){
     objs[0].poly[i] = sphere_oct.poly[i];
  }

  old = &(objs[0]);

  /* Subdivide each starting triangle (maxlevel - 1) times */
  for (level = 1; level < maxlevel; level++) {
    new = &(objs[level]);
    /* Subdivide each triangle in the old approximation and normalize
     *  the new points thus generated to lie on the surface of the unit
     *  sphere.
     * Each input triangle with vertices labelled [0,1,2] as shown
     *  below will be turned into four new triangles:
     *
     *			Make new points
     *			    a = (0+2)/2
     *			    b = (0+1)/2
     *			    c = (1+2)/2
     *	  1
     *	 /\		Normalize a, b, c
     *	/						\
     *    b/____\ c		Construct new triangles
     *    /\    /\		    [0,b,a]
     *   /	\  /  \		    [b,1,c]
     *  /____\/____\	    [a,b,c]
     * 0	  a	2	    [a,c,2]
     */
    for (i = 0; i < old->npoly; i++) {
      oldt = &old->poly[i];
      newt = &new->poly[i*4];
      
      a = *sphere_normalize(sphere_midpoint(&oldt->pt[0], &oldt->pt[2]));
      b = *sphere_normalize(sphere_midpoint(&oldt->pt[0], &oldt->pt[1]));
      c = *sphere_normalize(sphere_midpoint(&oldt->pt[1], &oldt->pt[2]));
      
      newt->pt[0] = oldt->pt[0];
      newt->pt[1] = b;
      newt->pt[2] = a;
      newt++;
      
      newt->pt[0] = b;
      newt->pt[1] = oldt->pt[1];
      newt->pt[2] = c;
      newt++;
      
      newt->pt[0] = a;
      newt->pt[1] = b;
      newt->pt[2] = c;
      newt++;
      
      newt->pt[0] = a;
      newt->pt[1] = c;
      newt->pt[2] = oldt->pt[2];
    }
   
    /* Continue subdividing new triangles */
    old = new;
  }
  
  if(triangles==NULL || *n *9 > npoint){
    if(triangles)free(triangles);
    triangles=(double *)v_allocate(old->npoly *9,sizeof(double));
    npoint = old->npoly *9;
  }
  
  /* Spit out coordinates for each triangle */
  for (i = 0; i < old->npoly; i++){
    triangles[i*9+0] = old->poly[i].pt[0].x;
    triangles[i*9+1] = old->poly[i].pt[0].y;
    triangles[i*9+2] = old->poly[i].pt[0].z;
    triangles[i*9+3] = old->poly[i].pt[1].x;
    triangles[i*9+4] = old->poly[i].pt[1].y;
    triangles[i*9+5] = old->poly[i].pt[1].z;
    triangles[i*9+6] = old->poly[i].pt[2].x;
    triangles[i*9+7] = old->poly[i].pt[2].y;
    triangles[i*9+8] = old->poly[i].pt[2].z;
  }

  /*for(i=0;i<maxlevel;i++){
    objs[i].npoly = 0;
    free(objs[i].poly);
    objs[i].poly=NULL;
  }
  free(objs);*/

  return(triangles); 
}

/* Normalize a point p */
sphere_point *sphere_normalize(p)
sphere_point *p;
{
    static sphere_point r;
    double mag;

    r = *p;
    mag = r.x * r.x + r.y * r.y;
    if (mag != 0.0) {
	mag = 1.0 / sqrt(mag);
	r.x *= mag;
	r.y *= mag;
    }

    return &r;
}

sphere_point *sphere_midpoint();
/* Return the midpoint on the line between two points */
sphere_point *sphere_midpoint(sphere_point *a, sphere_point *b){
    static sphere_point r;

    r.x = (a->x + b->x) * 0.5;
    r.y = (a->y + b->y) * 0.5;
    r.z = (a->z + b->z) * 0.5;

    return &r;
}

/* Write out all triangles in an object */
double *sphere_print_object(sphere_object *obj, int *n)
{
    void *v_allocate();
    int i;
    static double *triangles=NULL;
    static int npoint=0;
  
    if(triangles==NULL || obj->npoly*9 > npoint){
      if(triangles)free(triangles);
      triangles=(double *)v_allocate(obj->npoly*9,sizeof(double));
      npoint = obj->npoly*9;
    }

    (*n) = obj->npoly;
    /* Spit out coordinates for each triangle */
    for (i = 0; i < obj->npoly; i++){
      triangles[i*9+0] = obj->poly[i].pt[0].x;
      triangles[i*9+1] = obj->poly[i].pt[0].y;
      triangles[i*9+2] = obj->poly[i].pt[0].z;
      triangles[i*9+3] = obj->poly[i].pt[1].x;
      triangles[i*9+4] = obj->poly[i].pt[1].y;
      triangles[i*9+5] = obj->poly[i].pt[1].z;
      triangles[i*9+6] = obj->poly[i].pt[2].x;
      triangles[i*9+7] = obj->poly[i].pt[2].y;
      triangles[i*9+8] = obj->poly[i].pt[2].z;
    }
    return(triangles);
}

