#ifndef FILELIB_H
#define FILELIB_H
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a<b)?(a):(b))
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#include "files.h"

char *getFilenameExtension(char *fileName);
char    *prependEnv(char *file,char *Env);
char    *prependDIR(char *file,char *DIR,int freed);
char *typer(int inputFlag);
FILE *openFile(char *fileName,int inputFlag,char *env);


#endif
