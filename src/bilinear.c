#include "prat.h"
/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

/*
**	point on bilinear patch
*/

int	ray_on_BiLinearPatch(D_Ptr,patch_Ptr,ray,r)
double	*D_Ptr,*r;
Ray	*ray;
BiLinearPatch	*patch_Ptr;
{
	double	s,t,det,m3_inverse(),lambda,mu,Random();
	triplet	a,b,c,d,o,A,B,C,D,O,R,vector_cross(),vector_copy();
	matrix3	M;

	A=vector_minus(patch_Ptr->corner[1],patch_Ptr->corner[0]);
	B=vector_minus(patch_Ptr->corner[3],patch_Ptr->corner[0]);
	a=vector_plus(patch_Ptr->corner[0],patch_Ptr->corner[2]);
	b=vector_plus(patch_Ptr->corner[1],patch_Ptr->corner[3]);
	C=vector_minus(a,b);
	D=V_factor(ray->direction,-1.0);
/* observation vector */
	O=vector_minus(ray->origin,patch_Ptr->corner[0]);



	a=vector_cross(A,B);
	c=vector_cross(C,B);
	d=vector_cross(D,B);
	M.data[0][0]=a.x;
	M.data[0][1]=a.y;
	M.data[0][2]=a.z;
	M.data[1][0]=c.x;
	M.data[1][1]=c.y;
	M.data[1][2]=c.z;
	M.data[2][0]=d.x;
	M.data[2][1]=d.y;
	M.data[2][2]=d.z;
	
	det=m3_inverse(&M);

	if(det){
		O=vector_cross(O,B);
		R=matrix3_mult(M,O);
/*
** R.x = lambda
** R.y = lambda*mu
** R.z = d
*/
		if(R.x<0.||R.x>1.||R.y<0.||R.y>1.||(*D_Ptr=R.z)<0.)
			return(0);

		if(R.x==0.){
/* need to recalculate mu */
			lambda=0.;
			O=vector_minus(ray->origin,patch_Ptr->corner[0]);
			A=vector_cross(O,ray->direction);
			C=vector_cross(B,ray->direction);
			t=V_mod(A);
			s=V_mod(C);
			if(s!=0){
				mu=t/s;
				if(mu<0||mu>1.)
					return(0);
			}else{
				return(0); /* TODO ... probably */
			}
		}else{
			mu=R.y/R.x;
		}
	}else{
		o=vector_cross(O,A);
		d=vector_cross(D,A);		
		s=V_dot(o,C);
		t=V_dot(d,C);
		if(t==0.0){
			o=vector_cross(O,B);
			d=vector_cross(D,B);		
			s=V_dot(o,C);
			t=V_dot(d,C);
			if(t==0.0)
				return(0);
		}
		*D_Ptr=s/t;
		if(*D_Ptr<0)
			return(0);
/* calculate lambda & mu knowing d */

/* TODO */
	}
/* calculate normal ? bilinear interpolation of corner normals */
	A=V_factor(vector_minus(patch_Ptr->corner[1],patch_Ptr->corner[0]),lambda);
	B=V_factor(vector_minus(patch_Ptr->corner[3],patch_Ptr->corner[2]),lambda);
	patch_Ptr->normal=V_factor(vector_minus(B,A),mu);

/* store lambda, mu as local coordinates */
	if(patch_Ptr->local_coords){
		patch_Ptr->local_coords->vertex_local.x=lambda;
		patch_Ptr->local_coords->vertex_local.y=mu;
	}
/* Vertex_normals not used on patch - need to implement later
   to allow bump mapping onto patch ???
*/
	return(1);
}
