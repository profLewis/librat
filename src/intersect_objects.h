#ifndef _INTERSECT_OBJECTS_
#define _INTERSECT_OBJECTS_

struct Sort_Hit {
	double	D;
	char	type;
	void	*hit;
	struct Sort_Hit	*next_hit;
};


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

void    reset_local_z_to_local_max_height(RATobj *bb,void *materialbag,triplet *look_at,triplet *camera_position,FlagBag *flagbag,BBox *bbox_Ptr);
Material_table *doIHitAnything(RATobj *bb,triplet *normal,triplet *plane_normal,int *tree_depth_Ptr,ObjectList *objectlist_Ptr,BBox *bbox_Ptr,IlluminationBag *illumination,FlagBag *flagbag,WavebandBag *wavebandbag,MaterialBag *materialbag,Ray *ipray);
int getMaterialIndex(Material_table *material_Ptr,MaterialBag *materialbag);
double lidarDistance(MaterialBag *materialbag,Ray *ipray,IlluminationBag *illumination,FlagBag *flagbag,int sunNo);
Ray     renders(int hitCamera,RATobj *bb,HitPoint *hitPoint,int *tree_depth_Ptr,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,ObjectList *objectlist_Ptr,BBox *bbox_Ptr,FlagBag *flagbag,Material_table *material_table_Ptr);
Ray     render(RATobj *bb,HitPoint *hitPoint,int *tree_depth_Ptr,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,ObjectList *objectlist_Ptr,BBox *bbox_Ptr,FlagBag *flagbag,Material_table *material_table_Ptr);
struct Sort_Hit *quantise_ray_direction(int *quantised_direction_Ptr,triplet *ray_direction_Ptr,BBox *bbox_Ptr);
int     intersect_objects(RATobj *bb,MaterialBag *materialbag,FlagBag *flag_Ptr,BBox *bbox_Ptr,Ray *ipray,ObjectList *objectlist_Ptr,int ray_clip);

#endif
