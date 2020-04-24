/*
** needs: 
** useful3.h
** ipair, ipair
** matrix_2D, triangle   
*/

#ifndef _2D_VECTORS_H
#define _2D_VECTORS_H

#include "useful3.h"

/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
int FsIgN(float input);
pair same_sign(pair a, pair b);
pair	normalise_2D(pair a);
pair	fabs_2D(pair a);
pair	copy_2D(float x,float y);
ipair	icopy_2D(int x,int y);
pair	*bbox(pair *vectors,int num_vectors);
int odd_check(int in);
int 	check_group_belonging(pair way_out,triangle tri,pair test);
pair	plus_2D(pair a,pair b);
ipair	iplus_2D(ipair a,ipair b);
pair	factor_2D(pair a,float factor);
pair	multiply_2D(pair a,pair b);
pair	divide_2D(pair a,pair b);
pair	minus_2D(pair a,pair b);
int	compare_2D(pair a,pair b);
float	determinant_2D(matrix_2D in);
matrix_2D	factor_matrix_2D(matrix_2D in,float factor);
matrix_2D	inverse_2D(matrix_2D in);
float	mod_2D(pair in);
matrix_2D	transpose_2D(matrix_2D in);
pair	matrix_mult_2D(matrix_2D matrix,pair v);
float	add_2D(pair a);
ipair	f2i_2D(pair in);
pair	i2f_2D(ipair in);
pair	lo_allign_to_grid(pair in,pair spacing,pair origin);
pair	hi_allign_to_grid(pair in,pair spacing,pair origin);
pair	line_intersection_2D(pair v1_point,pair v1_dir,pair v2_point,pair v2_dir);
float	line_distance_2D(pair v1_point,pair v1_dir,pair v2_point,pair v2_dir);
triangle	quick_affine(int triangle_number,pair v0,pair v1,pair v2);
pair	affine_transform(triangle trans_matrix,pair point);
pair	backwards_affine_transform(triangle trans_matrix,pair point);
#endif /* _2D_VECTORS_H */
