#ifndef CYLINDER_H
#define CYLINDER_H
int     ray_on_infinite_cylinder(double *D_Ptr,Cylinder *cylinder_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);
int     ray_on_finite_cylinder(Cylinder *cylinder_Ptr,double *D_Ptr,double *p1_Ptr,double *p2_Ptr,double *D,Ray *ray);
int     vray_on_finite_cylinder(RATobj *bb,double *D_Ptr,double *lengthToTravel,Cylinder *cylinder_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);

#endif

