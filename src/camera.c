#include "frat.h"
/*
 * *    Plant RAy Tracer (PRAT):
 */

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

extern void     error1(), error2(), exit(), reset_local_z_to_local_max_height();
extern void    *calloc();
#ifdef __SUNPRO_C
char           *strchr();
#endif
void allocate_material_list();
/*
 * *    flagged camera ip format: - options *
 * 
 *    CAMERA_TYPE spherical/plane/albedo *    CAMERA_POSITION cx cy cz *
 * FOCUSSED_AT fx fy fz    (watch point) *    ROLL_ANGLE psi (degrees) *
 * FRAME_NUMBER n *    FOCAL_LENGTH f *    F_STOP f_stop *    ZOOM zoom
 */

char           *
read_data_from_line(buff, flag, n, data, idata)
char           *buff, *flag;
double         *data;
int            *idata;
int             n;
{
	int             i, atoi();
	char           *obuff, nflag[100];
	
	obuff = strchr(buff, flag[0]) + strlen(flag);
	for (i = 0; i < n; i++) {
		if (sscanf(obuff, "%s", nflag) == 1) {
			data[i] = atof(nflag);
			idata[i] = atoi(nflag);
			if ((data[i] == 0.0 && !(nflag[0] == '0' || nflag[0] == '.'))) {
				return (0);
			}
			obuff = strchr(obuff, nflag[0]) + strlen(nflag);
		} else
			return (0);
	}
	return (obuff);
}

void 
read_flagged_camera_ip(local_height, camera_ip, buffer, restartPtr)
Camera_ip      *camera_ip;
char           *buffer;
Restart        *restartPtr;
int            *local_height;
{
	char           *buff, flag[100];
	double          data[3];
	int             idata[4];
	
	buff = buffer;
	
	
	/* defaults */
	camera_ip->frame = 0;
	camera_ip->camera_position = vector_copy2(0.0, 0.0, 1.0);
	camera_ip->look_at = vector_copy2(0.0, 0.0, 0.0);
	camera_ip->roll_angle = 0.0;
	camera_ip->focal_length = 1.0;
	camera_ip->aperture = 0.0;
	camera_ip->zoom = 1.0;
	
	/* get rest of flags */
	
	while (sscanf(buff, "%s", flag) == 1) {
		if (strcmp(flag, "master_terminate_stream") == 0) {
			fflush(stdout);
			fprintf(stdout, " slave_terminate_stream ");
			exit(0);
		} else if (strcmp(flag, "CAMERA_TYPE") == 0) {
			buff = strchr(buffer, flag[0]) + strlen(flag);
			if (sscanf(buff, "%s", flag) == 1) {
				if (strcmp(flag, "albedo") == 0)
					camera_ip->camera_type = ALBEDO;
				else if (strcmp(flag, "spherical") == 0)
					camera_ip->camera_type = SPHERICAL;
				else if (strcmp(flag, "planar") == 0)
					camera_ip->camera_type = PLANAR;
				else
					error1("drat:\tcamera parser format error after flag CAMERA_TYPE");
			}
			buff = strchr(buffer, flag[0]) + strlen(flag);
		} else if (strcmp(flag, "PIXELS") == 0) {
			if (!(buff = read_data_from_line(buff, "PIXELS", 4, data, idata)) && !(buff = read_data_from_line(buff, "LS", 4, data, idata)))
				error2("drat:\tcamera parser format error after flag", "PIXELS");
			restartPtr->from = idata[0];
			restartPtr->to = idata[1];
			restartPtr->from_sample = idata[2];
			restartPtr->to_sample = idata[3];
		} else if (strcmp(flag, "LS") == 0) {
			if (!(buff = read_data_from_line(buff, "LS", 4, data, idata)) && !(buff = read_data_from_line(buff, "LS", 4, data, idata)))
				error2("drat:\tcamera parser format error after flag", "LS");
			restartPtr->from = idata[0];
			restartPtr->to = idata[1];
			restartPtr->from_sample = idata[2];
			restartPtr->to_sample = idata[3];
		} else if (strcmp(flag, "CAMERA_POSITION") == 0) {
			/*
			 * *    try to read x,y coords
			 */
			if (!(buff = read_data_from_line(buff, "CAMERA_POSITION", 2, data, idata)))
				error2("drat:\tcamera parser format error after flag", "CAMERA_POSITION");
			/*
			 * *    try to read z coords
			 */
			sscanf(buff, "%s", flag);
			
			if (strcmp(flag, "MAX") == 0) {
				*local_height = 1;
				data[2] = 0.0;
			} else {
				data[2] = atof(flag);
			}
			buff = strchr(buff, flag[0]) + strlen(flag);
			camera_ip->camera_position.x = data[0];
			camera_ip->camera_position.y = data[1];
			camera_ip->camera_position.z = data[2];
		} else if (strcmp(flag, "ZOOM") == 0) {
			if (!(buff = read_data_from_line(buff, "ZOOM", 1, data, idata)))
				error2("drat:\tcamera parser format error after flag", "ZOOM");
			camera_ip->zoom = data[0];
		} else if (strcmp(flag, "FOCUSSED_AT") == 0) {
			if (!(buff = read_data_from_line(buff, "FOCUSSED_AT", 3, data, idata)))
				error2("drat:\tcamera parser format error after flag", "FOCUSSED_AT");
			camera_ip->look_at.x = data[0];
			camera_ip->look_at.y = data[1];
			camera_ip->look_at.z = data[2];
		} else if (strcmp(flag, "ROLL_ANGLE") == 0) {
			if (!(buff = read_data_from_line(buff, "ROLL_ANGLE", 1, data, idata)))
				error2("drat:\tcamera parser format error after flag", "ROLL_ANGLE");
			camera_ip->roll_angle = data[0];
		} else if (strcmp(flag, "FRAME_NUMBER") == 0) {
			if (!(buff = read_data_from_line(buff, "FRAME_NUMBER", 1, data, idata)))
				error2("drat:\tcamera parser format error after flag", "FRAME_NUMBER");
			camera_ip->frame = idata[0];
		} else if (strcmp(flag, "FOCAL_LENGTH") == 0) {
			if (!(buff = read_data_from_line(buff, "FOCAL_LENGTH", 1, data, idata)))
				error2("drat:\tcamera parser format error after flag", "FOCAL_LENGTH");
			camera_ip->focal_length = data[0];
		} else if (strcmp(flag, "F_STOP") == 0) {
			if (!(buff = read_data_from_line(buff, "F_STOP", 1, data, idata)))
				error2("drat:\tcamera parser format error after flag", "F_STOP");
			camera_ip->aperture = data[0];
		} else
			error2("drat:\tunrecognised flag in camera parser format", flag);
	}
	return;
}

/*
 * *    read camera ip
 */
