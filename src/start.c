
/* this first line is required in the main() file */
#define RAT_MAIN
/* you need to include this file */
#include "rat.h"
#include "ratFront.h"  
#include "image_formats.h"

/* main() */ 
 
int main(int argc,char **argv){
	void *info=NULL; 
	RATobj *ratObj=NULL;
	void doStuff(RATobj *,void *);
 	int retval=0;
	
	/* RAT initialisation and memory allocation */
	ratObj=RATinit(argc,argv);
	
	/* check default frat arguments
		** user parsing is done within this function by calls to RATuserParse()
		** with help options etc. specified in RATuserPrintOptions()
		** examples of which are below
		*/
	retval=RATparse(ratObj,argc,argv,info);   
	
	/* read the data from the object file (if specified) 
		** into the ratObj structure 
		*/
	if(RATisWavefrontFile(ratObj))
		RATreadObject(ratObj);
	
	doStuff(ratObj,info);
	return(1);
}

