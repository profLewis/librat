/*
**
**	4-D vectors / matrices library
**
**	by:	Lewis
**	date:	Mon Jul 22 1991
**
*/

#include	<stdio.h>
#include	<math.h>
#include	"useful3.h"
#include	"vectors2.h"
#include <unistd.h>
#include <stdlib.h>
#include "matrix.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void	error1();

Matrix4 load_identity_matrix4()
{
	Matrix4 out;
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==j)out.data[i][j] = (double)1.0;
		else    out.data[i][j] = (double)0.0;
	}
	return(out);
}

void	pload_identity_matrix4(out)
Matrix4 *out;
{
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==j)out->data[i][j] = (double)1.0;
		else    out->data[i][j] = (double)0.0;
	}
	return;
}

Matrix4 load_translation_matrix4(t)
triplet	t;
{
	Matrix4 out;
	out = load_identity_matrix4();
	out.data[0][3] = t.x;
	out.data[1][3] = t.y;
	out.data[2][3] = t.z;
	return(out);
}

Matrix4 multiply_matrix4_matrix4(a,b)
Matrix4 a,b;
{
	Matrix4 out;
	int	i,j,k;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		out.data[i][j] = 0.0;
		for(k=0;k<4;k++){
			out.data[i][j] += (a.data[i][k] * b.data[k][j]);
		}
	}
	return(out);
}

Vector4 vector_copy4(a,b,c,d)
double	a,b,c,d;
{
	Vector4	out;
	out.v[0]=a;
	out.v[1]=b;
	out.v[2]=c;
	out.v[3]=d;
	return(out);
}

Vector4	multiply_matrix4_vector4(m,v)
Matrix4 m;
Vector4 v;
{
	Vector4	out;
	int	i,j;

	for(i=0;i<4;i++){
		out.v[i] = 0.0;
		for(j=0;j<4;j++)
			out.v[i] += m.data[i][j] * v.v[j];
	}
	return(out);
}

triplet	multiply_matrix4_vector3(m,v,translation_flag)
Matrix4 m;
triplet v;
int	translation_flag;
{
	triplet	out;

	out.x = m.data[0][0]*v.x + m.data[0][1]*v.y + m.data[0][2]*v.z;
	out.y = m.data[1][0]*v.x + m.data[1][1]*v.y + m.data[1][2]*v.z;
	out.z = m.data[2][0]*v.x + m.data[2][1]*v.y + m.data[2][2]*v.z;
	if(translation_flag){
		out = vector_plus(out,vector_copy2(m.data[0][3],m.data[1][3],m.data[2][3]));
	}
	return(out);
}

Matrix4 load_scaling_matrix4(s)
double s;
{
	Matrix4 out;
	out = load_identity_matrix4();
	out.data[0][0] = s;
	out.data[1][1] = s;
	out.data[2][2] = s;
	return(out);
}

Matrix4 load_differential_scaling_matrix4(s)
triplet s;
{
	Matrix4 out;
	out = load_identity_matrix4();
	out.data[0][0] = s.x;
	out.data[1][1] = s.y;
	out.data[2][2] = s.z;
	return(out);
}

Matrix4 load_x_axis_rotation_matrix4(theta)
double	theta;
{
	Matrix4 out;
	double	s,c,th;

	th= DTOR(theta);
	s = sin(th);
	c = cos(th);
	out = load_identity_matrix4();
	out.data[1][1] = c;
	out.data[1][2] = -s;
	out.data[2][1] = s;
	out.data[2][2] = c;
	return(out);
}

Matrix4 load_y_axis_rotation_matrix4(theta)
double	theta;
{
	Matrix4 out;
	double	s,c,th;

	th= DTOR(theta);
	s = sin(th);
	c = cos(th);
	out = load_identity_matrix4();
	out.data[0][0] = c;
	out.data[2][0] = -s;
	out.data[0][2] = s;
	out.data[2][2] = c;
	return(out);
}

Matrix4 load_z_axis_rotation_matrix4(theta)
double	theta;
{
	Matrix4 out;
	double	s,c,th;

	th= DTOR(theta);
	s = sin(th);
	c = cos(th);
	out = load_identity_matrix4();
	out.data[0][0] = c;
	out.data[0][1] = -s;
	out.data[1][0] = s;
	out.data[1][1] = c;
	return(out);
}

Matrix4 load_scaling_fix_point_matrix4(s,fix_point)
double s;
triplet	fix_point;
{
	Matrix4 out,t,t1,m;
	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_scaling_matrix4(s);
/*
**	t1 followed by m followed by t
*/
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

Matrix4 load_differential_scaling_fix_point_matrix4(s,fix_point)
triplet s;
triplet	fix_point;
{
	Matrix4 out,t,t1,m;
	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_differential_scaling_matrix4(s);
/*
**	t1 followed by m followed by t
*/
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

Matrix4 load_x_axis_rotation_fix_point_matrix4(theta,fix_point)
double	theta;
triplet	fix_point;
{
	Matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_x_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));	return(out);
}

Matrix4 load_y_axis_rotation_fix_point_matrix4(theta,fix_point)
double	theta;
triplet	fix_point;
{
	Matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_y_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));	return(out);
}

