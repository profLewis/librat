#ifndef ELLIPSE_H
#define ELLIPSE_H
int     ray_on_ellipse(double *D_Ptr,Ellipse *ellipse_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);
int     vray_in_ellipse(double *D_Ptr,double *lengthToTravel,Ellipse *ellipse_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D);
#endif
