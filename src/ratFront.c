/*
 *  ratFront.c
 *  ratlib
 *
 *  Created by Philip Lewis on 05/09/2007.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <string.h>
#include "rat.h"
#include "ratFront.h" 
#include "imagelib.h"
/*size_t strlen();*/
#ifndef i386
size_t strlen();
#endif
/*
**  rubbish on suns ... use strtok() instead of strsep()
*/
#include <string.h>
#include <strings.h>
 
#define strsep(a,b) strtok(a,b)
#define index(a,b) strchr((a),(b))



/* private functions */
/* some local utility functions (essentially private) */

/* need to mimic strsep for some architectures */
/* strsep splits *stringp into 2 strings
** separated by any character in delim
** and returns the first
** and sets *stringp to the rest
*/
char *Strsep(stringp,delim)
     register char **stringp;
     register const char *delim;
{
  register char *s;
  register const char *spanp;
  register int c, sc;
  char *tok;
  
  if ((s = *stringp) == NULL)
    return (NULL);
  for (tok = s;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = NULL;
        else
          s[-1] = 0;
        *stringp = s;
        return (tok);
      }
    }while (sc != 0);
  }
  /* NOTREACHED */
}

double *tripletToArray(triplet t){
  double *out=NULL;
  out=(double *)v_allocate(3,sizeof(double));
  out[0]=t.x;
  out[1]=t.y;
  out[2]=t.z;
  return(out);
}

triplet arrayToTriplet(double *t){
  triplet out;
  out.x=t[0];
  out.y=t[1];
  out.z=t[2];
  return(out);
}

void processPSF(RATdevice *rat){ 
  int i,j,k,c,r,thisPixel,C,R;
  double *integral=NULL,*thresh=NULL,sumIntegral,delta,sum,*rands=NULL;
  int *columnStore=NULL;
  int rPrev,cPrev,rFirst,rLast,cFirst,cLast,location,skip;
  double sumPrev=-1,easyThresh=1.0,var;
  
  float *f_imagedata=NULL;
  int *i_imagedata=NULL;
  int rows,cols,frames,type,size;
  
  /* check to see if information aready exists */
  
  if(rat->gridMap && rat->PSFRead && rat->modulationMap && rat->sampleCountImage){
    skip=0;
    /* grid map */
    f_imagedata=(float *)RATgetImage(rat->gridMap,&rows,&cols,&frames,&type,&size);
    if(f_imagedata && frames == 2 && cols == rat->nPsf[0] && rows == rat->nPsf[1] && type == IMAGE_FLOAT){
      /* data file right size & type, so use this */
      for(i=0;i<rows*cols;i++){
	rat->psfmap[0][i]=(int)(f_imagedata[i]+0.5);
	rat->psfmap[1][i]=(int)(f_imagedata[i+rows*cols]+0.5);	
      }
    }else skip=1;
    /* modulation map */
    if(!skip){
      f_imagedata=(float *)RATgetImage(rat->modulationMap,&rows,&cols,&frames,&type,&size);
      if(f_imagedata && frames == 1 && cols == rat->nS[0] && rows == rat->nS[1] && type == IMAGE_FLOAT){
	/* data file right size & type, so use this */
	for(i=0;i<rows*cols;i++){
	  rat->psfModulation[i]=f_imagedata[i];
	}
      }else skip=1;
      if(!skip){
	/*SampleCount  map */
	i_imagedata=(int *)RATgetImage(rat->sampleCountImage,&rows,&cols,&frames,&type,&size);
	if(i_imagedata && frames == 1 && cols == rat->nPsf[0] && rows == rat->nPsf[1] && type == IMAGE_INT){
	  /* data file right size & type, so use this */
	  for(i=0;i<rows*cols;i++){
	    rat->psfSampleCount[i]=i_imagedata[i];
	  }
	}else skip=1;
      }
    }
    if(!skip)return;
    rat->PSFRead=0;
  }
  
  integral = (double *)v_allocate(1+MAX(rat->nPsf[0],rat->nPsf[1]),sizeof(double));
  thresh = (double *)v_allocate(MAX(rat->nS[0],rat->nS[1]),sizeof(double));
  columnStore = (int *)v_allocate(MAX(rat->nS[0],rat->nS[1]),sizeof(int));
  rands = (double *)v_allocate(rat->rpp,sizeof(double));
  
  /* loop over columns & calculate cumulative sum */
  var=0.;
  for(i=0;i<rat->nPsf[0];i++){ 
    integral[i]=0.;
    for(j=0;j<rat->nPsf[1];j++){
      integral[i] += rat->psf[j*rat->nPsf[0]+i];
      var += rat->psf[j*rat->nPsf[0]+i]*rat->psf[j*rat->nPsf[0]+i];
    }
    if(i>0)integral[i]+=integral[i-1];
  }
  sumIntegral = integral[rat->nPsf[0]-1];
  var = var/(rat->nPsf[0]*rat->nPsf[1]) - pow(integral[rat->nPsf[0]-1]/(rat->nPsf[0]*rat->nPsf[1]),2);
  /* spcial 'simple' case */
  if ( var < easyThresh  && rat->nPsf[0] == rat->nS[0] && rat->nPsf[1] == rat->nS[1]){
    rat->nPixels = 0; 
    for(i=0;i<rat->nS[0];i++){
      for(j=0;j<rat->nS[1];j++){
	if(rat->psf[j*rat->nPsf[0]+i]>0){
          rat->psfmap[0][j*rat->nPsf[0]+i]=i;
          rat->psfmap[1][j*rat->nPsf[0]+i]=j;
	  rat->psfModulation[i+j*rat->nS[0]]=1.0;
	  rat->psfSampleCount[i+j*rat->nS[0]]=rat->rpp;
	  (rat->nPixels)++;
        }else{
	  rat->psfmap[0][j*rat->nPsf[0]+i]=-1;
	  rat->psfmap[1][j*rat->nPsf[0]+i]=-1;
	  rat->psfModulation[i+j*rat->nS[0]]=0.0;
	  rat->psfSampleCount[i+j*rat->nS[0]]=0;
        }
      }
    }
    return;
  }


  integral[rat->nPsf[0]] = sumIntegral*10.; /* in case of rounding overflow */
  /* split integral into nS[0] sections */
  for(i=0,j=0;i<rat->nS[0];i++){
    thresh[i] = sumIntegral*(i+1)/rat->nS[0];
    while(thresh[i]>integral[j])j++;
    delta = (integral[j] - thresh[i])/(j == 0 ? 1 : integral[j]-integral[j-1]);
    if(delta>0.5){
      j--;
    }
    columnStore[i]=j=MAX(0,j);
    j++;
  }
  /* store the selected column */
  k=0;
  for(i=0;i<rat->nS[0];i++){
    for(;k<=columnStore[i]&&k<rat->nPsf[0];k++){
      for(j=0;j<rat->nPsf[1];j++){
	if(rat->psf[j*rat->nPsf[0]+k] > 0)
	  rat->psfmap[0][j*rat->nPsf[0]+k] = i;
        else
	  rat->psfmap[0][j*rat->nPsf[0]+k] = -1;
      }
    }
  }	
  /* now sort the rows */
  /* loop over column */
  c=0;
  for(i=0;i<rat->nS[0];i++){
    sum=0.;thisPixel=0;
    C=c;
    for(r=0;r<rat->nPsf[1];r++){
      c=C;
      for(;c<=columnStore[i]&&c<rat->nPsf[0];c++){
	if(rat->psf[c+r*rat->nPsf[0]]>0){
	  sum += rat->psf[c+r*rat->nPsf[0]];
	  rat->psfmap[1][r*rat->nPsf[0]+c] = thisPixel;
	}else{
	  rat->psfmap[1][r*rat->nPsf[0]+c] = -1;
	}
				/* update to be in line with sum */
	thisPixel=(int)(sum*rat->nS[1]/(i == 0 ? integral[columnStore[i]] : integral[columnStore[i]] - integral[columnStore[i-1]]));
      }
    }
  }
  /* psfmap[0] : col
  ** psfmap[1] : row
  ** psfmap gives the mapping from the psf map to image coordinates
  ** for the output image
  **
  ** rat->nPsf : the number of samples for each psf grid point
  */	
  
  /* loop over image columns & rows */
  for(i=0;i<rat->nS[0];i++){
    for(j=0;j<rat->nS[1];j++){
      /* find r of pixels in hires image columns c = (i == 0 ? 0 : columnStore[i-1])  to (i == 0 ? columnStore[i] : columnStore[i] - columnStore[i-1])
      ** for which rat->psfmap[1][r*rat->nPsf[0]+c] == j
      */
      rLast = -1;rFirst = -1;
      for(k=(i == 0 ? 0 : MAX(0,columnStore[i-1]-1));k<=MIN(columnStore[i]+1,rat->nPsf[0]-1);k++){
	for(r=0;r<rat->nPsf[1];r++){
	  if((rFirst<0 || r < rFirst)&& (rat->psfmap[1][r*rat->nPsf[0]+k] == j))rFirst = r;
	  if((rFirst<0 || r > rLast) && (rat->psfmap[1][r*rat->nPsf[0]+k] == j))rLast = r;
	}
      }
      cLast = columnStore[i]+1;
      cFirst = (i == 0 ? 0 : MAX(0,columnStore[i-1]-1));	
      /* loop over cFirst,rFirst to cLast,rLast and sum up */
      sum=0.;
      for(r=rFirst;r<=MIN(rLast,rat->nPsf[1]-1);r++){
	for(c=cFirst;c<=MIN(cLast,rat->nPsf[0]-1);c++){
	  location = r*rat->nPsf[0]+c;
	  if(rat->psfmap[1][location]==j && rat->psfmap[0][location]==i && rat->psf[location]>0){
	    sum += rat->psf[location];
	  }
	}
      }
      for(k=0;k<rat->rpp;k++)rands[k]=Random()*sum;
      rPrev=-1;cPrev=-1;sumPrev=-1;sum=0;
      for(r=rFirst;r<=MIN(rLast,rat->nPsf[1]-1);r++){
	for(c=cFirst;c<=MIN(cLast,rat->nPsf[0]-1);c++){
	  location = r*rat->nPsf[0]+c;
	  if(rat->psfmap[1][location]==j && rat->psfmap[0][location]==i && rat->psf[location]>0){
	    sum += rat->psf[location];
	    for(k=0;k<rat->rpp;k++)if(rands[k]>=0){
	      if(rands[k]<=sum && rands[k]>=sumPrev){
		delta = (rands[k] - (sumPrev == -1 ? 0 : sumPrev))/(sum - (sumPrev == -1 ? 0 : sumPrev));
		if(delta<0.5){
		  C=(cPrev == -1 ? c : cPrev);
		  R=(rPrev == -1 ? r : rPrev); 
		}else{
		  C=c;
		  R=r;
		}
		/* increment counter */
		(rat->psfSampleCount[C+R*rat->nPsf[0]])++;
		/* delete this one */
		rands[k] = -1;
	      }
	    }
	    rPrev=r;cPrev=c;sumPrev=sum;
	  }
	}
      }
      /* put any remainders in rPrev cPrev */
      if(cPrev != -1 && rPrev != -1)for(k=0;k<rat->rpp;k++)if(rands[k]>=0){
	(rat->psfSampleCount[cPrev+rPrev*rat->nPsf[0]])++;
	rands[k] = -1;
      }
    }
  }
  /* calculate a modulation map */
  
  for(i=0;i<rat->nS[1]*rat->nS[0];i++)rat->psfModulation[i]=0.;
  for(r=0;r<rat->nPsf[1];r++){
    for(c=0;c<rat->nPsf[0];c++){
      location = r*rat->nPsf[0]+c;
      i = rat->psfmap[0][location];
      j = rat->psfmap[1][location];
      if(rat->psf[location]>0){
	rat->psfModulation[i+j*rat->nS[0]] += rat->psf[location];
      }
    }
  }
  for(r=0;r<rat->nS[1];r++){
    for(c=0;c<rat->nS[0];c++){
      if(rat->psfModulation[c+r*rat->nS[0]]>0){
	rat->psfModulation[c+r*rat->nS[0]] = (sumIntegral/rat->nPixels)/rat->psfModulation[c+r*rat->nS[0]];
      }
    }
  }
  /* ideal: sumIntegral/rat->nPixels */
  
  free(integral);
  free(thresh);
  free(columnStore);
  free(rands);
  
  return;
}

/* some dumper utilities */



float *RATgetPsf(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i;
  float *out=NULL; 
  *rows = rat->nPsf[1];
  *cols = rat->nPsf[0];
  *frames = 1;
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(i=0;i<*rows * *cols * *frames;i++){
    out[i] = rat->psf[i];
  }
  return(out);
}

float *RATgetPsfModulation(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i;
  float *out=NULL; 
  *rows = rat->nS[1];
  *cols = rat->nS[0];
  *frames = 1;
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(i=0;i<*rows * *cols * *frames;i++){
    out[i] = rat->psfModulation[i];
  }
  return(out);
}


float *RATgetPsfResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i,k,row,col,nPix;
  float *out=NULL;
  *rows = rat->nPsf[1];
  *cols = rat->nPsf[0];
  nPix = rat->nS[0]*rat->nS[1];
  
  *frames = rat->nBands * (rat->nBins == 0 ? 1 : rat->nBins + 2);
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(k=0;k<*frames;k++){
    for(i=0;i<*rows * *cols;i++){
      col = rat->psfmap[0][i];
      row = rat->psfmap[1][i];
      out[i + k*rat->nPsf[1]*rat->nPsf[0]] = rat->imagedata[col + row*rat->nS[0] + k*nPix];
    }
  }
  return(out);
}


float *RATgetPsfRawResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i;
  float *out=NULL;
  *rows = rat->nPsf[1];
  *cols = rat->nPsf[0];
  *frames = rat->nBands * (rat->nBins == 0 ? 1 : rat->nBins + 2);
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(i=0;i<*rows * *cols * *frames;i++){
    out[i] = rat->psfdata[i];
  }
  return(out);
}


float *RATgetResultImage(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i;
  float *out=NULL;
  *rows = rat->nS[1];
  *cols = rat->nS[0];
  *frames = rat->nBands * (rat->nBins == 0 ? 1 : rat->nBins + 2);
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(i=0;i<*rows * *cols * *frames;i++){
    out[i] = rat->imagedata[i];
  }
  return(out);
}

float *RATgetPsfMap(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i,j;
  float *out=NULL;
  *rows = rat->nPsf[1];
  *cols = rat->nPsf[0];
  *frames = 2;
  *type = IMAGE_FLOAT;
  /* returns a copy of the image */
  out = (float *)v_allocate(*rows * *cols * *frames, sizeof(float));
  for(j=0;j<2;j++){
    for(i=0;i<*rows * *cols ;i++){
      out[i+j* *rows * *cols] = rat->psfmap[j][i];
    }
  }
  return(out);
}


