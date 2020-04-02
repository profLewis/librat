/*
**	Plant RAy Tracer (PRAT) object primitives (Contents):
**	
**	Facet,
**	Cylinder,
**	Sphere,
**	Bezier4,
**	BBox
**      Ellipsoid
**      Spheroid
**
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */
#ifndef _PRAT_OBJECTS_
#define _PRAT_OBJECTS_
#include	"materials.h"
/*
**	Plane struct
*/
struct Plane_struct{
	triplet	normal;
	double	dw;
	Material_table     *material;
	struct Plane_struct	*next_plane;
};

typedef struct Plane_struct Plane;

typedef struct{
	triplet	vertex_normal;
	triplet normal_du;
	triplet normal_dv;
}Vertex_normals;

typedef struct{
	pair	vertex_local;
	pair 	local_du;
	pair 	local_dv;
	triplet	basis[2];
	double	du_length;
	double	dv_length;
}Vertex_locals;

/*
**	Facet struct
*/
typedef struct{
	char	orientation;
	pair	g[3];
	double	dw;
	triplet	normal;
	Material_table     *material;
}Facet_quick;



struct D_Facet_struct{
	char	orientation;
	Pair	base;
	double	store[4];
	double	dw;
	triplet	normal;
	Vertex_normals	 *vertex_normals;
	Vertex_locals	 *local_coords;
	Material_table     *material;
	struct D_Facet_struct	*next_facet;
};

typedef struct D_Facet_struct D_Facet;

struct Disk_struct{
        double objectSize;
	triplet	origin;
	triplet	normal;
	double r2;
	Material_table     *material;
	struct Disk_struct *next_disk;
};

typedef struct Disk_struct Disk;

/* bilinear patch structure */

struct BiLinearPatch_struct{
        double objectSize;
	triplet	corner[4];
	triplet	cornerNormals[4];
	triplet	normal;
	Vertex_normals	 *vertex_normals;
	Vertex_locals	 *local_coords;
		
	Material_table     *material;
	struct BiLinearPatch_struct *next_BiLinearPatch;
};

typedef struct BiLinearPatch_struct BiLinearPatch;
/*
**	Cylinder struct
*/
struct Cylinder_struct{
        double objectSize;
	triplet	origin;
	triplet	normal;
	double	height;
	double	store_h;
	double	r2;
/*	Disk at ends */
	Disk *ends;
	Material_table     *material;
	struct Cylinder_struct	*next_cylinder;
};

typedef struct Cylinder_struct Cylinder;

/*
**	Ellipse struct
*/
struct Ellipse_struct{
        double objectSize;
	triplet	origin;
	triplet	normal;
	triplet	dimensions;
	Material_table     *material;
	struct Ellipse_struct	*next_ellipse;
};

typedef struct Ellipse_struct Ellipse;


/*
**      Spheroid struct
*/
struct Spheroid_struct{
        double objectSize;
        /* inputs */
        triplet origin;
        triplet tip;
        double radius;
        /* derived */
        triplet naxes[3];
        triplet centre;
        triplet V1;
        double a2;
/*
** recap:
** pre-store:
** naxes[0] = axes[0]/radius
** naxes[1] = axes[1]/radius
** naxes[2] = axes[2]/semilen
** where axes are normliased axes.
**
** 1. Calculate O' = O-centre
** 2. Calculate A = [O'.naxes[0],O'.naxes[1],O'.naxes[2]]
** 3. calculate B = [d.naxes[0],d.naxes[1],d.naxes[2]]
** 4. Calculate Q = ((A . B)^2 - (A . A -1) (B . B))
** 5. If Q < 0: return fail
** 6. l = [-(A . B) +/- sqrt(Q)]/(B . B)
*/
        
        triplet normal;
        triplet dimensions;
        Material_table     *material;
        struct Spheroid_struct   *next_spheroid;
};

typedef struct Spheroid_struct Spheroid;

/*
**	dem structure
*/

typedef struct {
	triplet	offset;
	triplet	extent;
	triplet	inv_extent;
	triplet	bbox_max;
	triplet	bbox_min;
	char	*direction;
}Dem_Attributes;

#define TRIANGULAR_REPEAT 1
#define SQUARE_REPEAT 0


