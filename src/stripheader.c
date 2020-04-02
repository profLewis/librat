/* new 'PD' version by Lewis
*/
#include <stdio.h>
#include "hipl_format.h"
#include <stdlib.h>
void fp_fread_header();
#include <string.h>
void perr();

int main(argc,argv)
int	argc;
char	**argv;
{

	struct header hd;
	FILE *fpIn=stdin,*fpOut=stdout;
	char	*opFilename=NULL;

	int c,nrc,pixlen,pixelFormat,i,j;
	char *pic;

	if(argc>=2){	/* ip filename */
		if(!(fpIn=(FILE *)fopen(argv[1],"r")))
			perr("error opening input file",argv[1]);
		if(argc==3){	/* op filename */
			opFilename=argv[2];
		}else{
/* if ip filename has .hips ending, change to .dat & use this */
			i=strlen(argv[1])-5;
			opFilename=&(argv[1][i]);
			if(!strcmp(opFilename,".hips")){
				opFilename=(char *)strdup(argv[1]);
				opFilename[i+1]='d';
				opFilename[i+2]='a';
				opFilename[i+3]='t';
				opFilename[i+4]='\0';
			}else
				opFilename=NULL;
		}
		if(opFilename)if(!(fpOut=(FILE *)fopen(opFilename,"w")))
			perr("error opening output file",opFilename);
	}
	fp_fread_header(fpIn,&hd);

	pixelFormat = hd.pixel_format;

/* adapted from code of Landy and Pearson */

	if (pixelFormat==PFBYTE || pixelFormat==PFSHORT || pixelFormat==PFINT || pixelFormat==PFFLOAT || pixelFormat==PFCOMPLEX) {
		nrc = hd.cols;
		if (pixelFormat==PFBYTE) {
			pixlen = sizeof(char);
			if (hd.bit_packing)
				nrc = (hd.cols+7)/8;
		}else if (pixelFormat==PFSHORT)
			pixlen = sizeof(short int);
		else if (pixelFormat==PFINT)
			pixlen = sizeof(int);
		else if (pixelFormat==PFFLOAT)
			pixlen = sizeof(float);
		else if (pixelFormat==PFCOMPLEX) {
			pixlen = sizeof(float);
			nrc *= 2;
		}
		if ((pic = (char *) malloc(nrc*pixlen)) == 0)
			perr(argv[0],"can't allocate core");

		for (i=0;i<hd.num_frame*hd.rows;i++) {
			if (fread(pic,pixlen,nrc,fpIn)!=nrc*pixlen)
				perr(argv[0],"error during read");
			if (fwrite(pic,pixlen,nrc,fpOut)!=nrc)
				perr(argv[0],"error during write");
		}
	}
	else {
		while ((c=getchar()) != EOF) putchar(c);
	}
	exit(0);
}
void perr(a,b)
char	*a, *b;
{
	fprintf(stderr,"%s: %s\n",a,b);
	exit(1);
}
