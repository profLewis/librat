/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
#include <stdio.h>
#include <math.h>
#include "useful4.h"
#include "2D_vectors_double.h"

INT	FSIgN();

INT FSIgN(input)
double	input;
{
	if(input<0.0)return(-1);
	if(input==0.0)return(0);
	return(1);
}

Pair Same_sign(a,b)
Pair a,b;
{
	/* force b vector to be in same direction as a */
	Pair	out;
	if((FSIgN(a.x)==(-1*FSIgN(b.x)))&&(FSIgN(a.y)==(-1*FSIgN(b.y))))
		out=Factor_2D(b,-1.0);
	else
		out=b;
	return(out);
}

Pair	Normalise_2D(a)
Pair	a;
{
	double	mod;Pair	out;
	mod=Mod_2D(a);
	if(mod==0.0)return(Copy_2D(-2.0,-2.0));	/* error */
	out=Factor_2D(a,1.0/mod);
	return(out);
}

Pair	Copy_2D(x,y)
double	x,y;
{
	Pair	out;
	out.x=x;out.y=y;
	return(out);
}

Pair	Fabs_2D(a)
Pair	a;
{
	Pair	out,Copy_2D();
	out=Copy_2D(fabs(a.x),fabs(a.y));
	return(out);
}

Pair	*Bbox(vectors,num_vectors)
Pair	*vectors;INT	num_vectors;
{
	INT	i;Pair	max,min,*out;
	out=(Pair *)calloc(2,sizeof(Pair));
	max=vectors[0];min=vectors[0];
	for(i=1;i<num_vectors;i++){
		max=Copy_2D(MAX(max.x,vectors[i].x),MAX(max.y,vectors[i].y));
		min=Copy_2D(MIN(min.x,vectors[i].x),MIN(min.y,vectors[i].y));
	}
	out[0]=max;out[1]=min;
	return(out);
}

INT Check_group_belonging(way_out,tri,test)
Pair	way_out,test;Triangle	tri;
{
	INT	i,Odd_check(),next,out=1,no_of_cross_points=0;
	Pair	v1_dir,v2_point,v2_dir;
	double	mu;
	v1_dir=Minus_2D(test,way_out);
	for(i=0;i<3;i++){
		next=i+1;if(next==3)next=0;
		v2_point=tri.vertex[i];
		v2_dir=Minus_2D(tri.vertex[next],tri.vertex[i]);
		mu=Line_distance_2D(way_out,v1_dir,v2_point,v2_dir);
		if(mu<=0.0 && mu <=1.0)no_of_cross_points++;
	}
	if((INT)Odd_check(no_of_cross_points)==0)out=0;
	return(out);
}

INT Odd_check(in)
INT	in;
{
	INT	hold,out;double	temp;
	hold=in/2;temp=2.0*(double)hold;
	out=(INT)temp-in;
	return(out);	/* 0-> even */
}
	
Pair	Plus_2D(a,b)
Pair	a,b;
{
	Pair	out;
	out=Copy_2D(a.x + b.x,a.y + b.y);
	return(out);
}

Pair	Factor_2D(a,factor)
Pair	a;double	factor;
{
	Pair	out;
	out=Copy_2D(a.x * factor,a.y * factor);
	return(out);	
}

Pair	Multiply_2D(a,b)
Pair	a,b;
{
	Pair	out;
	out=Copy_2D(a.x * b.x,a.y * b.y);
	return(out);	
}

Pair	Divide_2D(a,b)
Pair	a,b;
{
	Pair	out;
	out=Copy_2D(a.x / b.x,a.y / b.y);
	return(out);	
}

Pair	Minus_2D(a,b)
Pair	a,b;
{
	Pair	out;
	out=Copy_2D(a.x - b.x,a.y - b.y);
	return(out);
}

INT Compare_2D(a,b)
Pair	a,b;
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

double	Determinant_2D(in)
Matrix_2D	in;
{
	double	out;
	out=( in.column[0].x * in.column[1].y )-( in.column[1].x * in.column[0].y );
	return(out);
}

Matrix_2D	Factor_Matrix_2D(in,factor)
Matrix_2D	in;double	factor;
{
	Matrix_2D	out;
	out.column[0]=Factor_2D(in.column[0],factor);
	out.column[1]=Factor_2D(in.column[1],factor);
	return(out);
}

Matrix_2D	Inverse_2D(in)
Matrix_2D	in;
{
	Matrix_2D	out,temp;double	mod;
	mod=Determinant_2D(in);
	if(mod==0.0)return(out);
	temp.column[0].x=in.column[1].y;
	temp.column[1].y=in.column[0].x;
	temp.column[0].y=-1.0*in.column[0].y;
	temp.column[1].x=-1.0*in.column[1].x;
	out=Factor_Matrix_2D(temp,1.0/mod);
	return(out);
}

double	Mod_2D(in)
Pair	in;
{
	double	out;
	out=sqrt(in.x*in.x + in.y*in.y);
	return(out);
}

Matrix_2D	Transpose_2D(in)
Matrix_2D	in;
{
	Matrix_2D	out;
	out.column[0].x=in.column[0].x;
	out.column[1].x=in.column[0].y;
	out.column[0].y=in.column[1].x;
	out.column[1].y=in.column[1].y;
	return(out);
}

