#include "prat.h"

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Thur Jun 13 1991 */

/*
**
**	 plane of arbitrary inclination intersection 
**
*/

int	intersect_plane(FlagBag *flagbag,Plane *plane_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr,ObjectList *objectlist_Ptr)
{
	double	D;

	if(ray_to_plane(&D,&(plane_Ptr->dw),&(plane_Ptr->normal),ray_origin_Ptr,ray_direction_Ptr)){
		if(D<objectlist_Ptr->RayLength){
			objectlist_Ptr->ObjectType=PLANE;
			objectlist_Ptr->RayLength=D;
			objectlist_Ptr->PlaneObj=plane_Ptr;
			objectlist_Ptr->intersection=vector_plus(*ray_origin_Ptr,V_factor(*ray_direction_Ptr,D-flagbag->ray_length_tol));
			objectlist_Ptr->normal=plane_Ptr->normal;
			return(1);
		}
	}
	return(0);
}


/*
**
**	calculate where ray hit object
**
*/

int	where_hit_plane(triplet *hit_Ptr,double *ray_length_Ptr,triplet *ray_origin_Ptr,triplet *ray_direction_Ptr)
{
	*hit_Ptr=vector_plus(*ray_origin_Ptr,V_factor(*ray_direction_Ptr,*ray_length_Ptr));
	return(1);
}
