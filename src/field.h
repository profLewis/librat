#ifndef _FIELD_H
#define _FIELD_H

/*
**	field type
*/


typedef	struct  {
	float	mean,sigma;
	} distribution;

typedef struct {
	float		x_origin;
	float		y_origin;
	float		x_extent;
	float		y_extent;
	distribution	row_phase,
			row_spacing,
			row_width;
	int		row_flag;		/* row or random */
	distribution	plant_density;
} Field;

#endif /* _FIELD_H */
