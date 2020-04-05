#ifndef _2D_VECTORS_H
#define _2D_VECTORS_H
/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
pair		same_sign(),fabs_2D(),copy_2D(),*bbox(),plus_2D(),factor_2D(),multiply_2D(),divide_2D(),minus_2D();
int		compare_2D();
pair		factor_2D();
matrix_2D	factor_matrix_2D(),inverse_2D(),transpose_2D();
pair		normalise_2D(),matrix_mult_2D();
ipair		icopy_2D(),iplus_2D(),f2i_2D();
pair		i2f_2D(),lo_allign_to_grid(),hi_allign_to_grid(),line_intersection_2D();
double		mod_2D(),add_2D(),line_distance_2D();
triangle	quick_affine();
pair	affine_transform(),backwards_affine_transform();

#endif /* _2D_VECTORS_H */
