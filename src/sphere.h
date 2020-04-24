#ifndef SPHERE_H
#define SPHERE_H

#include "useful3.h"
#include "prat_objects.h"

int	ray_in_sphere(triplet *normal,double *D_Ptr,Sphere *sphere_Ptr,Ray *ray,double *local);
int	ray_in_sphere_data(double *tnear_Ptr,double *tfar_Ptr,D_Sphere *sphere_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr);
int	point_on_sphere(triplet *p_Ptr,Sphere *sphere_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr,double ray_length);
int     vray_in_sphere(double *D_Ptr,double *L_Ptr,Sphere *sphere_Ptr,Ray *ray);
#endif

