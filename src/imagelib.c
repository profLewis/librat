#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define IMAGEFORMATS_H_BETTER
#include "imagelib.h" 
#include "image_formats.h"
#include <errno.h>
#include <stdint.h>

int dontReWrite = 0;
/*
 * if -DMMAP not stated on command-line, this is ignored * mmap() is not
 * supported by all UNIX systems - this should be changed * for more portable
 * system V schmap() etc.
 */

float getPixelFloat(GenericImage *image, int b, int r, int c)
{
        float pixelFloat(void *value, GenericImage *ImagePtr);

	double         *pixVal, _pixVal;
	float           x;
	pixVal = &_pixVal;
	if (r >= 0 && c >= 0. && r < getImageRows(image) && c < getImageCols(image)) {
		getPixel(image, b, r, c, (void **)&pixVal);
		x = pixelFloat(pixVal, image);
		return (x);
	} else
		return (0.0);
}

int getPixelInt(GenericImage *image, int b, int r, int c)
{
	double         *pixVal, _pixVal;
	int             x;
	pixVal = &_pixVal;

	if (r >= 0 && c >= 0. && r < getImageRows(image) && c < getImageCols(image)) {
		getPixel(image, b, r, c, (void **)&pixVal);
		x = pixelInt(pixVal, image);
		return (x);
	} else
		return (0);
}


short getPixelShort(GenericImage *image, int b, int r, int c)
{
	double         *pixVal, _pixVal;
	short           x;
	pixVal = &_pixVal;

	if (r >= 0 && c >= 0. && r < getImageRows(image) && c < getImageCols(image)) {
		getPixel(image, b, r, c, (void **)&pixVal);
		x = pixelInt(pixVal, image);
		return (x);
	} else
		return (0);
}

unsigned char getPixelChar(GenericImage *image, int b, int r, int c)
{
	double         *pixVal, _pixVal;
	unsigned char   x;
	pixVal = &_pixVal;

	if (r >= 0 && c >= 0. && r < getImageRows(image) && c < getImageCols(image)) {
		getPixel(image, b, r, c, (void **)&pixVal);
		x = pixelInt(pixVal, image);
		return (x);
	} else
		return (0);
}

unsigned char getPixelByte(GenericImage *image, int b, int r, int c)
{
	return (getPixelChar(image, b, r, c));
}


int copyImageData(void *data, GenericImage *ImagePtr, GenericImage *InImagePtr, int fromb, int fromr, int fromc, int tob, int tor, int toc,int skip)
{
	int             b,r,c,rows,cols;
	float           f;

	if(!skip)skip=1;

	rows=getImageRows(ImagePtr);
	cols=getImageCols(ImagePtr);

	for (b = fromb; b < tob; b+=skip) 
	for (r = fromr; r < tor; r++) 
	for (c = fromc; c < toc; c++) {
		/* get the value from the i/p image as a double */
		f=getPixelFloat(InImagePtr, b/skip, r, c);
		switch(getImageFormat(ImagePtr)){
			case IMAGE_BYTE:
			*((unsigned char *)data + b*rows*cols + r*cols + c) = (unsigned char)f;
			break;
			case IMAGE_SHORT:
			*((short *)data + b*rows*cols + r*cols + c) = (short)f;
			break;
			case IMAGE_INT:
			*((int *)data + b*rows*cols + r*cols + c) = (int)f;
			break;
			case IMAGE_FLOAT:
			*((float *)data + b*rows*cols + r*cols + c) = (float)f;
			break;
		}
	}
	return(1);
}

int copyImageMaskData(void *data, GenericImage *ImagePtr, GenericImage *InImagePtr, GenericImage *Mask, int maskValue, int fromb, int fromr, int fromc, int tob, int tor, int toc,int skip)
{
	int             b,r,c,rows,cols,m;
	float           f;

	if(!skip)skip=1;

	rows=getImageRows(ImagePtr);
	cols=getImageCols(ImagePtr);

	for (b = fromb; b < tob; b+=skip) 
	for (r = fromr; r < tor; r++) 
	for (c = fromc; c < toc; c++) {
		/* get the value from the i/p image as a double */
		f=getPixelFloat(InImagePtr, b/skip, r, c);
		m=getPixelInt(Mask, 0, r, c);
		if(m==maskValue)
		switch(getImageFormat(ImagePtr)){
			case IMAGE_BYTE:
			*((unsigned char *)data + b*rows*cols + r*cols + c) = (unsigned char)f;
			break;
			case IMAGE_SHORT:
			*((short *)data + b*rows*cols + r*cols + c) = (short)f;
			break;
			case IMAGE_INT:
			*((int *)data + b*rows*cols + r*cols + c) = (int)f;
			break;
			case IMAGE_FLOAT:
			*((float *)data + b*rows*cols + r*cols + c) = (float)f;
			break;
		}
	}
	return(1);
}

float getValidPixelFloat(GenericImage *image, int b, int r, int c, int *valid)
{
	if (valid && r >= 0 && r < getImageRows(image) && c >= 0 && c < getImageCols(image) && b >= 0 && b < getImageFrames(image)) {
		*valid = TRUE;
		return (getPixelFloat(image, b, r, c));

	} else if (!valid)
		return (getPixelFloat(image, b, r, c));
	*valid = FALSE;
	return (0.0);
}

int getValidPixelInt(GenericImage *image, int b, int r, int c, int *valid)
{
	if (valid && r >= 0 && r < getImageRows(image) && c >= 0 && c < getImageCols(image) && b >= 0 && b < getImageFrames(image)) {
		*valid = TRUE;
		return (getPixelInt(image, b, r, c));

	} else if (!valid)
		return (getPixelInt(image, b, r, c));
	*valid = FALSE;
	return (0);
}

unsigned char getValidPixelChar(GenericImage *image, int b, int r, int c, unsigned char  *valid)
{
	if (valid && r >= 0 && r < getImageRows(image) && c >= 0 && c < getImageCols(image) && b >= 0 && b < getImageFrames(image)) {
		*valid = TRUE;
		return (getPixelChar(image, b, r, c));

	} else if (!valid)
		return (getPixelChar(image, b, r, c));
	*valid = FALSE;
	return (0);
}

unsigned char getValidPixelByte(GenericImage *image, int b, int r, int c, unsigned char  *valid)
{
	return (getValidPixelChar(image, b, r, c, valid));
}

