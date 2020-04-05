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

int	clock_count();
double	modulus_matrix4();
triplet	multiply_matrix4_vector3(),multiply_upper_matrix3_vector3(),multiply_matrix4_vector3();
Vector4	vector_copy4(),multiply_matrix4_vector4();
Matrix4	load_identity_matrix4(),load_translation_matrix4(),load_scaling_matrix4(),load_differential_scaling_matrix4(),load_x_axis_rotation_matrix4(),load_y_axis_rotation_matrix4(),load_z_axis_rotation_matrix4(),load_scaling_fix_point_matrix4(),load_differential_scaling_fix_point_matrix4(),load_x_axis_rotation_fix_point_matrix4(),load_y_axis_rotation_fix_point_matrix4(),load_z_axis_rotation_fix_point_matrix4(),multiply_matrix4_matrix4(),transpose4(),inverse_matrix4(),read_matrix4(),load_arbitrary_axis_rotation_matrix4();

#endif /* _4D_VECTORS_H */
