#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define MAX(a,b) (a > b ? a : b)

/* 
**	libhipl : Hipl compatible library routines
**
**	These routines provide the basic header I/O routines
**	to read/write Hips images.
**
**	Routines supported:
**
**	(f)read_header, (f)write_header, fp_(f)read_header,
**	fp_(f)write_header, init_header, hpread, update_desc,
**	desc_massage, update_header
**
**	The fp_ routines are not part of standard HIPS. They use
**	stdio instead of UNIX i/o.
**
**	See relevant man pages for these routines.
**
**	None of this code contains any original HIPL source code.
**	These routines are provided so that UCL source can be compiled
**	and run at sites that do not have HIPS.
**
**	Note: These routines have to be compiled with the supplied
**	"hipl_format.h" include file. (They will not compile with the
**	"real" hipl_format.h and vice versa).
**
**	James Pearson UCL P&S 11/6/92 (jpearson@@ps.ucl.ac.uk)
**	(parts based on the PGM hips reading routine by Jef Poskanzer)
**
**	Updated JCP 1/7/92
**	Added HIPS2 compatible code JCP 7/12/93
**	(adapted from the hipspub routines, Copyright (c) 1992 Michael Landy)
*/
#include <stdio.h>
#include <math.h>

#include "hipl_format.h"

#define SIZE 4096
#define HIPS1 "HIPS"
#define HIPS2 "HIPS\n"
#define UNIT 4

static int fd_read_line(), fp_read_line();
static int fd_write_line(), fp_write_line();
static char *copy_buf(), *add_buf();
static void h2_fp_rhd(), h2_fd_rhd(), hperr();
static void h2clean_header();
static char *h2sgetline();
static char *h2stripspace(), *h2clean_hist();

char *fix_desc();

void fp_fread_header();
void fp_fwrite_header();
void fread_header();
void fwrite_header();

void read_header(struct header *hd){fread_header(0,hd);}
void write_header(struct header *hd){fwrite_header(1,hd);}
void fp_read_header(struct header *hd){fp_fread_header(stdin,hd);}
void fp_write_header(struct header *hd){fp_fwrite_header(stdout,hd);}

void fp_fread_header(fp, hd)
FILE *fp;
struct header *hd;
{
	char buf[SIZE];
	int len,atoi();

	/* Read orig_name. */
	len = fp_read_line(fp, buf, SIZE);

	/* HIPS-2 images use the Magic Number "HIPS\n". Luckly UCL HIPS uses
	   "HIPS " as its Magic Number */

	if(strcmp(buf,HIPS2) == 0)
	{
		h2_fp_rhd(fp,hd);
		h2clean_header(hd);
		return;
	}
	hd->orig_name = copy_buf(buf,len);

	/* Read seq_name. */
	len = fp_read_line(fp, buf, SIZE);
	hd->seq_name = copy_buf(buf,len);

	/* Read num_frame. */
	fp_read_line(fp, buf, SIZE);
	hd->num_frame = atoi(buf);

	/* Read orig_date. */
	len = fp_read_line(fp, buf, SIZE);
	hd->orig_date = copy_buf(buf,len);

	/* Read rows. */
	fp_read_line(fp, buf, SIZE);
	hd->rows = atoi(buf);

	/* Read cols. */
	fp_read_line(fp, buf, SIZE);
	hd->cols = atoi(buf);

	/* Read bits_per_pixel. */
	fp_read_line(fp, buf, SIZE);
	hd->bits_per_pixel = atoi(buf);

	/* Read bit_packing. */
	fp_read_line(fp, buf, SIZE);
	hd->bit_packing = atoi(buf);

	/* Read pixel_format. */
	fp_read_line(fp, buf, SIZE);
	hd->pixel_format = atoi(buf);

	hd->seq_history = 0;
	do
	{
		len = fp_read_line(fp, buf, SIZE);
		hd->seq_history = add_buf(hd->seq_history, buf, len);
	}
	while (buf[strlen(buf)-3] == '|');

	hd->seq_desc = 0;

	while(1)
	{
		len = fp_read_line(fp, buf, SIZE);
		if (strcmp(buf, ".\n") == 0)
			break;
		hd->seq_desc = add_buf(hd->seq_desc, buf, len);
	}
}

