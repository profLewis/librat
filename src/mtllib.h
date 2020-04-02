#ifndef _MTLLIB_
#define _MTLLIB_

#define DEFAULT_PRAT_MAX_MATERIALS 1024

#ifndef LOCAL
#include "volume.h"
#endif
#include "rpv.h"

typedef struct {
  int    	 			no_of_samples;
  double *wavelength;
  Standard_Reflectance_Model	srm;
  Volume						volume;
}Standard_Material_List;


typedef struct {
  Standard_Material_List	*material;
  int			no_of_materials;
  MaterialUseage		*useage;
}Material_List;

#endif
