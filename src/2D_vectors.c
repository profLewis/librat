/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
#include <stdio.h>
#include <math.h>
#include	"2D_vectors.h"
#include <stdlib.h>


int FsIgN(float input)
{
	if(input<0.0)return(-1);
	if(input==0.0)return(0);
	return(1);
}

pair same_sign(pair a, pair b)
{
	/* force b vector to be in same direction as a */
	pair	out;
	if((FsIgN(a.x)==(-1*FsIgN(b.x)))&&(FsIgN(a.y)==(-1*FsIgN(b.y))))
		out=factor_2D(b,-1.0);
	else
		out=b;
	return(out);
}

pair	normalise_2D(pair a)
{
	float	mod;pair	out;
	mod=mod_2D(a);
	if(mod==0.0)return(copy_2D(-1.0,-1.0));	/* error */
	out=factor_2D(a,1.0/mod);
	return(out);
}

pair	fabs_2D(pair a)
{
	pair	out;
	out=copy_2D(fabs(a.x),fabs(a.y));
	return(out);
}

pair	copy_2D(float x,float y)
{
	pair	out;
	out.x=x;out.y=y;
	return(out);
}

ipair	icopy_2D(int x,int y)
{
	ipair	out;
	out.x=x;out.y=y;
	return(out);
}

pair	*bbox(pair *vectors,int num_vectors)
{
	int i;pair	max,min,*out;
	out=(pair *)calloc(2,sizeof(pair));
	max=vectors[0];min=vectors[0];
	for(i=1;i<num_vectors;i++){
		max=copy_2D(MAX(max.x,vectors[i].x),MAX(max.y,vectors[i].y));
		min=copy_2D(MIN(min.x,vectors[i].x),MIN(min.y,vectors[i].y));
	}
	out[0]=max;out[1]=min;
	return(out);
}

int odd_check(int in)
{
	int hold,out;float	temp;
	hold=in/2;temp=2.0*(float)hold;
	out=(int)temp-in;
	return(out);	/* 0-> even */
}
	
int check_group_belonging(pair way_out,triangle tri,pair test)
{
	int	i,next,out=1,no_of_cross_points=0;
	pair	v1_dir,v2_point,v2_dir;
	float	mu;
	v1_dir=minus_2D(test,way_out);
	for(i=0;i<3;i++){
		next=i+1;if(next==3)next=0;
		v2_point=tri.vertex[i];
		v2_dir=minus_2D(tri.vertex[next],tri.vertex[i]);
		mu=line_distance_2D(way_out,v1_dir,v2_point,v2_dir);
		if(mu<=0.0 && mu <=1.0)no_of_cross_points++;
	}
	if(odd_check(no_of_cross_points)==0)out=0;
	return(out);
}

pair	plus_2D(pair a,pair b)
{
	pair	out;
	out=copy_2D(a.x + b.x,a.y + b.y);
	return(out);
}

ipair	iplus_2D(ipair a,ipair b)
{
	ipair	out;
	out=icopy_2D(a.x + b.x,a.y + b.y);
	return(out);
}

pair	factor_2D(pair a,float factor)
{
	pair	out;
	out=copy_2D(a.x * factor,a.y * factor);
	return(out);	
}

pair	multiply_2D(pair a,pair b)
{
	pair	out;
	out=copy_2D(a.x * b.x,a.y * b.y);
	return(out);	
}

pair	divide_2D(pair a,pair b)
{
	pair	out;
	out=copy_2D(a.x / b.x,a.y / b.y);
	return(out);	
}

pair	minus_2D(pair a,pair b)
{
	pair	out;
	out=copy_2D(a.x - b.x,a.y - b.y);
	return(out);
}

int	compare_2D(pair a,pair b)
{
/*
**	0 -> both different
**	1 -> x same
**	2 -> y same
**	3 -> x,y same
*/
	int	out=0;
	if(a.x==b.x)out+=1;
	if(a.y==b.y)out+=2;
	return(out);
}

float	determinant_2D(matrix_2D in)
{
	float	out;
	out=( in.column[0].x * in.column[1].y )-( in.column[1].x * in.column[0].y );
	return(out);
}
matrix_2D	factor_matrix_2D(matrix_2D in,float factor)
{
	matrix_2D	out;
	out.column[0]=factor_2D(in.column[0],factor);
	out.column[1]=factor_2D(in.column[1],factor);
	return(out);
}

matrix_2D	inverse_2D(matrix_2D in)
{
	matrix_2D	out,temp;float	mod;
        out.column[0].x=out.column[1].x=0.;
        out.column[0].y=out.column[1].y=0.;
	mod=determinant_2D(in);
	if(mod==0.0)return(out);
	temp.column[0].x=in.column[1].y;
	temp.column[1].y=in.column[0].x;
	temp.column[0].y= -1.0*in.column[0].y;
	temp.column[1].x= -1.0*in.column[1].x;
	out=factor_matrix_2D(temp,1.0/mod);
	return(out);
}

