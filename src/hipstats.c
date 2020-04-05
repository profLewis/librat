#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "hipl_format.h"
#ifndef MAXFLOAT 
#include <values.h>
#endif
/* O/P to stdout rather than stderr changed by Lewis July 22 1999 */
#ifndef _stderr_
#define OPCHANNEL stdout
#else
#define OPCHANNEL stderr
#endif /* OPCHANNEL */

#define MAXLEN 256
#define BUFSIZE 1024
#define MAX_NUM_LEN 20
#ifndef NULL
#define NULL 0
#endif
#ifndef MAX
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#endif
#ifndef MIN
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#endif
#define TOL 1e-6
#define TOL_BYTE 1
#define TOL_SHORT 1e-2
#define TOL_FLOAT 1e-4
#define TOL_DOUBLE 1e-8

typedef struct{
  float exclude_val;
  int exclude_flag;
}Exclude;

main(argc,argv)
int argc;
char **argv;
{
	char in_file[MAXLEN];
	int rows=288,cols=384,frames=1,i,j,mmapFlag=0,expFlag=0;
	/* expFlag: do we print out in exponent form or just %.3f decimal places */
	float excludeVal=0;
	FILE *in_fp;
	/*	char *in_buf=NULL; */
	void *in_buf=NULL;
	int (*doTheData)(), *getTheRightFunc(), doTheDataBYTE(),  doTheDataINT(), doTheDataSHORT(), doTheDataUSHORT(), doTheDataFLOAT(), doTheDataASCII(), nBytes=4;
	struct header *in_header;
	Exclude *exclude;

	if(!(in_header=(struct header *)calloc(1,sizeof(struct header)))){
	  fprintf(stderr,"%s: error allocating core for input header\n",argv[0]);
	  exit(1);
	}

	if(!(exclude=(Exclude *)calloc(1,sizeof(Exclude)))){
	  fprintf(stderr,"%s: error allocating core for struct exclude\n",argv[0]);
	  exit(1);
	}

	exclude->exclude_val=0.;
	exclude->exclude_flag=0;
	
	/* parser - stdin by default */

	strcpy(in_file,"-");

	if(argc!=1&&(*argv[argc-1]!='-'&&strncasecmp(argv[argc-2],"-x",2))){
	  strcpy(in_file,argv[argc-1]);
	}

	/* if arg. doesn't begin with a leading `-' then assume it's in_file */
	for(i=1;i<argc;i++){
	  if(*argv[i]=='-'){
	    if(!strncasecmp(argv[i],"-x",2)){
	      exclude->exclude_val=atof(argv[++i]);
	      exclude->exclude_flag=1;
	    }else if(!strncasecmp(argv[i],"-u",2)||!strncasecmp(argv[i],"-h",2)){ fprintf(stderr,"usage: %s [-x n] hips_image\n",argv[0]);exit(0);}
	    else if(!strncasecmp(argv[i],"-m",2)) mmapFlag=1;
	    else if(!strncmp(argv[i],"--",2)){
	      /* file has two leading `-' chars, so assumed filename is "-blah" */
	      strcpy(in_file,(argv[i]+1));
	    }
	    else if(!strncasecmp(argv[i],"-e",2)||!strncasecmp(argv[i],"-z",2)) expFlag=1;
	    else{
	      fprintf(stderr,"%s: unknown argument: %s\n",argv[0],argv[i]);
	      exit(1);
	    }
	  }else strcpy(in_file,argv[i]);
	}
	
	/* 

	if(argc==2){ must be progname and filename (not stdin)
	  strcpy(in_file,argv[argc-1]);
	}else if(argc==4){  must be progname, filename and -x 0 option
	  strcpy(in_file,argv[argc-1]);
	  exclude->exclude_val=atof(argv[argc-2]);
	  exclude->exclude_flag=1;
	}else{
	  for(i=1;i<argc;i++){
	    if(*argv[i]=='-'){
	      if(!strncasecmp(argv[i],"-x",2)){
		exclude->exclude_val=atof(argv[++i]);
		exclude->exclude_flag=1;
	      }else if(!strncasecmp(argv[i],"-u",2)||!strncasecmp(argv[i],"-h",2)){ fprintf(stderr,"usage: %s [-x n] hips_image\n",argv[0]);exit(0);}
	      else if(!strncasecmp(argv[i],"-m",2)) mmapFlag=1;
	      else if(!strncasecmp(argv[i],"-e",2)||!strncasecmp(argv[i],"-z",2)) expFlag=1;
	      else{
		fprintf(stderr,"%s: unknown argument: %s\n",argv[0],argv[i]);
		exit(1);
	      }
	    }
	  }
	}
	
	*/

	if(mmapFlag&&!strcmp(in_file,"-")){
	    fprintf(stderr,"%s: can't mmap on stdin\n",argv[0]);
	    exit(1);
	}
    
	if(strcmp(in_file,"-")){
	  if(!(in_fp=fopen(in_file,"r"))){
	    fprintf(stderr,"Couldn't open file %s\n",in_file);
	    exit(1);
	  }
	}else in_fp=stdin;
	
	fp_fread_header(in_fp, in_header);

	switch(in_header->pixel_format){
	case PFBYTE:
	case PFASCII:
	  nBytes=1;
	  break;
	case PFSHORT:
	  nBytes=2;
	  break;
	case PFINT:
	case PFFLOAT:
	  nBytes=4;
	  break;
	default:
	  fprintf(stderr,"%s: can't deal with format code %i\n",argv[0],in_header->pixel_format);
	  exit(1);
	}

	
	if(!(in_buf=(void *)calloc(in_header->rows*in_header->cols*nBytes,sizeof(char)))){
	  fprintf(stderr,"%s:\t error in core allocation for in_buf, frame %i\n",argv[0],i);
	  exit(1);
	}
	
	
	for(i=0;i<in_header->num_frame;i++){
	  if(in_header->pixel_format==PFASCII){
	    if(doTheDataASCII(in_header,in_fp,i,exclude)){
	      fprintf(stderr,"%s: error in function doTheDataASCII\n",argv[0]);
	      exit(1);
	    }
	  }else{	    
	    if(fread((void *)in_buf,sizeof(char),in_header->rows*in_header->cols*nBytes,in_fp)!=in_header->rows*in_header->cols*nBytes){
	      fprintf(stderr,"%s: Error during read of file %s, frame %i\n",argv[0],in_file,i);
	      exit(1);
	    }
	    
	    switch(in_header->pixel_format){
	    case PFBYTE:
	      doTheData=doTheDataBYTE;
	      break;
	    case PFSHORT:
	      doTheData=doTheDataSHORT;
	      break;
	    case PFINT:
	      doTheData=doTheDataINT;
	      break;
	    case PFFLOAT:
	      doTheData=doTheDataFLOAT;
	      break;
	    default:
	      fprintf(stderr,"%s: can't deal with format code %i\n",argv[0],in_header->pixel_format);
	      exit(1);
	    }
	    
	    doTheData(in_header,in_buf,exclude,i,expFlag);
	  }

	}
	fclose(in_fp);
	return(0);
}

