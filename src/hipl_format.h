/*
**	hipl_format.h
**
**	Addapted from the HIPL Picture Header Format Standard
**	Michael Landy - 2/1/82
**
**      These routines are provided so that UCL source can be compiled 
**      and run at sites that do not have HIPS.
**
**	Note: These supplied routines have to be compiled with this
**      "hipl_format.h" include file. (They will not compile with the  
**      "real" hipl_format.h and vice versa).
**
**	James Pearson UCL P&S 11/6/92 (jpearson@ps.ucl.ac.uk)
*/


#ifndef HIPL_DEFINED
#define HIPL_DEFINED

#include <stdio.h>
#ifdef sgi
#include <malloc.h>
#endif

struct header {
	char *orig_name;	/* An indication of the originator of
				   this sequence. */
	char *seq_name;		/* The sequence name. */
	int num_frame;		/* The number of frames in this sequence. */
	char *orig_date;	/* The date the sequence was originated. */
	int rows;		/* The number of rows in each image,
				   the height. */
	int cols;		/* The number of columns in each image,
				   the width. */
	int bits_per_pixel;	/* The number of significant bits per pixel. */
	int bit_packing;	/* Nonzero if the bits were packed such as to
				   eliminate any unused bits resulting from a
				   bits_per_pixel value which was not an even
				   multiple of eight. */
	int pixel_format;	/* An indication of the format of each pixel. */
	char *seq_history;	/* A description of the sequence of
				   transformations leading up to the
				   current image. */
	char *seq_desc;		/* A free form description of the contents
				   of the sequence. */
};

#define PFBYTE 0
#define PFSHORT 1
#define PFINT 2
#define PFFLOAT 3
#define PFCOMPLEX 4
#define PFASCII 5

#define MSBFIRST 1
#define LSBFIRST 2

#define PFMSBF 30
#define PFLSBF 31

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#define desc_massage(A)		fix_desc(A)

/* halloc should be a library routine that calls calloc and exits if
** no memory available. No one uses it, but its added here anyway
*/
#define halloc(A,B)		calloc(A,B)

#endif /* HIPL_DEFINED */
