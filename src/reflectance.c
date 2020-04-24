#include <math.h>
#include "prat.h"


triplet rotateBymatrix(triplet v,double *m,int transpose){
/* rotation matrix */
	triplet out;
	if(!transpose){
	  out.x = m[0*3+0]*v.x + m[0*3+1]*v.y + m[0*3+2]*v.z;
	  out.y = m[1*3+0]*v.x + m[1*3+1]*v.y + m[1*3+2]*v.z;
	  out.z = m[2*3+0]*v.x + m[2*3+1]*v.y + m[2*3+2]*v.z;
	}else{
	  out.x = m[0*3+0]*v.x + m[1*3+0]*v.y + m[2*3+0]*v.z;
	  out.y = m[0*3+1]*v.x + m[1*3+1]*v.y + m[2*3+1]*v.z;
	  out.z = m[0*3+2]*v.x + m[1*3+2]*v.y + m[2*3+2]*v.z;
	}
	return(out);
}

triplet	calculate_diffuse_sample_ray_direction(triplet *normal_Ptr)
{
  triplet	out,geocentrics,ngx,nx;
  double	r,z;
  
  /*
  **	azimuth angle -> geocentrics.y
  */
  geocentrics.y = Random()*2.0*M_PI;			
  /*
  **	including cosine term in sampling..assumes zenith = asin(sqrt(Rand())
  */
  r=Random();
  z=sqrt(r);
  /*
  **	zenith angle -> geocentrics.z
  */
  geocentrics.z = asin(z);
  ngx = cartesian_to_spherical(*normal_Ptr);
  ngx.z += M_PI_2;
  nx = spherical_to_cartesian(ngx,1);

  out = rotate_about(*normal_Ptr,nx,RTOD(geocentrics.z));
  out = rotate_about(out,*normal_Ptr,RTOD(geocentrics.y));
  return(normalise(out));
  
  /*
  **	normal zenith angle -> theta_n
  */
  /*theta_n=acos(MIN(1.0,MAX(-1.0,(double)normal_Ptr->z)));
    
    if(fabs(normal_Ptr->z)<1.0){
    *
    **	normal azimuth angle -> phi_n
    *
    phi_n=atan2((double)(normal_Ptr->y),(double)(normal_Ptr->x));
    geocentrics.y += phi_n;
    }
    *
    **	zenith angle relative to normal vector
    *
    geocentrics.z += theta_n;
    *
    **	horizontal projection
    *
    d1=sin(geocentrics.z);
    *
    **	op relative to normal
    *
    out=vector_copy2(d1*cos(geocentrics.y),d1*sin(geocentrics.y),cos(geocentrics.z));
    
    return(normalise(out));
    */
}

