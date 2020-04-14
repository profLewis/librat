#include <stdio.h>
void	exit();
void	error1(msg)
char *msg;
{
	fprintf(stderr,"%s\n",msg);
	exit(0);
}
void	error3(s1,s2,s3)
char *s1,*s2,*s3;
{
	fprintf(stderr,"%s %s %s\n",s1,s2,s3);
	exit(0);
}
void	error2(s1,s2)
char *s1,*s2;{
        fprintf(stderr,"%s %s\n",s1,s2);
        exit(0);
}
