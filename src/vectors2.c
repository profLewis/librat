#include	<stdio.h>
#include	<math.h>
#include	"useful3.h"
#include	"vectors2.h"
#include <unistd.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif

int	nearly(exponent,value)
int	exponent;double	value;
{
	int	i;
	for(i=0;i<exponent;i++)
		value *=10.0;
	i=(int)value;
	return(i);
}

void	test_program_ok()
{
	matrix3	temp,matrix3_ip();static triplet v1 = {0, 1, 1},v2 = {0, 2, 1},v3 = {0, 3, 1};
	temp=matrix3_ip(v1,v2,v3);
	temp=matrix3_ip(v2,v1,v3);
	temp=matrix3_ip(v3,v2,v1);
}

matrix3 matrix3_ip(v1,v2,v3)
triplet v1,v2,v3;
{
	matrix3 out;
	out.data[0][0]=v1.x;
	out.data[1][0]=v1.y;
	out.data[2][0]=v1.z;
	out.data[0][1]=v2.x;
	out.data[1][1]=v2.y;
	out.data[2][1]=v2.z;
	out.data[0][2]=v3.x;
	out.data[1][2]=v3.y;
	out.data[2][2]=v3.z;
	return(out);
}
triplet matrix3_mult(matrix,vector)
matrix3 matrix;triplet vector;
{
	triplet out;
	out.x = V_dot(vector_copy2(matrix.data[0][0],matrix.data[0][1],matrix.data[0][2]),vector);
	out.y = V_dot(vector_copy2(matrix.data[1][0],matrix.data[1][1],matrix.data[1][2]),vector);
	out.z = V_dot(vector_copy2(matrix.data[2][0],matrix.data[2][1],matrix.data[2][2]),vector);
	return(out);
}

matrix3 matrix3_copy(input)
matrix3 input; 
{
	matrix3 out;int	i,j;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
		out.data[i][j]=input.data[i][j];
	return(out);
}

matrix3 matrix3_inverse(input)
matrix3 input;
{
	matrix3 out;int	i,j;double	scale_factor;
	scale_factor=scale(input);
	if(scale_factor==0){
		fprintf(stderr,"libvect:\twarning: matrix scale factor = 0.0 in matrix3_inverse()\n");
	}
	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		out.data[j][i]=cofactors(input,i,j)/scale_factor;
	}
	return(out);
}

double m3_inverse(input)
matrix3 *input;
{
	matrix3 out;INT	i,j;double	scale_factor,Cofactors();
	scale_factor=scale(*input);
	if(scale_factor==0)
		return(scale_factor);

	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		out.data[j][i]=Cofactors(*input,i,j)/scale_factor;
	}
	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		input->data[j][i]=out.data[j][i];
	}
	return(scale_factor);
}


matrix3 inverse3(input)
matrix3 input;
{
	matrix3 out;int	i,j;double	scale_factor;
	scale_factor=scale(input);
	if(scale_factor==0)fprintf(stderr,"libvect:\twarning: matrix scale factor = 0.0 in matrix3_inverse()\n");
	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		out.data[j][i]=cofactors(input,i,j)/scale_factor;
	}
	return(out);
}

matrix3 transpose3(input)
matrix3 input;
{
	matrix3 out;int	i,j;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
		out.data[i][j]=input.data[j][i];
	return(out);
}

int	matrix_sign(i)
int	i;
{
	if(i==0||i==2)return(1);
	return(-1);
}

double scale(input)
matrix3 input;
{
	double	out=0;int	column=0,i;
	for(i=0;i<3;i++){
		out+=input.data[i][column]*cofactors(input,i,column);
	}
	return(out);
}

int	boundck(input)
int	input;
{
	int	out;
	while(input>2)input-=3;
	while(input<0)input+=3;
	out=input;
	return(out);
}