/*
**	Sphere struct
*/
struct Sphere_struct{
        double objectSize;
	triplet	centre;
	double	radius;
	double	r2;
	Material_table     *material;
	struct Sphere_struct	*next_sphere;
};

typedef struct Sphere_struct Sphere;

struct D_Sphere_struct{
	triplet	centre;
	double	radius;
	double	r2;
	Material_table     *material;
	struct D_Sphere_struct	*next_sphere;
};

typedef struct D_Sphere_struct D_Sphere;

typedef struct {
	int		spherical_cell_bounds_flag;		/* 0 for global data 1 for partial */
	D_Facet		spherical_cell_bounds[4];
	triplet		extent;
	triplet		inv_extent;
	triplet		offset;
}Partial_Spherical_Data;	

/*
**	spherical dem structure
*/

struct Spherical_Dem_struct{
        double objectSize;
	Data		data;
	struct header	hd;
	Material_table	*material;
	D_Sphere	base_sphere;		/* sphere offset */
	D_Sphere	bounding_sphere[2];	/* bounding (global) sphere data */
	Partial_Spherical_Data	partial_spherical_data;
	struct Spherical_Dem_struct	*next_dem;
};

typedef struct Spherical_Dem_struct Spherical_Dem;

#define VIEWER 0 
#define ILLUMINATION 1 

typedef struct {
	triplet	origin;
	triplet	direction;
	double	ray_length;
	int	sourceOfRay;
	double	lengthOfRaySoFar;
	double  rayLengthThroughObject;
}Ray;

typedef struct {
	triplet	origin;
	triplet	direction;
	double	ray_length;
}D_Ray;

/*
**	Bezier4 struct
*/
struct Bezier4_struct{
        double objectSize;
	int	coords[16];
	Material_table     *material;
	struct Bezier4_struct	*next_bezier4;
};

typedef struct Bezier4_struct Bezier4;

struct Facet_struct{
        double objectSize;
        char    orientation;
        pair    base;
        double  store[4];
        double  dw;
        double size;
        triplet normal;
        triplet v[3];
        Vertex_normals   *vertex_normals;
        Vertex_locals    *local_coords;
        Material_table     *material;
        struct Facet_struct     *next_facet;
};

typedef struct Facet_struct fFacet;

struct Dem_struct{
        double objectSize;
        Dem_Attributes  dem_attributes;
        int                     repeatMode;
        triplet         *local;
        fFacet           facet;
        Data            data;
        struct header   hd;
        Material_table  *material;
        struct Dem_struct       *next_dem;
};

typedef struct Dem_struct Dem;


struct Content{

	struct	Bbox	*bbox;
	short		no_of_bboxes;

	struct Clone	*clone;
	short		no_of_clones;

        fFacet           *ttriangle;
	short		no_of_triangles;
	
	Bezier4		*bezier;
	short		no_of_beziers;
	
	Sphere		*sphere;
	short		no_of_spheres;

	Cylinder	*cylinder;
	short		no_of_cylinders;

	Ellipse		*ellipse;
	short		no_of_ellipses;

        Spheroid        *spheroid;
        short           no_of_spheroids;

	Disk		*disk;
	short		no_of_disks;

	Dem		*dem;
	short		no_of_dems;

	Spherical_Dem	*spherical_dem;
	short		no_of_spherical_dems;

	Plane		*plane;
	short		no_of_planes;

	BiLinearPatch		*blp;
	short		no_of_blps;
};

/*
**	BBox struct
*/
struct Bbox{
	triplet		max;
	triplet		min;
	struct Content	contents;
	Contents_order	*hit[7];
	char		transform_flag;
	char		define_flag;
	Matrix4 	m;
	Matrix4  	m_inverse;
	Matrix4 	m_inverse_transpose;
	struct Bbox	*next_bbox;	
};


/*
**	clone object
*/

