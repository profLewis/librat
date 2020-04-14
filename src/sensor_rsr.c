#include "prat.h"

void dummy_read_spectral_file(Sensor_Wavebands        *sensor_wavebands){
  sensor_wavebands->no_of_wavebands=1;
  if(sensor_wavebands->sensor_rsr)free(sensor_wavebands->sensor_rsr);
  sensor_wavebands->sensor_rsr = (Spectral_Data *)v_allocate(1,sizeof(Spectral_Data));
  sensor_wavebands->sensor_rsr[0].no_of_samples=1;
  sensor_wavebands->sensor_rsr[0].rsr[0][0]= -1.0;
  sensor_wavebands->sensor_rsr[0].rsr[1][0]=0.0;
  return;
}

#define CLOSE -1
FILE *openFile();

void read_spectral_file(verbose,filename,rsr_flag,fixed_wavelength_flag,sensor_wavebands,env)
Sensor_Wavebands	*sensor_wavebands;
int	verbose,*fixed_wavelength_flag;
char	**filename;
int	rsr_flag;
char *env;
{
	int	no_of_wavebands,sample_counter,counter=0,I=0,i=0,j,quit_flag=0;
	FILE	*fp;
	char	buffer[2016];
	double	f1,f2;
	void	exit();

	if(!rsr_flag){
		dummy_read_spectral_file(sensor_wavebands);
		sensor_wavebands->no_of_wavebands=1;
		if(sensor_wavebands->sensor_rsr)free(sensor_wavebands->sensor_rsr);
		sensor_wavebands->sensor_rsr = (Spectral_Data *)v_allocate(1,sizeof(Spectral_Data));
		sensor_wavebands->sensor_rsr[0].no_of_samples=1;
		sensor_wavebands->sensor_rsr[0].rsr[0][0]= 0.0;
		sensor_wavebands->sensor_rsr[0].rsr[1][0]=0.0;
		return;
	}	
/*
**	open file
*/
	fp=openFile(filename[0],TRUE,env);
	i=0;
	while(fgets(buffer,2000,fp)){
		if(sscanf(buffer,"%d %lf",&j,&f1)==2){
			i++;
		}
	}
	rewind(fp);
	*fixed_wavelength_flag=1;
	if(sensor_wavebands->sensor_rsr)free(sensor_wavebands->sensor_rsr);
        sensor_wavebands->sensor_rsr = (Spectral_Data *)v_allocate(i,sizeof(Spectral_Data));
	no_of_wavebands=sensor_wavebands->no_of_wavebands=i;
	i = 0;	
	while(fgets(buffer,2000,fp)){
		if(sscanf(buffer,"%d %lf",&j,&f1)==2){
			sensor_wavebands->sensor_rsr[i].rsr[0][0]=f1;
		        sensor_wavebands->sensor_rsr[i].rsr[1][0]=0.0;
			sensor_wavebands->sensor_rsr[i].no_of_samples=1;
			i++;
			sensor_wavebands->sensor_rsr[i].no_of_samples=0;
		}else{
			fprintf(stderr,"read_spectral_file:\tread error at line %d in spectral file %s\n\t\t(%%d %%f) expected\n",i,filename[0]);
			exit(1);
		}
	}
	if(verbose)fprintf(stderr,"read_spectral_file:\t%d data entries read in file %s\n",no_of_wavebands,filename[0]);
#ifdef PFAT
	/* set up 3 priamry wavelengths for use in pfat visualisation tool */
	for(i=0;i<3;i++){
	  pfatTexture.lambda[i]=sensor_wavebands->sensor_rsr[MAX(i,no_of_wavebands-1)].rsr[0][0];
	}
#endif	       
        fp=openFile(filename[0],CLOSE,fp);
	return;
}


int	quantise_sky_waveband(hd,lambda,no_of_wavelengths_sampled,lambda_0,lambda_range)
double	lambda,lambda_0,lambda_range;
int	no_of_wavelengths_sampled;
struct header *hd;
{
	int	out;
	double	d_lambda,lambda_dash_0;
 
        if(hd->orig_name){	
	 lambda_dash_0=lambda_0-(lambda_range/2.0);
	 d_lambda = (lambda-lambda_dash_0)/( hd->num_frame * lambda_range);
	 d_lambda=MMAX(d_lambda,0.0);
	 d_lambda=MMIN(d_lambda,1.0);
	 out=d_lambda*hd->num_frame;
        }else{
         out=0;
        }
	return(out);
}