double	cofactors(input,row,column)
matrix3 input;
int	column,row;
{
	double	out;
	out=	input.data[boundck(row+1)][boundck(column+1)]*input.data[boundck(row+2)][boundck(column+2)]- input.data[boundck(row+1)][boundck(column+2)]*input.data[boundck(row+2)][boundck(column+1)];
	return(out);
}

int	vector_compare(precision,v1,v2)
triplet	v1,v2;int	precision;
{
	if((nearly(precision,v1.x)==nearly(precision,v2.x))&&(nearly(precision,v1.y)==nearly(precision,v2.y))&&(nearly(precision,v1.z)==nearly(precision,v2.z)))return(1);
	return(0);
}

triplet vector_plus(a,b)
triplet	a,b;
{
	triplet out;
	out.x = (a.x)+(b.x);
	out.y = (a.y)+(b.y);
	out.z = (a.z)+(b.z);
	return(out);
}

triplet vector_minus(a,b)
triplet	a,b;
{
	triplet out;
	out.x = (a.x)-(b.x);
	out.y = (a.y)-(b.y);
	out.z = (a.z)-(b.z);
	return(out);
}

triplet spherical_to_cartesian(spherical,mode)
triplet	spherical;int	mode;
{
/*
**	r,theta,phi
*/
	triplet	out;
	double	y,z;
	switch(mode){
		case 1:
		out.x = (spherical.x)*cos((spherical.z))*cos((spherical.y));
		out.y = (spherical.x)*cos((spherical.z))*sin((spherical.y));
		out.z = (spherical.x)*sin((spherical.z));
		break;
		default:
		z=DTOR(spherical.z);
		y=DTOR(spherical.y);
		out.x = (spherical.x)*cos((z))*cos((y));
		out.y = (spherical.x)*cos((z))*sin((y));
		out.z = (spherical.x)*sin((z));
		break;
	}
	return(out);
}

triplet cartesian_to_spherical(cartesian)
triplet	cartesian;
{
	triplet	out,in;double	m;
	out.x = V_mod(cartesian);			/* r */

/* theta */
	in=vector_copy2(cartesian.x,cartesian.y,0.);
	m=V_mod(in);
	out.z = atan2(cartesian.z,m);

	if(cartesian.y == 0.0){
                if(cartesian.x<0)out.y=M_PI;
                else out.y=0.0;
        }else if(cartesian.x == 0.0){
                if(cartesian.y<0)out.y=-M_PI_2;
                else out.y=M_PI_2;
        }else
		out.y = atan2(cartesian.y,cartesian.x);	/* phi */

	/*if(cartesian.x < 0)
		out.y = M_PI - out.y;*/
	return(out);
}

double	xy_angle(in)
triplet	in;
{ 
	double	out;
	out=atan(in.y/in.x);
	if(in.x < 0)
		out = out-M_PI;
	return(RTOD(out));
}

triplet	multiply_vector(a,b)
triplet a,b;
{
	triplet	out;
	out.x = (a.x)*(b.x);	
	out.y = (a.y)*(b.y);	
	out.z = (a.z)*(b.z);
	return(out);	
}

triplet	V_factor(a,b)
triplet a;double	b;
{
	triplet	out;
	out.x = (a.x)*b;	
	out.y = (a.y)*b;	
	out.z = (a.z)*b;
	return(out);	
}

triplet normalise(a)
triplet a;
{
	triplet out;double	mod;
	mod=V_mod(a);
        if(mod==0)return(a);
	out=V_factor(a,1.0/mod);
	return(out);
}

double	V_dot(a,b)
triplet	a,b;
{
	return((a.x)*(b.x)+(a.y)*(b.y)+(a.z)*(b.z));
}

triplet vector_cross(a,b)
triplet	a,b;
{
	triplet out;
	out.x = (a.y)*(b.z)-(a.z)*(b.y);
	out.y = (a.z)*(b.x)-(a.x)*(b.z);
	out.z = (a.x)*(b.y)-(a.y)*(b.x);
	return(out);
}