void	setReflectanceValue(int *timer,int mat_count,double *depth_value,double *out,double *presult,double f,int C)
{
/* EDIT LEWIS Jan 2010 ... not sure what 'result' does, so remove its influence
** This was stopping the mxture model from working
*/
  double result=1.0;
  if(*timer==0 || mat_count<=0){
    if(C)*timer=1;
    
    if(depth_value){
      *depth_value=f;
      *out=result*f;
    }else{
      *out= f;
    }
  }else{ 
    if(depth_value){
      *depth_value+=f;
      *out+= result * f;
    }else{
      *out+= result * f;
    }
  }
  return;
}
void	*calculate_current_reflectance(int isDiffuse,int m,double material_proportion,int mode,CurrentReflectanceBag *current_reflectance_bag,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,double *out,double *depth_value,int *tree_depth_ptr,Material_table *material_Ptr,int *mat_count)
{
  int	transparent=0,k,j,no_of_samples=0;
  double	*depthValuePtr,lambda,radiometric=0.0;
  int	texture_done=0,index;
  Material_table	*material,*texture_material;
  Rpv *rpv=NULL;

  material=material_Ptr;
 
  if(material->no_of_materials==4){/* && material->material_type[0] ==RPV){*/
    /* for RPV, calculate the rho0 term */
    calculate_current_reflectance(isDiffuse,m,material->material_proportions[0],mode,current_reflectance_bag,materialbag,wavebandbag,flagbag,out,depth_value,tree_depth_ptr,&(materialbag->materials[material->material[0]]),mat_count); 
    /* get the other parameters */
    rpv = (Rpv *)v_allocate(wavebandbag->sensor_wavebands->no_of_wavebands,sizeof(Rpv));
    for(j=0;j<wavebandbag->sensor_wavebands->no_of_wavebands;j++){
      rpv[j].rho0 = materialbag->material_lut[material->material[0]].srm.diffuse_reflectance[j];
      rpv[j].k = materialbag->material_lut[material->material[1]].srm.diffuse_reflectance[j];
      rpv[j].bigtheta = materialbag->material_lut[material->material[2]].srm.diffuse_reflectance[j];
      rpv[j].rhoc = materialbag->material_lut[material->material[3]].srm.diffuse_reflectance[j];
    }
    return((void *)rpv);
  }else if(material->no_of_materials>1){
    for(k=0;k<material->no_of_materials;k++){
      calculate_current_reflectance(isDiffuse,m,material->material_proportions[k],mode,current_reflectance_bag,materialbag,wavebandbag,flagbag,out,depth_value,tree_depth_ptr,&(materialbag->materials[material->material[k]]),mat_count);
    }
    return(NULL);
  }
  
  for(j=0;j<wavebandbag->sensor_wavebands->no_of_wavebands;j++){
    lambda=wavebandbag->lambda[j];
    switch(material->material_type[0]){
    case HIPL_TEXTURE_MAP:
      if(!texture_done){
	texture_material=material;
	index=apply_material_map((texture_material),1);
	texture_material=(Material_table *)material->material_table[index];
	calculate_current_reflectance(isDiffuse,m,material->material_proportions[0],mode,current_reflectance_bag,materialbag,wavebandbag,flagbag,out,depth_value,tree_depth_ptr,texture_material,mat_count);
	(*mat_count)++;
	texture_done=1;
      }
      break;
    case SRM:
      switch(current_reflectance_bag->reflectance_type){
      case REFLECTANCE:
	switch(mode){
	case DIFFUSE:
	  radiometric=materialbag->material_lut[material->material[0]].srm.diffuse_reflectance[j];
	  if(isDiffuse && materialbag->material_lut[material->material[0]].srm.diffuse_transmittance)radiometric *= material->reflWeighting; /* to account for single diffuse sampling */
	  break;
	}
	no_of_samples=current_reflectance_bag->no_of_diffuse_reflectance_samples;
	break;
      case TRANSMITTANCE:
	switch(mode){
	case DIFFUSE:
	  if(materialbag->material_lut[material->material[0]].srm.diffuse_transmittance)radiometric=materialbag->material_lut[material->material[0]].srm.diffuse_transmittance[j];else radiometric=0;
	  if(isDiffuse)radiometric *= material->transWeighting;
	  break;
	}
	no_of_samples=current_reflectance_bag->no_of_diffuse_transmittance_samples;
	break;
      default:
	error1("calculate_current_reflectance:\tattempt to use undefined material type");
      }
      if(depth_value)depthValuePtr=&depth_value[j];else depthValuePtr=NULL;
      setReflectanceValue(&current_reflectance_bag->timer,*mat_count,depthValuePtr,&out[j],&materialbag->samples->result[j],material_proportion*current_reflectance_bag->lambertian*radiometric/(no_of_samples),j==wavebandbag->sensor_wavebands->no_of_wavebands -1);
      if(j== wavebandbag->sensor_wavebands->no_of_wavebands -1)(*mat_count)++;
      break;
    case M_TRANSPARENT:
     transparent=1;
     if(j== wavebandbag->sensor_wavebands->no_of_wavebands -1)(*mat_count)++;
     break;
    default:
      break;
    }
  }
  return(NULL);
}



