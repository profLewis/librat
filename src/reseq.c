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
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

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
	int	f,r,c,rc,i;		/* frames, rows and columns */
	int	size, type;		/* size and type of input data */
	struct header hd,ohd;		/* image header */
	int	unflag = 0,nBands=1,atoi();
	double	atof();
	char	*prog;
        int *bands=NULL;

	prog = get_prog(argv[0]);
        nBands=argc;
        if(!(bands=(int *)calloc(nBands,sizeof(int)))){
            fprintf(stderr,"error in core allocation\n");
            exit(1);
        }
        for(i=0;i<argc;i++)bands[i]=atoi(argv[i]);
	read_header(&hd);
        ohd=hd;
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
        ohd.num_frame = nBands;
	if ((ifr=(unsigned char *)malloc(rc*size*hd.num_frame)) == 0) 
		perr(prog,"not enough memory");

        if ((ofr=(unsigned char *)malloc(rc*size)) == 0)
                perr(prog,"not enough memory");
/*
**	Update header and write it out
*/
	hd.bits_per_pixel = 8;
	update_header(&ohd,argc,argv);
	write_header(&ohd);
/*
*/
	if (read(0,ifr,hd.num_frame*rc*size) != rc*size*hd.num_frame)
       		perr(prog,"error during read");

	for (f=0;f<ohd.num_frame;f++) {
	  i = bands[f];
          for(r=0;r<rc*size;r++){
	    ofr[r] = ifr[r+i*rc*size]; 
	  }
          if (write(1,ofr,rc*size) != rc*size)
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
perr(s)
char *s;
{
	fprintf(stderr,"linear: %s\n",s);
	exit(1);
}
*/