int *RATgetPsfSampleCount(RATdevice *rat,int *rows,int *cols,int *frames,int *type){
  int i;
  int *out=NULL;
  *rows = rat->nPsf[1];
  *cols = rat->nPsf[0];
  *frames = 1;
  *type = IMAGE_INT;
  /* returns a copy of the image */
  out = (int *)v_allocate(*rows * *cols * *frames, sizeof(int));
  for(i=0;i<*rows * *cols * *frames;i++){
    out[i] = rat->psfSampleCount[i];
  }
  return(out);
}

void *RATgetImage(char *name,int *rows,int *cols,int *frames,int *type,int *size){
  GenericImage   *image=NULL;
  int ok;
  int i;

  i = (int)strlen(name);
  image = allocateImageArray(1,NULL);
  setImageDefaults(image,TRUE);
  if(globalRATObj->flagbag->verbose)setImageVerbose(image);
  setImageName(image,name);
  ok = readImage(image,NULL);
  if(!ok)return(0);	
  *rows = getImageRows(image);
  *cols = getImageCols(image);
  *frames = getImageFrames(image);
  *type = getImageFormat(image);
  *size = getDataSize(*type);
  return(image->image);
  
}

void *RATgetImageDouble(char *name,int *rows,int *cols,int *frames,int *type){
  GenericImage   *image=NULL;
  int ok,i,j,k;
  double *out;
  int pixelsPerFrame, pixelsPerColumn;
  
  image = allocateImageArray(1,NULL);
  setImageDefaults(image,TRUE);
  if(globalRATObj->flagbag->verbose)setImageVerbose(image);
  setImageName(image,name);
  ok = readImage(image,NULL);
  if(!ok)return(0);	
  *rows = getImageRows(image);
  *cols = getImageCols(image);
  *frames = getImageFrames(image);
  *type = getImageFormat(image);
  pixelsPerFrame = *rows * *cols;
  pixelsPerColumn = *cols;
  out = (double *)v_allocate(pixelsPerFrame * *frames,sizeof(double));
  for(k=0;k<*frames;k++){
    for(j=0;j<*rows;j++){
      for(i=0;i<*cols;i++){
	out[k * pixelsPerFrame + j * pixelsPerColumn + i] = getPixelFloat(image,k,j,i);
      }
    }
  }
  return(out);
}

double *RATgenerateGaussian(char *typeOfFunction,int cols,int rows,double sdx,double sdy,double Cx,double Cy,double thresh,double sum){
  double *out=NULL,sumer=0.,value;
  int cx,cy;
  int i,j;
  double s2p,X,Y,ss2x,ss2y,scale;
  
  s2p = sqrt(2.*M_PI);
  
  out=(double *)v_allocate(cols*rows,sizeof(double));
  if(Cx<0 || Cx >= cols){
    cx = (int)(cols/2.0+0.5);
  }else cx=Cx;
  if(Cy<0 || Cy >= rows){
    cy = (int)(rows/2.0+0.5);
  }else cy = Cy;


  if(!strcasecmp(typeOfFunction,"gaussian")){
    if(sdx<=0)sdx = cols/5.0;
    if(sdy<=0)sdy = rows/5.0;
    ss2x = sdx*sdx*2.;
    ss2y = sdy*sdy*2.;
  
    for(i=0;i<cols;i++){
      for(j=0;j<rows;j++){
        X = i - cx;
        Y = j - cy;
        value = exp(-X*X/ss2x)*exp(-Y*Y/ss2y);
        if(value > thresh){
	  out[i+j*cols] = value;
	  sumer += value;
        }
      }
    }
  }else if(!strcasecmp(typeOfFunction,"circular")){
    thresh=MIN(cols,rows)*0.5;

     for(i=0;i<cols;i++){
      for(j=0;j<rows;j++){
        X = i - cx;
        Y = j - cy;
        value = sqrt(X*X+Y*Y);
        if(value <= thresh){
          out[i+j*cols] = 1.0;
          sumer += 1.0;
        }
      }
    }
  }
  if(sumer != sum && sum > 0){
    scale = sum/sumer;
    for(i=0;i<cols;i++){
      for(j=0;j<rows;j++){
	out[i+j*cols] *= scale;
      }
    }
  }
  return(out);
}

int RATputImage(char *name,void *data,int rows,int cols,int frames,int type){
  GenericImage   *image=NULL;
  
  image = allocateImageArray(1,NULL);
  setImageDefaults(image,FALSE);
  setImageName(image,name);
  setImageRows(image,rows);
  setImageCols(image,cols);
  setImageFrames(image,frames);
  setImageFormat(image,type);
  image->mmap=-1;
  allocateImage(image);
  image->image = data;
  image->mmap=-1;
  if(globalRATObj->flagbag->verbose)setImageVerbose(image);
  return(writeImage(image,globalRATObj->globalArgc,globalRATObj->globalArgv,1,1,NULL));
}

double linearInterpolateFunction(double *fn,int ns,double where){
  int ia,ib;
  double a,b,prop;
  
  if(where<0)return(0.);
  if(where>ns)return(1.0);
  ia=(int)where;
  ib=ia+1;
  a=fn[ia];
  b=(ib<ns ? fn[ib] : 1.0);
  prop = where - ia;
  return(a + prop*(b-a)); 
}

int pulseResample(double *binWeight,int *rayBin,double rayLengthStart,RATdevice *camera,RATdevice *light){
  int nBins=0,ki,i,rbStart=-1;
  int iRayLengthStart,iRayLengthEnd;
  double a,b,k,d,prop,cumulativeStart,cumulativeEnd,sum=0;
  
  /* deal with underflow */
  if(rayLengthStart-camera->binStart<0){
    /* locate binStart */
    a=linearInterpolateFunction(light->cumulativePulse,light->pulseSamples,(camera->binStart-rayLengthStart)/light->pulseStep); 
    rayBin[0]=0;
    binWeight[0] = a;
    if(binWeight[0]<0){
      printf("err...");
    }
    sum += a;
    nBins++;
  }
  /* loop over the lidar sample bins */
  for(i=1+MIN(camera->nBins-1,MAX(0,(int)((double)(rayLengthStart-camera->binStart)/camera->binStep)));i<=1+MIN(camera->nBins-1,MAX(0,(int)((double)(rayLengthStart-camera->binStart+light->pulseLength)/camera->binStep)));i++){
    rayBin[nBins]=i;
    a=linearInterpolateFunction(light->cumulativePulse,light->pulseSamples,MAX(0,((i-1)*camera->binStep + camera->binStart - rayLengthStart)/light->pulseStep)-1);
    b=linearInterpolateFunction(light->cumulativePulse,light->pulseSamples,MAX(0,((i  )*camera->binStep + camera->binStart - rayLengthStart)/light->pulseStep)-1);
    binWeight[nBins]=b-a;
    if(binWeight[nBins]<0){
      printf("err... %lf %lf %lf",a,b,b-a);
    }
    sum += (b-a);
    nBins++; 
  }
  /* deal with overflow */
  if(sum <1.0){
    rayBin[nBins]=camera->nBins+1;
    binWeight[nBins]=1-sum;
    if(binWeight[nBins]<0){
      printf("err... %lf %lf %lf",a,b,b-a);
    }
    sum = 1.0;
    nBins++;  
  } 
  return(nBins);
}

int getPulseBins(int *rayBin,double *binWeight,int nRayBins,RATdevice *camera,RATdevice *light,double rayLength){
  int nBins=0;
  double rayLengthStart,rayLengthEnd,*resampled=NULL; 
  int iRayLengthStart,iRayLengthEnd;
  
  if(nRayBins==1 || camera->nBins == 0 ){
    /* no pulse */
    binWeight[0]=1.;
    rayBin[0]=(camera->nBins == 0 ? 0 : MIN(camera->nBins+1,MAX(0,(int)(1+(double)(rayLength - camera->binStart)/(double)camera->binStep))));
    return(1); 
  }
  
  /* pulse */
  /* shorten by pulse start */
  rayLengthStart = rayLength - light->pulseStart;
  rayLengthEnd = rayLengthStart + light->pulseLength;
  /* which bins for start and end? */
  if(rayLengthEnd <= camera->binStart){
    /* all of ray is before the start of recording so into bin 0 */
    nBins=1;
    binWeight[0]=1.;
    rayBin[0]=0;
  }else if(rayLengthStart>=camera->binMax){
    /* all of the ray is after the end of recording so in bin camera->nBins+1 */
    nBins=1;
    binWeight[0]=1.;
    rayBin[0]=camera->nBins+1;
  }else{
    /* resample the pulse to the waveform sampling intervals */
    nBins=pulseResample(binWeight,rayBin,rayLengthStart,camera,light); 
  }
  return(nBins);
}

int searchForThisMat(RATobj *rat,RATorder *order,int thisMat,int thisType){
  static int **lut=NULL;
  int retval,i,j,_thisMat,_thisType;
  if(thisType == -1)thisType=0;
 if(lut==NULL){
  lut=(int **)v_allocate(2,sizeof(int *));
  for(i=0;i<2;i++){
   lut[i]=(int *)v_allocate(rat->nMaterials,sizeof(int));
   for(j=0;j<*(rat->nMaterials);j++)lut[i][j]=-1;
  }
  for(i=0;i<order->m;i++){
   _thisMat = order->ratMat[i].index;
   if(!(strcmp(order->ratMat[i].typeName,"srm with reflectance")))_thisType=0;
   else if(!(strcmp(order->ratMat[i].typeName,"srm with transmittance")))_thisType=1;  
   else _thisType = -1;
   if(_thisType != -1){
    lut[_thisType][_thisMat] = i;
   }
  }
 }
 /* else lut is already sorted */
 if(thisType == -1)thisType=0;
 if(thisType >=0 && thisType <= 1)retval = lut[thisType][thisMat]; 
 else retval = -1;
 return(retval);
}

int *decodeDiffusePath(int i,RATobj *rat,RATtree *ratTree,RATorder *order){
 static int *code=NULL;
 int coder,j,thisMat,thisType;
 if(!code)code=(int *)v_allocate(order->m,sizeof(int));
 else for(j=0;j<order->m;j++)code[j]=0;

 for(j=0;j<=i;j++){
  thisMat = ratTree->ratmat[j];
  thisType = ratTree->diffusePathIntersectType[j];
  coder = searchForThisMat(rat,order,thisMat,thisType);
  if(coder == -1)
   return(NULL); /* dodgy material */
  code[coder]++;
 }
 return(code);
}

int *decodeDirectPath(int i,RATobj *rat,RATtree *ratTree,RATorder *order){
 static int *code=NULL; 
 int coder,j,thisMat,thisType;
 if(!code)code=(int *)v_allocate(order->m,sizeof(int));
 else for(j=0;j<order->m;j++)code[j]=0;

 for(j=0;j<=i;j++){
  thisMat = ratTree->ratmat[j];
  thisType = ratTree->directPathIntersectType[j];
  coder = searchForThisMat(rat,order,thisMat,thisType);
  if(coder == -1)
   return(NULL); /* dodgy material */
  code[coder]++;
 } 
 return(code);
}

/* public functions with RAT... */

double **RATreadSpectra(char *filename){
  int i,j,nbands=0;
  int nspectra=0;
  FILE *fp=NULL;
  double **fdata=NULL;
  char buffer[MAX_STRING_LENGTH];
  char **ap,*Str,*strings[1024];

  if(!(fp=(FILE *)fopen(filename,"r"))){
    fprintf(stderr,"error opening spectra file %s\n",filename);
    exit(0);
  }
  /* read one line */
  fgets(buffer,MAX_STRING_LENGTH-1,fp);
  /* how many columns? */
  Str=&buffer[0];
  for (nspectra=0,ap = strings; (*ap = Strsep(&Str," \t\n")) != NULL;nspectra++){
   ;
  }
  nspectra-=2; /* the first is wavelength + a newline assumed  */
  rewind(fp);
  fdata=(double **)v_allocate(nspectra+2,sizeof(double *)); 
  while(fgets(buffer,MAX_STRING_LENGTH-1,fp) != NULL)
   nbands++;  
  for(i=0;i<nspectra+2;i++)fdata[i]=v_allocate(MAX(2,nbands),sizeof(double)); 
  rewind(fp);
  for(i=0;i<nbands;i++){
    for(j=0;j<nspectra+1;j++)fscanf(fp,"%lf",&(fdata[j+1][i]));
  } 
  fdata[0][0]=nbands;
  fdata[0][1]=nspectra;
  return(fdata);
}

void addContributions(RATorder *order,double *sum, struct RATorderStore *base,double **refl,int m,int n);
void addContributions(RATorder *order,double *sum, struct RATorderStore *base,double **refl,int m,int n){
   struct RATorderStore *nbase;
   int i,j,k,scatteringOrder;
   double tmp[2],r,s,t;
   if(base->code){
     /* we are here */
     scatteringOrder=0;for(i=0;i<m;i++)scatteringOrder+=base->code[i];
     s=order->direct[scatteringOrder][base->index];
     t=order->diffuse[scatteringOrder][base->index];
     tmp[0]=s;tmp[1]=t;
     if(s>0 || t>0){
      for(i=0;i<m;i++){
       tmp[0] *= refl[base->code[i]][i];
       tmp[1] *= refl[base->code[i]][i];
      }
      sum[0] += tmp[0];
      sum[1] += tmp[1];
     }
     return;
   }else{
    for(i=0;i<base->nelements;i++){
      nbase=&(base->r[i]);
      addContributions(order,sum,nbase,refl,m,n);
    }
   }
   return;
}

double ****RATassignSpectraToOrder(double **fdata,RATorder *orders,double *bininfo,int nLidarBins){
  double ****fullRadiance=NULL,sum[2];
  RATorder *order=NULL;
  int i,j,k,l,h;
  int n,m,nbands;
  double **refl=NULL;
  struct RATorderStore *base=NULL;

  n=orders->n;
  m=orders->m;
  nLidarBins=(nLidarBins == 0 ? 1 : nLidarBins+2);
  nbands=orders->ratMat->nWavelengths;
  refl=(double **)v_allocate(n+1,sizeof(double *));
  for(k=0;k<n+1;k++)refl[k]=(double *)v_allocate(m,sizeof(double));

  fullRadiance=(double ****)v_allocate(2,sizeof(double ***));

  fullRadiance[0]=(double ***)v_allocate(nLidarBins,sizeof(double **));
  fullRadiance[1]=(double ***)v_allocate(1,sizeof(double **));
  
  for(i=0;i<nLidarBins;i++){
    order=&orders[i];
    fullRadiance[0][i]=(double **)v_allocate(nbands,sizeof(double *));
    if(i==0)fullRadiance[1][i]=(double **)v_allocate(nbands,sizeof(double *));
    for(j=0;j<nbands;j++){
      /* load up refl[order][m] */
      for(k=0;k<m;k++){
        refl[0][k] = order->ratMat[k].diffuse_reflectance[j];
        for(l=1;l<n+1;l++){
          refl[l][k] = pow(refl[0][k],l);
        }
      } 
      /* allocate storage */
      fullRadiance[0][i][j]=(double *)v_allocate(n+1,sizeof(double));
      if(i==0)fullRadiance[1][i][j]=(double *)v_allocate(n+1,sizeof(double)); 
      for(k=1;k<n+1;k++){
        /* scan over order */
        sum[0]=sum[1]=0;
        base = order->r[k];
        addContributions(order,sum,base,refl,m,n); 
        /*fprintf(stderr,"%d %d %d %lf %lf\n",i,j,k,sum[0],sum[1]); */
        fullRadiance[0][i][j][k-1] = sum[0];   
        if(i==0)fullRadiance[1][0][j][k-1] = sum[1];  
      }
    }
  }
  return(fullRadiance);
}

