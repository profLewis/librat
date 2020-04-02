#include "prat.h"

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

/*
**	calculate axis-aligned facet limits
*/

void	calculate_facet_limits(bounds,vertices)
double	*bounds;
triplet	*vertices;
{
	int	i;

	for(i=0;i<3;i++)
		bounds[i]= -BIG;
	for(i=3;i<6;i++)
		bounds[i]=BIG;

	for(i=0;i<3;i++){
		bounds[0]=MMAX(bounds[0],vertices[i].x);
		bounds[1]=MMAX(bounds[1],vertices[i].y);
		bounds[2]=MMAX(bounds[2],vertices[i].z);
		bounds[3]=MMIN(bounds[3],vertices[i].x);
		bounds[4]=MMIN(bounds[4],vertices[i].y);
		bounds[5]=MMIN(bounds[5],vertices[i].z);
	}
}

void	double_calculate_facet_limits(bounds,vertices)
double	*bounds;
triplet	*vertices;
{
	int	i;

	for(i=0;i<3;i++)
		bounds[i]= -BIG;
	for(i=3;i<6;i++)
		bounds[i]=BIG;

	for(i=0;i<3;i++){
		bounds[0]=MMAX(bounds[0],vertices[i].x);
		bounds[1]=MMAX(bounds[1],vertices[i].y);
		bounds[2]=MMAX(bounds[2],vertices[i].z);
		bounds[3]=MMIN(bounds[3],vertices[i].x);
		bounds[4]=MMIN(bounds[4],vertices[i].y);
		bounds[5]=MMIN(bounds[5],vertices[i].z);
	}
}


/*
**
**	 precompute triangular facet features 
**
*/

int	precompute_facet_features(bounds,tri,facet_Ptr)
fFacet	*facet_Ptr;
double	*bounds;
triplet	*tri;
{
	triplet	n,normal,du,dv,vector_minus(),vector_cross(),Tri[3],V_factor();
	pair Du,Dv;
	double	tmp,mod_normal,nx,ny,nz,v_mod();
	int	i;

/*
**	store axis-aligned limits
*/

	calculate_facet_limits(bounds,tri);
	for(i=0;i<3;i++){
		Tri[i].x=(double)tri[i].x;
		Tri[i].y=(double)tri[i].y;
		Tri[i].z=(double)tri[i].z;
		facet_Ptr->v[i].x=tri[i].x;
		facet_Ptr->v[i].y=tri[i].y;
		facet_Ptr->v[i].z=tri[i].z;
	}
/*
**	u
*/
	du=vector_minus(Tri[1],Tri[0]);
/*
**	v
*/
	dv=vector_minus(Tri[2],Tri[0]);

/*
**	normal vector
*/

	normal=vector_cross(du,dv);

/*
**	store facet normal
*/

	if((mod_normal=v_mod(normal))==0.0){
		fprintf(stderr,"facet:\tzero-sized facet input\n");
		return(0);
	}
	n=V_factor(normal,1.0/mod_normal);

	facet_Ptr->normal.x=(double)n.x;
	facet_Ptr->normal.y=(double)n.y;
	facet_Ptr->normal.z=(double)n.z;

/*
**	find max dimension of normal -> facet orientation
*/

	nx=fabs(normal.x);
	ny=fabs(normal.y);
	nz=fabs(normal.z);
	if(nx>ny){
		if(nx>nz){		/* use y-z plane */
			facet_Ptr->orientation=0;	
			facet_Ptr->base.x=tri[0].y;
			facet_Ptr->base.y=tri[0].z;
			Du.x=du.y;
			Du.y=du.z;
			Dv.x=dv.y;
			Dv.y=dv.z;
		}else {		/* use x-y plane */
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=tri[0].x;
			facet_Ptr->base.y=tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
		}
	}else{
		if(ny>nz){		/* use x-z plane */
			facet_Ptr->orientation=1;	
			facet_Ptr->base.x=tri[0].z;
			facet_Ptr->base.y=tri[0].x;
			Du.x=du.z;
			Du.y=du.x;
			Dv.x=dv.z;
			Dv.y=dv.x;
		}else {			/* use x-y plane */
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=tri[0].x;
			facet_Ptr->base.y=tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
		}
	}
			
	tmp=(Du.x * Dv.y - Dv.x * Du.y);
	if(tmp==0.0){
		switch(facet_Ptr->orientation){
			case 0:
			facet_Ptr->orientation=1;	
			facet_Ptr->base.x=tri[0].z;
			facet_Ptr->base.y=tri[0].x;
			Du.x=du.z;
			Du.y=du.x;
			Dv.x=dv.z;
			Dv.y=dv.x;
			break;
			case 1:
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=tri[0].x;
			facet_Ptr->base.y=tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
			break;
			case 2:
			facet_Ptr->orientation=0;	
			facet_Ptr->base.x=tri[0].y;
			facet_Ptr->base.y=tri[0].z;
			Du.x=du.y;
			Du.y=du.z;
			Dv.x=dv.y;
			Dv.y=dv.z;
			break;
		}

		tmp=(Du.x * Dv.y - Dv.x * Du.y);

		if(tmp==0.0)	
			error1("precompute_facet_features\t:error setting up facet");
	}
	tmp = 1.0/tmp;

	facet_Ptr->store[0]=(double)(Dv.y*tmp);
	facet_Ptr->store[1]=(double)(-Dv.x*tmp);
	facet_Ptr->store[2]=(double)(-Du.y*tmp);
	facet_Ptr->store[3]=(double)(Du.x*tmp);
/*
**	distance from origin to plane
*/
	facet_Ptr->dw= -V_dot(tri[0],facet_Ptr->normal);

	return(1);
}

