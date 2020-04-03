#include "rat.h"
#include <stdlib.h>

/* #define CALLOC(a,b) calloc((size_t)MAX(100,a),(size_t)b) */
#define CALLOC(a,b) calloc((size_t)a,(size_t)b)

#define N_CLEAR_FLAGS 10
int isNewObjectMaterial(RATobj *ratObj,char *material,int objectType);
char *typeTranslate(int);
/* new utilities Dec 2006 */

int RATgetNmaterials(RATobj *ratObj){
  return(ratObj->materialbag->material_list->no_of_materials);
}

int RATgetVerboseLevel(RATobj *ratObj){
  return(ratObj->flagbag->verbose);
}


void RATsetVerboseLevel(RATobj *ratObj,int level){
  ratObj->flagbag->verbose=level;
  return;
}
 
/* freeing utilities */
int levels=0,counter=0;

void pl(){
  int i;
  for(i=0;i<levels;i++)fprintf(stderr," ");
}

void freeBBoxContents();
void viewObject();
void viewBBoxContents();
/* start of view */

void printHistogram(RATdistribution *d,char *name,char *histogramType,int type,char *material,double numberdensity,double *min,double *max){
  int i,sum=0;
  
  if(!name)return;
  if(!material){
    return;
  }
  if(!d->name){
    d->name = c_allocate(strlen(name)+strlen(material)+strlen(histogramType)+1024);
    sprintf(d->name,"%s.%s.type%d.%s.dat",name,histogramType,type,material);
  }

  if(!d->fp){ 
    if((d->fp=(FILE *)fopen(d->name,"w"))==NULL){
      fprintf(stderr,"error opening histogram file %s\n",d->name);
      exit(1);
    }
  }
  for(i=0;i<d->nSamples;i++){
    sum += d->data[i];
  }
  /* header */
  fprintf(d->fp,"# number density (per unit volume): %e\n",numberdensity);
  fprintf(d->fp,"# number of objects in scene: %d\n",(int)(0.5+numberdensity*(max[0]-min[0])*(max[1]-min[1])*(max[2]-min[2])));
  fprintf(d->fp,"# min extent: %lf %lf %lf\n",min[0],min[1],min[2]);
  fprintf(d->fp,"# max extent: %lf %lf %lf\n",max[0],max[1],max[2]);
  fprintf(d->fp,"# bin size: %lf \n",d->step);
  fprintf(d->fp,"# min bin: %lf \n",d->min);
  fprintf(d->fp,"# max bin: %lf \n",d->max);
  fprintf(d->fp,"# no. samples: %d \n",d->nSamples);
  /* data */
  for(i=0;i<d->nSamples;i++){
    fprintf(d->fp,"%lf %lf\n",d->min+(i+0.5)*d->step,d->data[i]/(double)sum);
  }
  fclose(d->fp);
  d->fp=NULL;
  return;
}

void RATprintObjects(RATobj *ratObj){
  int i,j;
  RATobject *obj;
  double volume,v[3],numberdensity;
  int nsamps,isok=1;

  for(i=0;i<ratObj->nratObjs;i++){
    obj=&ratObj->ratObj[i];
    if((obj->nsamps[0]>0) && (obj->nsamps[1]>0) && (obj->nsamps[2]>0)){
      volume=0.;
      nsamps=1;
      for(j=0;j<obj->nsamps[0]*obj->nsamps[1]*obj->nsamps[2];j++){
	volume+=obj->nd[j];
      }
      if(volume){
        if(obj->datafp){
	  /* number density */
	  for(j=0;j<nsamps;j++){
	    obj->nd[j] /= volume;
	  }
        }
      }else isok=0;
    }
    if(isok){
      /* total volume considered */
      volume=1.;
      for(j=0;j<3;j++){
        v[j] = obj->max[j] - obj->min[j];
        if(v[j] == 0 )v[j]=1.;
        volume *= v[j];
      }
      numberdensity = obj->nObjects / volume;
    }
    /* the histograms */
    if((isok) && (obj->materialName) && (numberdensity)){
      printHistogram(&obj->sizes,ratObj->ratObj[0].rawfilename,"SizeHistogram",obj->type,obj->materialName,numberdensity,obj->min,obj->max);
      printHistogram(&obj->angle,ratObj->ratObj[0].rawfilename,"AngleHistogram",obj->type,obj->materialName,numberdensity,obj->min,obj->max);
    
      /* hips header for nd */
      if((obj->nsamps[0]>0) && (obj->nsamps[1]>0) && (obj->nsamps[2]>0)){
        if(!(obj->datafilename)){
	  obj->datafilename = c_allocate(strlen(ratObj->ratObj[0].rawfilename)+strlen(obj->materialName)+strlen("NumberDensity")+1024);
	  sprintf(obj->datafilename,"%s.%s.type%d.%s.dat",ratObj->ratObj[0].rawfilename,"NumberDensity",obj->type,obj->materialName);	
        }
        if(!obj->datafp){
	
	/*	if((fd=open(obj->datafilename,O_WRONLY | O_CREAT))== -1){
		fprintf(stderr,"error opening density file %s\n",obj->datafilename);
		exit(1);
		}
		write(fd,'a',1);
		close(fd);
	  
		if((fd=open(obj->datafilename,O_WRONLY))== -1){
		fprintf(stderr,"error opening density file %s\n",obj->datafilename);
		exit(1);
		}	
		obj->datafp=fdopen(fd,"w");
	*/
       }
       {
	struct header *hd;
	hd=v_allocate(1,sizeof(struct header));
	init_header(hd, "Number Density image", "rat library output", obj->nsamps[2], "today", obj->nsamps[1], obj->nsamps[0], sizeof(float), 0, PFFLOAT, "");
	update_header(hd, ratObj->globalArgc,ratObj->globalArgv);
	/*fwrite_header(fd,hd);
	lseek(fd,0,SEEK_SET);
	offset=lseek(fd,0,SEEK_END);
	close(fd);*/
	obj->datafp=(FILE *)fopen(obj->datafilename,"w");
	fp_fwrite_header(obj->datafp,hd);
	/* get offset */
	fwrite(obj->nd,sizeof(float),obj->nsamps[0]*obj->nsamps[1]*obj->nsamps[2],obj->datafp);
	fclose(obj->datafp);
	obj->datafp=NULL;
	/* and an envi header just for good luck */
       }
     }      
    }
  }
}
void viewClone();
void viewClone(RATobj *ratObj,struct Clone *c,int *f,double start,double span){
  int i,j;
  double translation[3],matrix[16];
  double Matrix[16],IMatrix[16],thisTrans[16];
  if(!c)return;
  for(i=0;i<16;i++)thisTrans[i]=ratObj->Matrix[i];
  while(c){
    if(f && f[1]){
      translation[0]=c->translation.x;
      translation[1]=c->translation.y;
      translation[2]=c->translation.z;
      if(c->matrix){
        /* copy the matrix over */
        for(i=0;i<9;i++)matrix[i]=c->matrix[i];
        /* invert */
        /*Matrix_inversion(imatrix,matrix,interchanges,3,&det);*/
      }else for(i=0;i<3;i++)for(j=0;j<3;j++)matrix[i*3+j]=(i == j ? 1 : 0);
      for(i=0;i<3;i++)for(j=0;j<3;j++)Matrix[i*4+j]=matrix[i*3+j];
      for(i=0;i<3;i++)Matrix[i*4+3]=translation[i];
      for(i=0;i<3;i++)Matrix[3*4+i]=0.;
      Matrix[15]=1.;
      for(i=0;i<16;i++){ratObj->Matrix[i]=0.;}
      Matrix_Matrix_multiplication_to_Matrix(Matrix,thisTrans,ratObj->Matrix,4);
      counter++;
    }
    /* apply ratObj->Matrix to all contents */
    viewBBoxContents(ratObj,c->bbox,f,start,span);
    /* reset to IMatrix */
    if(f && f[1]){levels--;}
    c = c->next_clone;
  }
  for(i=0;i<16;i++)ratObj->Matrix[i]=thisTrans[i];
}

void *viewBLP();
void *viewBLP(RATobj *ratObj,struct BiLinearPatch_struct *n,int *f){
  int i=0,j,v1[] = {0,1,2},v2[] = {2,3,0},*v,d1[3],d2[3];
  int material=0,objectType;
  double location[3],size,normal[3];

  if(!n)return(NULL);
 
  if(f && f[1]){
    /* take the mean over two traingles */
    if(n->material)
    material=*n->material->material;

    objectType=FO;
    for(j=0;j<2;j++){
      if(j==0)v=&(v1[0]);else v=&(v2[0]);
      location[0]=location[1]=location[2]=0.;
      for(i=0;i<3;i++){
	location[0]+=n->corner[v[i]].x;
	location[1]+=n->corner[v[i]].y;
	location[2]+=n->corner[v[i]].z;
      }
      d1[0]=n->corner[v[0]].x-n->corner[v[1]].x;
      d1[1]=n->corner[v[0]].y-n->corner[v[1]].y;
      d1[2]=n->corner[v[0]].z-n->corner[v[1]].z;
      d2[0]=n->corner[v[0]].x-n->corner[v[2]].x;
      d2[1]=n->corner[v[0]].y-n->corner[v[2]].y;
      d2[2]=n->corner[v[0]].z-n->corner[v[2]].z;
      normal[0] = d1[1] * d2[2] - d1[2] * d2[1];
      normal[1] = -(d1[0] * d2[2] - d1[2] * d2[0]);
      normal[2] = d1[0] * d2[1] - d1[1] * d2[0];
      size=n->objectSize;
      for(i=0;i<3;i++){
	location[i] /= 3.0;
      }
      size = 100;
      /* set f[1] to 1 for first (loader) pass
      ** or >1 else 
      */
      loadObject(ratObj,material,objectType,location,size,normal,f[1]); 
    }
    counter++;
  }else{
    /* wavefront output */
    fprintf(stderr,"object\n");
  }
  return((void *)(n->next_BiLinearPatch));
}

void *viewPlane();
void *viewPlane(RATobj *ratObj,struct Plane_struct *n,int *f){
  if(!n)return(NULL);  
  if(f && f[1]){
    /* probably not a good idea to do a plane */
    /*
      material=*n->material->material;
      objectType=PLANE;
      normal[0]=n->normal.x;
      normal[1]=n->normal.y;
      normal[2]=n->normal.z;
      if(normal[2] != 0){
      location[0]=location[1]=0.;
      location[2] = n->dw / normal[2];
      }else if (normal[1] != 0){
      location[0]=location[2]=0.;
      location[1] = n->dw / normal[1];
      }else if (normal[0] != 0){
      location[1]=location[2]=0.;
      location[0] = n->dw / normal[0];
      }
      size = 100;
    */
    
    /* set f[1] to 1 for first (loader) pass
    ** or >1 else 
    */
    /* loadObject(ratObj,material,objectType,location,size,normal,f[1]); */
   
    counter++;
  }
  return((void *)n->next_plane);
}

void *viewSphericalDem();
void *viewSphericalDem(RATobj *ratObj,struct Spherical_Dem_struct *n,int *f){
  if(!n)return(NULL);  
  if(f && f[1]){
    counter++;
  }
  return((void *)n->next_dem);
}

void *viewDem();
void *viewDem(RATobj *ratObj,struct Dem_struct *n,int *f){
  if(!n)return(NULL); 
  if(f && f[1]){
    counter++;
  }
  return((void *)n->next_dem);
}
void *viewDisk();
void *viewDisk(RATobj *ratObj,struct Disk_struct *n,int *f){
  int material=0,objectType;
  double location[3],size,normal[3];

  if(!n)return(NULL); 
  if(f && f[1]){
    if(n->material)
      material=*n->material->material;
    objectType=DISK;
    normal[0]=n->normal.x;
    normal[1]=n->normal.y;
    normal[2]=n->normal.z;
    location[0]=n->origin.x;
    location[1]=n->origin.y;
    location[2]=n->origin.z;
    size = n->objectSize;
    /* set f[1] to 1 for first (loader) pass
    ** or >1 else 
    */
    loadObject(ratObj,material,objectType,location,size,normal,f[1]);
    counter++;
  }
  return((void *)n->next_disk);
}

void *viewTriangle(),loadWFObject();
void *viewEllipse();
void *viewEllipse(RATobj *ratObj,struct Ellipse_struct *n,int *f){
  int i,j,material=0,objectType;
  double location[3],size,normal[3];
  double location_[3],size_,normal_[3];
  static double *tesselate=NULL;
  static int nLevels=3;
  static int nTriangles=0;
  static struct Facet_struct *facet=NULL;

  if(tesselate==0){
    /* unit sphere tesselated */
    tesselate=sphereTesselate(nLevels,&nTriangles);
    facet=(struct Facet_struct *)v_allocate(1,sizeof(struct Facet_struct));
  }

  if(!n)return(NULL);  
    if(f && f[1]){
     if(n->material)
       material=*n->material->material;
     objectType=ELLIPSE;
     normal[0]=normal[1]=normal[2]=0;
     if(n->dimensions.z > n->dimensions.x && n->dimensions.z > n->dimensions.y ){
       normal[2]=1;
     }else if(n->dimensions.y > n->dimensions.x && n->dimensions.y > n->dimensions.z){
       normal[1]=1;
     }else{
       normal[1]=1;
     }
     location[0]=n->origin.x;
     location[1]=n->origin.y;
     location[2]=n->origin.z;
     /* NB this is the volume */
     size = n->objectSize;
     /* set f[1] to 1 for first (loader) pass
     *     ** or >1 else 
     *         */
     /* tesselate */
     /* convert to triangles */
     for(i=0;i<nTriangles;i++){
      for(j=0;j<3;j++){
       facet->v[j].x=location[0] + n->dimensions.x * tesselate[i*9 + j*3 + 0];
       facet->v[j].y=location[1] + n->dimensions.y * tesselate[i*9 + j*3 + 1];
       facet->v[j].z=location[2] + n->dimensions.z * tesselate[i*9 + j*3 + 2];
      }
      location_[0] = (facet->v[0].x + facet->v[1].x + facet->v[2].x)/3.;
      location_[1] = (facet->v[0].y + facet->v[1].y + facet->v[2].y)/3.;
      location_[2] = (facet->v[0].z + facet->v[1].z + facet->v[2].z)/3.;
      size_ = facet->objectSize=facetNormal(&normal_[0],facet);
      if(size_)loadObject(ratObj,material,objectType,location_,size_,normal_,f[1]);
     }
     counter++;
  }
  return((void *)n->next_ellipse);
}

