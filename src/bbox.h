#ifndef BBOX_H
#define BBOX_H
void    calculate_bbox_limits(double *bounds,BBox *bbox_Ptr);
triplet calculate_sphere_cg(Sphere *obj,int i);
triplet calculate_cylinder_cg(Cylinder *obj,int i);
int     sort_in_link_list(struct Sort_Hit *slot[6],int *timer_Ptr,struct Sort_Hit *base_hit[6],double *bounds,struct Sort_Hit *data);
int     bbox_shuffle(double *tnear,double *tfar,double min,double max,double origin,double direction);
int     bbox_infinite_shuffle(double *tnear,double *tfar,double min,double max,double origin,double direction,double offset);
int testIntersection(double d,double origin,double direction,double bmin,double bmax);
int intersectsOk(double d,struct Bbox *bbox,Ray *ray,int axis);
int intersectsOkd(double d,struct Bbox *bbox,Ray *ray,int axis);
int     double_bbox_shuffle(double *tnear,double *tfar,double min,double max,double origin,double direction,struct Bbox *bbox,Ray *ray,int axis);
int     far_bbox_shuffle(double *tnear,double *tfar,double min,double max,double origin,double direction);
int     ray_to_bbox(double *tnear_Ptr,double *tfar_Ptr,struct Bbox     *bbox_Ptr,Ray *ray_Ptr);
int     ray_to_infinite_bbox(FlagBag *flagbag,double *tnear_Ptr,double *tfar_Ptr,struct Bbox     *bbox_Ptr,Ray *ray_Ptr,triplet *offset);
int     double_ray_to_bbox(double  *tnear_Ptr,double  *tfar_Ptr,struct Bbox     *bbox_Ptr,Ray *ray_Ptr);
int     coded_ray_to_bbox(double  *tnear_Ptr,double  *tfar_Ptr,struct Bbox     *bbox_Ptr,Ray *ray_Ptr,int     *near_code,int     *far_code);
int     far_ray_to_bbox(double  *tnear_Ptr,double  *tfar_Ptr,triplet *bbox_min,triplet *bbox_max,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr);

#endif

