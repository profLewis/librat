#include "prat.h"

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept Geog UCL. Thu Jan  5 12:48:29 GMT 1995 */

/*
**	point on spheroid
**	
**  A spheroid is defined by:
**
**  origin, tip, radius
**
**  This becomes:
** 
**  centre, axes[3]
**  where axes[0] is the x axis in the space of the spheroid
**  where axes[1] is the y axis in the space of the spheroid
**  where axes[2] is the z axis in the space of the spheroid
**
** and these are unit vectors
** We also need radius (for the x-y plane) and semilen which is the semi-axis
** in the z-axis.
**
** semilen = 0.5*|tip-origin|
**
** A ray is of the form:
**  p = O + l d
** 
** where p is the point of intersection
** O is the ray origin
** l is the ray length
** d is the unit ray direction
**
** Also then
**
** p = centre + x axes[0] + y axes[1] + z axes[2]
**
** where x,y,z are scalars AND
**
** (x^2 + y^2)/radius^2 + z^2/semilen^2 = 1
**
** let O' = (O-centre)
**
** [O' + l d] . axes[0] = x
** [O' + l d] . axes[1] = y
** [O' + l d] . axes[2] = z
** 
** let
** O0 = O' . axes[0]/radius
** O1 = O' . axes[1]/radius
** O2 = O' . axes[2]/semilen
** d0 = d . axes[0]/radius
** d1 = d . axes[1]/radius
** d2 = d . axes[2]/semilen
**
** so
** O0 + l d0 = x
** O1 + l d1 = y
** O2 + l d2 = z
**
** (O0 + l d0)^2 = (x/radius)^2 = O0^2 + l^2 d0^2 + 2 l d0 etc.
**
** so
** 
** O0^2 + l^2 d0^2 + 2 l d0 O0 + O1^2 + l^2 d1^2 + 2 l d1 O1 + O2^2 + l^2 d2^2 + 2 l d2 O2- 1 = 0
** 
** (O0^2 + O1^2 + O2^2 -1) + l 2(d0 O0+d1 O1 +d2 O2) + l^2 (d0^2+d1^2+d2^2) = 0
** 
** so ... let
** A = [O' . axes[0]/radius,O' . axes[1]/radius,O' . axes[2]/semilen]
** B = [d . axes[0]/radius, d . axes[1]/radius,d . axes[2]/semilen]
**
** (A . A -1) + 2 l (A . B) + l^2 (B . B) = 0
** 
** Q = ((A . B)^2 - (A . A -1) (B . B))
** so
** 
** l = [-(A . B) +/- sqrt(Q)]/(B . B)
**
** provided Q>=0 else no intersection
**
** recap:
** pre-store:
** naxes[0] = axes[0]/radius
** naxes[1] = axes[1]/radius
** naxes[2] = axes[2]/semilen
** where axes are normliased axes.
**
** 1. Calculate O' = O-centre
** 2. Calculate A = [O'.naxes[0],O'.naxes[1],O'.naxes[2]]
** 3. calculate B = [d.naxes[0],d.naxes[1],d.naxes[2]]
** 4. Calculate Q = ((A . B)^2 - (A . A -1) (B . B))
** 5. If Q < 0: return fail
** 6. l = [-(A . B) +/- sqrt(Q)]/(B . B)
*/

int	ray_on_spheroid(double *D_Ptr,Spheroid *spheroid_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D)
{
	double	aa,bb,ab,q,sq;
	triplet	A,B,Oprime;

	Oprime=vector_minus(ray->origin,spheroid_Ptr->centre);
        A = vector_copy2(V_dot(Oprime,spheroid_Ptr->naxes[0]),V_dot(Oprime,spheroid_Ptr->naxes[1]),V_dot(Oprime,spheroid_Ptr->naxes[2]));
        B = vector_copy2(V_dot(ray->direction,spheroid_Ptr->naxes[0]),V_dot(ray->direction,spheroid_Ptr->naxes[1]),V_dot(ray->direction,spheroid_Ptr->naxes[2]));
        ab = V_dot(A,B);
        aa = V_dot(A,A);
        bb = V_dot(B,B);
        if(bb==0)
          return(0);
        q = ab*ab - (aa-1.)*bb;
        if(q<0)
          return(0);
        sq = sqrt(q);
        *p1_Ptr = (-ab + sq)/bb;
        *p2_Ptr = (-ab - sq)/bb;

	if(*p1_Ptr<*p2_Ptr&&*p1_Ptr>=0.0)
		*D_Ptr=*p1_Ptr;
	else
		*D_Ptr=*p2_Ptr;
	
	if(*D_Ptr<=0.0)return(0);

	ray->rayLengthThroughObject = fabs(MAX(0.,*p1_Ptr) - MAX(0.,*p2_Ptr));
	return(1);

}

int	vray_in_spheroid(double *D_Ptr,double *lengthToTravel,Spheroid *spheroid_Ptr,Ray *ray,double *p1_Ptr,double *p2_Ptr,double *D)
{
        double  aa,bb,ab,q,sq;
        triplet A,B,Oprime;

        Oprime=vector_minus(ray->origin,spheroid_Ptr->centre);
        A = vector_copy2(V_dot(Oprime,spheroid_Ptr->naxes[0]),V_dot(Oprime,spheroid_Ptr->naxes[1]),V_dot(Oprime,spheroid_Ptr->naxes[2]));
        B = vector_copy2(V_dot(ray->direction,spheroid_Ptr->naxes[0]),V_dot(ray->direction,spheroid_Ptr->naxes[1]),V_dot(ray->direction,spheroid_Ptr->naxes[2]));
        ab = V_dot(A,B);
        aa = V_dot(A,A);
        bb = V_dot(B,B);
        if(bb==0)
          return(0);
        q = ab*ab - (aa-1.)*bb;
        if(q<0)
          return(0);
        sq = sqrt(q);
        *p1_Ptr = (-ab + sq)/bb;
        *p2_Ptr = (-ab - sq)/bb;

	if(*p1_Ptr<*p2_Ptr&&*p1_Ptr>=0.0)
		*D_Ptr=*p1_Ptr;
	else if(*p2_Ptr>0)
		*D_Ptr=*p2_Ptr;
	else 
	  *D_Ptr=*p1_Ptr;
	
	if(*D_Ptr<=0.0)return(0);
	if(*p1_Ptr<0)*p1_Ptr=0;
	if(*p2_Ptr<0)*p2_Ptr=0;
	*lengthToTravel=fabs(*p1_Ptr-*p2_Ptr);
	return(1);
}
