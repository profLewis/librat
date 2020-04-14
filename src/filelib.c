#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "filelib.h"
#include "error.h"

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

char *typer(int inputFlag){
  switch(inputFlag){
    case TRUE:
    return("input");
    break;
    case FALSE:
    return("output");
    break;
  }
  return("unknown");
}
#define CLOSE -1

FILE *openFile(fileName,inputFlag,env)
char	*fileName;
int	inputFlag;
char	*env;
{
        char    flag[10],*newfileName=NULL;
	FILE    *stream;
	static int nOpen=0;

        if(strlen(fileName)==0){
          error2("Error: no file name specified","NONE");
          exit(1);
        }

        switch(inputFlag){
                case CLOSE:
                stream=(FILE *)env;
                fclose(stream);
                nOpen--;
#ifdef DEBUG
                fprintf(stderr,"Close: %d files open (%s)\n",nOpen,fileName);
#endif
                return(NULL);
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
#ifdef DEBUG
        fprintf(stderr,"I'm trying to open %s for %s ... \n",fileName,typer(inputFlag));
#endif
        if((stream=fopen(fileName,flag))==NULL){
#ifdef DEBUG
            fprintf(stderr,"I tried to open %s for %s but failed ... \n",fileName,typer(inputFlag));
#endif
#ifdef DEBUG
            char *getenv();
            fprintf(stderr,"Try env: %s %s ... \n",env,getenv(env));
#endif
	    if((stream=fopen((newfileName=prependEnv(fileName,env)),flag))==NULL ){

#ifdef DEBUG
                fprintf(stderr,"I'm trying to open %s for %s ... \n",newfileName,typer(inputFlag));
#endif	
		if(strcmp(newfileName,fileName)!=0)fileName=newfileName;
                fprintf(stderr,"openFile: error opening file %s for %s\n",fileName,typer(inputFlag));
                return(NULL);
	    }else{
		nOpen++;
		strcpy(fileName,newfileName);
	    }
       	}else{
		nOpen++;
	}
#ifdef DEBUG
	fprintf(stderr,"Open: %d files open (%s)\n",nOpen,fileName);
#endif
	rewind(stream);
	return(stream);
}

