#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void    error2i(char *msg,int i){
        fprintf(stderr,msg,i);
        exit(1);
}

void    error2f(char *msg,float i){
        fprintf(stderr,msg,i);
        exit(1);
}


void	error1(char *msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}
void	error3(char *s1,char *s2,char *s3)
{
	fprintf(stderr,"%s %s %s\n",s1,s2,s3);
	exit(1);
}
void	error2(char *s1,char *s2)
{
        fprintf(stderr,"%s %s\n",s1,s2);
        exit(1);
}