int hitDirectionalSource(double rayLength,int i,int k,double *binWeight,int *rayBin,int *nPulseBins,int nRayBins,double *PSF,RATobj *rat,RATtree *ratTree,RATdevice *suns,RATdevice *camera,RATsample *pixel){
  int hit=1;
  int j,bin;
  int nBands,nSuns,xxx,yyy,index;
  double lambda,x,y,xx,yy;
  triplet old,direction,dd,P;

  if(suns[k].fov[0]>0 && ratTree[k].angleToSun[i] <= suns[k].fovMax){
    old = vector_copy2(ratTree[k].intersectionPoints[i][0],ratTree[k].intersectionPoints[i][1], ratTree[k].intersectionPoints[i][2]);
    direction = normalise(vector_minus(suns[k].focalPoint,old));
    dd = vector_minus(suns[k].focalPoint,suns[k].principalPoint);
    lambda = -V_dot(dd,dd)/V_dot(dd,direction);
    P = vector_plus(dd,V_factor(direction,lambda));
    x = V_dot(P,suns[k].cameraX)/V_dot(suns[k].cameraX,suns[k].cameraX);
    y = V_dot(P,suns[k].cameraY)/V_dot(suns[k].cameraY,suns[k].cameraY);
    xx = (x + suns[k].imagingPlaneDimensions[0]*0.5)/suns[k].imagingPlaneDimensions[0];
    yy = (y + suns[k].imagingPlaneDimensions[1]*0.5)/suns[k].imagingPlaneDimensions[1];
    xxx = (int)(suns[k].nPsf[0]*xx);
    yyy = (int)(suns[k].nPsf[1]*yy);
    if(xxx >= 0 && xxx < suns[k].nPsf[0] && yyy >= 0 && yyy < suns[k].nPsf[1]){
      /* within fov -> set PSF */
      *PSF = suns[k].psf[xxx+yyy*suns[k].nPsf[0]];
      pixel[k].sumRayLength[i]=rayLength+ratTree[k].lengthToSun[i];
      /* implement pulse duration if specified */
      *nPulseBins=getPulseBins(rayBin,binWeight,nRayBins,camera,suns,ratTree[k].lengthToSun[i]+rayLength);
    }else{
      *PSF=1.0;
      hit=0;
      ratTree[k].hitSun[i]=ratTree[k].lengthToSun[i]=0;
    }
  }else{
    hit=0;
  }
  return(hit);
}

int RATunpackRatTree(double modulation,RATobj *rat,RATtree *ratTree,RATdevice *suns,RATdevice *camera,RATsample *pixel,double *norm){
  int i,j,k,hit,bin,nPulseBins,*code=NULL;
  int material,nBands,nSuns,xxx,yyy,index;
  static int *rayBin=NULL,nRayBins=0;
  static double *binWeight=NULL;
  double r,rayLength=0,PSF=1.0,**ptr,thisNorm,**RATgetOrderAddress();
  
  nSuns = RATgetNsuns(rat);
  nBands=pixel[0].nBands;
  if((nRayBins==0 && rayBin==NULL) || nRayBins<camera->nBins+2){
    if(nRayBins<camera->nBins+2){
      if(rayBin)free(rayBin);
      if(binWeight)free(binWeight);
    }
    /*if(suns->cumulativePulse){
      nRayBins=camera->nBins+2; 
    }else{
      nRayBins=1;
    }*/
    nRayBins=camera->nBins+2; /* the maximum */
    rayBin=(int *)v_allocate(nRayBins,sizeof(int));
    binWeight=(double *)v_allocate(nRayBins,sizeof(double));
    if(camera->order != NULL){
      if(camera->order->shaded == NULL){
       camera->order->shaded=(double **)v_allocate(nRayBins,sizeof(double *));
       camera->order->sunlit=(double **)v_allocate(nRayBins,sizeof(double *));
       {int z;
        for(z=0;z<nRayBins;z++){
         camera->order->shaded[z]=(double *)v_allocate(camera->order->m,sizeof(double));
         camera->order->sunlit[z]=(double *)v_allocate(camera->order->m,sizeof(double));
        }
       }
      }
    }
  }else{
    for(i=0;i<nRayBins;i++){rayBin[i]=0;binWeight[i]=0.;}
  }
  for(k=0;k<nSuns;k++){
    for(i=0;i<nBands*(camera->nBins > 0 ? 2+camera->nBins : 1);i++)pixel[k].sumImage[i]=0.;	
    for(i=0;i<=ratTree->thisRTD;i++){
      pixel[k].sumRayLength[i]=0.;
      pixel[k].hit[i]=0.;
    }
    if(ratTree[k].thisRTD>=0){
      rayLength=0.;
      /* unpack rat */
      for(i=0;i<=ratTree[k].thisRTD;i++){
        rayLength += ratTree[k].rayLengths[i];
	if(ratTree[k].hitSun[i]){
          if(ratTree[k].lengthToSun)
            hit=hitDirectionalSource(rayLength,i,k,binWeight,rayBin,&nPulseBins,nRayBins,&PSF,rat,ratTree,suns,camera,pixel);
          else{
            PSF=1.0;
            hit=1;
	    pixel[k].sumRayLength[i]=rayLength;
	    nPulseBins=getPulseBins(rayBin,binWeight,nRayBins,camera,suns,rayLength);
    	  }

          if(hit==1)for(j=0;j<nBands;j++){
	    for(bin=0;bin<nPulseBins;bin++){
              thisNorm = PSF*binWeight[bin]*modulation;
              r =  thisNorm*ratTree[k].directRadiance[i*nBands+j];
              camera->directRadiance[rayBin[bin]][j][i] += r;
	      pixel[k].sumImage[j+nBands*rayBin[bin]] += r;
              /* i is scattering order, j is band, rayBin[bin] is ray bin */
              if(camera->order){
                code=decodeDirectPath(i,rat,&ratTree[k],camera->order);
                ptr = RATgetOrderAddress(&index,i+1,code,camera->order->m,camera->order->nbands,&(camera->order[rayBin[bin]]));
                if(ptr)
                 *(ptr[0]) += r;
	      }
	    }		
	  }
        }else{
          /* shadow */
          hit=-1;
          PSF=1.0; /* this doesnt make much sense for a directional source with PSF variation */
          pixel[k].sumRayLength[i]=rayLength;
          nPulseBins=getPulseBins(rayBin,binWeight,nRayBins,camera,suns,rayLength);
        }
        if(i==0){
         if(camera->order != NULL){
          material = searchForThisMat(rat,camera->order,ratTree[0].ratmat[0],ratTree[0].directPathIntersectType[0]);
          if(material>=camera->order->m){
            fprintf(stderr,"out of bound material determined\n");
          } 
          for(bin=0;bin<nPulseBins;bin++){
           r = binWeight[bin]*modulation*PSF;
           switch(hit){
            case -1:
            case 0:
            if(camera->order->shaded[rayBin[bin]])camera->order->shaded[rayBin[bin]][material]+=r;   
            break;
            case 1:
            if(camera->order->sunlit[rayBin[bin]])camera->order->sunlit[rayBin[bin]][material]+=r;
            break;
           }
          }
	  camera->order->matcount+=modulation*PSF;
         }
        }else if(i==0 && hit<0){
          camera->order->matcount+=modulation*PSF;
        }
     }
     /* diffuse - only comes in at i = ratTree[k].thisRTD */
     i = ratTree[k].thisRTD;
     bin=0;
     if(i>=0){
       for(j=0;j<nBands;j++){
        thisNorm = modulation;
        r =  ratTree[k].diffuseRadiance[i*nBands+j]*thisNorm;
        camera->diffuseRadiance[bin][j][i] += r;
        /* only put diffuse into image for non-lidar mode*/
        if(camera->nBins == 0)pixel[k].sumImage[j] += r;
       }  
       if(camera->order){
         code=decodeDiffusePath(i,rat,&ratTree[k],camera->order);
         ptr = RATgetOrderAddress(&index,i+1,code,camera->order->m,camera->order->nbands,camera->order);
         if(ptr)
          *(ptr[1]) += r;
       } 
     }
    }else{
      if(camera->order)camera->order->matcount+=modulation*PSF;
      /* hit sky  which is zero-oredr scattering */
      return(0);
    }
  }
  norm[0] += PSF*modulation;
  norm[1] += modulation;
  return(1);
}



int getFields(char *str,char *oToken,char fields[MAXFIELDS][128]){
  char *Str,*Str2;
  int i=0,j,k;char *ind,*ind1,*ind2;
  char **ap,*strings[1024],**token;
  int n=-1,ret,isBlank=0;
  
  
  if(str){
    oToken[0]='\0';
    Str=&str[0];
    i++;
    if((ind=index(Str,'#'))!=NULL)*ind='\0';
    /* check for blank string */
    isBlank=1;
    for(i=0;i<strlen(Str);i++){
      if(Str[i] != ' ' && Str[i] != '\t' && Str[i] != '\n'){
          isBlank=0;
	  break;
      }
    }
    char **tmpStr=&Str;
    if(isBlank == 1)
      return(0);
    for (j=0,ap = strings; (*ap = strsep(tmpStr,"=")) != NULL;j++){
      if(strlen(*ap)>0){
        if(j==0){
          Str2=*ap;
          for(token=ap;(*token=strsep(&Str2," \t")) != NULL;)
            if(strlen(*token)>0){
              strcpy(oToken,*token);
              n=0;
              break;
            }
        }else if(n>=0 && n<MAXFIELDS){
          /* remove trailing white space */
          Str2=*ap;
          while(Str2 && strlen(Str2)>0){
            while(Str2[0]==' ' || Str2[0] == '\t')Str2++;
            /* find enclosed quotes */
            if(Str2[0] == '"' && (ind1=index(Str2,'"')) != NULL){
              ind1++;
              if((ind2=index(ind1,'"')) != NULL){
                k=strlen(ind1)-strlen(ind2);
                ind1[k]='\0';
                strcpy(&fields[n][0],ind1);
                n++;
                Str2 = ind2;
              }
            }else if(Str2[0] == ',' || Str2[0] == ' '  || Str2[0] == '\t') {
	      Str2++;
            }else if(Str2[0] == '#'){
	      return(n);
            }else{
	      /* return rest of string or up to , or # */
	      if((ind1 = index(Str2,',')) != NULL || (ind1 = index(Str2,'#')) != NULL){
		k = strlen(Str2)-strlen(ind1);
		ind1++;
		Str2[k] = '\0';
		ret=0;
		for(i=strlen(Str2)-1;i>=0;i--){
		  if(Str2[i]==';'){ret=1;Str2[i]='\0';}
		  if(Str2[i]==' ' || Str2[i] == '\t')Str2[i]='\0';
		}
		if(strlen(Str2)>0){
		  strcpy(&fields[n][0],Str2);
		  n++;
		}
		if(ret)return(n);
		Str2 = ind1;
	      }else{
		ret=0;
		for(i=strlen(Str2)-1;i>=0;i--){
		  if(Str2[i]==';'){ret=1;Str2[i]='\0';}
		  if(Str2[i]==' ' || Str2[i] == '\t')Str2[i]='\0';
		}
		if(strlen(Str2)>0){
		  strcpy(&fields[n][0],Str2);
		  n++;
		}
		if(ret)return(n);
		Str2 += strlen(Str2);
	      }
            }
          }
	  
        }
      }
      if (**ap != '\0')
	if (++ap >= &strings[1024])
	  break;
    }
  }
  return(n);
}

/* awkward dealing with this -- store in rat */
RATdevice  *RATreadCameraFile(char *file,RATobj *rat){
  RATdevice *ratCamera=NULL;
  ratCamera = (RATdevice *)v_allocate(1,sizeof(RATdevice));
  RATreadACameraFile(file,rat,ratCamera);
  return ratCamera;
}

RATdevice *RATreadCameraFiles(char **file,int nfiles,RATobj *rat,int type){
  int i;
  RATdevice *ratCamera=NULL,**store=NULL;
  ratCamera = (RATdevice *)v_allocate(nfiles,sizeof(RATdevice));
  for(i=0;i<nfiles;i++){
    RATreadACameraFile(file[i],rat,(&ratCamera[i]));
  }
  /* store it */
  if(!rat->store){
    rat->store = (void **)v_allocate(2,sizeof(RATdevice *));
  }
  store = (RATdevice **)rat->store;

  if (store[type]) free(store[type]);
  store[type] = (RATdevice *)v_allocate(nfiles,sizeof(RATdevice));
  for(i=0;i<nfiles;i++){
    store[type][i] = ratCamera[i];
  }
  rat->nstore = nfiles;

  return ratCamera;
}

