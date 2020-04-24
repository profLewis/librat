#ifndef _REFLECTANCE_
#define _REFLECTANCE_
#endif

#ifndef REFLECTANCE_H
#define REFLECTANCE_H

triplet rotateBymatrix(triplet v,double *m,int transpose);
triplet calculate_diffuse_sample_ray_direction(triplet *normal_Ptr);
void    setReflectanceValue(int *timer,int mat_count,double *depth_value,double *out,double *presult,double f,int C);
void    *calculate_current_reflectance(int isDiffuse,int m,double material_proportion,int mode,CurrentReflectanceBag *current_reflectance_bag,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,double *out,double *depth_value,int *tree_depth_ptr,Material_table *material_Ptr,int *mat_count);
int test_calculate_current_reflectance(int m,double material_proportion,int mode,CurrentReflectanceBag *current_reflectance_bag,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,double *out,double *depth_value,int *tree_depth_ptr,Material_table *material_Ptr,int *mat_count);
void    calculate_specific_reflectance(int w,double lambda,double material_proportion,MaterialBag *materialbag,double *out,Material_table *material_Ptr,int *mat_count);
Ray     apply_diffuse_model(HitPoint *hitPoint,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,FlagBag *flagbag,int reflectance_type,Material_table *material_Ptr,triplet *normal_Ptr,triplet *plane_normal_Ptr,int *tree_depth_Ptr,ObjectList *objectlist_Ptr,BBox *bbox_Ptr);
Ray     apply_shadow_model(int l,HitPoint *hitPoint,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,FlagBag *flagbag,Material_table *material_Ptr,triplet *normal_Ptr,triplet *plane_normal_Ptr,int *tree_depth_Ptr,ObjectList *objectlist_Ptr,BBox *bbox_Ptr);
int     apply_bump_map(Material_table *material,triplet *normal,double *correction_ratio);
int     apply_material_map(Material_table *material,int flag);
void calculate_local_coords(Material_table *material,ObjectList *objectlist_Ptr,int flag,double *correction_ratio,int type);
Material_table *apply_local_coordinates(Material_table *material,ObjectList *objectlist_Ptr,triplet *plane_normal,MaterialBag *materialbag,int type);
Material_table  *get_reflection_ray_value(MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,int *wavelength,triplet *normal,triplet *plane_normal,ObjectList *objectlist_Ptr);

#endif
