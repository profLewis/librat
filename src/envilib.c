#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "imagelib.h"
#include "filelib.h"
#include "hipl_format.h"
#include "hiplib.h"
#include "envilib.h"
#include <stdlib.h>
#include <unistd.h>

/* This will only have effect on Windows with MSVC */
#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS 1
    #define restrict __restrict
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#ifndef EINVAL
  #define EINVAL 22
#endif

#ifndef ENOMEM
  #define ENOMEM 12
#endif


#ifndef EOVERFLOW
  #define EOVERFLOW 75
#endif
/*
https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/

POSIX getline replacement for non-POSIX systems (like Windows)
Differences:
    - the function returns int64_t instead of ssize_t
    - does not accept NUL characters in the input file
Warnings:
    - the function sets EINVAL, ENOMEM, EOVERFLOW in case of errors. The above are not defined by ISO C17,
    but are supported by other C compilers like MSVC
*/
int my_getline(char **restrict line, size_t *restrict len, FILE *restrict fp) {
    // Check if either line, len or fp are NULL pointers
    if(line == NULL || len == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    // Use a chunk array of 128 bytes as parameter for fgets
    char chunk[128];

    // Allocate a block of memory for *line if it is NULL or smaller than the chunk array
    if(*line == NULL || *len < sizeof(chunk)) {
        *len = sizeof(chunk);
        if((*line = malloc(*len)) == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }

    // "Empty" the string
    (*line)[0] = '\0';

    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
        // Resize the line buffer if necessary
        size_t len_used = strlen(*line);
        size_t chunk_used = strlen(chunk);

        if(*len - len_used < chunk_used) {
            // Check for overflow
            if(*len > SIZE_MAX / 2) {
                errno = EOVERFLOW;
                return -1;
            } else {
                *len *= 2;
            }
            
            if((*line = realloc(*line, *len)) == NULL) {
                errno = ENOMEM;
                return -1;
            }
        }

        // Copy the chunk to the end of the line buffer
        memcpy(*line + len_used, chunk, chunk_used);
        len_used += chunk_used;
        (*line)[len_used] = '\0';

        // Check if *line contains '\n', if yes, return the *line length
        if((*line)[len_used - 1] == '\n') {
            return len_used;
        }
    }

    return -1;
}

char *strcat2(char *s1,char *s2){
  /* NB for strcat:
  ** The string s1 must have sufficient space to hold the result.
  */
  char *S1;
  if(!(S1 = (char *)calloc((size_t)(strlen(s1)+strlen(s2)+1),sizeof(char)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  strcpy(S1,s1);
  free(s1);
  strcat(S1,s2);
  return(S1);
}

char *getField(char *field,char *data){
  /* assume data contains all of the text we might need
  ** and we want to return a string array of the fields
  */
  char *buffer=NULL;
  int lField,j;

  lField = (int)strlen(field);
  /* look for a hook with the first letter */
  buffer = strchr(data,field[0]);
  if(buffer==NULL)return(NULL);
  for(j=1;j<lField;j++){
    if (buffer[j] != field[j])
      return(NULL);
  }
  buffer = &data[lField];
  /* strip leading white space */
  while((int)strlen(buffer) > 1 && (buffer[0] == ' ' || buffer[0] == '\t'))buffer = &buffer[1];
  return(buffer);
}

char *getData(FILE *f,char *field){
  char *data=NULL,*reste,*reste2,*tmp,*my_string=NULL,*my_string1=NULL;
  int bytes_read=1,foundField=0,keepGoing;
  size_t nbytes = 2084;
  char *buffer;

  rewind(f);
  my_string = (char *) calloc (nbytes + 1,sizeof(char));
  buffer = (char *) calloc (nbytes + 1,sizeof(char));
  while(bytes_read>0){
    bytes_read = my_getline(&my_string, &nbytes,f);
    tmp = strchr(my_string,'\r');if(tmp != NULL)*tmp = '\0';
    tmp = strchr(my_string,'\n');if(tmp != NULL)*tmp = '\0';

    strcpy(buffer,my_string);
    if((reste = getField(field,my_string))){
      foundField = 1;
      /* now look for = */
      if((reste = getField("=",reste))){
        /* now we either find { or we dont */
        if((reste2 = getField("{",reste))){
          /* now look for the close brace */
          keepGoing = 1;
          while (keepGoing){
            if((reste = getField("}",reste2))){
              tmp = strrchr(reste2,'}');if(tmp != NULL)*tmp = '\0';
              if(data)free(data);
              data = (char *) calloc ((int)strlen(reste2) + 1,sizeof(char));
              strcpy(data,reste2);
              keepGoing = 0;
            }else{
              my_string1 = (char *) calloc (nbytes + 1,sizeof(char));
              bytes_read = my_getline(&my_string1, &nbytes,f);
              tmp = strchr(my_string1,'\r');if(tmp != NULL)*tmp = '\0';
              tmp = strchr(my_string1,'\n');if(tmp != NULL)*tmp = '\0';
              if(bytes_read == 0){
                keepGoing=0;
              }else{
                strcpy(buffer,reste2);
                buffer=strcat2(buffer,my_string1);
                reste2 = buffer;
              }
              free(my_string1);
            }
          }
          bytes_read=keepGoing;
          if(data)free(data);
          data = (char *) calloc ((int)strlen(reste2) + 1,sizeof(char));
          strcpy(data,reste2);
        }else{
          if(data)free(data);
          data = (char *) calloc ((int)strlen(reste) + 1,sizeof(char));
          strcpy(data,reste);
          bytes_read=0;
        }
      }
    }
  }
  free(my_string);
  return(data);
}

char *fgetData(FILE *f,char *field){
  char *data=NULL;
  data = getData(f,field);
  if(data == NULL){
    data = (char *) calloc (2,sizeof(char));
    data[0] = '\0';
  }
  return(data);
}

char *enviTidyImagename(char *imageName){
  char *out=NULL,*out2;
  int l;
  l = (int)strlen(imageName);
  if( !(out=(char *)calloc(l+1+4,sizeof(char)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  strcpy(out,imageName);
  /* check to see if of is .img at the end */
  out2 = &(out[l-4]);
  if ( strcmp(out2,".img") == 0)out[l-4] = '\0';
  out=strcat2(out,".hdr");
  return(out);
}

int writeEnviHeader(GenericImage *ImagePtr,char *filename){
  static char tmp[] = "";

  if(!ImagePtr->seq_history)ImagePtr->seq_history=&tmp[0];
  if(!ImagePtr->seq_desc)ImagePtr->seq_desc=&tmp[0];
  if(!ImagePtr->seq_name)ImagePtr->seq_name=&tmp[0];
  if(!ImagePtr->orig_date)ImagePtr->orig_date=&tmp[0];
  if(!ImagePtr->orig_name)ImagePtr->orig_name=&tmp[0];
/* header translator */
  /* open header file */
  if(!(ImagePtr->imageName)){
    fprintf(stderr,"error with unspecified image file\n");
    exit(1);
  }
  if (!filename || (int)strlen(filename) == 0) filename = ImagePtr->imageName;

  if(filename)ImagePtr->imageNameH=NULL;
  if(!(ImagePtr->imageNameH)){
    ImagePtr->imageNameH = enviTidyImagename(filename);  
  }
  if(ImagePtr->streamH){
    fclose(ImagePtr->streamH);
  }
  if(!(ImagePtr->streamH = fopen(ImagePtr->imageNameH,"w"))){
    fprintf(stderr,"error opening image header file %s for writing\n",ImagePtr->imageNameH);
    exit(1);
  }
  fprintf(ImagePtr->streamH,"ENVI\n");
  fprintf(ImagePtr->streamH,"description = {\n%s}\n",ImagePtr->seq_desc);
  fprintf(ImagePtr->streamH,"samples = %d\n",ImagePtr->cols);
  fprintf(ImagePtr->streamH,"lines = %d\n",ImagePtr->rows);
  fprintf(ImagePtr->streamH,"bands = %d\n",ImagePtr->frames);
  fprintf(ImagePtr->streamH,"header offset = 0\n");
  fprintf(ImagePtr->streamH,"file type = ENVI Standard\n");
  fprintf(ImagePtr->streamH,"data type = 4\n");
  fprintf(ImagePtr->streamH,"interleave = bsq\n");
  fprintf(ImagePtr->streamH,"byte order = 0");
  fclose(ImagePtr->streamH);
  ImagePtr->streamH=NULL;
  return(0);
}

int readEnviHeader(GenericImage *ImagePtr){
  char *reste,*headerName=NULL,*dummy,*fn;
  FILE *f;
  int l;
  char *filename;
  filename = getImageName(ImagePtr);

  if( !(dummy=(char *)calloc(2,sizeof(char)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  dummy[0] = '\0';
  l = (int)strlen(filename);

  if (! (f = open_file_for_read(filename))){
    char *filename2;

    filename2 = calloc((size_t)(l+1+4),sizeof(char));
    strcpy(filename2,filename);
    filename2=strcat2(filename2,".img");
    if (! (f = open_file_for_read(filename2))){
      fprintf(stderr,"error opening image file %s or %s for reading\n",filename,filename2);
      exit(1);
    }else{
      fn = (char *)calloc(l+1+4,sizeof(char));
      strcpy(fn,filename2);
      ImagePtr->imageName=fn;
    }
  }else
    ImagePtr->imageName=filename;
  ImagePtr->stream = f;
  ImagePtr->openFlag = TRUE;

  headerName = enviTidyImagename(filename);
  if (! (f = open_file_for_read(headerName))){
    fprintf(stderr,"error opening header file %s for reading\n",headerName);
    exit(1);
  }
  ImagePtr->imageNameH = headerName;
  ImagePtr->streamH = f;

  ImagePtr->imageNameHExt = "hdr";
  ImagePtr->read_header=readEnviHeader;
  ImagePtr->write_header=(void *)writeEnviHeader;
  /* use same as hips for some of these */
  ImagePtr->update_header=updateHipsHeader;
  ImagePtr->index=arrayIndexHips;


  reste = fgetData(f,"bands");
  ImagePtr->frames=atoi(reste);free(reste);
  reste = fgetData(f,"samples");
  ImagePtr->cols=atoi(reste);free(reste);
  reste = fgetData(f,"lines");
  ImagePtr->rows=atoi(reste);free(reste);
  reste = fgetData(f,"data type");
  if( atoi(reste) == 4){
    ImagePtr->format = PFFLOAT;
    ImagePtr->bits_per_pixel = (int)sizeof(float)*8;
  }else{
    fprintf(stderr,"error understanding pixel format for envi image: I only know about float (type 4) at present");
    exit(1);
  }  
  reste = fgetData(f,"interleave");
  if(!(strcasecmp(reste,"bil")))
    ImagePtr->arrayAccess=BIL;
  else if(!(strcasecmp(reste,"bsq")))
    ImagePtr->arrayAccess=BSQ;
  else{
    fprintf(stderr,"error understanding pixel interleave format for envi image: I only know about bsq and bil at present");
    exit(1);
  }
  free(reste);
  reste = fgetData(f,"test broken");
  ImagePtr->seq_desc=reste;ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  reste = fgetData(f,"description");
  ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,reste);ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  reste = fgetData(f,"projection info");ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  reste=strcat2(reste,fgetData(f,"coordinate system string"));ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  reste=strcat2(reste,fgetData(f,"map info"));ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  reste=strcat2(reste,fgetData(f,"band names"));ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");

  ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,reste);ImagePtr->seq_desc=strcat2(ImagePtr->seq_desc,"#");
  ImagePtr->seq_history=dummy;
  ImagePtr->bit_packing=FALSE;
  ImagePtr->seq_name=dummy;
  ImagePtr->orig_name=dummy;
  ImagePtr->orig_date=dummy;
  fclose(f);
  return(1);
}


int     checkEnvi(GenericImage *ImagePtr,char *env)
{
        int     i,openFlag;
        char    *extension,*name,c,*nameH;
        char HEAD[10];
        FILE    *stream=NULL,*streamH=NULL;

        strcpy(HEAD,"ENVI");
        name=getImageName(ImagePtr);
        nameH=getImageNameH(ImagePtr);

/*
**      first check filename extension (hips/HIPS)
*/
        extension=getFilenameExtension(nameH);
        if(extension && (strcasecmp(extension,"hdr")==0))return(TRUE);
/*
**      check for "E N V I  nl"
**      if can rewind the file, else, check
**      for H as first byte
*/
        if(!(streamH=getStreamH(ImagePtr))){
                setImageNameH(ImagePtr,"-");
                setStreamH(ImagePtr,stdin);
        }
        openFlag=ImagePtr->openFlag;

        if(!openFlag)openImage(ImagePtr,TRUE,env);
        if(strcmp(name,"-")==0){        /* stdin/stdout */
                if((c=fgetc(stream))!=EOF){
                        if(c==HEAD[0]){ /* probably */
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
                if(!openFlag){fclose(stream);} /* for other formats might close streamH */
                return(TRUE);
        }
        if(!openFlag)fclose(stream);
        return(FALSE);
}


/*
**      set HIPS header functions into generic image format
*/
/* return TRUE if detected image is Hips format or if !check */
int     isEnvi(GenericImage *ImagePtr,int check,char *env)
{
        int     i,retval=TRUE;
        if(check)retval=checkEnvi(ImagePtr,env);
        ImagePtr->read_header=readEnviHeader;
        ImagePtr->write_header=(void *)writeEnviHeader;
        /*ImagePtr->update_header=updateEnviHeader;
        ImagePtr->index=arrayIndexEnvi;*/
        for(i=0;i<=IMAGE_FLOAT;i++)ImagePtr->supported[i]=TRUE;
        return(retval);
}



