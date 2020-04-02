#ifndef HIPLIB_H
#define HIPLIB_H
#define TRUE 1
#define FALSE 0
#define BLANK_EDGES 1
#define WRAPAROUND 2
#define FATAL_ACCESS 3

#define MAXFRAMES 1280
typedef struct{
        int                statsSet;
 	double min[MAXFRAMES],max[MAXFRAMES];
/* could add more stuff in here */
}hipsStats;

typedef struct{        
        struct  header hd;      /* image header */
        int     noElements;     /* no of elements in array */
        int     dataSize;       /* no of bytes per pixel */
        void    *image;         /* ip image data */
        void    *pixel;         /* arbitrary data point holder */
        int     allocatedFlag;  /* flag to set when image array has been allocated */
        int     openFlag;       /* flag to set if file is open */
        int     arrayCheckFlag; /* flag to set if want to check array access violations */
        FILE    *stream;        /* data stream to be associated with image data */
        char    *imageName;     /* name of image, if applicable */
                hipsStats        stats;                /* image statistics */
}hips;
int         setImageDefaults();
int     getDataSize();
int     allocateImage();
int                writeImage();
int     readImage();
int     imageArrayIndex();
int     getPixel();
int     putPixel();
int                openImage();
int     getImageFormat();
struct header *getImageHeader();
int        copyImageHeader();
void       setImageFormat();
void       setImageRows();
void       setImageCols();
void 	setImageFrames();
hips	**allocateImageArray();

#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a<b)?(a):(b))
#endif

#endif
