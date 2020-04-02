#include <stdio.h>

#ifdef _NO_FORTRAN
void erf_(){
  fprintf(stderr,"ERROR: cannot make call to erf_ (-D_NO_FORTRAN was set on compile\n");
  exit(1);
}
#endif
 
#ifdef _NO_INDEX
char * index(s,c) char *s,*c;{
  return((char *)strchr(s,c));
}
#endif
