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
#include <unistd.h>
#include <stdlib.h>
#include "4D_vectors.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
triplet old_multiply_matrix4_vector3(matrix4 m,triplet v)
{
        int     i;
        triplet out;
        double  vout[3];

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


matrix4 load_identity_matrix4(void)
{
	matrix4 out;
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==j)out.data[i][j] = (double)1.0;
		else    out.data[i][j] = (double)0.0;
	}
	return(out);
}

void	pload_identity_matrix4(matrix4 *out)
{
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==j)out->data[i][j] = (double)1.0;
		else    out->data[i][j] = (double)0.0;
	}
	return;
}

matrix4 load_translation_matrix4(triplet t)
{
	matrix4 out;
	out = load_identity_matrix4();
	out.data[0][3] = t.x;
	out.data[1][3] = t.y;
	out.data[2][3] = t.z;
	return(out);
}

matrix4 multiply_matrix4_matrix4(matrix4 a,matrix4 b)
{
	matrix4 out;
	int	i,j,k;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		out.data[i][j] = 0.0;
		for(k=0;k<4;k++){
			out.data[i][j] += (a.data[i][k] * b.data[k][j]);
		}
	}
	return(out);
}

vector4 vector_copy4(double a,double b,double c,double d)
{
	vector4	out;
	out.v[0]=a;
	out.v[1]=b;
	out.v[2]=c;
	out.v[3]=d;
	return(out);
}

vector4	multiply_matrix4_vector4(matrix4 m,vector4 v)
{
	vector4	out;
	int	i,j;

	for(i=0;i<4;i++){
		out.v[i] = 0.0;
		for(j=0;j<4;j++)
			out.v[i] += m.data[i][j] * v.v[j];
	}
	return(out);
}

triplet	multiply_matrix4_vector3(matrix4 m,triplet v,int translation_flag)
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

matrix4 load_scaling_matrix4(double s)
{
	matrix4 out;
	out = load_identity_matrix4();
	out.data[0][0] = s;
	out.data[1][1] = s;
	out.data[2][2] = s;
	return(out);
}

matrix4 load_differential_scaling_matrix4(triplet s)
{
	matrix4 out;
	out = load_identity_matrix4();
	out.data[0][0] = s.x;
	out.data[1][1] = s.y;
	out.data[2][2] = s.z;
	return(out);
}

matrix4 load_x_axis_rotation_matrix4(double theta)
{
	matrix4 out;
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

matrix4 load_y_axis_rotation_matrix4(double theta)
{
	matrix4 out;
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

matrix4 load_z_axis_rotation_matrix4(double theta)
{
	matrix4 out;
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

matrix4 load_scaling_fix_point_matrix4(double s,triplet fix_point)
{
	matrix4 out,t,t1,m;
	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_scaling_matrix4(s);
/*
**	t1 followed by m followed by t
*/
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

matrix4 load_differential_scaling_fix_point_matrix4(triplet s,triplet fix_point)
{
	matrix4 out,t,t1,m;
	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_differential_scaling_matrix4(s);
/*
**	t1 followed by m followed by t
*/
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

matrix4 load_x_axis_rotation_fix_point_matrix4(double theta,triplet fix_point)
{
	matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_x_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));	return(out);
}

matrix4 load_y_axis_rotation_fix_point_matrix4(double theta,triplet fix_point)
{
	matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_y_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));	return(out);
}

matrix4 load_z_axis_rotation_fix_point_matrix4(double theta,triplet fix_point)
{
	matrix4 out,t,t1,m;

	t = load_translation_matrix4(fix_point);
	t1= load_translation_matrix4(V_factor(fix_point,-1.0));
	m = load_z_axis_rotation_matrix4(theta);
	out = multiply_matrix4_matrix4(t,multiply_matrix4_matrix4(m,t1));
	return(out);
}

matrix4 transpose4(matrix4 in)
{
	matrix4 out;
	int	i,j;

	for(i=0;i<4;i++)for(j=0;j<4;j++)
		out.data[i][j] = in.data[j][i];
	return(out);
}

int	clock_count(int mod,int in)
{
/* assumes in +ve */
	int	i,out;
	
	i = in/mod;
	out = in - i*mod;
	return(out);
}

double	modulus_matrix4(matrix4 in)
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

matrix4 inverse_matrix4(matrix4 in)
{
	matrix4 out;
	double	u[100],b[16],a[16];
	int	i,j;


	for(i=0;i<4;i++)for(j=0;j<4;j++)
		a[i*4+j]=(double)in.data[i][j];

/*
**	-> invert for nxn matrix
*/
	matrix_inversion(&a[0],&b[0],&u[0],4,NULL);

	for(i=0;i<4;i++)for(j=0;j<4;j++)
		out.data[i][j]=(double)a[i*4+j];

	return(out);
}

matrix4 read_matrix4(FILE *fp)
{
/*
**	read in 12 values to load matrix
*/
	matrix4 out;
	int	i,j;
	double	temp;

	for(i=0;i<4;i++)for(j=0;j<4;j++){
		if(i==4){
			if(j==4)out.data[j][i]=1;
			else 	out.data[j][i]=0;
		}else{
			if( fscanf(fp,"%lf",&temp)!=1){
                          fprintf(stderr,"error in matrix4 read");
                          exit(1);
                        }
			out.data[j][i] = temp;
		}
	}
	return(out);
}

matrix4 load_arbitrary_axis_rotation_matrix4(triplet axis,double theta,triplet fix_point)
{
/*
**	axis -> unit vector
*/
	matrix4 tm,m;
	double	d;

	d = sqrt(axis.y*axis.y + axis.z*axis.z);
	if(d==0.0){
		if(axis.x==0.0){
                  fprintf(stderr,"load_arbitrary_axis_rotation_matrix4:\tzero axis loaded");
	          exit(1);
   	        }
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

triplet	multiply_upper_matrix3_vector3(matrix4 m,triplet v)
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
