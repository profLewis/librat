#ifndef _REFLECTANCE_
#define _REFLECTANCE_
typedef struct{
	double	*diffuse_reflectance;
	double	*diffuse_transmittance;
	double	proportion;
	int	diffuse_reflectance_flag;
	int	diffuse_transmittance_flag;
}Standard_Reflectance_Model;

/*
**	gives info on 'useage' of each material
**	defined in geometric object
*/

typedef struct{
	char	**materials;
	char	**names;
	int	no_of_materials;
	char	*filename;
	FILE	*fp;	
}MaterialUseage;
Material_table *apply_local_coordinates();
#endif
