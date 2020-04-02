/*
**	leaf primitive attributes
*/
#ifndef PRMITIVE_ATTRIBUTES_H
#define PRMITIVE_ATTRIBUTES_H
#define MAX_ARRAY 100000
#define STRING_SIZE 20

#include "define_float.h"

typedef struct{
	double	angles[3];
	char	order[3];
}Plus;

typedef struct{
	double length;
	double width;
}AbsoluteParameters;

typedef struct{
	double *value;
	int	np;
	char	mode;
}F_Parameter;
	
struct Functional_Parameters{
	F_Parameter	inclination;
	F_Parameter	azimuth;
	F_Parameter	twist;
	F_Parameter	width;
	F_Parameter	curvature;
    struct Functional_Parameters *X;
};

typedef struct Functional_Parameters FunctionalParameters;

	
/*
**	stem primitive attributes
*/
typedef struct{
/*
**	stem representation:
**		'no_of_stem_sections' stem sections modelled by cylinders
**		joined by spheres
**
*/
/* description parameters */
	char	*stem_material;
/* absolute parameters */
	int	no_of_stem_sections;	/* no of stem sections */
	AbsoluteParameters	*a;
	FunctionalParameters	*f;	
	Data_type	data_type;
	triplet	*centreOfGravity;
	int		nCentreOfGravity;
	double 	*mass;
	double  density;
	double	youngs_modulus;
}Stem;

/* container attributes */
typedef struct{
	triplet	centreOfGravity;
	double	mass;
	int     bboxLevel;
}Container;

/*
**	head primitive attributes
*/
typedef struct{
/*
**	head representation:
**		'no_of_stem_sections' stem sections modelled by cylinders
**		joined by spheres
**
*/
/* link list pointer */
/* description parameters */
	char	*head_material;
	AbsoluteParameters	*a;
	FunctionalParameters	*f;
/* head stem */
	Stem		stalk;	
	Data_type	data_type;
	triplet	*centreOfGravity;
	int		nCentreOfGravity;
	double 	*mass;
	double  density;
	double	youngs_modulus;
}Head;

typedef struct{
/*
**   leaf representation:
**	leaf centre line (described by 
**	{f_leaf_inclination;f_leaf_azimuth;a_leaf_length;leaf_wrap}
**	perpendicular to this is the leaf X-sectional variation,
**	{a_leaf_width;;f_leaf_width;f_leaf_curvature;f_leaf_twist}
**	which is modelled by the arc of a circle
**	The leaf sheath is modelled by a cylinder of length
**	'leaf_sheath_length' and width 'a_leaf_width' oriented in the
**	direction 'local_normal'
**
*/
/* description parameters */
	char	*leaf_material;
/* representation parameters*/

        int     no_of_patches_lengthwise;    	/* no_of_lines of representation */
        int     no_of_patches_crosswise;    	/* no of points per line */
	AbsoluteParameters	*a;
	FunctionalParameters	*f;	
	Stem		leaf_sheath;
	Data_type	data_type;
	triplet	*centreOfGravity;
	int		nCentreOfGravity;
	double 	*mass;
	double  density;
	double	youngs_modulus;
	double	thickness;
}Leaf;

/*
**	bud primitive attributes
*/
typedef struct{
/*
**	bud representation:
**		sphere of radius 'bud_radius'
**
*/
/* description parameters */
	char	*bud_material;
	double	bud_age;
	int	bud_level;
/* absolute parameters */
	double	bud_radius;		/* bud radius */
	Data_type	data_type;
	triplet	*centreOfGravity;
	int		nCentreOfGravity;
	double  density;
	double	youngs_modulus;
	double 	*mass;
}Bud;

typedef struct	Plant {
	char	*axiom;		/* plant topology */
	int	define_flag;		/* set if not to be op */

/*	leaf 	*/
	int			no_of_leaves;
	Leaf			*leaf;
	
/*	stem 	*/
	int			no_of_stems;
	Stem			*stem;
	
/*	head 	*/
	int			no_of_heads;
	Head			*head;
	
/*	bud 	*/
	int			no_of_buds;
	Bud			*bud;
	
/*	rotation */
	int			no_of_rotations;
	Plus			*rotation;

/* containers */
	int			no_of_bboxes;
	Container	*bbox;

	triplet	base_location;
	triplet	direction[3];
	Data_type	data_type;
}Plant;

typedef	struct{
	int	no_of_vertices;
	int	old_no_of_vertices;
	int	no_of_leaves;
	int	no_of_heads;
	int	no_of_stems;
	int	no_of_buds;
	int	no_of_plants;
	int	no_of_rotations;
}counter;

#define	int_VARIABLE	0
#define CHAR_VARIABLE	1
#define double_VARIABLE 	2

typedef struct {
	int	no_of_columns;
	char	***data;
}Variable_data;

typedef struct{
	triplet	plane_location;
	triplet	plane_normal;
	char	plane_material[STRING_SIZE];
}Plane;

typedef struct{
	char  	(*full_flags)[40];
	int	no_of_full_flags;
	char	*Type;
	char	(*subcomponent_flags)[40];
	int	no_of_subcomponent_flags;
}PrimitiveFormat;

extern char bud_flags[][20];
extern	char	BudType[];	/* bud type parameters */
extern int no_of_bud_flags;

extern char	full_stem_flags[][40];
extern	char	StemType[];

extern char full_leaf_flags[][40];
extern char LeafType[];
/* stem subcomponent flags */
extern char leaf_stem_flags[][40];
extern char LeafStemType[];

extern char full_head_flags[][40];
extern char HeadType[];
/* stem subcomponent flags */
extern char head_stem_flags[][40];
extern char HeadStemType[];

#endif