double	V_mod(a)
triplet	a;
{
	double	p,q,r,out;
	p=(a.x)*(a.x);q=(a.y)*(a.y);r=(a.z)*(a.z);
	out=sqrt(p+q+r);
	return(out);
}

triplet vector_copy(b)
triplet	b;
{
	triplet a;
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
	return(a);
}

triplet vector_copy2(c,d,e)
double	c,d,e;
{
	triplet a;
	a.x = c;
	a.y = d;
	a.z = e;
	return(a);
}

/*
**      rotate
*/
triplet	rotate_vector(vect,angles)
triplet   vect,angles;
{
	double   sa,ca,phi,theta,psi;
	triplet	out,hold;
	phi =  DTOR(angles.x);
	theta =  DTOR(angles.y);
	psi =  DTOR(angles.z);
	out=vect;
	ca=cos(phi);sa=sin(phi);
        out.y = (vect.y)*ca + (vect.z)*sa;
        out.z = (vect.z)*ca - (vect.y)*sa;
	ca=cos(theta);sa=sin(theta);
	hold=out;
        out.z = (hold.z)*ca + (hold.x)*sa;
        out.x = (hold.x)*ca - (hold.z)*sa;
	ca=cos(psi);sa=sin(psi);
	hold=out;
        out.x = (hold.x)*ca + (hold.y)*sa;
        out.y = (hold.y)*ca - (hold.x)*sa;        
	return(out);
}

triplet	rotate_about(in,u,theta) /* FVD p. 227 */
triplet	in,u;double	theta;
{
	triplet t[3],out;
	double	cosTheta,sinTheta,ux2,uy2,uz2,a,b,c,d,e,f;

	theta=DTOR(theta);

	cosTheta=cos(theta);
	sinTheta=sin(theta);
	ux2=u.x*u.x;
	uy2=u.y*u.y;
	uz2=u.z*u.z;

	a=u.x*u.y*(1.-cosTheta);
	b=u.z*sinTheta;
	c=u.z*u.x*(1.-cosTheta);
	d=u.y*sinTheta;
	e=u.y*u.z*(1.-cosTheta);
	f=u.x*sinTheta;

	t[0].x = ux2 + cosTheta*(1.-ux2);
	t[0].y = a-b;
	t[0].z = c+d;

	t[1].x = a+b;
	t[1].y = uy2 + cosTheta*(1.-uy2);
	t[1].z = e-f;

	t[2].x = c-d;
	t[2].y = e+f;
	t[2].z = uz2 + cosTheta*(1.-uz2);

	out.x = V_dot(in,t[0]);
	out.y = V_dot(in,t[1]);
	out.z = V_dot(in,t[2]);

	return(out);
}


triplet	OLDrotate_about(in,axis,angle)
triplet	in,axis;double	angle;
{
	triplet	t1,t2,t3,out,x_dash,y_dash_cap,x_dash_cap,a_cap;
	double	phi,theta,r,a,cos_phi;
	r=V_mod(in);a=V_mod(axis);
	out=vector_copy2(0.0,0.0,0.0);
	if((a==0)||(r==0))return(out);
	cos_phi=MAX(-1.0,MIN(1.0,V_dot(axis,in)/(r*a)));
	phi=acos(cos_phi);
	if(phi==0.0 || phi==M_PI)return(in);	
	theta=DTOR(angle);
	x_dash=V_factor(vector_minus(in,V_factor(axis,r*cos_phi)),1.0/(r*sin(phi)));
	x_dash_cap=normalise(x_dash);
	a_cap=normalise(axis);
	y_dash_cap=vector_cross(a_cap,x_dash_cap);
	t1=V_factor(x_dash_cap,cos(theta));
	t2=V_factor(y_dash_cap,sin(theta));
	t3=vector_plus(t1,t2);
	t1=V_factor(t3,r*sin(phi));
	t2=V_factor(a_cap,r*cos_phi);
	out=vector_plus(t1,t2);
	return(out);
}