static int
fp_read_line(fp, buf, size)
FILE *fp;
char *buf;
int size;
{
	/* reads in one line - easy use fgets */
	if ( fgets(buf, size, fp) == NULL )
		/* hperr("error reading header"); */
                return(0);

	return (strlen(buf));
}
void fread_header(fd, hd)
int fd;
struct header *hd;
{
	char buf[SIZE];
	int len,atoi();

	/* Read orig_name. */
	len = fd_read_line(fd, buf, SIZE);

	/* HIPS-2 images use the Magic Number "HIPS\n". Luckly UCL HIPS uses
	   "HIPS " as its Magic Number */

	if(strcmp(buf,HIPS2) == 0)
	{
		h2_fd_rhd(fd,hd);
		h2clean_header(hd);
		return;
	}
	hd->orig_name = copy_buf(buf,len);

	/* Read seq_name. */
	len = fd_read_line(fd, buf, SIZE);
	hd->seq_name = copy_buf(buf,len);

	/* Read num_frame. */
	fd_read_line(fd, buf, SIZE);
	hd->num_frame = atoi(buf);

	/* Read orig_date. */
	len = fd_read_line(fd, buf, SIZE);
	hd->orig_date = copy_buf(buf,len);

	/* Read rows. */
	fd_read_line(fd, buf, SIZE);
	hd->rows = atoi(buf);

	/* Read cols. */
	fd_read_line(fd, buf, SIZE);
	hd->cols = atoi(buf);

	/* Read bits_per_pixel. */
	fd_read_line(fd, buf, SIZE);
	hd->bits_per_pixel = atoi(buf);

	/* Read bit_packing. */
	fd_read_line(fd, buf, SIZE);
	hd->bit_packing = atoi(buf);

	/* Read pixel_format. */
	fd_read_line(fd, buf, SIZE);
	hd->pixel_format = atoi(buf);

	hd->seq_history = 0;
	do
	{
		len = fd_read_line(fd, buf, SIZE);
		hd->seq_history = add_buf(hd->seq_history, buf, len);
	}
	while (buf[strlen(buf)-3] == '|');

	hd->seq_desc = 0;

	while(1)
	{
		len = fd_read_line(fd, buf, SIZE);
		if (strcmp(buf, ".\n") == 0)
			break;
		hd->seq_desc = add_buf(hd->seq_desc, buf, len);
	}
}

static int
fd_read_line(fd, buf, size)
int fd;
char *buf;
int size;
{
	/* fd_read_line acts like a UNIX I/O version of gets() */
	int	count = 0,hpread();
	char	val;

	size--;

	do
	{
		if(hpread(fd, &val, 1) != 1)
			/* hperr("error reading header");*/
			return(0);

		buf[count++] = val;

		if(count == size)return(0);
			/* hperr("error reading header"); */
	}
	while (val != '\n');

	buf[count] = '\0';

	return(count);
}
static char *
copy_buf(buf,len)
char	*buf;
int	len;
{
	/* allocate memory and copy buffer for header fields */
	char	*ptr;

	if((ptr = (char *)calloc(len+32,1)) == 0)
		hperr("not enough memory for image header");
	
	strcpy(ptr,buf);

	return(ptr);
/*
	if((ptr = strdup(buf)) == 0)
		hperr("not enough memory for image header");

	return(ptr);
*/
}

void init_header(hd,orig_name,seq_name,num_frame,orig_date,rows,
	cols,bits_per_pixel,bit_packing,pixel_format,seq_desc)
struct header *hd;
int num_frame, rows, cols, bits_per_pixel, bit_packing, pixel_format;
char *orig_name, *seq_name, *orig_date, *seq_desc;
{
	hd->orig_name = copy_buf(orig_name,strlen(orig_name));

	hd->seq_name = copy_buf(seq_name,strlen(seq_name));

	hd->num_frame = num_frame;

	hd->orig_date = copy_buf(orig_date,strlen(orig_date));

	hd->rows = rows;

	hd->cols = cols;

	hd->bits_per_pixel = bits_per_pixel;

	hd->bit_packing = bit_packing;

	hd->pixel_format = pixel_format;

	hd->seq_history = "";

	hd->seq_desc = fix_desc(copy_buf(seq_desc,strlen(seq_desc)));
}

