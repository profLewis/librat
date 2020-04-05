#include <stdio.h>
#include <math.h>
#include "hipl_format.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 256
#define BUFSIZE 1024
#ifndef NULL
#define NULL 0
#endif
void fp_fread_header();

int main(argc,argv)
int argc;
char **argv;
{
	char in_file[MAXLEN],out_file[MAXLEN],date_string[MAXLEN];
	int i=0,j=0,k=0,rows=512,cols=512,frames=0,total=0,swapFlag=1; /* swapFlag switched on for byte swapping */
	FILE *in_fd,*out_fd; /* file descriptors */
	char *in_buf,*temp_buf;
	struct header out_header;
	void usage();

	/* parser - stdout by default */

	strcpy(in_file,"-");
	strcpy(out_file,"-");

	for(i=1;i<argc;i++){
                if(*argv[i]=='-'){
                        if(!strcmp(argv[i],"-i")||!strcmp(argv[i],"-input"))strcpy(in_file,argv[++i]);
                        else if(!strcmp(argv[i],"-o")||!strcmp(argv[i],"-output")){
                                strcpy(out_file,argv[++i]);
                        }else{
                                fprintf(stderr,"%s: unknown argument: %s\n",argv[0],argv[i]);
                                usage(argv);
                                exit(1);
                        }
                }
        }
	if(strcmp(in_file,"-")){
		if(!(in_fd=fopen(in_file,"r"))){
			fprintf(stderr,"Couldn't open file %s\n",in_file);
			exit(1);
		}
	}else in_fd=stdin;
	
	if(strcmp(out_file,"-")){
		if(!(out_fd=fopen(out_file,"w"))){
			fprintf(stderr,"Couldn't open file %s\n",in_file);
			exit(1);
		}
	}else out_fd=stdout;
	
	fp_fread_header(in_fd,&out_header);
	if(out_header.pixel_format!=PFBYTE){
		fprintf(stderr,"only byte data for %s\n",argv[0]);
		exit(1);
	}

	if(out_header.num_frame>=3){
		out_header.num_frame=3;
		fprintf(out_fd,"P6\n# hips2pbm - 3 frame colour\n%d %d\n255\n",out_header.cols,out_header.rows);
	}else{
		out_header.num_frame=1;
		fprintf(out_fd,"P5\n# hips2pbm - 1 frame greyscale\n%d %d\n255\n",out_header.cols,out_header.rows);
	}

	rows=out_header.rows;
	cols=out_header.cols;
	frames=out_header.num_frame;

	if(!(in_buf=(char *)calloc(rows*cols*frames,sizeof(char)))){
		fprintf(stderr,"%s:\t error in core allocation for image\n",argv[0]);
		exit(1);
        }
 
	if(fread((char *)in_buf,sizeof(char),rows*cols*frames,in_fd)!=rows*cols*frames){
		fprintf(stderr,"%s: Error during read of file %s\n",argv[0],in_file);
		exit(1);
	}

	if(frames>1){ /* bsq -> bil */
		if(!(temp_buf=(char *)calloc(rows*cols*frames,sizeof(char)))){
			fprintf(stderr,"%s:\t error in core allocation for image\n",argv[0]);
			exit(1);
       	}
		for(i=0;i<rows;i++)
		for(j=0;j<cols;j++)
		for(k=0;k<frames;k++)
			*(temp_buf + i*cols*frames+j*frames+k)=*(in_buf + k*rows*cols+i*cols+j);
		in_buf=temp_buf;
		
		
	}

	/* write out data */
	if(fwrite(in_buf,sizeof(char),rows*cols*frames,out_fd)!=rows*cols*frames){
		fprintf(stderr,"%s: Error during write of file %s\n",argv[0],out_file);
		exit(1);	
	}

	/* tidy up */

	free(in_buf);
	return(0);
}

void usage(argv)
char **argv;
{
	fprintf(stderr,"usage: %s -i input_image -o output_image -s scale_factor (1/10000 by default)\n",argv[0]);
	fprintf(stderr,"	  -r rows -c cols (rows=288, cols=384 by default) -bs\n");
	return;
}
