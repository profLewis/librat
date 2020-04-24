#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "useful.h"

int	fsign(double f)
{
	if(f>0.0)return(1);
	if(f<0.0)return(-1);
	return(0);
}

int	atoint(char *str,int fatal,int *fail_Ptr)
{
	int	out,i,dot_flag,zero_flag;

	*fail_Ptr=0;
	if(sscanf(str,"%d",&out)!=1){
		*fail_Ptr=1;
		return(0);
	}
	if(out!=0){
		return(out);
	}
	dot_flag=0;zero_flag=0;
	for(i=0;i<(int)strlen(str);i++){
		if(str[i]=='0' && *fail_Ptr==0){zero_flag=1;*fail_Ptr=0;}
		else if(str[i]=='.')dot_flag++;
		else if(zero_flag && str[i]>'0' && str[i]<='9');
		else if(fatal){
			fprintf(stderr,"atoint:\tcannot read string %s as int\n",str);exit(1);
		}else{
			*fail_Ptr=1;
			return(0);
		}
		if(dot_flag>1){
			if(fatal){
				fprintf(stderr,"atoint:\tcannot read string %s as int\n",str);exit(1);
			}else{
				*fail_Ptr=1;
				return(0);
			}
		}
	}
	return(1);
}

double	atodouble(char *str,int fatal,int *fail_Ptr)
{
	double	out;
	int	i,dot_flag;

	sscanf(str,"%lf",&out);
	/*out=atof(str);*/
	if(out!=0.0){
		*fail_Ptr=0;
		return(out);
	}
	dot_flag=0;
	for(i=0;i<(int)strlen(str);i++){
		if(str[i]=='0')*fail_Ptr=0;
		else if(str[i]=='.')dot_flag++;
		else if(fatal){
			fprintf(stderr,"atodouble:\tcannot read string %s as double\n",str);exit(1);
		}else{
			*fail_Ptr=1;
			return(0.0);
		}
		if(dot_flag>1){
			if(fatal){
				fprintf(stderr,"atodouble:\tcannot read string %s as double\n",str);exit(1);
			}else{
				*fail_Ptr=1;
				return(0.0);
			}
		}
	}
	return(out);
}

/*
**	scan char ** [i+1] for integer
*/

int	sscan_int(char **argv,int argc,int *i_Ptr,int fatal,int *out)
{
	int	test_int,fail,i;

	if((*i_Ptr+1)>=argc){
		if(fatal){
			fprintf(stderr,"sscan_int:\tcannot read %dth element of string (%d elements):\n",(*i_Ptr+1),argc-1);
			fprintf(stderr,"\t\t");
			for(i=0;i<argc;i++)
				fprintf(stderr,"%s ",argv[i]);
			fprintf(stderr,"\n");
			exit(1);
		}else return(0);
	}
			
	test_int=atoint(argv[*i_Ptr+1],fatal,&fail);
	if(!fail){
		(*i_Ptr)++;
		*out=test_int;
		return(1);
	}
	return(0);
}
/*
**	scan char ** [i+1] for double
*/

int	sscan_double(char **argv,int argc,int *i_Ptr,int fatal,double *out)
{
	double	test_double;
	int	fail=1,i;

	if((*i_Ptr+1)>=argc){
		if(fatal){
			fprintf(stderr,"sscan_int:\tcannot read %dth element of string (%d elements):\n",(*i_Ptr+1),argc-1);
			fprintf(stderr,"\t\t");
			for(i=0;i<argc;i++)
				fprintf(stderr,"%s ",argv[i]);
			fprintf(stderr,"\n");
			exit(1);
		}else return(0);
	}
	test_double=atodouble(argv[*i_Ptr+1],fatal,&fail);
	if(!fail){
		(*i_Ptr)++;
		*out=test_double;
		return(1);
	}
	return(0);
}
