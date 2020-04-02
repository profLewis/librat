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

int	intersect_plane(flagbag,plane_Ptr,ray_origin_Ptr,ray_direction_Ptr,objectlist_Ptr)
triplet	*ray_origin_Ptr, *ray_direction_Ptr;
Plane	*plane_Ptr;
ObjectList *objectlist_Ptr;
FlagBag	*flagbag;
{
	double	D;
	int	ray_to_plane();

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

int	where_hit_plane(hit_Ptr,ray_length_Ptr,ray_origin_Ptr,ray_direction_Ptr)
triplet *hit_Ptr,*ray_origin_Ptr,*ray_direction_Ptr;
double	*ray_length_Ptr;
{
	*hit_Ptr=vector_plus(*ray_origin_Ptr,V_factor(*ray_direction_Ptr,*ray_length_Ptr));
	return(1);
}