void *viewCylinder();
void *viewCylinder(RATobj *ratObj,struct Cylinder_struct *n,int *f){
  int tmpCounter,i,j,D=10;
  int material=0,objectType;
  double radius,location[3],size,normal[3],*cylinderTesselate();
  double location_[3],size_,normal_[3];
  static double *tesselate=NULL;
  static int nLevels=3;
  static int nTriangles=0;
  static struct Facet_struct *facet=NULL;
  static triplet norm,zaxis,yaxis;
  triplet rotate_about();
  double theta,phi;

  if(tesselate==0){
    /* unit cylinder tesselated */
    tesselate=cylinderTesselate(nLevels,&nTriangles);
    facet=(struct Facet_struct *)v_allocate(1,sizeof(struct Facet_struct));
    zaxis.x=zaxis.y=zaxis.z=yaxis.x=yaxis.y=yaxis.z=0.;
    zaxis.z=yaxis.y=1.0;
  }

  if(!n)return(NULL);  
  if(f && f[1]){
    tmpCounter=counter;counter=0;
    /*if(n->ends){
      if(!n->ends[0].material)n->ends[0].material=n->material;
      if(!n->ends[0].material)n->ends[0].material=n->material;
      viewDisk(ratObj,&n->ends[0],f);
      viewDisk(ratObj,&n->ends[1],f);
    }*/
    counter=tmpCounter;
    /* need to deal properly with tesselation of closed cylinders at some point */
    if(n->material)
      material=*n->material->material;
    if(n->ends)objectType=CLOSED_CYLINDER;
    else objectType=CYLINDER;
    radius = sqrt(n->r2);
    norm = normalise(n->normal);
    theta = RTOD(acos(norm.z));
    phi = RTOD(atan2(norm.y,norm.x));
    normal[0]=n->normal.x;
    normal[1]=n->normal.y;
    normal[2]=n->normal.z;
    /* tesselate */
    for(i=0;i<nTriangles;i++){
      for(j=0;j<3;j++){
       facet->v[j].x= n->origin.x + radius * tesselate[i*9 + j*3 + 0];
       facet->v[j].y= n->origin.y  + radius * tesselate[i*9 + j*3 + 1];
       facet->v[j].z= n->origin.z + n->height * tesselate[i*9 + j*3 + 2];
       facet->v[j]=rotate_about(facet->v[j],yaxis,-theta);
       facet->v[j]=rotate_about(facet->v[j],zaxis,-phi);

      }
      location_[0] = (facet->v[0].x + facet->v[1].x + facet->v[2].x)/3.;
      location_[1] = (facet->v[0].y + facet->v[1].y + facet->v[2].y)/3.;
      location_[2] = (facet->v[0].z + facet->v[1].z + facet->v[2].z)/3.;
      size_ = facet->objectSize=facetNormal(&normal_[0],facet);
      if(size_)loadObject(ratObj,material,objectType,location_,size_,normal_,f[1]);
    }
  }
  return((void *)n->next_cylinder);
}


void *viewSphere();
void *viewSphere(RATobj *ratObj,struct Sphere_struct *n,int *f){
  int material=0,objectType,i,j;
  double location[3],size,normal[3];
  double location_[3],size_,normal_[3];
  static double *tesselate=NULL;
  static int nLevels=3;
  static int nTriangles=0;
  static struct Facet_struct *facet=NULL;

  if(tesselate==0){
    /* unit sphere tesselated */
    tesselate=sphereTesselate(nLevels,&nTriangles);
    facet=(struct Facet_struct *)v_allocate(1,sizeof(struct Facet_struct));
  }
  if(!n)return(NULL);  
  if(f && f[1]){
    if(n->material)
      material=*n->material->material;
    objectType=SPHERE;
    normal[0]=normal[1]=0;normal[2]=1;
    location[0]=n->centre.x;
    location[1]=n->centre.y;
    location[2]=n->centre.z;
    /* NB this is the volume */
    size = n->objectSize;
    /* set f[1] to 1 for first (loader) pass
    *      *     ** or >1 else 
    *           *         */
    /* convert to triangles */
    for(i=0;i<nTriangles;i++){
      for(j=0;j<3;j++){
       facet->v[j].x=location[0] + n->radius * tesselate[i*9 + j*3 + 0];
       facet->v[j].y=location[1] + n->radius * tesselate[i*9 + j*3 + 1];
       facet->v[j].z=location[2] + n->radius * tesselate[i*9 + j*3 + 2];
      }
      location_[0] = (facet->v[0].x + facet->v[1].x + facet->v[2].x)/3.;
      location_[1] = (facet->v[0].y + facet->v[1].y + facet->v[2].y)/3.;
      location_[2] = (facet->v[0].z + facet->v[1].z + facet->v[2].z)/3.;
      size_ = facet->objectSize=facetNormal(&normal_[0],facet);
      if(size_)loadObject(ratObj,material,objectType,location_,size_,normal_,f[1]);
    }
    counter++;
  }
  return((void *)n->next_sphere);
}

void *viewBezier();
void *viewBezier(RATobj *ratObj,struct Bezier4_struct *n,int *f){
  if(!n)return(NULL);  
  if(f && f[1]){ 
    counter++;
  }
  return((void *)n->next_bezier4);
}

double facetNormal(double *normal,struct Facet_struct *n){
  double size,v0[3],v1[3],cr[3];
  if(!normal)normal=&cr[0];
  v0[0]=n->v[0].x-n->v[1].x;
  v0[1]=n->v[0].y-n->v[1].y;
  v0[2]=n->v[0].z-n->v[1].z;
  v1[0]=n->v[0].x-n->v[2].x;
  v1[1]=n->v[0].y-n->v[2].y;
  v1[2]=n->v[0].z-n->v[2].z;
  normal[0]=v0[1]*v1[2]-v0[2]*v1[1];
  normal[1]=v0[2]*v1[0]-v0[0]*v1[2];
  normal[2]=v0[0]*v1[1]-v0[1]*v1[0];
  size = n->objectSize=sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  return(size);
}

void *viewTriangle(RATobj *ratObj,struct Facet_struct *n,int *f){
  int material=0,objectType;
  double location[3],size,normal[3];
  static FILE *fp=NULL;
  if(!n)return(NULL); 
  if(f){
    if(n->material)
    material=*n->material->material;
    objectType=FO;
    normal[0]=n->normal.x;
    normal[1]=n->normal.y;
    normal[2]=n->normal.z;
    location[0]=(n->v[0].x+n->v[1].x+n->v[2].x)/3.;
    location[1]=(n->v[0].y+n->v[1].y+n->v[2].y)/3.;
    location[2]=(n->v[0].z+n->v[1].z+n->v[2].z)/3.;
    size = n->objectSize;
    if(size==0){
      size = n->objectSize= facetNormal(NULL,n);
    }
    if(size==0){
      return((void *)n->next_facet);
    }
    /* set f[1] to 1 for first (loader) pass
    ** or >1 else 
    */
    if(size){
      if(f[1])loadObject(ratObj,material,objectType,location,size,normal,f[1]);
      else{
        loadWFObject(ratObj,material,objectType,location,&n->v,n->vertex_normals,n->local_coords,normal);
      }
    }
    counter++;
  }
  return((void *)n->next_facet);
}

/* start of free() */

void freeClone();
void freeClone(struct Clone *c,int *f){
  int i,j;
  if(!c)return;
  if(f && f[1]){pl();fprintf(stderr,"%16s %3d {\n","Clone level",levels);levels++;}
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d ","Clone",counter);
    if(1){
      fprintf(stderr,"translation: [%f %f %f] ",c->translation.x,c->translation.y,c->translation.z);
    }
    if(c->matrix){
      fprintf(stderr,"rotation: ");
      for(i=0,j=0;i<3;i++){
	j=0;
        fprintf(stderr,"[%f %f %f] ",c->matrix[i*3+j],c->matrix[i*3+j+1],c->matrix[i*3+j+2]);
        j += 3;
      }
    }
    fprintf(stderr,"\n");
    counter++;
  }
  freeBBoxContents(c->bbox,f);
  if(f[0]){
    c->bbox=NULL;
    c->translation.x=c->translation.y=c->translation.z=0.;
    c->z_rotation_flag=0;
    if(c->matrix)free(c->matrix);
    c->matrix=NULL;
  }
  if(f && f[1]){levels--;pl();fprintf(stderr,"%16s %3d }\n","Clone level",levels);}
  if(c->next_clone)freeClone(c->next_clone,f);
  if(f[0])c->next_clone=NULL;
}

void freeBLP();
void freeBLP(struct BiLinearPatch_struct *n,int *f){
  int i=0;
  if(!n)return;
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d corners [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f]\n","Bilinear patch",counter,n->corner[i].x,n->corner[i].y,n->corner[i].z,n->corner[i+1].x,n->corner[i+1].y,n->corner[i+1].z,n->corner[i+2].x,n->corner[i+2].y,n->corner[i+2].z,n->corner[i+3].x,n->corner[i+3].y,n->corner[i+3].z);
    i += 4;
    counter++;
  }
  freeBLP(n->next_BiLinearPatch,f);
  if(f[0]){
    n->next_BiLinearPatch=NULL;
    n->vertex_normals=NULL;
    n->local_coords=NULL;
    n->material=NULL;
    free(n);
  } 
}

void freePlane();
void freePlane(struct Plane_struct *n,int *f){
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d dw: %f normal [%f %f %f]\n","plane",counter,n->dw,n->normal.x,n->normal.y,n->normal.z);
    counter++;
  }
  freePlane(n->next_plane,f);
  if(f[0]){
    n->material=NULL;
    n->dw=0;
    n->normal.x=n->normal.y=n->normal.z=0.;
    free(n);
  }
}

void freeSphericalDem();
void freeSphericalDem(struct Spherical_Dem_struct *n,int *f){
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d TODO MORE\n","Spherical DEM",counter);
    counter++;
  }
  freeSphericalDem(n->next_dem,f);
  if(f[0]){
    n->material=NULL;
    if(n->data.fdata){
      free(n->data.fdata);
      n->data.fdata=NULL;
    }
    if(n->data.bdata){
      free(n->data.bdata);
      n->data.bdata=NULL;
    } 
    free(n);
  }
}

void freeDem();
void freeDem(struct Dem_struct *n,int *f){
  if(!n)return; 
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d TODO MORE\n","DEM",counter);
    counter++;
  }
  freeDem(n->next_dem,f);
  if(f[0]){
    n->local=NULL;
    n->repeatMode=0;
    n->material=NULL;
    if(n->data.fdata){
      free(n->data.fdata);
      n->data.fdata=NULL;
    }
    if(n->data.bdata){
      free(n->data.bdata);
      n->data.bdata=NULL;
    }
    free(n);
  }
}

void freeDisk();
void freeDisk(struct Disk_struct *n,int *f){
  if(!n)return; 
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d radius: %f origin: [%lf %lf %lf] normal: [%lf %lf %lf]\n","Disk",counter,sqrt(n->r2),n->origin.x,n->origin.y,n->origin.z,n->normal.x,n->normal.y,n->normal.z);
    counter++;
  }
  freeDisk(n->next_disk,f);
  if(f[0]){
    n->material=NULL;
    free(n);
  }
}

void freeEllipse();
void freeEllipse(struct Ellipse_struct *n,int *f){
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d semi-axes: [%f %f %f] origin: [%f %f %f] normal: [%f %f %f]\n","Ellipsoid",counter,n->dimensions.x,n->dimensions.y,n->dimensions.z,n->origin.x,n->origin.y,n->origin.z,n->normal.x,n->normal.y,n->normal.z);
    counter++;
  }
  freeEllipse(n->next_ellipse,f);
  if(f[0]){
    n->material=NULL;
    free(n);
  }
}

void freeCylinder();
void freeCylinder(struct Cylinder_struct *n,int *f){
  int tmpCounter;
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d height: %f radius: %f origin: [%f %f %f] normal: [%f %f %f]\n","Cylinder",counter,n->height,sqrt(n->r2),n->origin.x,n->origin.y,n->origin.z,n->normal.x,n->normal.y,n->normal.z);
    tmpCounter=counter;counter=0;
    if(n->ends){
      freeDisk(&n->ends[0],f);
      freeDisk(&n->ends[1],f);
    }
    counter=tmpCounter;
    counter++;
  }
  freeCylinder(n->next_cylinder,f);
  if(f[0]){
    n->material=NULL;
    if(n->ends){
      freeDisk(&n->ends[0],f);
      freeDisk(&n->ends[1],f);
      free(n->ends);
      n->ends=NULL;
    }
    free(n);
  }
}

void freeSphere();
void freeSphere(struct Sphere_struct *n,int *f){
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d radius: %f centre: [%f %f %f]\n","Sphere",counter,n->radius,n->centre.x,n->centre.y,n->centre.z);
    counter++;
  }
  freeSphere(n->next_sphere,f);
  if(f[0]){
    n->material=NULL;
    free(n);
  }
}

void freeBezier();
void freeBezier(struct Bezier4_struct *n,int *f){
  if(!n)return;  
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d TODO MORE\n","Bezier patch",counter);
    counter++;
  }
  freeBezier(n->next_bezier4,f);
  if(f[0]){
    n->material=NULL;
    free(n);
  }
}

void freeTriangle();
void freeTriangle(struct Facet_struct *n,int *f){
  int i=0;
  if(!n)return; 
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %d vertices: [%f %f %f] [%f %f %f] [%f %f %f] normal: [%f %f %f]\n","Triangle",counter,n->v[i].x,n->v[i].y,n->v[i].z, n->v[i+1].x,n->v[i+1].y,n->v[i+1].z,n->v[i+2].x,n->v[i+2].y,n->v[i+2].z, n->normal.x,n->normal.y,n->normal.z);
    i += 3;
    counter++;
  } 
  freeTriangle(n->next_facet,f);
  if(f[0]){
    n->material=NULL;
    free(n);
  }
}

