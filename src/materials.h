#ifndef _MATERIALS_
#define _MATERIALS_
#include "rpv.h"
typedef struct {
	Data		data;
	struct header	hd;
}ImageMap;

struct Material_table_struct{
  int	*material;
  double	*material_proportions;
  int	no_of_materials;
  ImageMap *imagemap;
  char	*material_type;
  char	u;
  char	v;
  char	diffuse_transmittance_flag;
  char	diffuse_reflectance_flag;
  struct 	Material_table_struct	**material_table;
  ipair	local_coords;
  double transWeightingThreshold;
  double transWeighting;
  double reflWeighting;
	triplet	*basis;
};

typedef struct Material_table_struct Material_table;

#ifdef PFAT
typedef struct {
	GLuint	texName[1024];			/* IDs for texture maps */
	Material_table	*materialPointer[1024];	/* pointers for actual materials */
	int		nTextures;				/* number of materials / textures */
	GLfloat	lambda[3];				/* wavelengths used */
	GLubyte	***images[1024];		/* texture images */
} Textures;
#ifndef PFAT_TEXTURES
extern Textures pfatTexture;
#endif
#endif

#define	SRM				0
#define	BUMPMAP				1
#define	BRDF_LUT			2
#define PVD_BRDF_MODEL			3
#define HIPL_TEXTURE_MAP		4
#define AHMADD_BRDF_MODEL		5
#define BEGUE_BRDF_MODEL		6
#define PROSPECT_REFLECTANCE_TRANSMITTANCE_MODEL 7
#define PROSPECT_REFLECTANCE_MODEL 	8
#define VOLUMETRIC 9
#define M_TRANSPARENT 10
#define RPV	11

#define	REFLECTANCE			0
#define	TRANSMITTANCE			1

#endif
