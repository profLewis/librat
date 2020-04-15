#include "prat.h"
/*#undef __STDC__
#include "./dlfcn.h"*/
#ifdef PFAT
#include "pfat/pratRead.h"
extern MaterialBag *Materialbag; 
#endif 
extern	FILE 	*open_file_for_read();
extern	void	mmap_read_hips_image();
extern	void	*calloc();
triplet readFileEqual();
double	Random();
FILE *openFile();
#define CLOSE -1

int getArrayNo(double lambda,Standard_Material_List *material){
  int i;
  double dl;
  /* return nearest neighbour */
  for(i=1;i<material->no_of_samples;i++){
    if(lambda<=material->wavelength[i] && (dl=material->wavelength[i]-material->wavelength[i-1]) != 0){
      dl=(lambda-material->wavelength[i-1])/dl;
      if(dl<=0.5)return(i-1);
      return(i);
    }
  }
  return(material->no_of_samples -1);
}

int VOLUME_nZeniths=0;
double LADEccentricity=0;

int	calculate_material_lut(nBands,out,lambda,material_list,waveband,material_no)
     int nBands;
     double				lambda;
     Material_List			*material_list;
     int				waveband,material_no;
     Standard_Material_List	*out;
{
  int				array_no; 
  double			*d_allocate(),k,bigtheta,rhoc;
  if(!material_list->material[material_no].wavelength){
      return(0);
  }
  if(lambda<=material_list->material[material_no].wavelength[0]){
    array_no=0;
  }else{
    array_no= getArrayNo(lambda,&material_list->material[material_no]);
  }
  if(material_list->material[material_no].srm.diffuse_transmittance){
    material_list->material[material_no].srm.diffuse_transmittance_flag=TRUE;
    if(!(out->srm.diffuse_transmittance))out->srm.diffuse_transmittance=d_allocate(nBands);
    out->srm.diffuse_transmittance[waveband] = (material_list->material[material_no].srm.diffuse_transmittance[array_no]);
    out->srm.diffuse_transmittance_flag=1;
  }
  if(material_list->material[material_no].srm.diffuse_reflectance){
    if(!out->srm.diffuse_reflectance)out->srm.diffuse_reflectance=d_allocate(nBands);
    material_list->material[material_no].srm.diffuse_reflectance_flag=TRUE;
    out->srm.diffuse_reflectance[waveband] = (material_list->material[material_no].srm.diffuse_reflectance[array_no]);
    out->srm.diffuse_reflectance_flag=1;
  }
  return(1);
}

void	preprocess_bumpmap_data(material_table_Ptr)
     Material_table	*material_table_Ptr;
{
  ImageMap	imagemap;
  int	i,j;
  double	dz_du,dz_dv,z=0,zu=0,zv=0;
  pair	gap;
  
  imagemap = *(material_table_Ptr->imagemap);
  if(material_table_Ptr->imagemap->data.fdata)
    free(material_table_Ptr->imagemap->data.fdata);
 
  material_table_Ptr->imagemap->data.fdata = f_allocate(material_table_Ptr->imagemap->hd.rows * material_table_Ptr->imagemap->hd.cols * 4);
  material_table_Ptr->imagemap->hd.num_frame = 4;
  material_table_Ptr->imagemap->hd.pixel_format = PFFLOAT;
  gap.x=(double)(material_table_Ptr->imagemap->hd.cols)-1.0;
  gap.y=(double)(material_table_Ptr->imagemap->hd.rows)-1.0;
  
  for(i=0;i<material_table_Ptr->imagemap->hd.rows;i++)
    for(j=0;j<material_table_Ptr->imagemap->hd.cols;j++){
      switch(imagemap.hd.pixel_format){
      case PFBYTE:
	z = (double)( *(imagemap.data.bdata + i*material_table_Ptr->imagemap->hd.cols + j));
	if(i != (material_table_Ptr->imagemap->hd.rows - 1)) zv = (double)( *(imagemap.data.bdata + (i+1)*material_table_Ptr->imagemap->hd.cols + j));
	else zv=(double)( *(imagemap.data.bdata + j));
	if(j != (material_table_Ptr->imagemap->hd.cols - 1)) zu = (double)( *(imagemap.data.bdata + i*material_table_Ptr->imagemap->hd.cols + j + 1));
	else zu = (double)( *(imagemap.data.bdata + i*material_table_Ptr->imagemap->hd.cols));
	break;
      case PFFLOAT:
	z = ( *(imagemap.data.fdata + i*material_table_Ptr->imagemap->hd.cols + j));
	if(i != (material_table_Ptr->imagemap->hd.rows - 1)) zv = ( *(imagemap.data.fdata + (i+1)*material_table_Ptr->imagemap->hd.cols + j));
	else zv=( *(imagemap.data.fdata + j));
	if(j != (material_table_Ptr->imagemap->hd.cols - 1)) zu = ( *(imagemap.data.fdata + i*material_table_Ptr->imagemap->hd.cols + j + 1));
	else zu = ( *(imagemap.data.fdata + i*material_table_Ptr->imagemap->hd.cols));
	break;
      }
      dz_du = atan2((zu - z),1.0/gap.x);
      dz_dv = atan2((zv - z),1.0/gap.y);
      *(material_table_Ptr->imagemap->data.fdata + i*material_table_Ptr->imagemap->hd.cols + j) = sin(dz_du);
      *(material_table_Ptr->imagemap->data.fdata + material_table_Ptr->imagemap->hd.rows*material_table_Ptr->imagemap->hd.cols + i*material_table_Ptr->imagemap->hd.cols + j) = cos(dz_du);
      *(material_table_Ptr->imagemap->data.fdata + 2*material_table_Ptr->imagemap->hd.rows*material_table_Ptr->imagemap->hd.cols + i*material_table_Ptr->imagemap->hd.cols + j) = sin(dz_dv);
      *(material_table_Ptr->imagemap->data.fdata + 3*material_table_Ptr->imagemap->hd.rows*material_table_Ptr->imagemap->hd.cols + i*material_table_Ptr->imagemap->hd.cols + j) = cos(dz_dv);
    }
  material_table_Ptr->u = 1;
  material_table_Ptr->v = 1;
  /*mmap_write_hips_image("slope.hips",&(material_table_Ptr->imagemap->hd),&(material_table_Ptr->imagemap->data),0,NULL,0);		** TEST OP **/
  
  
}

/* 
**	determine max. number indexed on material map
*/

