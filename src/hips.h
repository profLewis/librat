#ifndef _HIPS_
#define _HIPS_
#include <stdio.h>
#include "hipl_format.h"

typedef struct {
	float	*fdata;
	unsigned char	*bdata;
        short *sdata;
        int *idata;
}Data;

typedef struct {
	Data	data;
	struct header	hd;
	int	fd;
	FILE	*datafp;
	char	*imagename;
}Image_characteristics;
#endif
