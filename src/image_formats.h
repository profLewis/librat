#ifndef IMAGEFORMATS_H_BETTER
#define IMAGEFORMATS_H_BETTER

#include "imagelib.h"

#include "hipl_format.h"
extern int isHips();

#define N_FORMATS 2

/* store relevant info. in set_format[*..FORMAT -1] */
#define HIPSFORMAT 1	
#define ENVIFORMAT 2


/* (global)  array of functions */

extern int	(*set_format[])();
extern int format_array[];


#else

#include "imagelib.h"

#include "hipl_format.h"
extern int isHips();

#define N_FORMATS 2

/* store relevant info. in set_format[*..FORMAT -1] */
#define HIPSFORMAT 1	
#define ENVIFORMAT 2


/* (global)  array of functions */

int	(*set_format[])() = {	
	isHips, isEnvi
};
int format_array[] = {
	HIPSFORMAT, ENVIFORMAT
};

#endif