int test_calculate_current_reflectance(int m,double material_proportion,int mode,CurrentReflectanceBag *current_reflectance_bag,MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,double *out,double *depth_value,int *tree_depth_ptr,Material_table *material_Ptr,int *mat_count)
{
  int	transparent=0,k,j;
  double	lambda;
  int	texture_done=0,index;
  Material_table	*material,*texture_material;
  
  material=material_Ptr;
  
  if(material->no_of_materials>1){
    for(k=0;k<material->no_of_materials;k++){
      if((transparent=test_calculate_current_reflectance(m,material->material_proportions[k],mode,current_reflectance_bag,materialbag,wavebandbag,flagbag,out,depth_value,tree_depth_ptr,&(materialbag->materials[material->material[k]]),mat_count)))return(transparent);
    }
    return(transparent);
  }
  
  for(j=0;j<wavebandbag->sensor_wavebands->no_of_wavebands;j++){
    lambda=wavebandbag->lambda[j];
    switch(material->material_type[0]){
    case HIPL_TEXTURE_MAP:
      if(!texture_done){
	texture_material=material;
	index=apply_material_map((texture_material),1);
	texture_material=(Material_table *)material->material_table[index];
	if((transparent=test_calculate_current_reflectance(m,material->material_proportions[0],mode,current_reflectance_bag,materialbag,wavebandbag,flagbag,out,depth_value,tree_depth_ptr,texture_material,mat_count)))return(transparent);				
	texture_done=1;
      }
      break;		
    case M_TRANSPARENT:
      transparent=1;return(transparent);			
      break;
    default:
      break;
    }
  }
	return(transparent);
}

/* simple interface to reflectance data */

void	calculate_specific_reflectance(int w,double lambda,double material_proportion,MaterialBag *materialbag,double *out,Material_table *material_Ptr,int *mat_count)
{
  int	k;
  double	radiometric=0.0;
  int	texture_done=0,index;
  Material_table	*material,*texture_material;
  
  material=material_Ptr;
  
  if(material->no_of_materials>1){
    for(k=0;k<material->no_of_materials;k++){
      calculate_specific_reflectance(w,lambda,material->material_proportions[k],materialbag,out,&(materialbag->materials[material->material[k]]),mat_count);
    }
    return;
  }
  switch(material->material_type[0]){
  case HIPL_TEXTURE_MAP:
    if(!texture_done){
      texture_material=material;
      index=apply_material_map((texture_material),1);
      
      texture_material=(Material_table *)material->material_table[index];
      calculate_specific_reflectance(w,lambda,texture_material->material_proportions[0],materialbag,out,texture_material,mat_count);
      (*mat_count)++;
      texture_done=1;
    }
    break;
  case SRM:
    radiometric=materialbag->material_list->material[material->material[0]].srm.diffuse_reflectance[w];
    break;
  default:
    break;
  }
  *out+=radiometric;
  return;
}



