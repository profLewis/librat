#include "hips.h"

/*
**	include file for parallel processing
**	of drat
**
**	define structures to store gathered data
*/

struct StorageLinkStruct{
	int	processed;		/* done / not */
	Image_characteristics	frame;	/* multispectral frame storage */
	double	*results;	/* integrated pixel value */
	double	*diffuse_contributions;
	double	*direct_contributions;
	double	*specular_contributions;	/* not used */
	struct StorageLinkStruct	*next_link;
	char	camera_info[3000];	/* camera info command */
	int	frame_number;
	int	slave_number;	/* slave on which being processed */
};

typedef struct StorageLinkStruct StorageLink;

