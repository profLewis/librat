#ifndef BEGUE_H
#define BEGUE_H


typedef struct{
	int	leaf_inclination_model;
	int	canopy_structure;
	double	clump_density;
	double	clump_spacing;
	double	radius;
	double	height;
	double	radius_to_height_ratio;
	double	lai;
	double	reflectance;
	double	transmittance;

	char	*spectral_filename[5];

	double	porosity;

}BegueModel;

#define SQRT3 1.7320508076
#define TWOPI 6.2831853070
#define HEXAGONAL_BEGUE	1
#define RANDOM_BEGUE 2
#define PLANOPHILE_BEGUE 1
#define ERECTOPHILE_BEGUE 2
#define PLAGIOPHILE_BEGUE 3
#define EXTREMOPHILE_BEGUE 4
#define UNIFORM_BEGUE 5
#define SPHERICAL_BEGUE 6

#define LEAF_OPTICAL_PROPERTIES_BEGUE 1
#define CLUMP_DENSITY_BEGUE 2
#define PLANT_RADIUS_BEGUE 3
#define PLANT_HEIGHT_BEGUE 4
#define LAI_BEGUE 5
#define LEAF_INCLINATION_MODEL_BEGUE 6
#define CANOPY_STRUCTURE_BEGUE 7

double	calculate_begue_function();

#endif