int
hpread(int fd, char *buf, int len)
{
	/* used for reads on stdin */
	int in, count;

	count = len;
	do
	{
		in = read(fd,(void *)buf,(size_t)count);
		count -= in;
		buf += in;
	}
	while (count > 0 && in != 0);

	return (len - count);
}

#undef	NEWLINE
#define	NEWLINE putc('\n',fp); len++

void fp_fwrite_header(fp, hd)
FILE *fp;
struct header *hd;
{
	char buf[SIZE];
	int len = 0;
	int i;

	/* make first 4 bytes = HIPS  (magic number?) */
	if(strncmp(hd->orig_name,HIPS1,4) != 0)
	{
		fwrite("HIPS ",5,1,fp);
		len += 5;
	}

	/* write out bits of header */
	len += fp_write_line(fp, hd->orig_name);
	NEWLINE;

	len += fp_write_line(fp, hd->seq_name);
	NEWLINE;

	sprintf(buf,"%d",hd->num_frame);
	len += fp_write_line(fp, buf);
	NEWLINE;

	len += fp_write_line(fp, hd->orig_date);
	NEWLINE;

	sprintf(buf,"%d",hd->rows);
	len += fp_write_line(fp, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->cols);
	len += fp_write_line(fp, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->bits_per_pixel);
	len += fp_write_line(fp, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->bit_packing);
	len += fp_write_line(fp, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->pixel_format);
	len += fp_write_line(fp, buf);
	NEWLINE;

	/* Write out seq_history */
	len += fp_write_line(fp, hd->seq_history);
	NEWLINE;

	/* Write out seq_desc without '\n' */
	len += fp_write_line(fp, fix_desc(hd->seq_desc));

	/* pad header to a multiple of UNIT bytes */
	len += 3;

	len %= UNIT;

	if (len)
		for(i=0;i<UNIT-len;i++)
			putc('\0',fp);

	/* write out '\n' for end of seq_desc and the End-Of-Header */

	fwrite("\n.\n",3,1,fp);
}

static int
fp_write_line(fp, buf)
FILE *fp;
char *buf;
{
	/* writes line without '\n' character at end */
	int num;

	num = strlen(buf);

	if (num == 0)
		return(0);

	if (buf[num-1] == '\n')
/*		buf[--num] = '\0'; */
		num--;

	fwrite(buf,num,1,fp);

	return(num);
}

#undef	NEWLINE
#define	NEWLINE write(fd,"\n",1); len++

/* #define write_header(a) fwrite_header(stdout,a) */

void fwrite_header(fd, hd)
int fd;
struct header *hd;
{
	char buf[SIZE];
	int len = 0;
	int i;

	/* make first 4 bytes = HIPS  (magic number?) */
	if(strncmp(hd->orig_name,HIPS1,4) != 0)
	{
		write(fd,"HIPS ",5);
		len += 5;
	}

	/* write out bits of header */
	len += fd_write_line(fd, hd->orig_name);
	NEWLINE;

	len += fd_write_line(fd, hd->seq_name);
	NEWLINE;

	sprintf(buf,"%d",hd->num_frame);
	len += fd_write_line(fd, buf);
	NEWLINE;

	len += fd_write_line(fd, hd->orig_date);
	NEWLINE;

	sprintf(buf,"%d",hd->rows);
	len += fd_write_line(fd, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->cols);
	len += fd_write_line(fd, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->bits_per_pixel);
	len += fd_write_line(fd, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->bit_packing);
	len += fd_write_line(fd, buf);
	NEWLINE;

	sprintf(buf,"%d",hd->pixel_format);
	len += fd_write_line(fd, buf);
	NEWLINE;

	/* Write out seq_history */
	len += fd_write_line(fd, hd->seq_history);
	NEWLINE;

	/* Write out seq_desc without '\n' */
	len += fd_write_line(fd, fix_desc(hd->seq_desc));

	len += 3;

	len %= UNIT;

	if (len)
		for(i=0;i<UNIT-len;i++)
			write(fd,"\0",1);

