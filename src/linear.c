/*
**	linear : linear contrast enhancement
**
**	usage : linear < HIPSimage > HIPSbyte_image
**
**	Scales double, float, integer, short or byte images to the range 0-255
**
** 	James Pearson UCL 3/11/87
**	Added option for unsigned input JCP 17/11/90
**	Added PFDOUBLE JCP UCL P&S 6/2/95
*/
#define HIPS_IMAGE

#include <hipl_format.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
void fp_fread_header();
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void read_header(),perr(),fread_header(),update_header(),write_header();
void lindl(),linfl(),linin(),linby(),linush(),linsh(),linuin();

#define MAX_FLOAT 3.4028234e38
#define MAX_INT   0x7fffffff
#define MAX_UINT   0xffffffff
#define MAX_SHORT 0x7fff
#define MAX_USHORT 0xffff
#define MAX_BYTE  0xff
#define max(A,B) ((A)>(B)) ? A:B
#define min(A,B) ((A)<(B)) ? A:B
#define byte(A)	 max(0,min(255,A))

double	range = (double)MAX_BYTE;	/* range of values */
double	u_ma, u_mi;			/* override min/max values */
int	u_flag = 0, zflag = 0;
double	zero;
char	*get_prog();

int main(argc,argv)
int argc;
char *argv[];
{
	unsigned char	*ifr,*ofr;	/* input and output images */
	int	f,r,c,rc;		/* frames, rows and columns */
	int	size, type;		/* size and type of input data */
	struct header hd;		/* image header */
	int	unflag = 0;
	double	atof();
	char	*prog;

	prog = get_prog(argv[0]);

/*	This needs to be cleaned up a bit */
	switch (argc)
	{
		case 4:	if(strcmp("-u",argv[3]))
				perr(prog,"unknown option %s",argv[3]);
			unflag = 1;
		case 3: if(!strcmp("-z",argv[1]))
			{
				zero = atof(argv[2]);
				zflag = 1;
			}
			else
			{
				u_ma = atof(argv[2]);
				u_mi = atof(argv[1]);
				u_flag = 1;
			}
			break;
		case 2: if(strcmp("-u",argv[1]))
				perr(prog,"unknown option %s",argv[1]);
			unflag = 1;
		case 1:	break;
		default:
			perr(prog,"incorrect number of parameters");
			break;
	}

	read_header(&hd);

	type = hd.pixel_format;
/*
**	set size of input data
*/
	switch(type)
	{
		case PFBYTE:	size = sizeof(char);
				if(hd.bit_packing)
				    perr(prog,"can't cope with binary images");
				break;
		case PFSHORT:	size = sizeof(short);
				break;
		case PFINT:	size = sizeof(int);
				break;
		case PFFLOAT:	size = sizeof(float);
				break;
#ifdef PFDOUBLE
		case PFDOUBLE:	size = sizeof(double);
				break;
#endif /* PFDOUBLE */
		default:	perr(prog,"unknown pixel format %d",type);
				break;
	}
/*
** 	Set up constants and input/output image storage
*/
	r=hd.rows;
	c=hd.cols;
	rc=r*c;

	if ((ifr=(unsigned char *)malloc(rc*size)) == 0) 
		perr(prog,"not enough memory");

	ofr = ifr;
/*
**	Update header and write it out
*/
	hd.pixel_format = PFBYTE;
	hd.bits_per_pixel = 8;
	update_header(&hd,argc,argv);
	write_header(&hd);
/*
**	Scale each frame independently
*/
	for (f=0;f<hd.num_frame;f++) {
		if ((int)read(0,ifr,rc*size) != rc*size)
			perr(prog,"error during read");
/*
**		Call the routine depending on data type
*/
		range = 255.0;

		switch(type)
		{
			case PFBYTE:	linby(ifr,ofr,rc);
					break;
			case PFSHORT:	if(unflag)
						linush((short *)ifr,ofr,rc);
					else
						linsh((short *)ifr,ofr,rc);
					break;
			case PFINT:	if(unflag)
						linuin((int *)ifr,ofr,rc);
					else
						linin((int *)ifr,ofr,rc);
					break;
			case PFFLOAT:	linfl((float *)ifr,ofr,rc);
					break;
#ifdef PFDOUBLE
			case PFDOUBLE:	lindl((double *)ifr,ofr,rc);
					break;
#endif /* PFDOUBLE */
		}
/*
**		Write out each frame
*/
		if (write(1,ofr,rc) != rc)
			perr(prog,"error during write");
	}
	exit(0);
}

