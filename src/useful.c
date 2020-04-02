#include <stdio.h>
#include <ctype.h>
#include "define_float.h"
#include <string.h>

int	fsign(f)
double	f;
{
	if(f>0.0)return(1);
	if(f<0.0)return(-1);
	return(0);
}

int	atoint(str,fatal,fail_Ptr)
char	*str;
int	fatal;
int	*fail_Ptr;
{
	int	out,i,dot_flag,zero_flag,atoi();
	void	exit();

	*fail_Ptr=0;
	if(sscanf(str,"%d",&out)!=1){
		*fail_Ptr=1;
		return(0);
	}
	if(out!=0){
		return(out);
	}
	dot_flag=0;zero_flag=0;
	for(i=0;i<strlen(str);i++){
		if(str[i]=='0' && *fail_Ptr==0){zero_flag=1;*fail_Ptr=0;}
		else if(str[i]=='.')dot_flag++;
		else if(zero_flag && str[i]>'0' && str[i]<='9');
		else if(fatal){
			fprintf(stderr,"atoint:\tcannot read string %s as int\n",str);exit(-1);
		}else{
			*fail_Ptr=1;
			return(0);
		}
		if(dot_flag>1){
			if(fatal){
				fprintf(stderr,"atoint:\tcannot read string %s as int\n",str);exit(-1);
			}else{
				*fail_Ptr=1;
				return(0);
			}
		}
	}
	return(1);
}

double	atodouble(str,fatal,fail_Ptr)
char	*str;
int	fatal;
int	*fail_Ptr;
{
	double	out,atof();
	int	i,dot_flag;
	void	exit();

#ifdef DOUBLEDEF
	sscanf(str,"%lf",&out);
#else
	sscanf(str,"%f",&out);
#endif	
	/*out=atof(str);*/
	if(out!=0.0){
		*fail_Ptr=0;
		return(out);
	}
	dot_flag=0;
	for(i=0;i<strlen(str);i++){
		if(str[i]=='0')*fail_Ptr=0;
		else if(str[i]=='.')dot_flag++;
		else if(fatal){
			fprintf(stderr,"atodouble:\tcannot read string %s as double\n",str);exit(-1);
		}else{
			*fail_Ptr=1;
			return(0.0);
		}
		if(dot_flag>1){
			if(fatal){
				fprintf(stderr,"atodouble:\tcannot read string %s as double\n",str);exit(-1);
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

int	sscan_int(argv,argc,i_Ptr,fatal,out)
char	**argv;
int	*i_Ptr;
int	fatal;
int	argc;
int	*out;
{
	int	test_int,fail,i;
	void	exit();

	if((*i_Ptr+1)>=argc){
		if(fatal){
			fprintf(stderr,"sscan_int:\tcannot read %dth element of string (%d elements):\n",(*i_Ptr+1),argc-1);
			fprintf(stderr,"\t\t");
			for(i=0;i<argc;i++)
				fprintf(stderr,"%s ",argv[i]);
			fprintf(stderr,"\n");
			exit(-1);
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

int	sscan_double(argv,argc,i_Ptr,fatal,out)
char	**argv;
int	*i_Ptr;
int	fatal;
int	argc;
double	*out;
{
	double	test_double,atodouble();
	int	fail=1,i;
	void	exit();

	if((*i_Ptr+1)>=argc){
		if(fatal){
			fprintf(stderr,"sscan_int:\tcannot read %dth element of string (%d elements):\n",(*i_Ptr+1),argc-1);
			fprintf(stderr,"\t\t");
			for(i=0;i<argc;i++)
				fprintf(stderr,"%s ",argv[i]);
			fprintf(stderr,"\n");
			exit(-1);
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
/*
int is_double(char *buffer,double *ptr){
	int tmpVar;
	double atof();
	*ptr = atof(buffer);
	for(tmpVar=0;tmpVar<strlen(buffer);tmpVar++){
		if(!isdigit(buffer[tmpVar] && buffer[tmpVar] != '.'))return(0);
	}
	return(1);
}
*/
