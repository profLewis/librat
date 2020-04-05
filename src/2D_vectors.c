/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
#include <stdio.h>
#include <math.h>
#include	"useful4.h"
#include	"2D_vectors.h"
void *calloc();

INT	FsIgN();

INT FsIgN(input)
FLOAT	input;
{
	if(input<0.0)return(-1);
	if(input==0.0)return(0);
	return(1);
}

PAIR same_sign(a,b)
PAIR a,b;
{
	/* force b vector to be in same direction as a */
	PAIR	out;
	if((FsIgN(a.x)==(-1*FsIgN(b.x)))&&(FsIgN(a.y)==(-1*FsIgN(b.y))))
		out=factor_2D(b,-1.0);
	else
		out=b;
	return(out);
}

PAIR	normalise_2D(a)
PAIR	a;
{
	FLOAT	mod;PAIR	out;
	mod=mod_2D(a);
	if(mod==0.0)return(copy_2D(-1.0,-1.0));	/* error */
	out=factor_2D(a,1.0/mod);
	return(out);
}

PAIR	fabs_2D(a)
PAIR	a;
{
	PAIR	out;
	out=copy_2D(fabs(a.x),fabs(a.y));
	return(out);
}

PAIR	copy_2D(x,y)
FLOAT	x,y;
{
	PAIR	out;
	out.x=x;out.y=y;
	return(out);
}

IPAIR	icopy_2D(x,y)
INT	x,y;
{
	IPAIR	out;
	out.x=x;out.y=y;
	return(out);
}

PAIR	*bbox(vectors,num_vectors)
PAIR	*vectors;INT	num_vectors;
{
	INT	i;PAIR	max,min,*out;
	out=(PAIR *)calloc(2,sizeof(PAIR));
	max=vectors[0];min=vectors[0];
	for(i=1;i<num_vectors;i++){
		max=copy_2D(MAX(max.x,vectors[i].x),MAX(max.y,vectors[i].y));
		min=copy_2D(MIN(min.x,vectors[i].x),MIN(min.y,vectors[i].y));
	}
	out[0]=max;out[1]=min;
	return(out);
}

INT	odd_check(in)
INT	in;
{
	INT	hold,out;FLOAT	temp;
	hold=in/2;temp=2.0*(FLOAT)hold;
	out=(INT)temp-in;
	return(out);	/* 0-> even */
}
	