short getValidPixelShort(GenericImage *image, int b, int r, int c, unsigned char  *valid)
{
	if (valid && r >= 0 && r < getImageRows(image) && c >= 0 && c < getImageCols(image) && b >= 0 && b < getImageFrames(image)) {
		*valid = TRUE;
		return (getPixelShort(image, b, r, c));

	} else if (!valid)
		return (getPixelShort(image, b, r, c));
	*valid = FALSE;
	return (0);
}

void setValueFromInt(double *out, GenericImage *image, int value)
{
	float           x;
	unsigned char   y;
	short           z;

	switch (getImageFormat(image)) {
	case IMAGE_FLOAT:
		x = (float) value;
		*((float *) out) = x;
		break;
	case IMAGE_BYTE:
		y = (unsigned char) value;
		*((unsigned char *) out) = y;
		break;
	case IMAGE_SHORT:
		z = (short) value;
		*((short *) out) = z;
		break;
	case IMAGE_INT:
		*((int *) out) = value;
		break;
	}
	return;
}

void setValueFromDouble(double *out, GenericImage *image, double value)
{
	float           x;
	unsigned char   y;
	short           z;

	switch (getImageFormat(image)) {
	case IMAGE_FLOAT:
		x = (float) value;
		*((float *) out) = x;
		break;
	case IMAGE_BYTE:
		y = (unsigned char) value;
		*((unsigned char *) out) = y;
		break;
	case IMAGE_SHORT:
		z = (short) value;
		*((short *) out) = z;
		break;
	case IMAGE_INT:
		*((int *) out) = value;
		break;
	}
	return;
}

void setValueFromFloat(double *out, GenericImage *image, float value)
{
	float           x;
	unsigned char   y;
	short           z;

	switch (getImageFormat(image)) {
	case IMAGE_FLOAT:
		x = (float) value;
		*((float *) out) = x;
		break;
	case IMAGE_BYTE:
		y = (unsigned char) value;
		*((unsigned char *) out) = y;
		break;
	case IMAGE_SHORT:
		z = (short) value;
		*((short *) out) = z;
		break;
	case IMAGE_INT:
		*((int *) out) = value;
		break;
	}
	return;
}

void setValueFromChar(double *out, GenericImage *image, unsigned char value)
{
	float           x;
	unsigned char   y;
	short           z;

	switch (getImageFormat(image)) {
	case IMAGE_FLOAT:
		x = (float) value;
		*((float *) out) = x;
		break;
	case IMAGE_BYTE:
		y = (unsigned char) value;
		*((unsigned char *) out) = y;
		break;
	case IMAGE_SHORT:
		z = (short) value;
		*((short *) out) = z;
		break;
	case IMAGE_INT:
		*((int *) out) = value;
		break;
	}
	return;
}

void setValueFromByte(double *out, GenericImage *image, unsigned char value)
{
	setValueFromChar(out, image, value);
	return;
}

void setValueFromShort(double *out, GenericImage *image, short value)
{
	float           x;
	unsigned char   y;
	short           z;

	switch (getImageFormat(image)) {
	case IMAGE_FLOAT:
		x = (float) value;
		*((float *) out) = x;
		break;
	case IMAGE_BYTE:
		y = (unsigned char) value;
		*((unsigned char *) out) = y;
		break;
	case IMAGE_SHORT:
		z = (short) value;
		*((short *) out) = z;
		break;
	case IMAGE_INT:
		*((int *) out) = value;
		break;
	}
	return;
}


int getImageVerbose(GenericImage *ImagePtr)
{
	return (ImagePtr->verbose);
}

void setImageVerbose(GenericImage *ImagePtr)
{
	ImagePtr->verbose = TRUE;
}

void unsetImageVerbose(GenericImage *ImagePtr)
{
	ImagePtr->verbose = FALSE;
}

void setArrayCheckFlag(GenericImage *ImagePtr, int flag)
{
	ImagePtr->arrayCheckFlag = flag;
	return;
}

int getArrayCheckFlag(GenericImage *ImagePtr)
{
	return (ImagePtr->arrayCheckFlag);
}

void setImageFormat(GenericImage *ImagePtr, int format)
{
	ImagePtr->format = format;
	ImagePtr->dataSize = getDataSize(format);
	ImagePtr->bits_per_pixel = 8 * getDataSize(format);
	return;
}
void setImageFrames(GenericImage *ImagePtr, int frames)
{
	ImagePtr->frames = frames;
	ImagePtr->noElements = ImagePtr->rows * ImagePtr->cols * ImagePtr->frames;
	return;
}
int getImageRows(GenericImage *ImagePtr)
{
	return (ImagePtr->rows);
}
int getImageCols(GenericImage *ImagePtr)
{
	return (ImagePtr->cols);
}
int getImageFrames(GenericImage *ImagePtr)
{
	return (ImagePtr->frames);
}
void setImageRows(GenericImage *ImagePtr, int rows)
{
	ImagePtr->rows = rows;
	ImagePtr->noElements = ImagePtr->rows * ImagePtr->cols * ImagePtr->frames;
	return;
}
void setImageCols(GenericImage *ImagePtr, int cols)
{
	ImagePtr->cols = cols;
	ImagePtr->noElements = ImagePtr->rows * ImagePtr->cols * ImagePtr->frames;
	return;
}
/*
 * *	 only included for backwards compatibility *     GenericImage
 */
GenericImage   *getImageHeader(GenericImage *ImagePtr)
{
	return (ImagePtr);
}

int copyImage(GenericImage *input, GenericImage *output)
{
	if (!input || !output)
		return (0);
	*output = *input;
	return (1);
}
/*
 */
int getImageFormat(GenericImage *ImagePtr)
{
	if (!ImagePtr->supported[(int) ImagePtr->format])
		return (NO_FORMAT);
	return ((int) ImagePtr->format);
}

/*
 * *	BIL/BSQ/etc.
 */
int getArrayAccess(GenericImage *ImagePtr, int format)
{
	return (ImagePtr->arrayAccess);
}

/*
 * *	BIL/BSQ/etc.
 */
void setArrayAccess(GenericImage *ImagePtr, int format)
{
	ImagePtr->arrayAccess = format;
	return;
}

/*
 * *      set up defaults for image
 */
