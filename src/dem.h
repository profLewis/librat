#ifndef DEM_H
#define DEM_H
#include "prat.h"

int     intersect_bbox_contents(double *tnear,double *tfar,triplet *normal_Ptr,triplet *hit_point,double *ray_length_Ptr,Ray *ray_Ptr,ipair *grid_Ptr,pair *ggrid_Ptr,Dem *dem_Ptr,int type_of_cell,double *uv);
void	enter_dem(int verbose,Data *data,struct  header  *hd_Ptr,char *dem_filename,int type);
void	get_candidate_point(Data *data,triplet *candidate_Ptr,int i,int j,struct  header  *hd_Ptr,Dem_Attributes  *dem_attributes_Ptr);
void	get_dem_bbox(int verbose,Data *data,struct  header  *hd_Ptr,triplet *bbox_max,triplet *bbox_min,Dem_Attributes  *dem_attributes_Ptr,int type);
int	dem_read(double  *bounds,Dem *dem,int verbose,int local_coords,char *line,Data *data,struct header   *hd,triplet  *bbox_max,triplet  *bbox_min,Dem_Attributes  *dem_attributes_Ptr);
int	spherical_dem_read(double  *bounds,int verbose,char *line,Data *data,struct header   *hd,triplet  *bbox_max,triplet  *bbox_min,Spherical_Dem *spherical_dem_Ptr,VeRtIcEs *vertices);
int	intersect_spherical_dem_cell_contents(double *ray_length_Ptr,triplet *normal_Ptr,Ray *ray_Ptr,triplet *hit_point,triplet *d);
int	is_even(int number);
int	type_of_cell(ipair *grid_Ptr,int cols,char *direction);
void	global2local_ray(Ray *global_ray_Ptr,Ray *local_ray_Ptr,Dem *dem_Ptr);
pair	global2local_2D(pair coord,Dem *dem_Ptr);
pair	local2global_2D(ipair coord,Dem *dem_Ptr);
pair	double_global2local_2D(pair coord,Dem *dem_Ptr);
pair	double_local2global_2D(ipair coord,Dem *dem_Ptr);
int	ray_within_local_bbox(double *tnear_Ptr,double *tfar_Ptr,Ray *ray_Ptr,ipair *grid_Ptr,pair *ggrid_Ptr,Dem *dem_Ptr,int *ray_direction_code);
int	dem_intersect(RATobj *bb,triplet *normal_Ptr,Dem *dem_Ptr,Ray *global_ray_Ptr,double *ray_length_Ptr,double *uv);
void	setup_grid(ipair *grid,pair ggrid,Spherical_Dem *dem_Ptr);
double	get_grid_value(ipair *grid,Data *data,struct header *hd);
int	setup_simple_facet(double *scale_Ptr,fFacet *facet_Ptr,triplet *du,triplet *dv,triplet *base);
int	spherical_cell_intersect(ipair *grid,Spherical_Dem *dem_Ptr,Ray *ray,triplet *normal_Ptr,double *tmin_Ptr,int *next_direction_Ptr);
int getIntersectionLimits(triplet *hit_points,Ray *ray,Spherical_Dem *dem_Ptr);
void    getRowColNumbers(int *row,int *col,triplet *intersectionPoints,Ray *ray,Spherical_Dem *demPtr);
void	grid2Cartesian(triplet *d,int r,int c,Spherical_Dem *dem_Ptr);
void	getCornerValues(triplet *d,int r,int c,int movementR,int movementC,Spherical_Dem *dem_Ptr);
int	spherical_dem_intersect(RATobj *bb,triplet *normal_Ptr,Spherical_Dem *dem_Ptr,Ray *ray);
int	ray_dem_intersect(RATobj *bb,triplet *normal_Ptr,double *distance_Ptr,void *dem_Ptr,Ray *ray,int type,double *uv);
#endif

