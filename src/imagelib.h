#ifndef IMAGELIB_H
#define IMAGELIB_H

#include "hipl_format.h"
#include "filelib.h"

#define TRUE 1
#define FALSE 0
#define BLANK_EDGES 1
#define WRAPAROUND 2
#define FATAL_ACCESS 3

#ifndef HUGE
#define HUGE 1000000
#endif

#define CALC_STATS_STDIN 3
#define CALC_STATS 5

#define	BIL	0	/* band interleaved */
#define BSQ	1	/* band sequential */

/* image formats currently supported */
#define NO_FORMAT	-1	/* check for failure */
#define IMAGE_BYTE	0	
#define IMAGE_SHORT	1
#define IMAGE_INT	2
#define IMAGE_FLOAT	3
#define NFORMATS	4

/* required for image stats array, other than that, it has no meaning */
#define MAXFRAMES 1280


typedef struct{
        int                statsSet;
 	double min[MAXFRAMES],max[MAXFRAMES];
/* could add more stuff in here */
}Stats;

/* N.B the *basis*
** of this image header format, taken from
** HIPL Picture Header Format Standard
** by Michael Landy (header / i/o routines now in P.D.)
*/

/*
**	this is defined as a Generic image format
**	for internal use with this library
**	- the user must provide own i/o routines
**	for particular image format headers
**
**	note: you do not have to use *all* fields 
**	the only *compulsory* fields are the base-level info. fields
*/

typedef struct{  
/*
**	base-level image information
*/      
        int     frames;      	/* The number of frames in this sequence */
        int     rows;           /* The number of rows in each image */
        int     cols;           /* The number of columns in each image */
	int	arrayAccess;	/* BIL/BSQ etc. */

        int     format;   	/* The format of each pixel, see above */
	int	(*read_header)(void *);	/* read header function */
	void    (*write_header)(void *);	/* write header function */
	int	(*update_header)(void *,int argc, char **argv);	/* optional */
	int	(*index)(void *,int,int,int);		/* function returning array index */

	char	supported[NFORMATS];	
	/* set flags to say if image format is supported */

/*
**	associated information
*/
       	char    *orig_name;     /* The originator of this sequence */
        char    *seq_name;      /* The name of this sequence */
        char    *orig_date;     /* The date the sequence was originated */
        char    *seq_history;   /* The sequence's history of transformations */
        char    *seq_desc;      /* Descriptive information */
        char    *imageName;     /* name of image, if applicable */
        char    *imageNameH;     /* name of header, if applicable */
        char    *imageNameHExt;     /* name of header ext, if applicable */

	int	bit_packing;	/* bit packing (ignored at present) */
        int     arrayCheckFlag; 
	/* flag to set if want to check array access violations */
	/* set to BLANK_EDGES, WRAPAROUND, or FATAL_ACCESS */

/* 
** dummy for holding extra information
** not supported by current header
*/

	void	*info;		
/*
**	derived information
*/
        int     bits_per_pixel; /* The number of significant bits per pixel */
        int     noElements;     /* no of elements in array */
        int     dataSize;       /* no of bytes per pixel */
        int     blockSize;       /* no of bytes per block (0 if unknown) */
        Stats   *stats;		/* image statistics storage */

/* 
**	internal storage information
*/
        void    *image;         /* ip image data */
        void    *pixel;         /* arbitrary data point holder */
        FILE    *stream;        
	/* data stream to be associated with image data */
        FILE    *streamH;
        /* data stream to be associated with header */

/* 
**	internal flag information
*/
        int     verbose;  
        int     allocatedFlag;  
	/* flag to set when image array has been allocated */
        int     openFlag;       
	/* flag to set if file is open */
	int	mmap;
	/* flag to set for memory-mapping images (automatic) */
	int	image_format;	/* HIPS etc. */
	int	fd;	/* don't worry about this .... */

}GenericImage;

#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a<b)?(a):(b))
#endif
#endif