int setImageDefaults(GenericImage *ImagePtr, int inputFlag)
{
        FILE           *setStream(GenericImage *ImagePtr, FILE *stream);
	int             frame;

	ImagePtr->openFlag = FALSE;
	ImagePtr->verbose = FALSE;
	ImagePtr->noElements = ImagePtr->dataSize = 0;
	ImagePtr->imageName = ImagePtr->image = ImagePtr->pixel = NULL;
	ImagePtr->allocatedFlag = ImagePtr->openFlag = ImagePtr->verbose = FALSE;
	ImagePtr->blockSize = 0;/* unknown i.e. a single block for data */
	if (inputFlag == TRUE || inputFlag == CALC_STATS_STDIN)
		setStream(ImagePtr, stdin);
	else
		setStream(ImagePtr, stdout);

	if (inputFlag == CALC_STATS_STDIN || inputFlag == CALC_STATS) {
		if (!(ImagePtr->stats = (Stats *) calloc(1, sizeof(Stats)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(0);
		}
		for (frame = 0; frame < MAXFRAMES; frame++) {
			ImagePtr->stats->min[frame] = HUGE;
			ImagePtr->stats->max[frame] = -HUGE;
		}
	}
	/* set defaults for compulsory fields */
	ImagePtr->frames = ImagePtr->rows = ImagePtr->cols = 0;
	ImagePtr->format = NO_FORMAT;
	ImagePtr->bit_packing = FALSE;	/* this isn't supported anyway */
	ImagePtr->arrayAccess = BSQ;	/* default */
	ImagePtr->image_format = FALSE;	/* undefined */
	ImagePtr->stats = NULL;	/* undefined */
	ImagePtr->arrayCheckFlag = FALSE;
	ImagePtr->bits_per_pixel = 0;
	ImagePtr->seq_name = NULL;
	ImagePtr->seq_desc = NULL;
	ImagePtr->seq_history = NULL;
	ImagePtr->orig_name=NULL;
	ImagePtr->orig_date=NULL;
	/* default - hips */
	queryImageStyle(ImagePtr, -ENVIFORMAT, NULL);
	return (1);
}

void setImageBlockSize(GenericImage *ImagePtr, int size)
{
	ImagePtr->blockSize = size;
}

int getImageBlockSize(GenericImage *ImagePtr, int size)
{
	return (ImagePtr->blockSize);
}



void setImageStyle(GenericImage *ImagePtr, int style)
{
	ImagePtr->image_format = style;
}

int getImageStyle(GenericImage *ImagePtr)
{
	return (ImagePtr->image_format);
}

int pixelInt(void *value, GenericImage *ImagePtr)
{
	int             out;
	switch (getImageFormat(ImagePtr)) {
	case IMAGE_BYTE:
		out = (int) (*((unsigned char *) value));
		break;
	case IMAGE_FLOAT:
		out = (int) (*((float *) value));
		break;
	case IMAGE_INT:
		out = (int) (*((int *) value));
		break;
	case IMAGE_SHORT:
		out = (int) (*((short *) value));
		break;
	default:
		perror("pixelInt: data format not supported");
		exit(0);
	}
	return (out);
}

unsigned char pixelByte(void *value, GenericImage *ImagePtr)
{
	unsigned char   out;
	switch (getImageFormat(ImagePtr)) {
	case IMAGE_BYTE:
		out = (unsigned char) (*((unsigned char *) value));
		break;
	case IMAGE_FLOAT:
		out = (unsigned char) (*((float *) value));
		break;
	case IMAGE_INT:
		out = (unsigned char) (*((int *) value));
		break;
	case IMAGE_SHORT:
		out = (unsigned char) (*((short *) value));
		break;
	default:
		perror("pixelInt: data format not supported");
		exit(0);
	}
	return (out);
}

unsigned char pixelChar(void *value, GenericImage *ImagePtr)
{
	return (pixelByte(value, ImagePtr));
}

float pixelFloat(void *value, GenericImage *ImagePtr)
{
	float           out;
	switch (getImageFormat(ImagePtr)) {
	case IMAGE_BYTE:
		out = (float) (*((unsigned char *) value));
		break;
	case IMAGE_FLOAT:
		out = (float) (*((float *) value));
		break;
	case IMAGE_INT:
		out = (float) (*((int *) value));
		break;
	case IMAGE_SHORT:
		out = (float) (*((short *) value));
		break;
	default:
		perror("pixelInt: data format not supported");
		exit(0);
	}
	return (out);
}

short pixelShort(void *value, GenericImage *ImagePtr)
{
	short           out;
	switch (getImageFormat(ImagePtr)) {
	case IMAGE_BYTE:
		out = (short) (*((unsigned char *) value));
		break;
	case IMAGE_FLOAT:
		out = (short) (*((float *) value));
		break;
	case IMAGE_INT:
		out = (short) (*((int *) value));
		break;
	case IMAGE_SHORT:
		out = (short) (*((short *) value));
		break;
	default:
		perror("pixelInt: data format not supported");
		exit(0);
	}
	return (out);
}


/*
 * *      function to return no. of bytes per pixel
 */
int getDataSize(int pixelFormat)
{
	switch (pixelFormat) {
	case IMAGE_BYTE:
		return ((int)sizeof(unsigned char));
		break;
	case IMAGE_FLOAT:
		return ((int)sizeof(float));
		break;
	case IMAGE_INT:
		return ((int)sizeof(int));
		break;
	case IMAGE_SHORT:
		return ((int)sizeof(short));
		break;
	default:
		return (0);
	}
}

GenericImage   *allocateImageArray(int n, GenericImage *ImagePtr)
{
	GenericImage   *image;

	if (!ImagePtr) {
		if (!(image = (GenericImage *) calloc(n, sizeof(GenericImage)))) {
			fprintf(stderr, "allocateImageArray:\terror in core allocation\n\tattempting to allocate image structure: %d elements of %d bytes\n", n, (int)sizeof(GenericImage));
			exit(0);
		}
	} else
		image = ImagePtr;
	return (image);
}

/*
 * *      function to allocate image array of relevent *      dimensions and
 * data type *
 * 
 * returns 1 on successful output *      exits with code 0 on failure
 */
int allocateImage(GenericImage *ImagePtr)
{
	/*
	 * *      check to see if data already allocated *      if so,
	 * deallocate it
	 */
	if (ImagePtr->imageName == NULL) {	/* hmm.. allow this error,
						 * but don't let it mmap */
		setImageName(ImagePtr, "-");
		ImagePtr->mmap = FALSE;
	}
	if (!ImagePtr->mmap && ImagePtr->allocatedFlag == TRUE) {
		free(ImagePtr->image);
		free(ImagePtr->pixel);
	}
	/*
	 * *      noElements:     no of frames * no of rows * no of columns *
	 * dataSize:       no of bytes per pixel
	 */
	ImagePtr->noElements = ImagePtr->frames * ImagePtr->rows * ImagePtr->cols;
	ImagePtr->dataSize = getDataSize(ImagePtr->format);
	/*
	 * *      allocate sufficient memory for image array
	 */
	if (!ImagePtr->mmap) {
		if (!(ImagePtr->image = (void *) calloc(ImagePtr->noElements, ImagePtr->dataSize))) {
			perror("GenericImage_basis: error in core allocation");
			exit(0);
		}
	} else {

	}
	/*
	 * *      allocate memory for a single pixel of size dataSize
         ** some strange allocation bug if allocate only small (1)
	 */
	if (!(ImagePtr->pixel = (void *) calloc(1+100, ImagePtr->dataSize))) {
		perror("GenericImage_basis: error in core allocation");
		exit(0);
	}
	/*
	 * *      set flag to say image data has been allocated
	 */
	ImagePtr->allocatedFlag = 1;
	/*
	 * *      make sure bits-per-pixel flag is correctly set
	 */
	ImagePtr->bits_per_pixel = ImagePtr->dataSize * 8;
	return (1);
}

int upDateImage(GenericImage *ImagePtr, int argc, char **argv)
{
	if (!(intptr_t) ImagePtr || !(int) argc || !(intptr_t) argv)
		return (0);
	if (ImagePtr->update_header)
		ImagePtr->update_header(ImagePtr, argc, argv);
	return (1);
}


/*
 * *      function to write out image header and data to chosen stream * and,
 * if deallocateFlag is set, deallocate array in memory *      also, if
 * closeFlag is set, close data stream
 */
int writeImage(GenericImage *ImagePtr, int argc, char **argv, int closeFlag, int deallocateFlag,char *env)
{
FILE           *getStream(GenericImage *ImagePtr);
char           *getImageName(GenericImage *ImagePtr);
	/*
	 * *      update header information with current command-line * and
	 * write out header to selected stream
	 */
	if (ImagePtr->update_header)
		ImagePtr->update_header(ImagePtr, argc, argv);

	if (!ImagePtr->write_header) {	/* compulsory */
		perror("writeImage: write header function not set");
		exit(0);
	}
	if (!ImagePtr->openFlag)
		openImage(ImagePtr, FALSE, env);

#ifdef MMAP
	if (!ImagePtr->mmap) {
#endif
		ImagePtr->write_header(ImagePtr);
		if (!fwrite(ImagePtr->image, ImagePtr->dataSize, ImagePtr->noElements, getStream(ImagePtr))) {
			fprintf(stderr,"writeImage: error in writing image data for %s (%d elements of size %d)",getImageName(ImagePtr),ImagePtr->noElements,ImagePtr->dataSize);
			exit(0);
		}
		/*
		 * *      deallocate image memory if so requested
		 */
		if (deallocateFlag && ImagePtr->allocatedFlag) {
			free(ImagePtr->image);
			free(ImagePtr->pixel);
		}
		/*
		 * *      close data stream, if so requested
		 */
		if (closeFlag && ImagePtr->openFlag) {
			/* could put some error checking in here */
			fclose(getStream(ImagePtr));
		}
#ifdef MMAP
	} else {
		/* implement munmap here */
	}
#endif
	/* return 1 if ok */
	return (1);
}

int copyFromHdr(GenericImage *im,struct  header  *hdr){
  im->orig_name = hdr->orig_name;
  im->seq_name = hdr->seq_name;
  im->orig_date = hdr->orig_date;
  im->seq_history = hdr->seq_history;
  im->seq_desc = hdr->seq_desc;
  im->rows = hdr->rows;
  im->cols = hdr->cols;
  im->frames = hdr->num_frame;
  im->bits_per_pixel = hdr->bits_per_pixel;
  im->bit_packing = hdr->bit_packing;
  im->format = hdr->pixel_format;
  return(1);
}


int copyToHdr(struct  header  *hdr,GenericImage *im){
  hdr->orig_name = im->orig_name;
  hdr->seq_name = im->seq_name;
  hdr->orig_date = im->orig_date;
  hdr->seq_history = im->seq_history;
  hdr->seq_desc = im->seq_desc;
  hdr->rows = im->rows;
  hdr->cols = im->cols;
  hdr->num_frame = im->frames;
  hdr->bits_per_pixel = im->bits_per_pixel;
  hdr->bit_packing = im->bit_packing;
  hdr->pixel_format = im->format;
  return(1);
}

int readImage(GenericImage *ImagePtr, char *env)
{
FILE           *getStream(GenericImage *ImagePtr);
	/*
	 * *     read image header
	 */
#ifdef MMAP
	off_t             offset;
	char           *buffer;

	if (!ImagePtr->read_header) {	/* compulsory */
		perror("readImage: read header function not set");
		exit(0);
	}
	if (!ImagePtr->openFlag)
		if(openImage(ImagePtr, TRUE, env)==0)return(0);
	if (!ImagePtr->mmap) {
#endif
        if(!ImagePtr->openFlag)if(openImage(ImagePtr, TRUE, env)==0)return(0);

		ImagePtr->read_header(ImagePtr);
		/*
		 * *      allocate relevent data *      area and set up image
		 * size etc. information
		 */
		allocateImage(ImagePtr);
		/*
		 * *      read in image data (whole image) *
		 * 
		 * could conceivably modify this to read in only * as much data
		 * as it needs to save on memory
		 */
		if ((int)fread(ImagePtr->image, ImagePtr->dataSize, ImagePtr->noElements, getStream(ImagePtr)) != (int)ImagePtr->noElements) {
			perror("GenericImage_basis: error in reading image data");
			fprintf(stderr, "only %d items read\n", ImagePtr->noElements);
			exit(0);
		}
#ifdef MMAP
	} else {
		ImagePtr->read_header(ImagePtr);
		ImagePtr->dataSize = getDataSize(getImageFormat(ImagePtr));
		ImagePtr->noElements = getImageFrames(ImagePtr) * getImageRows(ImagePtr) * getImageCols(ImagePtr);
		offset = ftello(getStream(ImagePtr));
		if (offset % 4 != 0) {	/* can't mmap ... needs to be on a
					 * 4-byte boundary */
#ifdef VERBOSE
			fprintf(stderr, "readImage:\t cant memory-map image ");
			if (ImagePtr->imageName)
				fprintf(stderr, "%s", ImagePtr->imageName);
			fprintf(stderr, " - no. of columns doesnt divide by four\n");

#endif
			ImagePtr->mmap = FALSE;
			allocateImage(ImagePtr);
			if ((int)fread(ImagePtr->image, ImagePtr->dataSize, ImagePtr->noElements, getStream(ImagePtr)) != (int)ImagePtr->noElements) {
				fprintf(stderr, "error reading image: %s\n", ImagePtr->imageName);
				fprintf(stderr, "only %d items read\n", ImagePtr->noElements);
				exit(0);
			}
		} else {
#ifdef VERBOSE 
			if (getImageVerbose(ImagePtr))
				fprintf(stderr, "readImage:\t memory-mapping image ");
			if (getImageVerbose(ImagePtr))
				if (ImagePtr->imageName)
					fprintf(stderr, "%s\n", ImagePtr->imageName);
				else if (getImageVerbose(ImagePtr))
					fprintf(stderr, "\n");

#endif
			allocateImage(ImagePtr);
			rewind(getStream(ImagePtr));
			if ((buffer = (char *) mmap(0, ImagePtr->dataSize * ImagePtr->noElements + offset, PROT_READ, MAP_SHARED, ImagePtr->fd, (off_t)0)) == MAP_FAILED) {
				fprintf(stderr, "error memory-mapping image: %s\n%s\n",ImagePtr->imageName,strerror(errno));
				
				exit(0);
			}
			buffer += offset;
			ImagePtr->image = buffer;
		}

	}
#endif
	/* fclose(getStream(ImagePtr));
	ImagePtr->openFlag=FALSE;*/
	return (1);
}

int defaultIndex(GenericImage *ImagePtr, int frame, int row, int col)
{

	switch (ImagePtr->arrayAccess) {
	case BSQ:
		return ((ImagePtr->rows * ImagePtr->cols * frame + ImagePtr->cols * row + col));
		break;
	case BIL:
		return ((ImagePtr->cols * ImagePtr->frames * row + ImagePtr->frames * col + frame));
		break;
	default:
		perror("defaultIndex: illegal array access format requested");
		exit(0);
		break;
	}
}

int imageArrayIndex(GenericImage *ImagePtr, int frame, int row, int col, int *blankPixel)
{
	int             index;
	/*
	 * *      image array index
	 */
	if (ImagePtr->index != NULL)
		index = ImagePtr->index(ImagePtr, frame, row, col);
	else
		index = defaultIndex(ImagePtr, frame, row, col);
	if (ImagePtr->arrayCheckFlag || index >= ImagePtr->noElements) {
		/* if frame number is wrong, exit */
		if (frame > ImagePtr->frames || frame < 0) {
			perror("getPixel: illegal image frame index requested");
			exit(0);
		}
		switch (ImagePtr->arrayCheckFlag) {
		case BLANK_EDGES:	/* assume NULL data if attempt
					 * invalid array access */
			if (row < 0 || col < 0 || row >= ImagePtr->rows || col >= ImagePtr->cols)
				*blankPixel = TRUE;
			else
				*blankPixel = FALSE;
			break;
		case WRAPAROUND:	/* assume data is periodic */
			col = (col % ImagePtr->cols);
			row = (row % ImagePtr->rows);
			frame = (frame % ImagePtr->frames);
			if (ImagePtr->index != NULL)
				index = ImagePtr->index(ImagePtr, frame, row, col);
			else
				index = defaultIndex(ImagePtr, frame, row, col);
			break;
		case FATAL_ACCESS:	/* fatal error -> exit */
		default:
			if (index < 0 || index > ImagePtr->noElements) {
				fprintf(stderr, "request for: row:%d col:%d rejected\n", row, col);
				perror("getPixel: illegal image pixel requested");
				exit(0);
			}
			break;
		}
	}
	return (index);
}
/*
 * *      get value from array *
 * 
 * this is stored in ImagePtr->pixel, *      the value is also * returned via
 * this pointer
 */
 /* int */
int getPixel(GenericImage *ImagePtr, int frame, int row, int col, void **valuePtr)
{
	int             index;
        char           *getImageName(GenericImage *ImagePtr);

	int             blankPixel;
	/* flag to unset if pixel value is zero */
	blankPixel = FALSE;
	if (!ImagePtr->image) {
		fprintf(stderr, "can't access image %s (not allocated?)\n", getImageName(ImagePtr));
		exit(0);
	}
	if (!ImagePtr->pixel) {
		if (!(ImagePtr->pixel = (void *) calloc(1, 100))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
	}
	index = imageArrayIndex(ImagePtr, frame, row, col, &blankPixel);
	switch (ImagePtr->format) {
	case IMAGE_BYTE:
		if (!blankPixel)
			*((unsigned char *) ImagePtr->pixel) = *((unsigned char *) ImagePtr->image + index);
		else
			*((unsigned char *) ImagePtr->pixel) = 0;
		if (valuePtr != NULL)
			*valuePtr = ((unsigned char *) ImagePtr->image + index);
		break;
	case IMAGE_FLOAT:
		if (!blankPixel)
			*((float *) ImagePtr->pixel) = *((float *) ImagePtr->image + index);
		else
			*((float *) ImagePtr->pixel) = 0.0;
		if (valuePtr != NULL)
			*valuePtr = ((float *) ImagePtr->image + index);
		break;
	case IMAGE_INT:
		if (!blankPixel)
			*((int *) ImagePtr->pixel) = *((int *) ImagePtr->image + index);
		else
			*((int *) ImagePtr->pixel) = 0;
		if (valuePtr != NULL)
			*valuePtr = ((int *) ImagePtr->image + index);
		break;
	case IMAGE_SHORT:
		if (!blankPixel)
			*((short *) ImagePtr->pixel) = *((short *) ImagePtr->image + index);
		else
			*((short *) ImagePtr->pixel) = 0;
		if (valuePtr != NULL)
			*valuePtr = ((short *) ImagePtr->image + index);
		break;
	default:
		perror("getPixel: unrecognised pixel format");
		exit(0);
		break;
	}
	/*
	 * *      return value accessed from array via valuePtr
	 */
	/*
	 * if(blankPixel)return(0); return(1);
	 */
	return(0);
}
/*
 * *      put value in array *
 * 
 * if valuePtr set to NULL, value is taken from ImagePtr->pixel * otherwise from
 * the contents of valuePtr;
 */
int putPixel(GenericImage *ImagePtr, int frame, int row, int col, void *valuePtr)
{
	int             index;
        char           *getImageName(GenericImage *ImagePtr);

	int             blankPixel;	/* flag to unset if pixel value is
					 * zero */
	void           *setValue;

	blankPixel = FALSE;
	if (!ImagePtr->image) {
		fprintf(stderr, "can't access image %s (not allocated?)\n", getImageName(ImagePtr));
		exit(0);
	}
	if (valuePtr == NULL)
		setValue = ImagePtr->pixel;
	else
		setValue = valuePtr;
	index = imageArrayIndex(ImagePtr, frame, row, col, &blankPixel);
	if (blankPixel == TRUE)
		return (0);	/* non-fatal accessing out of array
				 * dimensions */
	switch (ImagePtr->format) {
	case IMAGE_BYTE:
		*((unsigned char *) ImagePtr->image + index) = *((unsigned char *) setValue);
		break;
	case IMAGE_FLOAT:
		*((float *) ImagePtr->image + index) = *((float *) setValue);
		break;
	case IMAGE_INT:
		*((int *) ImagePtr->image + index) = *((int *) setValue);
		break;
	case IMAGE_SHORT:
		*((short *) ImagePtr->image + index) = *((short *) setValue);
		break;
	default:
		perror("getPixel: unrecognised pixel format");
		exit(0);
		break;
	}
	return (1);
}
/*
 * *        open image file for read/write *
 * 
 * if declared as '-' use stdin/stdout
 */
int openImage(GenericImage *ImagePtr, int inputFlag, char *env)
{
#ifdef MMAP
  int             offset, i, j, blockAccumulator = 0, block;
  char           *buffer, *data;
#endif
  char            *imagename;
  FILE *fp=NULL;
  FILE           *getStream(GenericImage *ImagePtr);
  FILE           *setStream(GenericImage *ImagePtr, FILE *stream);
  char           *getImageName(GenericImage *ImagePtr);

  imagename = ImagePtr->imageName;
  if (!imagename) {
    fprintf(stderr, "openImage:\timage name not set: make sure you use setImageName() before attempting to opan an image\n");
    exit(1);
  }
  if (ImagePtr->openFlag == TRUE) {
    ImagePtr->openFlag = FALSE;
    fclose(getStream(ImagePtr));
  }
  /* open file */
  fp= openFile(imagename, inputFlag, env);
  if(fp==0)return(0); 
  setStream(ImagePtr, fp);
  
  /*
   * * set ImagePtr->mmap to a negative number (e.g. -TRUE) * to
   * override MMAP_IP/MMAP_OP env. variables
   */
#ifdef MMAP
  if (strcmp(imagename, "-") == 0 || ImagePtr->mmap < 0)
    ImagePtr->mmap = 0;
  else
    ImagePtr->mmap = 1;
  
  if (ImagePtr->mmap) {
    
    switch (inputFlag) {
    case TRUE:
      ImagePtr->fd = fileno(getStream(ImagePtr));
      break;
    case FALSE:
    default:
      ImagePtr->fd = fileno(getStream(ImagePtr));
      chmod(imagename, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
      /* only bother with rewriting the file if its not big enough */
      /* this is because writing big files can sometimes take a long time */
      if(0){
	/* check the current file - is the header the same */
	GenericImage tmpImage;
	int eof=0;

        setImageDefaults(&tmpImage,FALSE);
        setImageName(&tmpImage,getImageName(ImagePtr));
        setImageNameH(&tmpImage,getImageNameH(ImagePtr));

        tmpImage.openFlag=FALSE;

	openImage(&tmpImage, TRUE, env);
	queryImageStyle(&tmpImage,-ENVIFORMAT,NULL); 
        tmpImage.read_header(&tmpImage);	
        if(getImageRows(&tmpImage) == getImageRows(ImagePtr) && 
	   getImageCols(&tmpImage) == getImageCols(ImagePtr) &&
	   getImageFrames(&tmpImage) == getImageFrames(ImagePtr) &&
	   getImageFormat(&tmpImage) == getImageFormat(ImagePtr)){
	  dontReWrite=TRUE;
	  /* get the offset */
	  
	  offset = ftell(getStream(&tmpImage));
          /* go to end of file */
	  fseek(getStream(&tmpImage),0,SEEK_END);
	  eof=ftell(getStream(&tmpImage));
 
	  /* see if we can find the end of the file */
	  if(eof<offset+getImageRows(&tmpImage)*getImageCols(&tmpImage)*getImageFrames(&tmpImage)*tmpImage.bits_per_pixel/8){
	    if(ImagePtr->verbose)
	      fprintf(stderr,"attempt to seek to the end of existing output file %s failed ... will have to write it out in full\n",getImageName(ImagePtr));
	    offset=0;
	    dontReWrite=FALSE;
	  }
	  
	  
	}
	fclose(getStream(&tmpImage));
	
      }
      if(!dontReWrite){
	ImagePtr->write_header(ImagePtr);	/* write on stream */
	offset = ftell(getStream(ImagePtr));
	/* rewind(getStream(ImagePtr)); */
	if (offset % 4 != 0) {	/* can't mmap ... needs to be
				 * on a 4-byte boundary */
#ifdef VERBOSE
	  fprintf(stderr, "openImage:\t cant memory-map image ");
	  if (ImagePtr->imageName)
	    fprintf(stderr, "%s", ImagePtr->imageName);
	  fprintf(stderr, " - no. of columns doesnt divide by four\n");
	  
#endif
	  ImagePtr->mmap = FALSE;
	  ImagePtr->allocatedFlag = FALSE;
	  allocateImage(ImagePtr);
	} else {
#ifdef VERBOSE
	  if (getImageVerbose(ImagePtr))
	    fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bopenImage:\t memory-mapping image ");
	  if (getImageVerbose(ImagePtr))
	    if (ImagePtr->imageName)
	      fprintf(stderr, "%s\n", ImagePtr->imageName);
	    else if (getImageVerbose(ImagePtr))
	      fprintf(stderr, "\n");
#endif
	  if (getImageBlockSize(ImagePtr) == 0) {
	    if (!(data = (char *) calloc(ImagePtr->noElements * ImagePtr->dataSize, sizeof(char)))) {
	      fprintf(stderr, "error in core allocation");
	      exit(0);
	    }
	    if ((j = (fwrite(data, ImagePtr->dataSize * ImagePtr->noElements, 1, getStream(ImagePtr)))) != 1) {
	      fprintf(stderr, "error writing image file: %s\t%d", getImageName(ImagePtr), j);
	      exit(-1);
	    }
	  } else {
#ifdef VERBOSE
	    if (getImageVerbose(ImagePtr))
	      fprintf(stderr, "writing output image for memory mapping in blocks of %d bytes\n", getImageBlockSize(ImagePtr));
#endif
	    block = MIN(getImageBlockSize(ImagePtr) * ImagePtr->dataSize, ImagePtr->noElements * ImagePtr->dataSize);
	    blockAccumulator = ImagePtr->noElements * ImagePtr->dataSize;
	    if (!(data = (void *) calloc(block, sizeof(char)))) {
	      fprintf(stderr, "error in core allocation");
	      exit(0);
	    }
	    while (blockAccumulator > 0) {
	      if ((j = (fwrite(data, MIN(block, blockAccumulator), 1, getStream(ImagePtr)))) != 1) {
		fprintf(stderr, "error writing image file: %s\t%d", getImageName(ImagePtr), j);
		exit(-1);
	      }
	      if (getImageVerbose(ImagePtr))
		fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b(%ld)  ", blockAccumulator);
	      blockAccumulator -= block;
	      
	    }
	  }
	  if (getImageVerbose(ImagePtr))
	    fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b(0)");
	  rewind(getStream(ImagePtr));
	  free(data);
	}
      }else{
	if (getImageVerbose(ImagePtr)){
	  fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bopenImage:\t memory-mapping image ");
	  if (ImagePtr->imageName)
	    fprintf(stderr, "%s\n", ImagePtr->imageName);
	  else if (getImageVerbose(ImagePtr))
	    fprintf(stderr, "\n");
	}
	/* close the file, then open again */
	fclose(getStream(ImagePtr));
	close(ImagePtr->fd);
	ImagePtr->openFlag=0;
	setStream(ImagePtr, openFile(imagename, inputFlag, env));
	ImagePtr->fd = fileno(getStream(ImagePtr));
      }
      rewind(getStream(ImagePtr));
      if ((buffer = (char *) mmap(NULL, ImagePtr->dataSize * ImagePtr->noElements + offset, PROT_READ | PROT_WRITE, MAP_SHARED, ImagePtr->fd,  (off_t)0)) == NULL) {
	  fprintf(stderr, "error memory-mapping image: %s\n", ImagePtr->imageName);
	  exit(0);
      }
      buffer += offset;
      ImagePtr->image = buffer;
      
      break;
    }
  }
#endif
  if (ImagePtr->allocatedFlag == FALSE) {
    allocateImage(ImagePtr);
  }
  ImagePtr->openFlag = TRUE;
  return(1);
}

int openImageCopy(GenericImage *ImagePtr, GenericImage *InImagePtr, int outFormat, char *env,int skip)
{

	return(openImageMaskedCopy(ImagePtr, InImagePtr, NULL, (int)0, outFormat, env,skip));
}

/*
 * *        open image file for read/write copying input *
 * 
 * if declared as '-' use stdin/stdout
 */
int openImageMaskedCopy(GenericImage *ImagePtr, GenericImage *InImagePtr, GenericImage *MaskImage, int maskValue, int outFormat, char *env,int skip)
{
#ifdef MMAP
	int             b0,b1,row0,col0,row1,col1,offset, i, j, blockAccumulator = 0, block;
	char           *buffer;
	void		*data;
#endif
        FILE           *getStream(GenericImage *ImagePtr);
	char            *imagename;
	if (!InImagePtr || !InImagePtr->openFlag || !InImagePtr->noElements || !InImagePtr->image)
		return (0);

	if(MaskImage && !MaskImage->openFlag){
		fprintf(stderr, "openImage:\tmask image not open - open before entering function\n");
		return(0);
	}

	if (!outFormat)
		outFormat = getImageFormat(InImagePtr);

	setImageFormat(ImagePtr, outFormat);

	imagename = ImagePtr->imageName;
	if (!imagename) {
		fprintf(stderr, "openImage:\timage name not set: make sure you use setImageName() before attempting to open an image\n");
		exit(1);
	}
	if (ImagePtr->openFlag == TRUE) {
		ImagePtr->openFlag = FALSE;
		fclose(getStream(ImagePtr));
	}
	/* open file */
	setImageName(ImagePtr,imagename);
	openImage(ImagePtr, FALSE, env);

	/*
	 * * set ImagePtr->mmap to a negative number (e.g. -TRUE) * to
	 * override MMAP_IP/MMAP_OP env. variables
	 */

	if (strcmp(imagename, "-") == 0 || ImagePtr->mmap < 0)
		ImagePtr->mmap = 0;
	else
		ImagePtr->mmap = 1;
	/* copy image to data */
	if(!MaskImage){
	  copyImageData(ImagePtr->image, ImagePtr, InImagePtr, 0, 0, 0, ImagePtr->frames, ImagePtr->rows, ImagePtr->cols,skip);
	}else{
	  copyImageMaskData(ImagePtr->image, ImagePtr, InImagePtr, MaskImage, maskValue, 0, 0, 0, ImagePtr->frames, ImagePtr->rows, ImagePtr->cols,skip);
	}
	return(1);
}


/* queries & sets image style/format */

int queryImageStyle(GenericImage *ImagePtr, int styleSet, char *env)
{
	int             nstyles, n;
	char           *str, strTmp[20];

	nstyles = N_FORMATS;
	/*
	 * bug fix *	16 June 1994 *	these values are not otherwise
	 * initialized
	 */
	ImagePtr->arrayCheckFlag = 0;
	ImagePtr->mmap = 0;

	if (styleSet >= 0) {	/* ! styleSet on command-line */
		for (n = 0; n < nstyles; n++) {
			/*
			 * test file formats/styles for magic nos., file
			 * extensions etc
			 */
			if (set_format[n] (ImagePtr, TRUE, env)) {
				styleSet = n;
				n = nstyles + 1;
			}
		}
	} else {		/* still need to set up functions & find
				 * index number of styleSet */
		for (n = 0; n < nstyles; n++) {
			if (format_array[n] == -styleSet) {
				set_format[n] (ImagePtr, FALSE, env);
				styleSet = n;
				n = nstyles + 1;
			}
		}

	}
	if (styleSet < 0 || styleSet == nstyles) {	/* can't support file
							 * format */
		str = ImagePtr->imageName;
		if (strcmp(str, "-") == 0) {
			strcpy(strTmp, "stdin/stdout");
			str = strTmp;
		}
		fprintf(stderr, "GetImageStyle:\timage format not supported for file %s\n", str);
		exit(0);
	}
	return (styleSet);
}


FILE           *getStreamH(GenericImage *ImagePtr)
{       
        return (ImagePtr->streamH);
}

FILE           *putStreamH(GenericImage *ImagePtr, FILE *stream)
{
        FILE           *setStreamH(GenericImage *ImagePtr, FILE *stream);
        return (setStreamH(ImagePtr, stream));
}

FILE           *setStreamH(GenericImage *ImagePtr, FILE *stream)
{
        ImagePtr->streamH = stream;
        return (stream);
}


char           *getImageNameH(GenericImage *ImagePtr)
{
        char *imageNameH = NULL;
        if (ImagePtr->imageNameH)
                return (ImagePtr->imageNameH);
        if (ImagePtr->imageName){
          if ( !(imageNameH = (char *)calloc(strlen(ImagePtr->imageName)+5,sizeof(char)))){
            fprintf(stderr, "error allocating name array from image %s\n", (ImagePtr->imageName));
            exit(0);
          }
          if(ImagePtr->imageNameHExt)
            sprintf(imageNameH,"%s.%s",ImagePtr->imageName,ImagePtr->imageNameHExt);
          else
            sprintf(imageNameH,"%s",ImagePtr->imageName);
          ImagePtr->imageNameH = imageNameH;
          return (ImagePtr->imageNameH);
        }
        return ("");
}

FILE           *getStream(GenericImage *ImagePtr)
{
	return (ImagePtr->stream);
}

FILE           *putStream(GenericImage *ImagePtr, FILE *stream)
{
        FILE           *setStream(GenericImage *ImagePtr, FILE *stream);
	return (setStream(ImagePtr, stream));
}

FILE           *setStream(GenericImage *ImagePtr, FILE *stream)
{
	ImagePtr->stream = stream;
	return (stream);
}


char           *getImageName(GenericImage *ImagePtr)
{
	if (ImagePtr->imageName)
		return (ImagePtr->imageName);
	return ("");
}

int setImageName(GenericImage *ImagePtr, char *name){
  int len,isHips=FALSE;char *tail;
  len=strlen(name);
  /*len+=1024;*/

  ImagePtr->imageName=name;
  /* NB only at this point can we start to make some guesses
  ** about the image format
  ** One (the first) big clue to this should be the image name
  */
  if (len > 5){
    /* this could be .hips */
    tail = &(name[len-5]);
    if (!(strcasecmp(tail,".hips")))isHips=TRUE;
  }
  /* set the type to hips */
  if(isHips){
    queryImageStyle(ImagePtr,-HIPSFORMAT,NULL);
  }else{
    queryImageStyle(ImagePtr,-ENVIFORMAT,NULL);
  }
  if(ImagePtr->stream)rewind(ImagePtr->stream);
  if(ImagePtr->streamH)rewind(ImagePtr->streamH);

  return (1);
}

int setImageNameH(GenericImage *ImagePtr, char *name){

  if ( ImagePtr->format == -(ENVIFORMAT-1)){
    ImagePtr->imageNameH=enviTidyImagename(name);
  }else ImagePtr->imageNameH=name;
  return (1);
}

/* this function resets the image to start at frame frame */
GenericImage   *resetImageFrame(GenericImage *ImagePtr, GenericImage *outImagePtr, int frame, int nFrames)
{
	int             index, blankPixel = FALSE;
	index = imageArrayIndex(ImagePtr, frame, 0, 0, &blankPixel);
	if (!outImagePtr) {
		outImagePtr = allocateImageArray(1, NULL);
	}
	copyImage(ImagePtr, outImagePtr);
	switch (ImagePtr->format) {
	case IMAGE_BYTE:
		if (!blankPixel)
			outImagePtr->image = (void *) &(((unsigned char *) ImagePtr->image)[index]);
		else
			outImagePtr->image = (void *) NULL;
		break;
	case IMAGE_FLOAT:
		if (!blankPixel)
			(outImagePtr->image) = (void *) &(((float *) ImagePtr->image)[index]);
		else
			(outImagePtr->image) = (void *) NULL;
		break;
	case IMAGE_INT:
		if (!blankPixel)
			(outImagePtr->image) = (void *) &(((int *) ImagePtr->image)[index]);
		else
			(outImagePtr->image) = (void *) NULL;
		break;
	case IMAGE_SHORT:
		if (!blankPixel)
			(outImagePtr->image) = (void *) &(((short *) ImagePtr->image)[index]);
		else
			(outImagePtr->image) = (void *) NULL;
		break;
	default:
		perror("getPixel: unrecognised pixel format");
		exit(0);
		break;
	}
	outImagePtr->frames = nFrames;
	outImagePtr->noElements = ImagePtr->rows * ImagePtr->cols * ImagePtr->frames;
	return (outImagePtr);
}

/* create an image from scratch */
GenericImage   *setOutImageArray(char *name, int nImages, int nFrames, int nRows, int nCols, int format, int styleSet, char *env, int mmap)
{
	GenericImage   *out;
	int             i;

	out = allocateImageArray(nImages, NULL);
	for (i = 0; i < nImages; i++) {
		setImageDefaults(&out[i], FALSE);	/* image is write only */
		setImageName(&out[i], name);
		setImageVerbose(&out[i]);
		setImageFrames(&out[i], nFrames);
		setImageRows(&out[i], nRows);
		setImageCols(&out[i], nCols);
		if (styleSet > 0) {
			fprintf(stderr, "setImageArray:\t warning: positive value used for styleSet - set a non-positive number\n");
			exit(1);
		}
		queryImageStyle(&out[i], styleSet, env);
		out[i].mmap = mmap;	/* force no memory mapping if -ve */
		if (!(out[i].orig_name = (char *) calloc(100, sizeof(char)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
		if (!(out[i].seq_name = (char *) calloc(100, sizeof(char)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
		if (!(out[i].orig_date = (char *) calloc(100, sizeof(char)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
		if (!(out[i].seq_history = (char *) calloc(100, sizeof(char)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
		if (!(out[i].seq_desc = (char *) calloc(100, sizeof(char)))) {
			fprintf(stderr, "error in core allocation\n");
			exit(1);
		}
		strcpy(out[i].orig_name, "");
		strcpy(out[i].seq_name, "");
		strcpy(out[i].orig_date, "");
		strcpy(out[i].seq_history, "");
		strcpy(out[i].seq_desc, "");
		setImageFormat(&out[i], format);
		allocateImage(&out[i]);
		openImage(&out[i], FALSE, env);
	}
	return (out);
}

GenericImage   *setOutImage(char *name, int nFrames, int nRows, int nCols, int format,int styleSet, char *env, int mmap)
{
	return (setOutImageArray(name, 1, nFrames, nRows, nCols, format, styleSet, env, mmap));
}

/* simple image create */
GenericImage   *setImage(char *name, int nFrames, int nRows, int nCols, int format)
{
	return (setOutImageArray(name, 1, nFrames, nRows, nCols, format, -ENVIFORMAT, NULL, 0));
}
