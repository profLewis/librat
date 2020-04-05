#ifndef _ALLOCATE_H
#define _ALLOCATE_H
#include "hips.h"
void	*v_allocate();
triplet	*t_allocate();
double *d_allocate();
int *i_allocate();
Data	D_allocate();
Ivect3	*I_allocate();
float	*f_allocate();
char	*b_allocate();
char	*c_allocate();
Bezier_patch	*B_allocate();
#endif /* _ALLOCATE_H */
