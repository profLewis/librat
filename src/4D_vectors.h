
#include        <stdio.h>
#include        <math.h>
#include <unistd.h>
#include <stdlib.h>

#include        "vectors2.h"
#include "matrix.h"

#ifndef _4D_VECTORS_H
#define _4D_VECTORS_H
/*
**
**	4-D vectors / matrices library (include file)
**
**	by:	Lewis
**	date:	Mon Jul 22 1991
**
*/

matrix4 load_identity_matrix4(void);
void	pload_identity_matrix4(matrix4 *out);
matrix4 load_translation_matrix4(triplet t);
matrix4 multiply_matrix4_matrix4(matrix4 a,matrix4 b);
vector4 vector_copy4(double a,double b,double c,double d);
vector4	multiply_matrix4_vector4(matrix4 m,vector4 v);
triplet	multiply_matrix4_vector3(matrix4 m,triplet v,int translation_flag);
matrix4 load_scaling_matrix4(double s);
matrix4 load_differential_scaling_matrix4(triplet s);
matrix4 load_x_axis_rotation_matrix4(double theta);
matrix4 load_y_axis_rotation_matrix4(double theta);
matrix4 load_z_axis_rotation_matrix4(double theta);
matrix4 load_scaling_fix_point_matrix4(double s,triplet fix_point);
matrix4 load_differential_scaling_fix_point_matrix4(triplet s,triplet fix_point);
matrix4 load_x_axis_rotation_fix_point_matrix4(double theta,triplet fix_point);
matrix4 load_y_axis_rotation_fix_point_matrix4(double theta,triplet fix_point);
matrix4 load_z_axis_rotation_fix_point_matrix4(double theta,triplet fix_point);
matrix4 transpose4(matrix4 in);
int	clock_count(int mod,int in);
double	modulus_matrix4(matrix4 in);
matrix4 inverse_matrix4(matrix4 in);
matrix4 read_matrix4(FILE *fp);
matrix4 load_arbitrary_axis_rotation_matrix4(triplet axis,double theta,triplet fix_point);
triplet	__multiply_matrix4_vector3(matrix4 m,triplet v);
triplet old_multiply_matrix4_vector3(matrix4 m,triplet v);
triplet	multiply_upper_matrix3_vector3(matrix4 m,triplet v);
#endif /* _4D_VECTORS_H */