Ray	apply_diffuse_model(HitPoint *hitPoint,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,FlagBag *flagbag,int reflectance_type,Material_table *material_Ptr,triplet *normal_Ptr,triplet *plane_normal_Ptr,int *tree_depth_Ptr,ObjectList *objectlist_Ptr,BBox *bbox_Ptr)
{
  int signbit=0,invert_ray=0;
  triplet	*tmp_normal_Ptr,S1,ray;
  Ray	opray;
  double	s2;			
  triplet normal;

  switch(reflectance_type){
  case REFLECTANCE:
    signbit= -1;
    if(V_dot(*normal_Ptr,ipray->direction)>0.0)invert_ray=1;
    break;
  case TRANSMITTANCE:
    signbit= 1;
    if(V_dot(*normal_Ptr,ipray->direction)<0.0)invert_ray=1;
    break;
  case -1:
    signbit=0;
    break;
  }

  if(flagbag->normal && objectlist_Ptr->ObjectType== FO){
     tmp_normal_Ptr=plane_normal_Ptr;
  }else{
     tmp_normal_Ptr=normal_Ptr;
  }
  
  if(invert_ray){	/* invert ray */
    S1=V_factor(*tmp_normal_Ptr,-1.0);
    ray=calculate_diffuse_sample_ray_direction(&S1);
  }else
    ray=calculate_diffuse_sample_ray_direction(tmp_normal_Ptr);
    
  s2=V_dot(*normal_Ptr,ray );
  /* force the normal to be the same side as the output ray */
  if(FSIGN(s2)==-1){normal.x=-normal_Ptr->x;normal.y=-normal_Ptr->y;normal.z=-normal_Ptr->z;}else{normal.x=normal_Ptr->x;normal.y=normal_Ptr->y;normal.z=normal_Ptr->z;}
  opray.origin= vector_plus(objectlist_Ptr->intersection,V_factor(normal,flagbag->ray_length_tol));

  if((objectlist_Ptr->ObjectType == ELLIPSE || objectlist_Ptr->ObjectType == SPHERE || objectlist_Ptr->ObjectType == SPHEROID) && reflectance_type == TRANSMITTANCE){
    /* jump the ray by objectlist_Ptr->rayLengthThroughObject through the object */
    opray.origin= vector_plus(opray.origin,V_factor(normal,objectlist_Ptr->rayLengthThroughObject));
  }

   opray.direction=ray;
   opray.sourceOfRay=!ipray->sourceOfRay;
   opray.lengthOfRaySoFar=ipray->lengthOfRaySoFar;
  
   /* store info in hitPoint */
   hitPoint->interactionType=reflectance_type;
   hitPoint->location = opray.origin;
   hitPoint->fromvector  = ipray->direction;
   hitPoint->tovector = opray.direction;
   return(opray);
}

Ray	apply_shadow_model(int l,HitPoint *hitPoint,Ray *ipray,MaterialBag *materialbag,IlluminationBag *illumination,WavebandBag *wavebandbag,FlagBag *flagbag,Material_table *material_Ptr,triplet *normal_Ptr,triplet *plane_normal_Ptr,int *tree_depth_Ptr,ObjectList *objectlist_Ptr,BBox *bbox_Ptr)
{
  double	s1,s2;
  Ray	opray;
  triplet normal,ray;
  if(!flagbag->sunlocation)
    ray= *illumination->sun_Ptr;
  else
    ray=flagbag->illuminationvector[l];

  s1= -V_dot(*normal_Ptr,ipray->direction);
  s2=V_dot( *normal_Ptr ,ray);
  if(flagbag->normal && objectlist_Ptr->ObjectType== FO){
 	hitPoint->lambertian[l]=fabs(V_dot(*plane_normal_Ptr,ray));
  }else{
	hitPoint->lambertian[l]=fabs(V_dot(*normal_Ptr,ray));
  }
  opray.sourceOfRay=1;
  hitPoint->sunInteractionType[l]=-1;
  opray.direction=ray;
  /* force the normal to be the same side as the output ray */
  if(FSIGN(s2)==-1){normal.x=-normal_Ptr->x;normal.y=-normal_Ptr->y;normal.z=-normal_Ptr->z;}else{normal.x=normal_Ptr->x;normal.y=normal_Ptr->y;normal.z=normal_Ptr->z;}
  if((FSIGN(s1)!=FSIGN(s2))){ 
    if(material_Ptr->diffuse_transmittance_flag){	
      hitPoint->sunInteractionType[l]=TRANSMITTANCE;
    }else{
     opray.sourceOfRay = 0; /* flag for dont bother */
    }
  }else{
    hitPoint->sunInteractionType[l]=REFLECTANCE; 
  }
  opray.origin= vector_plus(objectlist_Ptr->intersection,V_factor(normal,flagbag->ray_length_tol));
  /* horrible hack for transmitting ellipsoids & spheres ONLY */
  if((objectlist_Ptr->ObjectType == ELLIPSE || objectlist_Ptr->ObjectType == SPHERE || objectlist_Ptr->ObjectType == SPHEROID) && hitPoint->sunInteractionType[l] == TRANSMITTANCE){
    /* jump the ray by objectlist_Ptr->rayLengthThroughObject through the object */
    opray.origin= vector_plus(opray.origin,V_factor(normal,objectlist_Ptr->rayLengthThroughObject));
  }
  hitPoint->sunvector[l]=opray.direction;  
  return(opray);
}

