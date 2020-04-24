#ifndef PRAT_WAVEFRONT_READ_H
#define PRAT_WAVEFRONT_READ_H


triplet Vmin(triplet old,triplet new);
triplet Vmax(triplet old,triplet new);
triplet vmin(triplet old,triplet new);
triplet vmax(triplet old,triplet new);
void    vertex_to_block(int *block_Ptr,int *vertex_in_block_Ptr,int vertex_number,int blocksize);
int     initialise_wavefront_storage(int blocksize,VeRtIcEs *vertices);
void    set_up_local_basis_system(triplet   *vertex,Vertex_locals   *local_coords);
int     blp_read(triplet  *vertex,int verbose,char *line,VeRtIcEs *vertices,VeRtIcEs *normals,VeRtIcEs *locals,triplet  *bbox_max,triplet  *bbox_min,int normal,int local,BiLinearPatch   *facet_Ptr,int vertexStore);
int     fo_read(triplet  *vertex,int verbose,char *line,VeRtIcEs *vertices,VeRtIcEs *normals,VeRtIcEs *locals,triplet  *bbox_max,triplet  *bbox_min,int normal,int local,fFacet  *facet_Ptr,int vertexStore);
int     sph_read(double  *bounds,Sphere  *sph,int verbose,char *line,VeRtIcEs *vertices,triplet  *bbox_max,triplet  *bbox_min,int vertexStore);
int     cyl_read(double *bounds,Cylinder *cyl,int verbose,char *line,VeRtIcEs *vertices,triplet  *bbox_max,triplet  *bbox_min,int closed,int vertexStore);
int     disk_read(double *bounds,Disk *disk,int verbose,char *line,VeRtIcEs *vertices,triplet  *bbox_max,triplet  *bbox_min,int vertexStore);
int     ellipse_read(double *bounds,Ellipse *ell,int verbose,char *line,VeRtIcEs *vertices,triplet  *bbox_max,triplet  *bbox_min,int vertexStore);
int     spheroid_read(double *bounds,Spheroid *ell,int verbose,char *line,VeRtIcEs *vertices,triplet  *bbox_max,triplet  *bbox_min,int vertexStore);
triplet multiply_matrix3_vector3(double *m,triplet *translate,triplet *v);
void    transform_bbox2(double scale,double *m,triplet *translate,triplet *bboxmin,triplet *bboxmax);
void    transform_bbox(matrix4 m,triplet *bboxmin,triplet *bboxmax);
void matrixCopy(double *m1,double *m0);
void matrixMult(double *out,double *a,double *b);
double *setXRotation(double *matrix,double rotation);
double *setYRotation(double *matrix,double rotation);
double *setZRotation(double *matrix,double rotation);
int     clone_read(double *bounds,Clones  *clone,int verbose,char *line,Group *group_Ptr,BBox *bbox_Ptr,BBox *transformed_bbox_Ptr);
int     plane_read(triplet  *normal_coords,double *dw,int verbose,char *line,VeRtIcEs *vertices,int vertexStore);
int     mtllib_read(RATobj *bb,int verbose,char *line,Material_List *material_list_Ptr,char **material_names,Material_table *material_table);
Material_table  *current_material_read(int verbose,char *line,char **material_names,Material_List *materialList,Material_table *material_table);
int     load_transformation_matrix(char *line,BBox *bbox_Ptr,matrix4 *m_inv_reverse_Ptr,matrix4 *m_inverse_fwd_Ptr);
int     vertex_read(char *line,VeRtIcEs *vertices,VeRtIcEs *normals,VeRtIcEs *locals,int normal,int local,int vertexStore);
int     normal_read(char *line,VeRtIcEs *vertices,VeRtIcEs *normals,int normal,int vertexStore);
int     local_coord_read(char *line,VeRtIcEs *vertices,VeRtIcEs *locals,int local,int vertexStore);
void    add_total_box_contents(Contents *total_box_contents_Ptr,Contents *contents_Ptr);
int     type_of_element(char *option);
Dem     *find_dem(BBox *bbox_Ptr);
Spherical_Dem   *find_spherical_dem(BBox *bbox_Ptr);
Cylinder        *find_cylinder(BBox *bbox_Ptr);
Spheroid *find_spheroid(BBox *bbox_Ptr);
Ellipse *find_ellipse(BBox *bbox_Ptr);
fFacet  *find_facet(BBox *bbox_Ptr);
fFacet  *delete_facet(BBox *bbox_Ptr,fFacet *fo);
BiLinearPatch   *find_blp(BBox *bbox_Ptr);
Plane   *find_plane(BBox *bbox_Ptr);
Sphere  *find_sphere(BBox *bbox_Ptr);
Disk    *find_disk(BBox *bbox_Ptr);
Clones  *find_clone(BBox *bbox_Ptr);
int     find_current_hit(struct Sort_Hit *out[6],int *timer_Ptr,struct Sort_Hit *base_hit_Ptr[6]);
void    check_material(Material_table *current_mtl);
int approximateContents(BBox *bbox_Ptr,double angleToler,double distanceTol,double sizeTol,void **deleted);
void loadObjectInfo(RATobj *bb,char *materialName,int objectType,double size);
char *getMaterialName(RATobj *bb,int type,void *object);
double getObjectSize(RATobj *bb,int type,void *object);
void loadObjectSizeType(RATobj *bb,struct Sort_Hit *hit);
int     parse_prat_wavefront_data(RATobj *bb,int verbose,BBox *top_root_bbox_Ptr,BBox *bbox_Ptr,FILE *fp,int *level,Group *group_Ptr,Material_table *current_mtl,VeRtIcEs *vertices,VeRtIcEs *normals,VeRtIcEs *locals,int normal,int local_coords,matrix4 *m_inv_reverse_prev,matrix4 *m_inverse_fwd_prev,char **material_names,Material_List *material_list_Ptr,Material_table *material_table,int vertexStore,double angleTol,double distanceTol,double sizeTol);

#endif