	/* write out '\n' for end of seq_desc and the End-Of-Header */

	write(fd,"\n.\n",3);
}

static int
fd_write_line(fd, buf)
int fd;
char *buf;
{
	/* writes line without '\n' character at end */
	int num;

	num = strlen(buf);

	if (num == 0)
		return(0);

	if (buf[num-1] == '\n')
/*		buf[--num] = '\0'; */
		num--;

	write(fd,buf,num);

	return(num);
}
static char *
add_buf(desc, buf, len)
char *desc, *buf;
int len;
{
	int size;
        char *_desc;

	if (desc == 0)
		return(copy_buf(buf,len));

	size = strlen(desc);

	_desc = (char *)calloc(size+MAX(strlen(buf),len)+1,sizeof(char));
        if(_desc==NULL){
          fprintf(stderr,"error in core allocation in libhipl add_buf()\n");
          return(desc);
        }
        sprintf(_desc,"%s%s",desc,buf);
        desc[0] = 0;
        free(desc);
	return(_desc);
}

void update_desc(hd, buf)
struct header	*hd;
char *buf;
{
	/* update header description */
	int	len, size;

	len = strlen(hd->seq_desc);

	size = strlen(buf);

	/* copy orig seq_desc to new buffer */
	/* (allocate extra space) */
	hd->seq_desc = copy_buf(hd->seq_desc, len+size+1);

	if(hd->seq_desc[len-1] != '\n')
		strcat(hd->seq_desc, "\n");

	strcat(hd->seq_desc, buf);

	len = strlen(hd->seq_desc);

	if(hd->seq_desc[len-1] != '\n')
		strcat(hd->seq_desc, "\n");

	hd->seq_desc = fix_desc(hd->seq_desc);
}
	
char *
fix_desc(desc)
char *desc;
{
	/* strips out any "\n.\n" that may creep in to the description */
	char 	*p, *q;
	int	num;
	int	i, j;
	
	num = strlen(desc);
	num -= 2;

	p = desc;

	for (i=0;i<num;i++)
	{
		if(strncmp(p,"\n.\n",3) == 0)
		{
			for(j=i,q=p;j<num;j++,q++)
				*q = *(q + 2);

			*q = '\0';
		}
		else
			p++;
	}
	return(desc);
}
void update_header(hd, argc, argv)
struct header *hd;
int	argc;
char	*argv[];
{
	char	*seq, *prog, *new;
	int	i, slen = 0, nlen = 0;
	long	t;

	/* strip off any leading pathname components */
	if((prog = (char *)strrchr(argv[0],'/')) != 0)
		argv[0] = ++prog;

	/* find out how long the command line is */
	for (i=0;i<argc;i++)
		nlen += strlen(argv[i]);

	nlen += argc;

	/* add a bit for extras */

	nlen += SIZE;

	if ((new = (char *)calloc(nlen,1)) == 0)
		hperr("not enough memory for image description");

	new[0] = '\0';

	/* copy seq_history to a buffer */
	seq = copy_buf(hd->seq_history,strlen(hd->seq_history)+1);

	slen = strlen(seq);

	/* check to see if existing seq is empty */
	if(slen != 0)
	{
		if(seq[slen - 1] != '\n')
			seq[0] = 0;
		else
		{
			/* add on continuation bits */
			seq[slen - 1] = '\0';
			seq=add_buf(seq,"|\\\n",3);
		}
	}

	/* copy command line to new buffer */
	for (i=0;i<argc;i++)
	{
		strcat(new, argv[i]);
		strcat(new, " ");
	}

	/* add date */
	strcat(new, "\"-D ");
	
	t = time(0);
	strcat(new, (char *)ctime(&t));

	nlen = strlen(new);

	/* ctime has '\n' at end of string, replace with " */
	new[nlen - 1] = '\"';

	/* just in case any extra '\n' creeps in, replace with ' ' */
	for(i=0;i<nlen;i++)
		if (new[i] == '\n')
			new[i] = ' ';

	strcat(new, " \n");

	/* concatenate to buffers */
	hd->seq_history = add_buf(seq, new, nlen+2);

	free(new);
}

static void
h2_fp_rhd(fp,hd)
FILE	*fp;
struct header *hd;
{
	char buf[SIZE];
	int len, i;

