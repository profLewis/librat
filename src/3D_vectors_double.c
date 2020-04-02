#include	<math.h>
#include	<stdio.h>
#include	"useful4.h"
#include	"3D_vectors_double.h"
#include <unistd.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif

INT	Nearly(exponent,value)
INT	exponent;double	value;
{
	INT	i;
	for(i=0;i<exponent;i++)
		value *=10.0;
	i=(INT)value;
	return(i);
}

void	Test_program_ok()
{
	Matrix3	temp;static Triplet v1 = {0, 1, 1},v2 = {0, 2, 1},v3 = {0, 3, 1};
	temp=Matrix3_ip(v1,v2,v3);
	temp=Matrix3_ip(v2,v1,v3);
	temp=Matrix3_ip(v3,v2,v1);

	
	
}

Matrix3 Matrix3_ip(v1,v2,v3)
Triplet v1,v2,v3;
{
	Matrix3 out;
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
Triplet Matrix3_mult(matrix,vector)
Matrix3 matrix;Triplet vector;
{
	Triplet out;
	out.x = v_dot(Vector_copy2(matrix.data[0][0],matrix.data[0][1],matrix.data[0][2]),vector);
	out.y = v_dot(Vector_copy2(matrix.data[1][0],matrix.data[1][1],matrix.data[1][2]),vector);
	out.z = v_dot(Vector_copy2(matrix.data[2][0],matrix.data[2][1],matrix.data[2][2]),vector);
	return(out);
}

Matrix3 Matrix3_copy(input)
Matrix3 input; 
{
	Matrix3 out;INT	i,j;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
		out.data[i][j]=input.data[i][j];
	return(out);
}

Matrix3 Matrix3_inverse(input)
Matrix3 input;
{
	Matrix3 out;INT	i,j;double	scale_factor;
	scale_factor=Scale(input);
	if(scale_factor==0)fprintf(stderr,"libvect:\twarning: matrix scale factor = 0.0 in Matrix3_inverse()\n");
	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		out.data[j][i]=Cofactors(input,i,j)/scale_factor;
	}
	return(out);
}