Pair	Matrix_mult_2D(matrix,v)
Matrix_2D	matrix;Pair	v;
{
	Pair	out;Matrix_2D	temp;
	temp = Transpose_2D(matrix);
	out=Copy_2D(Add_2D(Multiply_2D(temp.column[0],v)),Add_2D(Multiply_2D(temp.column[1],v)));
	return(out);
}

double	Add_2D(a)
Pair	a;
{
	double	out;
	out=a.x + a.y;
	return(out);
}

ipair	F2i_2D(in)
Pair	in;
{
	ipair	out;
	out.x=(INT)in.x;out.y=(INT)in.y;
	return(out);
}

Pair	I2f_2D(in)
ipair	in;
{
	Pair	out;
	out.x=(double)in.x;out.y=(double)in.y;
	return(out);
}	

Pair	Lo_allign_to_grid(in,spacing,origin)
Pair	in,spacing,origin;
{
	Pair	position,out,lesser_position;
	position=Divide_2D(Minus_2D(in,origin),spacing);
	lesser_position=I2f_2D(F2i_2D(position));
/*	switch(Compare_2D(position,lesser_position)){
		case 0:	/ 	both x,y different 	**
				**	 so not integer		/
		out=Plus_2D(lesser_position,Copy_2D(1.0,1.0));break;
		case 1:	/ 	x same	 		**
				**	 so not integer		/
		out=Plus_2D(lesser_position,Copy_2D(0.0,1.0));break;
		case 2:	/ 	y same	 		**
				**	 so not integer		/
		out=Plus_2D(lesser_position,Copy_2D(1.0,0.0));break;
		case 3:	/ 	both x,y same 		**
				**	 so not integer		/
		out=lesser_position;break;
	}
*/
	out=lesser_position;
	return(out);
}

Pair	Hi_allign_to_grid(in,spacing,origin)
Pair	in,spacing,origin;
{
	Pair	position,out;
	position=Divide_2D(Minus_2D(in,origin),spacing);
	out=I2f_2D(F2i_2D(position));
	return(out);
}

Pair	Line_intersection_2D(v1_point,v1_dir,v2_point,v2_dir)
Pair	v1_point,v1_dir,v2_point,v2_dir;
{
	Pair	out,lambda_mu;
	double	lambda,mu;
	Matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=Matrix_mult_2D(Inverse_2D(matrix),Minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=Plus_2D(v1_point,Factor_2D(v1_dir,lambda));
	return(out);
}

double	Line_distance_2D(v1_point,v1_dir,v2_point,v2_dir)
Pair	v1_point,v1_dir,v2_point,v2_dir;
{
	Pair	out,lambda_mu;
	double	lambda,mu;
	Matrix_2D	matrix;
	matrix.column[0]=v1_dir;matrix.column[1]=v2_dir;
	lambda_mu=Matrix_mult_2D(Inverse_2D(matrix),Minus_2D(v1_point,v2_point));
	lambda=lambda_mu.x;mu=lambda_mu.y;
	out=Plus_2D(v1_point,Factor_2D(v1_dir,lambda));
	return(mu);
}

Triangle	Quick_affine(Triangle_number,v0,v1,v2)
Pair	v0,v1,v2;INT	Triangle_number;
{
	Triangle	out;
	void	exit();
	double	mod_matrix;
	mod_matrix=(v1.x*v2.y - v2.x*v1.y) - v0.x*(v2.y-v1.y) + v0.y*(v2.x - v1.x);
	if(mod_matrix==0){
		fprintf(stderr,"Triangle %d incorrectly defined\n",Triangle_number);exit(-4);
	}
	out.vertex[0].x=(v0.x*v1.y-v0.y*v1.x)/mod_matrix;
	out.vertex[0].y=-(v0.x*v2.y-v0.y*v2.x)/mod_matrix;
	out.vertex[1].x=-(v1.y-v0.y)/mod_matrix;
	out.vertex[1].y=(v1.x-v0.x)/mod_matrix;
	out.vertex[2].x=(v2.y-v0.y)/mod_matrix;
	out.vertex[2].y=-(v2.x-v0.x)/mod_matrix;
	return(out);
}
Pair	Affine_transform(trans_matrix,point)
Triangle	trans_matrix;Pair	point;
{
	Pair	out;
	out.x=Add_2D(Multiply_2D(trans_matrix.vertex[1],point))+trans_matrix.vertex[0].x;
	out.y=Add_2D(Multiply_2D(trans_matrix.vertex[2],point))+trans_matrix.vertex[0].y;
	return(out);
}
Pair	Backwards_affine_transform(trans_matrix,point)
Triangle	trans_matrix;Pair	point;
{
	Pair	out;
	Matrix_2D	in,matrix;
	in.column[0]=trans_matrix.vertex[1];
	in.column[1]=trans_matrix.vertex[2];
	matrix=Inverse_2D(in);
	out.x=Add_2D(Multiply_2D(matrix.column[0],Minus_2D(point,trans_matrix.vertex[0])));
	out.y=Add_2D(Multiply_2D(matrix.column[1],Minus_2D(point,trans_matrix.vertex[0])));
	return(out);
}
