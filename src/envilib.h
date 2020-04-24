#include "imagelib.h"

#ifndef ENVILIB_H
#define ENVILIB_H
int     isEnvi(GenericImage *ImagePtr,int check,char *env);
int     checkEnvi(GenericImage *ImagePtr,char *env);
int readEnviHeader(GenericImage *ImagePtr);
int writeEnviHeader(GenericImage *ImagePtr,char *filename);
char *enviTidyImagename(char *imageName);
char *fgetData(FILE *f,char *field);
#endif
