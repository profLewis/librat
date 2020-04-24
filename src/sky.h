#ifndef SKY_H
#define SKY_H

int get_skymap_wavelengths(int lambda,struct header *hd,double *lambda_min,double *lambda_width);
int hit_sky(int sky_black,triplet *sun_Ptr,double *output_Ptr,triplet *ray_direction_Ptr);
void pre_calculate_sun_parameters(triplet *sun_Ptr,double *theta_sun,double *phi_gap,double *theta_gap,Image_characteristics *sky_data_Ptr);
double intersect_sky(int frame,triplet *sun_Ptr,Image_characteristics *sky_data_Ptr,triplet *ray_direction_Ptr,double *phi_gap,double *theta_sun,double *theta_gap);

#endif

