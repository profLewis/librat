#ifndef _HIPS_
#define _HIPS_
#include "define_float.h"
#include <stdio.h>

typedef struct {
	float	*fdata;
	unsigned char	*bdata;
}Data;

typedef struct {
	Data	data;
	struct header	hd;
	INT	fd;
	FILE	*datafp;
	char	*imagename;
}Image_characteristics;
#endif
