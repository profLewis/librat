#ifndef _FRATP
#define _FRATP
int isNewObjectMaterial(RATobj *ratObj,char *material,int objectType);
char *typeTranslate(int);
int RATgetNmaterials(RATobj *ratObj);
int RATgetVerboseLevel(RATobj *ratObj);
void RATsetVerboseLevel(RATobj *ratObj,int level);
void pl(void);
void printHistogram(RATdistribution *d,char *name,char *histogramType,int type,char *material,double numberdensity,double *min,double *max);
void RATprintObjects(RATobj *ratObj);
void viewClone(RATobj *ratObj,struct Clone *c,int *f,double start,double span);
void *viewBLP(RATobj *ratObj,struct BiLinearPatch_struct *n,int *f);
void *viewPlane(RATobj *ratObj,struct Plane_struct *n,int *f);
void *viewSphericalDem(RATobj *ratObj,struct Spherical_Dem_struct *n,int *f);
void *viewDem(RATobj *ratObj,struct Dem_struct *n,int *f);
void *viewDisk(RATobj *ratObj,struct Disk_struct *n,int *f);
void *viewtriangle(RATobj *ratObj,struct Facet_struct *n,int *f);
void loadWFObject(RATobj *ratObj,int material,int objectType,double *location,triplet *v,Vertex_normals *vertex_normals,Vertex_locals *local_coords,double *norm);
void *viewEllipse(RATobj *ratObj,struct Ellipse_struct *n,int *f);
void *viewCylinder(RATobj *ratObj,struct Cylinder_struct *n,int *f);
void *viewSphere(RATobj *ratObj,struct Sphere_struct *n,int *f);
void *viewBezier(RATobj *ratObj,struct Bezier4_struct *n,int *f);
double facetNormal(double *normal,struct Facet_struct *n);
void *viewtriangle(RATobj *ratObj,struct Facet_struct *n,int *f);
void freeClone(struct Clone *c,int *f);
void freeBLP(struct BiLinearPatch_struct *n,int *f);
void freePlane(struct Plane_struct *n,int *f);
void freeSphericalDem(struct Spherical_Dem_struct *n,int *f);
void freeDem(struct Dem_struct *n,int *f);
void freeDisk(struct Disk_struct *n,int *f);
void freeCylinder(struct Cylinder_struct *n,int *f);
void freeSphere(struct Sphere_struct *n,int *f);
void freeBezier(struct Bezier4_struct *n,int *f);
void freetriangle(struct Facet_struct *n,int *f);
void freeBBoxContents(BBox *bb,int *f);
void viewBBoxContents(RATobj *ratObj,BBox *bb,int *f,double start,double span);
void viewObject(RATobj *ratObj,int *f);
void freeObject(RATobj *ratObj,int *f);
void RATsetWavefrontFilename(RATobj *ratObj,char *filename);
char *RATgetWavefrontFilename(RATobj *ratObj);
int RATisWavefrontFile(RATobj *ratObj);
int RATprintObject(RATobj *ratObj);
RATvol *RATgetVolumetric(RATobj *ratObj,int *grid,double *extent);
int isNewObjectMaterial(RATobj *ratObj,char *material,int objectType);
void clearDistribution(RATdistribution *d);
char *typeTranslate(int type);
RATobject *allocateRatObject(RATobj *ratObj,int init,int nratObjs,double *min,double *max,int *nSamples,int nHistogramSamples,char *name);
void loadIntoHistogram(double v,RATdistribution *d,double wt);
void loadWFObject(RATobj *ratObj,int material,int objectType,double *location,triplet *v,Vertex_normals *vertex_normals,Vertex_locals *local_coords,double *norm);
int loadObject(RATobj *ratObj,int material,int objectType,double *location,double size,double *normal,int pass);
double *RATgetBBoxMax(RATobj *ratObj,double *max);
double *RATgetBBoxMin(RATobj *ratObj,double *min);
RATobject *RATgetObjects(RATobj *ratObj,int whichOne,double *min,double *max,int *nsamples,int nHistogramSamples,char *filenames);
int RATreadWavefrontFile(RATobj *ratObj);
double *RATgetSun(RATobj *ratObj);
char *RATgetProgramName(RATobj *ratObj);
int RATgetNWavebands(RATobj *ratObj,double *wavebands);
void RATsetNWavebands(RATobj *ratObj,int n,double *minwavelength,double *bandwidth);
int RATgetnMaterials(RATobj *ratObj);
void RATprintDefinedMaterials(RATobj *ratObj);
char *RATtranslateMaterial(RATobj *ratObj,int thisMat,int *index);
RATmaterials *RATgetMaterialsUsed(RATobj *ratObj,int *nMat);
RATmaterials *RATgetMaterials(RATobj *ratObj,int *nMat);
RATtree *RATgetRatTree(RATobj *ratObj);
int RATgetRTD(RATobj *ratObj);
RATtree *deAllocateRatTree(RATtree *out);
RATtree * allocateRatTree(int nRTD,int nBands,int infiniteSun);
Ray     renders(int hitCamera,RATobj *bb,HitPoint *hitPoint,int *tree_depth_Ptr,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,ObjectList *objectlist_Ptr,BBox *bbox_Ptr,FlagBag *flagbag,Material_table *material_table_Ptr);
int RATtraceRay(RATobj *ratObj,double *from,double *direction,double *illuminationPosition);
int setMaterialUseage(RATobj *bb,FlagBag *flagbag,MaterialBag *materialbag,char *name);
int sortIllumination(int *no_of_sun_wavelength_samples,FlagBag *flagbag,IlluminationBag *illuminations,char *Filename);
int getMaxMaterials(char *objName,int verbose);
RATobj *RATinit(int argc,char **argv);
void	RATuseage(RATobj *bb);
void RATprintOptions(RATobj *bb);
void RATsetNsuns(RATobj *bb,int i);
int RATgetNsuns(RATobj *bb);
void RATsetSun(RATobj *bb,double *a);
int RATparser(RATobj *bb,int argc,char **argv,void *info,int *ii,int jj);   
int	RATparse(RATobj *bb,int argc,char **argv,void *info);
void	initialise_reflectance_storage(Samples *samples,int no_of_wavebands);
void	initialise_flagbag(RATobj *bb);
void	initialise_skybag(RATobj *bb);
void	initialise_wavebandbag(RATobj *bb);
void	initialise_materialbag(MaterialBag *materialbag,Material_table *material_table,Material_List *material_list,Samples *samples);
void	initialise_image_characteristics(Image_characteristics *image_characteristics,char *op_image_file);
void RATstart(RATobj *bb);
void RATreadObject(RATobj *bb);


#endif





