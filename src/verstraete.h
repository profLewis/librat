#ifndef _VERSTRAETE_
#define _VERSTRAETE_

/*  Argument(s): **

**  Definitions: **

**     ANGLES       Illumination and observation angles: **
**        ANGLES (1): Illumination zenith angle, in radians **
**        ANGLES (2): Observation zenith angle, in radians **
**        ANGLES (3): Relative azimuth angle, in radians **
**     ERROR        Error code: **
**        0:   No error found, normal return. **
**        If more than one error condition occurs, only the most recent **

**        (usually the most severe) error code is returned. **
**        Error codes between 100 and 199 indicate warnings: computations 
**
**        are proceeding, but the result may not be what you expected. **
**        101: OPTION (1) not an acceptable value, reset to 1 **
**        102: OPTION (2) not an acceptable value, reset to 1 **
**        103: OPTION (3) not an acceptable value, reset to 1 **
**        104: OPTION (4) not an acceptable value, reset to 1 **
**        105: OPTION (5) not an acceptable value, reset to 1 **
**        Error codes above 200 indicate severe conditions that should **
**        not be encountered. The reflectance value MVBP1 is set to -1 **
**        and the execution is aborted. Check your code and usage of this 
**
**        function. **
**        201: Illumination zenith angle (ANGLES (1)) not an acceptable **

**             value, execution aborted **
**        202: Observation zenith angle (ANGLES (2)) not an acceptable **
**             value, execution aborted **
**        203: Relative azimuth angle (ANGLES (3)) not an acceptable **
**             value, execution aborted **
**        301: Single scattering albedo (OPTICS (1)) not an acceptable **
**             value, execution aborted **
**        302: Phase function parameter (OPTICS (2) not an acceptable **
**             value, execution aborted **
**        401: Leaf area density (kappa (1)) not an acceptable value, **
**             execution aborted **
**        402: Radius of Sun flecks (kappa (2)) not an acceptable value, 
**
**             execution aborted **
**        411: Given value of KAPPA1 (kappa (3) with OPTION (3) = 0) **
**             not an acceptable value, execution aborted **
**        412: Given value of KAPPA2 (kappa (4) with OPTION (3) = 0) **
**             not an acceptable value, execution aborted **
**        421: Leaf orientation parameter CHIL (kappa (3) with **
**             OPTION (3) = 1) not an acceptable value, execution aborted 
**
**     OPTICS       Optical characteristics of the scatterers: **
**        OPTICS (1): Single scattering albedo, N/D value between 0.0 **
**                    and 1.0 **
**        OPTICS (2): Phase function parameter: **
**           if OPTION (4) = 0: This input is not used **
**           if OPTION (4) = 1: Asymmetry factor, N/D value between -1.0 
**
**              and 1.0 **
**           if OPTION (4) = 2: First coefficient of Legendre polynomial 
**
**        OPTICS (3): Phase function parameter: **
**           if OPTION (4) = 0: This input is not used **
**           if OPTION (4) = 1: This input is not used **
**           if OPTION (4) = 2: Second coefficient of Legendre polynomial 
**
**     OPTION       Integer array to select the desired output: **
**        model_type: 0 for full theoretical model (See Ref. 1) **
**                    1 for parameterized model (See Ref. 2) **
**        kappa_flag: 0 for given values of Kappa (See kappa below) **
**                    1 for Goudriaan's parameterization of Kappa **
**                    2 for Dickinson et al's correction to Goudriaan's **

**                      parameterization of Kappa (See Ref. 3) **
**        OPTION (4): 0 for isotropic phase function **
**                    1 for Heyney and Greensteins' phase function **
**                    2 for Legendre polynomial phase function **
**        OPTION (5): 0 for single scattering only **
**                    1 for Dickinson et al. parameterization of multiple 
**
**                      scattering **
**     kappa       Structural parameters of the medium: **
**        kappa (1): "Leaf area density", in m2 m-3 **
**        kappa (2): Radius of the sun flecks on the scatterer, in m **
**        kappa (3): Leaf orientation parameter: **
**           if OPTION (3) = 0: kappa (3) = KAPPA1 **
**           if OPTION (3) = 1 or 2: kappa (3) = CHIL **
**        kappa (4): Leaf orientation parameter: **
**           if OPTION (3) = 0: kappa (4) = KAPPA2 **
**           if OPTION (3) = 1 or 2: This input is not used */


typedef struct{
	DOUBLE	mu1;
	DOUBLE	mu2;
	DOUBLE	cosphi;
	DOUBLE	cospha;
	DOUBLE	phaang;
	DOUBLE	tante1;
	DOUBLE	tante2;
	DOUBLE	geofac;
}Calculated_Verstraete_Data;

typedef struct{
	float	theta[3];
	float	optics[3];
	int	model_type;
	int	kappa_flag;
	int	phase_model;
	int	scattering_model;
	float	kappa[4];
	int	option[5];

	Calculated_Verstraete_Data	data;
}Verstraete_model;

typedef struct{
	 Verstraete_model	*waveband;
	int		no_of_wavebands;
	double		*wavelength;
        double           *data;
        double           theta[4];
}Spectral_brdf_model;

/* for backwards compatibility with ray tracer */
typedef Spectral_brdf_model Spectral_Verstraete_model;

#define	SINGLE_SCATTERING_ALBEDO	0
#define	ASYMMETRY_FACTOR		1
#define	LEGENDRE_COEFFS			2
#define	MODEL_TYPE			3
#define	KAPPA_MODEL			4
#define	PHASE_MODEL			5
#define	SCATTERING_MODEL		6
#define	LEAF_AREA_DENSITY		7
#define	SUN_FLECK_RADIUS		8
#define	KAPPA				9
#define	CHIL				10

#define FULL_THEORETICAL_MODEL          0
#define PARAMETERISED_MODEL             1
/* kappa_flag */
#define GIVEN_KAPPA                     0
#define GOUDRIAAN_KAPPA                 1
#define CORRECTED_GOUDRIAAN_KAPPA       2
/* phase_model */
#define ISOTROPIC                       0
#define HEYNEY_GREENSTEIN               1
#define LEGENDRE_POLYNOMIAL             2

#define MAX_SINGLE_SCATTERING_ALBEDO 1.0
#define MIN_SINGLE_SCATTERING_ALBEDO 0.0
#define MAX_ASYMMETRY_FACTOR 1.0
#define MIN_ASYMMETRY_FACTOR -1.0
#define MAX_SUN_FLECK_RADIUS 1.0
#define MIN_SUN_FLECK_RADIUS 1.0
#define MAX_CHIL 0.6
#define MIN_CHIL -0.4
#define MAX_LEAF_AREA_DENSITY 5.0
#define MIN_LEAF_AREA_DENSITY 0.0
#define MAX_LEGENDRE_COEFFS 10.0
#define MIN_LEGENDRE_COEFFS 0.0
#define MAX_KAPPA 10.0
#define MIN_KAPPA 0.0

float	calculate_pvd_function();
void	load_pvd_model_from_data(),load_data_from_pvd_model();
#endif
