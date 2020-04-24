#ifndef SPHEROID_H
#define SPHEROID_H
int     ray_on_spheroid(double *D_Ptr,Spheroid *spheroid_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);
int     vray_in_spheroid(double *D_Ptr,double *lengthToTravel,Spheroid *spheroid_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);
#endif
