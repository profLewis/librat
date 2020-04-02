#include "prat.h"

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept Geog UCL. Thu Jan  5 12:48:29 GMT 1995 */

/*
**	point on ellipse
**	glassner p.36
*/

int	ray_on_ellipse(D_Ptr,ellipse_Ptr,ray,p1_Ptr,p2_Ptr,D)
double	*D_Ptr,*p1_Ptr,*p2_Ptr,*D;
Ray	*ray;
Ellipse	*ellipse_Ptr;
{
	double	p,q,r,b;
	triplet	A,B,C;

	A=vector_minus(ray->origin,ellipse_Ptr->origin);
	B=vector_copy2(A.x/ellipse_Ptr->dimensions.x,A.y/ellipse_Ptr->dimensions.y,A.z/ellipse_Ptr->dimensions.z);
	C=vector_copy2(ray->direction.x/ellipse_Ptr->dimensions.x,ray->direction.y/ellipse_Ptr->dimensions.y,ray->direction.z/ellipse_Ptr->dimensions.z);
	p=V_dot(B,B)-1.0;
	q=V_dot(B,C)*2.0;
	r=V_dot(C,C);
	b=q*q-4*p*r;
	if(b<0)return(0);
	b=sqrt(b);
	*p1_Ptr=(-q+b)/(2*r);
	*p2_Ptr=(-q-b)/(2*r);

	if(*p1_Ptr<*p2_Ptr&&*p1_Ptr>=0.0)
		*D_Ptr=*p1_Ptr;
	else
		*D_Ptr=*p2_Ptr;
	
	if(*D_Ptr<=0.0)return(0);

	ray->rayLengthThroughObject = fabs(MAX(0.,*p1_Ptr) - MAX(0.,*p2_Ptr));
	return(1);

}

int	vray_in_ellipse(D_Ptr,lengthToTravel,ellipse_Ptr,ray,p1_Ptr,p2_Ptr,D)
double	*D_Ptr,*p1_Ptr,*p2_Ptr,*D,*lengthToTravel;
Ray	*ray;
Ellipse	*ellipse_Ptr;
{
	double	p,q,r,b;
	triplet	A,B,C;

	A=vector_minus(ray->origin,ellipse_Ptr->origin);
	B=vector_copy2(A.x/ellipse_Ptr->dimensions.x,A.y/ellipse_Ptr->dimensions.y,A.z/ellipse_Ptr->dimensions.z);
	C=vector_copy2(ray->direction.x/ellipse_Ptr->dimensions.x,ray->direction.y/ellipse_Ptr->dimensions.y,ray->direction.z/ellipse_Ptr->dimensions.z);
	p=V_dot(B,B)-1.0;
	q=V_dot(B,C)*2.0;
	r=V_dot(C,C);
	b=q*q-4*p*r;
	if(b<0)return(0);
	b=sqrt(b);
	*p1_Ptr=(-q+b)/(2*r);
	*p2_Ptr=(-q-b)/(2*r);

 
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
