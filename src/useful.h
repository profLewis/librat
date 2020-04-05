/* Miscellaneous useful subroutines and declarations */

#ifndef TIMUSEFUL
#define TIMUSEFUL

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define ABS(X) ((X)<0?(-(X)):(X))

	extern double drand48();
	extern void srand48();

	extern int find_switch();
	extern int find_string();
#endif

