#ifndef PLANE_H
#define PLANE_H
int     intersect_plane(FlagBag *flagbag,Plane *plane_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr,ObjectList *objectlist_Ptr);
int     where_hit_plane(triplet *hit_Ptr,double *ray_length_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr);


#endif

