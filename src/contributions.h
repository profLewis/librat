/*#define DIFFUSE_CONTRIBUTIONS(wavelength,sample_no,from_level,to_level) *(materialbag->samples->diffuse_depth_result + wavelength*materialbag->samples->max_no_of_ray_samples*(flagbag->max_ray_depth+1) + to_level*materialbag->samples->max_no_of_ray_samples + sample_no + from_level*wavebandbag->sensor_wavebands->no_of_wavebands*(flagbag->max_ray_depth+1)*materialbag->samples->max_no_of_ray_samples)
#define DIRECT_CONTRIBUTIONS(wavelength,sample_no,from_level,to_level) *(materialbag->samples->direct_depth_result + wavelength*materialbag->samples->max_no_of_ray_samples*(flagbag->max_ray_depth+1) + to_level*materialbag->samples->max_no_of_ray_samples + sample_no + from_level*wavebandbag->sensor_wavebands->no_of_wavebands*(flagbag->max_ray_depth+1)*materialbag->samples->max_no_of_ray_samples)*/

/*#define CONTRIBUTIONS_INDEX(wavelength,sample_no,from_level,to_level) (wavelength*materialbag->samples->max_no_of_ray_samples*(flagbag->max_ray_depth+1) + to_level*materialbag->samples->max_no_of_ray_samples + sample_no + from_level*wavebandbag->sensor_wavebands->no_of_wavebands*(flagbag->max_ray_depth+1)*materialbag->samples->max_no_of_ray_samples)*/

#define DIFFUSE_RESULT materialbag->samples->diffuse_depth_result
#define DIRECT_RESULT materialbag->samples->direct_depth_result

#ifndef _ARARAT_
	extern int ArrayL[7];
	extern double	access_array();
	extern	void	set_array();
#endif