/*char  globalCameraInfo[5000];*/
int 
read_camera_description(cameraFile, local_height, camera_ip, restartPtr)
Camera_ip      *camera_ip;
Restart        *restartPtr;
int            *local_height;
CameraFile     *cameraFile;
{
	char            buffer[5000], *format_str, *format_str1, *format_str2, *format_str3, flag[100];
char  globalCameraInfo[5000];
	int             n;
	FILE           *fp = stdin;
	format_str = "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
	format_str1 = "%d %lf %lf %lf %lf %lf %lf %lf %lf";
	format_str2 = "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf";
	format_str3 = "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
	
	camera_ip->col_fwhm = -1;
	camera_ip->row_fwhm = -1;
	if (cameraFile && cameraFile->fp)
		fp = cameraFile->fp;
	camera_ip->rowextent=camera_ip->colextent=-1;  
	if (fgets(buffer, 5000 - 1, fp) != NULL) {
		strcpy(globalCameraInfo,buffer);
		if (sscanf(buffer, "%s", flag) == 1) {
			/* flagged ip */
			if (strcmp(flag, "CAMERA_TYPE") == 0 || strcmp(flag, "PIXELS") == 0 || strcmp(flag, "LS") == 0) {
				read_flagged_camera_ip(local_height, camera_ip, &buffer[0], restartPtr);
				/* finite aperture default ip */
			}else if ((n = sscanf(buffer, format_str3, &camera_ip->frame, &camera_ip->camera_position.x, &camera_ip->camera_position.y, &camera_ip->camera_position.z, &camera_ip->look_at.x, &camera_ip->look_at.y, &camera_ip->look_at.z, &camera_ip->roll_angle, &camera_ip->colextent, &camera_ip->rowextent, &camera_ip->col_fwhm, &camera_ip->row_fwhm)) == 12) {
				camera_ip->aperture = 0;
				camera_ip->camera_type = PLANAR2;
				/* pinhole default ip */
			} else if ((n = sscanf(buffer, format_str, &camera_ip->frame, &camera_ip->camera_position.x, &camera_ip->camera_position.y, &camera_ip->camera_position.z, &camera_ip->look_at.x, &camera_ip->look_at.y, &camera_ip->look_at.z, &camera_ip->roll_angle, &camera_ip->zoom, &camera_ip->aperture, &camera_ip->focal_length)) == 11){
				camera_ip->camera_type = PLANE;
			}else if ((n = sscanf(buffer, format_str2, &camera_ip->frame, &camera_ip->camera_position.x, &camera_ip->camera_position.y, &camera_ip->camera_position.z, &camera_ip->look_at.x, &camera_ip->look_at.y, &camera_ip->look_at.z, &camera_ip->roll_angle, &camera_ip->colextent, &camera_ip->rowextent)) == 10) {
				camera_ip->aperture = 0;
				camera_ip->camera_type = PLANAR2;
				/* pinhole default ip */
			}else if ((n = sscanf(buffer, format_str1, &camera_ip->frame, &camera_ip->camera_position.x, &camera_ip->camera_position.y, &camera_ip->camera_position.z, &camera_ip->look_at.x, &camera_ip->look_at.y, &camera_ip->look_at.z, &camera_ip->roll_angle, &camera_ip->zoom)) == 9) {
				camera_ip->aperture = 0;
				camera_ip->camera_type = PLANAR;
			} else
				fprintf(stderr, "drat:\twarning invalid line of camera description\n");
			
			return (1);
		}
	}
	return (0);
}

/*
 * *
 * 
 *     camera calculations *
 * 
 */


