#ifndef FACET_H
#define FACET_H
void    calculate_facet_limits(double *bounds,triplet *vertices);
void    double_calculate_facet_limits(double *bounds,triplet *vertices);
int     precompute_facet_features(double *bounds,triplet *tri,fFacet *facet_Ptr);
int     double_precompute_facet_features(double *scale_Ptr,double *bounds,triplet *Tri,fFacet *facet_Ptr,int isDem);
int     ray_to_plane(double *D_Ptr,double *dw_Ptr,triplet *normal_Ptr,triplet *origin, triplet *direction);
int     double_ray_to_plane(double *D_Ptr,double *dw_Ptr,triplet *normal_Ptr,Ray *ray);
int     point_in_triangle(triplet *p_Ptr,fFacet *facet_Ptr,Ray *ray,double ray_length,double *uv);
int     double_point_in_triangle(double *scale_Ptr,triplet *p_Ptr,fFacet *facet_Ptr,Ray *ray,double ray_length,double *uv);

#endif

