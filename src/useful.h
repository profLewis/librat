/* Miscellaneous useful subroutines and declarations */

#include <stdlib.h>
#include <string.h>

#ifndef TIMUSEFUL
#define TIMUSEFUL

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifndef MAX
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#endif
#ifndef MIN
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#endif
#ifndef ABS
#define ABS(X) ((X)<0?(-(X)):(X))
#endif
#endif


#ifndef USEFUL_H
#define USEFUL_H
int     fsign(double f);
int     atoint(char *str,int fatal,int *fail_Ptr);
double  atodouble(char *str,int fatal,int *fail_Ptr);
int     sscan_int(char **argv,int argc,int *i_Ptr,int fatal,int *out);
int     sscan_double(char **argv,int argc,int *i_Ptr,int fatal,double *out);

#endif