/*
**	linfl() : scale float data
**
**	usage :	linfl(in,out,rc)
**		float   *in;
**		unsigned char *out;
**		int     rc;
*/
	
void linfl(in,out,rc)
float	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register float *inp;
	register unsigned char *outp;
	float	ma, mi;			/* max and min values */

	mi = MAX_FLOAT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = u_ma;
		mi = u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = byte((*inp-mi)*range);
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)((*inp-mi)*range);
}

#ifdef PFDOUBLE
/*
**	lindl() : scale double data
**
**	usage :	lindl(in,out,rc)
**		double   *in;
**		unsigned char *out;
**		int     rc;
*/
	
void lindl(in,out,rc)
double	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register double *inp;
	register unsigned char *outp;
	double	ma, mi;			/* max and min values */

	mi = MAX_FLOAT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = u_ma;
		mi = u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = byte((*inp-mi)*range);
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)((*inp-mi)*range);
}
#endif /* PFDOUBLE */

/*
**	linin() : scale integer data
**
**	usage :	linfl(in,out,rc)
**		int     *in;
**		unsigned char *out;
**		int     rc;
*/
	
void linin(in,out,rc)
int	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register int *inp;
	register unsigned char *outp;
	int	ma, mi;

	mi = MAX_INT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = (int)u_ma;
		mi = (int)u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == (int)zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (float)(ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)byte((((float)*inp-mi)*range));
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)(((float)*inp-mi)*range);
}

void linuin(in,out,rc)
unsigned int	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register unsigned int *inp;
	register unsigned char *outp;
/*	unsigned int	ma, mi; */
	int	ma, mi;

	mi = MAX_UINT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = u_ma;
		mi = u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == (unsigned int)zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (float)(ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)byte((((float)*inp-mi)*range));
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)(((float)*inp-mi)*range);
}

/*
**	linsh() : scale short data
**
**	usage :	linfl(in,out,rc)
**		short   *in;
**		unsigned char *out;
**		int     rc;
*/
	
void linsh(in,out,rc)
short	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register short *inp;
	register unsigned char *outp;
	short	mi, ma;

	mi = MAX_SHORT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = (short)u_ma;
		mi = (short)u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == (short)zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (float)(ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)byte((((float)*inp-mi)*range));
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)(((float)*inp-mi)*range);
}

void linush(in,out,rc)
unsigned short	*in;
unsigned char *out;
int	rc;
{
	register int i;
	register unsigned short *inp;
	register unsigned char *outp;
/*	unsigned short	mi, ma; */
	short	mi, ma;

	mi = MAX_USHORT;
	ma = -mi;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = u_ma;
		mi = u_mi;
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == (unsigned short)zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range /= (float)(ma-mi);

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)byte((((float)*inp-mi)*range));
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)(((float)*inp-mi)*range);
}

/*
**	linby() : scale byte data
**
**	usage :	linfl(in,out,rc)
**		unsigned char   *in;
**		unsigned char *out;
**		int     rc;
*/

void linby(in,out,rc)
unsigned char *in;
unsigned char *out;
int	rc;
{
	register int i;
	register unsigned char *inp;
	register unsigned char *outp;
/*	unsigned char ma, mi; */
	float ma, mi;

	mi = MAX_BYTE;
	ma = 0;

	inp = in;
	outp = out;

	if(u_flag)
	{
		ma = u_ma;
		mi = u_mi;
/*		ma = (unsigned char)u_ma;
		mi = (unsigned char)u_mi; */
	}
	else
	{
		for (i=0;i<rc;i++,inp++)
		{
			if(zflag && *inp == (unsigned char)zero)
				continue;
			ma = max(ma,*inp);
			mi = min(mi,*inp);
		}
	}

	range = range/((float)(ma-mi));

	inp = in;

	if(u_flag || zflag)
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)byte((((float)*inp-mi)*range));
	else
		for(i=0;i<rc;i++,inp++,outp++)
			*outp = (unsigned char)(((float)*inp-mi)*range);
}

/*
void perr(s)
char *s;
{
	fprintf(stderr,"linear: %s\n",s);
	exit(1);
}
*/
