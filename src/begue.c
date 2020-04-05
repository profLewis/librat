#include "prat.h"
 

/*
**	calculates distance between cylinders (in metres)
**	given as a function of density of canopy (clumps/ha)
**
**	Begue, 1993 (RSE 46:1-25(1993) - 'leaf area index, intercepted PAR
**	and spectral vegetation indices: a sensitivity analysis
**	for regular-clumped canopies
**	
**	p.3	e  = 100 * sqrt(2/(sqrt(3)*d))
*/

double	hexagonal_clump_distance(clump_density)
double	clump_density;
{
	return(100*sqrt(2.0/(clump_density*SQRT3)));
}

double	random_clump_distance(clump_density)
double	clump_density;
{
	return(100*sqrt(1.0/clump_density));
}

int	local_read_begue_model(begue,verbose,filename,fp)
Spectral_brdf_model	*begue;
FILE			*fp;
int			verbose;
char			*filename;
{
        char    	buffer[2000];
	int		i,j,rows=0,no_of_columns=0,timer=0,get_no_of_columns_in_file();
	FILE		*fp1,*open_file_for_read();
	double		wavelength,albedo,dum,reflectance,transmittance;
	double		clump_spacing,clump_density;
	BegueModel	*model;
/*
*/
	if(fscanf(fp,"%s",buffer)!=1 || strcmp(buffer,"{")!=0)error1("read_begue_model model format error");

/* find leaf_optical_properties flag -> file of spectral values -> allocation size */

	while(fscanf(fp,"%s",buffer)==1 && strcmp(buffer,"}")!=0){
		if( begue_parameter(buffer,fp) == LEAF_OPTICAL_PROPERTIES_BEGUE){
			if(fscanf(fp,"%s",buffer)==1){
				no_of_columns=get_no_of_columns_in_file(0,buffer,&rows);
				if(no_of_columns!=3)error1("read_begue_model:\t3 columns of data expected (wavelength/reflectance/transmittance)");
			}
		}
	}
	begue->waveband=(BegueModel *)v_allocate(rows,sizeof(BegueModel));
/*
**	allocate inversion data storage as max. 15*rows
*/
	begue->data=d_allocate(15*rows);
	
	model=(BegueModel *)begue->waveband;

	begue->wavelength=d_allocate(rows);

	fclose(fp);

	fp=open_file_for_read(filename);
		
	for(i=0;i<2;i++)if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\t very unexpected begue model format error (is file being written to?)");
	while(fscanf(fp,"%s",buffer)==1 && strcmp(buffer,"}")!=0){
		switch(begue_parameter(buffer,fp)){
			case LEAF_OPTICAL_PROPERTIES_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying leaf_optical_properties filename");
			model[timer].spectral_filename[0]=c_allocate(500);
			strcpy(model[timer].spectral_filename[0],buffer);
			fp1=open_file_for_read(buffer);
			begue->no_of_wavebands=rows;
			timer=0;
			while(fscanf(fp1,"%lf %lf %lf",&begue->wavelength[timer],&reflectance,&transmittance)==3){
				model[timer].reflectance=reflectance;
				model[timer].transmittance=transmittance;
				timer++;
			}
			fclose(fp1);
			break;
			case CLUMP_DENSITY_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying clump_density parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].clump_density=dum;
			}else{
				model[timer].spectral_filename[1]=c_allocate(500);
				strcpy(model[timer].spectral_filename[1],buffer);
				fp1=open_file_for_read(buffer);timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].clump_density=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case PLANT_RADIUS_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying plant_radius parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].radius=dum;
			}else{
				model[timer].spectral_filename[2]=c_allocate(500);
				strcpy(model[timer].spectral_filename[2],buffer);
				fp1=open_file_for_read(buffer);timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].radius=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case PLANT_HEIGHT_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying plant_height parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].height=dum;
			}else{
				model[timer].spectral_filename[3]=c_allocate(500);
				strcpy(model[timer].spectral_filename[3],buffer);
				fp1=open_file_for_read(buffer);timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].height=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case LAI_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying plant_lai parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].lai=dum;
			}else{
				model[timer].spectral_filename[3]=c_allocate(500);
				strcpy(model[timer].spectral_filename[3],buffer);
				fp1=open_file_for_read(buffer);timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].lai=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case CANOPY_STRUCTURE_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying canopy_structure parameter");
			if(strcmp(buffer,"HEXAGONAL")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=HEXAGONAL_BEGUE;		
			}else if(strcmp(buffer,"RANDOM")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=RANDOM_BEGUE;	
			}else error1("read_begue_model:\tread error specifying canopy_structure parameter");
			break;
			case LEAF_INCLINATION_MODEL_BEGUE:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_begue_model:\tread error specifying canopy_structure parameter");
			if(strcmp(buffer,"PLANOPHILE")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=PLANOPHILE_BEGUE;		
			}else if(strcmp(buffer,"ERECTOPHILE")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=ERECTOPHILE_BEGUE;	
			}else if(strcmp(buffer,"PLAGIOPHILE")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=PLAGIOPHILE_BEGUE;	
			}else if(strcmp(buffer,"EXTREMOPHILE")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=EXTREMOPHILE_BEGUE;	
			}else if(strcmp(buffer,"UNIFORM")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=UNIFORM_BEGUE;	
			}else if(strcmp(buffer,"SPHERICAL")!=0){
				for(i=0;i<rows;i++)model[i].canopy_structure=SPHERICAL_BEGUE;	
			}else error1("read_begue_model:\tread error specifying canopy_structure parameter");
			break;
		}
	}
			
