#ifndef _intERSECT_OBJECTS_
#define _intERSECT_OBJECTS_

struct Sort_Hit {
	double	D;
	char	type;
	void	*hit;
	struct Sort_Hit	*next_hit;
};

typedef struct Sort_Hit Contents_order;

#define	BBOX		0
#define	FO		1
#define	SPHERE		2
#define CYLINDER	3
#define BEZIER		4
#define PLANE		5
#define CLONE		6
#define DEM		7
#define SPHERICAL_DEM	8
#define ELLIPSE		9
#define VOLUMETRIC_SPHERE 10
#define VOLUMETRIC_CYLINDER 11
#define VOLUMETRIC_ELLIPSE 12
#define VOLUMETRIC_SPHEROID 25
#define CLOSED_CYLINDER 13
#define DISK 14
#define BILINEARPATCH 15
#define SPHEROID 26

#define MAX_OBJECT_TYPES 16

#define MAX_TREE_DEPTH 20

#endif
