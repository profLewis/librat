#include <prat.h>



/*
**	main
*/

int	main(argc,argv)
int     argc;
char    **argv;
{
	int	start=0,every=1,i,j,k;
	struct header head;
	float	*f_allocate(),*data,min = 1e99, max = -1e99;

	for(i=1;i<argc;i++)
        if(*argv[i]=='-'){
                switch(*(argv[i]+1)){
			case 'e':
			every=atoi(argv[++i]);
			if(every < 0 )error1("useage:\n\tstatships -e every -s start");
			break;
			case 's':
			start=atoi(argv[++i]);
			if(start < 0 )error1("useage:\n\tstatships -e every -s start");
			break;
		}
	}					
			

	read_header(&head);
	if(head.pixel_format != PFFLOAT)error1("statships: double format expected");
	data=f_allocate(head.rows*head.cols);

	for(i=0;i<head.num_frame;i++){
		if(!read(0,data,head.rows*head.cols*sizeof(float)))error1("statships: error reading image data");
		if( ((i-start) % every )==0){
			fprintf(stderr,"statships:processing frame %d\n",i);
			for(j=0;j<head.rows;j++)
			for(k=0;k<head.cols;k++){
				if( *(data + j*head.cols + k) > max)max= *(data + j*head.cols + k);
				if( *(data + j*head.cols + k) < min)min= *(data + j*head.cols + k);
			}
		}
	}
	fprintf(stdout,"%f %f\n",min,max);
				
	return(0);
}