int 
precalculate_camera_characteristics(verbose, camera_ip, camera_op, image_characteristics, flagbag)
Image_characteristics *image_characteristics;
Camera_ip      *camera_ip;
Camera_op      *camera_op;
int             verbose;
FlagBag        *flagbag;
{
	double          focal_length, U, V, HD=0, circle_of_confusion = (1.0 / 20.0);
	triplet         v;
	
	/*
	 * *    error checking
	 */
	if (camera_ip->zoom == 0.0)
		error1("camera:\tzero zoom factor i/p");
	/*
	 * *    focal point
	 */
	camera_op->paxis = normalise(vector_minus(camera_ip->look_at, camera_ip->camera_position));
	
	U = fabs(V_mod(vector_minus(camera_ip->look_at, camera_ip->camera_position)));
	if (camera_ip->aperture != 0.0) {
		focal_length = camera_ip->focal_length;
		camera_op->format.y = 2.0 * focal_length / camera_ip->zoom;
		camera_op->format.x = (image_characteristics->hd.cols / image_characteristics->hd.rows) * camera_op->format.y;
		circle_of_confusion = MIN(camera_op->format.x / image_characteristics->hd.cols, camera_op->format.y / image_characteristics->hd.rows);
		if (U <= focal_length){
			if (flagbag->joint_probability){
				fprintf(stderr, "WARNING: drat:\tcannot really focus that close (focal length <= %f)\n", (float)U);
			}else{
				error1("drat:\tcannot focus that close (focal length <= %lf)", U);
			}
		}
		V = U * focal_length / (U - focal_length);
	} else {
		if (camera_ip->camera_type == SPHERICAL || camera_ip->camera_type == ALBEDO)
			focal_length = camera_ip->focal_length;
		else
			focal_length = camera_ip->zoom * (image_characteristics->hd.rows) / 2.0;
		camera_op->format.x = image_characteristics->hd.cols;
		camera_op->format.y = image_characteristics->hd.rows;
		V = focal_length;
	}
	if(camera_ip->camera_type == PLANAR2){
		camera_op->format.x = camera_ip->colextent;
		camera_op->format.y = camera_ip->rowextent;
	}
	
	if(camera_ip->row_fwhm>0){
		camera_op->sigma_row=camera_ip->row_fwhm*0.5/( camera_op->format.y * sqrt(-2.0*log(0.5)));
		camera_op->sigma_col=camera_ip->col_fwhm*0.5/(camera_op->format.x * sqrt(-2.0*log(0.5)));
	}else{
		camera_op->sigma_row=0;
		camera_op->sigma_col=0;
	}
	
	camera_op->U = U;
	camera_op->V = V;
	camera_op->focal_point = camera_ip->camera_position;
	camera_op->magnification_factor = V / U;
	camera_op->ppoint = vector_plus(camera_op->focal_point, V_factor(camera_op->paxis, -V));
	if (camera_ip->aperture != 0.0)
		camera_op->aperture = focal_length / (2.0 * camera_ip->aperture);
	else
		camera_op->aperture = 0.0;
	if (camera_op->aperture * circle_of_confusion)
		HD = focal_length * focal_length / (camera_op->aperture * circle_of_confusion);
	
	if (camera_ip->aperture != 0.0)
		camera_op->aperture_fov_factor = 0.5 * camera_op->aperture * camera_op->aperture / (V * V);	/* aperture_fov_factor*
		* cos^3(theta_pixel) =
		* solid angle of
		* aperture (see
					* Rushmeier, Parker &
					* Lee, 1991,
					* "Physically accurate
* synthetic images for
* computer vision
* system design",
					* report GIT-GVU-91-25,
					* Graphics,
					* Visualisation &
					* useability center,
					* college of computing,
					* Georgia Institute of
					* Technology, Atlanta,
					* GA, USA) */
	else
		camera_op->aperture_fov_factor = 1.0;
	
	camera_op->pixel_fov_factor = (((camera_op->format.x / image_characteristics->hd.cols) * (camera_op->format.y / image_characteristics->hd.rows)) / V);
	
	camera_op->watch_point = camera_ip->look_at;
	/*
	 * *    imaging plane coordinate system (u,v)
	 */
	v.x = -camera_op->paxis.x;
	v.y = -camera_op->paxis.y;
	if (v.x == 0.0 && v.y == 0.0) {	/* looking straight down */
		v.y = 1.0;
		v.z = 0.0;
	} else if (camera_op->paxis.z == 0.0) {
		v = vector_copy2(0.0, 0.0, 1.0);
	} else {
		v.z = (v.x * v.x + v.y * v.y) / camera_op->paxis.z;
		v = normalise(v);
		if (camera_op->paxis.z > 0.0)
			v = V_factor(v, -1.0);
	}
camera_op->v = rotate_about(v, camera_op->paxis, camera_ip->roll_angle + 180.0);
camera_op->u = vector_cross(camera_op->paxis, camera_op->v);
camera_op->u = V_factor(camera_op->u, -1.0);
camera_op->focal_length = focal_length;

/* set camera type */

camera_op->camera_type = camera_ip->camera_type;

/*
 * for a spherical camera, this is the ratio of the no of columns in
 * the ** * final row to the sine of the angle represented at the
 * midpoint of that** * row - used in calculating the current no of
 * samples required per row  *    NOTE: *    this method of sampling
 * over a hemisphere has now been *    superceded by using
 * non-uniform sample intervals in zenith angle
 */

if (camera_op->camera_type == SPHERICAL || camera_op->camera_type == ALBEDO) {
    camera_op->spherical_reduction = image_characteristics->hd.cols / sin((M_PI / (2.0 * image_characteristics->hd.rows)) * (image_characteristics->hd.rows - 0.5));
}
if (verbose) {
    switch (camera_op->camera_type) {
		case PLANAR2:
			fprintf(stderr, "\n\tcamera:\t\torthographic:\n");
			break;
		case PLANAR:
			fprintf(stderr, "\n\tcamera:\t\tplanar:\n");
			break;
		case SPHERICAL:
			fprintf(stderr, "\n\tcamera:\t\tspherical:\n");
			break;
		case ALBEDO:
			fprintf(stderr, "\n\tcamera:\t\talbedo (cos-corrected spherical):\n");
			break;
    }
    if(camera_op->camera_type != PLANAR2)fprintf(stderr, "\tfocal length:\t\t\t%f\n", focal_length);
    fprintf(stderr, "\timaging plane format:\t\t%f x %f\n", camera_op->format.x, camera_op->format.y);
    if(camera_op->camera_type != PLANAR2)fprintf(stderr, "\tpixel size (in camera):\t\t%.3f\n", 1.0 / camera_op->magnification_factor);
    if(camera_op->camera_type != PLANAR2)fprintf(stderr, "\timage format size:\t\t%f x %f\n", camera_op->format.x / camera_op->magnification_factor, camera_op->format.y / camera_op->magnification_factor);
    if(camera_op->sigma_row){
		fprintf(stderr, "\tFWHM:\t\t\t%lf %lf\n",camera_ip->col_fwhm,camera_ip->row_fwhm );
		fprintf(stderr, "\tSigma:\t\t\t%lf %lf\n",camera_op->sigma_col,camera_op->sigma_row);
    }
	
    if (camera_op->aperture != 0.0) {
		fprintf(stderr, "\taperture:\t\t\t%f\n", 2.0 * camera_op->aperture);
		fprintf(stderr, "\treal imaging distance:\t\t%f\n", U);
		fprintf(stderr, "\tvirtual imaging distance:\t\t%.3f\n", V);
		fprintf(stderr, "\thyperfocal distance:\t\t%.3f\n", HD);
		fprintf(stderr, "\tdepth of field:\t\t\t%.3f to %.3f\n", HD * U / (HD + U), HD * U / (HD - U));
		fprintf(stderr, "\tmagnification:\t\t\t1.0/%.3f\n", 1.0 / camera_op->magnification_factor);
    }
    fprintf(stderr, "\tfocal point:\t\t\t%.2f %.2f %.2f\n", camera_op->focal_point.x, camera_op->focal_point.y, camera_op->focal_point.z);
    if(camera_op->camera_type != PLANAR2)fprintf(stderr, "\tprincipal point:\t\t%.2f %.2f %.2f\n", camera_op->ppoint.x, camera_op->ppoint.y, camera_op->ppoint.z);
    fprintf(stderr, "\tdirection of principal axis:\t%.2f %.2f %.2f\n", camera_op->paxis.x, camera_op->paxis.y, camera_op->paxis.z);
    fprintf(stderr, "\n\timaging plane::\n");
    fprintf(stderr, "\tu:\t\t\t\t%.2f %.2f %.2f\n", camera_op->u.x, camera_op->u.y, camera_op->u.z);
    fprintf(stderr, "\tv:\t\t\t\t%.2f %.2f %.2f\n", camera_op->v.x, camera_op->v.y, camera_op->v.z);
    
    
}
return (1);
}

void 
calculate_sun_elevation_axis(sun_elevation_axis_Ptr, sun_Ptr)
triplet        *sun_elevation_axis_Ptr, *sun_Ptr;
{
	
	double          nx, ny, nz, sx, sy, sz;
	
	if (sun_Ptr->z == 0.0) {
		sun_elevation_axis_Ptr->z = 1.0;
		sun_elevation_axis_Ptr->x = sun_elevation_axis_Ptr->y = 0.0;
	} else if (sun_Ptr->x == 0.0) {
		sun_elevation_axis_Ptr->x = 1.0;
		sun_elevation_axis_Ptr->y = sun_elevation_axis_Ptr->z = 0.0;
	} else if (sun_Ptr->y == 0.0) {
		sun_elevation_axis_Ptr->y = 1.0;
		sun_elevation_axis_Ptr->x = sun_elevation_axis_Ptr->z = 0.0;
	} else {
		nx = 1;
		ny = 1;
		sx = sun_Ptr->x;
		sy = sun_Ptr->y;
		sz = sun_Ptr->z;
		nz = -(sx + sy) / sz;
		
		*sun_elevation_axis_Ptr = normalise(vector_copy2(nx, ny, nz));
	}
	return;
}


void 
calculate_pixel_variance(n, pixel_variance, pixel_condition)
int             n;
double         *pixel_variance;
PixelCondition *pixel_condition;
{
	*pixel_variance = fabs((pixel_condition->sigma_f_sqr - ((pixel_condition->sigma_f * pixel_condition->sigma_f) / (double) n)) / (double) n);
	return;
}