int	find_number_of_materials_in_material_map(material_map)
     ImageMap	*material_map;
{
  int	i,j,max_number=0;
  
  for(i=0;i<material_map->hd.rows;i++)
    for(j=0;j<material_map->hd.cols;j++)
      if((unsigned char)*(material_map->data.bdata + (material_map->hd.rows)*j + i) >(unsigned char)max_number)
	max_number= (unsigned char)*(material_map->data.bdata + (material_map->hd.rows)*i + j);
  
  return(max_number+1);
}

void	read_material_map_table(fp,no_of_materials_Ptr,material_table,material_names)
     FILE	*fp;
     int	*no_of_materials_Ptr;
     Material_table	*material_table;
     char	**material_names;
{
  int	i,found_material=0,index;
  char	material[1024];
  
  while(fscanf(fp,"%d %s",&index,material)==2){
    
    if(index < 0 || index > 255)error1("mtllib:\terror in material_map definition - material_map index must be in the range 0-255");
    
    found_material=0;
    
    for(i=0;i< *no_of_materials_Ptr;i++){
      if(strcmp(material,material_names[i])==0){
	found_material=1;
	if(material_table[i].diffuse_transmittance_flag)material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=1;
	if(material_table[i].diffuse_reflectance_flag)material_table[*no_of_materials_Ptr].diffuse_reflectance_flag=1;
	
	material_table[*no_of_materials_Ptr].material[index]=i;
	material_table[*no_of_materials_Ptr].material_proportions[index]=1.0;
	material_table[*no_of_materials_Ptr].material_table[index]= ( &(material_table[i]));
	
	material_table[*no_of_materials_Ptr].material_proportions[index]=1.0;
	i = *no_of_materials_Ptr;
      }
    }
  }
  if(!found_material)error1("mtllib:\tmaterial_map material not found");
  
  return;
}

void	read_material_map_file(material_table,filename,no_of_materials_Ptr,material_names)
     char	*filename;
     char	**material_names;
     int	*no_of_materials_Ptr;
     Material_table	*material_table;
{
  char	buf[1064];
  FILE	*fp,*open_file_for_read();
  int	no_of_materials;
  
  fp=open_file_for_read(filename);
  /*
  **	read HIPL-format material map
  */
  if(fscanf(fp,"%s",buf)!=1)error1("matllib:\terror in material_map file format");
  if(strcmp(buf,"material_map")==0 || strcmp(buf,"materialmap")==0 || strcmp(buf,"matmap")==0||strcmp(buf,"material_map:")==0 || strcmp(buf,"materialmap:")==0 || strcmp(buf,"matmap:")==0 );else error1("matllib:\terror in material_map file format:\n\t\tflag material_map expected");
  if(fscanf(fp,"%s",buf)!=1)error1("matllib:\terror in material_map file format");
  mmap_read_hips_image(buf,&(material_table[*no_of_materials_Ptr].imagemap->hd),&(material_table[*no_of_materials_Ptr].imagemap->data),"MATLIB");	
  
  /* only byte format supported at present */
  
  /* edited 3 April 2020 --- not sure this is needed
  if(material_table[*no_of_materials_Ptr].imagemap->hd.pixel_format != PFBYTE)error1("mtllib:\terror reading non-byte format of material_map material");
  */

  /* only single frame input format supported */
  
  switch(material_table[*no_of_materials_Ptr].imagemap->hd.num_frame){
  case 1:
    break;
  default:
    material_table[*no_of_materials_Ptr].imagemap->hd.num_frame=1;
  }
  
  material_table[*no_of_materials_Ptr].no_of_materials=1;
  no_of_materials=find_number_of_materials_in_material_map((material_table[*no_of_materials_Ptr].imagemap));
 
  if(material_table[*no_of_materials_Ptr].material)
    free(material_table[*no_of_materials_Ptr].material); 
  material_table[*no_of_materials_Ptr].material=i_allocate(no_of_materials);
 
  if(material_table[*no_of_materials_Ptr].material_table)
    free(material_table[*no_of_materials_Ptr].material_table);
 
  if( !(material_table[*no_of_materials_Ptr].material_table=(Material_table **)calloc(no_of_materials,sizeof(Material_table *)) ))error1("core allocation error");
 
  if(material_table[*no_of_materials_Ptr].material_proportions)
    free(material_table[*no_of_materials_Ptr].material_proportions); 
  material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(no_of_materials,sizeof(double));
  (material_table[*no_of_materials_Ptr].u)=0;
  (material_table[*no_of_materials_Ptr].v)=0;
  
  /*
  **	set material map material type
  */
  if(material_table[*no_of_materials_Ptr].material_type)
    free(material_table[*no_of_materials_Ptr].material_type);\

  material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
  *(material_table[*no_of_materials_Ptr].material_type)=HIPL_TEXTURE_MAP;
  
  /*
  **	read material table
  */
  if(fscanf(fp,"%s",buf)!=1)error1("matllib:\terror in material_map file format");
  if(strcmp(buf,"attributes")==0 || strcmp(buf,"attributes:")==0);else error1("matllib:\terror in material_map file format:\n\t\tflag attributes expected");
  
  read_material_map_table(fp,no_of_materials_Ptr,material_table,material_names);
  
  return;
}