int	apply_bump_map(Material_table *material,triplet *normal,double *correction_ratio)
{
  double	sin_dz_du,sin_dz_dv,cos_dz_du,cos_dz_dv,u,v;
  triplet	perturbed_normal;
  
  sin_dz_du = *(material->imagemap->data.fdata + (material->local_coords.y * material->imagemap->hd.cols) + material->local_coords.x);
  cos_dz_du = *(material->imagemap->data.fdata + (material->imagemap->hd.cols*material->imagemap->hd.rows) + (material->local_coords.y * material->imagemap->hd.cols) + material->local_coords.x);
  sin_dz_dv = *(material->imagemap->data.fdata + 2*(material->imagemap->hd.cols*material->imagemap->hd.rows)+ (material->local_coords.y * material->imagemap->hd.cols) + material->local_coords.x);
  cos_dz_dv = *(material->imagemap->data.fdata + 3*(material->imagemap->hd.cols*material->imagemap->hd.rows)+ (material->local_coords.y * material->imagemap->hd.cols) + material->local_coords.x);
  /*
  **	use library stuff to see if it works
  */
  u = RTOD(asin(sin_dz_du)); /**correction_ratio[0];*/
  v = RTOD(asin(sin_dz_dv)); /**correction_ratio[1];*/
  /*
  **	rotate about u,v bases
  */
  
  perturbed_normal = rotate_about( *normal,material->basis[0],u);
  *normal = rotate_about(perturbed_normal,material->basis[1],v);
  
  return(1);
}

int	apply_material_map(Material_table *material,int flag)
{
  unsigned char	map_value;
  int	retval;
  
  
  map_value = (unsigned char)(*(material->imagemap->data.bdata + (material->local_coords.y * material->imagemap->hd.cols) + material->local_coords.x));
  
  if(material->material_proportions[map_value] < 0.9){
    fprintf(stderr,"mtllib:\terror in useage of material index %d in material map at coordinate %d %d",map_value,material->local_coords.x,material->local_coords.y);
    error1("");
  }
  retval=(int)map_value;
  
  return(retval);
}

