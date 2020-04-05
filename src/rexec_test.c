#include <stdio.h>

int	main(argc,argv)
int	argc;
char	**argv;
{
	char	buffer[2000];
	int	strcmp(),quit=0,i=0;

/* open coms with master */
	fprintf(stdout,"slave_status_ok ");fflush(stdout);

/* do job */
	while(!quit){
		if( fscanf(stdin,"%s",buffer)==1){
/* check for active termination message from master process */
			if(strcmp(buffer,"master_terminate_stream")==0)quit=1;
			else
			fprintf(stdout,"%s_%d ",buffer,i);
			fflush(stdout);i++;
		}
	}

/* tidy up op */
	for(i=0;i<10;i++){
		fprintf(stdout,"doghead%d ",i);
		fflush(stdout);
	}

/* send active termination message to master befor quitting */
	fprintf(stdout," slave_terminate_stream ");
	return(1);
}
