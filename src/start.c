
/* this first line is required in the main() file */
#define RAT_MAIN
/* you need to include this file */
#include "rat.h"
#include "ratFront.h"  
#include "image_formats.h"

char *interactionType(int i){
	static char r[] = "reflectance",t[]="transmittance",n[]="nothing";
	char *out;
	switch(i){
		case 0:
			out=&r[0];
			break;
		case 1:
			out=&t[0];
			break;
		default:
			out=&n[0];
			break;   
	}
	return(out);
}

/*
########################
 */

void doStuff(RATobj *ratObj,void *info){
	int nSuns=1,flag,i,j,k,usedNumber=-1,nsamples[3];
	char *thisname=NULL,yesno,nl;
	double from[3],*solar=NULL,view[3],direction[3],min[3],max[3],bbox[6];
	double *sunVector=NULL,*wavelength=NULL,*bandwidth=NULL;
	int nBands=0,nMaterials;
	RATtree *ratTree=NULL;
	RATmaterials *ratMat=NULL;
	void *nullstr=NULL;
	int nHistogramSamples,ok;
	char filename[1024];
	
	RATsetNsuns(ratObj,nSuns);
	while ( (fscanf(stdin,"%d",&flag))==1){
		switch(flag){
			case 0:
				/* break */
				return;
				break;
			case 1:
				ok=0;
				while(ok==0){
					ok=1;
					fprintf(stderr,"how many sun vectors? (>0): ");
					if(fscanf(stdin,"%d",&nSuns) != 1 || nSuns < 1 )ok=0;
					else{
						if(solar)free(solar);
						if(!(solar=(double *)calloc(3*nSuns,sizeof(double)))){
							fprintf(stderr,"error in core allocation\n");
							exit(1);
						}
						RATsetNsuns(ratObj,nSuns);
						/* set sun vector from the array solar */
						for(i=0;i<nSuns;i++){
							fprintf(stderr,"enter sun vector number %d (3 floats): ",i+1);
							fscanf(stdin,"%lf %lf %lf",&solar[3*i+0],&solar[3*i+1],&solar[3*i+2]);
						}
						RATsetSun(ratObj,solar); 
					}
				}
					break;
			case 2:
				/* print sun */
				nSuns=RATgetNsuns(ratObj);
				sunVector=RATgetSun(ratObj);
				for(i=0;i<nSuns;i++){
					fprintf(stdout,"Sun vector %d: %lf %lf %lf\n",i+1,sunVector[i*3+0],sunVector[i*3+1],sunVector[i*3+2]);
				}
					break;
			case 3:
				/* read a new set of wavelengths (min and width) and set these 
				** i.e. nBands w0 b0 w1 b1 ...
				*/
				fscanf(stdin,"%d",&nBands);
				wavelength=(double *)v_allocate(nBands,sizeof(double));
				bandwidth=(double *)v_allocate(nBands,sizeof(double));
				for(i=0;i<nBands;i++){
					fscanf(stdin,"%lf %lf",&wavelength[i],&bandwidth[i]);
				}
					RATsetNWavebands(ratObj,nBands,wavelength,bandwidth);
				/* free up memory */
				free(wavelength);wavelength=NULL;
				free(bandwidth);bandwidth=NULL; 
				nBands=0;
				break;
			case 4:
				/* print wavebands */
				fprintf(stdout,"wavebands: ");
				wavelength=(double *)v_allocate(MAX_WAVEBANDS,sizeof(double));
				nBands=RATgetNWavebands(ratObj,wavelength);
				for(i=0;i<nBands;i++){
					fprintf(stdout,"%lf ",wavelength[i]);
				}
					fprintf(stdout,"\n");
				free(wavelength);wavelength=NULL;
				break;
			case 5:
				/* read wavefront object file */
				fscanf(stdin,"%s",RATgetWavefrontFilename(ratObj));
				RATreadWavefrontFile(ratObj);  
				break;
			case 6:
				/* trace a ray tree */
				fscanf(stdin,"%lf %lf %lf",&from[0],&from[1],&from[2]);
				fscanf(stdin,"%lf %lf %lf",&direction[0],&direction[1],&direction[2]);
				ratTree=RATgetRatTree(ratObj);
                                ratObj->hit_camera=0;
				RATtraceRay(ratObj,from,direction,NULL);
				ratTree=RATgetRatTree(ratObj);
				nBands=RATgetNWavebands(ratObj,NULL);
				nSuns=RATgetNsuns(ratObj);
				fprintf(stderr,"RTD %d\n",ratTree->thisRTD);
				for(i=0;i<=ratTree->thisRTD;i++){
					fprintf(stderr,"order: %d\tintersection point:\t%f %f %f\n\t\tray length:\t\t%f\n\t\tintersection material:\t%d\n",i,ratTree->intersectionPoints[i][0],ratTree->intersectionPoints[i][1],ratTree->intersectionPoints[i][2],ratTree->rayLengths[i], ratTree->ratmat[i]);
					for(k=0;k<nSuns;k++){
						if(ratTree[k].hitSun[i]){
							fprintf(stderr,"\t\tsun %d:\t\t\t%d %s\n", k,ratTree[k].hitSun[i], interactionType(ratTree[k].directPathIntersectType[i]));        
							if(ratTree->lengthToSun)fprintf(stderr,"lengthToSun: %f angleToSun %f\n",ratTree[k].lengthToSun[i],ratTree[k].angleToSun[i]);
							fprintf(stderr,"\t\tdirect:\t\t\t");
							for(j=0;j<nBands;j++){
								fprintf(stderr,"%e ",ratTree[k].directRadiance[i*nBands+j]);
							}
							fprintf(stderr,"\n"); 
						}else{
							fprintf(stderr,"\t\tsun %d:\t\t\tno hit\n",k);
						}
					}
				}
					i=ratTree->thisRTD;
				if(i>0 || ratTree->thisRTD==0){
					fprintf(stderr,"\t\tsky  :\t\t\t%s\n",interactionType(ratTree->diffusePathIntersectType[i]));
					fprintf(stderr,"\t\tdiffuse:\t\t");
					for(j=0;j<nBands;j++){
						fprintf(stderr,"%e ",ratTree->diffuseRadiance[i*nBands+j]);
					}
					fprintf(stderr,"\n");
				}
					break;
			case 16:
				{
					double centre[3],extent[2],value,sx,sy,ox,oy;
					int n,r,c,nrows,ncols,rpp;
					char filename[MAX_STRING_LENGTH];
					float *buffer=NULL;

					ratTree=RATgetRatTree(ratObj);
					direction[0]=0.0;
					direction[1]=0.0;
					direction[2]=-1.0;
					/* height map */
					fscanf(stdin,"%lf %lf %lf",&centre[0],&centre[1],&centre[2]);
					/* specify centre */
					from[2]=centre[2];
					fscanf(stdin,"%lf %lf",&extent[0],&extent[1]);
					/* specify x,y extent */
					fscanf(stdin,"%d %d %d",&nrows,&ncols,&rpp);
					/* specify nrows ncols rpp */
					fscanf(stdin,"%s",filename);
					buffer = (float *)v_allocate(sizeof(float),nrows*ncols);
					sx = extent[0]/(float)ncols;
					sy = extent[1]/(float)nrows;
					ox = -extent[0]*0.5 + centre[0];
					oy = -extent[1]*0.5 + centre[1];
					for(r=0;r<nrows;r++){
						for(c=0;c<ncols;c++){
							fprintf(stderr,"\r(%8.4f)              ",(r*ncols+c)*100./(ncols*nrows));
							value = 0.0;
							for(n=0;n<rpp;n++){
								from[0] = ox + (c+Random())*sx;
								from[1] = oy + (r+Random())*sy;
                                                                ratObj->hit_camera=0;
                                				RATtraceRay(ratObj,from,direction,NULL);
                                				ratTree=RATgetRatTree(ratObj);
                                        			value += ratTree->intersectionPoints[0][2];
							}
							buffer[c+r*ncols] = value/(float)rpp;
						}
					}
					/* put data into an image and write out */
					RATputImage(filename,buffer,nrows,ncols,1,IMAGE_FLOAT);
			      	}
				break; 
			case 7:
				/* get and material information */
				ratMat = RATgetMaterials(ratObj,&nMaterials);
				fprintf(stderr,"There are %d materials:\n\n",nMaterials);
				for(i=0;i<nMaterials;i++){
					fprintf(stderr,"***** Material %d *****\n",i);
					
					/* example of translation to used material list */
					thisname = RATtranslateMaterial(ratObj,i,&usedNumber);
					
					fprintf(stderr,"\tname: %s\tis used? %d\n\ttype: (%d) = %s\n\tNumber in used material list = %d ",ratMat[i].name,ratMat[i].used,ratMat[i].type,ratMat[i].typeName,usedNumber);
					
					if(thisname){
						fprintf(stderr,"\tname: %s",thisname);
					}
					fprintf(stderr,"\n"); 
					/* if its SRM, print it out */
					/* other types include TRANSPARENT and VOLUMETRIC */
					if(ratMat[i].type == SRM ) {
						fprintf(stderr,"\tnBands: %d:\n",ratMat[i].nWavelengths);
						for(j=0;j<ratMat[i].nWavelengths;j++){
							fprintf(stderr,"%lf ",ratMat[i].wavelengths[j]);
							if(ratMat[i].diffuse_reflectance){
								fprintf(stderr,"%lf ",ratMat[i].diffuse_reflectance[j]);
							}
							if(ratMat[i].diffuse_transmittance){
								fprintf(stderr,"%lf ",ratMat[i].diffuse_transmittance[j]);
							}
							fprintf(stderr,"\n");
						}
					}
				}
					break;
			case 8:
				/* get rat object summary */
				/* set max & min from bbox */
				RATgetBBoxMin(ratObj,&min[0]);
				RATgetBBoxMax(ratObj,&max[0]);
				nsamples[0] =  nsamples[1] = nsamples[2] =  -1;
				yesno='n';
				while(yesno != 'y' && nl != 'y'){
					fprintf(stderr,"Extent of scene is {%lf %lf %lf} to {%lf %lf %lf} accept? (y/n)? ",min[0],min[1],min[2],max[0],max[1],max[2]);
					fscanf(stdin,"%c%c",&nl,&yesno);
					if(yesno == 'n'){
						fprintf(stderr,"Enter minimum extent (3 floats): ");
						fscanf(stdin,"%lf %lf %lf",&min[0],&min[1],&min[2]);
						fprintf(stderr,"Enter maximum extent (3 floats): ");
						fscanf(stdin,"%lf %lf %lf",&max[0],&max[1],&max[2]);
						for(j=0;j<3;j++)if(max[j]<=min[j]){
							fprintf(stderr,"Error in definition: max must be greater than min\n");
							yesno=0;
						}
					}
				}
					yesno='n';
					nsamples[0]=512;nsamples[1]=512;nsamples[2]=3;
					while(yesno != 'y' && nl != 'y'){
						fprintf(stderr,"How many samples in each dimension? %d %d %d  accept? (y/n)? ",nsamples[0],nsamples[1],nsamples[2]);
						fscanf(stdin,"%c%c",&nl,&yesno);
						if(yesno == 'n'){
							fprintf(stderr,"Enter number of samples (3 integers): ");
							if(fscanf(stdin,"%d %d %d",&nsamples[0],&nsamples[1],&nsamples[2]) != 3 )nsamples[0]=-1;
						}
						if(nsamples[0]<0 || nsamples[0]<0 || nsamples[0]<0){nsamples[0]=512;nsamples[1]=512;nsamples[2]=3;yesno='n';}	
					}
					nHistogramSamples=90;
					yesno='n';
					while(yesno != 'y' && nl != 'y'){
						fprintf(stderr,"How many histogram samples? %d accept? (y/n)? ",nHistogramSamples);
						fscanf(stdin,"%c%c",&nl,&yesno);
						if(yesno == 'n'){	
							fprintf(stderr,"Enter number of samples (1 integer): ");	
							if(fscanf(stdin,"%d",&nHistogramSamples) != 1 )nHistogramSamples=-1;
							if(nHistogramSamples<=0){nHistogramSamples=90;yesno='n';}
						}
					}
					strcpy(filename,"volume");
					yesno='n';
					while(yesno!='y' && nl != 'y'){
						fprintf(stderr,"Root filename for output files? %s accept? (y/n)? ",filename);
						fscanf(stdin,"%c%c",&nl,&yesno);
						if(yesno == 'n'){
							fprintf(stderr,"Enter root filename for output files (1 string): "); 
							if(fscanf(stdin,"%s",filename) != 1 )strcpy(filename,"");
							if(strlen(filename)==0){strcpy(filename,"volume");yesno='n';}
						}
					}
					fprintf(stderr,"getting object information ...\n");
					if(nsamples[0] == 0 || nsamples[1] == 0 ||  nsamples[2] == 0)
						RATgetObjects(ratObj,-1,min,max,NULL,nHistogramSamples,filename);
					else
						RATgetObjects(ratObj,-1,min,max,nsamples,nHistogramSamples,filename);
					/* print out object information */
					fprintf(stderr,"printing object information ...\n");
					RATprintObjects(ratObj);
					fprintf(stderr,"done\n");
					break;
					case 9:
						/* print out how many materials there are */
						fprintf(stderr,"There are %d materials defined\n",RATgetNmaterials(ratObj));
						/* and print the ones that are used */
						ratMat = RATgetMaterialsUsed(ratObj,&nMaterials);
						fprintf(stderr,"There are %d materials used:\n\n",nMaterials);
						for(i=0;i<nMaterials;i++){
							fprintf(stderr,"***** Material %d *****\n",i);
							fprintf(stderr,"\tname: %s\tindex: %d\n\ttype: (%d) = %s\n",ratMat[i].name,ratMat[i].index,ratMat[i].type,ratMat[i].typeName);
							/* if its SRM, print it out */
							/* other types include TRANSPARENT and VOLUMETRIC */
							if(ratMat[i].type == SRM ) {
								fprintf(stderr,"\tnBands: %d:\n",ratMat[i].nWavelengths);
								for(j=0;j<ratMat[i].nWavelengths;j++){
									fprintf(stderr,"%lf ",ratMat[i].wavelengths[j]);
									if(ratMat[i].diffuse_reflectance){
										fprintf(stderr,"%lf ",ratMat[i].diffuse_reflectance[j]);
									}
									if(ratMat[i].diffuse_transmittance){
										fprintf(stderr,"%lf ",ratMat[i].diffuse_transmittance[j]);
									}
									fprintf(stderr,"\n");
								}
							}
						}
							break;
					case 10:
						fprintf(stderr,"Verbose level is currently %d",RATgetVerboseLevel(ratObj));
						yesno='u';
						{  
							int verbosity=-1;
							while ( yesno != 'y' && yesno != 'n' ){
								fprintf(stderr,"\n\tChange it? (y/n): ");
								fscanf(stdin,"%c%c",&nl,&yesno);
							}
							if(yesno == 'y'){
								while ( verbosity < 0 || verbosity > 1){
									fprintf(stderr,"\nset verbosity level (0-1): ");
									fscanf(stdin,"%c%d",&nl,&verbosity); 
								}
								RATsetVerboseLevel(ratObj,verbosity);
							}else fprintf(stderr,"\n");
						}
							fprintf(stderr,"Verbose level is now %d\n",RATgetVerboseLevel(ratObj));
						break;
					case 11:
						/* print out bbox info */
						RATgetBBoxMin(ratObj,&min[0]);
						RATgetBBoxMax(ratObj,&max[0]);
						fprintf(stdout,"x: %lf %lf\ny: %lf %lf\nz: %lf %lf\nbbox centre @ %lf %lf %lf\n",min[0],max[0],min[1],max[1],min[2],max[2],0.5*(min[0]+max[0]),0.5*(min[1]+max[1]),0.5*(min[2]+max[2]));
						break; 
					case 12:  
					break;
					case 14:
                                        {
                                                RATdevice *ratCamera=NULL;
                                                RATdevice *ratLight=NULL;
						char camerafile[1024],lightfile[1024];

						fscanf(stdin,"%s %s",camerafile,lightfile);
                                                ratCamera=RATreadCameraFile(camerafile,ratObj);
                                                ratLight=RATreadCameraFile(lightfile,ratObj);

                                                RAToutputCameraDriverData(ratCamera,1);
                                                RAToutputCameraDriverData(ratLight,1);

                                                RATScanPSF(ratObj,ratCamera,1,ratLight,1);

                                                RAToutputCameraResults(ratObj,ratCamera,1);
						exit(0);
                                        }
					case 13: 
					{
						RATdevice *ratCamera=NULL;
						RATdevice *ratLight=NULL;
						
						ratCamera=RATreadCameraFile("camera.dat",ratObj);
						ratLight=RATreadCameraFile("light.dat",ratObj);
						
						RAToutputCameraDriverData(ratCamera,1);
						RAToutputCameraDriverData(ratLight,1);
						
						RATScanPSF(ratObj,ratCamera,1,ratLight,1);

						RAToutputCameraResults(ratObj,ratCamera,1);
					}
					break;
                                        case 15: /* read in orders format */
                                        {
                                                char ordersfileroot[1024],spectralfile[1024],outputfile[1024];
                                                double **fdata=NULL,bininfo[2];
                                                RATorder *orders=NULL;
 						int nLidarBins=0;
                                                double ****radiance,***direct,***diffuse;

                                                fscanf(stdin,"%s %s %s",ordersfileroot,spectralfile,outputfile);
                                                fdata=RATreadSpectra(spectralfile);
						orders=RATreadOrderTerms(bininfo,&nLidarBins,fdata,ordersfileroot);   
						radiance=RATassignSpectraToOrder(fdata,orders,bininfo,nLidarBins);  
                                       		RATOutputIntegralFromOrder(outputfile,orders,nLidarBins,bininfo,radiance,RESULT_INTEGRAL_MODE_DISTANCE);
 
                                        }
 					break;
					case -1:
						/* print out memore usage (Kb) */
						fprintf(stderr,"memory use (Kb): %d\n",RATgetMemory());
						break;
					case -2:
						/* print PID */
						fprintf(stderr,"%s: PID %d\n",RATgetProgramName(ratObj),RATgetPID());
						break;
					default:
						fprintf(stderr,"options:\n");
						fprintf(stderr,"\t-2                   : print PID\n");
						fprintf(stderr,"\t-1                   : print memory use\n");
						fprintf(stderr,"\t 0                   : quit\n");
						fprintf(stderr,"\t 1 n s1x s1y s1z ... : set sun vectors\n");
						fprintf(stderr,"\t 2                   : print sun vectors\n");
						fprintf(stderr,"\t 3 n b1 w1 ...i bn wn: set wavebands\n");
						fprintf(stderr,"\t 4                   : print wavebands\n");
						fprintf(stderr,"\t 5 file.obj          : read object file\n");
						fprintf(stderr,"\t 6 fx fy fz dx dy dz : trace ray from f in direction d\n");
						fprintf(stderr,"\t 7                   : get and print materials\n");
						fprintf(stderr,"\t 8                   : print object information\n");
						fprintf(stderr,"\t 9                   : print info on materials used\n");
						fprintf(stderr,"\t 10                  : get and set verbosity level (0-1)\n");
						fprintf(stderr,"\t 11                  : get and print object bbox information\n");
						fprintf(stderr,"\t 12		       :\n");
						fprintf(stderr,"\t 13                  : same as 14 assuming filenames camera.dat light.dat\n");
						fprintf(stderr,"\t 14 camera.dat light.dat                : ray tracing using defined camera & illumination\n");
						fprintf(stderr,"\t 15 		       : dont go there\n");
						fprintf(stderr,"\t 16 cx cy cz sx sy nrows ncols rpp name : produce a height map in name\n");				
						break;
		}
	}
		return;
}

