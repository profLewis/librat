#include "prat.h"

int op_material_file(materials)
FILE	*materials;
{
	char	temp;
	while( fscanf(materials,"%c",&temp)==1)fprintf(stdout,"%c",temp);
	rewind(materials);return(1);
}

int op_material_filename(name)
char *name;
{
	fprintf(stdout,"mtllib %s\n",name);
	return(1);
}

int check_material_exists(material_file,material,material_mode)
FILE	*material_file;char	*material;int	material_mode;
{
	char	temp[100];
	while( fscanf(material_file,"%s",temp)==1){
		if(strcmp(temp,material)==0){
			if(material_mode==OLD){
			}
			rewind(material_file);return(1);
		}	/* -> found */
	}
	rewind(material_file);return(0);
}

int old_define_material(name,material,np)
double   *material;char *name;int np;
{           
	int	i;    
        fprintf(stdout,"#\tdefine material:\t%s\n",material);
        fprintf(stdout,"newmtl\t%s\nplain ",name);
	for(i=0;i<np;i++)fprintf(stdout,"%f ",material[i]);
	fprintf(stdout,"\n");
        return(0);
}
int use_material(name,debug)
char	*name;
{
	fprintf(stdout,"usemtl\t%s\n",name);
	if(debug)fprintf(stderr,"switching to material:\t%s\n",name);
	return(0);
}