void freeBBoxContents(BBox *bb,int *f){
  int i;
  struct Content *b;

  if(!bb)return;
  b = &(bb->contents);
  if(f && f[1]){
    pl();fprintf(stderr,"%16s %3d {\n","BBox level",levels);
    if(bb->define_flag){pl();fprintf(stderr," %16s\n","defined");}
    pl();fprintf(stderr," %16s [%f %f %f]\n","Max",bb->max.x,bb->max.y,bb->max.z);
    pl();fprintf(stderr," %16s [%f %f %f]\n","Min",bb->min.x,bb->min.y,bb->min.z);
    if(bb->transform_flag){
      pl();fprintf(stderr," %16s\n","transform:");
      for(i=0;i<4;i++){
        pl();fprintf(stderr," %16s %f %f %f %f ]\n","[",bb->m.data[i][0],bb->m.data[i][1],bb->m.data[i][2],bb->m.data[i][3]);
      }
    }
    levels++;
  }
  if(b->blp){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","BLPs:",b->no_of_blps);
    }
    freeBLP(b->blp,f);
    if(f[0]){
      b->blp=NULL;
      b->no_of_blps=0;
    }
  }
  if(b->plane){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Planes:",b->no_of_planes);
    }
    freePlane(b->plane,f);
    if(f[0]){
      b->plane=NULL;
      b->no_of_planes=0;
    }
  }
  if(b->spherical_dem){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Spherical DEMs:",b->no_of_spherical_dems);
    }
    freeSphericalDem(b->spherical_dem,f);
    if(f[0]){
      b->spherical_dem=NULL;
      b->no_of_spherical_dems=0;
    }
  }
  if(b->dem){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","DEMs:",b->no_of_dems);
    }
    freeDem(b->dem,f);
    if(f[0]){
      b->dem=NULL;
      b->no_of_dems=0;
    }
  }
  if(b->disk){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Disks:",b->no_of_disks);
    }
    freeDisk(b->disk,f);
    if(f[0]){
      b->disk=NULL;
      b->no_of_disks=0;
    }
  }
  if(b->ellipse){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Ellipsoids:",b->no_of_ellipses);
    }
    freeEllipse(b->ellipse,f);
    if(f[0]){
      b->ellipse=NULL;
      b->no_of_ellipses=0;
    }
  }
  if(b->cylinder){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Cylinders:",b->no_of_cylinders);
    }
    freeCylinder(b->cylinder,f);
    if(f[0]){
      b->cylinder=NULL;
      b->no_of_cylinders=0;
   }
  }
  if(b->sphere){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Spheres:",b->no_of_spheres);
    }
    freeSphere(b->sphere,f);
    if(f[0]){
      b->sphere=NULL;
      b->no_of_spheres=0;
    }
  }
  if(b->bezier){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Beziers:",b->no_of_beziers);
    }
    freeBezier(b->bezier,f);
    if(f[0]){
      b->bezier=NULL;
      b->no_of_beziers=0;
    }
  }
  if(b->ttriangle){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Triangles:",b->no_of_triangles);
    }
    freeTriangle(b->ttriangle,f);
    if(f[0]){
      b->ttriangle=NULL;
      b->no_of_triangles=0;
    }
  }
  if(b->clone){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","Clones:",b->no_of_clones);
    }
    freeClone(b->clone,f);
    if(f[0]){
      free(b->clone);
      b->clone=NULL;
      b->no_of_clones=0;
    }
  }
  if(b->bbox){
    if(f && f[1]){
      counter=0;pl();fprintf(stderr," %16s %3d\n","BBoxes:",b->no_of_bboxes);
    }
    freeBBoxContents(b->bbox,f);
    if(f[0]){
      free(b->bbox);
      b->bbox=NULL;
      b->no_of_bboxes=0;
    }
  }
  if(bb->next_bbox){
    freeBBoxContents(bb->next_bbox,f);
    if(f[0]){
      free(bb->next_bbox);
      bb->next_bbox=NULL;
    }
  }
  if(f && f[1]){levels--;pl();fprintf(stderr,"%16s %3d }\n","BBox level",levels);}
  if(f[0]){
    bb->max.x=bb->max.y=bb->max.z=-BIG;
    bb->min.x=bb->min.y=bb->min.z=BIG;
  }
  return; 
}

void viewBBoxContents(RATobj *ratObj,BBox *bb,int *f,double start,double span){
  struct Content *b;
  int verbose=1,nBoxes=0,boxCount=0,count=0;
  double totalProportion;
  void *next=NULL;
  if(!bb){
    return;
  }
  if(0 &&verbose)fprintf(stderr,"{");
  nBoxes=1;
  next = (void *)bb->next_bbox;
  while(next){nBoxes++; next = (void *)bb->next_bbox;}
  next=NULL;
  totalProportion = span/MAX(1.,(double)nBoxes);
  /* check to see if bbox is withing required limits */
  while(bb){
    if(!( bb->max.x < ratObj->ratObj->min[0] || bb->min.x > ratObj->ratObj->max[0] || bb->max.y < ratObj->ratObj->min[1] || bb->min.y > ratObj->ratObj->max[1] || bb->max.z < ratObj->ratObj->min[2] || bb->min.z > ratObj->ratObj->max[2])){
      b = &(bb->contents);
      if(verbose && totalProportion>=1)fprintf(stderr,"\r%8.4f          ",start +0.5);
      start += totalProportion;
      boxCount=0;
      if(f && f[1]){
        if(bb->transform_flag){
	  ;
        }
        levels++;
      }
      if(b->blp){
        if(f && f[1]){
	  counter=0;pl();
        }
        next=(void *)b->blp;
        while(next){
          next=viewBLP(ratObj,next,f);
        }
      }
      if(b->plane){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->plane;
        while(next){
          next=viewPlane(ratObj,next,f);
        }
      }
      if(b->spherical_dem){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->spherical_dem;
        while(next){
          next=viewSphericalDem(ratObj,next,f);
        }
      }
      if(b->dem){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->dem;
        while(next){
          next=viewDem(ratObj,next,f);
        }
      }
      if(b->disk){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->disk;
        while(next){
          next=viewDisk(ratObj,next,f);
        }
      }
      if(b->ellipse){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->ellipse;
        while(next){
          next=viewEllipse(ratObj,next,f);
        }
      }
      if(b->cylinder){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->cylinder;
        while(next){
          next=viewCylinder(ratObj,next,f);
        }
      }
      if(b->sphere){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->sphere;
        while(next){
          next=viewSphere(ratObj,next,f);
        }
      }
      if(b->bezier){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->bezier;
        while(next){
          next=viewBezier(ratObj,next,f);
        }
      }
      if(b->ttriangle){
        if(f && f[1]){
	  counter=0;
        }
        next=(void *)b->ttriangle;
        while(next){
          next=viewTriangle(ratObj,next,f);
        }
      }
      if(b->clone){
        if(f && f[1]){
	  counter=0;
        }
        viewClone(ratObj,b->clone,f,start-totalProportion,totalProportion);
      }
      if(b->bbox){
        if(f && f[1]){
	  counter=0;
        }
        if(0 && verbose)fprintf(stderr,".");
        viewBBoxContents(ratObj,b->bbox,f,start-totalProportion,totalProportion);
      }
    }
    bb = bb->next_bbox;
  }
  if(f && f[1]){levels--;}
  return; 
}

void viewObject(RATobj *ratObj,int *f){
  BBox *top_root_bbox_Ptr;
  static FILE *fp=NULL;

  top_root_bbox_Ptr=ratObj->bbox;
  viewBBoxContents(ratObj,top_root_bbox_Ptr,f,0.0,100.0);
  return;
}

void freeObject(RATobj *ratObj,int *f){
  BBox *top_root_bbox_Ptr;

  top_root_bbox_Ptr=ratObj->bbox;
  freeBBoxContents(top_root_bbox_Ptr,f);
  return;
}

int RATgetPID(){
  return(getpid());
}

/* return memory use in Kb */
int RATgetMemory(){
  static char cmd[1024];
  FILE *fp;
  int out;
  sprintf(cmd,"/bin/ps -p %d -o rss",getpid());
  fp=popen(cmd,"r");
  fgets(cmd,1024,fp);
  fscanf(fp,"%d",&out);
  pclose(fp);
  return(out);
}

void RATsetWavefrontFilename(RATobj *ratObj,char *filename){
  /* 

  static int *f=NULL;

  if(!f && !(f=(int *)CALLOC(N_CLEAR_FLAGS,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  f[0]=1;

*/
  if(!filename)strcpy(ratObj->wavebandbag->wavefront_file,"");
  strcpy(ratObj->wavebandbag->wavefront_file,filename);
/*  freeObject(ratObj,f); */
  return;
}

char *RATgetWavefrontFilename(RATobj *ratObj){
  return(ratObj->wavebandbag->wavefront_file);
}

int RATisWavefrontFile(RATobj *ratObj){
  if(strcmp(ratObj->wavebandbag->wavefront_file,"")||strcmp(ratObj->wavebandbag->wavefront_file,"None"))
    return(1);
  return(0);
}