struct Clone{
	struct	Bbox	*bbox;
	triplet translation;	/* separate clone translation vector */
/*	
** Monday November 26 20:37:06 GMT 2001
**
** Lewis: attempt to introduce arbitrary clone rotation
**
** status:
** parser is complete
** needs to make use of stored matrix
**
** delete variables:
**	double	cos_theta;
**	double	sin_theta;
**
** old style - for z-rotation only -
** all instances of cos_theta, sin_theta 
** should now be replaced by a multiplication 
** of the relevant vector by matrix
**
**	out.x=0.;for(i=0;i<3;i++) out.x += m[0*3+i]*v.x;
**	out.y=0.;for(i=0;i<3;i++) out.y += m[1*3+i]*v.y;
**	out.z=0.;for(i=0;i<3;i++) out.z += m[2*3+i]*v.z;
**
** this only occurs in reflectance.c & intersect_objects.c
** 
** although the rotation also carries through to 
** the objectlist_Ptr, so need to sort out where that is
** set and stored
**
** there is one odd occurrence of 
** 		objectlist_Ptr->cos_theta=store_clone_Ptr->cos_theta;
** in intersect_objects.c
**
** also, as a tidy up, replace z_rotation_flag
** simply by rotation_flag
** or better still, check to see if matrix is set
** which should be the same switch
*/
	char	z_rotation_flag;
	double 	*matrix;
	struct	Clone	*next_clone;
};


/*
**	contents box
*/
typedef struct Bbox 	BBox;
typedef struct Content 	Contents;
typedef struct Clone 	Clones;

/*
**	group name & pointer storage
*/

typedef struct{

	BBox	**group_bboxes;
	char	**group_names;
	int	no_of_groups;

} Group;

/*
**	Objectlist
*/

typedef struct{
  double	RayLength;
  triplet	intersection;
  triplet real_intersection;

  triplet	normal;
  
  double	matrix[9]; /* rotation matrix for clones */
  Matrix4 m;
  
  fFacet	*TriangleObj;
  Cylinder *CylinderObj;
  Ellipse *EllipseObj;
  Disk *DiskObj;
  BiLinearPatch *BiLinearPatchObj;
  Sphere	*SphereObj;
  Bezier4	*BezierObj;
  Plane	*PlaneObj;
  Dem	*DemObj;
  Spheroid *SpheroidObj; 
  
  double	*OutPut;
  double	ancilliary[4];	/* to pass extra data via this structure */
  char	ObjectType;
  char	transform_flag;
  char	clone_rotation;
  triplet	cloneTranslation;
  char	cloneTranslationFlag;
  double rayLengthThroughObject;

}ObjectList;

/*
**	pixel variance stuff
*/

typedef struct{
	double	sigma_f;
	double	sigma_f_sqr;
}PixelCondition;

typedef struct{
	double	variance;
	int	no_of_rays_per_pixel;
}PixelVarianceLimits;

/*
**	link list bbox stuff
*/

struct BOX {
	int		no_of_boxes;
	struct BOX 	*contents;
	struct BOX 	*next_box;
};

typedef struct BOX Box;

typedef struct {
	triplet location;  	/* where it hits */
	triplet localNormal;    /* local normal vector */

	triplet fromLocation;
	triplet fromVector;     /* previous direction */
	triplet toVector;	/* next direction */
	double fromVectorLength; /* distance from previous to here */
	int interactionType;     /* REFLECTANCE / TRANSMITTANCE / -1 */

	/* direct */
        int nSuns;
	int *hitSun;
	triplet *sunVector;
	int *sunInteractionType;
	double *lambertian;

	/* for LIDAR */
	double *distanceBackToViewer;

	/* diffuse */
	int hitSky;
	triplet skyVector;	

	Material_table *material; /* material of object hit */
	int materialIndex; 
	/* in materialbag->material_list->useage-materials[i] list */
}HitPoint;

typedef struct {
  double ***direct;	/* direct[bin][l][rtd] */
  double ***diffuse;	/* diffuse[l][rtd] */
  double ***directI;
  double ***diffuseI;

  double *sunlit;		/* sunlit[mat] */
  double **absorptance;
  
  double **diffuseRadiance;	/* diffuseRadiance[bin][l] */
  double **directRadiance;		/* directRadiance[bin][l] */
  double **diffuseIllumination;
  double **directIllumination;
  double distance;
}PStats;

typedef struct {
  int singlePathDiffuseArrayIndex;
  
  PStats singlePath;
  PStats singlePixel;
  PStats imageStats;
  
  Image_characteristics *image;
  int doTestFrame;
  int whichTestFrame;
  
  int nBins,nBands,nRPP;
  int argc;
  char **argv;
}PixelStats;



#endif