INT	check_group_belonging(way_out,tri,test)
PAIR	way_out,test;triangle	tri;
{
	INT	i,next,out=1,no_of_cross_points=0;
	PAIR	v1_dir,v2_point,v2_dir;
	FLOAT	mu;
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

PAIR	plus_2D(a,b)
PAIR	a,b;
{
	PAIR	out;
	out=copy_2D(a.x + b.x,a.y + b.y);
	return(out);
}

IPAIR	iplus_2D(a,b)
IPAIR	a,b;
{
	IPAIR	out;
	out=icopy_2D(a.x + b.x,a.y + b.y);
	return(out);
}

PAIR	factor_2D(a,factor)
PAIR	a;FLOAT	factor;
{
	PAIR	out;
	out=copy_2D(a.x * factor,a.y * factor);
	return(out);	
}

PAIR	multiply_2D(a,b)
PAIR	a,b;
{
	PAIR	out;
	out=copy_2D(a.x * b.x,a.y * b.y);
	return(out);	
}

PAIR	divide_2D(a,b)
PAIR	a,b;
{
	PAIR	out;
	out=copy_2D(a.x / b.x,a.y / b.y);
	return(out);	
}

PAIR	minus_2D(a,b)
PAIR	a,b;
{
	PAIR	out;
	out=copy_2D(a.x - b.x,a.y - b.y);
	return(out);
}

INT	compare_2D(a,b)
PAIR	a,b;
{
/*
**	0 -> both different
**	1 -> x same
**	2 -> y same
**	3 -> x,y same
*/
	INT	out=0;
	if(a.x==b.x)out+=1;
	if(a.y==b.y)out+=2;
	return(out);
}

FLOAT	determinant_2D(in)
matrix_2D	in;
{
	FLOAT	out;
	out=( in.column[0].x * in.column[1].y )-( in.column[1].x * in.column[0].y );
	return(out);
}
matrix_2D	factor_matrix_2D(in,factor)
matrix_2D	in;FLOAT	factor;
{
	matrix_2D	out;
	out.column[0]=factor_2D(in.column[0],factor);
	out.column[1]=factor_2D(in.column[1],factor);
	return(out);
}

matrix_2D	inverse_2D(in)
matrix_2D	in;
{
	matrix_2D	out,temp;FLOAT	mod;
	mod=determinant_2D(in);
	if(mod==0.0)return(out);
	temp.column[0].x=in.column[1].y;
	temp.column[1].y=in.column[0].x;
	temp.column[0].y= -1.0*in.column[0].y;
	temp.column[1].x= -1.0*in.column[1].x;
	out=factor_matrix_2D(temp,1.0/mod);
	return(out);
}

FLOAT	mod_2D(in)
PAIR	in;
{
	FLOAT	out;
	out=sqrt(in.x*in.x + in.y*in.y);
	return(out);
}
matrix_2D	transpose_2D(in)
matrix_2D	in;
{
	matrix_2D	out;
	out.column[0].x=in.column[0].x;
	out.column[1].x=in.column[0].y;
	out.column[0].y=in.column[1].x;
	out.column[1].y=in.column[1].y;
	return(out);
}

PAIR	matrix_mult_2D(matrix,v)
matrix_2D	matrix;PAIR	v;
{
	PAIR	out;matrix_2D	temp;
	temp = transpose_2D(matrix);
	out=copy_2D(add_2D(multiply_2D(temp.column[0],v)),add_2D(multiply_2D(temp.column[1],v)));
	return(out);
}

FLOAT	add_2D(a)
PAIR	a;
{
	FLOAT	out;
	out=a.x + a.y;
	return(out);
}

IPAIR	f2i_2D(in)
PAIR	in;
{
	IPAIR	out;
	out.x=(INT)in.x;out.y=(INT)in.y;
	return(out);
}

PAIR	i2f_2D(in)
IPAIR	in;
{
	PAIR	out;
	out.x=(FLOAT)in.x;out.y=(FLOAT)in.y;
	return(out);
}	

PAIR	lo_allign_to_grid(in,spacing,origin)
PAIR	in,spacing,origin;
{
	PAIR	position,out,lesser_position;
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

PAIR	hi_allign_to_grid(in,spacing,origin)
PAIR	in,spacing,origin;
{
	PAIR	position,out;
	position=divide_2D(minus_2D(in,origin),spacing);
	out=i2f_2D(f2i_2D(position));
	return(out);
}

PAIR	line_intersection_2D(v1_point,v1_dir,v2_point,v2_dir)
PAIR	v1_point,v1_dir,v2_point,v2_dir;
{
	PAIR	out,lambda_mu;
	FLOAT	lambda,mu;
	matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=matrix_mult_2D(inverse_2D(matrix),minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=plus_2D(v1_point,factor_2D(v1_dir,lambda));
	return(out);
}

FLOAT	line_distance_2D(v1_point,v1_dir,v2_point,v2_dir)
PAIR	v1_point,v1_dir,v2_point,v2_dir;
{
	PAIR	out,lambda_mu;
	FLOAT	lambda,mu;
	matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=matrix_mult_2D(inverse_2D(matrix),minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=plus_2D(v1_point,factor_2D(v1_dir,lambda));
	return(mu);
}
triangle	quick_affine(triangle_number,v0,v1,v2)
PAIR	v0,v1,v2;INT	triangle_number;
{
	triangle	out;
	FLOAT	mod_matrix;
	void	exit();
	mod_matrix=(v1.x*v2.y - v2.x*v1.y) - v0.x*(v2.y-v1.y) + v0.y*(v2.x - v1.x);
	if(mod_matrix==0){
		fprintf(stderr,"triangle %d incorrectly defined\n",triangle_number);exit(-4);
	}
	out.vertex[0].x=(v0.x*v1.y-v0.y*v1.x)/mod_matrix;
	out.vertex[0].y= -(v0.x*v2.y-v0.y*v2.x)/mod_matrix;
	out.vertex[1].x= -(v1.y-v0.y)/mod_matrix;
	out.vertex[1].y=(v1.x-v0.x)/mod_matrix;
	out.vertex[2].x=(v2.y-v0.y)/mod_matrix;
	out.vertex[2].y= -(v2.x-v0.x)/mod_matrix;
	return(out);
}

PAIR	affine_transform(trans_matrix,point)
triangle	trans_matrix;PAIR	point;
{
	PAIR	out;
	out.x=add_2D(multiply_2D(trans_matrix.vertex[1],point))+trans_matrix.vertex[0].x;
	out.y=add_2D(multiply_2D(trans_matrix.vertex[2],point))+trans_matrix.vertex[0].y;
	return(out);
}

PAIR	backwards_affine_transform(trans_matrix,point)
triangle	trans_matrix;PAIR	point;
{
	PAIR	out;
	matrix_2D	in,matrix;
	in.column[0]=trans_matrix.vertex[1];
	in.column[1]=trans_matrix.vertex[2];
	matrix=inverse_2D(in);
	out.x=add_2D(multiply_2D(matrix.column[0],minus_2D(point,trans_matrix.vertex[0])));
	out.y=add_2D(multiply_2D(matrix.column[1],minus_2D(point,trans_matrix.vertex[0])));
	return(out);
}