int RATprintObject(RATobj *ratObj){
  static int *f=NULL;

  if(!f && !(f=(int *)CALLOC(N_CLEAR_FLAGS,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  f[1]=1;
  levels=0;
  if(RATisWavefrontFile(ratObj))
    freeObject(ratObj,f);
  else
    return(0);
  return(1);
}

/* get a volumetric representation */
RATvol *RATgetVolumetric(RATobj *ratObj,int *grid,double *extent){
  static int *f=NULL;

  if(!ratObj)return(NULL);

  /* if extent not specified, work it out */ 

  if(!f && !(f=(int *)CALLOC(N_CLEAR_FLAGS,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  f[1]=1;
  levels=0;
  if(RATisWavefrontFile(ratObj))
    freeObject(ratObj,f);
  else
    return((RATvol *)0);
  return((RATvol *)1);
}

int isNewObjectMaterial(RATobj *ratObj,char *material,int objectType){
  int i;
  if(!ratObj->ratObj)return(0);

  for(i=0;i<ratObj->nratObjs;i++){
    if(ratObj->objectTypes[i] == objectType && ratObj->objectMaterialNames[i] == material)return(i);
  }
  return(-1);
}

void clearDistribution(RATdistribution *d){
  if(!d)return;
  d->nSamples=0;
  d->min=d->max=d->step=0.0;
  if(d->data)free(d->data);
  d->data=NULL;
  return;
}

/* object type to name translator */
char *typeTranslate(int type){
  
  switch(type){
    case FO:
    return("triangle");
    break;
    case SPHERE:
    return("sphere");
    break;
    case CYLINDER:
    return("cylinder");
    break;
    case BEZIER:
    return("bezier");
    break;
    case DEM:
    return("DEM");
    break;
    case SPHERICAL_DEM:
    return("sphericalDEM");
    break;
    case ELLIPSE:
    return("ellipsoid");
    break;
    case VOLUMETRIC_SPHERE:
    return("volumetricSphere");
    break;
    case VOLUMETRIC_CYLINDER:
    return("volumetricCylinder");
    break;
    case VOLUMETRIC_ELLIPSE:
    return("volumetricEllipsoid");
    break;
    case CLOSED_CYLINDER:
    return("closedCylinder");
    break;
    case DISK:
    return("disk");
    break;
    case BILINEARPATCH:
    return("bilinearPatch");
    break;
  }
  return("notype");
}

RATobject *allocateRatObject(RATobj *ratObj,int init,int nratObjs,double *min,double *max,int *nSamples,int nHistogramSamples,char *name){
  int nObjects = 0,i,j;
  RATobject *tmp=NULL;
  static char rat[] = "rat";

  if(name == NULL)name = &rat[0];
  if(nHistogramSamples<=0)nHistogramSamples=90;
  
  /* note: ratObj->nratObjs is the current number of RATobjects */
  /* nratObjs is how many we want now */
  
  if(init){
    if(ratObj->ratObj)for(i=0;i<=ratObj->nratObjs;i++){
      ratObj->ratObj[i].type=-1;
      ratObj->ratObj[i].materialName=NULL;
      ratObj->ratObj[i].material=-1;
      for(j=0;j<3;j++){
	ratObj->ratObj[i].min[j]=ratObj->ratObj[i].max[j]=ratObj->ratObj[i].delta[j]=0;
	ratObj->ratObj[i].nsamps[j]=0;
      }
      ratObj->ratObj[i].nObjects=0;
      clearDistribution(&ratObj->ratObj[i].sizes);
      clearDistribution(&ratObj->ratObj[i].angle);
      if(ratObj->ratObj[i].nd)free(ratObj->ratObj[i].nd);
      if(ratObj->ratObj[i].otherStuff)free(ratObj->ratObj[i].otherStuff);   
      ratObj->ratObj[i].nd=NULL;
      ratObj->ratObj[i].otherStuff=NULL;
    }
    if(ratObj->ratObj)free(ratObj->ratObj);
    ratObj->ratObj=NULL;
  }
  
  /* none allocated */
  if(!ratObj->ratObj){
    ratObj->nratObjs=ratObj->nObjectTypes;
    /* how many objects do we need ? */
    nObjects=ratObj->nObjectTypes;

    if(!(ratObj->ratObj=(RATobject *)CALLOC(nObjects,sizeof(RATobject)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    tmp = ratObj->ratObj;

    /* load up min/max */
    for(i=0;i<nObjects;i++){
      tmp[i].rawfilename=name;
      tmp[i].sizes.min=ratObj->minObjectSize[i];
      tmp[i].angle.min=0.;
      tmp[i].sizes.max=(ratObj->maxObjectSize[i] > ratObj->minObjectSize[i] ? ratObj->maxObjectSize[i] : ratObj->minObjectSize[i] *1.1 );
      tmp[i].angle.max=90.;
      tmp[i].sizes.nSamples = nHistogramSamples;
      tmp[i].materialName = ratObj->objectMaterialNames[i];
      tmp[i].type = ratObj->objectTypes[i];
      tmp[i].sizes.name = c_allocate(strlen(name)+strlen(tmp[i].materialName)+strlen("SizeHistogram")+1024);
      sprintf(tmp[i].sizes.name,"%s.SizeHistogram.%s.%s.dat",name,typeTranslate(tmp[i].type),tmp[i].materialName);
      tmp[i].angle.nSamples = nHistogramSamples;
      tmp[i].angle.name = c_allocate(strlen(name)+strlen(tmp[i].materialName)+strlen("AngleHistogram")+1024);
      sprintf(tmp[i].angle.name,"%s.AngleHistogram.%s.%s.dat",name,typeTranslate(tmp[i].type),tmp[i].materialName);

      if(min){
        for(j=0;j<3;j++)tmp[i].min[j]=min[j];
      }else if(i==0){
	tmp[i].min[0]=ratObj->bbox->min.x;
	tmp[i].min[1]=ratObj->bbox->min.y;
	tmp[i].min[2]=ratObj->bbox->min.z;
      }else{
        for(j=0;j<3;j++)tmp[i].min[j]=tmp[0].min[j];
      }
      if(max){
        for(j=0;j<3;j++)tmp[i].max[j]=max[j];
      }else if(i==0){
	tmp[i].max[0]=ratObj->bbox->max.x;
	tmp[i].max[1]=ratObj->bbox->max.y;
	tmp[i].max[2]=ratObj->bbox->max.z;
      }else{
        for(j=0;j<3;j++)tmp[i].max[j]=tmp[0].max[j];
      } 
      if(nSamples){
        for(j=0;j<3;j++)tmp[i].nsamps[j]=nSamples[j];
      }else if(i==0){
        for(j=0;j<3;j++)tmp[i].nsamps[j]=0;
      }else{
        for(j=0;j<3;j++)tmp[i].nsamps[j]=tmp[0].nsamps[j];
      }
      if(tmp[i].nsamps[0]*tmp[i].nsamps[1]*tmp[i].nsamps[2] != 0){
        if(!tmp[i].nd){
	  /* allocate data structure */
	  if(!(tmp[i].nd=(float *)CALLOC(tmp[i].nsamps[0]*tmp[i].nsamps[1]*tmp[i].nsamps[2],sizeof(float)))){
	    fprintf(stderr,"error in core allocation\n");
	    exit(1);
	  }
        }
	tmp[i].datafilename = c_allocate(strlen(name)+strlen(tmp[i].materialName)+strlen("NumberDensity")+1024);
	sprintf(tmp[i].datafilename,"%s.NumberDensity.%s.%s.dat",name,typeTranslate(tmp[i].type),tmp[i].materialName);
      }
      /* sort remaining bits & bobs */
      for(j=0;j<3;j++){
        if(tmp[i].nsamps[j] != 0)
          tmp[i].delta[j] = (tmp[i].max[j] - tmp[i].min[j])/(double)tmp[i].nsamps[j];
        else
          tmp[i].delta[j] = (tmp[i].max[j] - tmp[i].min[j]);
      }
      tmp[i].sizes.data=d_allocate(tmp[i].sizes.nSamples);
      tmp[i].sizes.step=(tmp[i].sizes.max - tmp[i].sizes.min)/(double)tmp[i].sizes.nSamples;
      tmp[i].angle.data=d_allocate(tmp[i].angle.nSamples);
      tmp[i].angle.step=(tmp[i].angle.max - tmp[i].angle.min)/(double)tmp[i].angle.nSamples;
    }
  }
  /* return NULL if ok */
  return(NULL);
}

void loadIntoHistogram(double v,RATdistribution *d,double wt){
  int bin;
  if(!d)return;
  bin=MAX(0,MIN(d->nSamples-1,(int)((v-d->min)/d->step)));
  (d->data[bin])+=wt;
  return;
}

void loadWFObject(RATobj *ratObj,int material,int objectType,double *location,triplet *v,Vertex_normals *vertex_normals,Vertex_locals *local_coords,double *norm){
  RATobject *ThisObj=NULL;
  int nobj=-1,i,j,k;
  double local[3][2],normal[3],V[4],Vout[4],vDot(),RMatrix[9];
  static int *count=NULL,*_count=NULL;
  static FILE **fp=NULL,**_fp=NULL;
  static char **filename=NULL,**_filename=NULL;
  static int NObjs=-1;

/* close the files  & deallocate */
  if(NObjs>0 && ratObj==NULL && material<0){
    for(i=0;i<NObjs;i++){
      if(fp[i])fclose(fp[i]);fp[i]=NULL;
      if(filename[i])free(filename[i]);filename[i]=NULL;
      if(count && count[i])count[i]=0;
    } 
   if(count)free(count);count=NULL;
   if(fp)free(fp);fp=NULL;
   if(filename)free(filename);filename=NULL; 
  }

 /* check to see if its a new material of object type */
  if((nobj=isNewObjectMaterial(ratObj,ratObj->material_name[material],objectType)) == -1){
    fprintf(stderr,"error finding occurrence of material %s object type %s\n",ratObj->material_name[material],typeTranslate(objectType));
    return;
  }else{
    ThisObj=&(ratObj->ratObj[nobj]);
  }

  if(nobj > NObjs || fp == NULL){
     if(count){_count=count;count=NULL;}
     if(fp){_fp=fp;fp=NULL;}
     if(filename){_filename=filename;filename=NULL;}
     count=(int *)v_allocate(NObjs,sizeof(int));
     for(i=0;i<nobj;i++)count[i]=1;
     fp=(FILE **)v_allocate(NObjs,sizeof(FILE *));
     filename=(char **)v_allocate(NObjs,sizeof(char *));
     for(i=0;i<nobj;i++){
       if(_count)count[i]=_count[i];
       if(_fp)fp[i]=_fp[i];
       if(_filename)filename[i]=_filename[i];
     } 
     free(_count);_count=NULL;
     free(_fp);_fp=NULL;
     free(_filename);_filename=NULL;
  } 

  /* check that the file is open */
  if(!filename[nobj]){
    filename[nobj]=(char *)c_allocate(strlen(ratObj->ratObj->rawfilename)+1024);
    sprintf(filename[nobj],"%s.obj",ratObj->ratObj->rawfilename);
    if(!(fp[nobj]=(FILE *)fopen(filename[nobj],"w+"))){
      fprintf(stderr,"error opening wavefront file %s for writing\n",filename[nobj]);
      exit(1);
    } 
    fprintf(fp[nobj],"mtllib %s.mtl\n",ratObj->ratObj->rawfilename);
  } 
  /* centroid location check */
  V[0]=location[0];
  V[1]=location[1];
  V[2]=location[2];
  V[3]=1.0;
  Vector_Matrix_multiplication_to_Vector(V,ratObj->Matrix,Vout,4);
  for(i=0;i<3;i++)location[i]=Vout[i];
  for(i=0;i<3;i++){
    if ( location[i] < ratObj->ratObj->min[i] || location[i] > ratObj->ratObj->max[i] )return;
  }
  /* material */
  fprintf(fp[nobj],"usemtl %s\n",ratObj->material_name[material]); 
  for(j=0;j<3;j++){
    V[0]=v[j].x;
    V[1]=v[j].y;
    V[2]=v[j].z;
    V[3]=1.0;
    /* location rotation */
    Vector_Matrix_multiplication_to_Vector(V,ratObj->Matrix,Vout,4);
    for(i=0;i<3;i++)location[i]=Vout[i];
    fprintf(fp[nobj],"v %lf %lf %lf\n",location[0],location[1],location[2]); 
    /* normal */
    if(vertex_normals){
      V[0]=vertex_normals[j].vertex_normal.x;
      V[1]=vertex_normals[j].vertex_normal.y;
      V[2]=vertex_normals[j].vertex_normal.z;
      for(i=0;i<3;i++){
        for(k=0;k<3;k++)RMatrix[i*3+k]=ratObj->Matrix[i*4+k];
      }
      V[3]=0.0;
      Vector_Matrix_multiplication_to_Vector(V,RMatrix,Vout,3);
      for(i=0;i<3;i++)normal[i]=Vout[i];
    }else if(j==0){
      V[0]=norm[0];
      V[1]=norm[1];
      V[2]=norm[2];
      V[3]=0.0;
      for(i=0;i<3;i++){
        for(k=0;k<3;k++)RMatrix[i*3+k]=ratObj->Matrix[i*4+k];
      }
      Vector_Matrix_multiplication_to_Vector(V,RMatrix,Vout,3);
      for(i=0;i<3;i++)normal[i]=Vout[i];
    }
    fprintf(fp[nobj],"vn %lf %lf %lf\n",normal[0],normal[1],normal[2]);
    if(local_coords){
      local[j][0]=local_coords[j].vertex_local.x;
      local[j][1]=local_coords[j].vertex_local.y;
    }else if(j==0){
      local[j][0]=0.0;
      local[j][1]=1.0;
    }
    fprintf(fp[nobj],"vt %lf %lf\n",local[j][0],local[j][1]);
  }
  fprintf(fp[nobj],"f %d/%d/%d %d/%d/%d %d/%d/%d\n",count[nobj],count[nobj],count[nobj],count[nobj]+1,count[nobj]+1,count[nobj]+1,count[nobj]+2,count[nobj]+2,count[nobj]+2);
  count[nobj] +=3;
  return;
}

int loadObject(RATobj *ratObj,int material,int objectType,double *location,double size,double *normal,int pass){
  RATobject *ThisObj=NULL;
  int nobj=-1,i,j,cell[3];
  double nlen,V[4],Vout[4],cost,angle,vDot(),RMatrix[9];
  static double up[] = {0.,0.,1.};

  /* normalise normal */
  nlen=0.;
  for(i=0;i<3;i++)nlen+=normal[i]*normal[i];
  if(nlen>0)nlen=sqrt(nlen);else nlen=1.0;
  for(i=0;i<3;i++)normal[i]/=nlen;
  /* location rotation */
  for(i=0;i<3;i++)V[i]=location[i]; 
  V[3]=1.0;
  Vector_Matrix_multiplication_to_Vector(V,ratObj->Matrix,Vout,4);
  for(i=0;i<3;i++)location[i]=Vout[i];
/* temporary */
/* fprintf(stderr,"%lf %lf %lf\n",Vout[0],Vout[1],Vout[2]); */
  /* check to see if the location is within bounds */
  for(i=0;i<3;i++){
    if ( location[i] < ratObj->ratObj->min[i] || location[i] > ratObj->ratObj->max[i] )return(0);
  }

  /* check to see if its a new material of object type */
  if((nobj=isNewObjectMaterial(ratObj,ratObj->material_name[material],objectType)) == -1){
    fprintf(stderr,"error finding occurrence of material %s object type %s\n",ratObj->material_name[material],typeTranslate(objectType));
    return(-1);
  }else{
    ThisObj=&(ratObj->ratObj[nobj]);
  }
  /* rotations and translations */
  
  for(i=0;i<3;i++){
    V[i]=normal[i];
    for(j=0;j<3;j++)RMatrix[i*3+j]=ratObj->Matrix[i*4+j];
  }
  V[3]=0.0;
  Vector_Matrix_multiplication_to_Vector(V,RMatrix,Vout,3);
  for(i=0;i<3;i++)normal[i]=Vout[i];
  
  cost = vDot(normal,up,3);
  angle = acos(fabs(cost))*180./M_PI;

  /* load into number density array */
  if(ThisObj->nd){
    for(i=0;i<3;i++){
      cell[i] = MAX(0,MIN(ThisObj->nsamps[i]-1,(location[i]-ThisObj->min[i])/ThisObj->delta[i]));
    }
    (*(ThisObj->nd + cell[2]*ThisObj->nsamps[0]*ThisObj->nsamps[1]+cell[1]*ThisObj->nsamps[0]+cell[0]))+=size;
  }

  /* pass == 3: the limits are pre-defined */
  (ThisObj->nObjects)++;
  
  loadIntoHistogram(size,&ThisObj->sizes,1);
  loadIntoHistogram(angle,&ThisObj->angle,size);
  return(ThisObj->nObjects);
}

double *RATgetBBoxMax(RATobj *ratObj,double *max){
  if(!ratObj || !max)return(NULL);
  max[0]=ratObj->bbox->max.x;
  max[1]=ratObj->bbox->max.y;
  max[2]=ratObj->bbox->max.z;
  return(max);
}

double *RATgetBBoxMin(RATobj *ratObj,double *min){
  if(!ratObj || !min)return(NULL);
  min[0]=ratObj->bbox->min.x;
  min[1]=ratObj->bbox->min.y;
  min[2]=ratObj->bbox->min.z;
  return(min);
}

/* get a summary representation */
RATobject *RATgetObjects(RATobj *ratObj,int whichOne,double *min,double *max,int *nsamples,int nHistogramSamples,char *filenames){
  static int *f=NULL;
  int i,j;

  if(!ratObj)return(NULL);

  if(!f && !(f=(int *)CALLOC(N_CLEAR_FLAGS,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  f[1]=1;
  if(whichOne==0)f[1]=0;
  levels=0;
  if(RATisWavefrontFile(ratObj)){
    /* clear rotation/translation matrix */
    for(i=0;i<4;i++)for(j=0;j<4;j++)ratObj->Matrix[i*4+j] = (i == j ? 1 : 0); 
    allocateRatObject(ratObj,1,0,min,max,nsamples,nHistogramSamples,filenames);
    /* the the histogram limits on first pass */
    viewObject(ratObj,f);
    /* deallocate */
    /*loadWFObject(NULL,-1,-1,NULL,NULL,NULL,NULL,NULL);*/
  }else
    return(NULL);
  return(NULL);
}

int RATreadWavefrontFile(RATobj *ratObj){
  static int *f=NULL;

  if(!f && !(f=(int *)CALLOC(N_CLEAR_FLAGS,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  f[0]=1;
  if(RATisWavefrontFile(ratObj))
    freeObject(ratObj,f);
  if(!expand_filename(&(ratObj->wavebandbag->wavefront_file),"ARARAT_OBJECT",TRUE)){
    fprintf(stderr,"%s: error opening wavefront object file %s\n",ratObj->globalArgv[0],ratObj->wavebandbag->wavefront_file);
    strcpy(ratObj->wavebandbag->wavefront_file,"");
    return(0);
  }
  if(ratObj->flagbag->verbose){
    fprintf(stderr,"%s: input file %s specified\n",ratObj->globalArgv[0],ratObj->wavebandbag->wavefront_file);
  }
  RATreadObject(ratObj); 
  return(1);
}

double *RATgetSun(RATobj *ratObj){
  static double *out=NULL;
  int i=0,n;
  FlagBag *flagbag;
  IlluminationBag *illumination;

  if(out)free(out);
  n=MAX(1,ratObj->nSuns);
  if(!(out=(double *)CALLOC(3*n,sizeof(double)))){fprintf(stderr,"error in core allocation\n");exit(1);}
  for(i=0;i<n;i++){
    flagbag=ratObj->flagbag;
    illumination=&(ratObj->illumination[i]);
    if(illumination->sun_Ptr){
      out[i*3+0]=illumination->sun_Ptr->x;
      out[i*3+1]=illumination->sun_Ptr->y;
      out[i*3+2]=illumination->sun_Ptr->z;
    }
  }
  return(out);
}

char *RATgetProgramName(RATobj *ratObj){
  return(ratObj->globalArgv[0]);
}

void 	RATsignalInterrupt(int sig)
{
/*
*	allow kill -10 to clean kill
*	writing out data in buffer before exiting
*/
  RATuserInterrupt((RATobj *)globalRATObj,sig);
  return;
}

int RATgetNWavebands(RATobj *ratObj,double *wavebands){
  int i;
  if(!wavebands){
    return(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands);
  }
  ratObj->wavebandbag->sensor_wavebands->no_of_wavebands=MAX(1,ratObj->wavebandbag->sensor_wavebands->no_of_wavebands);
  if(!ratObj->wavebandbag->lambda)ratObj->wavebandbag->lambda=v_allocate(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,sizeof(double));
  for(i=0;i<ratObj->wavebandbag->sensor_wavebands->no_of_wavebands;i++){
    wavebands[i]=ratObj->wavebandbag->lambda[i];
  }
  return(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands);
}

void RATsetNWavebands(RATobj *ratObj,int n,double *minwavelength,double *bandwidth){
  int i,k;
  double *rsr0,*rsr1,Random(),lambda;
  double          lambda_min, lambda_width;
  void *v_allocate();
  ratObj->flagbag->fixed_wavelength=1;
  if(!minwavelength){
    fprintf(stderr,"%s: error in call to RATsetNWavebands(RATobj *ratObj,int n,double  *wavebands)\nminwavelength==NULL\n",ratObj->globalArgv[0]);
    exit(1);
  }
  if(ratObj->wavebandbag->sensor_wavebands->sensor_rsr)free(ratObj->wavebandbag->sensor_wavebands->sensor_rsr);
  ratObj->wavebandbag->sensor_wavebands->no_of_wavebands=n;
  ratObj->wavebandbag->sensor_wavebands->sensor_rsr = (Spectral_Data *)v_allocate(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,sizeof(Spectral_Data)); 
  for(i=0;i<n;i++){
    ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].no_of_samples=1;
    /* min */
    ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].rsr[0][0]=minwavelength[i];
    /* width */
    if(bandwidth){
      ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].rsr[1][0]=bandwidth[i];
    }else{
      ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].rsr[1][0]=0.;
    }
  }

  lambda_min = *ratObj->wavebandbag->lambda_min_Ptr;
  lambda_width = *ratObj->wavebandbag->lambda_width_Ptr;

  /* now sort wavebandbag samples 
  ** which involves setting
  ** materialbag->samples->wavelength[l] to the corresponding 
  ** values in sun_magnitude
  */
  if(ratObj->wavebandbag->lambda)free(ratObj->wavebandbag->lambda);
  if(!(ratObj->wavebandbag->lambda=(double *)CALLOC(n,sizeof(double)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  if(ratObj->materialbag->samples->wavelength)free(ratObj->materialbag->samples->wavelength);

  if(!(ratObj->materialbag->samples->wavelength=(int *)CALLOC(n,sizeof(int )))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }

  for(i=0;i<n;i++){
    rsr0 = ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].rsr[0];
    rsr1 = ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].rsr[1];
    lambda = (rsr0[0]) + (Random() * rsr1[0]);
    ratObj->wavebandbag->lambda[i]=lambda;
    if(ratObj->illumination->sky_data_Ptr && ratObj->illumination->sky_data_Ptr->hd.orig_name){
      ratObj->materialbag->samples->wavelength[i]=MAX(0,MMIN(ratObj->illumination->sky_data_Ptr->hd.num_frame - 1, quantise_sky_waveband(&(ratObj->illumination->sky_data_Ptr->hd),lambda, ratObj->wavebandbag->sensor_wavebands->sensor_rsr[i].no_of_samples, lambda_min, lambda_width)));
    }else{
      ratObj->materialbag->samples->wavelength[i]=0;
    } 
    if(!(ratObj->materialbag->material_lut))
      allocate_material_list(ratObj->materialbag,ratObj->wavebandbag);

    for (k = 0; k < ratObj->materialbag->material_list->no_of_materials; k++){
      calculate_material_lut(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,&(ratObj->materialbag->material_lut[k]), lambda, ratObj->materialbag->material_list, i, k);
    }
  }
  return;
}

int RATgetnMaterials(RATobj *ratObj){

  return(ratObj->materialbag->material_list->no_of_materials);
  /*
  nMaterials=ratObj->materialbag->material_list->no_of_materials;
  for(i=0;i<nMaterials;i++){
    material=&(ratObj->materialbag->material_list->material[i]);
    name=ratObj->materialbag->material_names[i];
    for(j=0;j<ratObj->materialbag->material_list->useage->no_of_materials;j++){
      if(!strcmp(name,ratObj->materialbag->material_list->useage->names[j])){
        matCount++;
      }
    }
  }
  return(matCount);
  */
}

void RATprintDefinedMaterials(RATobj *ratObj){
  int i,j,k;
  int nMaterials;
  Standard_Material_List *material;
  char *name;
  char *isused=NULL;
  static char yes[]="yes",no[]="no";
  char *materialType=NULL;
  static char srmr[]="srm with reflectance",srmt[]="srm with transmittance",transparent[]="transparent",srm[]="srm",volumetric[]="volumetric";

  isused=no;
  nMaterials=ratObj->materialbag->material_list->no_of_materials;
  fprintf(stderr,"========\n%d materials defined\n========\n",nMaterials);
  for(i=0;i<nMaterials;i++){
    material=&(ratObj->materialbag->material_list->material[i]);
    name=ratObj->materialbag->material_names[i];
    if(material->srm.diffuse_reflectance && material->srm.diffuse_transmittance){
      materialType=srm;
    }else if(material->srm.diffuse_reflectance){
      materialType=srmr;
    }else if(material->srm.diffuse_transmittance){
      materialType=srmt;
    }else if(material->volume.uL){
      materialType=volumetric;
    }else{
      materialType=transparent;
    }

    fprintf(stderr,"material %d {\n",i);
    fprintf(stderr,"\t    name: %s\n",name);
    for(j=0;j<ratObj->materialbag->material_list->useage->no_of_materials;j++){
      if(!strcmp(name,ratObj->materialbag->material_list->useage->names[j])){
        isused=yes;
      }
    }
    fprintf(stderr,"\tis used?: %s\n",isused);
    fprintf(stderr,"\t    type: %s\n",materialType);
    /* print properties */
    for(k=0;k<material->no_of_samples;k++){
      if(materialType==srm){
        fprintf(stderr,"\t\t%lf %lf %lf\n",material->wavelength[k],material->srm.diffuse_reflectance[i],material->srm.diffuse_transmittance[i]);
      }else if(materialType==srmr){
        fprintf(stderr,"\t\t%lf %lf\n",material->wavelength[k],material->srm.diffuse_reflectance[i]);
      }else if(materialType==srmt){
        fprintf(stderr,"\t\t%lf %lf\n",material->wavelength[k],material->srm.diffuse_transmittance[i]);
      } 
    }
    if(materialType==volumetric){
      fprintf(stderr,"\t\t      uL: %lf\n",material->volume.uL);
      fprintf(stderr,"\t\tmaterial: %d\n",material->volume.material);
    }
    fprintf(stderr,"}\n");
  }
  return;
}

/* translate from the material code used internally to
** name (returned value) and an index 
*/
char *RATtranslateMaterial(RATobj *ratObj,int thisMat,int *index){
  RATmaterials *ratMat;
  int nMat,i,thisone;
  if(*index)*index=-1;

  if(!ratObj || thisMat<0){if(*index)*index=-1;return(NULL);}
  /* first call RATgetMaterialsUsed() */
  ratMat = RATgetMaterialsUsed(ratObj,&nMat);
  if(ratMat == NULL || thisMat >= *(ratObj->nMaterials) || ratObj->ratMat[thisMat].used == 0 ){if(*index)*index=-1;return(NULL);}

  if(!ratObj->materialTranslatorLut){
    /* allocate & load materialTranslatorLut */
    ratObj->materialTranslatorLut=(int *)v_allocate(*(ratObj->nMaterials),sizeof(int));
    for(i=0;i<*(ratObj->nMaterials);i++)ratObj->materialTranslatorLut[i]=-1;
    for(i=0;i<nMat;i++){
      ratObj->materialTranslatorLut[ratMat[i].index]=i;
    }
  }
  thisone = ratObj->materialTranslatorLut[thisMat];
  if(thisone < 0){
    /* some error */
    {if(*index)*index=-1;return(NULL);}
  }
  if(index)*index=thisone;
  return(ratObj->ratMat[thisMat].name);
}

RATmaterials *RATgetMaterialsUsed(RATobj *ratObj,int *nMat){
  RATmaterials *out,*tmp;
  int i,j,nMaterials;

  if(!ratObj)return(NULL);

  if(ratObj->ratMatUsed){
    if(nMat)*nMat = *(ratObj->nMaterialsUsed);
    return(ratObj->ratMatUsed);
  }

  /* get a list of all of the materials */
  tmp = RATgetMaterials(ratObj,&nMaterials);
  /* if the material isnt used, reorder */
  if(!(out=(RATmaterials *)CALLOC(nMaterials,sizeof(RATmaterials)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  for(i=0,j=0;i<nMaterials;i++){
    if(tmp[i].used && tmp[i].type != TRANSPARENT){
      out[j]=tmp[i];
      j++;
    }
  }
  if(nMat)*nMat = j;
  ratObj->ratMatUsed=out;
  ratObj->nMaterialsUsed = (int *)v_allocate(1,sizeof(int));
  *(ratObj->nMaterialsUsed) = j;

  return(out);
}

/* set and load ratMaterials */
RATmaterials *RATgetMaterials(RATobj *ratObj,int *nMat){
  static char srmr[]="srm with reflectance",srmt[]="srm with transmittance",transparent[]="transparent",srm[]="srm",volumetric[]="volumetric";
  Standard_Material_List *material;
  int i,j;
  int nMaterials;
  char *name;
  char *materialType=NULL;

  if(!ratObj)return(NULL);

  nMaterials=RATgetNmaterials(ratObj);
  if(!ratObj->nMaterials)ratObj->nMaterials=(int *)v_allocate(1,sizeof(int));
  *(ratObj->nMaterials) = nMaterials;

  if(nMat)*nMat=nMaterials;

  /* if its already done, just return it */
  if(ratObj->ratMat != NULL)return(ratObj->ratMat);

  /* else allocate & load it */

  if(!(ratObj->ratMat=(RATmaterials *)CALLOC(nMaterials,sizeof(RATmaterials)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }

  /* load things up */
  for(i=0;i<nMaterials;i++){
    ratObj->ratMat[i].index= i;
    material=&(ratObj->materialbag->material_list->material[i]);
    name=ratObj->materialbag->material_names[i];
    if(material->srm.diffuse_reflectance && material->srm.diffuse_transmittance){
      materialType=srm;
      ratObj->ratMat[i].type = SRM;
    }else if(material->srm.diffuse_reflectance){
      ratObj->ratMat[i].type = SRM;
      materialType=srmr;
    }else if(material->srm.diffuse_transmittance){
      ratObj->ratMat[i].type = SRM;
      materialType=srmt;
    }else if(material->volume.uL){
      materialType=volumetric;
      ratObj->ratMat[i].type = VOLUMETRIC;
    }else{
      if(*(ratObj->materialbag->materials[i].material_type) == HIPL_TEXTURE_MAP){
/* more complex - need to look up what the materials used are */
        materialType=transparent;
        ratObj->ratMat[i].type = HIPL_TEXTURE_MAP;
      }else{
        materialType=transparent;
        ratObj->ratMat[i].type = TRANSPARENT;
      }
    }
    ratObj->ratMat[i].name = name;
    ratObj->ratMat[i].typeName = materialType;
    ratObj->ratMat[i].used=0;
    if( ratObj->ratMat[i].type == HIPL_TEXTURE_MAP ){
     { int isused=0,no_of_materials,I,this;
/* have to look up all materials used to flag them */
      for(j=0;j<ratObj->materialbag->material_list->useage->no_of_materials;j++){
       if(!strcmp(name,ratObj->materialbag->material_list->useage->names[j])){
        isused=1;
        j=ratObj->materialbag->material_list->useage->no_of_materials;
       }
      }
      if(isused==1){
       no_of_materials=find_number_of_materials_in_material_map(ratObj->materialbag->materials[i].imagemap);      
       for(I=0;I<no_of_materials;I++){
        if(ratObj->materialbag->materials[i].material_table[I]){
         this = (ratObj->materialbag->materials[i]).material_table[I][0].material[0];
         ratObj->ratMat[this].used=1;
        }
       }
      }
     }
    }else{ 
      for(j=0;j<ratObj->materialbag->material_list->useage->no_of_materials;j++){
        if(!strcmp(name,ratObj->materialbag->material_list->useage->names[j])){
          ratObj->ratMat[i].used=1;
        }
      }
    }
    ratObj->ratMat[i].nWavelengths = material->no_of_samples;
    ratObj->ratMat[i].wavelengths = material->wavelength;
    if(ratObj->ratMat[i].type == SRM ){
      ratObj->ratMat[i].diffuse_reflectance = material->srm.diffuse_reflectance;
      ratObj->ratMat[i].diffuse_transmittance = material->srm.diffuse_transmittance;
    }
  }

  return(ratObj->ratMat);
}

RATtree *RATgetRatTree(RATobj *ratObj){
  RATtree *allocateRatTree();

  if(ratObj->ratTree==NULL){
    ratObj->ratTree=allocateRatTree(ratObj->flagbag->max_ray_depth+1,ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,(ratObj->flagbag->sunlocation != NULL ? 1 : 0));
  }
  return(ratObj->ratTree);
}

int RATgetRTD(RATobj *ratObj){
  if(!ratObj)return(0);
  return(ratObj->materialbag->samples->max_diffuse_sampling_depth);
}

RATtree *deAllocateRatTree(RATtree *out){
  int i;
  if(out->intersectionPoints){
    for(i=0;i<out->n+1;i++){
      if(out->intersectionPoints[i])free(out->intersectionPoints[i]);
      out->intersectionPoints[i]=NULL;
    }
    free(out->intersectionPoints);
    out->intersectionPoints=NULL;
  }
  if(out->rayLengths){
    free(out->rayLengths);
    out->rayLengths=NULL;
  }
  if(out->ratmat){
    free(out->ratmat);
    out->ratmat=NULL;
  }
  if(out->diffusePathIntersectType){
    free(out->diffusePathIntersectType);
    out->diffusePathIntersectType=NULL;
  }
  if(out->directPathIntersectType){
    free(out->directPathIntersectType);
    out->directPathIntersectType=NULL;
  }
  if(out->hitSun){
    free(out->hitSun);
    out->hitSun=NULL;
  }
  if(out->hitSky){
    free(out->hitSky);
    out->hitSky=NULL;
  }
  if(out->diffuseRadiance){
    free(out->diffuseRadiance);
    out->diffuseRadiance=NULL;
  }
  if(out->directRadiance){
    free(out->directRadiance);
    out->directRadiance=NULL;
  }
  if(out->lengthToSun){
    free(out->lengthToSun);
    out->lengthToSun=NULL;
  }
  if(out->angleToSun){
    free(out->angleToSun);
    out->angleToSun=NULL;
  }
  out->n=0;
  return(NULL);
}

RATtree * allocateRatTree(int nRTD,int nBands,int infiniteSun){
  RATtree *out;
  int i,j,_MAX_SUNS=0;
  char *tmp;

  if((tmp=getenv("MAX_SUNS")) != NULL){
    _MAX_SUNS=atoi(tmp);
  }else _MAX_SUNS=MAX_SUNS;

  _MAX_SUNS=MAX(1,_MAX_SUNS);

  if(nRTD<=0)
    return(NULL);

  if(!(out=(RATtree *)CALLOC(_MAX_SUNS,sizeof(RATtree)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }

  for(j=0;j<_MAX_SUNS;j++){
    out[j].n=nRTD;
    if(!(out[j].intersectionPoints=(double **)CALLOC(nRTD+1,sizeof(double *)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    for(i=0;i<nRTD+1;i++){
      if(!(out[j].intersectionPoints[i]=(double *)CALLOC(3,sizeof(double)))){
        fprintf(stderr,"error in core allocation\n");
        exit(1);
      }    
    }
    if(!(out[j].rayLengths=(double *)CALLOC(nRTD+1,sizeof(double)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].ratmat=(int *)CALLOC(nRTD+1,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].diffusePathIntersectType=(int *)CALLOC(nRTD+1,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].directPathIntersectType=(int *)CALLOC(nRTD+1,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].hitSun=(int *)CALLOC(nRTD+1,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].hitSky=(int *)CALLOC(nRTD+1,sizeof(int)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].diffuseRadiance=(double *)CALLOC((nRTD+1)*nBands,sizeof(double)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(!(out[j].directRadiance=(double *)CALLOC((nRTD+1)*nBands,sizeof(double)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
    if(infiniteSun != 0){
      if(!(out[j].lengthToSun=(double *)CALLOC((nRTD+1),sizeof(double)))){
        fprintf(stderr,"error in core allocation\n");
        exit(1);
      }
      if(!(out[j].angleToSun=(double *)CALLOC((nRTD+1),sizeof(double)))){
        fprintf(stderr,"error in core allocation\n");
        exit(1);
      }
    }
  }
  return(out);
}


int RATtraceRay(RATobj *ratObj,double *from,double *direction,double *illuminationPosition){
  Ray ray,renders();
  int rtd;
  static int nRTD=0;
  static HitPoint *hitPoint=NULL;
  /* 
  ** Modified 9 May 2012 P.Lewis to provide
  ** a mechanism to not have to call calculate_reflectance_data()
  ** every time this function is called
  */
  static int sortReflectance = 1;
  double norm=1.0;
  ObjectList objectlist;
  Material_table material;
  int i,j,k,nSuns=0;
  int hitCamera;
  hitCamera=ratObj->hit_camera;

  if(!from || !direction)return(-1);
  ray.ray_length=0.;
  ray.lengthOfRaySoFar=0.;
  ray.rayLengthThroughObject=0.;
  ray.sourceOfRay = VIEWER;
  ray.origin.x = from[0];
  ray.origin.y = from[1];
  ray.origin.z = from[2];
  ray.direction.x=direction[0];
  ray.direction.y=direction[1];
  ray.direction.z=direction[2];
  if(ray.direction.x==0 && ray.direction.y==0 &&ray.direction.z==0){
    return(0);
  } 
  nSuns=ratObj->flagbag->nSuns=ratObj->nSuns;
  ray.direction=normalise(ray.direction);
  if(ratObj->flagbag->sunlocation || illuminationPosition){
    if(!ratObj->flagbag->sunlocation){
      ratObj->flagbag->sunlocation=(double *)v_allocate(3*nSuns,sizeof(double));
    }
    if(illuminationPosition){
      for(j=0;j<nSuns;j++)
      for(i=0;i<3;i++)ratObj->flagbag->sunlocation[i+j*3]=illuminationPosition[i+j*3]; 
    }
/* this means its been set on the cmd line */
    if(!ratObj->illumination->position){
      /* really this should be set by now, but just in case not */      
      for(i=0;i<nSuns;i++){
        if(! ratObj->illumination[i].position)
          ratObj->illumination[i].position =(Camera_op *)v_allocate(nSuns,sizeof(Camera_op));
        ratObj->illumination[i].position->ppoint.x=ratObj->flagbag->sunlocation[0+i*3];
        ratObj->illumination[i].position->ppoint.y=ratObj->flagbag->sunlocation[1+i*3];
        ratObj->illumination[i].position->ppoint.z=ratObj->flagbag->sunlocation[2+i*3];
      }
    }
  }


/* only do these if things have changed */
  if( !nRTD || ratObj->materialbag->samples->max_diffuse_sampling_depth>nRTD){
    if(hitPoint)free(hitPoint);
    nRTD = MAX(1, ratObj->materialbag->samples->max_diffuse_sampling_depth);
    hitPoint=(HitPoint *)v_allocate(nRTD+1,sizeof(HitPoint));
    for(j=0;j<nRTD+1;j++){
      hitPoint[j].nSuns=ratObj->nSuns;
      hitPoint[j].hitSun=(int *)v_allocate(ratObj->nSuns,sizeof(int));
      hitPoint[j].sunVector=(triplet *)v_allocate(ratObj->nSuns,sizeof(triplet));
      hitPoint[j].sunInteractionType=(int *)v_allocate(ratObj->nSuns,sizeof(int));      
      hitPoint[j].lambertian=(double *)v_allocate(ratObj->nSuns,sizeof(double));
      hitPoint[j].distanceBackToViewer=(double *)v_allocate(ratObj->nSuns,sizeof(double));
    }
    if(ratObj->ratTree && ratObj->materialbag->samples->max_diffuse_sampling_depth != nRTD ){
      if(ratObj->ratTree)ratObj->ratTree=deAllocateRatTree(ratObj->ratTree);
      ratObj->ratTree=allocateRatTree(nRTD,ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,(ratObj->flagbag->sunlocation != NULL ? 1 : 0));
    }
    if(!ratObj->ratTree)
      ratObj->ratTree=allocateRatTree(nRTD,ratObj->wavebandbag->sensor_wavebands->no_of_wavebands,(ratObj->flagbag->sunlocation != NULL ? 1 : 0));		
    
  }
  if(!(ratObj->materialbag->material_lut))
    allocate_material_list(ratObj->materialbag,ratObj->wavebandbag);
  for(i=0;i<nSuns;i++)
    calculate_sun_elevation_axis(&ratObj->illumination[i].sun_elevation_axis, ratObj->illumination[i].sun_Ptr);
  if(!ratObj->wavebandbag->lambda)
    ratObj->wavebandbag->lambda = (double *) v_allocate(ratObj->wavebandbag->sensor_wavebands->no_of_wavebands, sizeof(double));

  for(i=0;i<nSuns;i++)pre_calculate_sun_parameters(ratObj->illumination[i].sun_Ptr, &ratObj->illumination[i].theta_sun, &ratObj->illumination[i].phi_gap, &ratObj->wavebandbag->theta_gap, ratObj->illumination[i].sky_data_Ptr);

  /*
  ** Modified 9 May 2012 P. Lewis
  ** only call this next function is we need to as its quite expensive
  */
  if(sortReflectance){
    calculate_reflectance_data(ratObj->wavebandbag, ratObj->materialbag, ratObj->illumination, *ratObj->wavebandbag->lambda_min_Ptr, *ratObj->wavebandbag->lambda_width_Ptr, 0);
    if(ratObj->flagbag->fixed_wavelength)sortReflectance=0;
  }
  ratObj->globalPixelStats.nBins=1;
  { int n;
    n = ratObj->wavebandbag->sensor_wavebands->no_of_wavebands;
    ratObj->globalPixelStats.nBands=n;
  }
 
  /* reset volume ray tracing stack counter */ 
  ratObj->whichFacet=0;
  objectlist.cloneTranslationFlag = 0;
  
  clearHitPoint(nRTD,hitPoint);
  for(rtd=0;rtd<=nRTD;rtd++){ 
    ray = renders((rtd == 0 ? hitCamera : 0),ratObj,&(hitPoint[rtd]),&rtd,&ray,ratObj->materialbag,ratObj->illumination,ratObj->wavebandbag,&objectlist,ratObj->bbox,ratObj->flagbag,&material);
    for(k=0;k<nSuns;k++)
    if(ratObj->flagbag->sunlocation){
      if(!ratObj->ratTree[k].lengthToSun)ratObj->ratTree[k].lengthToSun=(double *) v_allocate(nRTD+1, sizeof(double));
      if(!ratObj->ratTree[k].angleToSun)ratObj->ratTree[k].angleToSun=(double *) v_allocate(nRTD+1, sizeof(double));

      ratObj->ratTree[k].lengthToSun[rtd]=ratObj->flagbag->lengthToSun[k];
      ratObj->ratTree[k].angleToSun[rtd]=ratObj->flagbag->sunrayangle[k];
    } 
    if(hitPoint[rtd].hitSky==1 || hitPoint[rtd].hitSky== -1)
      break;
    hitPoint[rtd].fromVectorLength = V_mod(vector_minus(hitPoint[rtd].fromLocation,hitPoint[rtd].location));
    ray.ray_length=1e20;ray.lengthOfRaySoFar=0.;ray.rayLengthThroughObject=-1.;
  }
  gatherPixelStatsForRay(ratObj,norm,&ratObj->globalPixelStats,nRTD,hitPoint,ratObj->materialbag,ratObj->illumination,ratObj->wavebandbag,&objectlist,ratObj->bbox,ratObj->flagbag,&material);

  /* load this information into a RATtree */
  return(rtd);
}


/* ################### */

/*
 *	group name storage area
 */

void	initialise_group_storage(group_Ptr,no_of_groups,max_len)
     Group	*group_Ptr;
     int	no_of_groups,max_len;
{
  int	i;
  
  if((group_Ptr->group_bboxes = (BBox **)CALLOC(no_of_groups,sizeof(BBox *)))==0)error1("librat:\terror in BBox allocation");
  if((group_Ptr->group_names = (char **)CALLOC(no_of_groups,sizeof(char *)))==0)error1("librat:\terror in char * allocation");
  for(i=0;i<no_of_groups;i++)
    if(((group_Ptr->group_names)[i] = (char *)CALLOC(max_len,sizeof(char)))==0)error1("librat:\terror in char allocation");
  
  group_Ptr->no_of_groups=0;
  return;
  
}

/*
 *	storage area
 */

int	initialise_contents_storage(contents,no_of_bboxes,no_of_triangles,no_of_beziers,no_of_spheres,no_of_cylinders,no_of_clones,no_of_dems)
     Contents	*contents;
     int	no_of_bboxes,no_of_triangles,no_of_beziers,no_of_spheres,no_of_cylinders,no_of_clones,no_of_dems;
{
  if(no_of_bboxes!=0)if((contents->bbox 	= (BBox *)CALLOC(no_of_bboxes,sizeof(BBox)))==0)
    error1("librat: error in contents box memory storage 1");
  if(no_of_triangles!=0)if((contents->ttriangle	= (fFacet *)CALLOC(no_of_triangles,sizeof(fFacet)))==0)error1("librat: error in contents box memory storage 2");
  if(no_of_beziers!=0)if((contents->bezier	= (Bezier4 *)CALLOC(no_of_beziers,sizeof(Bezier4)))==0)error1("librat: error in contents box memory storage 3");
  if(no_of_spheres!=0)if((contents->sphere	= (Sphere *)CALLOC(no_of_spheres,sizeof(Sphere)))==0)error1("librat: error in contents box memory storage 4");
  if(no_of_cylinders!=0)if((contents->cylinder	= (Cylinder *)CALLOC(no_of_cylinders,sizeof(Cylinder)))==0)error1("librat: error in contents box memory storage 5");
  if(no_of_clones!=0)if((contents->clone	= (Clones *)CALLOC(no_of_clones,sizeof(Clones)))==0)error1("librat: error in contents box memory storage 5");
  if(no_of_dems!=0)if((contents->dem	= (Dem *)CALLOC(no_of_dems,sizeof(Dem)))==0)error1("librat: error in contents box memory storage 6");
  contents->no_of_bboxes=0;
  contents->no_of_triangles=0;
  contents->no_of_beziers=0;
  contents->no_of_spheres=0;
  contents->no_of_cylinders=0;	
  contents->no_of_clones=0;	
  contents->no_of_dems=0;	
  return(1);
}

int setMaterialUseage(BigBag *bb,FlagBag *flagbag,MaterialBag *materialbag,char *name)
{
  FILE	*openFileForWrite();
  if(!(materialbag->material_list->useage=(MaterialUseage *)CALLOC(1,sizeof(MaterialUseage)))){
    fprintf(stderr,"error in core allocation\n");
    exit(0);
  }

  if(!(materialbag->material_list->useage->materials=(char **)CALLOC(bb->PRAT_MAX_MATERIALS,sizeof(char *)))){
    fprintf(stderr,"error in core allocation\n");
    exit(0);
  }
  if(!(materialbag->material_list->useage->names=(char **)CALLOC(bb->PRAT_MAX_MATERIALS,sizeof(char *)))){
    fprintf(stderr,"error in core allocation\n");
    exit(0);
  }
  return(1);
}

int sortIllumination(int *no_of_sun_wavelength_samples,FlagBag *flagbag,IlluminationBag *illuminations,char *Filename){
  char buffer[1024];
  FILE *fp=NULL;
  double fdum,fdum2,sunValue;
  int i,expand_filename(),ii;
  triplet normalise(triplet);
  IlluminationBag *illumination;
  char *filename;
  int _MAX_SUNS=0;
  char *tmp;

  if((tmp=getenv("MAX_SUNS")) != NULL){
       _MAX_SUNS=atoi(tmp);
  }else _MAX_SUNS=MAX_SUNS;

  _MAX_SUNS=MAX(1,_MAX_SUNS);

  /* need to pass an allocated string */
  for(ii=0;ii<_MAX_SUNS;ii++){
    filename=Filename;
    illumination=&(illuminations[ii]);
    if(illumination->sun_Ptr){free(illumination->sun_Ptr);illumination->sun_Ptr=NULL;}
    illumination->sun_Ptr=(triplet *)v_allocate(1,sizeof(triplet));
    illumination->sun_Ptr->z=1.0;
    if(!filename || !strlen(filename)){
      if(!filename && !(filename=(char *)CALLOC(strlen("default solar illumination")+1,sizeof(char)))){
        fprintf(stderr,"error in core allocation\n");
        exit(1);
      }
      illumination->direct_file=filename;
      sunValue = 1.; /* /MAX(0.00000001,fabs(sunDirection->z)); */
      if(illumination->sky_flag)*no_of_sun_wavelength_samples=illumination->sky_data_Ptr->hd.num_frame;
      else *no_of_sun_wavelength_samples=1;

      if(!illumination->sun_magnitude && !(illumination->sun_magnitude=(double *)CALLOC(*no_of_sun_wavelength_samples,sizeof(double)))){
        fprintf(stderr,"error in core allocation\n");
        exit(1);
      }
      for(i=0;i<*no_of_sun_wavelength_samples;i++){
        illumination->sun_magnitude[i]=sunValue;
      }
      strcpy(illumination->direct_file,"default solar illumination");
    }else{
      if(strcpy(illumination->direct_file,filename)==0)error1("parser:\terror in specifying -direct option");
      flagbag->direct_flag=1;
      if(!expand_filename(&(illumination->direct_file),"DIRECT_ILLUMINATION",FALSE))error2("librat:\terror opening direct irradaince file",illumination->direct_file);
      if((fp=fopen(illumination->direct_file,"r+"))==NULL)error2("parser:\terror in specifying -direct option - cannot open file",illumination->direct_file);
      *no_of_sun_wavelength_samples=0;
      while( (fgets(buffer,1024-1,fp))!=NULL){
        if((sscanf(buffer,"%lf %lf",&fdum,&fdum2))==2){
	  illumination->sun_magnitude[*no_of_sun_wavelength_samples]=fdum2/M_PI;
	  /*
	   *	direct irradiance values scaled by PI
	   *	so that op DN is RADIANCE
	   */
	  (*no_of_sun_wavelength_samples)++;
        }
      }
      if(illumination->sky_flag) /*  error check */
        if(*no_of_sun_wavelength_samples != illumination->sky_data_Ptr->hd.num_frame)error1("parser:\tinconsistent no of wavelength samples between -skymap data and -direct data\n\t\t(the number of wavelength samples in the direct illumination file must equal the number of frames in the skymap data)");
    }
  }
  return(1);
}

/*
** p.lewis@ucl.ac.uk 12 May 2012
** assume argv[argc-1] is the object file
** then check for mtllib lines in this
** then check how many lines in this & set bb->PRAT_MAX_MATERIALS
** accordingly
**
** This is to override the older mechanism where 
** bb->PRAT_MAX_MATERIALS comes from DEFAULT_PRAT_MAX_MATERIALS
** or getenv("PRAT_MAX_MATERIALS") which are now redundant
*/

int getMaxMaterials(char *objName,int verbose){
  int count = 0;
  int type_of_element(),getMaxMaterials();
  FILE *fp,*fp2;
  char    *liner,line[SHORT_STRING_LENGTH],option[SHORT_STRING_LENGTH];
  char    buf[SHORT_STRING_LENGTH],dum[SHORT_STRING_LENGTH];
  char objName2[SHORT_STRING_LENGTH];

  if(verbose)fprintf(stderr,"testing object file for material library %s\n",objName);
  fp=openFileForRead(&objName,"ARARAT_OBJECT",FATAL);
  while(fgets(line,SHORT_STRING_LENGTH,fp)){
    liner=line;
    if(sscanf(liner,"%s",option)==1){
      liner=strchr(liner,option[0])+strlen(option);
      if(!strcmp(option,"#include")){
        if(sscanf(liner,"%s",objName2)==1){
          if(verbose)fprintf(stderr,"#include %s",objName2);
          /* a moot point -- should we allow multiple material files? */
          count += getMaxMaterials(objName2,verbose);
        }
      }else switch(type_of_element(option)){
        case MTLLIB_RET:
          /* so the matrial filename should be liner */
          if(sscanf(liner,"%s",objName2)==1){
            liner = objName2;
            if(verbose)fprintf(stderr,"testing material library %s\n",liner);
            fp2=openFileForRead(&liner,"MATLIB",TRUE);
            /* get a line, check there is a string to read & check it doesnt start with hash */
            while(fgets(buf,SHORT_STRING_LENGTH,fp2) != NULL )if(sscanf(buf,"%s",dum) && dum[0]!=35)
              count++;
            fclose(fp2);
          }
        break;
      }
    }
  }
  fclose(fp);
  if(verbose)fprintf(stderr,"Found up to %d materials in %s and preceding files\n",count+getNdefaultMaterials(),objName);
  return count;
}


RATobj *RATinit(int argc,char **argv){
  BigBag *bb=NULL;
  int _MAX_SUNS=0;
  char *tmp;
  /*
  ** p.lewis@ucl.ac.uk 12 May 2012
  */
  int getNdefaultMaterials(),atoi();
  int i=0;
  /* end */
  if((tmp=getenv("MAX_SUNS")) != NULL){
     _MAX_SUNS=atoi(tmp);
  }else _MAX_SUNS=MAX_SUNS;

  _MAX_SUNS=MAX(1,_MAX_SUNS);

  if(!(bb=globalRATObj=(BigBag *)CALLOC(1,sizeof(BigBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  
  bb->globalArgc=argc;
  bb->globalArgv=argv;
  bb->nSuns=_MAX_SUNS;
  /* which contains:
     IlluminationBag *illumination;
     FlagBag        *flagbag;
     WavebandBag    *wavebandbag;
     MaterialBag    *materialbag;
  */
  if(!(bb->illumination=(IlluminationBag *)CALLOC(_MAX_SUNS,sizeof(IlluminationBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }  
  if(!(bb->skybag=(IlluminationBag *)CALLOC(1,sizeof(IlluminationBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }  
  if(!(bb->materialbag=(MaterialBag *)CALLOC(1,sizeof(MaterialBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }  
  if(!(bb->wavebandbag=(WavebandBag *)CALLOC(1,sizeof(WavebandBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }  
  if(!(bb->flagbag=(FlagBag *)CALLOC(1,sizeof(FlagBag)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  } 

  /*
  ** p.lewis@ucl.ac.uk 12 May 2012
  ** assume argv[argc-1] is the object file
  ** then check for mtllib lines in this
  ** then check how many lines in this & set bb->PRAT_MAX_MATERIALS
  ** accordingly
  **
  ** This is to override the older mechanism where 
  ** bb->PRAT_MAX_MATERIALS comes from DEFAULT_PRAT_MAX_MATERIALS
  ** or getenv("PRAT_MAX_MATERIALS") which are now redundant
  **
  ** find we also need to know the verbose level at this point
  ** so get that asd well
  */

  /*bb->PRAT_MAX_MATERIALS=DEFAULT_PRAT_MAX_MATERIALS;
  if(getenv("PRAT_MAX_MATERIALS")){
    bb->PRAT_MAX_MATERIALS = MAX(DEFAULT_PRAT_MAX_MATERIALS,atoi(getenv("PRAT_MAX_MATERIALS")));
  }
  */

  for(i=1;i<argc;i++)
    if(!strcasecmp(argv[i],"--ratv") || !strcasecmp(argv[i],"--v") || !strcasecmp(argv[i],"-ratv")  || !strcasecmp(argv[i],"-v")){
      if(i<argc-1)RATsetVerboseLevel((RATobj *)bb,atoi(argv[++i]));else RATsetVerboseLevel((RATobj *)bb,1);
      break;
    }
  ((RATobj *)bb)->PRAT_MAX_MATERIALS = getMaxMaterials(argv[argc-1],RATgetVerboseLevel((RATobj *)bb)) + getNdefaultMaterials();
  /* end */
  if(!(bb->material_name=(char **)CALLOC(bb->PRAT_MAX_MATERIALS,sizeof(char *))) || !(bb->material_table=(Material_table *)CALLOC(bb->PRAT_MAX_MATERIALS,sizeof(Material_table)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  bb->hit_camera = 0;
  return((RATobj *)bb);
}

void	RATuseage(RATobj *bb)
{
  fprintf(stderr,"%s: [-RATm 1] [-RATl] [-RATsun_position x y z] [-RATr randomSeed] [-RATblack] [-RATblockSize blocksize] [-RATstoreVertex N] [-RATsensor_wavebands wavebands.dat] [-RATsun_fov 0 (degrees)] [-RATskymap skymap.hips] [-RATtolerance 0.00001] [-RATdirect directIllumination.dat] [-help | -RAThelp] [-RATv 0|1] [-RATn] ",bb->globalArgv[0]);
  RATuserPrintOptions(bb);
  fprintf(stderr," data_filename.obj\n");
  return;
}

void RATprintOptions(RATobj *bb){
  RATuseage(bb);
}

void RATsetNsuns(RATobj *bb,int i){
  bb->nSuns=i;
}

int RATgetNsuns(RATobj *bb){
  return(bb->nSuns);
}

void RATsetSun(RATobj *bb,double *a){
  FlagBag *flagbag;
  IlluminationBag *illumination;
  int i;

  flagbag=bb->flagbag;
  if(flagbag->illuminationvector){
    free(flagbag->illuminationvector);
    free(flagbag->lengthToSun);
    free(flagbag->sunrayangle);
  }
  flagbag->illuminationvector = (triplet *)v_allocate(bb->nSuns,sizeof(triplet));
  flagbag->lengthToSun = (double *)v_allocate(bb->nSuns,sizeof(double));
  flagbag->sunrayangle = (double *)v_allocate(bb->nSuns,sizeof(double));

  for(i=0;i<MAX(1,bb->nSuns);i++){
    illumination=&(bb->illumination[i]);
    if(illumination->sun_Ptr){
      illumination->sun_Ptr->x=a[0+i*3];
      illumination->sun_Ptr->y=a[1+i*3];
      illumination->sun_Ptr->z=a[2+i*3];
      *(illumination->sun_Ptr)=normalise(*(illumination->sun_Ptr));
    }
  }
  return;
}

/*
 *	command line parser
 */
int RATparser(RATobj *bb,int argc,char **argv,void *info,int *ii,int jj){    
  IlluminationBag	*illumination;
  FlagBag	*flagbag;
  WavebandBag *wavebandbag;
  MaterialBag *materialbag;
  
  int	i,j,quit=0,no_of_sun_wavelength_samples=0;
  char	sky_imagemap[1000],**Ptr,*p,*argvi;
  int	expand_filename();
  int found=0,istart;

  flagbag=bb->flagbag;
  illumination=bb->illumination;
  wavebandbag=bb->wavebandbag;
  materialbag=bb->materialbag;
  
  p=&sky_imagemap[0];
  Ptr=&p;

  i = *ii;

  argvi = argv[i]+jj;

  switch(*(argvi)){
  case 'U':	/* user arguments */
    istart=i;
    i+=RATuserParse(bb,i,argc,argv,info);
    if(istart <= i)found=1;else i=istart;
    break;
  case 'm':	/* max rtd */
    sscan_int(argv,argc,&i,(int)FATAL,&(flagbag->max_ray_depth));
    materialbag->samples->max_diffuse_sampling_depth=MAX(flagbag->max_ray_depth,materialbag->samples->max_diffuse_sampling_depth);
    found=1;
    break;
  case 'l':
    flagbag->local=1;found=1;break;
  case 'r':
    sscan_int(argv,argc,&i,(int)FATAL,&(flagbag->random_seed));found=1;
    if(flagbag->verbose)fprintf(stderr,"random seed %d\n",flagbag->random_seed);
    seed_randomise(flagbag->random_seed);
    break;
  case 'b':
    if(strncmp(argvi,"black",2)==0){
      flagbag->blackSky=1;
    }else{
      if(!sscan_int(argv,argc,&i,(int)NON_FATAL,&(flagbag->blocksize)))error1("parser:\terror in specifying -blocksize option");
      /* sometime get an error if blocksize not multiples of 3 */
      flagbag->blocksize-=(flagbag->blocksize)%6;
    }
    found=1;
    break;
  case 's':		 		
    if(strcmp(argvi,"storeVertex")==0){
      flagbag->vertexStore=atoi(argv[++i]);
      found=1;
    }else if(strncmp(argvi,"sensor_wavebands",3)==0){
      while( !quit && *argv[i+1] != '-' && i != (argc-2)){
	if(i==argc-2)quit=1;
	if(!(wavebandbag->sensor_filenames) && !(wavebandbag->sensor_filenames=(char **)CALLOC(MAX_SENSOR_FILES,sizeof(char *)))){
	  fprintf(stderr,"error in core allocation\n");
	  exit(1);
	}
	wavebandbag->sensor_filenames[wavebandbag->sensor_wavebands->no_of_wavebands]=c_allocate(2000);
	strcpy(wavebandbag->sensor_filenames[(wavebandbag->sensor_wavebands->no_of_wavebands)],argv[++i]);
	if(!expand_filename(&(wavebandbag->sensor_filenames[(wavebandbag->sensor_wavebands->no_of_wavebands)]),"RSRLIB",FALSE))error2("librat:\terror opening sensor relative spectral response file",wavebandbag->sensor_filenames[(wavebandbag->sensor_wavebands->no_of_wavebands)]);
	(wavebandbag->sensor_wavebands->no_of_wavebands)++;
      }
      wavebandbag->rsr_flag=1;
      found=1;
    }else if(strcmp(argvi,"sun_fov")==0){
      sscan_double(argv,argc,&i,FATAL,&(illumination->sun_fov));
      illumination->sun_fov = (illumination->sun_fov)/3.14159265358979323846;	
      /* normalise for d_sun_elevation */
      found=1;
   }else if(strcmp(argvi,"sun_position")==0){
      flagbag->sunlocation=(double *)v_allocate(3,sizeof(double));
      sscan_double(argv,argc,&i,FATAL,&(flagbag->sunlocation[0]));
      sscan_double(argv,argc,&i,FATAL,&(flagbag->sunlocation[1]));
      sscan_double(argv,argc,&i,FATAL,&(flagbag->sunlocation[2]));
      found=1;
    }else if(strcmp(argvi,"s")==0){
      flagbag->rowsandcols=(int *)i_allocate(2,sizeof(int));
      sscan_int(argv,argc,&i,(int)FATAL,&(flagbag->rowsandcols[0]));
      sscan_int(argv,argc,&i,(int)FATAL,&(flagbag->rowsandcols[1]));
      found = 1;
    }else if(strcmp(argvi,"skymap")==0){
      if(strcpy(sky_imagemap,argv[++i])==0){
	RATprintOptions(bb);
	error1("error in specifying -skymap option");
      }
      illumination->sky_flag=1;
      if(!expand_filename(Ptr,"SKY_ILLUMINATION",FALSE))error2("librat:\terror opening skymap file",sky_imagemap);
      mmap_read_hips_image(sky_imagemap,&(illumination->sky_data_Ptr->hd),&(illumination->sky_data_Ptr->data));
      get_skymap_wavelengths(0,&(illumination->sky_data_Ptr->hd),(wavebandbag->lambda_min_Ptr),(wavebandbag->lambda_width_Ptr));
      if(flagbag->direct_flag) /*  error check */
	if(no_of_sun_wavelength_samples != illumination->sky_data_Ptr->hd.num_frame)error1("parser:\tinconsistent no of wavelength samples between -skymap data and -direct data\n\t\t(the number of wavelength samples in the direct illumination file must equal the number of frames in the skymap data)");				
      found=1;
    }
    break;
  case 'd':	/* -direct */
    i+=sortIllumination(&no_of_sun_wavelength_samples,flagbag,illumination,argv[i+1]);
    found=1;
    break;
  case 't':				
    if(strcmp(argvi,"tolerance")==0){  /* ray length tol */
      sscan_double(argv,argc,&i,FATAL,&(flagbag->ray_length_tol));
    }
    found=1;
    break;
  case 'h':				/* -help */
    RATprintOptions(bb);exit(1);
    found=1;   
    break;
  case 'v':				/* verbose */
    flagbag->verbose=1;
    j=1+i;
    if(j<argc){
		if(strlen(argv[j]) == 1 && (argv[j][0] == '0' || argv[j][0] == '1')){
			flagbag->data_verbose = atoi(argv[j]);
			i++;
		}
	}
    found=1;
    break;
  case 'n':				
    flagbag->normal=1;               /* use normals */
    found=1;
    break;
  }
  *ii = i;
  return(found);
}

int	RATparse(RATobj *bb,int argc,char **argv,void *info){    
  IlluminationBag	*illumination;
  FlagBag	*flagbag;
  WavebandBag *wavebandbag;
  MaterialBag *materialbag;
  
  int	i,no_of_sun_wavelength_samples=0;
  char	err[100],sky_imagemap[1000],**Ptr,*p;
  int	expand_filename(),found=0,istart;
  void dummy_read_spectral_file();
 
  RATstart(bb); 
  flagbag=bb->flagbag;
  illumination=bb->illumination;
  wavebandbag=bb->wavebandbag;
  materialbag=bb->materialbag;
  
  p=&sky_imagemap[0];
  Ptr=&p;
  /*
   *	defaults
   */
  bb->globalArgc=argc;
  bb->globalArgv=argv;
  bb->globalnBands=0;
  bb->globalOver=0.;
  bb->globalDoLidar=0;
  bb->globalnRTD=1;
  bb->nFacets=0;
  bb->whichFacet=0;
  bb->nFacetBlock=1024;
  bb->facets=NULL;
 
  if(!(materialbag->samples)){
    if(!(materialbag->samples=(Samples *)CALLOC(1,sizeof(Samples)))){
      fprintf(stderr,"error in core allocation\n");
      exit(1);
    }
  }
  materialbag->samples->max_diffuse_sampling_depth=0;
  materialbag->samples->no_of_diffuse_reflectance_samples=0;
  materialbag->samples->no_of_diffuse_transmittance_samples=0;
  materialbag->samples->no_of_shadow_rays=1;
  materialbag->samples->no_of_time_bins=0;
  materialbag->samples->startBinCount=0;
  materialbag->samples->endBinCount=0;
  materialbag->samples->binStep=0;

  if(!flagbag->pixel_variance){
    flagbag->pixel_variance=(PixelVarianceLimits *)v_allocate(1,sizeof(PixelVarianceLimits));
  }
  if(!wavebandbag->sensor_wavebands){
    wavebandbag->sensor_wavebands=(Sensor_Wavebands *)v_allocate(1,sizeof(Sensor_Wavebands));
  }
  if(!illumination->sun_Ptr){
    illumination->sun_Ptr=(triplet *)v_allocate(1,sizeof(triplet));
  } 
 
  illumination->sun_Ptr->x = illumination->sun_Ptr->y = 0.0;
  illumination->sun_Ptr->z = 1.0;
  /* need this for core dumper */
  flagbag->ray_length_tol=RAY_LENGTH_TOL;
  flagbag->parallel=0;
  flagbag->matUseage=0;
  flagbag->orthographic=0;
  flagbag->vertexStore=512;
  flagbag->blackSky=0; 
  flagbag->photonSpread=1;
  flagbag->lidar=0;
  flagbag->sz=-1e20;
  wavebandbag->sensor_wavebands->no_of_wavebands=0;
  flagbag->pixel_variance->no_of_rays_per_pixel=1;
  flagbag->pixel_variance->variance=0.0;
  flagbag->explicit_lambertian=0;
  flagbag->max_ray_depth=1;
  flagbag->inverse_direction=0;
  flagbag->PhaseFunction=0;
  flagbag->ellipse=0;
  flagbag->cameraFile=NULL;
  flagbag->distanceTol=0.;
  flagbag->angleTol=0.;
  flagbag->sizeTol=0.;
  flagbag->float_flag=1;
  flagbag->rtRatio=1.0;
 

  /* parser */ 
  for(i=1;i<argc;i++){
    found=0;
    /* starts with -- so built-in cmd line option */
    if(*(argv[i]) =='-' && *(argv[i]+1) == '-'){
      found=RATparser(bb,argc,argv,info,&i,2);
    }
   /* starts with -RAT so built-in cmd line option */
    if(*(argv[i]) =='-' && *(argv[i]+1) == 'R' && *(argv[i]+2) == 'A' && *(argv[i]+3) == 'T'){
      found=RATparser(bb,argc,argv,info,&i,4);
    }
   /* starts with -RAT so built-in cmd line option */
    if(*(argv[i]) =='-' && *(argv[i]+1) == 'r' && *(argv[i]+2) == 'a' && *(argv[i]+3) == 't'){
      found=RATparser(bb,argc,argv,info,&i,4);
    }
    /* next check to see if its a user option: still needs to start with - */
    if(found==0 && *(argv[i]) =='-'){
      istart=i;
      i+=RATuserParse(bb,i,argc,argv,info);
      if(i >= istart){
	/* then we found a user option */
	found=1;
      }else i=istart;
    }
    /* next check to see if its an old-style option */
    if(found == 0 && *(argv[i]) =='-'){
      found=RATparser(bb,argc,argv,info,&i,1);
    }
    /* if its the last argument, then it could be the object file */
    if(found == 0 && i == argc-1){
      RATsetWavefrontFilename(bb,argv[i]);
      found=1;
    }
    /* otherwise its a bum option */
    if(found==0){
      sprintf(err,"parser:\tunrecognised option %s in parser",argv[i]);
      RATprintOptions(bb);
      error1(err);
    }
  } 
  /* material useage */
  flagbag->matUseage=setMaterialUseage(bb,flagbag,materialbag,wavebandbag->op_image_file);
  if(RATisWavefrontFile(bb) && !expand_filename(&(wavebandbag->wavefront_file),"ARARAT_OBJECT",TRUE))error2("librat:\terror opening librat wavefront format object file",wavebandbag->wavefront_file);
  if(flagbag->vertexStore){
    flagbag->blocksize=flagbag->vertexStore+1;
  }
  if(flagbag->verbose){
    fprintf(stderr,"%s:\n",argv[0]);
    fprintf(stderr,"\tVERBOSE flag on (-v option)\n");
  }
  if(!illumination->direct_file || !strlen(illumination->direct_file))
    sortIllumination(&no_of_sun_wavelength_samples,flagbag,illumination,illumination->direct_file);
  /*
   *    read sensor rsr data
   */
  if(!wavebandbag->rsr_flag)dummy_read_spectral_file(wavebandbag->sensor_wavebands);
  else
    read_spectral_file(flagbag->verbose,wavebandbag->sensor_filenames,wavebandbag->rsr_flag,&flagbag->fixed_wavelength,wavebandbag->sensor_wavebands);
  calculate_reflectance_data(bb->wavebandbag, bb->materialbag, bb->illumination, *bb->wavebandbag->lambda_min_Ptr, *bb->wavebandbag->lambda_width_Ptr, 0);  
  bb->globalnBands=bb->wavebandbag->sensor_wavebands->no_of_wavebands;
   /*
   *    initialise storage area(s)
   */
  initialise_wavefront_storage(flagbag->blocksize,&bb->vertices);
  return(1);
}

void	initialise_reflectance_storage(samples,no_of_wavebands)
     Samples	*samples;
     int	no_of_wavebands;
{
  samples->no_of_time_bins = MAX(samples->no_of_time_bins,1);
  if((samples->result=(double *)CALLOC(no_of_wavebands*samples->no_of_time_bins,sizeof(double)))==0)error1("initialise_reflectance_storage:\terror allocating memory");
  if((samples->wavelength=(int *)CALLOC(no_of_wavebands,sizeof(int)))==0)error1("initialise_reflectance_storage:\terror allocating memory");
  return;
}

/*
 *	initialise data flags
 */

void	initialise_flagbag(BigBag *bb)
{
  static PixelVarianceLimits	*pixel_variance=NULL;
  static Restart *restart=NULL;
  FlagBag	*flagbag;

  flagbag=bb->flagbag;

 /* bug here that logic wasnt clear : Fri  1 May 2015 14:19:30 BST
  * fixed with extra brackets but not sute the logic is correct
  */
  if((!restart) && (!(restart=(Restart *)CALLOC(1,sizeof(Restart))) || !(pixel_variance=(PixelVarianceLimits *)CALLOC(1,sizeof(PixelVarianceLimits))))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }

  flagbag->reverse=1;
  flagbag->nGFunctionSamples=16;
  flagbag->restart_Ptr=restart;
  flagbag->verbose=0;
  flagbag->test_image_op=0;
  flagbag->fixed_wavelength=0;
  flagbag->explicit_lambertian=0;
  flagbag->local=1;
  flagbag->normal=0;
  flagbag->image_op=1;
  flagbag->data_verbose=0;
  flagbag->float_flag=0;
  flagbag->direct_flag=0;
  flagbag->blocksize=3336;
  flagbag->random_seed=1;
  flagbag->joint_probability=0;
  flagbag->pixel_variance=pixel_variance;
  flagbag->local_height=0;
  
  return;
}

/*
 *	initialise skybag
 */

void	initialise_skybag(BigBag *bb){
  static     Image_characteristics *sky_data=NULL;
  static char *direct_file=NULL;
  static triplet *solar=NULL;
  static double *sun_magnitude=NULL;
  
  if(!sky_data){
    direct_file=(char *)v_allocate(MAX_STRING_LENGTH,sizeof(char));
    sky_data=(Image_characteristics *)v_allocate(1,sizeof(Image_characteristics));
    solar=(triplet *)v_allocate(1,sizeof(triplet));
    sun_magnitude=(double *)v_allocate(MAX_WAVEBANDS,sizeof(double));
  } 
  strcpy(direct_file,"");

  bb->skybag->sky_data_Ptr=sky_data;
  bb->skybag->sun_magnitude= sun_magnitude;
  bb->skybag->sky_black=1;
  bb->skybag->sun_Ptr= solar;
  bb->skybag->sky_flag=0;
  bb->skybag->sun_fov=0.0;
  bb->skybag->theta_sun=0.0;
  bb->skybag->phi_gap=0.0;
  bb->skybag->direct_file=direct_file;
  
  return;
}

/*
 *	initialise waveband bag
 */

void	initialise_wavebandbag(BigBag *bb){
  static double *lambda_min=NULL,*lambda_width=NULL;
  static char **sensor_filenames=NULL,**rsr_filename=NULL;
  static char *wavefront_file=NULL;
  static Sensor_Wavebands  *sensor_wavebands=NULL;
  WavebandBag 	*wavebandbag;

  if(!lambda_min && (
!(lambda_min=(double *)CALLOC(1,sizeof(double)))  ||
!(lambda_width=(double *)CALLOC(1,sizeof(double))) || 
!(sensor_filenames=(char **)CALLOC(MAX_SENSOR_FILES,sizeof(char *))) || 
!(wavefront_file=(char *)CALLOC(MAX_STRING_LENGTH,sizeof(char))) || 
!(rsr_filename=(char **)CALLOC(MAX_SENSOR_FILES,sizeof(char *))) || 
!(sensor_wavebands=(Sensor_Wavebands *)CALLOC(1,sizeof(Sensor_Wavebands))))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }

  *lambda_min=0.0;
  *lambda_width=1.0;
  wavebandbag=bb->wavebandbag;
  
  wavebandbag->sensor_filenames=sensor_filenames;
  wavebandbag->wavefront_file= wavefront_file;
  wavebandbag->lambda_min_Ptr= lambda_min;
  wavebandbag->lambda_width_Ptr= lambda_width;
  wavebandbag->op_image_file=NULL;
  wavebandbag->rsr_filename=rsr_filename;
  wavebandbag->rsr_flag=0;
  wavebandbag->theta_gap=1;
  wavebandbag->sensor_wavebands= sensor_wavebands;
  
  return;
}

void	initialise_materialbag(materialbag,material_table,material_list,samples)
     MaterialBag *materialbag;
     Material_table	*material_table;
     Material_List	*material_list;
     Samples		*samples;
{
  material_list->useage=NULL;
  materialbag->materials= material_table;
  materialbag->material_list= material_list;
  materialbag->samples= samples;
  
  return;
}

void	initialise_image_characteristics(image_characteristics,op_image_file)
     Image_characteristics	*image_characteristics;
     char			*op_image_file;
{
  image_characteristics->imagename=op_image_file;
  
  return;
}

void RATstart(RATobj *bb){
  int atoi(),i;
  /*
   *	various initialisations
   */  
  initialise_flagbag((BigBag *)bb);
  initialise_skybag((BigBag *)bb);
  initialise_wavebandbag((BigBag *)bb);
  
  bb->mmap_flag=1;
  
  /*
   *	setup
   */
  bb->max_number_of_groups=50000;
  bb->current_mtl= bb->material_table;
  pload_identity_matrix4(&bb->m_inv_reverse);
  pload_identity_matrix4(&bb->m_inverse_fwd);
  if(getenv("MAX_GROUPS")){
    bb->max_number_of_groups=MAX(bb->max_number_of_groups,atoi(getenv("MAX_GROUPS")));
  }
  initialise_group_storage(&bb->group,bb->max_number_of_groups,SHORT_STRING_LENGTH); 
  for(i=0;i<bb->PRAT_MAX_MATERIALS;i++)bb->material_name[i]=c_allocate(SHORT_STRING_LENGTH);
  if(!(bb->material_list.material=(Standard_Material_List *)CALLOC(bb->PRAT_MAX_MATERIALS,sizeof(Standard_Material_List))) || !(bb->bbox=(BBox *)CALLOC(1,sizeof(BBox)))){
    fprintf(stderr,"error in core allocation\n");
    exit(0);
  }
  
  bb->materialbag->material_names=bb->material_name;
  initialise_materialbag(bb->materialbag,bb->current_mtl,&bb->material_list,&bb->samples);

  seed_randomise(bb->flagbag->random_seed);


  return;
}


void RATreadObject(RATobj *bb){

  FILE *fp;

  FlagBag *flagbag;
  WavebandBag *wavebandbag;
  MaterialBag *materialbag;

  flagbag=bb->flagbag;
  wavebandbag=bb->wavebandbag;
  materialbag=bb->materialbag; 

  /*
   *	open object file
   */
  fp=openFileForRead(&(wavebandbag->wavefront_file),"ARARAT_OBJECT",FATAL);
  
  /*
   *	print verbose header for object format
   */
  if(flagbag->data_verbose)fprintf(stderr,"{GLOBAL");
  
  /*
   *	read default materials
   */
  if(!(materialbag->materials))materialbag->materials=bb->material_table;
  read_default_materials((BigBag *)bb,flagbag->data_verbose,materialbag->material_list,materialbag->materials,bb->material_name);

  bb->timer=bb->level=0;
  
  /*
   *	read object file
   */
  parse_prat_wavefront_data((BigBag *)bb,flagbag->data_verbose,bb->bbox,bb->bbox,fp,&bb->level,&bb->group,&bb->current_mtl,&bb->vertices,&bb->normals,&bb->locals,flagbag->normal,flagbag->local,&bb->m_inv_reverse,&bb->m_inverse_fwd,
bb->material_name,materialbag->material_list,materialbag->materials,flagbag->vertexStore,flagbag->angleTol,flagbag->distanceTol,flagbag->sizeTol);
  
  /*
   *	initialise reflectance data storage
   */
  initialise_reflectance_storage(materialbag->samples,wavebandbag->sensor_wavebands->no_of_wavebands);
  
  if(materialbag->samples->binStep)bb->lidar=TRUE;else bb->lidar=FALSE;
  fclose(fp);
  return;
}
