#include <stdio.h>
#include "hipl_format.h"
#include "imagelib.h"
#include "filelib.h"
#include <stdlib.h>
#include "libhipl.h"

#define MAX_SIZE 2048

/*
**	update HIPS header
**	i.e. store info. on current process in image header
**	if this type of storage is supported
*/
int updateHipsHeader(GenericImage *ImagePtr,int argc,char **argv)
{
	struct header	hd;
/*
**      update header information with current command-line
*/
	if(!ImagePtr->orig_name)if(!(ImagePtr->orig_name=(char *)calloc(MAX_SIZE,(int)sizeof(char)))){fprintf(stderr,"error in core allocation:\tattempting to allocate %d elements of size %d",MAX_SIZE,(int)sizeof(char));exit(1);}
	if(!ImagePtr->seq_name)if(!(ImagePtr->seq_name=(char *)calloc(MAX_SIZE,(int)sizeof(char)))){fprintf(stderr,"error in core allocation:\tattempting to allocate %d elements of size %d",MAX_SIZE,(int)sizeof(char));exit(1);}
	if(!ImagePtr->orig_date)if(!(ImagePtr->orig_date=(char *)calloc(MAX_SIZE,(int)sizeof(char)))){fprintf(stderr,"error in core allocation:\tattempting to allocate %d elements of size %d",MAX_SIZE,(int)sizeof(char));exit(1);}
	if(!ImagePtr->seq_history)if(!(ImagePtr->seq_history=(char *)calloc(MAX_SIZE,(int)sizeof(char)))){fprintf(stderr,"error in core allocation:\tattempting to allocate %d elements of size %d",MAX_SIZE,(int)sizeof(char));exit(1);}
	if(!ImagePtr->seq_desc)if(!(ImagePtr->seq_desc=(char *)calloc(MAX_SIZE,(int)sizeof(char)))){fprintf(stderr,"error in core allocation:\tattempting to allocate %d elements of size %d",MAX_SIZE,(int)sizeof(char));exit(1);}

	hd.orig_name=ImagePtr->orig_name;
	hd.seq_name=ImagePtr->seq_name;
	hd.orig_date=ImagePtr->orig_date;
	hd.seq_history=ImagePtr->seq_history;
	hd.seq_desc=ImagePtr->seq_desc;

        update_header(&hd,argc,argv);

	ImagePtr->orig_name=hd.orig_name;
	ImagePtr->seq_name=hd.seq_name;
	ImagePtr->orig_date=hd.orig_date;
	ImagePtr->seq_history=hd.seq_history;
	ImagePtr->seq_desc=hd.seq_desc;


	return(1);
}

int writeHipsHeader(GenericImage *ImagePtr)
{
	struct header	hd;
	static char tmp[] = "";

	if(!ImagePtr->seq_history)ImagePtr->seq_history=&tmp[0];
	if(!ImagePtr->seq_desc)ImagePtr->seq_desc=&tmp[0];
	if(!ImagePtr->seq_name)ImagePtr->seq_name=&tmp[0];
	if(!ImagePtr->orig_date)ImagePtr->orig_date=&tmp[0];
	if(!ImagePtr->orig_name)ImagePtr->orig_name=&tmp[0];
/* header translator */
	hd.num_frame=ImagePtr->frames;
	hd.rows=ImagePtr->rows;
	hd.cols=ImagePtr->cols;
	hd.orig_date=ImagePtr->orig_date;
	hd.orig_name=ImagePtr->orig_name;
	hd.seq_name=ImagePtr->seq_name;
	hd.bits_per_pixel=ImagePtr->bits_per_pixel;
	hd.bit_packing=ImagePtr->bit_packing;
	hd.pixel_format=ImagePtr->format;
	hd.seq_history=ImagePtr->seq_history;
	hd.seq_desc=ImagePtr->seq_desc;
        fp_fwrite_header(ImagePtr->stream,&(hd));

	return(1);
}

/*
**     read image header
*/
int readHipsHeader(GenericImage *ImagePtr)
{
	struct header hd;

	fp_fread_header(ImagePtr->stream,&hd);

/* header translator */
	ImagePtr->frames=hd.num_frame;
	ImagePtr->rows=hd.rows;
	ImagePtr->cols=hd.cols;
	ImagePtr->orig_date=hd.orig_date;
	ImagePtr->orig_name=hd.orig_name;
	ImagePtr->seq_name=hd.seq_name;
	ImagePtr->bits_per_pixel=hd.bits_per_pixel;
	ImagePtr->bit_packing=hd.bit_packing;
	ImagePtr->format=hd.pixel_format;
	ImagePtr->seq_history=hd.seq_history;
	ImagePtr->seq_desc=hd.seq_desc;

	return(1);
}

int	arrayIndexHips(GenericImage *ImagePtr,int frame,int row,int col)
{
	
/* straightforward BSQ access (no BIL mode) */
	switch(ImagePtr->arrayAccess) {
		case BIL:
		return ((ImagePtr->rows * ImagePtr->frames * col + ImagePtr->frames * row + frame));
		break;
		default:
        return((ImagePtr->rows*ImagePtr->cols*frame + ImagePtr->cols*row + col));
	}
}

/*
**	check to see if file is hips format
*/
int	checkHips(GenericImage *ImagePtr,char *env)
{
	int	i,openFlag;
	char	*extension,*name,c;
	char HEAD[10];
	FILE	*stream;

	strcpy(HEAD,"HIPS ");
	name=getImageName(ImagePtr);

/*
**	first check filename extension (hips/HIPS)
*/
	extension=getFilenameExtension(name);
	if(extension && (strcmp(extension,"hips")==0 || strcmp(extension,"HIPS")==0) )return(TRUE);
/*
**	check for "H   I   P   S  sp  nl"
**	if can rewind the file, else, check
**	for H as first byte
*/
	if(!(stream=getStream(ImagePtr))){
		setImageName(ImagePtr,"-");
		getStream(ImagePtr);
	}
	openFlag=ImagePtr->openFlag;

	if(!openFlag)openImage(ImagePtr,TRUE,env);
	if(strcmp(name,"-")==0){	/* stdin/stdout */
		if((c=fgetc(stream))!=EOF){
			if(c==HEAD[0]){	/* probably */
				fputc(c,stream);
				return(TRUE);
			}
		}
	}else{
		for(i=0;i<(int)strlen(HEAD);i++){
			if((char)fgetc(stream)!=HEAD[i]){
				rewind(stream);
				if(!openFlag)fclose(stream);
				return(FALSE);
			}
		}
		if(!openFlag)fclose(stream);
		return(TRUE);
	}
	if(!openFlag)fclose(stream);
	return(FALSE);

}

/*
**	set HIPS header functions into generic image format
*/
/* return TRUE if detected image is Hips format or if !check */
int	isHips(GenericImage *ImagePtr,int check,char *env)
{
	int	i,retval=TRUE;
	if(check)retval=checkHips(ImagePtr,env);
	ImagePtr->read_header=readHipsHeader;
	ImagePtr->write_header=(void *)writeHipsHeader;
	ImagePtr->update_header=updateHipsHeader;
	ImagePtr->index=arrayIndexHips;
	for(i=0;i<=IMAGE_FLOAT;i++)ImagePtr->supported[i]=TRUE;
	return(retval);
}
