#include <stdio.h>
#include <math.h>
#include "hipl_format.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
void fp_fread_header();
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAXLEN 256
#define BUFSIZE 1024
#ifndef NULL
#define NULL 0
#endif
void init_header(),update_header(),fwrite_header();
/* simple bit of code to byte swap */

int main(argc,argv)
int argc;
char **argv;
{
	char in_file[MAXLEN],out_file[MAXLEN],date_string[MAXLEN];
	int i=0,j=0,rows=512,cols=512,frames=0,total=0,swapFlag=1; /* swapFlag switched on for byte swapping */
	int in_fd,out_fd; /* file descriptors */
	float *in_buf,*temp_buf;
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
		if(!(in_fd=open(in_file,O_RDONLY))){
			fprintf(stderr,"Couldn't open file %s\n",in_file);
			exit(1);
		}
	}else in_fd=0;
	
	if(strcmp(out_file,"-")){
		if(!(out_fd=open(out_file,O_WRONLY))){
			fprintf(stderr,"Couldn't open file %s\n",in_file);
			exit(1);
		}
	}else out_fd=1;
	
	/* initialise output header structure, and write it out */


	init_header(&out_header, in_file, "HIPS\n", 1, "", rows, cols, sizeof(float), 0, PFFLOAT, ""); 
	update_header(&out_header,argc,argv);
	fwrite_header(out_fd,&out_header);

	if(!(in_buf=(float *)calloc(rows*cols,sizeof(float)))){
		fprintf(stderr,"%s:\t error in core allocation for image\n",argv[0]);
		exit(1);
        }

	/* read first four bytes (Fortran header) from file, and discard */
 
	if((int)read(in_fd,(float *)in_buf,sizeof(float)*rows*cols)!=rows*cols){

		fprintf(stderr,"%s: Error during read of file %s\n",argv[0],in_file);
		exit(1);
	}


	/* if swapFlag is set, allocate temp buffer, and swap bytes */

	if(swapFlag){
		if(!(temp_buf=(float *)calloc(rows*cols,sizeof(float)))){
			fprintf(stderr,"%s:\t error in core allocation for byte swapping\n",argv[0]);
			exit(1);
		}
		swab(in_buf,temp_buf,2);
		in_buf=temp_buf;
	}


	/* write out data */
	if(write(out_fd,in_buf,sizeof(float)*rows*cols)!=rows*cols){
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
