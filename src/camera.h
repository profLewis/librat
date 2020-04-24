#ifndef _CAMERA_
#define _CAMERA_

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

#define	ALBEDO		0
#define	SPHERICAL	1
#define PLANAR		2
#define PLANAR2         3

/*
**	camera ip format: - options
**
**	CAMERA_TYPE spherical/plane
**	CAMERA_POSITION cx cy cz
**	FOCUSSED_AT fx fy fz	(watch point)
**	ROLL_ANGLE psi (degrees)
**	FRAME_NUMBER n
**	FOCAL_LENGTH f
**	F_STOP f_stop
**	ZOOM zoom
*/


char           *read_data_from_line(char *buff, char *flag, int n, double *data, int *idata);
void read_flagged_camera_ip(int *local_height, Camera_ip *camera_ip, char *buffer, Restart *restartPtr);
int precalculate_camera_characteristics(int verbose, Camera_ip *camera_ip, Camera_op *camera_op, Image_characteristics *image_characteristics, FlagBag *flagbag);
void calculate_sun_elevation_axis(triplet *sun_elevation_axis_Ptr, triplet *sun_Ptr);
void calculate_pixel_variance(int n, double *pixel_variance, PixelCondition *pixel_condition);
int calculate_reflectance_data(WavebandBag *wavebandbag, MaterialBag *materialbag, IlluminationBag *illumination, double lambda_min, double lambda_width, int verbose);
void get_ray(RATobj *bb,void *materialbag, Ray *ray, double i, double j, int col, int row, Camera_op *camera_op_Ptr, Image_characteristics *image_characteristics_Ptr, FlagBag *flagbag, BBox *bbox);
void allocate_material_list(MaterialBag *materialbag, WavebandBag *wavebandbag);
void clearIntArray(int *array,int size);
void clearDoubleArray(double *array,int size);
void clearInt_Array(int **array,int size1,int size2);
void clearDouble_Array(double **array,int size1,int size2);
void clearInt__Array(int ***array,int size1,int size2,int size3);
void clearDouble__Array(double ***array,int size1,int size2,int size3);
void skyRadiance(RATobj *bb,double norm,double *diffuseRadiance,WavebandBag *wavebandbag,IlluminationBag *illumination,triplet *tovector,MaterialBag *materialbag);
void sunRadiance(double norm,double *directRadiance,WavebandBag *wavebandbag,IlluminationBag *illumination,triplet *tovector,MaterialBag *materialbag);
void *getReflectance(int isDiffuse,double *out,int type,Material_table *material_Ptr,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag);
void *attenuateRadiance(triplet viewer,triplet illumin,int isDiffuse,Material_table *material,int type,double *radiance,double lambertian,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag);
void *attenuateRadianceRTD(int isDiffuse,int rtd,Material_table *material,int type,double **radiance,double lambertian,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag);
void gatherPixelStatsForRay(RATobj *bb,double norm,void *nowt,int rtd,HitPoint *hitPoint,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,ObjectList *objectlist,BBox *bbox_Ptr,FlagBag *flagbag,Material_table *material);
void clearvector(triplet *v);
void clearAHitPoint(HitPoint *hitPoint);
void clearHitPoint(int nRTD,HitPoint *hitPoint);
double gaussianFunction(double x);
void scan_image(int argc,char **argv,int timer, RATobj *bb, BBox *bbox_Ptr, Camera_op *camera_op_Ptr, Image_characteristics *image_characteristics_Ptr, FILE **datafile);


#endif
