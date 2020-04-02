#ifndef _CAMERA_
#define _CAMERA_

/*
**	Plant RAy Tracer (PRAT):
*/

/* coded by Lewis, dept P&S UCL, Gower St. London. Wed Jun 12 1991 */

/*
**
**	 camera structs 
**
*/

typedef struct {
	double	zoom;
	double	aperture;	/* f_stop */
	double	focal_length;
	pair	format;	/* format size */
	double	roll_angle;	/* degrees */
	triplet	camera_position;
	triplet	look_at;
	int	frame;
	int	camera_type;
        double rowextent;
        double colextent;
	double col_fwhm;
	double row_fwhm;
}Camera_ip;

typedef struct {
	triplet	u;		/* focal plane x-direction */
	triplet	v;		/* focal plane x-direction */
	pair	format;		/* format size */
	double	focal_length;
	triplet	ppoint;		/* principal point */
	triplet	paxis; 		/* principal axis */
	double	aperture;	
	triplet focal_point;	/* focal point */
	double	pixel_fov_factor; /* pixel fov / 2PI */
	double	magnification_factor;
	triplet	watch_point;
	double	U;		/* object-lens distance */
	double	V;		/* film-lens distance */
	double	aperture_fov_factor;
	double sigma_row;
	double sigma_col;
	int	camera_type;
/* for a spherical camera, this is the ratio of the no of columns in the **
** final row to the sine of the angle represented at the midpoint of that**
** row - used in calculating the current no of samples required per row  */
	double	spherical_reduction; 
}Camera_op;

typedef struct {
	int	restart;
	int	from;
	int	to;
	int	from_sample;
	int	to_sample;
	int recover;
}Restart;


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

#endif