	/* Read orig_name. */
	len = fp_read_line(fp, buf, SIZE);
	hd->orig_name = copy_buf(buf,len);

	/* Read seq_name. */
	len = fp_read_line(fp, buf, SIZE);
	hd->seq_name = copy_buf(buf,len);

	/* Read num_frame. */
	fp_read_line(fp, buf, SIZE);
	hd->num_frame = atoi(buf);

	/* Read orig_date. */
	len = fp_read_line(fp, buf, SIZE);
	hd->orig_date = copy_buf(buf,len);

	/* Read rows. */
	fp_read_line(fp, buf, SIZE);
	hd->rows = atoi(buf);

	/* Read cols. */
	fp_read_line(fp, buf, SIZE);
	hd->cols = atoi(buf);

	/* Skip then Region of Interest bits */
	for(i=0;i<4;i++)
		fp_read_line(fp, buf, SIZE);

	/* Read pixel_format. */
	fp_read_line(fp, buf, SIZE);
	hd->pixel_format = atoi(buf);

	/* Skip colours */
	fp_read_line(fp, buf, SIZE);

	/* Read sequence history */
	fp_read_line(fp, buf, SIZE);
	len = atoi(buf);

	if((hd->seq_history = (char *)calloc(len+1,1)) == 0)
		hperr("not enough memory for image history");

	if (fread(hd->seq_history,len,1,fp) != 1)return;
		/* hperr("error reading header"); */

	/* Read sequence description */
	fp_read_line(fp, buf, SIZE);
	len = atoi(buf);

	if((hd->seq_desc = (char *)calloc(len+1,1)) == 0)
		hperr("not enough memory for image history");

	if (fread(hd->seq_desc,len,1,fp) != 1)return;
		/* hperr("error reading header"); */

	/* Skip rest of header */
	fp_read_line(fp, buf, SIZE);
	len = atoi(buf);

	for (i=0;i<len;i++)
		fp_read_line(fp, buf, SIZE);

	fp_read_line(fp, buf, SIZE);
	len = atoi(buf);

	if (len)
	{
		for (i=0;i<len;i++)
			if(getc(fp) == EOF)return;
				/* hperr("error reading header"); */
/*
		char	*tmp;

		if ((tmp = (char *) malloc(len)) == 0)return;

		if (fread(tmp,len,1,fp) != 1)return;

		free(tmp);
*/
	}
}

static void
h2_fd_rhd(fd,hd)
int	fd;
struct header *hd;
{
	char buf[SIZE];
	int len, i;

	/* Read orig_name. */
	len = fd_read_line(fd, buf, SIZE);
	hd->orig_name = copy_buf(buf,len);

	/* Read seq_name. */
	len = fd_read_line(fd, buf, SIZE);
	hd->seq_name = copy_buf(buf,len);

	/* Read num_frame. */
	fd_read_line(fd, buf, SIZE);
	hd->num_frame = atoi(buf);

	/* Read orig_date. */
	len = fd_read_line(fd, buf, SIZE);
	hd->orig_date = copy_buf(buf,len);

	/* Read rows. */
	fd_read_line(fd, buf, SIZE);
	hd->rows = atoi(buf);

	/* Read cols. */
	fd_read_line(fd, buf, SIZE);
	hd->cols = atoi(buf);

	/* Skip then Region of Interest bits */
	for(i=0;i<4;i++)
		fd_read_line(fd, buf, SIZE);

	/* Read pixel_format. */
	fd_read_line(fd, buf, SIZE);
	hd->pixel_format = atoi(buf);

	/* Skip colours */
	fd_read_line(fd, buf, SIZE);

	/* Read sequence history */
	fd_read_line(fd, buf, SIZE);
	len = atoi(buf);

	if((hd->seq_history = (char *)calloc(len+1,1)) == 0)
		hperr("not enough memory for image history");

	if (hpread(fd,hd->seq_history,len) != len)return;
		/* hperr("error reading header"); */

	/* Read sequence description */
	fd_read_line(fd, buf, SIZE);
	len = atoi(buf);

	if((hd->seq_desc = (char *)calloc(len+1,1)) == 0)
		hperr("not enough memory for image history");

