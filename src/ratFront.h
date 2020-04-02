/*
 *  ratFront.h
 *  ratlib
 *
 *  Created by Philip Lewis on 05/09/2007.
 *  Copyright 2007 UCL. All rights reserved.
 *
 */

#define ORTHOGRAPHIC 1
#define PERSPECTIVE 0
#define DIFFUSE 2 
#define MAXFIELDS 5

#define RESULT_INTEGRAL_MODE_WAVEBAND 0
#define RESULT_INTEGRAL_MODE_SCATTERINGORDER 1
#define RESULT_INTEGRAL_MODE_DISTANCE 2
#define RESULT_INTEGRAL_FORMAT_ASCII 0
#define RESULT_INTEGRAL_FORMAT_BINARY 1

#ifndef _BYTESWAP_H
#define _BYTESWAP_H

static inline unsigned short bswap_16(unsigned short x) {
  return (x>>8) | (x<<8);
}

static inline unsigned int bswap_32(unsigned int x) {
  return (bswap_16(x&0xffff)<<16) | (bswap_16(x>>16));
}

static inline unsigned long long bswap_64(unsigned long long x) {
  return (((unsigned long long)bswap_32(x&0xffffffffull))<<32) | (bswap_32(x>>32));
}
#endif


#ifndef RAT_MAIN
const int ___i = 1;
#endif
#define is_bigendian() ( (*(char*)&___i) == 0 )
#define convertDouble(x) (is_bigendian() ? x : (sizeof(double) == 8 ? bswap_32(x) : (sizeof(double) == 16 ?  bswap_64(x) : -1)))


struct RATorderStore{
  struct RATorderStore *r;
  int nelements;
  int *code;
  int index;
};

typedef struct {
   double **sunlit,**shaded,matcount;
   double **direct,**diffuse;
   unsigned char ***coder;
   int *nsamples;
   char *rootfilename;
   char *filename[2];
   FILE *fp[2];
   int m,n,nbands;
   RATmaterials *ratMat;
   int *counter;
   struct RATorderStore **r;
   int nRatorders;
   int mode;
} RATorder;

/* device structure for illumination source or camera */
typedef struct {

	RATorder *order;
	char **wavebandFiles;
	int nWavebandFiles;
	
	WavebandBag *wavebandbag;

	char *inputImage;
	char *sampleCountImage;
	char *outputImage;
	char *gridMap;
	int PSFRead; /* set this to *read* files rather than calculate */
	char *modulationMap;

/* multiple scattered stuff */
	int nScatteredImages;
        char **scatteredImages;
	int *scatterStart;
	int *scatterEnd;
        double **scatterResultImage;
	
	char *resultImage;
	char *resultPsfImage;
	
	char *name;
	char *filename;
	/* image data */
	int nBands;
	int nBins;
	int nOrders;
	double binStart;
	double binStep;
	double binMax;
	double *imagedata;
	double *psfdata;
	double *radiance;
	double ***directRadiance; /* full radiance description */
        double ***diffuseRadiance;
	double radianceNorm[2];

/* integral results */
  int resultIntegralFormat;
  int resultIntegralMode;
  char *resultIntegralFilename;
  FILE *resultIntegralFp;
  char *resultIntegralFilenameDiffuse;
  FILE *resultIntegralFpDiffuse;

  int resultIntegralCompress;

/* flag for camera type */
  int cameraType;
/* number of pixels */ 
  int nPixels; 
  int rpp;
  
/* focal point */
  triplet focalPoint;
/* principal point (on the imaging plane */
  triplet principalPoint;
  triplet cameraX,cameraY;
  /* physical size */
  double imagingPlaneDimensions[2];
  
/* point spread function image (2D) */
  double *psf;
  int nPsf[2];
  int *psfmap[2];
  int *psfSampleCount;
  double *psfModulation;
  
/* pulse relative magnitude array */
  double *pulse;
  int nPulse;
  double pulseResolution;
  double pulseStart;
  char *pulseType;
  double pulseSD;
  double pulseLength;
  int pulseSamples;
  char *pulseIPFile;
  char *pulseOPFile;
  double *pulseSample;
  double pulseStep;
  double *cumulativePulse;
  
/* derived parameters */
  int nS[2]; 
  /* no. of pixels in the output image */
  triplet focalAxis;
  double focalLength;
  /* axis rotation angle (radians) */
  double twist, zenith, azimuth;
  /* fov x & y angle (degrees) */
  double fov[2];
  double fovMax;
  double aspectRatio;

} RATdevice;

typedef struct {
	int nBands;
	/* store summary refl/radiance image */
	int doSumImage;
	int *hit;
	double *sumImage;
	double *sumRayLength;
        int nScatteredImages;
        double **scatteredImages;
} RATsample;

void RATOutputIntegralFromOrder(char *outputfile,RATorder *orders,int nLidarBins,double *bininfo,double ****radiance,int mode);
double **RATreadSpectra(char *filename);
RATorder *RATreadOrderTerms(double *bininfo,int *nLidarBins,double **fdata,char *rootfilename);
double ****RATassignSpectraToOrder(double **fdata,RATorder *orders,double *bininfo,int nLidarBin);
RATdevice *RATloadCamera(RATdevice *rat,int orthographic,int nBands,int nBins,double binStart,double binStep,double *cameraLocation,double focalLength,double *angles,int rpp,int nPixels,double *imagingPlaneDimensions,double *psf,int *npsf);
int RATputImage(char *name,void *data,int rows,int cols,int frames,int type);
int *RATgetPsfSampleCount(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
float *RATgetResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
float *RATgetPsfRawResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
float *RATgetPsfResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
float *RATgetPsf(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
float *RATgetPsfMap(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
void *RATgetImage(char *name,int *rows,int *cols,int *frames,int *type,int *size);
void *RATgetImageDouble(char *name,int *rows,int *cols,int *frames,int *type);
double *RATgenerateGaussian(char *this,int cols,int rows,double sdx,double sdy,double Cx,double Cy,double thresh,double sum);
float *RATgetPsfModulation(RATdevice *rat,int *rows,int *cols,int *frames,int *type);
int RATScanPSFSample(int *c,int *r,int *schedule,int *active,int *xstart,int *ystart,RATobj *rat,RATdevice *camera,double *illumination);
int RATScanPSF(RATobj *rat,RATdevice *camera,int nCameras,RATdevice *illumination,int nSuns);
int RATScan(RATobj *rat);
int RATunpackRatTree(double modulation,RATobj *rat,RATtree *ratTree,RATdevice *suns,RATdevice *camera,RATsample *pixel,double *norm);
RATdevice *RATreadCameraFile(char *file,RATobj *rat);
RATdevice *RATreadCameraFiles(char **file,int nfiles,RATobj *rat,int type);
RATdevice  *RATreadACameraFile(char *file,RATobj *rat,RATdevice *ratCamera);

void RAToutputCameraDriverData(RATdevice *illumination,int nSuns);
void RAToutputCameraResults(RATobj *ratObj,RATdevice *illumination,int nSuns);
void RAToutputIntegral(RATobj *ratObj,RATdevice *camera);