/*
########################
 *
 * example of a user-defined command line arguments
 * these functions need to be declared whether you use them or not.
 */

/* argument checking:
**
** this gets called by RATparse
** use info to pass through any information you want to use
** and cast it within RATuserParse
**  
** all user arguments must start -U*
** set numberOfArguments to how ever many you use
*/ 

/* user arguments for help */
void RATuserPrintOptions(RATobj *ratObj){
	fprintf(stderr,"[-Useless] [-UsetSun x y z]");
	return;
}

int RATuserParse(RATobj *ratObj,int thisarg,int argc,char **argv,void *info){
	int numberOfArguments=-1,i;
	double solar[3],atof();
	
	if(!(strcmp(argv[thisarg],"-Useless"))){
		fprintf(stderr,"\n\t...this option is useless\n\tand does nothing other than print this\n\n");
		numberOfArguments=0;
	}else if(!(strcmp(argv[thisarg],"-UsetSun"))){
		/* say how many arguments */
		numberOfArguments=3;
		/* example ... setting a vector */
		for(i=0;i<numberOfArguments;i++){
			/* check we dont overrun */
			if(thisarg+1+i >= argc){
				fprintf(stderr,"error in number of arguments for -UsetSun option: 3 required\n");
				exit(1);
			}
			solar[i]=atof(argv[thisarg+1+i]);
		}
		/* set the sun vector */
		RATsetSun(ratObj,solar);
	}
	return(numberOfArguments);
}


/*
########################
 *
 * example of a signal interrupt
 *
 */

void userSignals(){
	/* always call RATsignalInterrupt */
	signal(SIGUSR1,RATsignalInterrupt);
	signal(SIGUSR2,RATsignalInterrupt);
	signal(SIGSTOP,RATsignalInterrupt);
        signal(SIGSEGV,RATsignalInterrupt);
}

void RATuserInterrupt(RATobj *ratObj,int sig){
	switch(sig){
		case SIGUSR1:
			fprintf(stderr,"rat:\tsignal interrupt %d for process %d\n",sig,RATgetPID());
			break;
		case SIGUSR2:
			fprintf(stderr,"rat:\tsignal interrupt %d for process %d\n",sig,RATgetPID());
			break;
		case SIGSTOP:
			fprintf(stderr,"rat:\tsignal interrupt %d for process %d\n",sig,RATgetPID());
			exit(1);
			break;
		case SIGSEGV:
			fprintf(stderr,"rat:\tsignal interrupt %d for process %d\n(try switching it off and on again ;-) )\n",sig,RATgetPID());
			exit(1);
			break;
	}
	return;
}
