#ifndef _AHMADD_
#define _AHMADD_
/*
**	parameters for ahmadd model
*/

typedef struct{
	double	omega;
	double	b;
	double	c;
	double	tau;
	double	S_0;
	double	h;
	double	d0;
	double	d1;
	double	e;
	double	H[2];
	double	H_dash[2];
	double	alpha;
	double	alpha_dash;
	double	P_180[2];
	double	P[2];
	double	specular;
	double	A;
	double	sigma2;
	double	refractive_index;
        char    *spectral_filename[15];
}AhmadDeering;

typedef struct{
	double	theta;
	double	phi;
	double	mu;
}Omega;

typedef struct{
	AhmadDeering	*waveband;
	int		no_of_wavebands;
	double		*wavelength;
}Spectral_AhmadDeering_model;

#define AHMADD_B 1
#define AHMADD_C 2
#define AHMADD_TAU 3
#define AHMADD_S0 4
#define AHMADD_H 5
#define AHMADD_REFRACTIVE_INDEX 6
#define AHMADD_SIGMA 7
#define AHMADD_A 8
#define AHMADD_D0 9
#define AHMADD_D1 10

#define MIN_SINGLE_SCATTERING_ALBEDO 0.0
#define MAX_SINGLE_SCATTERING_ALBEDO 1.0
#define MIN_B -1.0
#define MAX_B 1.0
#define MIN_C -1.0
#define MAX_C 1.0
#define MIN_S0 0.0
#define MAX_S0 10.0
#define MIN_H 0.0
#define MAX_H 10.0
#define MIN_TAU 0.0
#define MAX_TAU 10.0
#define MIN_D0 -1.0
#define MAX_D0 1.0
#define MIN_D1 -1.0
#define MAX_D1 1.0
#define MIN_A 0.0
#define MAX_A 1.0
#define MIN_N 1.0
#define MAX_N 2.0
#define MIN_SIGMA2 1e-10
#define MAX_SIGMA2 1e10



double	ahmad_deering_model(),calculate_e();
double	calculate_ahmadd_function();
void	load_ahmadd_model_from_data(),load_data_from_ahmadd_model();
int data_position[512],number_of_samples,no_of_dimensions;
AhmadDeering	AHMADD;
triplet	look[512],illumination[512];
triplet	normal;

#endif