int 
calculate_reflectance_data(wavebandbag, materialbag, illumination, lambda_min, lambda_width, verbose)
double          lambda_min, lambda_width;
int             verbose;
IlluminationBag *illumination;
WavebandBag    *wavebandbag;
MaterialBag    *materialbag;
{
	int             ns, k, l, sample, quantise_sky_waveband();
	double         sunmag=0., Random(), lambda=0.,*rsr0=NULL,*rsr1=NULL;
	void            calculate_material_lut();
	/*
	 * *
	 * 
	 *    pre-calculate wavelength samples to use *
	 * 
	 *    set initial radiance values to 0
	 */
	if (verbose && materialbag && illumination)
		fprintf(stderr, "direct illumination:\nwavelength\tirradiance\n");
	for (l = 0; l < wavebandbag->sensor_wavebands->no_of_wavebands; l++) {
		ns = wavebandbag->sensor_wavebands->sensor_rsr[l].no_of_samples - 1;
		/*
		 * *    select sample (ns) within waveband at random
		 */
		
		if (ns != 0)
			sample = ((int) (Random() * ns + 0.5));
		else
			sample = 0;
		
		/*
		 * *    calculate sensor wavelengths * 
		 * 
		 *    wavelength of first skymap frame -> lambda_min *
		 * wavelength_interval         -> lambda_width
		 */
		rsr0 = wavebandbag->sensor_wavebands->sensor_rsr[l].rsr[0];
		rsr1 = wavebandbag->sensor_wavebands->sensor_rsr[l].rsr[1];
		lambda = rsr0[sample];
		lambda += (Random() * rsr1[sample]);
		if(illumination && materialbag &&  materialbag->samples && materialbag->samples->wavelength){
			if(illumination->sky_data_Ptr && illumination->sky_data_Ptr->hd.orig_name){ 
				materialbag->samples->wavelength[l] = MAX(0,MMIN(illumination->sky_data_Ptr->hd.num_frame - 1, quantise_sky_waveband(&(illumination->sky_data_Ptr->hd), lambda, wavebandbag->sensor_wavebands->sensor_rsr[l].no_of_samples, lambda_min, lambda_width)));
			}else{
				materialbag->samples->wavelength[l]=0;
			} 
			sunmag = illumination->sun_magnitude[materialbag->samples->wavelength[l]];
			if (verbose)
				fprintf(stderr, " %f\t%f\n", (float)lambda, (float)sunmag);
		}
		if(!wavebandbag->lambda)wavebandbag->lambda=(double *)v_allocate(wavebandbag->sensor_wavebands->no_of_wavebands,sizeof(double));
		
		wavebandbag->lambda[l] = lambda;
		if(!materialbag->material_lut)allocate_material_list(materialbag,wavebandbag);
		if(materialbag && materialbag->material_list){
			for (k = 0; k < materialbag->material_list->no_of_materials; k++)
				calculate_material_lut(wavebandbag->sensor_wavebands->no_of_wavebands,&(materialbag->material_lut[k]), lambda, materialbag->material_list, l, k);
		}
	}
	return (1);
}

void 
get_ray(bb,materialbag, ray, i, j, col, row, camera_op_Ptr, image_characteristics_Ptr, flagbag, bbox)
BigBag *bb;
Ray            *ray;
int             col, row;
Camera_op      *camera_op_Ptr;
Image_characteristics *image_characteristics_Ptr;
double          i, j;
FlagBag        *flagbag;
BBox           *bbox;
void           *materialbag;
{
	triplet         dum_trip, projected_imaging_plane_point, p;
	double          r, theta;
	double          u, v, randomise_variable(), phi;
	
	ray->lengthOfRaySoFar = 0.;
	ray->rayLengthThroughObject=0.; 
	switch (camera_op_Ptr->camera_type) {
		case PLANAR2:
			u = (i + ((double) Random() - 0.5)) * camera_op_Ptr->format.x / image_characteristics_Ptr->hd.cols;
			v = (j + ((double) Random() - 0.5)) * camera_op_Ptr->format.y / image_characteristics_Ptr->hd.rows;
			p = vector_plus(camera_op_Ptr->watch_point, vector_plus(V_factor(camera_op_Ptr->u, u), V_factor(camera_op_Ptr->v, v)));
			ray->origin = p;
			ray->direction = camera_op_Ptr->paxis;
			break; 
		case PLANAR:
			u = (i + ((double) Random() - 0.5)) * camera_op_Ptr->format.x / image_characteristics_Ptr->hd.cols;
			v = (j + ((double) Random() - 0.5)) * camera_op_Ptr->format.y / image_characteristics_Ptr->hd.rows;
			p = vector_plus(camera_op_Ptr->ppoint, vector_plus(V_factor(camera_op_Ptr->u, u), V_factor(camera_op_Ptr->v, v)));
			if (camera_op_Ptr->aperture == 0.0) {
				if (!flagbag->orthographic) {
					ray->origin = camera_op_Ptr->focal_point;
					ray->direction = normalise(vector_minus(camera_op_Ptr->focal_point, p));
					camera_op_Ptr->focal_length = V_mod(vector_minus(camera_op_Ptr->focal_point, p));
				} else {
					u /= -camera_op_Ptr->magnification_factor;
					v /= -camera_op_Ptr->magnification_factor;
					ray->origin = vector_plus(camera_op_Ptr->focal_point, vector_plus(V_factor(camera_op_Ptr->u, u), V_factor(camera_op_Ptr->v, v)));
					
					/*
					 * modified by Lewis June 1998 * to put ray /
					 * camera origin at the focal point
					 */
					/*
					 * ray->origin=camera_op_Ptr->focal_point;
					 * D'OH ... no you dont
					 */
					ray->direction = camera_op_Ptr->paxis;
				}
			} else {
				projected_imaging_plane_point = vector_plus(camera_op_Ptr->watch_point, vector_plus(V_factor(camera_op_Ptr->u, u / camera_op_Ptr->magnification_factor), V_factor(camera_op_Ptr->v, -v / camera_op_Ptr->magnification_factor)));
				r = camera_op_Ptr->aperture * sqrt(Random());
				theta = 2.0 * M_PI * Random();
				ray->origin = vector_plus(camera_op_Ptr->focal_point, vector_plus(V_factor(camera_op_Ptr->u, r * cos(theta)), V_factor(camera_op_Ptr->v, r * sin(theta))));
				ray->direction = normalise(vector_minus(projected_imaging_plane_point, ray->origin));
				camera_op_Ptr->focal_length = V_mod(vector_minus(projected_imaging_plane_point, ray->origin));
			}
				
				/*
				 * if(!flagbag->orthographic){
					 * ray->origin=vector_plus(ray->origin,V_factor(ray->direction
																	* ,-camera_op_Ptr->focal_length)); }
				 */
				break;
		case ALBEDO:
			/* azimuth */
			phi = (col + ((double) Random() - 0.5)) * 2.0 * M_PI / image_characteristics_Ptr->hd.cols;
			v = (row + ((double) Random())) / image_characteristics_Ptr->hd.rows;
			/* zenith */
			theta = asin(sqrt(v));
			p = spherical_to_cartesian(vector_copy2(1.0, phi, (M_PI / 2.0) - theta), 1);
			ray->direction = V_factor(normalise(p), -1.0);
			if (camera_op_Ptr->aperture == 0.0) {
				ray->origin = camera_op_Ptr->focal_point;
			} else {
				r = camera_op_Ptr->aperture * sqrt(Random());
				theta = 2.0 * M_PI * Random();
				ray->origin = vector_plus(camera_op_Ptr->focal_point, vector_plus(V_factor(camera_op_Ptr->u, r * cos(theta)), V_factor(camera_op_Ptr->v, r * sin(theta))));
			}
				if (flagbag->local_height) {
					dum_trip = ray->origin;
					reset_local_z_to_local_max_height(bb,materialbag, &(dum_trip), &(ray->origin), flagbag, bbox);
				}
				ray->origin = vector_plus(ray->origin, V_factor(ray->direction, -camera_op_Ptr->focal_length));
			break;
		case SPHERICAL:
			phi = (col + ((double) Random() - 0.5)) * 2.0 * M_PI / image_characteristics_Ptr->hd.cols;
			v = (row + ((double) Random())) / image_characteristics_Ptr->hd.rows;
			theta = (M_PI / 2.0) * v;
			p = spherical_to_cartesian(vector_copy2(1.0, phi, (M_PI / 2.0) - theta), 1);
			ray->direction = V_factor(normalise(p), -1.0);
			if (camera_op_Ptr->aperture == 0.0) {
				ray->origin = camera_op_Ptr->focal_point;
			} else {
				r = camera_op_Ptr->aperture * sqrt(Random());
				theta = 2.0 * M_PI * Random();
				ray->origin = vector_plus(camera_op_Ptr->focal_point, vector_plus(V_factor(camera_op_Ptr->u, r * cos(theta)), V_factor(camera_op_Ptr->v, r * sin(theta))));
			}
				if (flagbag->local_height)
					reset_local_z_to_local_max_height(bb,materialbag, &(ray->origin), &(ray->origin), flagbag, bbox);
				ray->origin = vector_plus(ray->origin, V_factor(ray->direction, -camera_op_Ptr->focal_length));
			break;
			
	}
	if (flagbag->inverse_direction) {
		ray->origin = vector_plus(ray->origin, V_factor(ray->direction, camera_op_Ptr->focal_length));
		ray->direction = V_factor(ray->direction, -1.0);
	}
	return;
}