/*
**	calculate clump spacing (m)
*/
	for(j=0;j<timer;j++)
	for(i=0;i<rows;i++){
		switch(model[i].canopy_structure){
			case	HEXAGONAL_BEGUE:
			model[j].clump_spacing=hexagonal_clump_distance(model[j].clump_density);
			break;
			case	RANDOM_BEGUE:
			model[j].clump_spacing=random_clump_distance(model[j].clump_density);
			break;
		}		

/*
**	normalize height / radius
**
**	normalized begue->radius must be less than 0.45
*/
		model[j].radius /= clump_spacing;
		if(model[j].radius>0.45)error1("read_begue_model: normalized plant radius must be < 0.45\n\t\tchange clump_spacing or plant_radius");
		model[j].height /= clump_spacing;

		model[j].radius_to_height_ratio=model[j].radius/model[j].height;
	}
}

/*
**	calculates porosity of cylinders according to:
**	Begue, 1993 (RSE 46:1-25(1993) - 'leaf area index, intercepted PAR
**	and spectral vegetation indices: a sensitivity analysis
**	for regular-clumped canopies
**
**	equation (1), p.3:
**
**	p(theta_s) = exp(-Kc (1-transmittance) sqrt(3) * lai
**					       -------------
**					         2pi * R^2    )
**
** 	note that radius is normalized by the clump spacing
**
*/
double	porosity(extinction_coefficient,transmittance,lai,cylinder_radius)
double	extinction_coefficient,transmittance,lai,cylinder_radius;
{

	return(exp(-extinction_coefficient*(1-transmittance)*SQRT3*lai / (TWOPI * cylinder_radius*cylinder_radius)));

}

/*
**
**	calculate interception efficiency of canopy
**
**	zenith in radians
**
**
*/

double	direct_interception_efficiency(begue,zenith)
BegueModel	*begue;
double		*zenith;
{
	double	epsidir;
/*
**	calculation of the interception of diffuse and direct radiation
*/
	switch(begue->canopy_structure){
		case HEXAGONAL_BEGUE:
		hexadir_(zenith,&(begue->height),&(begue->radius),&(begue->porosity),&epsidir);
		break;
		case RANDOM_BEGUE:
		poisdir_(zenith,&(begue->height),&(begue->radius),&(begue->porosity),&epsidir);
		break;
	}

	return(epsidir);
}


/*
**	calculate directional single-scattering reflectance
*/

double	begue_single_scattering_reflectance(begue,solar_zenith,viewing_zenith)
BegueModel	begue;
static double	solar_zenith,viewing_zenith;
{
	double	rho[2],solar_efficiency,viewing_efficiency;
	double	solar_extinction,viewing_extinction;

	solar_efficiency  =direct_interception_efficiency(begue,&solar_zenith);
	viewing_efficiency=direct_interception_efficiency(begue,&viewing_zenith);
	
	rho[0]=(begue->reflectance * 
}




