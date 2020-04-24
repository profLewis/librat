#ifndef _VECTORS2_H
#define _VECTORS2_H 
#include "useful3.h"

int	nearly(int exponent,double value);
void	test_program_ok(void);
matrix3 matrix3_ip(triplet v1,triplet v2,triplet v3);
triplet matrix3_mult(matrix3 matrix,triplet vector);
matrix3 matrix3_copy(matrix3 input);
matrix3 matrix3_inverse(matrix3 input);
double m3_inverse(matrix3 *input);
matrix3 inverse3(matrix3 input);
matrix3 transpose3(matrix3 input);
int	matrix_sign(int i);
double scale(matrix3 input);
int	boundck(int input);
double	cofactors(matrix3 input,int row,int column);
int	vector_compare(int precision,triplet v1,triplet v2);
triplet vector_plus(triplet a,triplet b);
triplet vector_minus(triplet a,triplet b);
triplet spherical_to_cartesian(triplet spherical,int mode);
triplet cartesian_to_spherical(triplet cartesian);
double	xy_angle(triplet in);
triplet	multiply_vector(triplet a,triplet b);
triplet	V_factor(triplet a,double b);
triplet normalise(triplet a);
double	V_dot(triplet a,triplet b);
triplet vector_cross(triplet a,triplet b);
double	V_mod(triplet a);
triplet vector_copy(triplet b);
triplet vector_copy2(double c,double d,double e);
triplet	rotate_vector(triplet vect,triplet angles);
triplet	rotate_about(triplet in,triplet u,double theta);
triplet	OLDrotate_about(triplet in,triplet axis,double angle);

#endif /*  _VECTORS2_H */
