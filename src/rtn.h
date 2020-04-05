#ifndef RTN_H
#define RTN_H
#define RTN_NODE 1
#define RTN_LEAF 2
#define RTN_BASE 3
#define RTN_NULL 4
#define RTN_LEAF_SKY 5

#define RTN_DIRECT 1
#define RTN_DIFFUSE 2
#define RTN_SPECULAR 3

struct RTN_struct {
	int style; /* RTN_DIRECT | RTN_DIFFUSE | RTN_SPECULAR */
/* ray length to this node */
	double rayLength;
/* attenuation information */
	double attenuation;
	double *refl; /* nBands */
	double *irrad; /* nBands */
	char type;		/* RTN_NODE | RTN_LEAF | RTN_BASE | RTN_LEAF_SKY */
	int material;	/* index to currentMat with material name given in 
					** materialbag->material_list->useage->materials[currentMat] 
					*/ 
	char reflTrans; /* REFLECTANCE | TRANSMITTANCE - type of interaction of shadow ray */
	int 	rtd;
	struct RTN_struct *prev;	/* previous element in tree */
	struct RTN_struct **nextShadow;	/* next shadow element in tree */
	struct RTN_struct **nextDiffuseR;	/* next DiffuseR element in tree */
	struct RTN_struct **nextDiffuseT;	/* next DiffuseT element in tree */
	struct RTN_struct **nextSpecularR;	/* next SpecularR element in tree */
	struct RTN_struct **nextSpecularT;	/* next SpecularT element in tree */
};

typedef struct RTN_struct RTN;

extern double *distance, *direct, *diffuse;
extern double *distanceTot, *directTot, *diffuseTot;
extern double *directBinTot, *diffuseBinTot;
extern double *directSum, *diffuseSum;
extern int *nDirect, *nDiffuse;
extern RTN **RTNLeafArray, **RTNLeafSkyArray;
extern int nRTNLeafArray, nRTNLeafSkyArray;

extern double  *directPixSum, *diffusePixSum, *directPixTot,
	               *diffusePixTot, *directPixBinTot,
	               *diffusePixBinTot;
extern double  *imageDirectPixTot, *imageDiffusePixTot,
	               *imageDirectPixBinTot, *imageDiffusePixBinTot;
extern double  *imageDirectPixSum, *imageDiffusePixSum;

#endif
