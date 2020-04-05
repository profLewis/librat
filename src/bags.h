#define MAX_BBOX_LIST 128

typedef struct {
	Image_characteristics *sky_data_Ptr;
	Image_characteristics *image_Ptr;
	double         *sun_magnitude;
	int             sky_black;
	triplet        *sun_Ptr;
	int             sky_flag;
	double          sun_fov;
	double          theta_sun;
	double          phi_gap;
	triplet         sun_elevation_axis;
	char           *direct_file;
	Camera_op	   *position;
}    IlluminationBag;


typedef struct {
	FILE *fp;
	char *name;
	Camera_ip camera_ip;
	Camera_op camera_op;	
	Image_characteristics	*image_characteristics;
} CameraFile;

           
typedef struct {
	Restart        *restart_Ptr;
	int             orthographic;
	int             verbose;
	int             test_image_op;
	int             fixed_wavelength;
	int             explicit_lambertian;
	int             local;
	int             joint_probability;
	int             inverse_direction;
	int             max_ray_depth;
	int             normal;
	int             image_op;
	int             random_seed;
	int             data_verbose;
	int             blocksize;
	int             direct_flag;
	int             float_flag;
	int             test_image_op_frame;
	int             ray_length;
	int             single_scattering;
	int             multiple_scattering;
	int             local_height;
	int             parallel;
	int             PhaseFunction;
	int             matUseage;
	int             ellipse;
        int             spheroid;
	int             nGFunctionSamples;
	double          ray_length_tol;
	PixelVarianceLimits *pixel_variance;
	CameraFile			*cameraFile;
	int					vertexStore;
	double 			angleTol,distanceTol,sizeTol;
/* store distance and reflectance contributions per pixel */
	double			*distanceStore;
	double			*reflectanceStore;
	int			doInfinite;
	triplet			infiniteOffset;
	int blackSky;
  double rtRatio;
        int reverse;
	int photonSpread;
	int lidar;
        double sz;
  double *sunlocation;
  double *sunrayangle;
  int nSuns;
  triplet *illuminationvector;
  double *lengthToSun;
  int isSunTest;
  int *rowsandcols;
}               FlagBag;

typedef struct {
	double         *lambda_min_Ptr;
	double         *lambda_width_Ptr;
	Sensor_Wavebands *sensor_wavebands;
	double          theta_gap;
	char          **rsr_filename;
	int             rsr_flag;
	char           *op_image_file;
	char          **sensor_filenames;
	char           *wavefront_file;
	double         *lambda;
}               WavebandBag;

typedef struct {
	Material_table *materials;
	Material_List  *material_list;
	Samples        *samples;
	Standard_Material_List *material_lut;
	char          **material_names;
}               MaterialBag;

#define MAX_STRING_LENGTH 2048
#define SHORT_STRING_LENGTH 1024
#define MAX_WAVEBANDS 1024
#define MAX_SENSOR_FILES 50

typedef struct {
  /* the index number of this material */
  int index;
  /* strings with the material names */
  char *name;
  /* binary: 1 if is used in this object else 0 */
  int used;
  /* a code for type */
  int type;
  /* a string telling you the material type */
  char *typeName;
  /* wavelengths */
  int nWavelengths;
  double *wavelengths;
  double *diffuse_reflectance;
  double *diffuse_transmittance;
  /* put volumetric in at some point */
 
} RATmaterials;

typedef struct {
  int n; /* maximum ray tree depth */
  int thisRTD; /* how many rtds actually used (<=n) */
  double **intersectionPoints;
  double *rayLengths;
  int *ratmat; /* index number of material of object */
  int *diffusePathIntersectType; /*  0 = refl; 1 = trans for diffuse term */
  int *directPathIntersectType; /* 0 = refl; 1 = trans for direct term */
  int *hitSun; /* 1 if hits sun 0 else */
  int *hitSky; /* 1 if hits sky 0 else */
  double *diffuseRadiance; /* radiance from diffuse illumination source */
  double *directRadiance;  /* radiance from direct illumination source */
  double *lengthToSun; /* length to defined sun position */
  double *angleToSun; /* angle between vector to point sun and illumination direction (degrees) */
} RATtree;

/* a volumetric representation of the scene */
/* you have one of these *per* material that is used */

typedef struct {
  int nothing;
}RATvol;

typedef struct {
  int nSamples;
  double min;
  double max;
  double step;
  double *data;
  FILE *fp;
  char *name;
} RATdistribution;

/* store of the object types: one for each type & material */
typedef struct {
  /* see rat/intersect_objects.h */
  /* e.g. FO 0, SPHERE 1, CYLINDER 2 */
  int type;
  char *materialName;
  /* internal material code */
  int material;
  /* the spatial extent of each */
  double min[3];
  double max[3];
  double delta[3];
  /* how many */
  int nObjects;
  /* size distribution */
  RATdistribution sizes;
  /* zenith angle distribution */
  RATdistribution angle;
  /* number density */
  float *nd;
  int nsamps[3];
  /* a place holder for other stuff at the moment */
  void *otherStuff;
  char *rawfilename;
  char *datafilename;
  FILE *datafp;
}RATobject;

typedef struct {
  IlluminationBag *skybag;
  IlluminationBag *illumination;
  FlagBag        *flagbag;
  WavebandBag    *wavebandbag;
  MaterialBag    *materialbag;
  char **material_name;
  Material_table *material_table;
  int PRAT_MAX_MATERIALS;
  Material_table *current_mtl;
  Matrix4 m_inv_reverse,m_inverse_fwd;
  int max_number_of_groups;
  Material_List	material_list;
  Samples samples;
  Group	group;
  Sensor_Wavebands sensor_wavebands;
  int timer;
  int level;
  VeRtIcEs vertices,normals,locals;
  BBox *bbox;
  int mmap_flag;

  int lidar;

  int globalArgc;
  char **globalArgv; 

  PixelStats globalPixelStats;

  int globalnBands;
  double globalOver;
  int globalDoLidar;
  int globalnRTD;

  fFacet *facets;
  int nFacets,whichFacet,nFacetBlock;
  RATtree *ratTree;
  int nSuns;

  /* material information */
  RATmaterials *ratMat;
  int *nMaterials;
  RATmaterials *ratMatUsed;
  int *nMaterialsUsed;
  int *materialTranslatorLut;

  /* object type information */
  int nObjectTypes;
  int *objectTypes;
  int objectTypeCapacity;
  char **objectMaterialNames;
  double *minObjectSize;
  double *maxObjectSize;

  /* volumetric representation */
  RATvol  *ratvol;

  /* a count of the actual objects in the scene */
  RATobject *ratObj;
  int nratObjs;
  int hit_camera;

  int nstore;
  void **store;
  /* a matrix we use for some calculations */
  double Matrix[16];
} BigBag;

typedef struct {
	double          lambertian;
	int             timer;
	int             reflectance_type;
	int             no_of_diffuse_reflectance_samples;
	int             no_of_diffuse_transmittance_samples;
	triplet        *look, *normal, *illumination, *plane_normal;
}               CurrentReflectanceBag;


