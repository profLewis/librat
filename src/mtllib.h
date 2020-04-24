#ifndef _MTLLIB_
#define _MTLLIB_

#define DEFAULT_PRAT_MAX_MATERIALS 1024

#ifndef LOCAL
#include "volume.h"
#endif
#include "rpv.h"

int getNdefaultMaterials(void);
int getArrayNo(double lambda,Standard_Material_List *material);
int     calculate_material_lut(int nBands,Standard_Material_List *out,double lambda,Material_List *material_list,int waveband,int material_no);
void    preprocess_bumpmap_data(Material_table *material_table_Ptr);
int     find_number_of_materials_in_material_map(ImageMap *material_map);
void    read_material_map_table(FILE *fp,int *no_of_materials_Ptr,Material_table *material_table,char **material_names);
void    read_material_map_file(Material_table *material_table,char *filename,int *no_of_materials_Ptr,char **material_names);
void    read_default_materials(RATobj *bb,int verbose,Material_List *material_list_Ptr,Material_table *material_table,char **material_names);
int     read_material_definition_file(RATobj *bb,int verbose,char **filename,Material_List *material_list_Ptr,char **material_names,int *no_of_materials_Ptr,Material_table *material_table);
int     is_double(char *str,double *data_Ptr);
double *sortRPVData(char *name,char *data,int *material,int no_of_materials,char **material_names);
int     read_volume_model(int verbose,char *filename,Material_table *material_Ptr,Material_List *material_list_Ptr,int *no_of_materials_Ptr,FILE *fp,char **material_names);
int     read_volume_material_file(RATobj *bb,int verbose,char *filename,Material_List *material_list_Ptr,int *no_of_materials_Ptr,Material_table *material_Ptr,char **material_names);
int     read_material_file(RATobj *bb,int verbose,char *filename,Material_List *material_list_Ptr,int *no_of_materials_Ptr,Material_table *material_Ptr,int white);
triplet spherical(Volume *v);
triplet elliptical(Volume *v);
triplet readFileEqual(Volume *v);

#endif
