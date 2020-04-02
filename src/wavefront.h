/*
**
**
**	wavefront.h
** 
**	wavefront include file
**
**
**
*/
#ifndef _WAVEFRONT_
#define _WAVEFRONT_
#define BIG 1000000000.0
#define RAT_N_BLOCKS 1000

typedef struct {
	int	vertex[3];
}Ivect3;
typedef struct {
	int	vertex[16];
}Bezier_patch;
typedef struct {
	int		new;
	int		block;
	int		no_of_vertex_numbers;
	Ivect3		*triangles[200];
}Plant_triangles;
typedef struct {
	int		new;
	int		block;
	int		no_of_vertex_numbers;
	/*ezier_patch	*Bezier_patch[200];*/
}Plant_bzp;
typedef struct {
	double		leaf_area;
	double		projected_leaf_area;
	int		*zeniths;
	int		*look_zeniths;
	int		*azimuths;
}Plant_area;

/* plant definition */
typedef struct {
	int		in;
	int		blocksize;
	triplet		bbox_min;
	triplet		bbox_max;
	Plant_triangles	plant_triangles;
	Plant_bzp	plant_bzp;
	Plant_area	plant_area;
}Plant;

/* clone definition */
typedef struct {
/* note: assumes clone of 1 plant only */
	int		in;
	int		new;
	triplet		displacement;
	int		plant_number;
	Plant_area	plant_area;
}Clone;

/* vertex store definition */
typedef struct {
	int		new[RAT_N_BLOCKS];
	int		block;
	int		no_of_vertices;
	int		blocksize;
	triplet		*vertices[RAT_N_BLOCKS];
}VeRtIcEs;

typedef Ivect3 IVECT3;
#endif
