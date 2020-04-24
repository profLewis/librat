#include "hipl_format.h"
#include "hiplib.h"
#include "envilib.h"

#define N_FORMATS 2
/* store relevant info. in set_format[*..FORMAT -1] */
#define HIPSFORMAT 1
#define ENVIFORMAT 2


#ifndef IMAGEFORMATS_H_BETTER
#define IMAGEFORMATS_H_BETTER

/* (global)  array of functions */


#else

/* (global)  array of functions */

int	(*set_format[])() = {	
	isHips, isEnvi
};
int format_array[] = {
	HIPSFORMAT, ENVIFORMAT
};

#endif
