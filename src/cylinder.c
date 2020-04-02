#include "frat.h"
/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */



/*
**	point in cylinder
**	glassner p.36
*/

int	ray_on_infinite_cylinder(D_Ptr,cylinder_Ptr,ray,p1_Ptr,p2_Ptr,D)
double	*D_Ptr,*p1_Ptr,*p2_Ptr,*D;
Ray	*ray;
Cylinder	*cylinder_Ptr;
{
	double	a,b,c,q,t,len[2],r[2];
	triplet	A,P,E,F,G;
	int r1,r2,ray_on_disk();

	*D_Ptr=0.;
	if(cylinder_Ptr->ends){
	  if((r1=ray_on_disk(&len[0],&cylinder_Ptr->ends[0],ray,&r[0]))){
	    if(len[0]>0)*D_Ptr=len[0];
	  }
	  if((r2=ray_on_disk(&len[1],&cylinder_Ptr->ends[1],ray,&r[1]))){
	    if((!r1 || len[0]<=0.) && len[1]>0.)*D_Ptr=len[1];
	    else *D_Ptr=MIN(*D_Ptr,len[1]);
	  }

	}else{
	    r1=r2=0;
	}
	A=vector_minus(cylinder_Ptr->origin,ray->origin);
	*p1_Ptr=V_dot(A,cylinder_Ptr->normal);
	P=V_factor(cylinder_Ptr->normal,*p1_Ptr);
	E=vector_minus(P,A);
	*p2_Ptr=V_dot( ray->direction , cylinder_Ptr->normal );
	F=V_factor(cylinder_Ptr->normal,*p2_Ptr);
	G=vector_minus(ray->direction,F);
	
	a=V_dot(G,G);
	b=V_dot(G,E);
	c=V_dot(E,E)-(cylinder_Ptr->r2); /* pre */
	
	q=(b*b - a*c);
	if(q<=0.0 && (r1|| r2)){
	  D[0]=D[1]=-1.;
	  return(1);
	}
	if(q<0.){
	  return(0);
	}
	
	t = sqrt(q);
	D[0] = (-b +t) /(a);
	D[1] = (-b -t) /(a);
	if(D[0]>0&&D[1]>0){
	  if(*D_Ptr>0.)*D_Ptr = MMIN(*D_Ptr,MMIN(D[0],D[1]));
	  else *D_Ptr = MMIN(D[0],D[1]);
	}else if(D[0]>0){
	  if(*D_Ptr>0.)*D_Ptr = MMIN(*D_Ptr,D[0]);
	  else *D_Ptr=D[0];
	}else if(D[1]>0){
	  if(*D_Ptr>0.)*D_Ptr = MMIN(*D_Ptr,D[1]);
	  else *D_Ptr=D[1];
	}else{
	  if(*D_Ptr>0.)return(1);
	  return(0);	
	}
	return(1);

}

