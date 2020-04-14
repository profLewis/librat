#include "prat.h"

FILE	*open_file_for_read(filename)
char	*filename;
{
	FILE	*out;
	char	msg[2000];
	void	error1();
	
	if((out=fopen(filename,"r"))==NULL){
		sprintf(msg,"open_file_for_read:\tfailed to open file %s",filename);
		error1(msg);
	}
	return(out);
}
	
/*
**	improved version of open_file_for_read
**	which allows for other places to search for the filename
*/

FILE	*openFileForAny(filename,env,fatal,f)
char	*filename,*env,*f;
int	fatal;
{
	FILE	*out;
	char	*getenv();
	void	error2();
	int	found=FALSE;

	out=NULL;
/*
**	try the env. variable area first
*/

	if(filename[0] != '/' && (envVar=getenv(env))!=NULL){
		strcpy(environment,envVar);
		if((environment[0]=='/' && environment[strlen(environment)-1]!='/') || (strcmp(environment,".")==0 || strcmp(environment,"..")==0))
			strcat(environment,"/");
		strcat(environment,filename);
		if((out=fopen(environment,f))!=NULL){
			found=TRUE;
			if(!(filename=(char *)calloc(strlen(environment)+1,sizeof(char))))
				error2("open_file:\terror in core allocation for string",_filename);
			strcpy(filename,environment);
		}
	}
	
	if(!found && (out=fopen(filename,f))==NULL){
		if(fatal)
			error2("open_file:\tfailed to open file:",filename);
		else return((FILE *)-1);
	}
	return(out);
}

FILE *NewopenFileForRead(filename,env,fatal)
char    **filename,*env;
int     fatal;
{
        FILE    *out;
  if((out=fopen(*filename,"r"))==NULL){
     if(fatal==1)error2("openFileForRead: error opening file",*filename);
     return(NULL);
  }
  return(out);
}

FILE	*openFileForRead(filename,env,fatal)
char	**filename,*env;
int	fatal;
{
	FILE	*out;
	char	*envVar,environment[2000],*getenv();
	void	error2();
	int	found=FALSE;

	out=NULL;
#ifdef DEBUG
	fprintf(stderr,"openFileForRead: %s\n",*filename);
#endif
/*
**	try the env. variable area first
*/
	if((*filename)[0]!='/' && (envVar=getenv(env))!=NULL){
		strcpy(environment,envVar);
		if((environment[0]=='/' && environment[strlen(environment)-1]!='/') || (strcmp(environment,".")==0 || strcmp(environment,"..")==0))
			strcat(environment,"/");
		strcat(environment,*filename);
		if((out=fopen(environment,"r"))!=NULL){
			found=TRUE;
			/*free(*filename);*filename=NULL;*/
			if(!(*filename=(char *)calloc(strlen(environment)+1,sizeof(char))))
				error2("open_file:\terror in core allocation for string",*filename);
			strcpy(*filename,environment);
#ifdef DEBUG
	fprintf(stderr,"openFileForRead: extended %s\n",*filename);
#endif
		}
	}
	
	if(!found && (out=fopen(*filename,"r"))==NULL){
		if(fatal)
			error2("open_file:\tfailed to open file:",*filename);
		else return((FILE *)-1);
	}
	return(out);
}


FILE	*openFileForWrite(filename,env,fatal)
char	*filename,*env;
int	fatal;
{
	return(openFileForAny(filename,env,fatal,"w"));
}

/*
**	expand filename with environmental variable
**	and check file exists
*/

int	expand_filename(filename,env,fatal)
char	**filename,*env;
int	fatal;
{
	FILE	*fp;

	fp=openFileForRead(filename,env,fatal);
	if(fp != (FILE *)-1 && fp != NULL){
		fclose(fp);
		return(1);
	}else
		return(0);	/* cant open file */
}


int	get_no_of_columns_in_file(verbose,filename,rows)
char	*filename;
int	verbose,*rows;
{
	int	i[2],no_of_columns=0,no_of_columns_check=0;
	FILE	*fp;
	char	*Dummy,dummy[3000],dum[100];

	i[0]=0;
	i[1]=0;
	fp=open_file_for_read(filename);
	while(fgets(dummy,3000,fp)!=NULL){
		Dummy=dummy;
		if(i[0]==0){
			no_of_columns=0;
			while(sscanf(Dummy,"%s",dum)==1){
				Dummy=strchr(Dummy,dum[0])+strlen(dum);
				no_of_columns++;
			}
			no_of_columns_check=no_of_columns;
		}else{
			no_of_columns_check=0;
			while(sscanf(Dummy,"%s",dum)==1){
				Dummy=strchr(Dummy,dum[0])+strlen(dum);
				no_of_columns_check++;
			}
		}
		if(no_of_columns_check!=no_of_columns)fprintf(stderr,"get_no_of_columns_in_file:\tWarning: inconsistency in no of columns read in file %s at line %d\n",filename,i[0]);
		i[0]++;
		(*rows)++;
	}
	if(i[0]==0)
		fprintf(stderr,"get_no_of_columns_in_file:\tWarning: 0 lines read in file %s\n",filename);
	else if(verbose){
		fprintf(stderr,"%d lines   read in file %s\n",i[0],filename);
		fprintf(stderr,"%d columns read in file %s\n",no_of_columns,filename);
	}
	fclose(fp);
	i[1]=no_of_columns;
	return(i[1]);
}
