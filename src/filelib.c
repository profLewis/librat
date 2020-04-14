#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "filelib.h"

#define MAXOPEN 253
#define MAXFPOPEN 253


char *getFilenameExtension(fileName)
char	*fileName;
{
	int	len,i;
	len=strlen(fileName);
	for(i=len-1;i>=0;i--)
		if(fileName[i]=='.')return(&fileName[i+1]);
	return(NULL);
}

char	*prependEnv(file,Env)
char	*file,*Env;
{
	char *out,*env,*getenv();
	int	add_slash=FALSE,size;

	if(!file)return(NULL);
	if(!Env)return(file);
	if(file[0]=='/')return(file); /* absolute filename */
	if(Env&&(env=getenv(Env))!=NULL){
		if(env[strlen(env)-1]!='/' && strlen(file)> 0)add_slash=TRUE;
		size=strlen(env)+add_slash+strlen(file)+1;
		if(!(out=(char *)calloc(size+100,sizeof(char)))){
			fprintf(stderr,"error in core allocation (%d bytes)\n",size);
			exit(1);
		}
		strcpy(out,env);
		if(add_slash)strcat(out,"/");
		strcat(out,file);
		return(out);
	}
	return(file);
}

char	*prependDIR(file,DIR,freed)
char	*file,*DIR;
int	freed;
{
	char *out;
	int	add_slash=FALSE,len;

	if(!file)return(NULL);
	if(!DIR)return(file);
	len=strlen(file)+strlen(DIR);
	if( DIR[strlen(DIR)-1] != '/' && file[0] != '/'){
		add_slash=TRUE;
		len++;
	}
	if(!(out=(char *)calloc(len+100,sizeof(char)))){
		fprintf(stderr,"error in core allocation\n");
		exit(1);
	}	
	
	strcpy(out,DIR);
	if(add_slash)strcat(out,"/");
	strcat(out,file);
	if(freed)free(file);
	return(out);
}



FILE *openFile(fileName,inputFlag,env)
char	*fileName;
int	inputFlag;
char	*env;
{
        char    flag[10],*newfileName=NULL;
	FILE    *stream;
	static int nOpen=0;

        switch(inputFlag){
                case TRUE:
                strcpy(flag,"r");
                if(strcmp(fileName,"-")==0){
                        stream=stdin;
                        return(stream);
                }
                break;
                case FALSE:
              	default:
    		strcpy(flag,"w");
                if(!fileName || strcmp(fileName,"-")==0){
                        stream=stdout;
                        return(stream);
                }
                break;
        }
	/* attempt to open filename raw, then look in ENV directory */
        if((stream=fopen(fileName,flag))==NULL){
		if((stream=fopen((newfileName=prependEnv(fileName,env)),flag))==NULL ){
	
			if(strcmp(newfileName,fileName)!=0)fileName=newfileName;
                	fprintf(stderr,"openFile: error opening file %s for mode %s\n",fileName,flag);
                	exit(1);
		}else{
			nOpen++;
			strcpy(fileName,newfileName);
		}
       	}else{
		nOpen++;
	}
#ifdef DEBUG
	fprintf(stderr,"%d files open (%s)\n",nOpen,fileName);
#endif
	rewind(stream);
	return(stream);
}