void calculate_local_coords(Material_table *material,ObjectList *objectlist_Ptr,int flag,double *correction_ratio,int type)
{
  double	x,y;
  
  if(flag && type==DEM && objectlist_Ptr && objectlist_Ptr->DemObj && objectlist_Ptr->DemObj->local){
    /* its a DEM */
    x = objectlist_Ptr->ancilliary[0];
    y = objectlist_Ptr->ancilliary[1];
    switch(objectlist_Ptr->DemObj->repeatMode){
    case TRIANGULAR_REPEAT:
      x+=0.5*(((int)y)%2);
      break;
    }
    x-=(int)x;
    y-=(int)y;
    if(material->imagemap){
      material->local_coords.x = MAX(MIN((int)(((double)(material->imagemap->hd.cols) * x)),material->imagemap->hd.cols - 1),0);
      material->local_coords.y = MAX(MIN((int)(((double)(material->imagemap->hd.rows) * y)),material->imagemap->hd.rows),0);	    
    }
  }else if(flag && type==FO && objectlist_Ptr->triangleObj && objectlist_Ptr->triangleObj->local_coords){
    x = objectlist_Ptr->triangleObj->local_coords->vertex_local.x + objectlist_Ptr->ancilliary[1] * objectlist_Ptr->triangleObj->local_coords->local_du.x + objectlist_Ptr->ancilliary[0] * objectlist_Ptr->triangleObj->local_coords->local_dv.x;
    y = objectlist_Ptr->triangleObj->local_coords->vertex_local.y + objectlist_Ptr->ancilliary[1] * objectlist_Ptr->triangleObj->local_coords->local_du.y + objectlist_Ptr->ancilliary[0] * objectlist_Ptr->triangleObj->local_coords->local_dv.y;
    
    if(correction_ratio&&material->material_type[0]==BUMPMAP){
      
      correction_ratio[0] = fabs(((objectlist_Ptr->triangleObj->local_coords->du_length)*x*(objectlist_Ptr->triangleObj->local_coords->local_du.x) + (objectlist_Ptr->triangleObj->local_coords->du_length)*x*(objectlist_Ptr->triangleObj->local_coords->local_du.x) + (objectlist_Ptr->triangleObj->local_coords->vertex_local.x))/((objectlist_Ptr->triangleObj->local_coords->du_length)*(objectlist_Ptr->triangleObj->local_coords->local_du.x) + (objectlist_Ptr->triangleObj->local_coords->du_length)*(objectlist_Ptr->triangleObj->local_coords->local_du.x) + (objectlist_Ptr->triangleObj->local_coords->vertex_local.x)));
      
      correction_ratio[1] = fabs(((objectlist_Ptr->triangleObj->local_coords->dv_length)*y*(objectlist_Ptr->triangleObj->local_coords->local_dv.y) + (objectlist_Ptr->triangleObj->local_coords->dv_length)*y*(objectlist_Ptr->triangleObj->local_coords->local_dv.y) + (objectlist_Ptr->triangleObj->local_coords->vertex_local.y))/((objectlist_Ptr->triangleObj->local_coords->dv_length)*(objectlist_Ptr->triangleObj->local_coords->local_dv.y) + (objectlist_Ptr->triangleObj->local_coords->dv_length)*(objectlist_Ptr->triangleObj->local_coords->local_dv.y) + (objectlist_Ptr->triangleObj->local_coords->vertex_local.y)));
      
    }
    x-=(int)x;
    y-=(int)y;
    if(material->imagemap){
      material->local_coords.x = MAX(MIN((int)(((double)(material->imagemap->hd.cols) * x)),material->imagemap->hd.cols - 1),0);
      material->local_coords.y = MAX(MIN((int)(((double)(material->imagemap->hd.rows) * y)),material->imagemap->hd.rows),0);
    }
    
  }else{
    if(material->imagemap){
      /* its something else, sored in the anclilliaries */
      x = objectlist_Ptr->ancilliary[0];
      y = objectlist_Ptr->ancilliary[1];
      x-=(int)x;
	    y-=(int)y;
	    material->local_coords.x = MAX(MIN((int)(((double)(material->imagemap->hd.cols) * x)),material->imagemap->hd.cols - 1),0);
	    material->local_coords.y = MAX(MIN((int)(((double)(material->imagemap->hd.rows) * y)),material->imagemap->hd.rows),0);	
    }
  }
  return;
}