float	mod_2D(pair in)
{
	float	out;
	out=sqrt(in.x*in.x + in.y*in.y);
	return(out);
}
matrix_2D	transpose_2D(matrix_2D in)
{
	matrix_2D	out;
	out.column[0].x=in.column[0].x;
	out.column[1].x=in.column[0].y;
	out.column[0].y=in.column[1].x;
	out.column[1].y=in.column[1].y;
	return(out);
}

pair	matrix_mult_2D(matrix_2D matrix,pair v)
{
	pair	out;matrix_2D	temp;
	temp = transpose_2D(matrix);
	out=copy_2D(add_2D(multiply_2D(temp.column[0],v)),add_2D(multiply_2D(temp.column[1],v)));
	return(out);
}

float	add_2D(pair a)
{
	float	out;
	out=a.x + a.y;
	return(out);
}

ipair	f2i_2D(pair in)
{
	ipair	out;
	out.x=(int)in.x;out.y=(int)in.y;
	return(out);
}

pair	i2f_2D(ipair in)
{
	pair	out;
	out.x=(float)in.x;out.y=(float)in.y;
	return(out);
}	

pair	lo_allign_to_grid(pair in,pair spacing,pair origin)
{
	pair	position,out,lesser_position;
	position=divide_2D(minus_2D(in,origin),spacing);
	lesser_position=i2f_2D(f2i_2D(position));
/*	switch((char)compare_2D(position,lesser_position)){
		case 0:	/ 	both x,y different 	**
				**	 so not integer		/
		out=plus_2D(lesser_position,copy_2D(1.0,1.0));break;
		case 1:	/ 	x same	 		**
				**	 so not integer		/
		out=plus_2D(lesser_position,copy_2D(0.0,1.0));break;
		case 2:	/ 	y same	 		**
				**	 so not integer		/
		out=plus_2D(lesser_position,copy_2D(1.0,0.0));break;
		case 3:	/ 	both x,y same 		**
				**	 so not integer		/
		out=lesser_position;break;
	}
*/
	out=lesser_position;
	return(out);
}

pair	hi_allign_to_grid(pair in,pair spacing,pair origin)
{
	pair	position,out;
	position=divide_2D(minus_2D(in,origin),spacing);
	out=i2f_2D(f2i_2D(position));
	return(out);
}

pair	line_intersection_2D(pair v1_point,pair v1_dir,pair v2_point,pair v2_dir)
{
	pair	out,lambda_mu;
	float	lambda,mu;
	matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=matrix_mult_2D(inverse_2D(matrix),minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=plus_2D(v1_point,factor_2D(v1_dir,lambda));
	return(out);
}

float	line_distance_2D(pair v1_point,pair v1_dir,pair v2_point,pair v2_dir)
{
	pair	out,lambda_mu;
	float	lambda,mu;
	matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=matrix_mult_2D(inverse_2D(matrix),minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=plus_2D(v1_point,factor_2D(v1_dir,lambda));
	return(mu);
}
triangle	quick_affine(int triangle_number,pair v0,pair v1,pair v2)
{
	triangle	out;
	float	mod_matrix;
	mod_matrix=(v1.x*v2.y - v2.x*v1.y) - v0.x*(v2.y-v1.y) + v0.y*(v2.x - v1.x);
	if(mod_matrix==0){
		fprintf(stderr,"triangle %d incorrectly defined\n",triangle_number);exit(1);
	}
	out.vertex[0].x=(v0.x*v1.y-v0.y*v1.x)/mod_matrix;
	out.vertex[0].y= -(v0.x*v2.y-v0.y*v2.x)/mod_matrix;
	out.vertex[1].x= -(v1.y-v0.y)/mod_matrix;
	out.vertex[1].y=(v1.x-v0.x)/mod_matrix;
	out.vertex[2].x=(v2.y-v0.y)/mod_matrix;
	out.vertex[2].y= -(v2.x-v0.x)/mod_matrix;
	return(out);
}

pair	affine_transform(triangle trans_matrix,pair point)
{
	pair	out;
	out.x=add_2D(multiply_2D(trans_matrix.vertex[1],point))+trans_matrix.vertex[0].x;
	out.y=add_2D(multiply_2D(trans_matrix.vertex[2],point))+trans_matrix.vertex[0].y;
	return(out);
}

pair	backwards_affine_transform(triangle trans_matrix,pair point)
{
	pair	out;
	matrix_2D	in,matrix;
	in.column[0]=trans_matrix.vertex[1];
	in.column[1]=trans_matrix.vertex[2];
	matrix=inverse_2D(in);
	out.x=add_2D(multiply_2D(matrix.column[0],minus_2D(point,trans_matrix.vertex[0])));
	out.y=add_2D(multiply_2D(matrix.column[1],minus_2D(point,trans_matrix.vertex[0])));
	return(out);
}
