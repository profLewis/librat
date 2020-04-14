#include "prat.h"

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
        fprintf(stderr,"openFileForRead: env: %s\n",env);
        fprintf(stderr,"openFileForRead: env: %s\n",getenv(env));

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
				error2("openFileForRead:\terror in core allocation for string",*filename);
			strcpy(*filename,environment);
#ifdef DEBUG
	fprintf(stderr,"openFileForRead: extended %s\n",*filename);
#endif
		}
	}
	
	if(!found && (out=fopen(*filename,"r"))==NULL){
		if(fatal)
			error2("openFileForRead:\tfailed to open file:",*filename);
		else return((FILE *)-1);
	}
	return(out);
}

FILE    *open_file_for_read(filename)
char    *filename;
{       
        FILE    *out;
        char    msg[2000];
        void    error1();
        
        char *envVar,environment[2000],*getenv();
        char env[2000];
        int fatal = 0;

        strcpy(env,"BPMS_FILES");

#ifdef DEBUG
        fprintf(stderr,"open_file_for_read: %s\n",filename);
        fprintf(stderr,"open_file_for_read: env: %s\n",env);
#endif
        return openFileForRead(&filename,env,fatal);
/*      
        if((out=fopen(filename,"r"))==NULL){
                sprintf(msg,"open_file_for_read:\tfailed to open file %s",filename);
                error1(msg);
        }
        return(out);
*/
}


FILE	*openFileForWrite(filename,env,fatal)
char	**filename,*env;
int	fatal;
{
        FILE    *out;
        char    *envVar,environment[2000],*getenv();
        void    error2();
        int     found=FALSE;

        out=NULL;
#ifdef DEBUG
        fprintf(stderr,"openFileForWritE: %s\n",*filename);
#endif
/*
**      try the env. variable area first
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
                                error2("openFileForWrite:\terror in core allocation for string",*filename);
                        strcpy(*filename,environment);
#ifdef DEBUG
        fprintf(stderr,"openFileForWrite: extended %s\n",*filename);
#endif
                }
        }

        if(!found && (out=fopen(*filename,"w"))==NULL){
                if(fatal)
                        error2("openFileForWrite:\tfailed to open file:",*filename);
                else return((FILE *)-1);
        }
        return(out);
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
