#ifndef REFLECTANCE_FUNCTIONS_H
#define REFLECTANCE_FUNCTIONS_H

typedef struct{
	double		*result;
	double		*direct_depth_result;
	double		*diffuse_depth_result;
	int		max_no_of_ray_samples;
	int			no_of_time_bins; /* for time-resolved signals */
	double		startBinCount;
	double		endBinCount;
	double		binStep;
	int		direct_start;
	int		diffuse_reflectance_start;
	int		diffuse_transmittance_start;
	int		*wavelength;
	int		no_of_diffuse_reflectance_samples;
	int		no_of_diffuse_transmittance_samples;
	int		max_diffuse_sampling_depth;
	int		no_of_shadow_rays;
	int		sample_no;
	int nBands;
}Samples;

#endif