int	double_precompute_facet_features(scale_Ptr,bounds,Tri,facet_Ptr,isDem)
fFacet	*facet_Ptr;
double	*bounds,*scale_Ptr;
triplet	*Tri;
int isDem;
{
	triplet	n,normal,du,dv,vector_minus(),vector_cross(),V_factor();
	pair Du,Dv;
	double	tmp,mod_normal,nx,ny,nz,v_mod();

/*
**	store axis-aligned limits
*/

	double_calculate_facet_limits(bounds,Tri);

/*
**	u
*/
	du=vector_minus(Tri[1],Tri[0]);
/*
**	v
*/
	dv=vector_minus(Tri[2],Tri[0]);

/*
**	normal vector
*/

	normal=vector_cross(du,dv);

/*
**	store facet normal
*/

	if((mod_normal=v_mod(normal))==0.0){
	  fprintf(stderr,"facet:\tzero-sized facet input\n");
	  return(0);
	}
	n=V_factor(normal,1.0/mod_normal);

	facet_Ptr->normal.x=(double)n.x;
	facet_Ptr->normal.y=(double)n.y;
	facet_Ptr->normal.z=(double)n.z;
	facet_Ptr->size=sqrt(mod_normal*0.5);
/*
**	find max dimension of normal -> facet orientation
*/

	nx=fabs(normal.x);
	ny=fabs(normal.y);
	nz=fabs(normal.z);
        if (isDem || du.z == 0 || (du.x == 0 && dv.y == 0) || (dv.x == 0 && du.y == 0)){ 
          /* use x-y plane */
          facet_Ptr->orientation=2;
          facet_Ptr->base.x=Tri[0].x;
          facet_Ptr->base.y=Tri[0].y;
          Du.x=du.x;
          Du.y=du.y;
          Dv.x=dv.x;
          Dv.y=dv.y;
        }else if(nx>ny){
		if(nx>nz){		/* use y-z plane */
			facet_Ptr->orientation=0;	
			facet_Ptr->base.x=Tri[0].y;
			facet_Ptr->base.y=Tri[0].z;
			Du.x=du.y;
			Du.y=du.z;
			Dv.x=dv.y;
			Dv.y=dv.z;
		}else {		/* use x-y plane */
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=Tri[0].x;
			facet_Ptr->base.y=Tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
		}
	}else{
		if(ny>nz){		/* use x-z plane */
			facet_Ptr->orientation=1;	
			facet_Ptr->base.x=Tri[0].z;
			facet_Ptr->base.y=Tri[0].x;
			Du.x=du.z;
			Du.y=du.x;
			Dv.x=dv.z;
			Dv.y=dv.x;
		}else {			/* use x-y plane */
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=Tri[0].x;
			facet_Ptr->base.y=Tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
		}
	}
			
	tmp=(Du.x * Dv.y - Dv.x * Du.y);
	if(tmp==0.0){
		switch(facet_Ptr->orientation){
			case 0:
			facet_Ptr->orientation=1;	
			facet_Ptr->base.x=Tri[0].z;
			facet_Ptr->base.y=Tri[0].x;
			Du.x=du.z;
			Du.y=du.x;
			Dv.x=dv.z;
			Dv.y=dv.x;
			break;
			case 1:
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=Tri[0].x;
			facet_Ptr->base.y=Tri[0].y;
			Du.x=du.x;
			Du.y=du.y;
			Dv.x=dv.x;
			Dv.y=dv.y;
			break;
			case 2:
			facet_Ptr->orientation=0;	
			facet_Ptr->base.x=Tri[0].y;
			facet_Ptr->base.y=Tri[0].z;
			Du.x=du.y;
			Du.y=du.z;
			Dv.x=dv.y;
			Dv.y=dv.z;
			break;
		}

		tmp=(Du.x * Dv.y - Dv.x * Du.y);

		if(tmp==0.0)	
			error1("precompute_facet_features\t:error setting up facet");
	}
	*scale_Ptr = 1.0/tmp;

	facet_Ptr->store[0]=(Dv.y);
	facet_Ptr->store[1]=(-Dv.x);
	facet_Ptr->store[2]=(-Du.y);
	facet_Ptr->store[3]=(Du.x);
/*
**	distance from origin to plane
*/
	facet_Ptr->dw= -v_dot(Tri[0],facet_Ptr->normal);

	return(1);
}