void 
allocate_material_list(MaterialBag *materialbag, WavebandBag *wavebandbag)
{
	int             I;
	if(!materialbag->material_list->no_of_materials)return;
	if(materialbag->material_lut){
		for (I = 0; I < materialbag->material_list->no_of_materials; I++) {
			if(materialbag->material_lut[I].srm.diffuse_reflectance)free(materialbag->material_lut[I].srm.diffuse_reflectance);
			if(materialbag->material_lut[I].srm.diffuse_transmittance)free(materialbag->material_lut[I].srm.diffuse_transmittance);
		}
		free(materialbag->material_lut);
	}
	if ((materialbag->material_lut = (Standard_Material_List *) calloc(materialbag->material_list->no_of_materials, sizeof(Standard_Material_List))) == NULL)
		error1("scan_image:\terror in Standard_Material_List core allocation");
	
	for (I = 0; I < materialbag->material_list->no_of_materials; I++) {
		if (materialbag->material_list->material[I].srm.diffuse_reflectance_flag)
			materialbag->material_lut[I].srm.diffuse_reflectance = (double *) v_allocate(wavebandbag->sensor_wavebands->no_of_wavebands, sizeof(double));
		if (materialbag->material_list->material[I].srm.diffuse_transmittance_flag)
			materialbag->material_lut[I].srm.diffuse_transmittance = (double *) v_allocate(wavebandbag->sensor_wavebands->no_of_wavebands, sizeof(double));
	}
	return;
}


/* set array to 0 */
void clearIntArray(int *array,int size){
	int i;
	for(i=0;i<size;i++)array[i]=0;
}

void clearDoubleArray(double *array,int size){
	int i;
	for(i=0;i<size;i++)array[i]=0.;
}

/* set array to 0 */
void clearInt_Array(int **array,int size1,int size2){
	int i,j;
	for(i=0;i<size1;i++)for(j=0;j<size2;j++)array[i][j]=0;
}

void clearDouble_Array(double **array,int size1,int size2){
	int i,j;
	for(i=0;i<size1;i++)for(j=0;j<size2;j++)array[i][j]=0.;
}

/* set array to 0 */
void clearInt__Array(int ***array,int size1,int size2,int size3){
	int i,j,k;
	for(i=0;i<size1;i++)for(j=0;j<size2;j++)for(k=0;k<size2;k++)array[i][j][k]=0;
}

void clearDouble__Array(double ***array,int size1,int size2,int size3){
	int i,j,k;
	for(i=0;i<size1;i++)for(j=0;j<size2;j++)for(k=0;k<size2;k++)array[i][j][k]=0.;
}

void skyRadiance(BigBag *bb,double norm,double *diffuseRadiance,WavebandBag *wavebandbag,IlluminationBag *illumination,triplet *toVector,MaterialBag *materialbag){
	double intersect_sky();
	int i;
	if(bb->flagbag->blackSky){
		for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
      		diffuseRadiance[i] = 0.0;
    	}
	}else{
		if(illumination->sky_flag){
    		for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
      			diffuseRadiance[i] = norm*intersect_sky(materialbag->samples->wavelength[i],illumination->sun_Ptr,illumination->sky_data_Ptr,toVector,&illumination->phi_gap,&illumination->theta_sun,&wavebandbag->theta_gap);
    		}
		}else{
    		for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
      			diffuseRadiance[i] = norm*1.0;
			}
		}
	}
}

void sunRadiance(double norm,double *directRadiance,WavebandBag *wavebandbag,IlluminationBag *illumination,triplet *toVector,MaterialBag *materialbag){
	double intersect_sky(),V_dot();
	triplet vector_copy2();
	
	int i;
	if(strlen(illumination->direct_file)){
		for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
			directRadiance[i] = norm*illumination->sun_magnitude[materialbag->samples->wavelength[i]];
		}
	}else{
		for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
			directRadiance[i] = norm*1.0/MAX(0.00000001,V_dot(*illumination->sun_Ptr,vector_copy2(0.,0.,1.)));
		}
	}
}

void *getReflectance(int isDiffuse,double *out,int type,Material_table *material_Ptr,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag){
	CurrentReflectanceBag current_reflectance_bag;
	int tree_depth=0,mat_count=0;
	void *this=NULL;

	current_reflectance_bag.reflectance_type=type;
	current_reflectance_bag.no_of_diffuse_transmittance_samples=current_reflectance_bag.no_of_diffuse_reflectance_samples=1;
	current_reflectance_bag.lambertian=1.;
	current_reflectance_bag.timer=0;
	
	this=calculate_current_reflectance(isDiffuse,0,material_Ptr->material_proportions[0],DIFFUSE,&current_reflectance_bag,materialbag,wavebandbag,flagbag,out,NULL,&tree_depth,material_Ptr,&mat_count);
        return(this);	
}