double M3_inverse(input)
Matrix3 *input;
{
	Matrix3 out;INT	i,j;double	scale_factor;
	scale_factor=Scale(*input);
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

Matrix3 Inverse3(input)
Matrix3 input;
{
	Matrix3 out;INT	i,j;double	scale_factor;
	scale_factor=Scale(input);
	if(scale_factor==0)fprintf(stderr,"libvect:\twarning: matrix scale factor = 0.0 in Matrix3_inverse()\n");
	for(i=0;i<3;i++)
	for(j=0;j<3;j++){
		out.data[j][i]=Cofactors(input,i,j)/scale_factor;
	}
	return(out);
}

Matrix3 Transpose3(input)
Matrix3 input;
{
	Matrix3 out;INT	i,j;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
		out.data[i][j]=input.data[j][i];
	return(out);
}

INT	Matrix_sign(i)
INT	i;
{
	if(i==0||i==2)return(1);
	return(-1);
}

double Scale(input)
Matrix3 input;
{
	double	out=0;INT	column=0,i;
	for(i=0;i<3;i++){
		out+=input.data[i][column]*Cofactors(input,i,column);
	}
	return(out);
}

INT	Boundck(input)
INT	input;
{
	INT	out;
	while(input>2)input-=3;
	while(input<0)input+=3;
	out=input;
	return(out);
}

double	Cofactors(input,row,column)
Matrix3 input;
INT	column,row;
{
	double	out;
	out=	input.data[Boundck(row+1)][Boundck(column+1)]*input.data[Boundck(row+2)][Boundck(column+2)]-
		input.data[Boundck(row+1)][Boundck(column+2)]*input.data[Boundck(row+2)][Boundck(column+1)];
	return(out);
}

INT	Vector_compare(precision,v1,v2)
Triplet	v1,v2;INT	precision;
{
	if((Nearly(precision,v1.x)==Nearly(precision,v2.x))&&(Nearly(precision,v1.y)==Nearly(precision,v2.y))&&(Nearly(precision,v1.z)==Nearly(precision,v2.z)))return(1);
	return(0);
}

Triplet Vector_plus(a,b)
Triplet	a,b;
{
	Triplet out;
	out.x = (a.x)+(b.x);
	out.y = (a.y)+(b.y);
	out.z = (a.z)+(b.z);
	return(out);
}

Triplet Vector_minus(a,b)
Triplet	a,b;
{
	Triplet out;
	out.x = (a.x)-(b.x);
	out.y = (a.y)-(b.y);
	out.z = (a.z)-(b.z);
	return(out);
}

Triplet Spherical_to_cartesian(spherical,mode)
Triplet	spherical;
INT	mode;
{
/*
**	r,theta,phi
*/
	Triplet	out;
	out.x = (spherical.x)*cos((spherical.z))*cos((spherical.y));
	out.y = (spherical.x)*cos((spherical.z))*sin((spherical.y));
	out.z = (spherical.x)*sin((spherical.z));
	return(out);
}

Triplet Cartesian_to_spherical(cartesian)
Triplet	cartesian;
{
	Triplet	out,in;double	m;
        out.x = v_mod(cartesian);                       /* r */
        out.y = atan2(cartesian.y,cartesian.x); /* theta */
       /* if(cartesian.x < 0)
                out.y = M_PI - out.y; */
        in=Vector_copy2(cartesian.x,cartesian.y,0);
        m=v_mod(in);
        out.z = atan2(cartesian.z,m);
        return(out);
}

double	Xy_angle(in)
Triplet	in;
{ 
	double	out;
	out=atan(in.y/in.x);
	if(in.x < 0)
		out = out-M_PI;
	return(RTOD(out));
}

Triplet	Multiply_vector(a,b)
Triplet a,b;
{
	Triplet	out;
	out.x = (a.x)*(b.x);	
	out.y = (a.y)*(b.y);	
	out.z = (a.z)*(b.z);
	return(out);	
}

Triplet	v_factor(a,b)
Triplet a;double	b;
{
	Triplet	out;
	out.x = (a.x)*b;	
	out.y = (a.y)*b;	
	out.z = (a.z)*b;
	return(out);	
}

Triplet Normalise(a)
Triplet a;
{
	Triplet out;double	mod;
	mod=v_mod(a);
	out=v_factor(a,1.0/mod);
	return(out);
}

double	v_dot(a,b)
Triplet	a,b;
{
	return((a.x)*(b.x)+(a.y)*(b.y)+(a.z)*(b.z));
}

Triplet Vector_cross(a,b)
Triplet	a,b;
{
	Triplet out;
	out.x = (a.y)*(b.z)-(a.z)*(b.y);
	out.y = (a.z)*(b.x)-(a.x)*(b.z);
	out.z = (a.x)*(b.y)-(a.y)*(b.x);
	return(out);
}

double	v_mod(a)
Triplet	a;
{
	double	p,q,r;
	p=(a.x)*(a.x);q=(a.y)*(a.y);r=(a.z)*(a.z);
	return(sqrt(p+q+r));
}

Triplet Vector_copy(b)
Triplet	b;
{
	Triplet a;
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
	return(a);
}

Triplet Vector_copy2(c,d,e)
double	c,d,e;
{
	Triplet a;
	a.x = c;
	a.y = d;
	a.z = e;
	return(a);
}

/*
**      rotate
*/
Triplet	Rotate_vector(vect,angles)
Triplet   vect,angles;
{
	double   sa,ca,phi,theta,psi;
	Triplet	out,hold;
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

Triplet	Rotate_about(in,axis,angle)
Triplet	in,axis;double	angle;
{
	Triplet	t1,t2,t3,out,x_dash,y_dash_cap,x_dash_cap,a_cap;
	double	phi,theta,r,a,cos_phi;
	r=v_mod(in);a=v_mod(axis);
	out=Vector_copy2(0.0,0.0,0.0);
	if((a==0)||(r==0))return(out);
	cos_phi=v_dot(axis,in)/(r*a);
	phi=acos(cos_phi);
	if(phi==0.0 || phi==M_PI)return(in);	/* no rotation */
	theta=DTOR(angle);
	x_dash=v_factor(Vector_minus(in,v_factor(axis,r*cos_phi)),1.0/(r*sin(phi)));
	x_dash_cap=Normalise(x_dash);
	a_cap=Normalise(axis);
	y_dash_cap=Vector_cross(a_cap,x_dash_cap);
	t1=v_factor(x_dash_cap,cos(theta));
	t2=v_factor(y_dash_cap,sin(theta));
	t3=Vector_plus(t1,t2);
	t1=v_factor(t3,r*sin(phi));
	t2=v_factor(a_cap,r*cos_phi);
	out=Vector_plus(t1,t2);
	return(out);
}