/*
**	point on plane
*/

int	ray_to_plane(D_Ptr,dw_Ptr,normal_Ptr,ray)
double	*D_Ptr, *dw_Ptr;
triplet	*normal_Ptr;
Ray	*ray;
{
	double	a,b;

	a=V_dot( ray->direction , *normal_Ptr );
	if(a==0.0)return(0);	/* ray parallel to plane */
	b=V_dot(ray->origin,*normal_Ptr)+ *dw_Ptr;
	if(b==0.0)return(0);	/* ray on plane */
	
	*D_Ptr= -b/a;
	if( *D_Ptr <0.0)return(0);
	return(1);
}

/*
**	point on plane
*/

int	double_ray_to_plane(D_Ptr,dw_Ptr,normal_Ptr,ray)
double	*D_Ptr, *dw_Ptr;
triplet	*normal_Ptr;
D_Ray	*ray;
{
	double	a,b;

	a=v_dot( ray->direction , *normal_Ptr );
	if(a==0.0)return(0);	/* ray parallel to plane */
	b=v_dot(ray->origin,*normal_Ptr)+ *dw_Ptr;
	if(b==0.0)return(0);	/* ray on plane */
	
	*D_Ptr= -b/a;
	if( *D_Ptr <0.0){
		if(fabs(a)<RAY_LENGTH_TOL)
			*D_Ptr=fabs(*D_Ptr);
		else 
			return(0);
	}
	return(1);
}


/*
**	point in triangle test
*/

int	point_in_triangle(p_Ptr,facet_Ptr,ray,ray_length,uv)
fFacet	*facet_Ptr;
triplet	*p_Ptr;
double	ray_length,*uv;
Ray	*ray;
{
	pair	v;
	
/*
**	calculate coodinates of point on surface
*/
	*p_Ptr=vector_plus(V_factor(ray->direction,ray_length),ray->origin);
	switch(facet_Ptr->orientation){
		case 0:		/* x-aligned */
		v.x= (p_Ptr->y);
		v.y= (p_Ptr->z);
		break;
		case 1:		/* y-aligned */
		v.x= (p_Ptr->z);
		v.y= (p_Ptr->x);
		break;
		case 2:		/* z-aligned */
		v.x= (p_Ptr->x);
		v.y= (p_Ptr->y);
		break;
	}
/*
**	translate to origin
*/
	v.x -= facet_Ptr->base.x;
	v.y -= facet_Ptr->base.y;
/*
**	calculate local v
*/
	uv[1]= facet_Ptr->store[0] * v.x  + facet_Ptr->store[1] * v.y;
/*
**	out of bounds check
*/
	if(uv[1]<0.0 || uv[1]>1.0)
		return(0);

	uv[0]= facet_Ptr->store[2] * v.x  + facet_Ptr->store[3] * v.y;

	if(uv[0]<0.0-UNITY_TOL || uv[0]>1.0+UNITY_TOL || uv[0]+uv[1] > 1.0+UNITY_TOL)
		return(0);
	return(1);
}

/*
**	point in triangle test
*/

int	double_point_in_triangle(scale_Ptr,p_Ptr,facet_Ptr,ray,ray_length,uv)
fFacet	*facet_Ptr;
triplet	*p_Ptr;
double	ray_length,*uv,*scale_Ptr;
D_Ray	*ray;
{
	pair	v;
	
/*
**	calculate coodinates of point on surface
*/
	*p_Ptr=vector_plus(V_factor(ray->direction,ray_length),ray->origin);
	switch(facet_Ptr->orientation){
		case 0:		/* x-aligned */
		v.x= (p_Ptr->y);
		v.y= (p_Ptr->z);
		break;
		case 1:		/* y-aligned */
		v.x= (p_Ptr->x);
		v.y= (p_Ptr->z);
		break;
		case 2:		/* z-aligned */
		v.x= (p_Ptr->x);
		v.y= (p_Ptr->y);
		break;
	}
/*
**	translate to origin
*/
	v.x -= facet_Ptr->base.x;
	v.y -= facet_Ptr->base.y;
/*
**	calculate local v
*/
	uv[1]= (facet_Ptr->store[0] * v.x  + facet_Ptr->store[1] * v.y)* *scale_Ptr;
/*
**	out of bounds check
*/
	if(uv[1]<0.0 || uv[1]>1.0){
		return(0);
	}

	uv[0]= (facet_Ptr->store[2] * v.x  + facet_Ptr->store[3] * v.y)* *scale_Ptr;
#ifdef DEBUGGER
	printf(" u: %lf v: %lf\n",uv[0],uv[1]);
#endif
	if(uv[0]<0.0 || uv[0]>1.0 || uv[0]+uv[1] > 1.0+UNITY_TOL)
		return(0);
	return(1);
}