int doTheDataBYTE(h,image,x,k,expFlag)
struct header *h;
Exclude *x;
char *image;
int k; /* frame no. */
int expFlag; 
{
  register i=0,j=0,n=0;
  double mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  double min=UCHAR_MAX, max=-UCHAR_MAX;

  
  for(i=0;i<h->rows;i++){
    for(j=0;j<h->cols;j++){
      pixVal= (double) *((unsigned char *)image + i*(h->cols) + j);
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	sqrTot += (pixVal*pixVal);
	tot += pixVal;
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
	min=max=0.;
  }
  if(!expFlag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%d Max=%d\n",k,n,mean,sd,(int)min,(int)max);
  else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%d Max=%d\n",k,n,mean,sd,(int)min,(int)max);
  return(0);
}

int doTheDataFLOAT(h,image,x,k,expFlag)
     struct header *h;
     Exclude *x;
     float *image;
     int k; /* frame no. */
     int expFlag; 
{
  register i=0,j=0,n=0;
  double mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  double min=MAXFLOAT, max=-MAXFLOAT;

  for(i=0;i<h->rows;i++){
    for(j=0;j<h->cols;j++){
      pixVal= (double) *((float *)image + i*(h->cols) + j);
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	tot += pixVal;
	sqrTot += (pixVal*pixVal);
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
    min=max=0.;
  }  
  if(!expFlag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%.3f Max=%.3f\n",k,n,mean,sd,min,max);
  else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%e Max=%e\n",k,n,mean,sd,min,max);
  return;
}

int doTheDataSHORT(h,image,x,k,expFlag)
     struct header *h;
     Exclude *x;
     short *image;
     int k; /* frame no. */
     int expFlag; 
{
  register i=0,j=0,n=0;
  double mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  double min=SHRT_MAX, max=SHRT_MIN;

 
  for(i=0;i<h->rows;i++){
    for(j=0;j<h->cols;j++){
      pixVal = (double) *((short *)image + i*(h->cols) + j);
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	tot += pixVal;
	sqrTot += (pixVal*pixVal);
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
	min=max=0.;
  }  
  if(!expFlag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%i Max=%i\n",k,n,mean,sd,(int)min,(int)max);
  else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%d Max=%d\n",k,n,mean,sd,(int)min,(int)max);
  return(0);
}

int doTheDataUSHORT(h,image,x,k,expFlag)
     struct header *h;
     Exclude *x;
     unsigned short *image;
     int k; /* frame no. */
     int expFlag; 
{
  register i=0,j=0,n=0;
  double mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  double min=USHRT_MAX, max=0;

 
  for(i=0;i<h->rows;i++){
    for(j=0;j<h->cols;j++){
      pixVal = (double) *((unsigned short *)image + i*(h->cols) + j);
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	tot += pixVal;
	sqrTot += (pixVal*pixVal);
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
    min=max=0.;
  }
  if(!expFlag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%i Max=%i\n",k,n,mean,sd,(int)min,(int)max);
  else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%d Max=%d\n",k,n,mean,sd,(int)min,(int)max);
  return(0);
}

int doTheDataINT(h,image,x,k,expFlag)
     struct header *h;
     Exclude *x;
     int *image;
     int k; /* frame no. */
     int expFlag; 
{
  register i=0,j=0,n=0;
  float mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  float min=MAXFLOAT, max=-MAXFLOAT;
  
  
  for(i=0;i<h->rows;i++){
    for(j=0;j<h->cols;j++){
      pixVal= (float) *((int *)image + i*(h->cols) + j);
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	tot += pixVal;
	sqrTot += (pixVal*pixVal);
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
    min=max=0.;
  }
  if(!expFlag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%i Max=%i\n",k,n,mean,sd,(int)min,(int)max);
  else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%e Max=%e\n",k,n,mean,sd,(int)min,(int)max);
  return(0);
}

int doTheDataASCII(h,fp,k,x)
     struct header *h;
     FILE *fp;
     int k; /* frame no. */
     Exclude *x;
{
  register i=0,j=0,l=0,n=0;
  int nLines=0;
  double mean=0., sd=0., tot=0., sqrTot=0., pixVal=0.;
  double min=MAXFLOAT, max=-MAXFLOAT;
  unsigned char *line=NULL, *liney=NULL;
  static int num=0;

  if(!(line=(unsigned char *)calloc(h->cols*MAX_NUM_LEN,sizeof(unsigned char)))){
    fprintf(stderr,"error allocating core in function doTheDataASCII\n");
    return(1);
  }

  /*
** temporary copy of string
*/
 if(!(liney=(unsigned char *)calloc(h->cols*MAX_NUM_LEN,sizeof(unsigned char)))){
    fprintf(stderr,"error allocating core in function doTheDataASCII\n");
    return(1);
  }


l=0;
while(l<h->rows){
    fgets((unsigned char *)line,(h->cols*MAX_NUM_LEN),fp); /* *must* guarantee we get a whole line here - assuming MAX_NUM_LEN characters per pixel */
    liney=line;
    for(j=1;j<=h->cols;j++){
      if(j==1)
	pixVal = (double)atof(strtok(liney," "));
      else
	pixVal = (double)atof(strtok(NULL," "));
      if(!x->exclude_flag || (x->exclude_flag && pixVal !=x->exclude_val)){
	n++;
	tot += pixVal;
	sqrTot += (pixVal*pixVal);
	max=MAX(pixVal,max);
	min=MIN(pixVal,min);
      }
    }
    /* fgets(line,50,fp); get rid of new line chars */
    l++;
  }
  /* calc mean and ssd, then sd */
  if(n!=0){
    mean=tot/n;
    if((sqrTot-n*(mean*mean))<0||(sqrTot-n*(mean*mean))/(sqrTot+n*(mean*mean))<TOL){
      sd=0.;
    }else{
      sd=sqrt((sqrTot-n*(mean*mean))/(n-1));
    }
  }else{
    mean=0.;
    sd=0.;
 	min=max=0.;
  }
  /*
    if(!x->exclude_flag)
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%d Max=%d\n",k,n,mean,sd,min,max);
    else
    fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%e SD=%e Min=%e Max=%e\n",k,n,mean,sd,min,max);
  */
  
  fprintf(OPCHANNEL,"Frame\t%i : n=%i mean=%.3f SD=%.3f Min=%.3f Max=%.3f\n",k,n,mean,sd,min,max);

  return(0);
}