#ifndef IMAGE_H_PROTO
#define IMAGE_H_PROTO
float getPixelFloat(GenericImage *image, int b, int r, int c);
int getPixelInt(GenericImage *image, int b, int r, int c);
short getPixelShort(GenericImage *image, int b, int r, int c);
unsigned char getPixelChar(GenericImage *image, int b, int r, int c);
unsigned char getPixelByte(GenericImage *image, int b, int r, int c);
int copyImageData(void *data, GenericImage *ImagePtr, GenericImage *InImagePtr, int fromb, int fromr, int fromc, int tob, int tor, int toc,int skip);
int copyImageMaskData(void *data, GenericImage *ImagePtr, GenericImage *InImagePtr, GenericImage *Mask, int maskValue, int fromb, int fromr, int fromc, int tob, int tor, int toc,int skip);
float getValidPixelFloat(GenericImage *image, int b, int r, int c, int *valid);
int getValidPixelInt(GenericImage *image, int b, int r, int c, int *valid);
unsigned char getValidPixelChar(GenericImage *image, int b, int r, int c, unsigned char  *valid);
unsigned char getValidPixelByte(GenericImage *image, int b, int r, int c, unsigned char  *valid);
short getValidPixelShort(GenericImage *image, int b, int r, int c, unsigned char  *valid);
void setValueFromInt(double *out, GenericImage *image, int value);
void setValueFromDouble(double *out, GenericImage *image, double value);
void setValueFromFloat(double *out, GenericImage *image, float value);
void setValueFromChar(double *out, GenericImage *image, unsigned char value);
void setValueFromByte(double *out, GenericImage *image, unsigned char value);
void setValueFromShort(double *out, GenericImage *image, short value);
int getImageVerbose(GenericImage *ImagePtr);
void setImageVerbose(GenericImage *ImagePtr);
void unsetImageVerbose(GenericImage *ImagePtr);
void setArrayCheckFlag(GenericImage *ImagePtr, int flag);
int getArrayCheckFlag(GenericImage *ImagePtr);
void setImageFormat(GenericImage *ImagePtr, int format);
void setImageFrames(GenericImage *ImagePtr, int frames);
int getImageRows(GenericImage *ImagePtr);
int getImageCols(GenericImage *ImagePtr);
int getImageFrames(GenericImage *ImagePtr);
void setImageRows(GenericImage *ImagePtr, int rows);
void setImageCols(GenericImage *ImagePtr, int cols);
GenericImage   *getImageHeader(GenericImage *ImagePtr);
int copyImage(GenericImage *input, GenericImage *output);
int getImageFormat(GenericImage *ImagePtr);
int getArrayAccess(GenericImage *ImagePtr, int format);
void setArrayAccess(GenericImage *ImagePtr, int format);
int setImageDefaults(GenericImage *ImagePtr, int inputFlag);
void setImageBlockSize(GenericImage *ImagePtr, int size);
int getImageBlockSize(GenericImage *ImagePtr, int size);
void setImageStyle(GenericImage *ImagePtr, int style);
int getImageStyle(GenericImage *ImagePtr);
int pixelInt(void *value, GenericImage *ImagePtr);
unsigned char pixelByte(void *value, GenericImage *ImagePtr);
unsigned char pixelChar(void *value, GenericImage *ImagePtr);
float pixelFloat(void *value, GenericImage *ImagePtr);
short pixelShort(void *value, GenericImage *ImagePtr);
int getDataSize(int pixelFormat);
GenericImage   *allocateImageArray(int n, GenericImage *ImagePtr);
int allocateImage(GenericImage *ImagePtr);
int upDateImage(GenericImage *ImagePtr, int argc, char **argv);
int writeImage(GenericImage *ImagePtr, int argc, char **argv, int closeFlag, int deallocateFlag,char *env);
int copyFromHdr(GenericImage *im,struct  header  *hdr);
int copyToHdr(struct  header  *hdr,GenericImage *im);
int readImage(GenericImage *ImagePtr, char *env);
int defaultIndex(GenericImage *ImagePtr, int frame, int row, int col);
int imageArrayIndex(GenericImage *ImagePtr, int frame, int row, int col, int *blankPixel);
int getPixel(GenericImage *ImagePtr, int frame, int row, int col, void **valuePtr);
int putPixel(GenericImage *ImagePtr, int frame, int row, int col, void *valuePtr);
int openImage(GenericImage *ImagePtr, int inputFlag, char *env);
int openImageCopy(GenericImage *ImagePtr, GenericImage *InImagePtr, int outFormat, char *env,int skip);
int openImageMaskedCopy(GenericImage *ImagePtr, GenericImage *InImagePtr, GenericImage *MaskImage, int maskValue, int outFormat, char *env,int skip);
int queryImageStyle(GenericImage *ImagePtr, int styleSet, char *env);
FILE           *getStreamH(GenericImage *ImagePtr);
FILE           *putStreamH(GenericImage *ImagePtr, FILE *stream);
FILE           *setStreamH(GenericImage *ImagePtr, FILE *stream);
char           *getImageNameH(GenericImage *ImagePtr);
FILE           *getStream(GenericImage *ImagePtr);
FILE           *putStream(GenericImage *ImagePtr, FILE *stream);
FILE           *setStream(GenericImage *ImagePtr, FILE *stream);
char           *getImageName(GenericImage *ImagePtr);
int setImageName(GenericImage *ImagePtr, char *name);
int setImageNameH(GenericImage *ImagePtr, char *name);
GenericImage   *resetImageFrame(GenericImage *ImagePtr, GenericImage *outImagePtr, int frame, int nFrames);
GenericImage   *setOutImageArray(char *name, int nImages, int nFrames, int nRows, int nCols, int format, int styleSet, char *env, int mmap);
GenericImage   *setOutImage(char *name, int nFrames, int nRows, int nCols, int format,int styleSet, char *env, int mmap);
GenericImage   *setImage(char *name, int nFrames, int nRows, int nCols, int format);

#endif