void *attenuateRadiance(triplet viewer,triplet illumin,int isDiffuse,Material_table *material,int type,double *radiance,double lambertian,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag){
	static double *reflectance=NULL;
	int i;
	double *d_allocate(),len[2],cosphi,acos(),sqrt();
        double k,bigtheta,rhoc,rho0=1.0,modulation,thetas,thetas_,thetav,relphi;
        void *this=NULL;
	triplet invector,outvector;
	if(!reflectance){
		reflectance=d_allocate(wavebandbag->sensor_wavebands->no_of_wavebands);
	}
	this=getReflectance(isDiffuse,reflectance,type,material,materialbag,wavebandbag,flagbag);
	modulation=1.0;
        if(this != NULL){
	  if(viewer.z<=0.0)viewer=V_factor(viewer,-1.0);
	  if(illumin.z<=0.0)illumin=V_factor(illumin,-1.0);
	  invector=cartesian_to_spherical(viewer);
	  outvector=cartesian_to_spherical(illumin);
          thetas=M_PI_2 + outvector.z;
	  thetas_ = M_PI_2 - outvector.z;
          thetav=M_PI_2 - invector.z;
          relphi=fabs(outvector.y-invector.y)+M_PI;
	  if(relphi<0)relphi=-relphi;
	  while(relphi>2.*M_PI)relphi-=2.*M_PI;
	  if(relphi>M_PI)relphi=M_PI*2-relphi;
        } 
	for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
    		if(this != NULL){
                  if(thetav < M_PI_2 && thetas_ < M_PI_2){
	            k = ((Rpv *)this)[i].k;
		    bigtheta = ((Rpv *)this)[i].bigtheta;
		    rhoc = ((Rpv *)this)[i].rhoc;
	            /* thetas: 100 to 180 degrees (in radians)
		    ** thetav: 0 to 80 degrees (in radians)
		    ** relphi: 0 to 1280 degrees (in radians)
		    */
                    modulation = rpv(thetas,thetav,relphi,rho0,k,bigtheta,rhoc);
	          }else{
		    modulation=0.0;
		    fprintf(stderr,"inconsistency at in rpv angles in camera.c\n");
		  }
                }else modulation=1.0;
		radiance[i] *= reflectance[i] * lambertian * modulation;
	}
	return(this);
}

void *attenuateRadianceRTD(int isDiffuse,int rtd,Material_table *material,int type,double **radiance,double lambertian,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag){
	static double *reflectance=NULL;
	int i;
	double *d_allocate();
  	void *this=NULL;
	
	if(!reflectance)reflectance=d_allocate(wavebandbag->sensor_wavebands->no_of_wavebands);
	
	this=getReflectance(isDiffuse,reflectance,type,material,materialbag,wavebandbag,flagbag);
	
	for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
		radiance[i][rtd] *= reflectance[i] * lambertian;
	}
	return(this);
}
void gatherPixelStatsForRay(bb,norm,nowt,rtd,hitPoint,materialbag,illumination,wavebandbag,objectlist,bbox_Ptr,flagbag,material)
BigBag *bb;
void *nowt;
int rtd;
HitPoint *hitPoint;
Material_table  *material;
IlluminationBag *illumination;
FlagBag        *flagbag;
WavebandBag    *wavebandbag;
MaterialBag    *materialbag;
ObjectList     *objectlist;
BBox		   *bbox_Ptr;
double        norm;
{
	int nBands,i,j,k,l,nMaterials,thisRTD,thisMaterial;
	double directScale,sumDistance;
	static double *radiance=NULL,*diffuseRadiance=NULL;
	double *d_allocate(),v_dot();

	nMaterials = materialbag->material_list->useage->no_of_materials;
        nBands=bb->wavebandbag->sensor_wavebands->no_of_wavebands;	
	if(!radiance)radiance=d_allocate(nBands);
        if(!diffuseRadiance)diffuseRadiance=d_allocate(nBands);
        for(i=0;i<nBands;i++)diffuseRadiance[i]=1.0;	
	/* sunlit/shadowing */
	thisMaterial=hitPoint[0].materialIndex;
	for(l=0;l<bb->nSuns;l++)for(i=0;i<(bb->flagbag->max_ray_depth+1)*nBands;i++){
		bb->ratTree[l].diffuseRadiance[i]=bb->ratTree[l].directRadiance[i]=0.;
	}
		/* diffuse interactions */
	directScale=1.;
	sumDistance=0.;
	thisRTD=rtd-1;
	for(i=0;i<=rtd;i++){
		if(hitPoint[i].hitSky==1){
			i--; /* step back one */
			thisRTD=i;
			/* this is a diffuse interaction at rtd i (0 == 1 interaction) */
			
			/* 
				** only a single diffuse entry 
			 ** so diffuse[array] = skyRadiance(hitPoint[i].toVector)
			 ** multiplied by the reflectances along the way
			 */
			for(j=0;j<nBands;j++)radiance[j]=0.;  
			/* clear radiance array */
			
			skyRadiance(bb,norm,diffuseRadiance,wavebandbag,illumination,&hitPoint[MAX(0,i)].toVector,materialbag);
			if(flagbag->reverse){
				for(;i>=0;i--){
					attenuateRadiance((hitPoint[MAX(0,i)].fromVector),(hitPoint[MAX(0,i)].toVector),TRUE,hitPoint[i].material,hitPoint[i].interactionType,diffuseRadiance,1.0,materialbag,wavebandbag,flagbag);
				}
			}
			/* set the corresponding RTD value */
			for(j=0;j<nBands;j++){
				for(l=0;l<bb->nSuns;l++)bb->ratTree[l].diffuseRadiance[MAX(0,thisRTD)*nBands+j]=diffuseRadiance[j]; 
			}
			break;
		}
	}
	
	/* direct interactions */
	directScale=1.;
	sumDistance=0.;
	for(i=0;i<=thisRTD;i++){
		for(l=0;l<hitPoint[i].nSuns;l++){
			if(hitPoint[i].hitSun[l]==1){
				/* clear radiance array */
				for(j=0;j<nBands;j++)radiance[j]=0.;  
				/* put solar radiance in  */
				sunRadiance(norm,radiance,wavebandbag,illumination,&hitPoint[i].sunVector[l],materialbag);
				/* attenuate radiance by first scattering interactions */
				attenuateRadiance((hitPoint[i].fromVector),(hitPoint[i].sunVector[l]),FALSE,hitPoint[i].material,hitPoint[i].sunInteractionType[l],radiance,1.0,materialbag,wavebandbag,flagbag);
				/* attenuate by projection term */
				for(j=0;j<nBands;j++){
					radiance[j] *= hitPoint[i].lambertian[l]*directScale;
				}
				/* attenuate by other (diffuse) scttering interactions */
				for(k=i-1;k>=0;k--){
					attenuateRadiance((hitPoint[k].fromVector),(hitPoint[k].sunVector[l]),TRUE,hitPoint[k].material,hitPoint[k].interactionType,radiance,1.0,materialbag,wavebandbag,flagbag);
				}
				
				for(j=0;j<nBands;j++){
					bb->ratTree[l].directRadiance[i*nBands+j] += radiance[j];
				}
			}
		}
	}
	/* load up bb->ratTree */
	if(bb->ratTree){
		for(l=0;l<bb->nSuns;l++){
			/* load up intersectionPoints */
			bb->ratTree[l].thisRTD=thisRTD;
			for(i=0;i<=thisRTD;i++){
				bb->ratTree[l].ratmat[i]=*hitPoint[i].material->material;
				
				bb->ratTree[l].diffusePathIntersectType[i]=hitPoint[i].interactionType;
				bb->ratTree[l].directPathIntersectType[i]=hitPoint[i].sunInteractionType[l];
				bb->ratTree[l].hitSun[i]=hitPoint[i].hitSun[l];
				bb->ratTree[l].hitSky[i]=hitPoint[i].hitSky;
				
				bb->ratTree[l].rayLengths[i]=hitPoint[i].fromVectorLength;
				bb->ratTree[l].intersectionPoints[i][0]=hitPoint[i].location.x;
				bb->ratTree[l].intersectionPoints[i][1]=hitPoint[i].location.y;
				bb->ratTree[l].intersectionPoints[i][2]=hitPoint[i].location.z;
			}
		}
	}
}

