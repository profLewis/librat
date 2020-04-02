#include	"prat.h"
/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

/*
**	point in sphere
**	glassner p.36
*/

int	ray_in_sphere(normal,D_Ptr,sphere_Ptr,ray,local)
double	*D_Ptr,*local;
Ray	*ray;
Sphere	*sphere_Ptr;
triplet *normal;
{
	double	b,c,q,t0,t1,sinAzimuth,cosAzimuth,zenith,azimuth;
	triplet	temp,down;
	static triplet up = {0.,0.,1.},side={1.,0.,0.},side2={0.,1.,0.};

	temp=vector_minus(ray->origin,sphere_Ptr->centre);
	b=2.0*V_dot(ray->direction,temp);
	c=V_dot(temp,temp)-sphere_Ptr->r2;

	if( (q=((b * b) - (4.0* c))) <0.0)return(0);	/* miss */
	
	q=sqrt(q);

	t0=(-b-q);	
	t1=(-b+q);

	/* point in sphere */
	if(t0<=0.0){
	  if(t1>0.){
	    *D_Ptr=t1*0.5;
	    ray->rayLengthThroughObject=*D_Ptr;
	    *normal=normalise(vector_plus(V_factor(ray->direction,*D_Ptr),temp));
	    zenith = acos(V_dot(*normal,up))/M_PI;
	    down=normalise(vector_cross(up,*normal));
	    cosAzimuth=V_dot(down,side);
	    sinAzimuth=V_dot(down,side2);
	    azimuth=atan2(sinAzimuth,cosAzimuth)/(2.*M_PI);
	    if(azimuth<0)azimuth+=1.;
	    local[0]=azimuth;
	    local[1]=zenith;
	    return(1);
	  }else
	    return(0);
	}else if(t1<=0.0){
	  *D_Ptr=t0*0.5;
	  ray->rayLengthThroughObject=*D_Ptr;
	  *normal=normalise(vector_plus(V_factor(ray->direction,*D_Ptr),temp));
	  zenith = acos(V_dot(*normal,up))/M_PI;
	  down=normalise(vector_cross(up,*normal));
	  cosAzimuth=V_dot(down,side);
	  sinAzimuth=V_dot(down,side2);
	  azimuth=atan2(sinAzimuth,cosAzimuth)/(2.*M_PI);
	  if(azimuth<0)azimuth+=1.;
	  local[0]=azimuth;
	  local[1]=zenith;
	  return(1);
	}

	/* point on sphere */
	*D_Ptr = MMIN(t0,t1)*0.5;       
	if(*D_Ptr<=0.00)return(0);
	ray->rayLengthThroughObject=fabs(MAX(0.,t0)-MAX(0.,t1))*0.5;
	*normal=normalise(vector_plus(V_factor(ray->direction,*D_Ptr),temp));
	zenith = acos(V_dot(*normal,up))/M_PI;
	down=normalise(vector_cross(up,*normal));
	cosAzimuth=V_dot(down,side);
	sinAzimuth=V_dot(down,side2);
	azimuth=atan2(sinAzimuth,cosAzimuth)/(2.*M_PI);
	if(azimuth<0)azimuth+=1.;
	local[0]=azimuth;
	local[1]=zenith;
	
	return(1);
}

int	vray_in_sphere(D_Ptr,L_Ptr,sphere_Ptr,ray)
double	*D_Ptr,*L_Ptr;
Ray	*ray;
Sphere	*sphere_Ptr;
{
	double	b,c,q,t0,t1;
	triplet	temp;

	temp=vector_minus(ray->origin,sphere_Ptr->centre);
	b=2.0*V_dot(ray->direction,temp);
	c=V_dot(temp,temp)-sphere_Ptr->r2;

	if( (q=((b * b) - (4.0* c))) <0.0)return(0);	/* miss */
	
	q=sqrt(q);

	t0=(-b-q);	
	t1=(-b+q);

	if(t0<=0.0)t0=0.;
	if(t1<=0.0)t1=0.;	/* point in sphere */
	*L_Ptr = (t1-t0)/2.0;	/* length to travel in sphere */
	if(*L_Ptr<=0.00)return(0);
	*D_Ptr = MMIN(t0,t1)/2.0;
	return(1);
}


int	ray_in_sphere_data(tnear_Ptr,tfar_Ptr,sphere_Ptr,ray_origin_Ptr,ray_direction_Ptr)
double	*tnear_Ptr,*tfar_Ptr;
triplet	*ray_origin_Ptr,*ray_direction_Ptr;
D_Sphere	*sphere_Ptr;
{
	double	b,c,q,t0,t1;
	triplet	temp;

	temp=vector_minus(*ray_origin_Ptr,sphere_Ptr->centre);
	b=2.0*v_dot(*ray_direction_Ptr,temp);
	c=v_dot(temp,temp)-sphere_Ptr->r2;

	if( (q=((b * b) - (4.0* c))) <0.0)return(0);	/* miss */
	
	q=sqrt(q);

	t0=(-b-q);	
	t1=(-b+q);

	if(t0<=0.0 || t1 <=0.0)return(0);	/* point in sphere */
	*tnear_Ptr = MMIN(t0,t1)/2.0;
	*tfar_Ptr = MMAX(t0,t1)/2.0;
	if(*tnear_Ptr<=0.00)return(0);
	return(1);
}


/*
**	point in triangle test
*/

int	point_on_sphere(p_Ptr,sphere_Ptr,ray_origin_Ptr,ray_direction_Ptr,ray_length)
Sphere	*sphere_Ptr;
triplet	*ray_origin_Ptr,*ray_direction_Ptr,*p_Ptr;
double	ray_length;
{
	
/*
**	calculate coodinates of point on surface
*/
	*p_Ptr=vector_plus(V_factor(*ray_direction_Ptr,ray_length),*ray_origin_Ptr);
	return(1);


}

