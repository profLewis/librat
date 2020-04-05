#include "prat.h"

#ifdef AHMADOK

#define THETA_THRESH DTOR(83)
#ifdef LOCAL


extern int data_position[512],number_of_samples,no_of_dimensions;
extern AhmadDeering	AHMADD;


int	local_read_ahmadd_model(ahmadd_model,verbose,filename,fp)
Spectral_brdf_model	*ahmadd_model;
FILE	*fp;
char	*filename;
int	verbose;
{
	char	buffer[2000];
	int	i,rows=0,no_of_columns=0,timer=0,get_no_of_columns_in_file();
	FILE	*fp1,*open_file_for_read();
	double	wavelength,albedo,dum;
	AhmadDeering	*model;

	if(fscanf(fp,"%s",buffer)!=1 || strcmp(buffer,"{")!=0)error1("read_ahmadd_model:\t ahmadd model format error");
/* find SINGLE_SCATTERING_ALBEDO flag -> file of spectral values -> allocation size */
	while(fscanf(fp,"%s",buffer)==1 && strcmp(buffer,"}")!=0){
		if( ahmadd_parameter(buffer,fp) == SINGLE_SCATTERING_ALBEDO){
			if(fscanf(fp,"%s",buffer)==1){
				no_of_columns=get_no_of_columns_in_file(0,buffer,&rows);
				if(no_of_columns!=2)error1("read_ahmadd_model:\t2 columns of data expected");
			}
		}
	}
	ahmadd_model->waveband=(Verstraete_model *)v_allocate(rows,sizeof(AhmadDeering));
/*
**	allocate inversion data storage as max. 15*rows
*/
	ahmadd_model->data=d_allocate(15*rows);

	model=(AhmadDeering *)ahmadd_model->waveband;

	ahmadd_model->wavelength=d_allocate(rows);

	fclose(fp);
	fp=open_file_for_read(filename);
		
	for(i=0;i<2;i++)if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\t very unexpected ahmadd model format error (is file being written to?)");
	while(fscanf(fp,"%s",buffer)==1 && strcmp(buffer,"}")!=0){
		switch(ahmadd_parameter(buffer,fp)){
			case SINGLE_SCATTERING_ALBEDO:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying single_scattering_albedo filename");
			model[timer].spectral_filename[0]=c_allocate(500);
			strcpy(model[timer].spectral_filename[0],buffer);
			fp1=open_file_for_read(buffer);
			ahmadd_model->no_of_wavebands=rows;
			timer=0;
			while(fscanf(fp1,"%lf %lf",&ahmadd_model->wavelength[timer],&albedo)==2){
				model[timer].omega=albedo;
				timer++;
			}
			fclose(fp1);
			break;
			case	AHMADD_B:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying b parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].b=dum;
			}else{
				model[timer].spectral_filename[1]=c_allocate(500);
				strcpy(model[timer].spectral_filename[1],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].b=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_C:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying c parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].c=dum;
			}else{
				model[timer].spectral_filename[2]=c_allocate(500);
				strcpy(model[timer].spectral_filename[2],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].c=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_TAU:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying optical_depth/tau parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_TAU specified (value must be positive)");
				for(i=0;i<rows;i++)model[i].tau=dum;
			}else{
				model[timer].spectral_filename[3]=c_allocate(500);
				strcpy(model[timer].spectral_filename[3],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_TAU specified (value must be positive)");
					model[timer].tau=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_H:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying h parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_H specified (value must be positive)");
				for(i=0;i<rows;i++)model[i].h=dum;
			}else{
				model[timer].spectral_filename[4]=c_allocate(500);
				strcpy(model[timer].spectral_filename[4],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_H specified (value must be positive)");
					model[timer].h=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_REFRACTIVE_INDEX:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying refractive_index parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_REFRACTIVE_INDEX specified (value must be positive)");
				for(i=0;i<rows;i++)model[i].refractive_index=dum;
			}else{
				model[timer].spectral_filename[5]=c_allocate(500);
				strcpy(model[timer].spectral_filename[5],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_REFRACTIVE_INDEX specified (value must be positive)");
					model[timer].refractive_index=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_SIGMA:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying sigma parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_SIGMA specified (value must be positive)");
				for(i=0;i<rows;i++)model[i].sigma2=dum*dum;
			}else{
				model[timer].spectral_filename[6]=c_allocate(500);
				strcpy(model[timer].spectral_filename[6],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_SIGMA specified (value must be positive)");
					model[timer].sigma2=(double)dum*dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_A:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying sigma parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_A specified (value must be positive)");
				for(i=0;i<rows;i++)model[i].A=dum;
			}else{
				model[timer].spectral_filename[7]=c_allocate(500);
				strcpy(model[timer].spectral_filename[7],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tnegative AHMADD_A specified (value must be positive)");
					model[timer].A=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_D0:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying D0 parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].d0=dum;
			}else{
				model[timer].spectral_filename[8]=c_allocate(500);
				strcpy(model[timer].spectral_filename[8],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].d0=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_D1:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying D1 parameter");
			if(is_double(buffer,&dum)){
				for(i=0;i<rows;i++)model[i].d1=dum;
			}else{
				model[timer].spectral_filename[9]=c_allocate(500);
				strcpy(model[timer].spectral_filename[9],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					model[timer].d1=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case	AHMADD_S0:
			if(fscanf(fp,"%s",buffer)!=1)error1("read_ahmadd_model:\tread error specifying S0 parameter");
			if(is_double(buffer,&dum)){
				if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tAHMADD_S0 parameter must be positive");
				for(i=0;i<rows;i++)model[i].S_0=dum;
			}else{
				model[timer].spectral_filename[10]=c_allocate(500);
				strcpy(model[timer].spectral_filename[10],buffer);
				fp1=open_file_for_read(buffer);timer=0;
				timer=0;
#ifdef DOUBLEDEF
				while(fscanf(fp1,"%lf %lf",&wavelength,&dum)==2){
#else
				while(fscanf(fp1,"%f %f",&wavelength,&dum)==2){
#endif
					if(dum<0.0)error1("prat:\terror in defining ahmadd BRDF material:\n\t\tAHMADD_S0 parameter must be positive");
					model[timer].S_0=(double)dum;
					timer++;
				}
				fclose(fp1);
			}
			break;
			case -1:
			break;
			default:
			error1("read_ahmadd_model\tunrecognised entry in ahmadd model");
		}
	}
	for(i=0;i<rows;i++)
		model[i].e=calculate_e(&(model[i]));
	
	return(1);
}
#endif


void	calculate_Chandrasekha_H_functions(H,ahmadd,omega)
double	*H;
AhmadDeering	*ahmadd;
Omega	*omega;
{
	double	base;

	base = (1.0+2.0*omega->mu*sqrt(1.0-MIN(1.0,ahmadd->omega)));
	if(base==0)base=1e-10;
	H[0] = (1.02 + 2.0*omega->mu*(1.0+0.05*ahmadd->b))/base;
	H[1] = 1.01 + 0.13*omega->mu*ahmadd->omega*ahmadd->b*(2.0-omega->mu);
	/*fprintf(stderr,"H: %f %f\n",H[0],H[1]);*/
	return;
}

double	calculate_e(ahmadd)
AhmadDeering	*ahmadd;
{
	double	e;

	e=ahmadd->b*(0.365*ahmadd->omega - 0.233*(ahmadd->omega * ahmadd->omega) + 0.013*ahmadd->b*ahmadd->omega - 0.0129);
	return(e);
}

double	calculate_phase_angle(viewer,illumination)
Omega	*viewer,*illumination;
{
	double	phase;

/*	phase= -viewer->mu*illumination->mu + sqrt(1.0-viewer->mu*viewer->mu)*sqrt(1.0-illumination->mu*illumination->mu)*cos(viewer->phi-illumination->phi);*/
	phase = -viewer->mu*illumination->mu + (sin(viewer->theta)*sin(illumination->theta))*cos(illumination->phi-viewer->phi);

/*	fprintf(stderr,"phase angle: cos(%f): %f\n",RTOD(acos(MIN(1.0,MAX(-1.0,phase)))),phase);*/
	return(acos(MAX(-1.0,MIN(1.0,phase))));
}

void	calculate_Phase(P,alpha,ahmadd)
double	alpha,*P;
AhmadDeering	*ahmadd;
{
	double	cos_alpha;

	cos_alpha=cos(alpha);

	P[0] = 1.0 + ahmadd->b*cos_alpha;
	P[1] = P[0] + 0.5*ahmadd->c*(3.0*cos_alpha*cos_alpha - 1.0);

	if(P[0]==0.0)P[0]=1e-20;
	if(P[1]==0.0)P[1]=1e-20;

/*	fprintf(stderr,"P[0]: %f P[1]: %f\n",P[0],P[1]);*/

	return;
}

double	calculate_specular(viewer,illumination,ahmadd,current_reflectance_bag)
Omega	*viewer,*illumination;
AhmadDeering	*ahmadd;
CurrentReflectanceBag		*current_reflectance_bag;
{
	double	a,b,d,e,c,g,rf,s1,s2,t1,t2,p_tan_theta_n,mu_n,mu_n4;
        double  tan_theta_n,r_s,alpha_0,alpha_dash,alpha_dash2;
        double    Gamma2,Gamma1,Gamma;
	triplet	H,look,V_factor(),vector_plus();
	double	V_dot();
        double  Temp;
look=V_factor(*current_reflectance_bag->look,-1.0);
H = normalise(vector_plus(look,*current_reflectance_bag->illumination));
Temp = V_dot(look,H);
if(Temp==0.0){
        Gamma=0.0;
#ifdef SPECULAR_DEBUG
        fprintf(stderr,"{Zero-Gamma}");
#endif
}else{
Gamma = fabs(V_dot(*current_reflectance_bag->normal,H)*2/V_dot(look,H));

	Gamma1 = Gamma*V_dot(look,H);
	Gamma2 = Gamma*V_dot(*current_reflectance_bag->illumination,H);
	Gamma = MIN(1.0,MIN(Gamma1,Gamma2));
}
	alpha_dash=acos(fabs(V_dot(H,*current_reflectance_bag->normal)));
	mu_n=cos(alpha_dash);
	mu_n4=mu_n*mu_n;
	mu_n4 *= mu_n4;
	tan_theta_n=tan(alpha_dash);

	p_tan_theta_n=(1.0/(4*ahmadd->sigma2*mu_n4))*EXPONENTIAL(-tan_theta_n*tan_theta_n/ahmadd->sigma2);

	c=acos(V_dot(look,H));
	if(c==0.0){
		s1=(ahmadd->refractive_index-1)/(ahmadd->refractive_index+1);
		rf=s1*s1;
	}else{
		g=asin(sin(c)/ahmadd->refractive_index);
		s1=sin(c-g);s2=sin(c+g);
		t1=tan(c-g);t2=tan(c+g);
		rf=0.5*((s1*s1)/(s2*s2) + (t1*t1)/(t2*t2));
	}

	r_s=ahmadd->A*rf*Gamma*p_tan_theta_n/(M_PI*illumination->mu*viewer->mu);
	

/*	r_s=ahmadd->A*M_PI*rf*p_tan_theta_n/(4*mu_n4*illumination->mu*viewer->mu);*/

	return(r_s);
}

double	calculate_B(alpha,mu,ahmadd)
double	alpha,mu;
AhmadDeering	*ahmadd;
{
	double	B;

/*	fprintf(stderr,"%f %f %f %f\n",mu*ahmadd->S_0,EXPONENTIAL(-1.0*tan(alpha/2.0)/ahmadd->h),(ahmadd->omega),(ahmadd->P_180[0]));*/
	if(ahmadd->omega!=0.0)B = mu*ahmadd->S_0*EXPONENTIAL(-1.0*tan(alpha/2.0)/ahmadd->h)/(ahmadd->omega*ahmadd->P_180[1]);else B=0.0;

	/*fprintf(stderr,"B: %f\n",B);*/

	return(B);
}
double	ahmad_deering_model(ahmadd,viewer,illumination,current_reflectance_bag)
AhmadDeering	*ahmadd;
Omega		*viewer,*illumination;
CurrentReflectanceBag		*current_reflectance_bag;
{
	double	reflectance,B_alpha_dash;
/*	double	a,b,c,d,e,f,h;*/
	double	specular,diffuse,single_scattering,multiple_scattering,single_multiple_scattering,total_multiple_scattering;


	ahmadd->alpha=calculate_phase_angle(viewer,illumination);
	ahmadd->alpha_dash=M_PI-ahmadd->alpha;

	calculate_Phase(ahmadd->P_180,M_PI,ahmadd);
	calculate_Phase(ahmadd->P,ahmadd->alpha,ahmadd);

	B_alpha_dash=calculate_B(ahmadd->alpha_dash,illumination->mu,ahmadd);

	calculate_Chandrasekha_H_functions(ahmadd->H,ahmadd,viewer);
	calculate_Chandrasekha_H_functions(ahmadd->H_dash,ahmadd,illumination);

	ahmadd->specular=calculate_specular(viewer,illumination,ahmadd,current_reflectance_bag);

/*	a =0.25*(ahmadd->omega/(viewer->mu+illumination->mu))*(1.0-EXPONENTIAL(-1.0*ahmadd->tau*((1.0/viewer->mu)+(1.0/illumination->mu))));
	b =(ahmadd->P[1]*(1.0+B_alpha_dash));
	c = 0.25*(ahmadd->omega/(viewer->mu+illumination->mu));
	d = ahmadd->H[0]*ahmadd->H_dash[0]*(1.0 - ahmadd->e*(viewer->mu+illumination->mu) - ahmadd->b*(1.0-ahmadd->omega)*viewer->mu*illumination->mu);
	e = ahmadd->b*sqrt(1.0-viewer->mu*viewer->mu)*sqrt(1.0-illumination->mu*illumination->mu)*ahmadd->H[1]*ahmadd->H_dash[1]*(cos(ahmadd->alpha) - viewer->mu*illumination->mu);
	f = -0.25*(ahmadd->omega/(viewer->mu + illumination->mu))*ahmadd->P[0];
	h = ahmadd->specular;

	reflectance = a*b + c*(d+e) +f+h;
*/

	single_scattering = 0.25*(ahmadd->omega/(viewer->mu+illumination->mu)) * (1.0-EXPONENTIAL(-1.0*ahmadd->tau*((1.0/viewer->mu)+(1.0/illumination->mu)))) * (ahmadd->P[1]*(1.0+B_alpha_dash));

	single_multiple_scattering = 0.25*(ahmadd->omega/(viewer->mu+illumination->mu)) * ahmadd->P[0];
	total_multiple_scattering = 0.25*(ahmadd->omega/(viewer->mu+illumination->mu)) * ((ahmadd->H[0]*ahmadd->H_dash[0]*(1.0 - ahmadd->e*(viewer->mu+illumination->mu) - ahmadd->b*(1.0-ahmadd->omega)*viewer->mu*illumination->mu)) + (ahmadd->b*sin(viewer->theta)*sin(illumination->theta)*ahmadd->H[1]*ahmadd->H_dash[1]*cos(viewer->phi-illumination->phi)));

	multiple_scattering = total_multiple_scattering - single_multiple_scattering;

	diffuse = ahmadd->d0 + ahmadd->d1/(viewer->mu + illumination->mu);

	specular = ahmadd->specular;

	reflectance = single_scattering + multiple_scattering + diffuse + specular;


	return(reflectance);

}

extern AhmadDeering AHMADD;
extern triplet	look[512],illumination[512];
extern triplet	normal;

void	reformat_ahmadd_model(look,illumination,normal,ahmadd,viewer,illumin)
AhmadDeering	*ahmadd;
Omega		*viewer,*illumin;
triplet		*look, *illumination, *normal;

{
	double	s1,s2,cospha;
/* 
**	reformat angles to be relative to normal vector
*/

	viewer->mu=fabs(V_dot(*(look),*(normal)));
	illumin->mu=fabs(V_dot(*(illumination),*(normal)));
	viewer->theta=acos(MIN(1.0,MAX(-1.0,viewer->mu)));
	illumin->theta=acos(MIN(1.0,MAX(-1.0,illumin->mu)));
	if(illumin->theta>(THETA_THRESH)){
		illumin->theta=THETA_THRESH;
		illumin->mu=cos(THETA_THRESH);
	}
	s1=sin(viewer->theta);
	s2=sin(illumin->theta);
	cospha=V_dot(*(look),*(illumination));
	illumin->phi=M_PI;
	viewer->phi= acos(MMAX(MMIN(1.0,(cospha-viewer->mu*illumin->mu)/(s1*s2)),-1.0));

	return;
}

double	calculate_ahmadd_function(p,i,current_reflectance_bag)
double	*p;
int	i;
CurrentReflectanceBag		*current_reflectance_bag;
{
	double	reflectance;
	double	specular,diffuse,single_scattering,multiple_scattering,single_multiple_scattering,total_multiple_scattering,B_alpha_dash;
	Omega	viewer,illumin;
	AhmadDeering	*ahmadd;

	ahmadd= &AHMADD;
	load_ahmadd_model_from_data(ahmadd,p);
	calculate_e(ahmadd);
	reformat_ahmadd_model(&look[i],&illumination[i],&normal,ahmadd,&viewer,&illumin);
	ahmadd->alpha=calculate_phase_angle(&viewer,&illumin);
	ahmadd->alpha_dash= M_PI-ahmadd->alpha;
	calculate_Phase(ahmadd->P_180,M_PI,ahmadd);
	calculate_Phase(ahmadd->P,ahmadd->alpha,ahmadd);

	B_alpha_dash=calculate_B(ahmadd->alpha_dash,illumin.mu,ahmadd);

	calculate_Chandrasekha_H_functions(ahmadd->H,ahmadd,&viewer);
	calculate_Chandrasekha_H_functions(ahmadd->H_dash,ahmadd,&illumin);

	if(ahmadd->A != 0.0)ahmadd->specular=calculate_specular(&viewer,&illumin,ahmadd,current_reflectance_bag);else ahmadd->specular=0.0;

	single_scattering = 0.25*(ahmadd->omega/(viewer.mu+illumin.mu)) * (1.0-EXPONENTIAL(-1.0*ahmadd->tau*((1.0/viewer.mu)+(1.0/illumin.mu)))) * (ahmadd->P[1]*(1.0+B_alpha_dash));

	single_multiple_scattering = 0.25*(ahmadd->omega/(viewer.mu+illumin.mu)) * ahmadd->P[0];
	total_multiple_scattering = 0.25*(ahmadd->omega/(viewer.mu+illumin.mu)) * ((ahmadd->H[0]*ahmadd->H_dash[0]*(1.0 - ahmadd->e*(viewer.mu+illumin.mu) - ahmadd->b*(1.0-ahmadd->omega)*viewer.mu*illumin.mu)) + (ahmadd->b*sin(viewer.theta)*sin(illumin.theta)*ahmadd->H[1]*ahmadd->H_dash[1]*cos(viewer.phi-illumin.phi)));

	multiple_scattering = total_multiple_scattering - single_multiple_scattering;

	diffuse = ahmadd->d0 + ahmadd->d1/(viewer.mu + illumin.mu);

	specular = ahmadd->specular;

	reflectance = single_scattering + multiple_scattering + diffuse + specular;


	return((double)reflectance);

}

double	op_ahmadd_model(model,band,angle,current_reflectance_bag)
Spectral_brdf_model	*model;
int	band,angle;
CurrentReflectanceBag		*current_reflectance_bag;
{
	double	out,p[20],lb[20],ub[2];
	AhmadDeering	*ahmadd;

	ahmadd=(AhmadDeering *)model[angle].waveband;
	AHMADD=ahmadd[band];
	load_data_from_ahmadd_model(&AHMADD,p,lb,ub);
	out=calculate_ahmadd_function(p,angle,current_reflectance_bag);
	

	return(out);
}

void	load_ahmadd_model_from_data(ahmadd,data)
AhmadDeering	*ahmadd;
double		*data;
{
	ahmadd->omega=data[0];
	ahmadd->b=data[1];
	ahmadd->c=data[2];
	ahmadd->S_0=data[3];
	ahmadd->h=data[4];
	ahmadd->tau=data[5];
	ahmadd->d0=data[6];
	ahmadd->d1=data[7];

	if(number_of_samples>8){
		ahmadd->A=data[8];
		ahmadd->refractive_index=data[9];
		ahmadd->sigma2=data[10];
	}
	return;
}

void	load_data_from_ahmadd_model(ahmadd,data,lower_bounds,upper_bounds)
AhmadDeering	*ahmadd;
double		*data,*lower_bounds,*upper_bounds;
{
	int	no_of_params=0,counter=0;

	data[no_of_params]=MIN(ahmadd->omega,1.0);
	lower_bounds[no_of_params]=MIN_SINGLE_SCATTERING_ALBEDO;
	upper_bounds[no_of_params]=MAX_SINGLE_SCATTERING_ALBEDO;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->b;
	lower_bounds[no_of_params]=MIN_B;
	upper_bounds[no_of_params]=MAX_B;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->c;
	lower_bounds[no_of_params]=MIN_C;
	upper_bounds[no_of_params]=MAX_C;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->S_0;
	lower_bounds[no_of_params]=MIN_S0;
	upper_bounds[no_of_params]=MAX_S0;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->h;
	lower_bounds[no_of_params]=MIN_H;
	upper_bounds[no_of_params]=MAX_H;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->tau;
	lower_bounds[no_of_params]=MIN_TAU;
	upper_bounds[no_of_params]=MAX_TAU;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->d0;
	lower_bounds[no_of_params]=MIN_D0;
	upper_bounds[no_of_params]=MAX_D0;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	data[no_of_params]=ahmadd->d1;
	lower_bounds[no_of_params]=MIN_D1;
	upper_bounds[no_of_params]=MAX_D1;
	data_position[counter]=no_of_params;no_of_params++;counter++;

	if(ahmadd->A !=0.0){

		data[no_of_params]=ahmadd->A;
		lower_bounds[no_of_params]=MIN_A;
		upper_bounds[no_of_params]=MAX_A;
		data_position[counter]=no_of_params;no_of_params++;counter++;

		data[no_of_params]=ahmadd->refractive_index;
		lower_bounds[no_of_params]=MIN_N;
		upper_bounds[no_of_params]=MAX_N;
		data_position[counter]=no_of_params;no_of_params++;counter++;

		data[no_of_params]=ahmadd->sigma2;
		lower_bounds[no_of_params]=MIN_SIGMA2;
		upper_bounds[no_of_params]=MAX_SIGMA2;
		data_position[counter]=no_of_params;no_of_params++;counter++;
	}
	number_of_samples=no_of_params;
	no_of_dimensions=no_of_params;
	return;
}


#endif

