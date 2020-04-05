#ifndef _VECTORS3_H
#define _VECTORS3_H

Double	V_mod(),V_dot(),xy_angle();
Triplet rotate_about(),rotate_vector(),multiply_vector(),spherical_to_cartesian(),cartesian_to_spherical(),V_factor(),vector_plus(),vector_minus(),normalise(),vector_cross(),vector_copy(),vector_cross(),matrix3_mult(),vector_copy2();

MATRIX3 matrix3_ip(),matrix3_copy(),inverse3(),transpose3();
FLOAT 	scale(),cofactors();

#endif /* _VECTORS3_H */