Matrix4 load_z_axis_rotation_fix_point_matrix4(theta,fix_point)
double	theta;
triplet	fix_point;
{
	Matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_z_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

Matrix4 transpose4(in)
Matrix4 in;
{
	Matrix4 out;
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++)
		out.data[i][j] = in.data[j][i];
	return(out);
}

int	clock_count(mod,in)
int	mod,in;
{
/* assumes in +ve */
	int	i,out;
	
	i = in/mod;
	out = in - i*mod;
	return(out);
}

double	modulus_matrix4(in)
Matrix4 in;
{
	matrix3 temp;
	int	i,j,k;
	double	out=0.0;

	for(i=0;i<4;i++){
		for(j=0;j<3;j++)for(k=0;k<3;k++)
			temp.data[j][k] = in.data[clock_count(4,i+j+1)][k+1];
		out += in.data[i][0]*scale(temp);
	}
	return(out);
}

Matrix4 inverse_matrix4(in)
Matrix4 in;
{
	Matrix4 out;
	double	det=0,u[100],b[16],a[16];
	int	i,j;
	void error1();


	for(i=0;i<4;i++)for(j=0;j<4;j++)
		a[i*4+j]=(double)in.data[i][j];

/*
**	-> invert for nxn matrix
*/
	Matrix_inversion(&a[0],&b[0],&u[0],4,&det);

	for(i=0;i<4;i++)for(j=0;j<4;j++)
		out.data[i][j]=(double)a[i*4+j];

	return(out);
}

Matrix4 read_matrix4(fp)
FILE	*fp;
{
/*
**	read in 12 values to load matrix
*/
	Matrix4 out;
	int	i,j;
	double	temp;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==4){
			if(j==4)out.data[j][i]=1;
			else 	out.data[j][i]=0;
		}else{
			if( fscanf(fp,"%lf",&temp)!=1)error1("error in Matrix4 read");
			out.data[j][i] = temp;
		}
	}
	return(out);
}

Matrix4 load_arbitrary_axis_rotation_matrix4(axis,theta,fix_point)
triplet	axis,fix_point;
double	theta;
{
/*
**	axis -> unit vector
*/
	Matrix4 tm,m;
	double	d;

	d = sqrt(axis.y*axis.y + axis.z*axis.z);
	if(d==0.0){
		if(axis.x==0.0)error1("load_arbitrary_axis_rotation_matrix4:\tzero axis loaded");
		return(load_x_axis_rotation_fix_point_matrix4(theta,fix_point));
	}
	tm = load_translation_matrix4(V_factor(fix_point,-1.0));
/*
**	rotate about x to lie in xz plane
*/
	m = load_identity_matrix4();
	m.data[1][1] = axis.z/d;
	m.data[1][2] = axis.y/d;
	m.data[2][1] = -axis.y/d;
	m.data[2][2] = axis.z/d;
	tm = multiply_matrix4_matrix4(tm,m);
/*
**	rotate about y to line up with z-axis
*/
	m = load_identity_matrix4();
	m.data[0][0] = d;
	m.data[0][2] = axis.x;
	m.data[2][0] = -axis.x;
	m.data[2][2] = d;
	tm = multiply_matrix4_matrix4(tm,m);
/*
**	rotate about z using i/p angle
*/
	m = load_z_axis_rotation_matrix4(theta);
	tm = multiply_matrix4_matrix4(tm,m);
/*
**	do inverse of rotation ~ y
*/
	m = load_identity_matrix4();
	m.data[0][0] = d;
	m.data[0][2] = -axis.x;
	m.data[2][0] = axis.x;
	m.data[2][2] = d;
	tm = multiply_matrix4_matrix4(tm,m);
/*
**	do inverse of rotation ~ x
*/
	m = load_identity_matrix4();
	m.data[1][1] = axis.z/d;
	m.data[1][2] = -axis.y/d;
	m.data[2][1] = axis.y/d;
	m.data[2][2] = axis.z/d;
	tm = multiply_matrix4_matrix4(tm,m);
/*
**	do inverse of translation
*/
	m = load_translation_matrix4(fix_point);
	tm = multiply_matrix4_matrix4(tm,m);
	return(tm);
}

/*
**
**	old... redundent...
**

triplet	multiply_matrix4_vector3(m,v)
Matrix4 m;
triplet	v;
{
	int	i,j;
	triplet	out;
	double	vout[3];
	
	for(i=0;i<3;i++){
		vout[i] = v.x * m.data[i][0] + 
			v.y * m.data[i][1] +
			v.z * m.data[i][2] +
			m.data[i][3];
	}
	out.x = vout[0];
	out.y = vout[1];
	out.z = vout[2];
	return(out);

}
*/

triplet	multiply_upper_matrix3_vector3(m,v)
Matrix4 m;
triplet	v;
{
	triplet	out;
	int	i,j;
	double	vector[3],o[3];
	
	vector[0]=v.x;
	vector[1]=v.y;
	vector[2]=v.z;
	for(i=0;i<3;i++){
		o[i]=0.0;
		for(j=0;j<3;j++){
			o[i] += m.data[i][j] * vector[j];
		}
	}
	out.x=o[0];
	out.y=o[1];
	out.z=o[2];	
	return(out);
}
