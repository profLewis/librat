#ifndef _ALLOCATE_H
#define _ALLOCATE_H
#include "useful3.h"
#include "wavefront.h"
#include "hips.h"
triplet	*t_allocate(int blocksize);
char *VM_allocate(unsigned int size,unsigned int blocksize);
void *v_allocate(int size,int blocksize);
int *i_allocate(int size);
Bezier_patch	*B_allocate(int blocksize);
Ivect3	*I_allocate(int blocksize);
float	*f_allocate(int blocksize);
double	*d_allocate(int blocksize);
char	*b_allocate(int blocksize);
char	*c_allocate(int blocksize);
ipair	*ipair_allocate(int blocksize);
Data	D_allocate(int pixel_format,int blocksize);
#endif /* _ALLOCATE_H */
