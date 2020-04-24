#include "prat.h"

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

/*
**	point on disk
*/

int	ray_on_disk(double *D_Ptr,Disk *disk_Ptr,Ray *ray,double *r)
{
	double	a,b,r2;
	triplet	A,R;

	A=vector_minus(disk_Ptr->origin,ray->origin);
	a=V_dot(A,disk_Ptr->normal);
	b=V_dot(ray->direction,disk_Ptr->normal);
	if(!b)	/* normal to disk */
		return(0);
	*D_Ptr=a/b;
	if(*D_Ptr<=0)
		return(0);
	/* radial vector */
	R=vector_minus(V_factor(ray->direction,*D_Ptr),A);
	r2=V_dot(R,R);
	/* check radius */
	if(r2>disk_Ptr->r2)
		return(0);
	*r=sqrt(r2);
	return(1);
}