void	read_default_materials(bb,verbose,material_list_Ptr,material_table,material_names)
     BigBag *bb;
     Material_List   *material_list_Ptr;
     char    **material_names;
     Material_table  *material_table;
     int	verbose;
{
  int	i,*no_of_materials_Ptr,len,nmat=0;
  static char	**material_name=NULL;
  static int timer=0,*imat=NULL;
  Material_table	*material_Ptr,*thismat=NULL;
  Standard_Material_List *thesemat=NULL;

  timer++;
  if(timer>1){
    /* clear out all materials */
    for(i=2;i<material_list_Ptr->no_of_materials;i++){
      thismat=&material_table[i];  
      thesemat = &(material_list_Ptr->material[i]);
      free(thismat->material);
      free(thismat->material_proportions);
      free(thismat->material_type);
      thismat->material=NULL;
      thismat->material_proportions=NULL;
      thismat->material_type=NULL;
      thismat->no_of_materials=0;
      free(material_names[i]);
      material_names[i]=NULL;
    }
    if(bb->material_list.useage){
      for(i=0;i<bb->material_list.useage->no_of_materials;i++){
        bb->material_list.useage->materials[i]=NULL;
        bb->material_list.useage->names[i]=NULL;
      }
      bb->material_list.useage->no_of_materials=0;
    } 
    material_list_Ptr->no_of_materials=2; 
    return;
  }
  if(!material_name && !(material_name=(char **)calloc(2,sizeof(char *)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  
  len=MAX(strlen("WHITE")+1,strlen("TRANSPARENT")+1);
  if(timer==1 &&(!(material_name[0]=(char *)calloc(len,sizeof(char))) || !(material_name[1]=(char *)calloc(len,sizeof(char))))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  
  strcpy(material_name[0],"WHITE");
  strcpy(material_name[1],"TRANSPARENT");
  
  no_of_materials_Ptr=&(material_list_Ptr->no_of_materials);
  nmat=0;
  if(timer==1 && !(imat=(int *)calloc(1,sizeof(int)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  thismat=&material_table[nmat];
  thesemat = &(material_list_Ptr->material[nmat]);
  thismat->material=imat;
  thismat->material_proportions=(double *)v_allocate(1,sizeof(double));
  thismat->material_type=b_allocate(1);
  thismat->no_of_materials=1;
  *(thismat->material)= nmat;
  *(thismat->material_proportions) = 1.0;
  *(thismat->material_type) = SRM;
  thismat->no_of_materials = 1;
  if(verbose)fprintf(stderr,"\n\t(default) diffuse (reflectance) material\t%s...\n",material_name[0]);
  
  nmat=0;
  /* WHITE material */
  material_Ptr=thismat=&(material_table[nmat]);
  if(timer==1 && !material_names[nmat] && !(material_names[nmat]=(char *)calloc(strlen(material_name[0])+1,sizeof(char)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  strcpy(material_names[nmat],material_name[0]);
  
  if(nmat>bb->PRAT_MAX_MATERIALS)error1("read_material_file:\tonly %d materials can be defined (set with environment variable PRAT_MAX_MATERIALS)",bb->PRAT_MAX_MATERIALS);
  thismat->diffuse_transmittance_flag=0;
  thismat->diffuse_reflectance_flag=1;
  thismat->transWeightingThreshold=0.0;
  thismat->transWeighting=0.0;
  thismat->reflWeighting=1.0;
  if(thesemat->srm.diffuse_reflectance)
    free(thesemat->srm.diffuse_reflectance);

  thesemat->srm.diffuse_reflectance=(double *)v_allocate(2,sizeof(double));
  thesemat->srm.diffuse_reflectance[0]=1.0;
  thesemat->srm.diffuse_reflectance[1]=1.0;
  if(thesemat->wavelength)
    free(thesemat->wavelength);

  thesemat->wavelength=(double *)v_allocate(2,sizeof(double));
  thesemat->wavelength[0]=0;
  thesemat->wavelength[1]=1e20;
  thesemat->no_of_samples=2;
  
  /* TRANSPARENT material */
  
  nmat++;
  material_Ptr=thismat=&(material_table[nmat]);
  thesemat = &(material_list_Ptr->material[nmat]);
  strcpy(material_name[1],"TRANSPARENT");
 
  if(thismat->material)free(thismat->material);
 
  thismat->material=i_allocate(1);
  if(timer==1)thismat->material_proportions=(double *)v_allocate(1,sizeof(double));
  if(timer==1)thismat->material_type=b_allocate(1);
  thismat->no_of_materials=1;
  *(thismat->material)=nmat;
  *(thismat->material_proportions) = 1.0;
  *(thismat->material_type) = M_TRANSPARENT;
  thismat->no_of_materials = 1;
  if(verbose)fprintf(stderr,"\n\t(default) transparent material\t%s...\n",material_name[1]);
  if(timer==1 && !material_names[nmat] && !(material_names[nmat]=(char *)calloc(strlen(material_name[1])+1,sizeof(char)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  } 
  strcpy(material_names[ nmat],material_name[1]);
  if(nmat>bb->PRAT_MAX_MATERIALS)error1("read_material_file:\tonly %d materials can be defined (set with environment variable PRAT_MAX_MATERIALS)",bb->PRAT_MAX_MATERIALS);
  thismat->diffuse_transmittance_flag=material_Ptr->diffuse_reflectance_flag=0;
  if(timer==1)thesemat->wavelength=(double *)v_allocate(2,sizeof(double));
  thesemat->wavelength[0]=0;
  thesemat->wavelength[1]=1e20;
  thesemat->no_of_samples=2;
  thismat->transWeightingThreshold=1.0;
  thismat->transWeighting=1.0;
  thismat->reflWeighting=0.0;  
  (*no_of_materials_Ptr)=2;
  
  return;
}

/*
** p.lewis@ucl.ac.uk 12 May 2012
** This needs to be consistent with the function above 
*/
int getNdefaultMaterials(){return 2;}
/*
**	read material definition file
**	materials defined:
**
**	diffuse	<material_name>		<material_file>
**	mix	<material_name> <material_proportion> <material_name> <material_proportion> ...
*/

int	read_material_definition_file(bb,verbose,filename,material_list_Ptr,material_names,no_of_materials_Ptr,material_table)
     BigBag *bb;
     char	**filename;
     Material_List	*material_list_Ptr;
     char	**material_names;
     int	*no_of_materials_Ptr;
     int	verbose;
     Material_table	*material_table;
{
  static int timer=0;
  FILE	*fp;
  char	buf[1024],*line,k_filename[1024],bigtheta_filename[1024],rhoc_filename[1024],proportion_str[1024],material_name[1024],dum[1024],material_filename[1024],material[1024];
  int	i,j,ok_flag,found_material,*i_allocate(),read_brdf_material_file(),white=0;
  double	proportion,transWeightingThreshold;
  int	read_material_file(),isRT=0,read_volume_material_file();
#ifdef PFAT
  int I,row,col,n;
  Standard_Material_List  *sml=NULL;
#endif
  timer++;
#ifdef DEBUG
  fprintf(stderr,"reading material library %s\n",*filename);
#endif
  if(verbose)fprintf(stderr,"reading material library %s\n",*filename);
  if(!(fp=openFile(*filename,TRUE,"MATLIB"))){
    error2("Error opening MATLIB",filename);
    exit(1);
  }
  while(fgets(buf,1024,fp) != NULL )	/* get line */
    if(sscanf(buf,"%s",dum)==1)
      if(dum[0]!=35){	/* hash -> comment */
	ok_flag=0;
	if(material_table[*no_of_materials_Ptr].imagemap)free(material_table[*no_of_materials_Ptr].imagemap);
  
	if(strcmp(dum,"volume")==0){
	  ok_flag=1;
	  line=strchr(buf,'e')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after volume flag in material file");
	  if(!material_table[*no_of_materials_Ptr].material){
            material_table[*no_of_materials_Ptr].material=i_allocate(1);
	    material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(1,sizeof(double));
	    material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
	  }
          material_table[*no_of_materials_Ptr].no_of_materials=1;
	  *(material_table[*no_of_materials_Ptr].material)= *no_of_materials_Ptr;
	  *(material_table[*no_of_materials_Ptr].material_proportions) = 1.0;
	  material_table[*no_of_materials_Ptr].no_of_materials = 1;
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\tvolume material\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line=strchr(line,material_name[0])+strlen(material_name);
	  if(sscanf(line,"%s",material_filename)!=1)error1("material_filename expected after volume flag in material file");
	  *(material_table[*no_of_materials_Ptr].material_type)=VOLUMETRIC;
	  read_volume_material_file(bb,verbose,material_filename,material_list_Ptr,no_of_materials_Ptr,&(material_table[*no_of_materials_Ptr]),material_names);
	  material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=1;
	  material_table[*no_of_materials_Ptr].diffuse_reflectance_flag=1;
	  
	  if(verbose)fprintf(stderr,"\n\t\t\t...read");
	  
	}else if(strcmp(dum,"srm")==0){
	  ok_flag=1;
	  if(strcmp(dum,"srm")==0)line=strchr(buf,'m')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after diffuse flag in material file");
          if(!material_table[*no_of_materials_Ptr].material){
	    material_table[*no_of_materials_Ptr].material=i_allocate(1);
	    material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(1,sizeof(double));
	    material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
	  }
          material_table[*no_of_materials_Ptr].no_of_materials=1;
	  *(material_table[*no_of_materials_Ptr].material)= *no_of_materials_Ptr;
	  *(material_table[*no_of_materials_Ptr].material_proportions) = 1.0;
	  *(material_table[*no_of_materials_Ptr].material_type) = SRM;
	  material_table[*no_of_materials_Ptr].no_of_materials = 1;
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose){
            fprintf(stderr,"\n\tdiffuse material\t%s...\n",material_name);
          }
          if(!(material_names[ *no_of_materials_Ptr ]))material_names[ *no_of_materials_Ptr ]=(char *)v_allocate(strlen(material_name)+1,sizeof(char));
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line+=strlen(material_name)+1;
	  /*line=strchr(line,(int)material_name[0]);*/
	  if(strcmp(dum,"srm")==0){
            if(!line||((isRT=sscanf(line,"%s %lf",material_filename,&transWeightingThreshold))!=2&&(isRT=sscanf(line,"%s",material_filename))!=1)){
	      fprintf(stderr,"%s messed up\n",line);
	      error1("material_filename expected after diffuse flag in material file");
	    }
	    if(isRT==1)transWeightingThreshold=0.5;
          }
	
#ifdef DEBUG
	  fprintf(stderr,"material file: %s\n",material_filename);
#endif
          white=0;		
          if(bb->wavebandbag->sensor_wavebands->no_of_wavebands == 1 && bb->wavebandbag->sensor_wavebands->sensor_rsr[0].rsr[0][0] == -1 ){
	    white=1;
          }
	  read_material_file(bb,verbose,material_filename,material_list_Ptr,no_of_materials_Ptr,&(material_table[*no_of_materials_Ptr]),white);
	  if(material_table[*no_of_materials_Ptr].diffuse_transmittance_flag==0){
	    material_table[*no_of_materials_Ptr].transWeightingThreshold=-1.0;
	    material_table[*no_of_materials_Ptr].transWeighting=0.;
	    material_table[*no_of_materials_Ptr].reflWeighting=1.;
	  }else{
	    material_table[*no_of_materials_Ptr].transWeightingThreshold=MIN(1.0,MAX(0.0,transWeightingThreshold));
	    if(material_table[*no_of_materials_Ptr].transWeightingThreshold==0.){
	      /* all refl */
	      material_table[*no_of_materials_Ptr].transWeighting=0.;
	      material_table[*no_of_materials_Ptr].reflWeighting=1.;
	      material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=0;
	      material_table[*no_of_materials_Ptr].transWeightingThreshold=-1.0;
	    }else if(material_table[*no_of_materials_Ptr].transWeightingThreshold==1.0){
	      /* all transm */
	      material_table[*no_of_materials_Ptr].transWeighting=1.0;
	      material_table[*no_of_materials_Ptr].reflWeighting=0.0;
	    }else{
/* transWeightingThreshold is trans / (refl + trans ) */
	      material_table[*no_of_materials_Ptr].transWeighting=1./material_table[*no_of_materials_Ptr].transWeightingThreshold;
	      material_table[*no_of_materials_Ptr].reflWeighting=1./(1. - material_table[*no_of_materials_Ptr].transWeightingThreshold);
	    }
	  }
	  if(verbose)fprintf(stderr,"\n\t\t\t...read");
#ifdef PFAT
	  sml=Materialbag->material_lut;	
	  /* pfat requires LUTs to be set up at this point */
	  for(I=0;I<3;I++)
	    calculate_material_lut(Materialbag->material_lut+*no_of_materials_Ptr,pfatTexture.lambda[I],Materialbag->material_list,I,*no_of_materials_Ptr);
#endif
	}else if(strcmp(dum,"transparent")==0 ){
	  ok_flag=1;
	  line=strchr(buf,'t')+11;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after transparent flag in material file");
          if(!material_table[*no_of_materials_Ptr].material){
	    material_table[*no_of_materials_Ptr].material=i_allocate(1);
	    material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(1,sizeof(double));
	    material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
	  }
          material_table[*no_of_materials_Ptr].no_of_materials=1;
	  *(material_table[*no_of_materials_Ptr].material)= *no_of_materials_Ptr;
	  *(material_table[*no_of_materials_Ptr].material_proportions) = 1.0;
	  *(material_table[*no_of_materials_Ptr].material_type) = M_TRANSPARENT;
	  material_table[*no_of_materials_Ptr].no_of_materials = 1;
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\ttransparent material\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  
#ifdef PFAT
	  sml=Materialbag->material_lut;	
	  /* pfat requires LUTs to be set up at this point */
	  for(I=0;I<3;I++)
	    calculate_material_lut(Materialbag->material_lut+*no_of_materials_Ptr,pfatTexture.lambda[I],Materialbag->material_list,I,*no_of_materials_Ptr);
#endif
	  
	}else if(strcmp(dum,"image")==0){	/* multiple-frame HIPL image texture map */
	  ok_flag=0;			/* not implemented */
	  line=strchr(buf,'e')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after image flag in material file");
          if(!material_table[*no_of_materials_Ptr].material){
	    material_table[*no_of_materials_Ptr].material=i_allocate(1);
	    material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(1,sizeof(double));
	    material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
	  }
          material_table[*no_of_materials_Ptr].no_of_materials=1;
	  *(material_table[*no_of_materials_Ptr].material)= *no_of_materials_Ptr;
	  *(material_table[*no_of_materials_Ptr].material_proportions) = 1.0;
	  *(material_table[*no_of_materials_Ptr].material_type) = HIPL_TEXTURE_MAP;
	  material_table[*no_of_materials_Ptr].no_of_materials = 1;
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\ttexture-map material\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line=strchr(line,material_name[0])+strlen(material_name);
	  if(sscanf(line,"%s",material_filename)!=1)error1("material_filename (texture map HIPL image) expected after image flag in material file");
	  
	}else if(strcmp(dum,"mix")==0 || strcmp(dum,"rpv")==0){		/* material mixture  or rpv */
	  ok_flag=1;
	  if(strcmp(dum,"mix")==0)line=strchr(buf,'x')+1;else if(strcmp(dum,"rpv")==0)line=strchr(buf,'v')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after mix flag in material file");
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\tmaterial mixture\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line=strchr(line,material_name[0])+strlen(material_name);
          
          if(material_table[*no_of_materials_Ptr].material){
            free(material_table[*no_of_materials_Ptr].material);
            free(material_table[*no_of_materials_Ptr].material_proportions);
            free(material_table[*no_of_materials_Ptr].material_type);
          }
	  material_table[*no_of_materials_Ptr].material=i_allocate(bb->PRAT_MAX_MATERIALS);
	  material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(bb->PRAT_MAX_MATERIALS,sizeof(double));
	  material_table[*no_of_materials_Ptr].material_type=b_allocate(bb->PRAT_MAX_MATERIALS);
          if(strcmp(dum,"rpv")==0)material_table[*no_of_materials_Ptr].material_type[0]=RPV;
	  if(!(material_table[*no_of_materials_Ptr].imagemap=(ImageMap *)calloc(bb->PRAT_MAX_MATERIALS,sizeof(ImageMap))))error1("mtllib:\tcore allocation error");
          material_table[*no_of_materials_Ptr].no_of_materials=0;
	  material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=material_table[*no_of_materials_Ptr].diffuse_reflectance_flag=0;
	  while(sscanf(line,"%s %s",material,proportion_str)==2){
	    proportion=atof(proportion_str);
	    line=strchr(line,material[0])+strlen(material);
	    line=strchr(line,proportion_str[0])+strlen(proportion_str);
	    found_material=0;
	    for(i=0;i< *no_of_materials_Ptr;i++)
	      if(strcmp(material,material_names[i])==0){
		found_material=1;
		if(material_table[i].diffuse_transmittance_flag)material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=1;
		if(material_table[i].diffuse_reflectance_flag)material_table[*no_of_materials_Ptr].diffuse_reflectance_flag=1;
		
		/* deal with mixtures of mixtures */
		for(j=0;j<material_table[i].no_of_materials;j++){
		  material_table[*no_of_materials_Ptr].material_type[material_table[*no_of_materials_Ptr].no_of_materials]=material_table[i].material_type[j];
		  material_table[*no_of_materials_Ptr].material[material_table[*no_of_materials_Ptr].no_of_materials]=material_table[i].material[j];
		  material_table[*no_of_materials_Ptr].material_proportions[material_table[*no_of_materials_Ptr].no_of_materials]=proportion;
		  material_table[*no_of_materials_Ptr].no_of_materials++;
		}
		if(material_table[i].no_of_materials==1)
		  material_table[*no_of_materials_Ptr].material[material_table[*no_of_materials_Ptr].no_of_materials-1]=i;
	      }
	    if(!found_material)error1("mtllib:\tmix material not found");
	  }
	  
	}else if(strcmp(dum,"matmap")==0 || strcmp(dum,"materialmap")==0 || strcmp(dum,"material_map")==0){	/* material map */
	  ok_flag=1;
	  line=strchr(buf,'p')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after material_map flag in material file");
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\tmaterial material_map\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line=strchr(line,material_name[0])+strlen(material_name);
	  if(sscanf(line,"%s",material_name)!=1)error1("material_map filename expected after material_map material_name flag in material file");
	  if(!(material_table[*no_of_materials_Ptr].imagemap=(ImageMap *)calloc(1,sizeof(ImageMap))))error1("mtllib:\tcore allocation error");
	  material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=material_table[*no_of_materials_Ptr].diffuse_reflectance_flag=0;
	  read_material_map_file(material_table,material_name,no_of_materials_Ptr,material_names);
	  
	  if(verbose)fprintf(stderr,"material_map material: %s read\n",material_name);
#ifdef PFAT
	  /* need to convert material map to texture map */
	  
	  /* get image size */
	  row=material_table[*no_of_materials_Ptr].imagemap->hd.rows;
	  col=material_table[*no_of_materials_Ptr].imagemap->hd.cols;
	  n=pfatTexture.nTextures;
	  /* test dimensions valid */
	  checkTextureMapDimensions(row,material_name);
	  checkTextureMapDimensions(col,material_name);
	  /* allocate texture map */
	  allocateTextureMap(n,row,col);
	  /* copy material map to texture map */
	  copyTextureMap(n,row,col,&material_table[*no_of_materials_Ptr]);
	  /* set up material LUT for selected wavelengths */
	  sml=Materialbag->material_lut;	
	  for(I=0;I<3;I++)
	    calculate_material_lut(sml+*no_of_materials_Ptr,pfatTexture.lambda[I],Materialbag->material_list,I,*no_of_materials_Ptr);
	  initTextureMap(n,row,col);
	  pfatTexture.materialPointer[n]=&material_table[*no_of_materials_Ptr];
	  /* increment counter */
	  pfatTexture.nTextures++;
	  
#endif	
	}else if(strcmp(dum,"bump")==0){	/* 1-frame HIPL image bump map */
	  ok_flag=1;
	  line=strchr(buf,'p')+1;
	  if(sscanf(line,"%s",material_name)!=1)error1("material name expected after bump flag in material file");
	  for(i=0;i< *no_of_materials_Ptr;i++)
	    if(strcmp(material_name,material_names[i])==0)error1("multiple defined material name");
	  if(verbose)fprintf(stderr,"\n\tmaterial bumpmap\t%s...\n",material_name);
	  strcpy(material_names[ *no_of_materials_Ptr ],material_name);
	  line=strchr(line,material_name[0])+strlen(material_name);
	  if(sscanf(line,"%s",material_name)!=1)error1("bumpmap filename expected after bump material_name bump flag in material file");
	  if(!(material_table[*no_of_materials_Ptr].imagemap=(ImageMap *)calloc(1,sizeof(ImageMap))))error1("mtllib:\tcore allocation error");
	  if(!material_table[*no_of_materials_Ptr].material){
            material_table[*no_of_materials_Ptr].material=i_allocate(1);
	    material_table[*no_of_materials_Ptr].material_proportions=(double *)v_allocate(1,sizeof(double));
	    material_table[*no_of_materials_Ptr].material_type=b_allocate(1);
	  }
          material_table[*no_of_materials_Ptr].no_of_materials=1;
	  material_table[*no_of_materials_Ptr].diffuse_transmittance_flag=material_table[*no_of_materials_Ptr].diffuse_reflectance_flag==0;
	  *(material_table[*no_of_materials_Ptr].material_type)=BUMPMAP;
	  (material_table[*no_of_materials_Ptr].u)=0;
	  (material_table[*no_of_materials_Ptr].v)=0;
	  *(material_table[*no_of_materials_Ptr].material_proportions)=1.0;
	  *(material_table[*no_of_materials_Ptr].material)= *no_of_materials_Ptr;
	  mmap_read_hips_image(material_name,&(material_table[*no_of_materials_Ptr].imagemap->hd),&(material_table[*no_of_materials_Ptr].imagemap->data),"MATLIB");
	  switch(material_table[*no_of_materials_Ptr].imagemap->hd.num_frame){
	  case 1:
	    break;
	  case 2:
	    if(sscanf(line,"%s",buf)==1 && strcmp(buf,"u"))
	      (material_table[*no_of_materials_Ptr].u)=1;
	    else if(sscanf(line,"%s",buf)==1 && strcmp(buf,"v"))
	      (material_table[*no_of_materials_Ptr].v)=1;
	    else error1("bumpmap:\terror specifying bumpmap material for multiple input frames");
	    break;
	  case 4:
	    if(sscanf(line,"%s",buf)==1 && strcmp(buf,"uv")){
	      (material_table[*no_of_materials_Ptr].u)=1;
	      (material_table[*no_of_materials_Ptr].v)=1;
	    }else if(sscanf(line,"%s",buf)==1 && strcmp(buf,"v")){
	      material_table[*no_of_materials_Ptr].imagemap->hd.num_frame=2;
	      (material_table[*no_of_materials_Ptr].v)=1;
	    }else if(sscanf(line,"%s",buf)==1 && strcmp(buf,"u")){
	      material_table[*no_of_materials_Ptr].imagemap->hd.num_frame=2;
	      (material_table[*no_of_materials_Ptr].u)=1;
	    }else error1("bumpmap:\terror specifying bumpmap material for multiple input frames");
	  default:
	    material_table[*no_of_materials_Ptr].imagemap->hd.num_frame=1;
	  }
	  if(material_table[*no_of_materials_Ptr].imagemap->hd.num_frame==1)preprocess_bumpmap_data(&(material_table[*no_of_materials_Ptr]));
	  if(verbose)fprintf(stderr,"bumpmap material: %s read\n",material_name);
	}
	if(!ok_flag)error1("read_material_definition_file:\tfile format error");
	(material_list_Ptr->no_of_materials)++;
	/*(*no_of_materials_Ptr)++;*/
      }
  fp=openFile(*filename,CLOSE,fp);
  return(1);
}

int	is_double(str,data_Ptr)
     char	*str;
     double	*data_Ptr;
{
  int	i;
  if( (*data_Ptr = atof(str))!=0.0)return(1);
  for(i=0;i<(int)strlen(str);i++)
    if((int)str[i]<(int)46 ||(int)str[i]>(int)57)return(0);
  return(1);
}

double *sortRPVData(char *name,char *data,int *material,int no_of_materials,char **material_names){
  int i,found=0;
  double *out=NULL;
  /* first check to see if there is a material with this name */
  for(i=0;i<no_of_materials;i++){
    if(!strcmp(material_names[i],data)){
      fprintf(stderr,"%s %s\n",name,data);
      found=i+1;break;
    }
  }
  if(!found){
    /* assume it is constant */
    out=(double *)v_allocate(1,sizeof(double));
    sscanf(data,"%lf",out);
    fprintf(stderr,"%s %lf\n",name,*out);
  }else{ 
    found--;
    (*material)=found;
  }
  return(out);
}

int	read_volume_model(verbose,filename,material_Ptr,material_list_Ptr,no_of_materials_Ptr,fp,material_names)
     Material_List	*material_list_Ptr;
     int	*no_of_materials_Ptr;
     FILE	*fp;
     Material_table	*material_Ptr;
     char	*filename,**material_names;
     int	verbose;
{
  char	buf[1024],buffy[1024];
  int	found,i,rows=0,get_no_of_columns_in_file();
  FILE	*fp1;
  double	sum;
  
  LADEccentricity=1.0;
  if(fscanf(fp,"%s",buf)!=1 || strcmp(buf,"{")!=0)error1("read_volume_model:\t volume model format error");
  rows=0;
  material_list_Ptr->material[*no_of_materials_Ptr].volume.lad=NULL;
  material_list_Ptr->material[*no_of_materials_Ptr].volume.uL=0.0;
  material_list_Ptr->material[*no_of_materials_Ptr].volume.material=0;
  material_list_Ptr->material[*no_of_materials_Ptr].volume.l=0.0;
  
  while(fscanf(fp,"%s",buf)==1 && strcmp(buf,"}")!=0){
    /* find reflectance info. */
    if( !strcmp(buf,"reflectance")||!strcmp(buf,"material")){
      if(fscanf(fp,"%s",buf)==1){
	found=0;
	for(i=0;i<material_list_Ptr->no_of_materials;i++){
	  if(!strcmp(material_names[i],buf)){
	    found=i+1;break;
	  }
	}
	if(!found){
	  fprintf(stderr,"read_volume_model: material %s not found in material file (needs to be specified before current volume material in list)\n",buf);
	  exit(1);
	}
	found--;
	material_list_Ptr->material[*no_of_materials_Ptr].volume.material=found;
	material_list_Ptr->material[*no_of_materials_Ptr].wavelength=material_list_Ptr->material[found].wavelength;
        material_list_Ptr->material[*no_of_materials_Ptr].no_of_samples=material_list_Ptr->material[found].no_of_samples;
        material_list_Ptr->material[*no_of_materials_Ptr].srm=material_list_Ptr->material[found].srm;
      }else error1("read_volume_model: parse error after flag reflectance");
    }else if( !strcmp(buf,"angle")){
      if(fscanf(fp,"%s",buf)==1){
        /*if(strcmp(buf,"readFileEqual")){
          fprintf(stderr,"error in volumetric material definition: only type readFileEqual allowed for class angle\n");
          exit(1);
        }
	handle=dlopen(NULL,RTLD_LAZY);
	material_list_Ptr->material[*no_of_materials_Ptr].volume.lad=(triplet (*) ()) dlsym(handle,buf);
	 material_list_Ptr->material[*no_of_materials_Ptr].volume.lad=(triplet (*) ())readFileEqual;
	if(!material_list_Ptr->material[*no_of_materials_Ptr].volume.lad){
	  fprintf(stderr,"%s not found\n",buf);
	  error1("read_volume_model: parse error after flag angle - specified angle distribution function not found");
	}
	if(!strcmp(buf,"elliptical")){	
	  if(fscanf(fp,"%s",buffy)!=1)
	    error1("read_volume_model: parse error after flag angle elliptical - specified eccentricity not found");
	  LADEccentricity=atof(buffy);
	  if(!LADEccentricity)LADEccentricity=1e-20;
	}
	if(!strcmp(buf,"readFile")){	
	  if(fscanf(fp,"%s",buffy)!=1)
	    error1("read_volume_model: parse error after flag angle readFile - specified angle distribution filename not found");
	  fp1=open_file_for_read(buffy); 
	  VOLUME_nZeniths=0;
	  while(fscanf(fp1,"%s",buf)!=EOF){
	    VOLUME_nZeniths++;
	  }
	  if(!material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq)material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq=f_allocate(VOLUME_nZeniths);
	  rewind(fp1);VOLUME_nZeniths=0;sum=0;
	  while(fscanf(fp1,"%s",buf)!=EOF){
	    material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths]=atof(buf);
	    if(material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths]<0){
	      fprintf(stderr,"angle definition file: %s\n",buffy);
	      error1("read_volume_model: negative frequency specified");
	    }
	    VOLUME_nZeniths++;
	  }
	  if(VOLUME_nZeniths<2){
	    fprintf(stderr,"angle definition file: %s\n",buffy);
	    error1("read_volume_model: less than 2 frequencies specified in angle file");
	  }	
	  for(i=1;i<VOLUME_nZeniths;i++){
	    material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[i]+=material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[i-1];
	  }
	  if(material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths-1]==0){
	    fprintf(stderr,"angle definition file: %s\n",buffy);
	    error1("read_volume_model: zero summed frequency");
	  }
	  for(i=0;i<VOLUME_nZeniths;i++){
	    material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[i]/=material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths-1];
	  }
	  
	}
	if(!strcmp(buf,"readFileEqual")){
	  if(fscanf(fp,"%s",buffy)!=1)
	    error1("read_volume_model: parse error after flag angle readFileEqual - specified angle distribution filename not found");
	  */
	  fp1=open_file_for_read(buf); /* read in normal zenith angle distribution */
	  VOLUME_nZeniths=0;
	  while(fscanf(fp1,"%s",buf)!=EOF){
	    VOLUME_nZeniths++;
	  }
	  /*
	  ** required format:
	  **
	  ** equal area bins of the frequency distribution in degrees
	  **
	  ** first entry should be 0 last entry should be 90
	  */

	  if(!material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq)material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq=f_allocate(VOLUME_nZeniths);
	  rewind(fp1);VOLUME_nZeniths=0;sum=0;
	  while(fscanf(fp1,"%s",buf)!=EOF){
	    material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths]=DTOR(atof(buf));
	    VOLUME_nZeniths++;
	  }
          fclose(fp1);
	  if(VOLUME_nZeniths<2){
	    fprintf(stderr,"angle definition file: %s\n",buffy);
	    error1("read_volume_model: less than 2 angles specified in angle file");
	  }
	  if(material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[0]!=0.0||(int)(100*material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZenithsFreq[VOLUME_nZeniths-1])!=(int)(100*M_PI_2)){
	    fprintf(stderr,"angle definition file error: %s\n",buffy);
	    error1("read_volume_model: first angle needs to be 0, last 90 degrees");
	  }
          material_list_Ptr->material[*no_of_materials_Ptr].volume.VOLUME_nZeniths=VOLUME_nZeniths;
	}
      /*}else error1("read_volume_model: parse error after flag angle");
      */
    }else if( !strcmp(buf,"ul")){
      if(fscanf(fp,"%lf",&(material_list_Ptr->material[*no_of_materials_Ptr].volume.uL))==1); else error1("read_volume_model: parse error after flag angle");
    }else if( !strcmp(buf,"size")){
      if(fscanf(fp,"%lf",(&(material_list_Ptr->material[*no_of_materials_Ptr].volume.l)))!=1)
	error1("read_volume_model: parse error after flag angle");
    }else if( !strcmp(buf,"ul")){
      if(fscanf(fp,"%lf",&(material_list_Ptr->material[*no_of_materials_Ptr].volume.uL))==1); else error1("read_volume_model: parse error after flag angle");
    }
  }
  
  if(!material_list_Ptr->material[*no_of_materials_Ptr].volume.uL)
    error1("read_volume_model: required parameter ul not found");
  if(!material_list_Ptr->material[*no_of_materials_Ptr].volume.material)
    error1("read_volume_model: required parameter reflectance not found");
  
  fclose(fp);
  return(1);
}

int	read_volume_material_file(bb,verbose,filename,material_list_Ptr,no_of_materials_Ptr,material_Ptr,material_names)
     BigBag *bb;
     char	*filename;
     int	verbose;
     Material_List	*material_list_Ptr;
     int	*no_of_materials_Ptr;
     Material_table	*material_Ptr;
     char **material_names;
{
  FILE	*fp, *open_file_for_read();
  int	get_no_of_columns_in_file(),type=0;
  char	volume_type[96];
  
  if(*no_of_materials_Ptr>bb->PRAT_MAX_MATERIALS)error1("read_material_file:\tonly %d materials can be defined (set with environment variable PRAT_MAX_MATERIALS)",bb->PRAT_MAX_MATERIALS);
  material_Ptr->diffuse_transmittance_flag=material_Ptr->diffuse_reflectance_flag=0;
  fp=open_file_for_read(filename);
  if(fscanf(fp,"%s",volume_type)!=1)error1("read_volume_material_file:\tbrdf_model_type expected");
  if(verbose)fprintf(stderr,"Volumetric model:\n");
  read_volume_model(verbose,filename,material_Ptr,material_list_Ptr,no_of_materials_Ptr,fp,material_names);
  return(type);
}

int	read_material_file(bb,verbose,filename,material_list_Ptr,no_of_materials_Ptr,material_Ptr,white)
     BigBag *bb;
     char	*filename;
     int	verbose;
     Material_List	*material_list_Ptr;
     int	*no_of_materials_Ptr;
     Material_table	*material_Ptr;
     int white;
{
  FILE	*fp, *open_file_for_read();
  double	wavelength,reflectance,this;
  int	rows=0,no_more=0,no_of_columns,i,timer=0,rc, get_no_of_columns_in_file();
  
  if(*no_of_materials_Ptr>bb->PRAT_MAX_MATERIALS)error1("read_material_file:\tonly %d materials can be defined (set with environment variable PRAT_MAX_MATERIALS)",bb->PRAT_MAX_MATERIALS);
  material_Ptr->diffuse_transmittance_flag=material_Ptr->diffuse_reflectance_flag=0;

  if((fp=openFile(filename,TRUE,"MATLIB"))==NULL){
    /*fprintf(stderr,"\tmaterial value %s:",filename);*/
    sscanf(filename,"%lf",&this);
    /*fprintf(stderr," %lf\n",this);*/
    if(material_list_Ptr->material[*no_of_materials_Ptr].wavelength)
      free(material_list_Ptr->material[*no_of_materials_Ptr].wavelength);  
    if(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance)
      free(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance);
    if(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance)
      free(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance);
    material_list_Ptr->material[*no_of_materials_Ptr].wavelength=(double *)v_allocate(2,sizeof(double));
    material_list_Ptr->material[*no_of_materials_Ptr].wavelength[0]=0;
    material_list_Ptr->material[*no_of_materials_Ptr].wavelength[1]=1e20;
    material_list_Ptr->material[*no_of_materials_Ptr].no_of_samples=2;
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance=(double *)v_allocate(2,sizeof(double));
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance_flag=1;
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance_flag=0;
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance=NULL;
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance[0]=this;
    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance[1]=this;
    return(1);
  }else{
    rc=get_no_of_columns_in_file(verbose,filename,&rows);
    no_of_columns=rc;
    if(no_of_columns>5)error1("read_material_file:\tmaximum of 5 columns of data expected");
    if(material_list_Ptr->material[*no_of_materials_Ptr].wavelength)
      free(material_list_Ptr->material[*no_of_materials_Ptr].wavelength);  
    if(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance)
      free(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance);
    if(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance)
      free(material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance);
 
    material_list_Ptr->material[*no_of_materials_Ptr].wavelength=(double *)v_allocate(rows,sizeof(double));
    while( fscanf(fp,"%lf",&wavelength) == 1 && !no_more){
      if(timer==0){
        material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance=(double *)v_allocate(rows,sizeof(double));
        material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance_flag=1;
        if(no_of_columns>=3){
	  material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance=(double *)v_allocate(rows,sizeof(double));
	  material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance_flag=1;
        }
        timer=1;
      }else timer++;
    
      /* set the wavelength */
      material_list_Ptr->material[ *no_of_materials_Ptr].wavelength[timer-1]=wavelength;

      for(i=0;i<no_of_columns-1;i++)
        if( fscanf(fp,"%lf",&reflectance) == 1){
          if(white==1)reflectance=1.0;
	  switch(i){
	  case 0:
	    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_reflectance[timer-1] = reflectance;material_Ptr->diffuse_reflectance_flag=1;break;
	  case 1:
	    material_list_Ptr->material[ *no_of_materials_Ptr].srm.diffuse_transmittance[timer-1] = reflectance;material_Ptr->diffuse_transmittance_flag=1;break;
	  }
        }
     }
  }
  material_list_Ptr->material[ *no_of_materials_Ptr].no_of_samples=timer;
  fp=openFile(filename,CLOSE,fp);
  return(1);
}

/*************************************/
/*material distribution functions here */
/* spherical distribution */
triplet spherical()
{
  double t,p,s,x;
  triplet out;
  x=Random();
  t=acos(x);
  s=sin(t);
  p=Random()*2.0*M_PI;
  out.x=s*cos(p);
  out.y=s*sin(p);
  out.z=cos(t);
  return(out);
}

/* spherical distribution */
triplet elliptical()
{
  double t,p,s,x;
  triplet out;
  x=Random();
  t=acos(x);
  s=sin(t);
  p=Random()*2.0*M_PI;
  out.x=s*cos(p);
  out.y=s*sin(p);
  out.z=cos(t)*LADEccentricity;
  out=normalise(out);
  return(out);
}


/* this one works on a definition of equal area bins */
triplet readFileEqual(Volume *v)
{
  double dd;
  double t,p,s;
  triplet out;
  int i;
  /* 
  ** VOLUME_nZeniths - no. of samples in zenith
  ** VOLUME_nZenithsFreq - array of size VOLUME_nZeniths (0 ... VOLUME_nZeniths-1)
  **					     containing frequency info. from file
  */
  if(!v->VOLUME_nZeniths||!v->VOLUME_nZenithsFreq)return(vector_copy2(0.,0.,0.));
  
  /*	dd=M_PI_2/v->VOLUME_nZeniths-1;*/
  /* select the bin */
  i=(int)( Random() * (v->VOLUME_nZeniths-1));
  /* get the range for that bin */
  dd=v->VOLUME_nZenithsFreq[i+1]-v->VOLUME_nZenithsFreq[i];
  t=Random()*dd+v->VOLUME_nZenithsFreq[i];
  s=sin(t);
  p=Random()*2.0*M_PI;
  out.x=s*cos(p);
  out.y=s*sin(p);
  out.z=cos(t);
  return(out);
}
