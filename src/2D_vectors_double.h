#ifndef _2D_VECTORS_DOUBLE_H
#define _2D_VECTORS_DOUBLE_H
/*
#
#	2D vector definitions
#
#	Lewis Apr 4 1990
#
*/
Pair		Same_sign(),Fabs_2D(),Copy_2D(),*Bbox(),Plus_2D(),Factor_2D(),Multiply_2D(),Divide_2D(),Minus_2D(),Factor_2D(),Normalise_2D(),Matrix_mult_2D(),I2f_2D(),Lo_allign_to_grid(),Hi_allign_to_grid(),Line_intersection_2D(),Affine_transform(),Backwards_affine_transform();
INT		Compare_2D();
Matrix_2D	Factor_matrix_2D(),Inverse_2D(),Transpose_2D();
IPAIR		F2i_2D();
DOUBLE		Mod_2D(),Add_2D(),Line_distance_2D(),Determinant_2D();
Triangle	Quick_affine();
#endif /* _2D_VECTORS_DOUBLE_H */
