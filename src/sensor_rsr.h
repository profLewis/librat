#ifndef _SENSOR_RSR_
#define _SENSOR_RSR_
/*#define MAX_NO_OF_RSR_SAMPLES 1024*/
#define MAX_NO_OF_RSR_SAMPLES 2
#define MAX_NO_RSR_SAMPS 2
typedef struct{
	double	rsr[2][MAX_NO_RSR_SAMPS];		/* rsr[info_channel][sample] */
	int	no_of_samples;
}Spectral_Data;

typedef struct{
	/*Spectral_Data	sensor_rsr[MAX_NO_OF_RSR_SAMPLES];*/
	Spectral_Data *sensor_rsr;
	int		no_of_wavebands;
}Sensor_Wavebands;


void dummy_read_spectral_file(Sensor_Wavebands        *sensor_wavebands);
void read_spectral_file(int verbose,char **filename,int rsr_flag,int *fixed_wavelength_flag,Sensor_Wavebands *sensor_wavebands,char *env);
int     quantise_sky_waveband(struct header *hd,double lambda,int no_of_wavelengths_sampled,double lambda_0,double lambda_range);


#endif

