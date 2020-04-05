#include "prat.h" 

/* 
** Plant RAy Tracer (PRAT): 
*/ 

/* coded by Lewis, dept P&S UCL, Gower St. London. Thur Jun 13 1991 */ 

/* 
** 
** sky 
** 
*/ 

int get_skymap_wavelengths(lambda,hd,lambda_min,lambda_width) 
struct header *hd; 
double *lambda_min,*lambda_width; 
int lambda; 
{ 
char *str,string[100],*ptr,found=0; 
int quit=0; 
double dum; 

str=hd->seq_history; 

if(!lambda && ((sscanf(str,"%s",string)!=1) || (strcmp(string,"sky")!=0))) 
error1("get_skymap_wavelengths:\tskymap format error.\n\t\t\t\tstring sky expected as first argument in header.seq_history\n\t\t\t\tor -lambda lmin lmax lgap option must be specified"); 

if(!lambda) 
while(!quit){ 
	ptr=strchr(str,'-'); 
	if( sscanf(ptr,"%s",string) == 1 ){ 
		if( strcmp(string,"-lambda") == 0 ){ 
			quit=1;ptr+=7; 
#ifdef DOUBLEDEF 
			if( sscanf(ptr,"%lf %lf %lf",lambda_min,&dum,lambda_width) == 3)found=1; 
#else 
			if( sscanf(ptr,"%f %f %f",lambda_min,&dum,lambda_width) == 3)found=1; 
#endif 
		} 
		str++;
	}else quit=1; 
} 
if(!found&&!lambda)error1("get_skymap_wavelengths:\tskymap format error.\n\t\t\t\tstring sky expected as first argument in header.seq_history\n\t\t\t\tor -lambda lambda_min lambda_max lambda_gap option must be specified"); 
return(1); 
} 

int hit_sky(sky_black,sun_Ptr,output_Ptr,ray_direction_Ptr) 
triplet *ray_direction_Ptr,*sun_Ptr; 
double *output_Ptr; 
int sky_black; 
{ 
if(sky_black){ 
*output_Ptr=0.0; 
return(0); 
} 
*output_Ptr=cos(acos(MAX(-1.0,MIN(1.0,V_dot(sun_Ptr,*ray_direction_Ptr))/2.0))); 
return(1); 
} 

void pre_calculate_sun_parameters(sun_Ptr,theta_sun,phi_gap,theta_gap,sky_data_Ptr) 
Image_characteristics *sky_data_Ptr; 
triplet *sun_Ptr; 
double *theta_sun,*phi_gap,*theta_gap; 
{ 
double d; 
if(!sky_data_Ptr){
  *theta_gap=*phi_gap=0.;
}else{
  *theta_gap=(sky_data_Ptr->hd.cols-1)/(2.0*M_PI); 
  *phi_gap=2.0*(sky_data_Ptr->hd.rows-1)/(M_PI); 
}
if(sun_Ptr->z!=1.0){ 
d=sqrt(fabs(1.0-sun_Ptr->z*sun_Ptr->z)); 
if(d==0){
  *theta_sun=0.;
}else{
  *theta_sun=acos(MAX(-1.0,MIN(1.0,sun_Ptr->x/d))); 
}
}else 
*theta_sun=0.0; 

return; 
} 

/* 
** sky image 
*/ 

double intersect_sky(frame,sun_Ptr,sky_data_Ptr,ray_direction_Ptr,phi_gap,theta_sun,theta_gap) 
Image_characteristics *sky_data_Ptr; 
triplet *ray_direction_Ptr, *sun_Ptr; 
double *phi_gap,*theta_sun,*theta_gap; 
int frame; 
{ 
double dr,dc,a,b,out=0.0,outt[4],theta,phi,d_ray; /* phi -> zenith */ 
int size[4],r0,r1,c0,c1,i; 
unsigned char A; 

if(ray_direction_Ptr->z <0.0)return(0.0); 

phi=acos(MAX(-1.0,MIN(1.0,ray_direction_Ptr->z))); 
d_ray=sqrt(fabs(1.0-(ray_direction_Ptr->z * ray_direction_Ptr->z))); 
a=ray_direction_Ptr->x/d_ray; 
a=FSIGN(a)*MIN(fabs(a),1.0); 
theta=acos(MAX(-1.0,MIN(1.0,a))); 
if(ray_direction_Ptr->y<0.0)theta*= -1; 

r0 = (int)(phi * *phi_gap); 
dr = (phi * *phi_gap)-r0; 
r0 = (r0+sky_data_Ptr->hd.rows) % sky_data_Ptr->hd.rows; 
r1 = (r0+1+sky_data_Ptr->hd.rows) % sky_data_Ptr->hd.rows; 
c0 = (int)(((theta- *theta_sun)* *theta_gap)+ ((sky_data_Ptr->hd.cols)/2.0)); 
dc = (((theta- *theta_sun)* *theta_gap)+ ((sky_data_Ptr->hd.cols)/2.0))-c0; 
c0 = (c0+sky_data_Ptr->hd.cols) % sky_data_Ptr->hd.cols; 
c1 = (c0+1+sky_data_Ptr->hd.cols) % sky_data_Ptr->hd.cols; 
size[0]=(frame*sky_data_Ptr->hd.cols*sky_data_Ptr->hd.rows)+(sky_data_Ptr->hd.cols*r0)+c0; 
size[1]=(frame*sky_data_Ptr->hd.cols*sky_data_Ptr->hd.rows)+(sky_data_Ptr->hd.cols*r0)+c1; 
size[2]=(frame*sky_data_Ptr->hd.cols*sky_data_Ptr->hd.rows)+(sky_data_Ptr->hd.cols*r1)+c0; 
size[3]=(frame*sky_data_Ptr->hd.cols*sky_data_Ptr->hd.rows)+(sky_data_Ptr->hd.cols*r1)+c1; 

for(i=0;i<4;i++)switch(sky_data_Ptr->hd.pixel_format){ 
case PFBYTE: 
A= *(sky_data_Ptr->data.bdata+size[i]); 
outt[i]=(double)(A);break; 
case PFFLOAT: 
outt[i]= *(sky_data_Ptr->data.fdata+(size[i]));break; 
} 

a = (dc*(outt[1]-outt[0])+outt[0]); 
b = (dc*(outt[3]-outt[2])+outt[2]); 
out = (dr*(b-a) + a); 


/* 
** nn 
** 
r = (int)(0.5+(phi * *phi_gap)); 
c = (int)(0.5+((theta- *theta_sun)* *theta_gap)+ ((sky_data_Ptr->hd.cols+0.5)/2.0)); 
if(c<0)c+=sky_data_Ptr->hd.cols; 
if(c>=sky_data_Ptr->hd.cols)c-=sky_data_Ptr->hd.cols; 
if(r<0)r+=sky_data_Ptr->hd.rows; 
if(r>=sky_data_Ptr->hd.rows)r-=sky_data_Ptr->hd.rows; 
size=(frame*sky_data_Ptr->hd.cols*sky_data_Ptr->hd.rows)+(sky_data_Ptr->hd.cols*r)+c; 

*/ 
return(out); 

} 
