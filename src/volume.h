#ifndef _VOLUME_
#define _VOLUME_
/*
**	parameters for volume model
*/

typedef struct{
  double	uL;
  double 	l;	/* leaf linear dimension */
  triplet (*lad)(void *);	/* normal vector */
  int	material;
  
  /* tmp storage */
  triplet	s;
  double m;
  double G;
  double *Glut;
  int nSamples;
  
  int VOLUME_nZeniths;
  float *VOLUME_nZenithsFreq;

}Volume;
#endif