RATdevice  *RATreadACameraFile(char *file,RATobj *rat,RATdevice *ratCamera){
  double zoom=0.,cameraLocation[3] = {0,0,0},lookAt[3];
  double focalLength = 0,zenith=0.0,azimuth=0.,twist=0.,angles[3];
  double imagingPlaneDimensions[2] = {-1,-1};
  int nPixels = -1,rpp=1,dolookat=0,nRows=-1,nCols=-1,cameralocationset=0;
  int getFields();
  double *gaussian=NULL; 
  int size[2]={-1,-1},nBins=0;
  double boomLength = -1, aspectRatio,binStart=0,binStep=1.0,sizesd[2] = {-1,-1},centre[2]={0,0},thresh=1e-10, areaToBeViewed=0, idealArea = 0;
  FILE *fp;
  char filename[1024] = "stdin",*samplingImage=NULL;
  int nf=0,i,doidealarea=0;
  char token[128];
  char fields[MAXFIELDS][128];
  char str[1024],nofile[] = "-",*samplingPattern=NULL;
  int format = 0,nBands;
  WavebandBag *wavebandbag;
  int nScatteredImages=0;
  char **scatteredImages=NULL;
  int *scatterStart=NULL,orthographic=1;	
  int *scatterEnd=NULL;
  nBands = RATgetNWavebands(rat,NULL);
  if(!file || strcmp(filename,file)==0){
    file = nofile;
    fp = stdin;	
  }else{
    if(!(fp=(FILE *)fopen(file,"r"))){
      fprintf(stderr,"RATreadCameraFile: error opening file %s\n",file);
      exit(1);
    }
  }
  if(!ratCamera)ratCamera = (RATdevice *)v_allocate(1,sizeof(RATdevice));
  ratCamera->filename = (char *)v_allocate(strlen(file)+1,sizeof(char));
  strcpy(ratCamera->filename,file);
  ratCamera->nBands = nBands;
  wavebandbag = rat->wavebandbag;
  ratCamera->wavebandbag = wavebandbag;
  ratCamera->nOrders = rat->flagbag->max_ray_depth;
  ratCamera->resultIntegralFilename=NULL;
  ratCamera->resultIntegralFormat=RESULT_INTEGRAL_FORMAT_ASCII;
  ratCamera->resultIntegralMode=RESULT_INTEGRAL_MODE_WAVEBAND;
  ratCamera->resultIntegralCompress=0;
  
  /* read the first line */
  if(fgets(str,1024,fp)){
    nf=getFields(str,token,fields);
    if(!(strcasecmp(token,"camera"))){
      format = 1;	
    }
  }else 
    return(NULL);
  
  if(format == 1)while(fgets(str,1024,fp)){
    if((nf=getFields(str,token,fields))>0){
      if(!strcasecmp(token,"camera.name")){
				/* name field */
	ratCamera->name = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->name,fields[0]);
      }else if(!strcasecmp(token,"geometry.fieldOfView")){
	focalLength = -1 * atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.idealArea") || !strcasecmp(token,"geometry.dimensionToView")){
	doidealarea = 1;
	if(nf<1){
	  fprintf(stderr,"error specifying idealArea option in RATreadCameraFile()\n\t 1 coordinates required %d given\n",nf);
	  exit(1);
	}
	idealArea = atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.boomLength")){
	boomLength =  atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.perspective")){
        focalLength = 0;
        orthographic = 0;
      }else if(!strcasecmp(token,"geometry.orthographic")){
	focalLength = 0;
        orthographic = 1;
      }else if(!strcasecmp(token,"geometry.diffuse")){
        focalLength = 0;
        orthographic = -1;
      }else if(!strcasecmp(token,"geometry.focalLength")){
	focalLength = atof(fields[0]);
        orthographic=0;
      }else if(!strcasecmp(token,"geometry.zoom")){
	zoom = atof(fields[0]);
        orthographic=0;
      }else if(!strcasecmp(token,"geometry.azimuth")){
	azimuth = atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.zenith")){
	zenith = atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.twist")){
	twist = atof(fields[0]);
      }else if(!strcasecmp(token,"geometry.lookAt")){
	dolookat = 1;
	if(nf<3){
	  fprintf(stderr,"error specifying lookAt option in RATreadCameraFile()\n\t 3 coordinates required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<3;i++){
	  lookAt[i] = atof(fields[i]);
	}
      }else if(!strcasecmp(token,"geometry.location")){
	if(nf<3){
	  fprintf(stderr,"error specifying location option in RATreadCameraFile()\n\t 3 coordinates required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<3;i++){
	  cameraLocation[i] = atof(fields[i]);
	}
	cameralocationset=1;
      }else if(0 && !strcasecmp(token,"spectral.wavebands")){
        /* THIS REMOVED ... YOU NEED TO SPECIFUY ON THE CND LINE */
	ratCamera->wavebandFiles = (char **)v_allocate(MAX(1,nf),sizeof(char *));
	ratCamera->nWavebandFiles = MAX(1,nf);
				/* load wavebands */
	/*for(i=0;i<wavebandbag->sensor_wavebands->no_of_wavebands;i++){
	  if(wavebandbag->sensor_filenames[i])free(wavebandbag->sensor_filenames[i]);
	}*/
	for(i=0;i<nf;i++){
	  ratCamera->wavebandFiles[i] = (char *)v_allocate(strlen(fields[i])+1,sizeof(char));
	  strcpy(ratCamera->wavebandFiles[i],fields[i]);
	  wavebandbag->sensor_filenames[i]=ratCamera->wavebandFiles[i];
	  if(!expand_filename(&(wavebandbag->sensor_filenames[i]),"RSRLIB",FALSE))error2("ratlib:\terror opening sensor relative spectral response file",wavebandbag->sensor_filenames[i]);
	}
	wavebandbag->sensor_wavebands->no_of_wavebands=MAX(1,nf);
	wavebandbag->rsr_flag=1;
	read_spectral_file(rat->flagbag->verbose,wavebandbag->sensor_filenames,wavebandbag->rsr_flag,&(rat->flagbag->fixed_wavelength),wavebandbag->sensor_wavebands);		
	calculate_reflectance_data(rat->wavebandbag, rat->materialbag, rat->illumination, *rat->wavebandbag->lambda_min_Ptr, *rat->wavebandbag->lambda_width_Ptr, 0); 
	nBands=ratCamera->nBands=rat->globalnBands=rat->wavebandbag->sensor_wavebands->no_of_wavebands;		
      }else if(!strcasecmp(token,"samplingPattern.form")){
	samplingPattern = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(samplingPattern,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.centre")){
	if(nf<2){
	  fprintf(stderr,"error specifying samplingPattern.centre option in RATreadCameraFile()\n\t 2 floats required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<2;i++){
	  centre[i] = atoi(fields[i]);
	}
      }else if(!strcasecmp(token,"samplingPattern.size")){
	if(nf<2){
	  fprintf(stderr,"error specifying samplingPattern.size option in RATreadCameraFile()\n\t 2 integers required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<2;i++){
	  size[i] = atoi(fields[i]);
	}
      }else if(!strcasecmp(token,"samplingPattern.IPImage")){
	ratCamera->inputImage = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->inputImage,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.sampleCountImage")){
	ratCamera->sampleCountImage = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->sampleCountImage,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.OPImage")){
	ratCamera->outputImage = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->outputImage,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.gridMap")){
	ratCamera->gridMap = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->gridMap,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.modulationMap")){
	ratCamera->modulationMap = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->modulationMap,fields[0]);
      }else if(!strcasecmp(token,"result.nScatteredImages")){
	if(nScatteredImages>0){
	  {
	    int i;
	    if(scatteredImages){
	      for(i=0;i<nScatteredImages;i++)if(scatteredImages[i]){free(scatteredImages[i]);scatteredImages[i]=NULL;}
	      free(scatteredImages);
	      scatteredImages=NULL;
	    }
	    if(scatterStart)free(scatterStart);
	    if(scatterEnd)free(scatterEnd);
	  }
	}
	nScatteredImages = atoi(fields[0]);
				/* allocate nScatteredImages images structure */
	scatteredImages = (char **)v_allocate(nScatteredImages,sizeof(char *));
	scatterStart = (int *)v_allocate(nScatteredImages,sizeof(int));
	scatterEnd = (int *)v_allocate(nScatteredImages,sizeof(int));
      }else if(!strcasecmp(token,"result.scatteredImage")){
	if(1+atoi(fields[0])>nScatteredImages){
	  int i;
	  if(scatteredImages){
	    for(i=0;i<nScatteredImages;i++)if(scatteredImages[i]){free(scatteredImages[i]);scatteredImages[i]=NULL;}
	    free(scatteredImages);
	    scatteredImages=NULL;
	  }
	  if(scatterStart)free(scatterStart);
	  if(scatterEnd)free(scatterEnd);
	  nScatteredImages=1+atoi(fields[0]);
	}
	if(!nScatteredImages)nScatteredImages=MAX(1,1+atoi(fields[0]));
	if(!scatteredImages)scatteredImages = (char **)v_allocate(nScatteredImages,sizeof(char *));
	if(!scatterStart)scatterStart = (int *)v_allocate(nScatteredImages,sizeof(int));
	if(!scatterEnd)scatterEnd = (int *)v_allocate(nScatteredImages,sizeof(int));
	scatteredImages[atoi(fields[0])]=(char *)v_allocate(strlen(fields[1])+1,sizeof(char));
	strcpy( scatteredImages[atoi(fields[0])], fields[1]);	
	if(nf>2)scatterStart[atoi(fields[0])] = atoi(fields[2]);else scatterStart[atoi(fields[0])]=0;
	if(nf>3)scatterEnd[atoi(fields[0])] = atoi(fields[3]);else scatterEnd[atoi(fields[0])]=0;
	
      }else if(!strcasecmp(token,"result.integral.mode")){
	if(!strcasecmp(fields[0],"scattering order")){
	  ratCamera->resultIntegralMode=RESULT_INTEGRAL_MODE_SCATTERINGORDER;
	}else if(!strcasecmp(fields[0],"waveband")){
	  ratCamera->resultIntegralMode=RESULT_INTEGRAL_MODE_WAVEBAND;
	}else if(!strcasecmp(fields[0],"distance")){
	  ratCamera->resultIntegralMode=RESULT_INTEGRAL_MODE_DISTANCE;
	}else{
	  fprintf(stderr,"parse error in field result.integral.mode: token %s not recognised\n",fields[0]);
	  exit(1);
	}
      }else if(!strcasecmp(token,"result.integral")){
	if(ratCamera->resultIntegralFilename)free(ratCamera->resultIntegralFilename);
	if(ratCamera->resultIntegralFp)fclose(ratCamera->resultIntegralFp);
	ratCamera->resultIntegralFilename=v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->resultIntegralFilename,fields[0]);
	if(!(ratCamera->resultIntegralFp=(FILE *)fopen(ratCamera->resultIntegralFilename,"w"))){
	  fprintf(stderr,"error at field result.integral: cannot open file %s for writing\n",fields[0]);
	  exit(1);
	} 
      }else if(!strcasecmp(token,"result.integral.format")){
	if(!strcasecmp(fields[0],"ascii")){
	  ratCamera->resultIntegralFormat=RESULT_INTEGRAL_FORMAT_ASCII;
	}else if(!strcasecmp(fields[0],"binary")){
	  ratCamera->resultIntegralFormat=RESULT_INTEGRAL_FORMAT_BINARY;
	}else{
	  fprintf(stderr,"parse error in field result.integral.format: token %s not recognised\n",fields[0]);
	  exit(1);
	}
      }else if(!strcasecmp(token,"result.integral.compress")){
	if(!strcasecmp(fields[0],"true")){
	  ratCamera->resultIntegralCompress=1;
	}else{
	  ratCamera->resultIntegralCompress=0;
	}
      }else if(!strcasecmp(token,"result.image")){
	ratCamera->resultImage = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->resultImage,fields[0]);
      }else if(!strcasecmp(token,"result.samplingImage")){
	ratCamera->resultPsfImage = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->resultPsfImage,fields[0]);
      }else if(!strcasecmp(token,"samplingPattern.read")){
	if(!strcasecmp(fields[0],"TRUE"))ratCamera->PSFRead=1;
	else ratCamera->PSFRead=0;
      }else if(!strcasecmp(token,"samplingPattern.threshold")){
	thresh = atof(fields[0]);
      }else if(!strcasecmp(token,"samplingCharacteristics.nPixels")){
	nPixels = atoi(fields[0]);
      }else if(!strcasecmp(token,"samplingCharacteristics.nRows")){
	nRows = atoi(fields[0]);
      }else if(!strcasecmp(token,"samplingCharacteristics.nCols")){
	nCols = atoi(fields[0]);
      }else if(!strcasecmp(token,"samplingCharacteristics.rpp")){
	rpp = atoi(fields[0]);
      }else if(!strcasecmp(token,"lidar.nBins")){
	nBins = atoi(fields[0]);
      }else if(!strcasecmp(token,"lidar.binStart")){
	binStart = atof(fields[0]);
      }else if(!strcasecmp(token,"lidar.binStep")){
	binStep = atof(fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseStart")){
	ratCamera->pulseStart = atof(fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseSD")){
	ratCamera->pulseSD = atof(fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseSamples")){
	ratCamera->nPulse=ratCamera->pulseSamples = atoi(fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseLength")){
	ratCamera->pulseLength = atof(fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseForm")){
	ratCamera->pulseType = (char *)v_allocate(strlen(fields[0])+1,sizeof(char));
	strcpy(ratCamera->pulseType,fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseOPFile")){
	ratCamera->pulseOPFile = (char *)v_allocate(strlen(fields[0]),sizeof(char));
	strcpy(ratCamera->pulseOPFile,fields[0]);
      }else if(!strcasecmp(token,"lidar.pulseIPFile")){
	ratCamera->pulseIPFile = (char *)v_allocate(strlen(fields[0]),sizeof(char));
	strcpy(ratCamera->pulseIPFile,fields[0]);
      }else if(!strcasecmp(token,"geometry.imagingPlaneDimensions")){
	if(nf<2){
	  fprintf(stderr,"error specifying imagingPlaneDimensions option in RATreadCameraFile()\n\t 2 floats required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<2;i++){
	  imagingPlaneDimensions[i] = atof(fields[i]);
	}
      }else if(!strcasecmp(token,"samplingPattern.sd")){
	if(nf<2){
	  fprintf(stderr,"error specifying samplingPattern.sd option in RATreadCameraFile()\n\t 2 floats required %d given\n",nf);
	  exit(1);
	}
	for(i=0;i<2;i++){
	  sizesd[i] = atoi(fields[i]);
	}
      }else{
	fprintf(stderr,"RATreadCameraFile: unrecognised camera file option: %s\n",token);
	exit(1);
      }
    }
  }
  /* check that PSFRead is reasonable */
  if(ratCamera->PSFRead){
    if(!ratCamera->modulationMap || !ratCamera->gridMap || !ratCamera->sampleCountImage){
      fprintf(stderr,"incorrect setting of PSFRead flag: modulationMap, gridMap & sampleCountImage must be set & exist to use this\n");
      ratCamera->PSFRead=0;
    }
  }
  
  /* 
  ** imagingPlaneDimensions may not be specified (e.g. if
  ** doidealarea), so work it out now
  */
  if(rat->flagbag->rowsandcols){
    /* check to see if there is a cmd line specification of size */
    nRows = rat->flagbag->rowsandcols[0];
    nCols = rat->flagbag->rowsandcols[1];
  }
  if(nPixels == -1){
    if(nRows > 0 && nCols > 0){
      nPixels = nRows * nCols;
    }else{
      nPixels = nRows = nCols = 1;
    }
  }
  
  if(dolookat){
    /* check to see if camera location is defined */
    if(cameralocationset){
      {
	double m=0,v[3];
	for(i=0;i<3;i++){v[i] = -(lookAt[i] - cameraLocation[i]);m+=v[i]*v[i];}
	boomLength = sqrt(m);
	if(m>0)for(i=0;i<3;i++){v[i] /= boomLength;}
	zenith = RTOD(acos(v[2]));
	azimuth = RTOD(atan2(v[0],-v[1]));
      }
    }else if(boomLength > 0){
      /* work out camera location */
      {
	triplet vector_copy2(),Vx,Vz,F;
	
	Vz = vector_copy2(0.,0.,1.0);
	Vx = vector_copy2(1.,0.,0.);
	Vx = rotate_about(Vx,Vz,azimuth);
	F = rotate_about(Vz,Vx,zenith); 
	cameraLocation[0] = lookAt[0] + boomLength * F.x;
	cameraLocation[1] = lookAt[1] + boomLength * F.y;
	cameraLocation[2] = lookAt[2] + boomLength * F.z;
      }
    }else if(boomLength == -1 && focalLength == 0 && zoom == 0){ /* orthographic */
      ;
    }else{
      fprintf(stderr,"RATreadCameraFile: in file %s:\n\terror in camera specification. geometry.lookAt location is specified, but to use this, either specify geometry.boomLength, geometry.zenith, geometry.azimuth OR geometry.location\n",file);
    }
  }else if(boomLength == -1 && focalLength == 0 && zoom == 0){ /* orthographic */
    /* requires setting of camera location if lookat not set */
    if(cameralocationset){
      {
        triplet vector_copy2(),Vx,Vz,F;
        double boomLength=0.;

        Vz = vector_copy2(0.,0.,1.0);
        Vx = vector_copy2(1.,0.,0.);
        Vx = rotate_about(Vx,Vz,azimuth);
        F = rotate_about(Vz,Vx,zenith);
        /* set an effective boomlength for z = 0 */
        boomLength = (F.z != 0 ? cameraLocation[2] /F.z : 1000.0);
        lookAt[0] =  cameraLocation[0] -  boomLength * F.x;
        lookAt[1] = cameraLocation[1] -  boomLength * F.y;
        lookAt[2] = cameraLocation[2] - boomLength * F.z;
        dolookat=1;        
      }
    }else{
      /* hmmm ... no location , no lookat ... lets hope its a ligth object ... */
      ;
    }
  }
  if(zoom>0){
    { double fov;
    /* fov */
    fov = RTOD(2.*atan2(1.,zoom));
    focalLength = -fov;
    }
  }
  if(doidealarea){
    if(orthographic==1 || orthographic == -1){ /* orthographic */
      imagingPlaneDimensions[0] = idealArea;
      imagingPlaneDimensions[1] = fabs(idealArea * cos(DTOR(zenith)));
    }else if(boomLength>0 && zoom == 0 && focalLength == 0){
      imagingPlaneDimensions[0] = 1.0;
      imagingPlaneDimensions[1] = fabs(imagingPlaneDimensions[0]*cos(DTOR(zenith)));
      if(orthographic==0)focalLength = boomLength/idealArea;
    }else if(focalLength<0 || zoom > 0){ /* fov: convert to focalLength */
      imagingPlaneDimensions[0] = 1.0;
      imagingPlaneDimensions[1] = fabs(imagingPlaneDimensions[0]*cos(DTOR(zenith)));
      boomLength = idealArea*0.5/tan(DTOR(-focalLength/2.));
      focalLength = 2.*tan(DTOR(-focalLength/2.));
    }else if(orthographic==1 || orthographic== -1){ /* orthographic */
      imagingPlaneDimensions[0] = idealArea;
      imagingPlaneDimensions[1] = fabs(idealArea * cos(DTOR(zenith)));
    }else if(boomLength>0 && (focalLength>0 || zoom > 0)){ /* focal length */
      /* need boomLength to work out area */
      imagingPlaneDimensions[0] = focalLength * idealArea/ boomLength;
      imagingPlaneDimensions[1] = fabs(focalLength * idealArea* cos(DTOR(zenith))) / boomLength;
    }
  }else if(focalLength<0){ /* fov definition */
    orthographic=0;
    if(imagingPlaneDimensions[0] < 0){
      imagingPlaneDimensions[0] = 1.0;
    }
    focalLength =  fabs((0.5*imagingPlaneDimensions[0])/tan(DTOR(-focalLength/2.)));
    /* see if there is any AR defined from rows / cols */
    aspectRatio = 1.0;
    if(imagingPlaneDimensions[1] > 0){
      aspectRatio = imagingPlaneDimensions[0]/imagingPlaneDimensions[1];
    }else if(nRows>0 && nCols != nRows)
      aspectRatio = nCols/(double)nRows;
    imagingPlaneDimensions[1] =  imagingPlaneDimensions[0]/aspectRatio; 
    /* set location */
    if(dolookat && boomLength>0){
    {
       triplet vector_copy2(),Vx,Vz,F;

        Vz = vector_copy2(0.,0.,1.0);
        Vx = vector_copy2(1.,0.,0.);
        Vx = rotate_about(Vx,Vz,azimuth);
        F = rotate_about(Vz,Vx,zenith);
        cameraLocation[0] = lookAt[0] +  boomLength * F.x;
        cameraLocation[1] = lookAt[1] +  boomLength * F.y;
        cameraLocation[2] = lookAt[2] + boomLength * F.z;
    }
    }else if(cameralocationset){
    {
        triplet vector_copy2(),Vx,Vz,F;
        double boomLength=0.;

        Vz = vector_copy2(0.,0.,1.0);
        Vx = vector_copy2(1.,0.,0.);
        Vx = rotate_about(Vx,Vz,azimuth);
        F = rotate_about(Vz,Vx,zenith);
        /* set an effective boomlength for z = 0 */
        boomLength = (F.z != 0 ? cameraLocation[2] /F.z : 1000.0);
        lookAt[0] =  cameraLocation[0] -  boomLength * F.x;
        lookAt[1] = cameraLocation[1] -  boomLength * F.y;
        lookAt[2] = cameraLocation[2] - boomLength * F.z;
        dolookat=1;
     }
     }
  }
  aspectRatio = imagingPlaneDimensions[0]/imagingPlaneDimensions[1];
  if(nCols == -1){
    if(nRows > -1){
      nCols = nPixels / nRows;	
    }else{
      nCols = (int)(sqrt(aspectRatio*(double)nPixels)+0.5);
    }
  }
  if(nRows == -1)nRows = nPixels/nCols;
  if(size[0] == -1 || size[0] < nCols || size[1] < nRows){
    size[0] = nCols;
    size[1] = nRows;
  }
  if(samplingPattern && (!strcasecmp(samplingPattern,"gaussian") || !strcasecmp(samplingPattern,"circular"))){
    if(sizesd[0] < 0 ){
      for(i=0;i<2;i++)sizesd[i]=3.0;
    }
    gaussian = RATgenerateGaussian(samplingPattern,size[0],size[1],(1./sizesd[0])*(size[0]-1)*0.5,(1./sizesd[1])*(size[1]-1)*0.5,(size[0]-1)*0.5+centre[0]*(size[0]-1)*0.5/sizesd[0],(size[1]-1)*0.5+(size[1]-1)*0.5*centre[1]/sizesd[1],thresh,1.0);
  }else if(samplingImage){
    ; /* read image in */
  }
  if(rat->flagbag->verbose){
    if(orthographic==0){
     fprintf(stderr,"camera %s: at %lf %lf %lf focalLength: %lf\n", ratCamera->name,cameraLocation[0],cameraLocation[1],cameraLocation[2],focalLength);
    }else if (orthographic==1){ /* orthographic */
     fprintf(stderr,"camera %s: orthographic at  %lf %lf %lf looking at  %lf %lf %lf angles: zenith %lf azimuth %lf twist %lf\n",ratCamera->name,cameraLocation[0],cameraLocation[1],cameraLocation[2],lookAt[0],lookAt[1],lookAt[2],zenith,azimuth,twist);
    }else if (orthographic==-1){ /* diffuse */
     fprintf(stderr,"camera %s: diffuse at  %lf %lf %lf looking at  %lf %lf %lf angles: zenith %lf azimuth %lf twist %lf\n",ratCamera->name,cameraLocation[0],cameraLocation[1],cameraLocation[2],lookAt[0],lookAt[1],lookAt[2],zenith,azimuth,twist);
    }
  } 
  if(ratCamera->pulseLength){
    /* define a pulse */
    if(ratCamera->pulseIPFile){
      /* read pulse file */
      {
	FILE *fp;
	int i=0;
	char buffer[MAX_STRING_LENGTH];
	
	if(!(fp=fopen(ratCamera->pulseIPFile,"r"))){
	  fprintf(stderr,"RATreadCameraFile: error opening input pulse file (pulseIPFile) %s\n",ratCamera->pulseIPFile);
	  exit(1);
	}
	while(fgets(buffer,1024,fp))i++;
	ratCamera->pulseSamples = i;
	rewind(fp);
	ratCamera->pulse = (double *)v_allocate(ratCamera->pulseSamples,sizeof(double));
	i=0;
	while(fgets(buffer,1024,fp))sscanf(buffer,"%lf",&(ratCamera->pulse[i++]));
	fclose(fp);
      }
    }else if(ratCamera->pulseType && !strcasecmp(ratCamera->pulseType,"gaussian")){
      /* generate a gaussian */
      if(ratCamera->pulseSD<=0)ratCamera->pulseSD=3.0;
      ratCamera->pulse = RATgenerateGaussian("gaussian",ratCamera->pulseSamples,1,(ratCamera->pulseSamples-1)*0.5/ratCamera->pulseSD,1e20,(ratCamera->pulseSamples-1)*0.5,0,0,1.0);
    }else{
      /* square pulse */
      ratCamera->pulse = (double *)v_allocate(ratCamera->pulseSamples,sizeof(double));
      for(i=0;i<ratCamera->pulseSamples;i++)ratCamera->pulse[i]=1.0;
    }
    if(ratCamera->pulseSamples<=0){
      fprintf(stderr,"RATreadCameraFile: error defining number of pulse samples\n");
      exit(1);
    }
    ratCamera->pulseSample = (double *)v_allocate(ratCamera->pulseSamples,sizeof(double));
    ratCamera->pulseStep = ratCamera->pulseLength/(double)(ratCamera->pulseSamples);
    for(i=0;i<ratCamera->pulseSamples;i++)ratCamera->pulseSample[i] = ratCamera->pulseStart + i*ratCamera->pulseStep;
    if(ratCamera->pulseOPFile){
      /* write pulse file */
      {
	FILE *fp;
	int i=0;
	
	if(!(fp=fopen(ratCamera->pulseOPFile,"w"))){
	  fprintf(stderr,"RATreadCameraFile: error opening output pulse file (pulseOPFile) %s\n",ratCamera->pulseOPFile);
	  exit(1);
	}
	for(i=0;i<ratCamera->pulseSamples;i++)
	  fprintf(fp,"%lf\n",ratCamera->pulse[i]);
	fclose(fp);
      }
    }
    /* normalise pulse and provide cumulative pulse */
    ratCamera->cumulativePulse = (double *)v_allocate(ratCamera->pulseSamples,sizeof(double));
    ratCamera->cumulativePulse[0]=ratCamera->pulse[0];
    for(i=1;i<ratCamera->pulseSamples;i++)ratCamera->cumulativePulse[i]=ratCamera->cumulativePulse[i-1]+ratCamera->pulse[i];
    for(i=0;i<ratCamera->pulseSamples;i++){
      ratCamera->pulse[i] /= ratCamera->cumulativePulse[ratCamera->pulseSamples-1];
      ratCamera->cumulativePulse[i] = (i == 0 ? ratCamera->pulse[i] : ratCamera->cumulativePulse[i-1]+ratCamera->pulse[i]);
    }
  }
  
  angles[0] = zenith; 
  angles[1] = azimuth; 
  angles[2] = twist; 
  
  ratCamera = RATloadCamera(ratCamera,orthographic,nBands,nBins,binStart,binStep,cameraLocation,focalLength,angles,rpp,nPixels,imagingPlaneDimensions,gaussian,size);
  /*8if(file != nofile){
    fclose(fp);	
  }*/
  /* load stuff up into ratCamera */
  ratCamera->nScatteredImages=nScatteredImages;
  ratCamera->scatteredImages=scatteredImages;
  ratCamera->scatterStart=scatterStart;
  ratCamera->scatterEnd = scatterEnd;
  return(ratCamera);
}

void RAToutputCameraDriverData(RATdevice *illumination,int nSuns){
  int i;
  void *data;
  int type,rows,cols,frames;
  
  for(i=0;i<nSuns;i++){
    if(illumination[i].sampleCountImage && illumination[i].PSFRead==0){
      /* dump the sampling data as an image */
      data = (void *)RATgetPsfSampleCount(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].sampleCountImage,data,rows,cols,frames,type);
    }
    if(illumination[i].outputImage){
      /* get the psf image you loaded up */
      data = (void *)RATgetPsf(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].outputImage,data,rows,cols,frames,type);	
    }
    if(illumination[i].gridMap && illumination[i].PSFRead==0){
      /* get the psf image map (mapping between psf grid coordinates and output image coordinates: col, row as the 2 frames) */
      data = (void *)RATgetPsfMap(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].gridMap,data,rows,cols,frames,type);
    }
    if(illumination[i].modulationMap && illumination[i].PSFRead==0){
      /* get the psf image map (mapping between psf grid coordinates and output image coordinates: col, row as the 2 frames) */
      data = (void *)RATgetPsfModulation(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].modulationMap,data,rows,cols,frames,type);
    }
  }
  return;
}

GenericImage *putHeader(char *name,FILE *fp,int mode,int format,int  rtd,int nBands,int nBins){
  static char *hd[] = {"waveband", "distance", "scattering order"};
  char str[1024];
  GenericImage *out=NULL;
  int rows,cols,frames;
  char *argv[1];
  
  argv[0]=&str[0];
  switch(mode){
  case RESULT_INTEGRAL_MODE_WAVEBAND:
    rows=nBands;
    cols=nBins;
    frames=rtd;
    sprintf(str,"# integral results, ordered by: %s (rows %d ) %s (cols %d ) %s  (frames %d)",hd[0],rows,hd[1],cols,hd[2],frames);
    break;
  case RESULT_INTEGRAL_MODE_SCATTERINGORDER:
    rows=rtd;
    cols=nBands;
    frames=nBins;
    sprintf(str,"# integral results, ordered by: %s (rows %d ) %s (cols %d ) %s  (frames %d)",hd[2],rows,hd[0],cols,hd[1],frames); 
    break;
  case RESULT_INTEGRAL_MODE_DISTANCE:
    rows=nBins;
    cols=rtd;
    frames=nBands;
    sprintf(str,"# integral results, ordered by: %s (rows %d ) %s (cols %d ) %s  (frames %d)",hd[1],rows,hd[2],cols,hd[0],frames);
    break;
  }
  switch(format){
  case RESULT_INTEGRAL_FORMAT_ASCII:
    fprintf(fp,"%s\n",str);
    break;
  case RESULT_INTEGRAL_FORMAT_BINARY:
    fclose(fp); 
    out=(GenericImage *)v_allocate(1,sizeof(GenericImage)); 
    setImageDefaults(out,FALSE);
    setImageName(out,name);
    setImageFrames(out,frames);
    setImageRows(out,rows);
    setImageCols(out,cols);
    setImageFormat(out,IMAGE_FLOAT);
    if(out->seq_desc)free(out->seq_desc);
    out->seq_desc=(char *)v_allocate(strlen(str)+1);
    strcpy(out->seq_desc,str);
    out->mmap=-1;
    openImage(out,FALSE,NULL);
    break;
  }
  return(out);
}

void putValue(GenericImage *im,int row,int col,int frame,FILE *fp,int mode,double value){
  float fvalue;
  if(im){
    fvalue=value;
    putPixel(im,frame,row,col,(void *)(&fvalue));
  }else{
    fprintf(fp,"%e ",value);
  }
  return;
}

void putReturn(GenericImage *im,FILE *fp,int mode){
  if(!im){
    fprintf(fp,"\n");
  }
}

void RAToutputOrder(RATobj *ratObj,RATdevice *camera,int mode){
  int rtd,nBands,nBins;
  int I,i,j,k,r,c,f;
  int J,K,L;
  int *code=NULL;
  GenericImage *out;
  double **data=NULL,*wavebands=NULL,binstart;
  nBands=camera->nBands;
  nBins=(camera->nBins == 0 ? 1 : camera->nBins + 2);
  rtd=camera->nOrders;
  if(mode==0)code=(int *)v_allocate(camera->order->m,sizeof(int));
  if(RATgetNWavebands(ratObj,wavebands) != nBands){
    fprintf(stderr,"inconsistency in defined number of wavebands in RAToutputIntegral()\n");
    exit(1);
  }
/* header -> ascii */
  for(i=0;i<2;i++){
    if(mode==0)fprintf(camera->order->fp[i],"# ");
    fprintf(camera->order->fp[i],"RATORDER materials= %d maxrtd= %d nbins= %d binstart= %e binstep= %e\n",camera->order->m,camera->order->n,nBins,camera->binStart,camera->binStep);
    for(j=0;j<camera->order->m;j++){
      if(mode==0)fprintf(camera->order->fp[i],"# ");
      fprintf(camera->order->fp[i],"material %d \"%s\" \"%s\"\n",j,camera->order->ratMat[j].name,camera->order->ratMat[j].typeName);
    }
  }
  if(camera->order->sunlit != NULL){
    FILE *fp;
    int i,j;
    double sum=0,n;
    char *filename=NULL,*outputfile;

    outputfile = camera->order->rootfilename;
    filename = (char *)v_allocate(strlen(outputfile)+strlen(".material")+1,sizeof(char));
    sprintf(filename,"%s.material",outputfile);
    if(!(fp=(FILE *)fopen(filename,"w"))){
      fprintf(stderr,"error opening material output file %s\n",outputfile);
      exit(1);
    }
    for(i=0;i<camera->order->m;i++)for(j=0;j<nBins;j++){
      if(camera->order->sunlit[j] && camera->order->shaded[j])sum += camera->order->sunlit[j][i] + camera->order->shaded[j][i];
    }
    n=camera->order->matcount;
    if(n==0)n=1.0;
    fprintf(fp,"# %s %.20e nohit\n","void",(n-sum)/n);
    for(j=0;j<nBins;j++){
      for(i=0;i<camera->order->m;i++){
        fprintf(fp,"%e %s %.20e sunlit %s\n%e %s %.20e shaded %s\n",(j-1)*camera->binStep+camera->binStart,camera->order->ratMat[i].name,camera->order->sunlit[j][i]/n,camera->order->ratMat[i].typeName,(j-1)*camera->binStep+camera->binStart,camera->order->ratMat[i].name,camera->order->shaded[j][i]/n,camera->order->ratMat[i].typeName);
      }
    }
    fflush(fp);
    fclose(fp);
  }
  /* now loop over nBins */
  for(j=0;j<nBins;j++){
    for(i=0;i<2;i++){
      if(mode==0)fprintf(camera->order->fp[i],"# ");
      fprintf(camera->order->fp[i],"BIN ");
      if(mode==0)fprintf(camera->order->fp[i],"%d ",j);
      else fwrite(&j,sizeof(int),1,camera->order->fp[i]);

      binstart = (nBins > 1 ? (j-1)*camera->binStep+camera->binStart : 0 ); 

      if(mode==0)fprintf(camera->order->fp[i],"%e\n",binstart);
      else fwrite(&binstart,sizeof(double),1,camera->order->fp[i]);

      if(mode != 0 ) {
       data = (i == 0 ? camera->order[j].direct : camera->order[j].diffuse );
       /* loop over scattering order */
       for(k=0;k<camera->order[j].nRatorders;k++){
        /* output all information for that scattering order */
        for(I=0;I<camera->order[j].counter[k];I++){
          data[k][I] /= camera->radianceNorm[i];
        }
        fwrite(data[k],sizeof(double),camera->order[j].counter[k],camera->order->fp[i]); 
       }
      }else{
       data = (i == 0 ? camera->order[j].direct : camera->order[j].diffuse );
       for(k=0;k<camera->order[j].nRatorders;k++){
        for(I=0;I<camera->order[j].counter[k];I++){
         if(data && data[k] && data[k][I]>0){
          data[k][I] /= camera->radianceNorm[i];
          for(J=0;J<camera->order[j].m;J++)code[J]= camera->order[j].coder[k][I][J];
          for(J=0;J<camera->order[j].m;J++)fprintf(camera->order->fp[i],"%d ",code[J]);
          fprintf(camera->order->fp[i],"%.32e\n",data[k][I]); 
         }
        }
       }
      }
    }  
  }  
  for(i=0;i<2;i++){fclose(camera->order->fp[i]);for(j=0;j<nBins;j++)camera->order[j].fp[i]=NULL;}
  return;
}

RATdevice *RATloadDummyCamera(int nBands,int nBins,int nOrders,double ***directRadiance,double radianceNorm,int resultIntegralMode,double binStep,double binStart,char *resultIntegralFilename){
/* set resultIntegralFp */
  RATdevice *out=NULL;
  out=(RATdevice *)v_allocate(1,sizeof(RATdevice));
  out->nBands=nBands;
  out->nBins=nBins;
  out->nOrders=nOrders;
  out->directRadiance=directRadiance;
  out->radianceNorm[0]=out->radianceNorm[1]=radianceNorm;
  out->resultIntegralMode=resultIntegralMode;
  out->binStep=binStep;
  out->binStart=binStart;
  out->resultIntegralFilename=resultIntegralFilename;
  if(!(out->resultIntegralFp=(FILE *)fopen(resultIntegralFilename,"w"))){
    fprintf(stderr,"error opening file %s for write\n",resultIntegralFilename);
    exit(0);
  }
  return(out);
}

void RATOutputIntegralFromOrder(char *outputfile,RATorder *orders,int nLidarBins,double *bininfo,double ****radiance,int mode){
  RATdevice *camera=NULL;
  char *filename=NULL;
  RATobj *dummy;
  double *wavelengths,radianceNorm=1.0;

  wavelengths = orders->ratMat->wavelengths;
  dummy = (RATobj *)(-(long)wavelengths);
  filename = (char *)v_allocate(strlen(outputfile)+MAX(MAX(strlen(".material"),strlen(".direct")),strlen(".diffuse"))+1,sizeof(char));
  sprintf(filename,"%s.direct",outputfile);
  camera = RATloadDummyCamera(orders->ratMat->nWavelengths,nLidarBins,orders->n,radiance[0],radianceNorm,mode,bininfo[1],bininfo[0],filename); 
  RAToutputIntegral(dummy,camera);

  free(camera);
  sprintf(filename,"%s.diffuse",outputfile);
  camera = RATloadDummyCamera(orders->ratMat->nWavelengths,0,orders->n,radiance[1],radianceNorm,mode,0,0,filename);
  RAToutputIntegral(dummy,camera);

 /* output material use info for 1st O */
  if(orders->sunlit != NULL){
    FILE *fp;
    int i,j;
    double sum=0,n;

    sprintf(filename,"%s.material",outputfile);
    if(!(fp=(FILE *)fopen(outputfile,"w"))){
      fprintf(stderr,"error opening material output file %s\n",outputfile);
      exit(1);
    }
    for(i=0;i<orders->m;i++)for(j=0;j<nLidarBins;j++){
      sum += orders->sunlit[j][i] + orders->shaded[j][i]; 
    }
    n=orders->matcount;
    if(n==0)n=1.0;
    fprintf(fp,"# %s %.20e nohit\n","void",(n-sum)/n);
    for(j=0;j<nLidarBins;j++){
      for(i=0;i<orders->m;i++){
        fprintf(fp,"%e %s %.20e sunlit %s\n%e %s %.20e shaded %s\n",(j-1)*camera->binStep+camera->binStart,orders->ratMat[i].name,orders->sunlit[j][i]/n,orders->ratMat[i].typeName,(j-1)*camera->binStep+camera->binStart,orders->ratMat[i].name,orders->shaded[j][i]/n,orders->ratMat[i].typeName);
      }
    }
    fclose(fp);
  } 
  free(filename);
  free(camera);
  return;
}

void RAToutputIntegral(RATobj *ratObj,RATdevice *camera){
  int rtd,nBands,nBins;
  int i,j,k,r,c,f,count,N=2;
  GenericImage *out;
  double *wavebands=NULL;
  char *filename=NULL,*filename2=NULL;
  char *filenames[2];
  double ***data[2];
  FILE *fps[2];
  
  nBands=camera->nBands;
  nBins=(camera->nBins == 0 ? 1 : camera->nBins + 2);
  rtd=camera->nOrders;
  if(ratObj>0 && RATgetNWavebands(ratObj,wavebands) != nBands){
    fprintf(stderr,"inconsistency in defined number of wavebands in RAToutputIntegral()\n");
    exit(1);
  }
  if(ratObj>0){
   wavebands=v_allocate(nBands,sizeof(double));
   RATgetNWavebands(ratObj,wavebands);
  }else{
   wavebands=(double *)(-(long)ratObj);
  }
  /* header */
  /*if(camera->resultIntegralFp)fclose(camera->resultIntegralFp);*/
  filename = (char *)v_allocate(strlen(camera->resultIntegralFilename)+MAX(MAX(strlen(".material"),strlen(".direct")),strlen(".diffuse"))+1,sizeof(char));
  filename2 = (char *)v_allocate(strlen(camera->resultIntegralFilename)+MAX(MAX(strlen(".material"),strlen(".direct")),strlen(".diffuse"))+1,sizeof(char));
  sprintf(filename2,"%s.direct",camera->resultIntegralFilename);
  sprintf(filename,"%s.diffuse",camera->resultIntegralFilename);

  camera->resultIntegralFilenameDiffuse=filename;
  camera->resultIntegralFilename = filename2;
  if ( nBins == 1){
        N=2;
  }else{
        N=1;
  }
  if(N==2)
  if(!(camera->resultIntegralFpDiffuse=(FILE *)fopen(filename,"w"))){
    fprintf(stderr,"error opening file %s for write\n",filename);
    exit(0);
  }
  if(!(camera->resultIntegralFp=(FILE *)fopen(filename2,"w"))){
    fprintf(stderr,"error opening file %s for write\n",filename2);
    exit(0);
  }
  
  fps[0] = camera->resultIntegralFp;
  fps[1] = camera->resultIntegralFpDiffuse;
  filenames[0] = camera->resultIntegralFilename;
  filenames[1] = camera->resultIntegralFilenameDiffuse;
  data[0] = camera->directRadiance;
  data[1] = camera->diffuseRadiance;
  for(count=0;count<N;count++){
    out=putHeader(filenames[count],fps[count],camera->resultIntegralMode,camera->resultIntegralFormat,rtd,nBands,nBins);
    if(out){
      /* binary mode */
      for(i=0;i<nBands;i++)
	for(j=0;j<nBins;j++)
	  for(k=0;k<rtd;k++){
	    r=(camera->resultIntegralMode==RESULT_INTEGRAL_MODE_WAVEBAND ? i : (camera->resultIntegralMode==RESULT_INTEGRAL_MODE_DISTANCE? j : k));
	    c=(camera->resultIntegralMode==RESULT_INTEGRAL_MODE_WAVEBAND ? j : (camera->resultIntegralMode==RESULT_INTEGRAL_MODE_DISTANCE? k : i));
	    f=(camera->resultIntegralMode==RESULT_INTEGRAL_MODE_WAVEBAND ? k : (camera->resultIntegralMode==RESULT_INTEGRAL_MODE_DISTANCE? i : j));
	    putValue(out,r,c,f,NULL,0,data[count][j][i][k]/camera->radianceNorm[0]);
	  }
      writeImage(out,((long)ratObj>0 ? ratObj->globalArgc : 0),((long)ratObj>0 ? ratObj->globalArgv : NULL),TRUE,TRUE,NULL);
    }else{
      switch(camera->resultIntegralMode){
      case RESULT_INTEGRAL_MODE_WAVEBAND:
	for(k=0;k<rtd;k++){
	  fprintf(fps[count],"# RTD:\n# %d\n",k+1);
	  fprintf(fps[count],"# Distance (start of bin):\n# ");
	  for(j=0;j<nBins;j++){
	    fprintf(fps[count],"%e ",(j-1)*camera->binStep+camera->binStart);
	  }
	  fprintf(fps[count],"\n");
	  for(i=0;i<nBands;i++){
	    fprintf(fps[count],"%e ",wavebands[i]);
	    for(j=0;j<nBins;j++){
	      fprintf(fps[count],"%e ",data[count][j][i][k]/camera->radianceNorm[0]);
	    }
	    fprintf(fps[count],"\n");
	  } 
	}
	break;
      case RESULT_INTEGRAL_MODE_SCATTERINGORDER:
	for(j=0;j<nBins;j++){
	  fprintf(fps[count],"# Distance (start of bin): %e\n",(j-1)*camera->binStep+camera->binStart);
	  fprintf(fps[count],"# Wavelength:\n# ");
	  for(i=0;i<nBands;i++){
	    fprintf(fps[count],"%e ",wavebands[i]);
	  }
	  fprintf(fps[count],"\n");
	  for(k=0;k<rtd;k++){
	    fprintf(fps[count],"%d ",k+1);
	    for(i=0;i<nBands;i++){
	      fprintf(fps[count],"%e ",data[count][j][i][k]/camera->radianceNorm[0]);
	    }
	    fprintf(fps[count],"\n");
	  }
	}
	
	break;
      case RESULT_INTEGRAL_MODE_DISTANCE:
	for(i=0;i<nBands;i++){
	  fprintf(fps[count],"# Wavelength: %e\n",wavebands[i]);
	  fprintf(fps[count],"# Scattering Order:\n# ");
	  for(k=0;k<rtd;k++){
	    fprintf(fps[count],"%d ",k+1);
	  }
	  fprintf(fps[count],"\n");
	  for(j=0;j<nBins;j++){
	    fprintf(fps[count],"%e ",(j-1)*camera->binStep+camera->binStart);
	    for(k=0;k<rtd;k++){
	      if(data[count][j])fprintf(fps[count],"%e ",data[count][j][i][k]/camera->radianceNorm[0]);
	    }
	    fprintf(fps[count],"\n");
	  }
	}
	break;
      }
    }
    fclose(fps[count]); 
  }
  return;
}

void RAToutputCameraResults(RATobj *ratObj,RATdevice *illumination,int nSuns){
  int i;
  void *data;
  int type,rows,cols,frames;
  
  for(i=0;i<nSuns;i++){
    if(illumination[i].order){
      RAToutputOrder(ratObj,&illumination[i],illumination[i].order->mode);
    }
    if(illumination[i].order == NULL && illumination[i].resultIntegralFilename){
      RAToutputIntegral(ratObj,&illumination[i]); 
    }
    if(illumination[i].resultImage){
      /* dump the sampling data as an image */
      data = (void *)RATgetPsfResultImage(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].resultImage,data,rows,cols,frames,type);
    }
    if(illumination[i].resultPsfImage){
      /* get the psf image you loaded up */
      data = (void *)RATgetResultImage(&illumination[i],&rows,&cols,&frames,&type);
      /* NB this function deallocates data */
      RATputImage(illumination[i].resultPsfImage,data,rows,cols,frames,type);	
    }
  }
  return;
}

void SequenceDoiIt(int nbands,struct RATorderStore *RATorder,int **store,int *c,int m,int n,int *o,int x,unsigned char **coder);
void SequenceDoiIt(int nbands,struct RATorderStore *RATorder,int **store,int *c,int m,int n,int *o,int x,unsigned char **coder){
  int i,j,sum=0,code=0;
  for(i=0;i<=x;i++)sum+=o[i];
  if(x>=m-2){
    o[m-1]=MAX(0,n-sum);
    RATorder->index=*c;
    if(!RATorder->code && !(RATorder->code=(int *)calloc(m,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    for(j=m-1;j>=0;j--){
     RATorder->code[j]=o[j];
     if(coder)coder[*c][j]=o[j];
    }
    (*c)++;
    return;
  }
  if(!RATorder->r && !(RATorder->r=(struct RATorderStore *)calloc(MAX(1,n-sum+1),sizeof(struct RATorderStore)))){
      fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  RATorder->nelements=MAX(1,n-sum+1);
  for(j=0;j<=MAX(0,n-sum);j++){
    o[x+1]=j;
    SequenceDoiIt(nbands,&(RATorder->r[j]),store,c,m,n,o,x+1,coder);
  }
  return;
}

int getRestOfOrder(struct RATorderStore *base,int *code,int level);
int getRestOfOrder(struct RATorderStore *base,int *code,int level){
   if(!base)return(-1);
   if(base->r){
     return(getRestOfOrder(&(base->r[code[level]]),code,level+1));
   }
   return(base->index);
}

int getOrderIndex(int rtorder,int *code,int m,RATorder *order){
  int M,i,ok;
  if(order==NULL || code == NULL)return(-1);
  if(rtorder==0)return(0);

  for(i=0;i<order->counter[rtorder];i++){
   ok=1;
   for(M=0;M<m;M++){
    if(order->coder[rtorder][i][M]!=code[M]){
      ok=0;
      M=m;
    } 
   }
   if(ok==1)
    return(i);
  }
  return(-1);
}

double **RATgetOrderAddress(int *index,int order,int *code,int m,int nbands,RATorder *RATorder){
  static double *out[2];
  int Index=0;
  out[0]=out[1]=NULL;

  Index= getOrderIndex(order,code,m,RATorder);
  /* direct */
  if(Index == -1)return(NULL);
  out[0] = &(RATorder->direct[order][Index]);
  if(RATorder->diffuse)out[1] = &(RATorder->diffuse[order][Index]);

  if(index)*index=Index; 
  return(&out[0]);
}

RATorder *RATsetOrderTerms(double **fdata,RATobj *rat,int *nLidarBins,char *resultIntegralFilename){
 int I,max_ray_depth=1,j,k,l,*o=NULL,i;
 char *Str=NULL,buffer[MAX_STRING_LENGTH],dum[6][MAX_STRING_LENGTH];
 double **data,binstart=0,binstep=0;
 RATmaterials *tmpm,*tmp;
 RATorder *orders=NULL,*order=NULL,*iorder=NULL;

 if(rat)max_ray_depth=rat->flagbag->max_ray_depth;

 /* preliminary if feeding information */
 if(fdata!= NULL){
  order=(RATorder *)v_allocate(1,sizeof(RATorder)); 
  if(resultIntegralFilename){order->rootfilename=(char *)v_allocate(strlen(resultIntegralFilename)+1,sizeof(char)); strcpy(order->rootfilename,resultIntegralFilename);}else{order->rootfilename=(char *)v_allocate(strlen("result.dat")+1,sizeof(char)); strcpy(order->rootfilename,"result.dat");}
  order->filename[0] = (char *)v_allocate(strlen(order->rootfilename)+1+strlen(".direct"),sizeof(char));
  order->filename[1] = (char *)v_allocate(strlen(order->rootfilename)+1+strlen(".diffuse"),sizeof(char));
  sprintf(order->filename[0],"%s.direct",order->rootfilename);
  sprintf(order->filename[1],"%s.diffuse",order->rootfilename);
  for(j=0;j<2;j++){
   if(!(order->fp[j]=(FILE *)fopen(order->filename[j],"r"))){
    fprintf(stderr,"error opening file %s\n",order->filename[j]);
    exit(1);
   }
   if(!fgets(buffer,MAX_STRING_LENGTH-1,order->fp[j])){
    fprintf(stderr,"error reading header line of file %s\n",order->filename[j]);exit(0);
   }
   if(sscanf(buffer,"%s %s %d %s %d %s %d %s %lf %s %lf",&(dum[0][0]),&(dum[1][0]),&(order->m),&(dum[2][0]),&max_ray_depth,&(dum[3][0]),nLidarBins,&(dum[4][0]),&binstart,&(dum[5][0]),&binstep)!=11){
    fprintf(stderr,"error reading from header line of file %s\n",order->filename[j]);exit(0);
   }
   *nLidarBins -= 2;
   if(*nLidarBins < 0)*nLidarBins=-1;
  }
 }
 iorder=order;
 orders=(RATorder *)v_allocate((*nLidarBins == 0 ? 1 : *nLidarBins + 2),sizeof(RATorder));
 if(fdata){
  orders[0].m = iorder->m;
  orders[0].rootfilename=iorder->rootfilename;
  for(j=0;j<2;j++){
   orders[0].filename[j]=iorder->filename[j];
   orders[0].fp[j]=iorder->fp[j];
  }
 }
 for(i=0;i<(*nLidarBins == 0 ? 1 : *nLidarBins + 2);i++){
  order=&(orders[i]);
  if(i==0){
   order->nsamples=(int *)v_allocate(max_ray_depth+1,sizeof(int));
   order->counter=(int *)v_allocate(max_ray_depth+1,sizeof(int));
   order->r=(struct RATorderStore **)v_allocate(max_ray_depth+1,sizeof(struct RATorderStore *));
   order->nRatorders=max_ray_depth+1;
   if(fdata==NULL){
    if(resultIntegralFilename){
     order->rootfilename=(char *)v_allocate(strlen(resultIntegralFilename)+1,sizeof(char)); 
     strcpy(order->rootfilename,resultIntegralFilename);
    }else{
     order->rootfilename=(char *)v_allocate(strlen("result.dat")+1,sizeof(char)); 
     strcpy(order->rootfilename,"result.dat");
    }
    order->filename[0] = (char *)v_allocate(strlen(order->rootfilename)+1+strlen(".direct"),sizeof(char));
    order->filename[1] = (char *)v_allocate(strlen(order->rootfilename)+1+strlen(".diffuse"),sizeof(char));
    sprintf(order->filename[0],"%s.direct",order->rootfilename);
    sprintf(order->filename[1],"%s.diffuse",order->rootfilename);
    for(j=0;j<2;j++){
     if(!(order->fp[j]=(FILE *)fopen(order->filename[j],"w"))){
      fprintf(stderr,"error opening file %s\n",order->filename[j]);
      exit(1);
     }
    }
    /* get the number of materials used */
    order->ratMat = tmp = RATgetMaterialsUsed(rat,&(order->m));
   }else{
     order->ratMat = (RATmaterials *)v_allocate(order->m,sizeof(RATmaterials));
     for(j=0;j<order->m;j++){
      for(l=0;l<2;l++)if(!fgets(buffer,MAX_STRING_LENGTH-1,order->fp[l])){
       fprintf(stderr,"error reading material header line of file %s\n",order->filename[l]);exit(0);
      }
      sscanf(buffer,"%s %d %s %s",&dum[0][0],&order->ratMat[j].index,&dum[1][0],&dum[2][0]);
      Str=dum[1];
      while(Str[0] == '\\' || Str[0] == '"'){
       k=strlen(Str);
       for(l=1;l<=k;l++)
        dum[1][l-1]=dum[1][l];
      }
      k=strlen(dum[1]);
      while(Str[k-1] == '\\' || Str[k-1] == '"'){
        k=strlen(Str);
        Str[k-1]='\0';
      }
      k=strlen(Str);
      order->ratMat[j].name=(char *)v_allocate(k+1,sizeof(char));
      strcpy(order->ratMat[j].name,Str);
      /*order->ratMat->index=j;*/
      order->ratMat[j].used=1;
      order->ratMat[j].type=0;
      order->ratMat[j].nWavelengths=(int)(fdata[0][0]+0.5);
      order->ratMat[j].wavelengths=fdata[1];
      if(j<=(int)(fdata[0][1]+0.5))
       order->ratMat[j].diffuse_reflectance=fdata[j+2];
      else 
       order->ratMat[j].diffuse_reflectance=(double *)v_allocate(order->ratMat[j].nWavelengths,sizeof(double)); 
     }
   }
   /* now we need to double count the ones with refl & transm & only count SRMs */
   if(fdata==NULL){
    if(!(tmpm=(RATmaterials *)calloc(order->m*2,sizeof(RATmaterials)))){
     fprintf(stderr,"error in core allocation\n");
     exit(1);
    }
    for(k=0,j=0;k<order->m;k++){
     if(!strcmp(tmp[k].typeName,"srm with reflectance") || !strcmp(tmp[k].typeName,"srm with transmittance")){
      tmpm[j]=tmp[k]; 
      j++;
     }else if(!strcmp(tmp[k].typeName,"srm")){ /* both */
      tmpm[j]=tmp[k]; 
      tmpm[j].typeName=(char *)v_allocate((int)strlen("srm with reflectance")+1,sizeof(char)); 
      strcpy(tmpm[j].typeName,"srm with reflectance"); 
      j++;
      tmpm[j]=tmp[k]; 
      tmpm[j].typeName=(char *)v_allocate((int)strlen("srm with transmittance")+1,sizeof(char)); 
      strcpy(tmpm[j].typeName,"srm with transmittance"); 
      j++;
     }
    }
    order->ratMat = tmpm; order->m=j; 
    order->nbands=rat->wavebandbag->sensor_wavebands->no_of_wavebands; 
    order->n=max_ray_depth;
   }else{
    order->nbands=1;
    order->n=max_ray_depth;
   }
   if(!o && !(o=(int *)calloc(order->m,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
   }
  }else{
    /* copy all pointers over */
    order->nsamples=orders[0].nsamples;
    order->counter=orders[0].counter;
    order->rootfilename=orders[0].rootfilename;
    for(j=0;j<2;j++){order->filename[j]=orders->filename[j];order->fp[j]=order->fp[j];}
    order->ratMat = orders[0].ratMat;
    order->r = orders[0].r;
    order->nRatorders=orders[0].nRatorders;
    order->m = orders[0].m;
    order->n = orders[0].n;
    order->nbands = orders[0].nbands;
  }
  /* calculate LUT for coding */
  if(i==0)order->diffuse=(double **)v_allocate(max_ray_depth+1,sizeof(double *));
  order->direct=(double **)v_allocate(max_ray_depth+1,sizeof(double *));
  order->coder=(unsigned char ***)v_allocate(max_ray_depth+1,sizeof(unsigned char **));
  for(l=0;l<=max_ray_depth;l++){
   if(i==0){
    order->counter[l]=order->nsamples[l]=0;
    order->r[l]=(struct RATorderStore *)v_allocate(l+1,sizeof(struct RATorderStore));
    for(k=0;k<=l;k++){
     for(j=0;j<order->m;j++)o[j]=0;
      o[0]=k;
      SequenceDoiIt(order->nbands,&(order->r[l][k]),NULL,&(order->counter[l]),order->m,l,o,0,NULL);
    }
   }
   /* now allocate a block for direct and diffuse */
   /* into camera[i].order[l][0].direct/diffuse */
   order->direct[l]=(double *)v_allocate(MAX(order->counter[l],1),sizeof(double));
   if(i==0)order->diffuse[l]=(double *)v_allocate(MAX(order->counter[l],1),sizeof(double));
   order->coder[l]=(unsigned char **)v_allocate(MAX(order->counter[l],1),sizeof(unsigned char *));
   for(I=0;I<MAX(order->counter[l],1);I++){
    order->coder[l][I]=(unsigned char *)v_allocate(order->m,sizeof(unsigned char)); 
   }
   /* now populate coder */
   order->counter[l]=order->nsamples[l]=0;
   order->r[l]=(struct RATorderStore *)v_allocate(l+1,sizeof(struct RATorderStore));
   for(k=0;k<=l;k++){
    for(j=0;j<order->m;j++)o[j]=0;
    o[0]=k;
    SequenceDoiIt(order->nbands,&(order->r[l][k]),NULL,&(order->counter[l]),order->m,l,o,0,order->coder[l]);
   }
  }  
 }
 free(o);
 /* pass these back via fdata */
 if(fdata){
  fdata[0][0]=binstart;
  fdata[0][1]=binstep;
  /* read data */
  /* now loop over nBins */
  {
   char dum[5];
   int J;
   double binstart;
   for(j=0;j<(*nLidarBins == 0 ? 1 : *nLidarBins + 2);j++){
    for(l=0;l<2;l++){
      fread(&dum[0],sizeof(char),4,orders[0].fp[l]);
      fread(&J,sizeof(int),1,orders[0].fp[l]);
      fread(&binstart,sizeof(double),1,orders[0].fp[l]);
      data = (l == 0 ? orders[j].direct : (j == 0 ? orders[j].diffuse : NULL ));
      /* loop over scattering order */
      if(data)for(k=0;k<orders[j].nRatorders;k++){
        /* input all information for that scattering order */
        fread(data[k],sizeof(double),orders[j].counter[k],orders[0].fp[l]);
      }
    }
   }
  }
  for(l=0;l<2;l++){
   fclose(orders[0].fp[l]);
   for(j=0;j<(*nLidarBins == 0 ? 1 : *nLidarBins + 2);j++)
    orders[j].fp[l]=NULL;
  }
 }
 return(orders);
}

RATorder *RATreadOrderTerms(double *bininfo,int *nLidarBins,double **fdata,char *rootfilename){
 RATorder *order; 
 order=RATsetOrderTerms(fdata,NULL,nLidarBins,rootfilename);
 if(bininfo){
  bininfo[0]=fdata[0][0]; /* binstart */
  bininfo[1]=fdata[0][1]; /* binstep */
 }
 return(order);
}

int RATScan(RATobj *rat){
  RATdevice **store,*camera,*light;
  int n;

  n = rat->nstore; 
  store = (RATdevice **)rat->store;
  camera = store[0];
  light = store[1];
  return RATScanPSF(rat,camera,n,light,n);
}  

int RATScanPSF(RATobj *rat,RATdevice *camera,int nCameras,RATdevice *illumination,int nSuns){
  int *schedule=NULL,*active=NULL,xstart=0,ystart=0,i,j,k;
  double *solar=NULL,*sunPosition=NULL,value;
  RATtree *ratTree=NULL;
  int nBands,thisRow=-1,thisCol=-1,grabber,ograbber;
  RATsample *pixel;
   
  nBands=RATgetNWavebands(rat,NULL);
  /*nSuns =RATgetNsuns(rat);*/
  /* sort the wavelenths */
  solar = (double *)v_allocate(3*nSuns,sizeof(double));
  pixel = (RATsample *)v_allocate(nSuns,sizeof(RATsample));
  for(k=0;k<nSuns;k++){
    pixel[k].sumImage = (double *)v_allocate(nBands*(camera[0].nBins > 0 ? 2+camera[0].nBins : 1),sizeof(double));
    pixel[k].sumRayLength = (double *)v_allocate(rat->flagbag->max_ray_depth+1,sizeof(double));
    pixel[k].hit = (int *)v_allocate(rat->flagbag->max_ray_depth+1,sizeof(int));
    
    pixel[k].nBands=nBands;
  }
  RATsetNsuns(rat,nSuns);
  for(i=0;i<nCameras;i++){
    if(!camera[i].order && rat->wavebandbag->sensor_wavebands->no_of_wavebands == 1 && rat->wavebandbag->sensor_wavebands->sensor_rsr[0].rsr[0][0] < 0){
      /* special format output */
      camera[i].order=RATsetOrderTerms(NULL,rat,&camera[i].nBins,camera[i].resultIntegralFilename);
    }
    schedule = (int *)v_allocate(camera[i].nPsf[0]*camera[i].nPsf[1],sizeof(int));
    active = (int *)v_allocate(camera[i].nPsf[0]*camera[i].nPsf[1],sizeof(int));
    for(j=0;j<camera[i].nPsf[0]*camera[i].nPsf[1];j++)
     schedule[j] = camera[i].psfSampleCount[j];
    
    /* set sun direction */
    for(j=0;j<nSuns;j++){
      solar[j*3+0]= illumination[j].focalAxis.x;solar[j*3+1]= illumination[j].focalAxis.y;solar[j*3+2]= illumination[j].focalAxis.z;
    }
    RATsetSun(rat,solar);
    
    if(illumination->cameraType==ORTHOGRAPHIC){
      sunPosition=NULL;
    }else{
      sunPosition = (double *)v_allocate(3*nSuns,sizeof(double));
      if(!rat->illumination)
	rat->illumination=(IlluminationBag *)v_allocate(1,sizeof(IlluminationBag));
      for(j=0;j<nSuns;j++){
	sunPosition[j*3+0]=illumination[j].focalPoint.x;
	sunPosition[j*3+1]=illumination[j].focalPoint.y;
	sunPosition[j*3+2]=illumination[j].focalPoint.z;
	if(!rat->illumination[j].position){
	  rat->illumination[j].position=(Camera_op *)v_allocate(1,sizeof(Camera_op));		
	}
	rat->illumination[j].position->ppoint=illumination[j].focalPoint;
      }
      rat->flagbag->sunlocation=sunPosition;
    }
    for(i=0;i<nSuns;i++)rat->illumination[i].image_Ptr=NULL;
    ratTree=RATgetRatTree(rat);
    
    while((grabber=RATScanPSFSample(&thisCol,&thisRow,schedule,active,&xstart,&ystart,rat,&camera[0],sunPosition)) != -2){
      ratTree=RATgetRatTree(rat);
      ograbber = thisCol + thisRow*camera[0].nS[0];
      
      RATunpackRatTree(camera[0].psfModulation[ograbber]/camera[0].rpp,rat,ratTree,illumination,&camera[0],pixel,camera[0].radianceNorm);
      active[grabber]--;
      /* store data in image */
      for(k=0;k<nSuns;k++){
	for(j=0;j<nBands*(camera[0].nBins > 0 ? 2+camera[0].nBins : 1);j++){  
	  value = pixel[k].sumImage[j];
	  camera[0].imagedata[ograbber + j*camera[0].nS[0]*camera[0].nS[1]] += value;
	  camera[0].psfdata[grabber + j*camera[0].nPsf[0]*camera[0].nPsf[1]] += value*camera[0].rpp/camera[0].psfSampleCount[grabber];
          camera[0].radiance[j] += value;
	}
      }
    }
    if(schedule)free(schedule);
    if(active)free(active);
  }
  if(solar)free(solar);
  if(sunPosition)free(sunPosition);
  return(1);
}

/* loop over camera PSF and trace rays */
int RATScanPSFSample(int *thisCol,int *thisRow,int *schedule,int *active,int *xstart,int *ystart,RATobj *rat,RATdevice *camera,double *sunPosition){
  int x,y,grabber,traced;
  triplet fromImagingPlane,X,Y,directionv,calculate_diffuse_sample_ray_direction();
  double direction[3],from[3],xx,yy,delta=1e-10;
  int nSuns,p;
  
  nSuns = RATgetNsuns(rat);
  
  /* schedule should originally be a copy of psfSampleCount */
  if(!schedule || !active)return(-1);
  from[0]=camera->focalPoint.x;from[1]=camera->focalPoint.y;from[2]=camera->focalPoint.z;
  for(y=*ystart;y<camera->nPsf[1];y++,*xstart=0,*ystart=y,p=(rat->flagbag->verbose == 1 ? fprintf(stderr,"\r(%5.2f)",100.*y/camera->nPsf[1]): 1)){
    for(x=*xstart;x<camera->nPsf[0];x++){
      grabber = x+y*camera->nPsf[0];
      if(schedule[grabber]>0){
	schedule[grabber]--;
	active[grabber]++;
	(*xstart)=x;
	*thisCol = camera->psfmap[0][grabber];
	*thisRow = camera->psfmap[1][grabber];
	/* assume reverse ray tracing for the moment */
	xx = x+Random();
	yy = y + Random();
	X = V_factor(camera->cameraX,camera->imagingPlaneDimensions[0]*0.5*(xx-camera->nPsf[0]*0.5)/(camera->nPsf[0]*0.5));
	Y = V_factor(camera->cameraY,camera->imagingPlaneDimensions[1]*0.5*(yy-camera->nPsf[1]*0.5)/(camera->nPsf[1]*0.5));
	fromImagingPlane = vector_plus(camera->principalPoint,vector_plus(X,Y));
	if(camera->cameraType==ORTHOGRAPHIC){
	  directionv = camera->focalAxis;
	  directionv.x *= -1.0;directionv.y *= -1.0;directionv.z *= -1.0;
	  directionv= normalise(directionv);
	  from[0] = fromImagingPlane.x - camera->focalAxis.x;
	  from[1] = fromImagingPlane.y - camera->focalAxis.y;
	  from[2] = fromImagingPlane.z - camera->focalAxis.z;
	}else if (camera->cameraType==PERSPECTIVE){
	  directionv = normalise(vector_minus(camera->focalPoint,fromImagingPlane));
	}else if (camera->cameraType==DIFFUSE){
          directionv = camera->focalAxis;
          directionv.x *= -1.0;directionv.y *= -1.0;directionv.z *= -1.0;
          directionv= normalise(directionv);
          from[0] = fromImagingPlane.x - camera->focalAxis.x;
          from[1] = fromImagingPlane.y - camera->focalAxis.y;
          from[2] = fromImagingPlane.z - camera->focalAxis.z;
        }
	direction[0]=directionv.x;direction[1]=directionv.y;direction[2]=directionv.z;
        rat->hit_camera = (camera->cameraType==DIFFUSE ? 1 : 0);
	traced = RATtraceRay(rat,from,direction,sunPosition);
	
	return(grabber);
      }
    }
  }
  /* nothing left to do */
  return(-2);
}

RATdevice *RATloadCamera(RATdevice *rat,int orthographic,int nBands,int nBins,double binStart,double binStep,double *cameraLocation,double focalLength,double *angles,int rpp,int nPixels,double *imagingPlaneDimensions,double *psf,int *npsf){
  int i,j;
  double cphi,sphi,ctheta,stheta,cbeta,sbeta,scale[3],aspectRatio,y_angle;
  int nPsfSamples;
  double zenith, azimuth, twist,sum,tantheta_2;
  triplet angels,Xv,Yv,Zv,vector_copy2();
  
  zenith = angles[0]; 
  azimuth = angles[1];
  twist = angles[2];
  
  if(!rat)rat = (RATdevice *)v_allocate(1,sizeof(RATdevice));
  Xv = vector_copy2(1.,0.,0.);
  Yv = vector_copy2(0.,1.,0.);
  Zv = vector_copy2(0.,0.,1.);	
  rat->nBands=nBands;
  rat->nBins=nBins;
  rat->binStart=binStart;
  rat->binStep=binStep;
  rat->binMax = rat->binStart+rat->nBins*rat->binStep;
  angels = vector_copy2(azimuth,zenith,0.0);	
  rat->twist = DTOR(twist);
  rat->zenith = DTOR(zenith);
  rat->azimuth = DTOR(azimuth);
  cphi  = cos(rat->azimuth);
  ctheta=cos(rat->zenith);
  cbeta =cos(rat->twist);
  sphi  = sin(rat->azimuth); 
  stheta=sin(rat->zenith);
  sbeta =sin(rat->twist);
  
  scale[0] = imagingPlaneDimensions[0]*.5;
  scale[1] = imagingPlaneDimensions[1]*.5;
  if(focalLength == 0 || orthographic != 0){
    if (orthographic == 1) rat->cameraType = ORTHOGRAPHIC;
    if (orthographic == -1) rat->cameraType = DIFFUSE;

    rat->focalLength=scale[2]=1.0;
    y_angle = 0.;
  }else if(focalLength < 0){
    /* -ve focal length: interpret this as camera Y-axis angle (degrees) */
    rat->cameraType = PERSPECTIVE;
    y_angle = -focalLength;
    rat->focalLength = scale[2] = fabs(imagingPlaneDimensions[1]/(2.*tan(DTOR(y_angle)*0.5)));
  }else{
    rat->cameraType = PERSPECTIVE;
    rat->focalLength = scale[2] = focalLength;
    tantheta_2 = imagingPlaneDimensions[1]/(2.*rat->focalLength);
    y_angle = 2.*RTOD(atan(tantheta_2));
  }
  
  rat->fov[1] = y_angle;
  
  tantheta_2 = tan(DTOR(rat->fov[1]*0.5));
  
  for(i=0;i<2;i++)rat->imagingPlaneDimensions[i]=imagingPlaneDimensions[i];
  
  /* focal axis runs 0 to 1 */
  rat->focalAxis = Zv;
  rat->cameraX = rotate_about(Xv,Zv,azimuth);
  rat->cameraY = rotate_about(Yv,Zv,azimuth);
  
  rat->focalAxis = rotate_about(rat->focalAxis,rat->cameraX,zenith);
  rat->cameraY = rotate_about(rat->cameraY,rat->cameraX,zenith);
  
  rat->cameraX = rotate_about(rat->cameraX,rat->focalAxis,twist);
  rat->cameraY = rotate_about(rat->cameraY,rat->focalAxis,twist);	       
  
  /* X & Y axes run -1 to +1 */
  
  rat->nPixels = nPixels;
  rat->rpp = rpp;
  
  rat->focalPoint=arrayToTriplet(cameraLocation);
  rat->principalPoint = vector_plus(rat->focalPoint,V_factor(rat->focalAxis,rat->focalLength));
  
  /* how many pixels in the output image (if there is one) */
  aspectRatio = imagingPlaneDimensions[0]/imagingPlaneDimensions[1];
  
  rat->aspectRatio = aspectRatio;
  rat->fov[0] = RTOD(2.*atan(tantheta_2*aspectRatio));
  rat->fovMax = RTOD(atan(tantheta_2*sqrt(1+aspectRatio*aspectRatio)));
  rat->nS[0] = (int)(sqrt(nPixels * aspectRatio)+0.5);
  rat->nS[1] = (int)(sqrt(nPixels / aspectRatio)+0.5);
  
  if(npsf == NULL || npsf[0] == 0 || npsf[1] == 0 ){
    /* need to define psf dimensions */
    rat->nPsf[0] = rat->nS[0];
    rat->nPsf[1] = rat->nS[1];
  }else{
    for(i=0;i<2;i++)rat->nPsf[i]=npsf[i];
  } 
  nPsfSamples = rat->nPsf[0]*rat->nPsf[1];
  if(psf == NULL){
    /* generate one */
    rat->psf = (double *)v_allocate(rat->nPsf[0]*rat->nPsf[1],sizeof(double));
    for(i=0;i<nPsfSamples;i++){
      rat->psf[i]=1.0;
    }
  }else{
    /* normalise so that sum is nPixels */
    sum = 0.;
    for(i=0;i<rat->nPsf[0]*rat->nPsf[1];i++){
      sum += psf[i];
    }
    for(i=0;i<rat->nPsf[0]*rat->nPsf[1];i++){
      psf[i] *= (rat->nPsf[0]*rat->nPsf[1])/sum;
    }
    rat->psf = psf;
  }
  /* allocate arrays for location of pixels in psf map space */
  rat->psfmap[0] = (int *)v_allocate(rat->nPsf[0]*rat->nPsf[1],sizeof(int));
  rat->psfmap[1] = (int *)v_allocate(rat->nPsf[0]*rat->nPsf[1],sizeof(int));
  rat->psfSampleCount = (int *)v_allocate(rat->nPsf[0]*rat->nPsf[1],sizeof(int));
  rat->psfModulation = (double *)v_allocate(rat->nS[0]*rat->nS[1],sizeof(double));
  
  processPSF(rat);
  
  if(nBands>0){
    rat->imagedata = (double *)v_allocate((nBins == 0 ? 1 : 2+nBins)*nBands*rat->nS[0]*rat->nS[1],sizeof(double));
    rat->radiance = (double *)v_allocate((nBins == 0 ? 1 :2+nBins)*nBands*rat->nPsf[0]*rat->nPsf[1],sizeof(double));
    rat->psfdata = (double *)v_allocate((nBins == 0 ? 1 : 2+nBins)*nBands*rat->nPsf[0]*rat->nPsf[1],sizeof(double));
    rat->directRadiance=(double ***)v_allocate((nBins == 0 ? 1 : 2+nBins),sizeof(double **));
    rat->diffuseRadiance=(double ***)v_allocate(1,sizeof(double **));
    for(i=0;i<(nBins == 0 ? 1 : 2+nBins);i++){
      rat->directRadiance[i]=(double **)v_allocate(rat->nBands,sizeof(double *));
      if(i==0)rat->diffuseRadiance[i]=(double **)v_allocate(rat->nBands,sizeof(double *));
      for(j=0;j<rat->nBands;j++){
        if(i==0)rat->diffuseRadiance[i][j]=(double *)v_allocate(rat->nOrders,sizeof(double));
        rat->directRadiance[i][j]=(double *)v_allocate(rat->nOrders,sizeof(double));
      }
    }
  }
  
  /* sort out how to do sampling across the PSF for nPixel samples */
  return(rat);
}

/* scan the image */


