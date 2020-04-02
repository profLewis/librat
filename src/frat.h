#ifndef _FRAT
#define _FRAT
#define _ARARAT_
#ifdef RAT_MAIN
#define FRAT_MAIN
#endif
#ifndef Power_Macintosh
#include <limits.h>
#else
#define MAXDOUBLE 1e20
#endif

#include "prat.h"
#include "materials.h"
#ifndef TRANSPARENT 
#define TRANSPARENT 10
#endif
#include <unistd.h>
typedef BigBag RATobj;
double Random();

#ifdef DEBUGGER_
#include <mcheck.h>
#endif
/*########################*/
#ifdef FRAT_MAIN
RATobj *globalRATObj=NULL;
/*########################*/
#else
/*########################*/
extern RATobj *globalRATObj;
#endif
/*########################*/


extern	void	write_hips_image(),reset_local_z_to_local_max_height(),read_default_materials();
int get_mmap_flag(),read_camera_description();
char 	*getenv();
Matrix4		load_identity_matrix4();
void	useage();
triplet	sort_contents_of_bbox();
void	*calloc(),init_header(),update_header(),mmap_read_hips_image(),get_skymap_wavelengths(),read_spectral_file(),update_desc(),scan_image();
int	initialise_wavefront_storage(),parse_prat_wavefront_data(),mmap_write_hips_image_no_free(),sscan_int(),sscan_double(),atoi();
void pload_identity_matrix4();
FILE *openFileForRead();
void freeObject(RATobj *ratObj,int *f);

#endif





