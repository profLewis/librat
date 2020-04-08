#ifndef IMAGELIB_H
#define IMAGELIB_H

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
	int	(*read_header)();	/* read header function */
	void    (*write_header)();	/* write header function */
	int	(*update_header)();	/* optional */
	int	(*index)();		/* function returning array index */

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

void	setImageStyle();	/* HIPS etc. */
int	getImageStyle();
int     setImageDefaults();
int     getImageBlockSize();
void    setImageBlockSize();
int     getDataSize();
int     allocateImage();
int     writeImage();
int     readImage();
int     imageArrayIndex();
GenericImage 	*resetImageFrame();
/*int*/ void     getPixel();
int     putPixel();
int     openImage();
int     getImageFormat();
GenericImage *getImageHeader();
int     copyImageHeader();
void    setImageFormat();
void    setImageRows();
void    setImageCols();
void 	setImageFrames();
int getImageFrames();
int getImageRows();
int getImageCols();
GenericImage	*allocateImageArray();
float	pixelFloat();
short	pixelShort();
int	pixelInt();
unsigned char	pixelChar();
unsigned char	pixelByte();
void	setArrayCheckFlag();
int	getArrayCheckFlag();
int	getArrayAccess();
void	setArrayAccess();
int	queryImageStyle();
char	*getImageName();
int	setImageName();
FILE *getStream();
FILE	*putStream(),*setStream();
int	getImageVerbose();
void	setImageVerbose(),unsetImageVerbose();
int	copyImage();
GenericImage *setOutImage(),*setImage(),*setOutImageArray();
float getPixelFloat();
int getPixelInt();
short getPixelShort();
unsigned char getPixelChar();
unsigned char getPixelByte();
float getValidPixelFloat();
int getValidPixelInt();
unsigned char getValidPixelChar();
unsigned char getValidPixelByte();
short getValidPixelShort();
void setValueFromInt();
void setValueFromDouble();
void setValueFromFloat();
void setValueFromChar();
void setValueFromByte();
void setValueFromShort();
FILE *getStreamH(GenericImage *ImagePtr), *putStreamH(GenericImage *ImagePtr, FILE *stream), *setStreamH(GenericImage *ImagePtr,FILE *stream);
char *getImageNameH(GenericImage *ImagePtr);
int setImageNameH(GenericImage *ImagePtr,char* file);
#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a<b)?(a):(b))
#endif

#endif
