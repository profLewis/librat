#include <stdio.h>
#include <math.h>

/* matherr, called by atan2() etc when they fail see: man matherr(3) */

/* 
** does not reset default return values, merely gives the user 
** extra info on why there was a domain error etc.
*/

#ifdef sun
int matherr(exc)
struct exception *exc;
{
	switch(exc->type){
		case	DOMAIN:
		if(strcmp(exc->name,"acos")==0){
			if(exc->arg1>1.0)exc->retval=0.0;
			if(exc->arg1< -1.0)exc->retval=M_PI;
		}else if(strcmp(exc->name,"asin")==0){
			if(exc->arg1> 1.0)exc->retval=M_PI_2;
			if(exc->arg1< -1.0)exc->retval= -M_PI_2;
		}
			
		fprintf(stderr,"%s: argument domain exception (parameters %lf %lf)\n",exc->name,exc->arg1,exc->arg2);
		break;
		case 	SING:
		fprintf(stderr,"%s: argument singularity (parameters %lf %lf)\n",exc->name,exc->arg1,exc->arg2);
		break;
		case 	OVERFLOW:
		fprintf(stderr,"%s: overflow range exception (parameters %lf %lf)\n",exc->name,exc->arg1,exc->arg2);
		break;
		case 	UNDERFLOW:
		fprintf(stderr,"%s: underflow range exception (parameters %lf %lf)\n",exc->name,exc->arg1,exc->arg2);
		break;
	}
	return(0);
}
#endif
