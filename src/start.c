
/* this first line is required in the main() file */
#define RAT_MAIN
/* you need to include this file */
#include "rat.h"
#include "ratFront.h"  
#include "image_formats.h"

/* main() */ 

void doStuff();
char *interactionType(int i);

 
int main(int argc,char **argv){
	void *info=NULL; 
	RATobj *ratObj=NULL;
	void userSignals(),doStuff(RATobj *,void *);
 	int retval=0;
	
	/* this function for setting up signal interrupts if you want such things */
	/* you then need to define a fn userSignals() and also RATuserInterrupt()
		** examples of which are below
		** if you dont want these, just define a dummy RATuserInterrupt() function
		*/
	userSignals();
	
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