/*
 * *
 * 
 *    image scanning *
 * 
 */


void clearVector(triplet *v){
	v->x = v->y = v->z = 0.;
}

void clearAHitPoint(HitPoint *hitPoint){
	int i;
	clearVector(&hitPoint->location);
	clearVector(&hitPoint->localNormal);
	clearVector(&hitPoint->fromLocation);
	clearVector(&hitPoint->toVector);
	for(i=0;i<hitPoint->nSuns;i++)clearVector(&hitPoint->sunVector[i]);
	clearVector(&hitPoint->skyVector);
	
	hitPoint->fromVectorLength=0.;
	hitPoint->interactionType=0;
	for(i=0;i<hitPoint->nSuns;i++){
		hitPoint->hitSun[i]=0;
		hitPoint->sunInteractionType[i]=0;
		hitPoint->lambertian[i]=0.;
		hitPoint->distanceBackToViewer[i]=0.;
	}
	hitPoint->hitSky=0;
	hitPoint->material=NULL;
	hitPoint->materialIndex=0;
}

void clearHitPoint(int nRTD,HitPoint *hitPoint){
	int i;
	for(i=0;i<nRTD;i++){
		clearAHitPoint(&hitPoint[i]);
	}
}

double gaussianFunction(double x){
	return((1./sqrt(2.*M_PI))*exp(-x*x/2.));
}

