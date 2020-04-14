#include "prat.h"
#include "filelib.h"

FILE	*openFileForRead(filename,env,fatal)
char	**filename,*env;
int	fatal;
{
	return(openFile(*filename,TRUE,env));
}

FILE    *open_file_for_read(filename)
char    *filename;
{       
        char *envVar,environment[2000],*getenv();
        char env[2000];
        int fatal = 0;

        strcpy(env,"BPMS_FILES");
        return(openFile(filename,TRUE,env));
}

FILE	*openFileForWrite(filename,env,fatal)
char	**filename,*env;
int	fatal;
{
  	return(openFile(*filename,FALSE,env));
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
