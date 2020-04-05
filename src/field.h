#ifndef _FIELD_H
#define _FIELD_H

/*
**	field type
*/

#include "define_float.h"

typedef	struct  {
	FLOAT	mean,sigma;
	} distribution;

typedef struct {
	FLOAT		x_origin;
	FLOAT		y_origin;
	FLOAT		x_extent;
	FLOAT		y_extent;
	distribution	row_phase,
			row_spacing,
			row_width;
	INT		row_flag;		/* row or random */
	distribution	plant_density;
} Field;

#endif /* _FIELD_H */
