#include <stdio.h>
#include <math.h>
#include <hipl_format.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#ifndef __SUNPRO_C
#include <strings.h>
#endif

#include "useful3.h"
#include "vectors2.h"
#include "hips.h"
#include "wavefront.h"
#include "error.h"

void	f_clear(float *f_Ptr,int size)
{
	int	i;
	for(i=0;i<size;i++) *(f_Ptr + i) = 0.0;
	return;
}

void	i_clear(int *i_Ptr,int size)
{
	int	i;
	for(i=0;i<size;i++) *(i_Ptr + i) = 0;
	return;
}

void	c_clear(char *c_Ptr,int size)
{
	int	i;
	for(i=0;i<size;i++) *(c_Ptr + i) = 0;
	return;
}

void	t_clear(triplet *t_Ptr,int size)
{
	int	i;
	for(i=0;i<size;i++){
		t_Ptr[i].x = 0.0;
		t_Ptr[i].y = 0.0;
		t_Ptr[i].z = 0.0;
	}
	return;
}

triplet	*t_allocate(int blocksize)
{
	triplet	*out;char	msg[100];
	if((out=(triplet *)calloc(blocksize,sizeof(triplet)))==0){
		sprintf(msg,"error in type triplet core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

char *VM_allocate(unsigned int size,unsigned int blocksize)
{
	void	*out;
	if((out=(char *)calloc(size,blocksize))==0)error1("error in memory allocation");
	return(out);
}

void *v_allocate(int size,int blocksize)
{
	void	*out;

	out=calloc((int)size,(int)blocksize);

	if((out)==0)error1("error in memory allocation");
	return(out);
}

int *i_allocate(int size)
{
	int	*out;
	if((out=(int *)calloc(size,sizeof(int)))==0)error1("error in integer allocation");
	return(out);
}

Bezier_patch	*B_allocate(int blocksize)
{
	Bezier_patch	*out;char	msg[100];
	if((out=(Bezier_patch *)calloc(blocksize,sizeof(Bezier_patch)))==0){
		sprintf(msg,"error in type Bezier_patch core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

Ivect3	*I_allocate(int blocksize)
{
	Ivect3	*out;char	msg[100];
	if((out=(Ivect3 *)calloc(blocksize,sizeof(Ivect3)))==0){
		sprintf(msg,"error in type Ivect3 core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

float	*f_allocate(int blocksize)
{
	float	*out;char	msg[100];
	if((out=(float *)calloc(blocksize,sizeof(float)))==0){
		sprintf(msg,"error in type float core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

double	*d_allocate(int blocksize)
{
	double	*out;char	msg[100];
	if((out=(double *)calloc(blocksize,sizeof(double)))==0){
		sprintf(msg,"error in type double core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

unsigned char	*b_allocate(int blocksize)
{
	unsigned char	*out;char	msg[100];
	if((out=(unsigned char *)calloc(blocksize,sizeof(unsigned char)))==0){
		sprintf(msg,"error in type (signed) char core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

char	*c_allocate(int blocksize)
{
	char	*out;char	msg[100];
	if((out=(char *)calloc((unsigned)blocksize,(unsigned)sizeof(char)))==0){
		sprintf(msg,"error in type char core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

ipair	*ipair_allocate(int blocksize)
{
	ipair	*out;char	msg[100];
	if((out=(ipair *)calloc((unsigned)blocksize,sizeof(ipair)))==0){
		sprintf(msg,"error in type double core allocation: size %d\n",blocksize);
		error1(msg);
	}
	return(out);
}

Data	D_allocate(int pixel_format,int blocksize)
{
	Data	buffer;
	switch(pixel_format){
		case PFFLOAT:
		buffer.fdata=f_allocate(blocksize);break;
		case PFBYTE:
		buffer.bdata=b_allocate(blocksize);break;
	}
	return(buffer);
}