	if (hpread(fd,hd->seq_desc,len) != len)return;
		/* hperr("error reading header"); */

	/* Skip rest of header */
	fd_read_line(fd, buf, SIZE);
	len = atoi(buf);

	for (i=0;i<len;i++)
		fd_read_line(fd, buf, SIZE);

	fd_read_line(fd, buf, SIZE);
	len = atoi(buf);

	if (len)
	{
		char	*tmp;

		if ((tmp = (char *) malloc(len)) == 0)return;
			/* hperr("not enough memory for image header"); */

		if (hpread(fd,tmp,len) != len)return;
			/* hperr("error reading header"); */

		free(tmp);
	}
}
static void
h2clean_header(hd)
struct	header *hd;
{

	char	*tmp;

	/* Clean up sequence history */
	tmp = hd->seq_history;

	hd->seq_history = h2clean_hist(tmp);

	free(tmp);

	/* Set the set bits_per_pixel for neatness */
	switch (hd->pixel_format)
	{
		case PFBYTE:    hd->bits_per_pixel = 8*sizeof(char);
				break;
		case PFSHORT:   hd->bits_per_pixel = 8*sizeof(short);
				break;
		case PFINT:     hd->bits_per_pixel = 8*sizeof(long);
				break;
		case PFFLOAT:   hd->bits_per_pixel = 8*sizeof(float);
				break;
		case PFCOMPLEX: hd->bits_per_pixel = 16*sizeof(float);
				break;
		default:	hd->bits_per_pixel = 0;
				break;
	}

	/* Sort out fields for binary images */
	if(hd->pixel_format == PFMSBF)
	{
		hd->pixel_format = PFBYTE;
		hd->bit_packing = MSBFIRST;
		hd->bits_per_pixel = 1;
	}
	else if (hd->pixel_format == PFLSBF)
	{
		hd->pixel_format = PFBYTE;
		hd->bit_packing = LSBFIRST;
		hd->bits_per_pixel = 1;
	}
	else
		hd->bit_packing = 0;
}

/*
**	h2sgetline() : break up a string by setting nl to null
*/

static char *
h2sgetline(s,e)
char *s, *e;
{
	while(s < e)
	{
		if(*s == '\n')
		{
			*s = '\0';
			return(s);
		}
		s++;
	}

	return(0);
}

/*
**	h2stripspace() : strip out any leading space characters in a line
*/
static char *
h2stripspace(s,e)
char *s, *e;  
{
	char *save;

	save = s;

	while(s < e)
	{
		if(*s != ' ')
			return(s);

		s++;
	}

	/* Shouldn't get here, but just in case return start of line */

	return(save);
}

#define CONTL "|\\"
#define SCONTL " |\\"

/*
**	h2clean_hist() : clean up sequence history
**
**	HIPS-1 is very fussy about the layout of the sequence history,
**	this routine removes the HIPS-2 specific bits.
*/

static char *
h2clean_hist(buf)
char	*buf;
{
	int	len;
	char	*nl, *bp, *c, *end;
	char	*hist;

	len = strlen(buf);

	if((hist = (char *)calloc(len+SIZE, sizeof(char))) == 0)
		hperr("not enough memory for image history");

	end = buf + len;

	bp = buf;

	while ((nl = h2sgetline(bp,end)) != 0)
	{

		/* Ignore any "combined history" lines */

		if(strncmp("****",bp,4))
		{

			/* Strip out any leading spaces */

			bp = h2stripspace(bp,nl);
			strcat(hist,bp);
			c = hist + strlen(hist) - 2;

			/* Check line ends with correct continuation */

			if (strncmp(CONTL,c--,2) != 0)
			{
				if(*c == ' ')
					strcat(hist,CONTL);
				else
					strcat(hist,SCONTL);
			}
			else
			{
				if(*c != ' ')
					strcpy(c,SCONTL);
			}

			strcat(hist,"\n");
		}

		bp = nl + 1;
	}

	len = strlen(hist);

	strcpy((hist + len - 3),"\n");
	return(hist);
}

static void
hperr(a)
char	*a;
{
	fprintf(stderr,"%s\n",a);
	exit(1);
}