int	ray_on_finite_cylinder(cylinder_Ptr,D_Ptr,p1_Ptr,p2_Ptr,D,ray)
double	*D_Ptr,*p1_Ptr,*p2_Ptr,*D;
Cylinder	*cylinder_Ptr;
Ray *ray;
{
	double	h,len[2],r[2],Dmax,Dstore[4],Hstore[4];
	int i,hitEnd= -1,r1,r2,nValid=0,ray_on_disk();
	
	if(cylinder_Ptr->ends){
/* cylinder with bounds */
	  if((r1=ray_on_disk(&len[0],&cylinder_Ptr->ends[0],ray,&r[0]))){
	    if(len[0]>0){
	      hitEnd=0;
	      Hstore[nValid]=0;
	      Dstore[nValid++]=len[0];
	    }
	  }
	  if((r2=ray_on_disk(&len[1],&cylinder_Ptr->ends[1],ray,&r[1]))){
	    if(len[1]>0){
	      if(hitEnd==0)hitEnd=2;else hitEnd=1;
	      Hstore[nValid]=cylinder_Ptr->height;
	      Dstore[nValid++]=len[1];
	    }	   
	  }
	  /* 2: both */
	  if(hitEnd==2){
	    ray->rayLengthThroughObject=fabs(MAX(0.,len[1])-MAX(0.,len[0]));
	    if(len[1]<len[0]){
	      *D_Ptr=len[1];
	      cylinder_Ptr->store_h=cylinder_Ptr->height;
	    }else{
	      *D_Ptr=len[0];
	      cylinder_Ptr->store_h=0;	      
	      /* how to store normal ? */
	    }
	    return(1);
	  }
	}
	if(D[0]>0){
	  h= ( D[0] * *p2_Ptr ) - *p1_Ptr;
	  if(h>=0.0 && h<=cylinder_Ptr->height){
	    Hstore[nValid]=h;
	    Dstore[nValid++]=D[0];
	  }
	}
	if(D[1]>0){
	  h= ( D[1] * *p2_Ptr ) - *p1_Ptr;
	  if(h>=0.0 && h<=cylinder_Ptr->height){
	    Hstore[nValid]=h;
	    Dstore[nValid++]=D[1];
	  }
	}

	if(nValid==0)
	  return(0);
	/* at this point, we have 4 potential hit points 
	** we need to find:
	**        which are valid
	**        what value of h they represent
	**        what the closest and furthest ones are
	*/
	*D_Ptr=Dstore[0];
	Dmax=MAX(Dstore[0],0.);
	cylinder_Ptr->store_h=Hstore[0];
	for(i=1;i<nValid;i++){
	  if(Dstore[i]>Dmax){
	    Dmax=Dstore[i];
	  }
	  if(Dstore[i]<*D_Ptr){
	    *D_Ptr=Dstore[i];
	    cylinder_Ptr->store_h=Hstore[i];
	  }
	}
	if(cylinder_Ptr->ends){
	  if(nValid==1){
	    Dmax=*D_Ptr;
	    *D_Ptr=0.;
	    cylinder_Ptr->store_h= - *p1_Ptr;
	  }
	  ray->rayLengthThroughObject=fabs(Dmax-*D_Ptr);
	}
	return(1);
}
	
	
int	vray_on_finite_cylinder(bb,D_Ptr,lengthToTravel,cylinder_Ptr,ray,p1_Ptr,p2_Ptr,D)
     BigBag *bb;
double	*D_Ptr,*p1_Ptr,*p2_Ptr,*D,*lengthToTravel;
Cylinder	*cylinder_Ptr;
Ray *ray;
{
	double	h[4],DD[2],len[2],r[2];
	int	isOk,ok[4],i,ray_on_disk();

	
	h[0]=h[1]=h[2]=h[3]=-1;
	DD[0]=DD[1]=-1;

	if(cylinder_Ptr->ends){
/* cylinder with bounds */
		if(ray_on_disk(&len[0],&cylinder_Ptr->ends[0],ray,&r[0])){
			h[0]= (len[0] * *p2_Ptr ) - *p1_Ptr;
                        if(h[0]<0 && h[0]+bb->flagbag->ray_length_tol>=0)
                          h[0]=0;
                        if(h[0]>cylinder_Ptr->height && h[0]-bb->flagbag->ray_length_tol<=cylinder_Ptr->height)
                          h[0]=cylinder_Ptr->height;
		}
		if(ray_on_disk(&len[1],&cylinder_Ptr->ends[1],ray,&r[1])){
			h[1]=(len[1] * *p2_Ptr ) - *p1_Ptr;
                        if(h[1]<0 && h[1]+bb->flagbag->ray_length_tol>=0)
                          h[1]=0;
                        if(h[1]>cylinder_Ptr->height && h[1]-bb->flagbag->ray_length_tol<=cylinder_Ptr->height)
                          h[1]=cylinder_Ptr->height;

		}
	}
	h[2]=(D[0] * *p2_Ptr ) - *p1_Ptr;
	h[3]=(D[1] * *p2_Ptr ) - *p1_Ptr;

/* initial set up */
        isOk=0;
        for(i=0;i<4;i++){
          ok[i]=0;
          if(h[i]>=0 && h[i]<=cylinder_Ptr->height){
	    DD[0]=DD[1]=h[i];
            ok[i]=isOk=1;
          }
        }
        if(!isOk)
          return(0);  /* ??? */
	for(i=0;i<4;i++){
		if(ok[i]){
			DD[0]=MMIN(DD[0],h[i]);
                        DD[1]=MMAX(DD[1],h[i]);
		}
	}
	if(DD[0]>=0&&DD[1]>=0){
		DD[0] += *p1_Ptr;
		DD[0] /= *p2_Ptr;
		DD[1] += *p1_Ptr;
		DD[1] /= *p2_Ptr;
                if(DD[0] != DD[1]) 
		  *lengthToTravel=fabs(DD[1]-DD[0]);
                else
                  *lengthToTravel=DD[1]; /* ??? is this true ? */
		return(1);
	}else
		return(0);
}
	
	