Material_table *apply_local_coordinates(Material_table *material,ObjectList *objectlist_Ptr,triplet *plane_normal,MaterialBag *materialbag,int type)
{
  int	i,index;
  double	correction_ratio[2];
  Material_table	*new_material;
  if(type==PLANE)return(material); 
  new_material = material; 
  for(i=0;i<material->no_of_materials;i++)
    if(material->material_type[i]==HIPL_TEXTURE_MAP){
      /* texture map / material map */
      /*
      **	calculate local coordinates of intersection point
      */
      calculate_local_coords(material,objectlist_Ptr,1,(double *)NULL,type);
      /*
      **	find which material to apply 
      */
      index=apply_material_map(material,1);
      new_material=material->material_table[index];
      new_material->local_coords=material->local_coords;
      material=apply_local_coordinates(new_material,objectlist_Ptr,plane_normal,materialbag,type);
      new_material = material;
    }else if(material->material_type[i]==BUMPMAP){
      /*
      **	bumpmap material
      */
      calculate_local_coords(&(materialbag->materials[material->material[i]]),objectlist_Ptr,1,&(correction_ratio[0]),type);
      materialbag->materials[material->material[i]].basis=&(objectlist_Ptr->triangleObj->local_coords->basis[0]);
      apply_bump_map(&(materialbag->materials[material->material[i]]),plane_normal,&(correction_ratio[0]));
    }else if(materialbag->materials[material->material[i]].no_of_materials>1){
      /*
      **	mixture of a mixture material
      */
      new_material=&(materialbag->materials[material->material[i]]);
      material=apply_local_coordinates(new_material,objectlist_Ptr,plane_normal,materialbag,type);
      new_material=material;
    }else
      calculate_local_coords(&(materialbag->materials[material->material[i]]),objectlist_Ptr,0,NULL,type);
  return(new_material);
}

Material_table	*get_reflection_ray_value(MaterialBag *materialbag,WavebandBag *wavebandbag,FlagBag *flagbag,int *wavelength,triplet *normal,triplet *plane_normal,ObjectList *objectlist_Ptr)
{
  Material_table	*material=0;
  
  switch(objectlist_Ptr->ObjectType){
  case FO:	/* triangle */
    material=(objectlist_Ptr->triangleObj->material);
    /*
    **	facet -> interpolated normal 
    */
    if(flagbag->normal&&objectlist_Ptr->triangleObj->vertex_normals){
      *plane_normal=normalise(vector_plus(vector_plus(V_factor(objectlist_Ptr->triangleObj->vertex_normals->normal_du,objectlist_Ptr->ancilliary[1]),V_factor(objectlist_Ptr->triangleObj->vertex_normals->normal_dv,objectlist_Ptr->ancilliary[0])),objectlist_Ptr->triangleObj->vertex_normals->vertex_normal));
    }else{
      *plane_normal = objectlist_Ptr->triangleObj->normal;
    }
    *normal=objectlist_Ptr->triangleObj->normal;		
    break;
  case SPHERE:	/* sphere */
    material=(objectlist_Ptr->SphereObj->material);
    break;
  case DEM:	/* dem */
    material=(objectlist_Ptr->DemObj->material);
    break;
  case SPHERICAL_DEM:	/* spherical dem */
    material=(((Spherical_Dem *)objectlist_Ptr->DemObj)->material);
    break;
  case CYLINDER:	/* cylinder */
    material=(objectlist_Ptr->CylinderObj->material);
    break;
  case DISK:	/* disk */
    material=(objectlist_Ptr->DiskObj->material);
    break;
  case BILINEARPATCH:	/* bilinear patch */
    material=(objectlist_Ptr->BiLinearPatchObj->material);
    break;
  case ELLIPSE:	/* cylinder */
    material=(objectlist_Ptr->EllipseObj->material);
    break;
  case SPHEROID: /* spheroid */
    material=(objectlist_Ptr->SpheroidObj->material);
    break;
  case PLANE:	/* plane */
    material=(objectlist_Ptr->PlaneObj->material);
    *normal=objectlist_Ptr->PlaneObj->normal;
    *plane_normal=*normal;
		break;
  }

  if(flagbag->local || material->imagemap){	/* do local maps (if any) */
    material=apply_local_coordinates(material,objectlist_Ptr,plane_normal,materialbag,objectlist_Ptr->ObjectType);
  }
  *normal = objectlist_Ptr->normal;
  *plane_normal = *normal;
  return(material);	
}