void 
scan_image(argc,argv,timer, bb, bbox_Ptr, camera_op_Ptr, image_characteristics_Ptr, datafile)
int argc;char **argv;
BigBag         *bb;
BBox           *bbox_Ptr;
Camera_op      *camera_op_Ptr;
Image_characteristics *image_characteristics_Ptr;
FILE          **datafile;
int             timer;
{
	IlluminationBag *illumination;
	FlagBag        *flagbag;
	WavebandBag    *wavebandbag;
	MaterialBag    *materialbag;
	PixelVarianceLimits *desired_pixel_variance;
	double          normSum=0.,norm=1.0,i, j, R, C;
	int             noUpdate,ps,timer2,counter, doLidar, nRPP, nBins, nBands, nRTD,  rtd,
		test_image_op_frame = 0, no_of_rays_per_pixel = 0,
		tree_depth = 0, I, J, II, JJ;
	Ray		    newRay(),render(),prevRay;
	ObjectList      objectlist;
	double          lambda_min, lambda_width;
	void            pre_calculate_sun_parameters();
	Material_table  material;
	Ray             prev2,prev3,ray;
	HitPoint *hitPoint;
	int jj;
	static double *normSki=NULL;
	Plane *planeHolder;
	
	int globalJ,globalI;
	prevRay.direction.x=prevRay.direction.y=prevRay.direction.z=0.;
	illumination = bb->illumination;
	flagbag = bb->flagbag;
	wavebandbag = bb->wavebandbag;
	materialbag = bb->materialbag;
	desired_pixel_variance = flagbag->pixel_variance;  
	
	lambda_min = *wavebandbag->lambda_min_Ptr;
	lambda_width = *wavebandbag->lambda_width_Ptr;
	
	/* set up some useful variables  - mostly for the dumper, when I get around to it */
	nBands = wavebandbag->sensor_wavebands->no_of_wavebands;
	nBins = MAX(1,materialbag->samples->no_of_time_bins);
	nRPP = desired_pixel_variance->no_of_rays_per_pixel;
	materialbag->samples->max_diffuse_sampling_depth = MAX(materialbag->samples->max_diffuse_sampling_depth, 1);
	if (materialbag->samples->binStep)doLidar = TRUE;
	else doLidar = FALSE;
	
	/* allocate hitPoint array */
	nRTD = MAX(1, materialbag->samples->max_diffuse_sampling_depth);
	hitPoint=(HitPoint *)v_allocate(nRTD+1,sizeof(HitPoint));
	for(jj=0;jj<nRTD+1;jj++){ 
		hitPoint[jj].nSuns=bb->nSuns;
		hitPoint[jj].hitSun=(int *)v_allocate(bb->nSuns,sizeof(int));
		hitPoint[jj].sunVector=(triplet *)v_allocate(bb->nSuns,sizeof(triplet));
		hitPoint[jj].sunInteractionType=(int *)v_allocate(bb->nSuns,sizeof(int));
		hitPoint[jj].lambertian=(double *)v_allocate(bb->nSuns,sizeof(double)); 
	} 
	
	calculate_sun_elevation_axis(&illumination->sun_elevation_axis, illumination->sun_Ptr);
	if (timer == 0)allocate_material_list(materialbag, wavebandbag);
	
	wavebandbag->lambda = (double *) v_allocate(wavebandbag->sensor_wavebands->no_of_wavebands, sizeof(double));
	C = (image_characteristics_Ptr->hd.cols - 1) / 2.0;
	R = (image_characteristics_Ptr->hd.rows - 1) / 2.0;
	
	if(camera_op_Ptr->sigma_row){
		if(!normSki)
			normSki=v_allocate(image_characteristics_Ptr->hd.cols*image_characteristics_Ptr->hd.rows, sizeof(double));
		/* FWHM */
		normSum=0.;
		for (J=0;J<image_characteristics_Ptr->hd.rows;J++) {
			j=J-R;
			for (I=0;I<image_characteristics_Ptr->hd.cols;I++) {
				i=I-C;
				*(normSki + image_characteristics_Ptr->hd.cols*J+I)=gaussianFunction(i/(camera_op_Ptr->sigma_col*image_characteristics_Ptr->hd.cols)) * gaussianFunction(j/(camera_op_Ptr->sigma_row*image_characteristics_Ptr->hd.rows));
				normSum+=*(normSki + image_characteristics_Ptr->hd.cols*J+I);
			}
		}
		
		normSum /= image_characteristics_Ptr->hd.cols*image_characteristics_Ptr->hd.rows;
		for (J=0;J<image_characteristics_Ptr->hd.rows;J++) {
			for (I=0;I<image_characteristics_Ptr->hd.cols;I++) {
				*(normSki + image_characteristics_Ptr->hd.cols*J+I)/=normSum;
			}
		}
	}
	
	pre_calculate_sun_parameters(illumination->sun_Ptr, &illumination->theta_sun, &illumination->phi_gap, &wavebandbag->theta_gap, illumination->sky_data_Ptr);
	planeHolder=bbox_Ptr->contents.plane;
	
	J = flagbag->restart_Ptr->from;
	I = flagbag->restart_Ptr->from_sample;
	if (flagbag->fixed_wavelength)
		calculate_reflectance_data(wavebandbag, materialbag, illumination, lambda_min, lambda_width, flagbag->verbose);
	
	if(flagbag->restart_Ptr->to <= flagbag->restart_Ptr->from)flagbag->restart_Ptr->to = flagbag->restart_Ptr->from+1;
	if(flagbag->restart_Ptr->to_sample <= flagbag->restart_Ptr->from_sample)flagbag->restart_Ptr->to_sample = flagbag->restart_Ptr->from_sample+1;
	/* loop over image */
	for (j = -R + flagbag->restart_Ptr->from; j <= (R - image_characteristics_Ptr->hd.rows + flagbag->restart_Ptr->to); j++) {
		if (flagbag->verbose)
			fprintf(stderr, "(%d)", J);
		globalJ = J;
		
		/*
		 * * check added to fix bug of only rendering beyond
		 * flagbag->restart_Ptr->from_sample if from_sample * is
		 * specified. Mat 20/2/98
		 */
		I = 0;
		if (J == flagbag->restart_Ptr->from) {
			if (flagbag->restart_Ptr->from_sample) {
				I = flagbag->restart_Ptr->from_sample;
			}
		} else if (J > flagbag->restart_Ptr->from) {
			flagbag->restart_Ptr->from_sample = 0;
		}
		for (i = -C + flagbag->restart_Ptr->from_sample; i <= (C - image_characteristics_Ptr->hd.cols + flagbag->restart_Ptr->to_sample); i++) {	
			/*
			 * need to store this info. as part of illumination
			 * if ranging used (LIDAR)
			 */
			if (materialbag->samples->binStep) {
				illumination->position = camera_op_Ptr;
				illumination->image_Ptr = image_characteristics_Ptr;
				switch (illumination->position->camera_type) {
					case SPHERICAL:
					case ALBEDO:
						fprintf(stderr, "cannot have spherical or albedo camera for ranging simulation at present\n");
						exit(1);
						break;
				}
			} else {
				illumination->position = NULL;
				illumination->image_Ptr = NULL;
			}
			no_of_rays_per_pixel = 0;
			timer2=0;
			ps=0;rtd=0;
			if(camera_op_Ptr->sigma_row){
				norm = *(normSki + image_characteristics_Ptr->hd.cols*J+I);
			}
			while((!flagbag->reverse && no_of_rays_per_pixel < nRPP) || (timer2 == 0 && flagbag->reverse)){
				timer2++;
				counter = 0;
				ps++;
				if (!flagbag->fixed_wavelength && ps == 1)
					calculate_reflectance_data(wavebandbag, materialbag, illumination, lambda_min, lambda_width, 0);
				
				while (((flagbag->reverse && no_of_rays_per_pixel < nRPP) || (!flagbag->reverse && counter == 0))) {
					/* reset volume ray tracing stack counter */
					counter ++;
					noUpdate=0;
					if(ps==1 || rtd <=1){
						bb->whichFacet=0;
						tree_depth = 0;
						get_ray(bb,materialbag, &ray, i, j, I, J, camera_op_Ptr, image_characteristics_Ptr, flagbag, bbox_Ptr);
						if(flagbag->sz>-1e19){
							
							/* reset camera location to flagbag->sz along ray direction */
						{
							double lambda;
							if(ray.direction.z ==0){
								fprintf(stderr,"error: cannot use -sz option for horizontal camera\n");
								exit(1);
								
							}
							lambda = (ray.origin.z - flagbag->sz)/ray.direction.z;
							ray.origin.z = flagbag->sz+flagbag->ray_length_tol;
							ray.origin.x += lambda * ray.direction.x;
							ray.origin.y += lambda * ray.direction.y;
						}
						}
						if(!flagbag->reverse)ray.direction = 
							V_factor(*(illumination->sun_Ptr),(double)-1);
						objectlist.cloneTranslationFlag = 0;
						ray.sourceOfRay = VIEWER;
						ray.lengthOfRaySoFar = 0.;
						if (materialbag->samples->binStep && flagbag->lidar) {
							*(illumination->sun_Ptr) = V_factor(ray.direction, -1.);
						}	/* if ranging (LiDAR) simulation, set
		 * illumination to be initial ray
		 * direction */
						clearHitPoint(nRTD,hitPoint);
						rtd=0;
					}else{
						/* continue on */
						hitPoint[rtd].hitSky=0;
						noUpdate=1;
						ray=prev2;
						prevRay=prev3;
						rtd--;
					}
					rtd=MAX(0,rtd);
					for(;rtd<=nRTD;rtd++){ 
						/* step over each rtd */
						if(noUpdate==0){
							prev3=prev2;
							prev2=prevRay;
							prevRay=ray;
						}else{
							noUpdate=0;
						}
						if(flagbag->sz>-1e19){
							if(rtd==0){
								bbox_Ptr->contents.plane=planeHolder;
							}else{
								bbox_Ptr->contents.plane=NULL;
							}
						}
						ray = render(bb,&(hitPoint[rtd]),&rtd,&ray,materialbag,illumination,wavebandbag,&objectlist,bbox_Ptr,flagbag,&material);
						if(hitPoint[rtd].hitSky==1 || hitPoint[rtd].hitSky== -1)
							break;
						hitPoint[rtd].fromVectorLength = V_mod(vector_minus(hitPoint[rtd].fromLocation,hitPoint[rtd].location));
						ray.ray_length=1e20;ray.lengthOfRaySoFar=0.;ray.rayLengthThroughObject=-1.;
					}
					if(rtd==0)hitPoint[rtd].toVector=prevRay.direction;
					gatherPixelStatsForRay(bb,norm,NULL,nRTD,hitPoint,materialbag,illumination,wavebandbag,&objectlist,bbox_Ptr,flagbag,&material);
					/* process the information gathered in hitPoint */
					if(flagbag->reverse)no_of_rays_per_pixel++;
				}
				/* reverse ray tracing */
				if(! flagbag->reverse){
					/* II : azimuth angle of exitant ray */
					/* phi = atan2(ray.direction.y,ray.direction.x);
					phi /= (2.*M_PI);
					while(phi<0)phi++;
					while(phi>1)phi--;
					II=MAX(0,MIN(phi*image_characteristics_Ptr->hd.cols,image_characteristics_Ptr->hd.cols-1));
					JJ = MAX(0,MIN(image_characteristics_Ptr->hd.rows-1,ray.direction.z * image_characteristics_Ptr->hd.rows));
					*/
					II = MAX(0,MIN(image_characteristics_Ptr->hd.cols-1,((ray.direction.x) * image_characteristics_Ptr->hd.cols * 0.5 + image_characteristics_Ptr->hd.cols * 0.5)));
					JJ = MAX(0,MIN(image_characteristics_Ptr->hd.rows-1,((ray.direction.y) * image_characteristics_Ptr->hd.rows* 0.5 + image_characteristics_Ptr->hd.rows* 0.5)));
				}else{
					II=I;JJ=J;
				}
				if(ps==flagbag->photonSpread){
					no_of_rays_per_pixel++;
					ps=0;
				}
			}
			globalI=I;
			/* end foreach ray within the pixel */
			I++;
		}
		J++;
	}
	/*
	 * *    write out integrated values for whole image frame *    to
	 * file *    also write out direct and diffuse contributions *    if
	 * using parallel version, write out sending_integrated_data *
	 * signal to let master process know what type of data is coming
	 */
	
	if (flagbag->verbose)
		fprintf(stderr, "\n");
	return;
}
