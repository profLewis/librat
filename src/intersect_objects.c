#include "frat.h"

triplet rotateByMatrix(triplet in,double *m,int transpose);

/*
**	in special cases, reset the local z-coordinate of the 
**	camera to the maximum local z-height at the x,y location
**	of the watch point
*/

void	reset_local_z_to_local_max_height(bb,materialbag,look_at,camera_position,flagbag,bbox_Ptr)
     BigBag *bb;
     triplet			*look_at, *camera_position;
     FlagBag			*flagbag;
     BBox			*bbox_Ptr;
     void			*materialbag;
{
  int		plane_hit_count=0,intersect_plane(),intersect_objects();
  Plane		*current_plane;
  Ray		ipray;
  ObjectList	objectlist;
  double		difference;
  
  ipray.direction=vector_copy2((double)0.0,(double)0.0,(double)(-1.0));
  ipray.origin=vector_copy2(look_at->x,look_at->y,(double)BIG/10000.0);
  ipray.sourceOfRay=VIEWER;
  objectlist.RayLength=BIG;
  objectlist.clone_rotation=0;
  objectlist.cloneTranslationFlag=0;
  current_plane=bbox_Ptr->contents.plane;
  
  /*
  **	intersect infinite bounded objects
  */
  while(current_plane!=NULL){
    if(intersect_plane(flagbag,current_plane,&ipray.origin,&ipray.direction,&objectlist))plane_hit_count++;
    current_plane=current_plane->next_plane;
  }
  
  /*
  **	intersect bounded objects -> return closest hit
  */
  if(intersect_objects(bb,materialbag,flagbag,bbox_Ptr,&ipray,&objectlist,1)||plane_hit_count){
    /*
    **	set difference to z-value of intersection point
    **	minus camera z-value
    */
    difference = objectlist.intersection.z - camera_position->z + flagbag->ray_length_tol;
  }else{
    /*
    **	set difference to min value of bbox
    **	minus camera z-value
    */
    difference =  bbox_Ptr->min.z - camera_position->z;		
  } 
  camera_position->z += difference;
  return;
}

Material_table *doIHitAnything(bb,normal,plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr,illumination,flagbag,wavebandbag,materialbag,ipray)
     BigBag *bb;
     ObjectList	*objectlist_Ptr;
     triplet *normal,*plane_normal;
     BBox		*bbox_Ptr;
     IlluminationBag		*illumination;
     FlagBag			*flagbag;
     WavebandBag		*wavebandbag;
     MaterialBag		*materialbag;
     Ray			*ipray;
     int 			*tree_depth_Ptr;
{
  int hitMe=0,continueOn=0,transparent=0,plane_hit_count=0,ray_clip=0;
  Plane *current_plane=NULL;
  Material_table	*get_reflection_ray_value(),*material_Ptr;
  Ray		initial_ray;
  int intersect_plane(),intersect_objects(),test_calculate_current_reflectance(); 
  material_Ptr=NULL;
  initial_ray= *ipray;
  objectlist_Ptr->RayLength=BIG;
  objectlist_Ptr->clone_rotation=0;
  if(*tree_depth_Ptr==0)ray_clip=1; 
  
  /*
  **	intersect infinite bounded objects
  */
  ipray->rayLengthThroughObject=0.;
  current_plane=bbox_Ptr->contents.plane;
  
  while(current_plane!=NULL){
    if(intersect_plane(flagbag,current_plane,&ipray->origin,&ipray->direction,objectlist_Ptr))plane_hit_count++;
    current_plane=current_plane->next_plane;    
  }
  
  /*
  **	intersect bounded objects -> return closest hit
  */
  if((hitMe=(intersect_objects(bb,materialbag,flagbag,bbox_Ptr,ipray,objectlist_Ptr,ray_clip)||plane_hit_count))!=0){
    plane_hit_count=0;
    /*
    **	hit some object
    */
    
    /* get the material that we hit */
    /* 
    ** this is returned as an index in 
    ** material_Ptr->material[0,..material_Ptr->no_of_materials]
    */
    material_Ptr=get_reflection_ray_value(materialbag,wavebandbag,flagbag,&(materialbag->samples->wavelength[0]),normal,plane_normal,objectlist_Ptr);
    
    /* special case of transparent materials - continue on */ 
    if((transparent=test_calculate_current_reflectance(0,material_Ptr->material_proportions[0],NULL,NULL,materialbag,wavebandbag,flagbag,NULL,NULL,NULL,material_Ptr,NULL))){
      /* continue on */
      continueOn=1;
    }
    if(transparent);
    else if( *tree_depth_Ptr + 1  > flagbag->max_ray_depth){
      return((Material_table *)-1);
    }else continueOn=0;
    
    if(!continueOn){
      /* bit of a hack to fix lidar for oil palm ... could be wrong Mar 24 2000 */
      ipray->lengthOfRaySoFar+=objectlist_Ptr->RayLength;
    }
    while(continueOn){
      /* add on a bit to the ray trajectory */
      material_Ptr=NULL;
      ipray->lengthOfRaySoFar+=objectlist_Ptr->RayLength+objectlist_Ptr->rayLengthThroughObject;
      ipray->origin=vector_plus(V_factor(ipray->direction,(double)flagbag->ray_length_tol+objectlist_Ptr->rayLengthThroughObject+objectlist_Ptr->RayLength),ipray->origin);
      objectlist_Ptr->RayLength=BIG;
      objectlist_Ptr->clone_rotation=0;
      current_plane=bbox_Ptr->contents.plane;
      objectlist_Ptr->rayLengthThroughObject=0.;
      while(current_plane!=NULL){
	if(intersect_plane(flagbag,current_plane,&ipray->origin,&ipray->direction,objectlist_Ptr))plane_hit_count++;
	current_plane=current_plane->next_plane;	
      }
      if((hitMe=(intersect_objects(bb,materialbag,flagbag,bbox_Ptr,ipray,objectlist_Ptr,ray_clip)||plane_hit_count))!=0){
	plane_hit_count=0;
	material_Ptr=get_reflection_ray_value(materialbag,wavebandbag,flagbag,&(materialbag->samples->wavelength[0]),normal,plane_normal,objectlist_Ptr);
	if(!(transparent=test_calculate_current_reflectance(0,material_Ptr->material_proportions[0],NULL,NULL,materialbag,wavebandbag,flagbag,NULL,NULL,NULL,material_Ptr,NULL)))
	  continueOn=0;	
      }else{
	continueOn=0;
      }
    }
  }
  if(hitMe && flagbag->sunlocation && flagbag->isSunTest){
    /* the 'sun' object is at flagbag->sunlocation
    ** the distance from the ray origin to the sun is flagbag->lengthToSun
    ** the minimum distance to an object is objectlist_Ptr->RayLength
    **
    ** so ... if flagbag->lengthToSun <=  objectlist_Ptr->RayLength
    ** the ray hits the illumination before it hits the closest object
    ** in which case we return NULL to indicate that it hasnt hit the object
    */
    if(flagbag->lengthToSun[0] <= objectlist_Ptr->RayLength)
      hitMe=0;
  }
  ipray->origin = initial_ray.origin;
  if(hitMe)
    return(material_Ptr);
  else
    return(NULL);
}

int getMaterialIndex(material_Ptr,materialbag)
     MaterialBag		*materialbag;
     Material_table	*material_Ptr;
{
  int currentMat,i,j;
  
  /* get index number for material */
  currentMat= -1;
  for(i=0;i<materialbag->material_list->useage->no_of_materials;i++){
    if(materialbag->material_list->useage->materials[i]==(char *)material_Ptr){	
      currentMat=i;
      break;
    }
  }
  if(currentMat==-1){ /* not yet used */
    materialbag->material_list->useage->materials[i]=(char *)material_Ptr;
    /* j - material number */
    for(j=0;j<materialbag->material_list->no_of_materials;j++){
      if((struct Material_table_struct *)material_Ptr==&(materialbag->materials[j])){
	materialbag->material_list->useage->names[i]=materialbag->material_names[j];
	currentMat=i;
	break;
      }
    }
    if(currentMat==-1){
      fprintf(stderr,"unknown material encountered - internal corruption\n");
      exit(1); 
    }
    (materialbag->material_list->useage->no_of_materials)++;
  }
  return(currentMat);
}

double lidarDistance(materialbag,ipray,illumination,flagbag,sunNo)
     MaterialBag		*materialbag;
     Ray			*ipray;
     IlluminationBag		*illumination;
     FlagBag			*flagbag;
     int sunNo;
{
  triplet hitpt,normalise(),vector_minus(),V2,vector_copy(),vector_copy2();
  double V_mod(),distanceBackToViewer=0., d,screen,V_dot();
  int nSuns;
  screen=1.0;
  /* LIDAR */
  if(flagbag->sunlocation){
    nSuns = flagbag->nSuns;
    if(!flagbag->illuminationvector){
      flagbag->illuminationvector = (triplet *)v_allocate(nSuns,sizeof(triplet));
      flagbag->lengthToSun = (double *)v_allocate(nSuns,sizeof(double));
      flagbag->sunrayangle = (double *)v_allocate(nSuns,sizeof(double));
    }
    hitpt=vector_copy2(ipray->origin.x + ipray->lengthOfRaySoFar * ipray->direction.x, ipray->origin.y + ipray->lengthOfRaySoFar * ipray->direction.y,ipray->origin.z + ipray->lengthOfRaySoFar * ipray->direction.z);
    /* multiple suns */
    
    V2=vector_minus(illumination->position->ppoint,hitpt); 
    /* calculate distance illumination from current point to add to travel distance */
    flagbag->illuminationvector[sunNo]=normalise(V2);
    d=V_dot(flagbag->illuminationvector[sunNo],*illumination->sun_Ptr);
    flagbag->lengthToSun[sunNo]=distanceBackToViewer=V_mod(V2);
    flagbag->sunrayangle[sunNo]=acos(d)*180./M_PI;
  }else return(0.);
  return(distanceBackToViewer);
}

/*
**	render pixel with multiple illumination vectors
*/

Ray	renders(hitCamera,bb,hitPoint,tree_depth_Ptr,ipray,materialbag,illumination,wavebandbag,objectlist_Ptr,bbox_Ptr,flagbag,material_table_Ptr)
     BigBag *bb;
     Material_table	*material_table_Ptr;
     HitPoint *hitPoint;
     ObjectList	*objectlist_Ptr;
     BBox		*bbox_Ptr;
     IlluminationBag		*illumination;
     FlagBag			*flagbag;
     WavebandBag		*wavebandbag;
     MaterialBag		*materialbag; 
     Ray			*ipray;  
     int                        *tree_depth_Ptr,hitCamera;
{
  ObjectList	shadowRayList;
  int			currentMat,intersect_plane(),intersect_objects();
  Ray apply_diffuse_model(),apply_shadow_model();
  triplet		plane_normal2,normal2,plane_normal,normal,normalise();
  double		Random(),intersect_sky();
  int diffuseTransmittance,diffuseReflectance;
  Ray diffuseRay,shadowRay;
  Material_table *material_Ptr; 
  int i,l;

  if(! hitCamera ){
    hitPoint->fromLocation = vector_copy2(ipray->origin.x,ipray->origin.y,ipray->origin.z);
    hitPoint->fromVector = vector_copy2(ipray->direction.x,ipray->direction.y,ipray->direction.z);

    flagbag->isSunTest=FALSE;
    material_Ptr = doIHitAnything(bb,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr,illumination,flagbag,wavebandbag,materialbag,ipray);
    normal=normal2=normalise(normal);
    plane_normal=plane_normal2=normalise(plane_normal);
    /* distance from current point back to viewer or 0 if not in lidar fov */
    for(l=0;l<bb->nSuns;l++)hitPoint->distanceBackToViewer[l]=lidarDistance(materialbag,ipray,&illumination[l],flagbag,l); 
    if(material_Ptr == (Material_table *)-1){
      /* run out of ray tree */
      hitPoint->hitSky=-1;
      for(l=0;l<bb->nSuns;l++)hitPoint->hitSun[l]=-1;
      return(diffuseRay);
    }
  }else{
    /* we make it seem as though the ray has hit the camera */
    objectlist_Ptr->ObjectType = FO;
    objectlist_Ptr->intersection = ipray->origin;
    flagbag->normal = 1;
    hitPoint->fromLocation = vector_copy2(ipray->origin.x,ipray->origin.y,ipray->origin.z);
    hitPoint->fromVector = vector_copy2(ipray->direction.x,ipray->direction.y,ipray->direction.z);
    flagbag->isSunTest=FALSE;
    material_Ptr = &(materialbag->materials[0]); /* WHITE */
    normal=normal2=normalise(vector_copy2(-ipray->direction.x,-ipray->direction.y,-ipray->direction.z));
    plane_normal=plane_normal2=normal;
    for(l=0;l<bb->nSuns;l++)hitPoint->distanceBackToViewer[l]=0.;
  }
  if(material_Ptr){
    
    /* 
    ** weve hit something
    ** and the material we hit is in material_Ptr
    */
    currentMat=getMaterialIndex(material_Ptr,materialbag);
    
    hitPoint->material=material_Ptr;
    hitPoint->materialIndex=currentMat;
    hitPoint->localNormal = normal;

    /* decide on interaction type */
    diffuseReflectance=1;
    if( material_Ptr && material_Ptr->diffuse_transmittance_flag){
      if(Random()<=material_Ptr->transWeightingThreshold)diffuseTransmittance=1;
      else diffuseTransmittance=0;
    }else diffuseTransmittance=0;
    hitPoint->hitSky=0;
    (*tree_depth_Ptr)++;
    
    if(flagbag->reverse){
     for(i=0;i<bb->nSuns;i++){
      shadowRay=apply_shadow_model(i,hitPoint,ipray,materialbag,&illumination[i],wavebandbag,flagbag,material_Ptr,&normal2,&plane_normal2,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);
      shadowRay.ray_length=1e20;shadowRay.lengthOfRaySoFar=0.;shadowRay.rayLengthThroughObject=-1.;
    
      /* does this hit the sun*/
      /* have to fool it that max rtd += 1 */
      flagbag->max_ray_depth ++;
      flagbag->isSunTest=TRUE;
      if(!shadowRay.sourceOfRay || doIHitAnything(bb,&normal2,&plane_normal2,tree_depth_Ptr,&shadowRayList,bbox_Ptr,&illumination[i],flagbag,wavebandbag,materialbag,&shadowRay))
        hitPoint->hitSun[i]=0;
      else
        hitPoint->hitSun[i]=1;
      flagbag->max_ray_depth--;
     }
    }else{
      for(i=0;i<bb->nSuns;i++)hitPoint->hitSun[i]=0; /* for fwd ray tracing, dont fire priority ray */
    }
    if(diffuseTransmittance)
      diffuseRay=apply_diffuse_model(hitPoint,ipray,materialbag,illumination,wavebandbag,flagbag,TRANSMITTANCE,material_Ptr,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);
    else
      diffuseRay=apply_diffuse_model(hitPoint,ipray,materialbag,illumination,wavebandbag,flagbag,REFLECTANCE,material_Ptr,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);

    (*tree_depth_Ptr)--;
    return(diffuseRay);
  }else{
    /*
    **	hit no objects -> hit sky
    */
    
    /* lidar distance for diffuse samples */ 	
    lidarDistance(materialbag,ipray,illumination,flagbag,0);
    /* hit sky */
    hitPoint->hitSky=1;
    return(diffuseRay);
  }
}


/*
**	render pixel
*/

Ray	render(bb,hitPoint,tree_depth_Ptr,ipray,materialbag,illumination,wavebandbag,objectlist_Ptr,bbox_Ptr,flagbag,material_table_Ptr)
     BigBag *bb;
     Material_table	*material_table_Ptr;
     HitPoint *hitPoint;
     ObjectList	*objectlist_Ptr;
     BBox		*bbox_Ptr;
     IlluminationBag		*illumination;
     FlagBag			*flagbag;
     WavebandBag		*wavebandbag;
     MaterialBag		*materialbag;
     Ray			*ipray;
     int                        *tree_depth_Ptr;
{
  ObjectList	shadowRayList;
  int			currentMat,intersect_plane(),intersect_objects();
  Ray apply_diffuse_model(),apply_shadow_model();
  triplet		plane_normal2,normal2,plane_normal,normal,normalise();
  double		Random(),intersect_sky();
  int diffuseTransmittance,diffuseReflectance;
  Ray diffuseRay,shadowRay;
  Material_table *material_Ptr;
  
  hitPoint->fromLocation = vector_copy2(ipray->origin.x,ipray->origin.y,ipray->origin.z);
  hitPoint->fromVector = vector_copy2(ipray->direction.x,ipray->direction.y,ipray->direction.z);
  
  flagbag->isSunTest=FALSE;
  material_Ptr = doIHitAnything(bb,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr,illumination,flagbag,wavebandbag,materialbag,ipray);
  normal=normal2=normalise(normal);
  plane_normal=plane_normal2=normalise(plane_normal);
  /* distance from current point back to viewer or 0 if not in lidar fov */
  lidarDistance(materialbag,ipray,illumination,flagbag,0); 
  if(material_Ptr == (Material_table *)-1){
    /* run out of ray tree */
    hitPoint->hitSky=-1;
    hitPoint->hitSun[0]=-1;
    return(diffuseRay);
  }if(material_Ptr){
    
    /* 
    ** weve hit something
    ** and the material we hit is in material_Ptr
    */
    currentMat=getMaterialIndex(material_Ptr,materialbag);
    
    hitPoint->material=material_Ptr;
    hitPoint->materialIndex=currentMat;
    hitPoint->localNormal = normal;

    /* decide on interaction type */
    diffuseReflectance=1;
    if( material_Ptr && material_Ptr->diffuse_transmittance_flag){
      if(Random()<=material_Ptr->transWeightingThreshold)diffuseTransmittance=1;
      else diffuseTransmittance=0;
    }else diffuseTransmittance=0;
    hitPoint->hitSky=0;
    (*tree_depth_Ptr)++;
    
    if(flagbag->reverse){
      shadowRay=apply_shadow_model(0,hitPoint,ipray,materialbag,illumination,wavebandbag,flagbag,material_Ptr,&normal2,&plane_normal2,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);
      shadowRay.ray_length=1e20;shadowRay.lengthOfRaySoFar=0.;shadowRay.rayLengthThroughObject=-1.;
    
      /* does this hit the sun*/
      /* have to fool it that max rtd += 1 */
      flagbag->max_ray_depth ++;
      flagbag->isSunTest=TRUE;
      if(!shadowRay.sourceOfRay || doIHitAnything(bb,&normal2,&plane_normal2,tree_depth_Ptr,&shadowRayList,bbox_Ptr,illumination,flagbag,wavebandbag,materialbag,&shadowRay))
        hitPoint->hitSun=0;
      else
        hitPoint->hitSun[0]=1;
      flagbag->max_ray_depth--;
    }else{
      hitPoint->hitSun[0]=0; /* for fwd ray tracing, dont fire priority ray */
    }
    if(diffuseTransmittance)
      diffuseRay=apply_diffuse_model(hitPoint,ipray,materialbag,illumination,wavebandbag,flagbag,TRANSMITTANCE,material_Ptr,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);
    else
      diffuseRay=apply_diffuse_model(hitPoint,ipray,materialbag,illumination,wavebandbag,flagbag,REFLECTANCE,material_Ptr,&normal,&plane_normal,tree_depth_Ptr,objectlist_Ptr,bbox_Ptr);

    (*tree_depth_Ptr)--;
    return(diffuseRay);
  }else{
    /*
    **	hit no objects -> hit sky
    */
    
    /* lidar distance for diffuse samples */ 	
    lidarDistance(materialbag,ipray,illumination,flagbag,0);
    /* hit sky */
    hitPoint->hitSky=1;
    return(diffuseRay);
  }
}


Contents_order	*quantise_ray_direction(quantised_direction_Ptr,ray_direction_Ptr,bbox_Ptr)
     triplet	*ray_direction_Ptr;
     BBox	*bbox_Ptr;
     int	*quantised_direction_Ptr;
{
  double	x,y,z;
  
  x=ray_direction_Ptr->x;
  y=ray_direction_Ptr->y;
  z=ray_direction_Ptr->z;
  if(fabs(x)>fabs(y)){
    if(fabs(x)>fabs(z)){	/* x biggest */
      if(FSIGN(x)==-1) *quantised_direction_Ptr=0;
      else *quantised_direction_Ptr=3;
    }else{		/* z biggest */
      if(FSIGN(z)==-1) *quantised_direction_Ptr=2;
      else *quantised_direction_Ptr=5;
    }
  }else{
    if(fabs(y)>fabs(z)){	/* y biggest */
      if(FSIGN(y)==-1) *quantised_direction_Ptr=1;
      else *quantised_direction_Ptr=4;
    }else{		/* z biggest */
      if(FSIGN(z)==-1) *quantised_direction_Ptr=2;
      else *quantised_direction_Ptr=5;
    }
  }
  if(*quantised_direction_Ptr<0 || *quantised_direction_Ptr >5){
    fprintf(stderr,"fatal exception: *quantised_direction_Ptr invalid in quantise_ray_direction()\n");
    exit(1);
  }
  return(bbox_Ptr->hit[*quantised_direction_Ptr]);
}

int	intersect_objects(bb,materialbag,flag_Ptr,bbox_Ptr,ipray,objectlist_Ptr,ray_clip)
     BigBag *bb;
     BBox	*bbox_Ptr;
     Ray	*ipray;
     ObjectList	*objectlist_Ptr;
     int	ray_clip;
     FlagBag	*flag_Ptr;
     MaterialBag		*materialbag;
{
  int		quit_flag=0,hit=0,i,quantised_ray_direction,store_hit=0,ray_to_bbox(),ray_in_sphere(),ray_on_infinite_cylinder(),ray_on_finite_cylinder(),ray_dem_intersect(),ray_to_plane(),point_in_triangle();
  fFacet		*triangle_Ptr,*volumeRayTrace();
  Sphere		*sphere_Ptr;
  Cylinder	*cylinder_Ptr;
  Ellipse		*ellipse_Ptr;
  Spheroid *spheroid_Ptr;
  Disk		*disk_Ptr;
  BiLinearPatch		*blp_Ptr;
  Dem			 *dem_Ptr;
  Spherical_Dem	*spherical_dem_Ptr;
  Clones		*clone_Ptr,*store_clone_Ptr=NULL;
  double		Radius,ray_length_clip,local_coords[2],mod_ray_direction,D[2],p1,p2,L,plane_distance=0;
  double		lengthToTravel,distanceTravelled,ray_length_so_far;
  Contents_order	*contents,*current_object,*previous_object,*object_start;
  triplet		normal,hit_point,*S;
  BBox		*next_bbox_Ptr;
  Ray		transformed_ray,new_ray;
  Volume		*volume;
  int ray_to_infinite_bbox(),vray_in_ellipse(),vray_in_spheroid(),vray_in_sphere(),ray_on_disk(),ray_on_BiLinearPatch(),vray_on_finite_cylinder(),ray_on_ellipse(),ray_on_spheroid();
  if(bbox_Ptr->hit[0]==NULL || bbox_Ptr->hit[0]->hit==NULL)return(0);	/* empty box return */
  previous_object=NULL;
  
  /*
  **	calculate best sort direction
  */
  
  contents=quantise_ray_direction(&quantised_ray_direction,&ipray->direction,bbox_Ptr);
  object_start=contents;
  current_object=object_start;i=0;
  /*
  **	step through ordered list for all potential hits
  */
  while(!quit_flag){
    switch(current_object->type){
    case BBOX:
      if(!current_object->hit)return(0);
      next_bbox_Ptr=(BBox *)current_object->hit;
      if(next_bbox_Ptr->transform_flag){
	transformed_ray.sourceOfRay=ipray->sourceOfRay;
	transformed_ray.origin=multiply_matrix4_vector3(next_bbox_Ptr->m_inverse,ipray->origin,1);
	transformed_ray.direction=multiply_matrix4_vector3(next_bbox_Ptr->m_inverse,ipray->direction,0);
	mod_ray_direction=V_mod(transformed_ray.direction);
	transformed_ray.direction=V_factor(transformed_ray.direction,1.0/mod_ray_direction);
	if(ray_to_bbox(&(plane_distance),&L,next_bbox_Ptr,&transformed_ray) && objectlist_Ptr->RayLength > plane_distance){
	  hit=intersect_objects(bb,materialbag,flag_Ptr,next_bbox_Ptr,&transformed_ray,objectlist_Ptr,0);
	  if(hit)store_hit=2;
	}
      }else{
	/* only need to treat infinite objects at top level
	** ie when ray_clip is set
	*/
	if(bb->flagbag->doInfinite&&ray_clip){
	  {
	    int nBoxHits;
	    double tnearList[MAX_BBOX_LIST],tfarList[MAX_BBOX_LIST];
	    triplet offsetList[MAX_BBOX_LIST];
	    double V_mod();
	    /* there are nBoxHits potential hits which must be checked */
	    nBoxHits=ray_to_infinite_bbox(bb->flagbag,tnearList,tfarList,bbox_Ptr,ipray,offsetList);
	    for(i=0;i<nBoxHits;i++){
	      if(objectlist_Ptr->RayLength > tnearList[i]){
		if(ray_clip){
		  ray_length_clip=bbox_Ptr->max.z-bbox_Ptr->min.z;
		  ray_length_so_far=(double)tnearList[i]-ray_length_clip;
		  if(ray_length_so_far<0){
		    ray_length_so_far=0.0;
		  }		
		  new_ray.sourceOfRay=ipray->sourceOfRay;
		  new_ray.origin=vector_plus(V_factor(ipray->direction,(double)ray_length_so_far),ipray->origin);
		  objectlist_Ptr->RayLength -= (double)ray_length_so_far;
		  new_ray.direction=ipray->direction;
		  hit=intersect_objects(bb,materialbag,flag_Ptr,next_bbox_Ptr,&new_ray,objectlist_Ptr,0);
		  objectlist_Ptr->RayLength+=(double)ray_length_so_far;
		  ipray->rayLengthThroughObject=new_ray.rayLengthThroughObject;
		}else{
		  hit=intersect_objects(bb,materialbag,flag_Ptr,next_bbox_Ptr,ipray,objectlist_Ptr,0);
		}
		if(hit){
		  store_hit=2;
		  objectlist_Ptr->intersection=vector_plus(objectlist_Ptr->intersection,offsetList[i]);
		  objectlist_Ptr->RayLength=V_mod(vector_minus(ipray->origin,objectlist_Ptr->intersection));
		}
	      }
	    }
	  }
	}else{
	  if(ray_to_bbox(&(plane_distance),&L,next_bbox_Ptr,ipray) && objectlist_Ptr->RayLength > plane_distance){
	    if(ray_clip){
	      ray_length_clip=bbox_Ptr->max.z-bbox_Ptr->min.z;
	      ray_length_so_far=(double)plane_distance-ray_length_clip;
	      if(ray_length_so_far<0){
		ray_length_so_far=0.0;
	      }		
	      new_ray.rayLengthThroughObject=-1;
	      new_ray.sourceOfRay=ipray->sourceOfRay;
	      new_ray.origin=vector_plus(V_factor(ipray->direction,(double)ray_length_so_far),ipray->origin);
	      objectlist_Ptr->RayLength -= (double)ray_length_so_far;
	      new_ray.direction=ipray->direction;
	      hit=intersect_objects(bb,materialbag,flag_Ptr,next_bbox_Ptr,&new_ray,objectlist_Ptr,0);
	      objectlist_Ptr->RayLength+=(double)ray_length_so_far;
	      ipray->rayLengthThroughObject=new_ray.rayLengthThroughObject;
	    }else{
	      hit=intersect_objects(bb,materialbag,flag_Ptr,next_bbox_Ptr,ipray,objectlist_Ptr,0);
	    }
	    if(hit)
	      store_hit=2;
	  }
	}
      }
      break;

    case VOLUMETRIC_SPHEROID:
      spheroid_Ptr=(Spheroid *)current_object->hit;
      if(vray_in_spheroid(&(plane_distance),&(lengthToTravel),spheroid_Ptr,ipray,&p1,&p2,D) && objectlist_Ptr->RayLength > plane_distance){
        /* test for intersection within the ellipse and return a facet ! */
        volume=&(materialbag->material_list->material[ellipse_Ptr->material->material[0]].volume);
        if(ipray->sourceOfRay==ILLUMINATION)S=&(volume->s);else S=NULL;
        triangle_Ptr=(fFacet *)volumeRayTrace(bb,flag_Ptr,&distanceTravelled,&(ipray->direction),volume,lengthToTravel,S);
        if(triangle_Ptr){
          triangle_Ptr->material=(Material_table *)(&materialbag->materials[volume->material]);
          objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance+distanceTravelled),ipray->origin);
          objectlist_Ptr->ObjectType=FO;
          objectlist_Ptr->TriangleObj=triangle_Ptr;
          objectlist_Ptr->RayLength=plane_distance+distanceTravelled;
          objectlist_Ptr->rayLengthThroughObject=0.0;
          objectlist_Ptr->normal=normalise(objectlist_Ptr->TriangleObj->normal);
          store_hit=1;
        }
      }
      /* not exactly an interaction:
      ** here, we must fire a ray in the current direction
      ** and see where / if it hits another boundary of the current object
      **
      ** This type is only valid for bounded objects, such
      ** as spheres etc.
      */
      /*
      **        NEED TO MAKE USE OF MATERIAL INFORMATION
      **        AT THIS POint
      **
      **        TODO    VOLUME
      **                        volume=&(materialbag->material_list->material[material->material[0]].volume);
      */
      break;

    case VOLUMETRIC_ELLIPSE:
      ellipse_Ptr=(Ellipse *)current_object->hit;
      if(vray_in_ellipse(&(plane_distance),&(lengthToTravel),ellipse_Ptr,ipray,&p1,&p2,D) && objectlist_Ptr->RayLength > plane_distance){
	/* test for intersection within the ellipse and return a facet ! */
	volume=&(materialbag->material_list->material[ellipse_Ptr->material->material[0]].volume);
	if(ipray->sourceOfRay==ILLUMINATION)S=&(volume->s);else S=NULL;
	triangle_Ptr=(fFacet *)volumeRayTrace(bb,flag_Ptr,&distanceTravelled,&(ipray->direction),volume,lengthToTravel,S);
	if(triangle_Ptr){
	  triangle_Ptr->material=(Material_table *)(&materialbag->materials[volume->material]);
	  objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance+distanceTravelled),ipray->origin);
	  objectlist_Ptr->ObjectType=FO;
	  objectlist_Ptr->TriangleObj=triangle_Ptr;
	  objectlist_Ptr->RayLength=plane_distance+distanceTravelled;
	  objectlist_Ptr->rayLengthThroughObject=0.0;
	  objectlist_Ptr->normal=normalise(objectlist_Ptr->TriangleObj->normal);
	  store_hit=1;
	}			
      }
      /* not exactly an interaction:
      ** here, we must fire a ray in the current direction
      ** and see where / if it hits another boundary of the current object
      **
      ** This type is only valid for bounded objects, such
      ** as spheres etc.
      */
      /* 
      ** 	NEED TO MAKE USE OF MATERIAL INFORMATION
      **	AT THIS POint
      **
      **	TODO	VOLUME
      **			volume=&(materialbag->material_list->material[material->material[0]].volume);
      */
      break;
      
    case VOLUMETRIC_SPHERE:
      sphere_Ptr=(Sphere *)current_object->hit;
      if(vray_in_sphere(&(plane_distance),&(lengthToTravel),sphere_Ptr,ipray) && objectlist_Ptr->RayLength > plane_distance){
	/* test for intersection within the sphere and return a facet ! */
	volume=&(materialbag->material_list->material[sphere_Ptr->material->material[0]].volume);
	if(ipray->sourceOfRay==ILLUMINATION)S=&(volume->s);else S=NULL;
	triangle_Ptr=(fFacet *)volumeRayTrace(bb,flag_Ptr,&distanceTravelled,&ipray->direction,volume,lengthToTravel,S);
	if(triangle_Ptr){
	  triangle_Ptr->material=(Material_table *)(&materialbag->materials[volume->material]);
	  objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance+distanceTravelled),ipray->origin);
	  objectlist_Ptr->ObjectType=FO;
	  objectlist_Ptr->TriangleObj=triangle_Ptr;
	  objectlist_Ptr->RayLength=plane_distance+distanceTravelled;
	  objectlist_Ptr->rayLengthThroughObject=0.0;
	  objectlist_Ptr->normal=normalise(objectlist_Ptr->TriangleObj->normal);
	  store_hit=1;
	}			
      }
      /* not exactly an interaction:
      ** here, we must fire a ray in the current direction
      ** and see where / if it hits another boundary of the current object
      **
      ** This type is only valid for bounded objects, such
      ** as spheres etc.
      */
      /* 
      ** 	NEED TO MAKE USE OF MATERIAL INFORMATION
      **	AT THIS POint
      **
      **	TODO	VOLUME
      **			volume=&(materialbag->material_list->material[material->material[0]].volume);
      */
      break;
      
    case SPHERE:
      sphere_Ptr=(Sphere *)current_object->hit;
      if(ray_in_sphere(&normal,&(plane_distance),sphere_Ptr,ipray,local_coords) && objectlist_Ptr->RayLength > plane_distance){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	objectlist_Ptr->ObjectType=SPHERE;
	objectlist_Ptr->SphereObj=sphere_Ptr;
	objectlist_Ptr->RayLength=plane_distance;
	objectlist_Ptr->rayLengthThroughObject=ipray->rayLengthThroughObject;
	objectlist_Ptr->normal=normal;
	objectlist_Ptr->ancilliary[0]=local_coords[0];
	objectlist_Ptr->ancilliary[1]=local_coords[1];
	store_hit=1;			
      }
      break;
    case DISK:
      disk_Ptr=(Disk *)current_object->hit;
      if(ray_on_disk(&(plane_distance),disk_Ptr,ipray,&Radius) && objectlist_Ptr->RayLength > plane_distance){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	objectlist_Ptr->ObjectType=DISK;
	objectlist_Ptr->DiskObj=disk_Ptr;
	objectlist_Ptr->RayLength=plane_distance;
	objectlist_Ptr->rayLengthThroughObject=0.0;
	objectlist_Ptr->normal=normalise(objectlist_Ptr->DiskObj->normal);
	store_hit=1;			
      }
      break;
    case BILINEARPATCH:
      blp_Ptr=(BiLinearPatch *)current_object->hit;
      if(ray_on_BiLinearPatch(&(plane_distance),blp_Ptr,ipray,&Radius) && objectlist_Ptr->RayLength > plane_distance){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	objectlist_Ptr->ObjectType=BILINEARPATCH;
	objectlist_Ptr->BiLinearPatchObj=blp_Ptr;
	objectlist_Ptr->RayLength=plane_distance;
	objectlist_Ptr->rayLengthThroughObject=0.0;
	objectlist_Ptr->normal=normalise(objectlist_Ptr->BiLinearPatchObj->normal);
	store_hit=1;			
      }
      break;
    case CYLINDER:
    case CLOSED_CYLINDER:
      cylinder_Ptr=(Cylinder *)current_object->hit;
      if(ray_on_infinite_cylinder(&(plane_distance),cylinder_Ptr,ipray,&p1,&p2,D) && objectlist_Ptr->RayLength > plane_distance){
	hit=ray_on_finite_cylinder(cylinder_Ptr,&plane_distance,&p1,&p2,D,ipray);
	if(hit && objectlist_Ptr->RayLength > plane_distance){
	  objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	  objectlist_Ptr->ObjectType=CYLINDER;
	  objectlist_Ptr->CylinderObj= cylinder_Ptr;
	  objectlist_Ptr->RayLength=plane_distance;
	  objectlist_Ptr->rayLengthThroughObject=ipray->rayLengthThroughObject;

	  if(objectlist_Ptr->CylinderObj->ends && (objectlist_Ptr->CylinderObj->store_h==objectlist_Ptr->CylinderObj->height||objectlist_Ptr->CylinderObj->store_h==0)){ /* end of cylinder */
	    objectlist_Ptr->normal=normalise(objectlist_Ptr->CylinderObj->normal);
	  }else objectlist_Ptr->normal=normalise(vector_minus(objectlist_Ptr->real_intersection,vector_plus(objectlist_Ptr->CylinderObj->origin,V_factor(objectlist_Ptr->CylinderObj->normal,objectlist_Ptr->CylinderObj->store_h))));
	  store_hit=1;
	}
      }
      /* note dummies p1,p2 used to pass **
      ** local info			   */
      break;
    case VOLUMETRIC_CYLINDER:
      cylinder_Ptr=(Cylinder *)current_object->hit;
      if(ray_on_infinite_cylinder(&(plane_distance),cylinder_Ptr,ipray,&p1,&p2,D) && objectlist_Ptr->RayLength > plane_distance){
	if(vray_on_finite_cylinder(bb,&(plane_distance),&(lengthToTravel),cylinder_Ptr,ipray,&p1,&p2,D) && objectlist_Ptr->RayLength > plane_distance){
	  /* test for intersection within the sphere and return a facet ! */
	  volume=&(materialbag->material_list->material[cylinder_Ptr->material->material[0]].volume);
	  if(ipray->sourceOfRay==ILLUMINATION)S=&(volume->s);else S=NULL;
	  triangle_Ptr=(fFacet *)volumeRayTrace(flag_Ptr,&distanceTravelled,&ipray->direction,volume,lengthToTravel,S);
	  if(triangle_Ptr){
	    objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance+distanceTravelled),ipray->origin);
	    triangle_Ptr->material=(Material_table *)(&materialbag->materials[volume->material]);
	    objectlist_Ptr->ObjectType=FO;
	    objectlist_Ptr->TriangleObj=triangle_Ptr;
	    objectlist_Ptr->RayLength=plane_distance+distanceTravelled;
	    objectlist_Ptr->rayLengthThroughObject=0.0;
	    objectlist_Ptr->normal=normalise(objectlist_Ptr->TriangleObj->normal);
	    store_hit=1;
	  }			
	}
      }
      /* note dummies p1,p2 used to pass **
      ** local info			   */
      
      break;
    case ELLIPSE:
      ellipse_Ptr=(Ellipse *)current_object->hit;
      if((hit = ray_on_ellipse(&(plane_distance),ellipse_Ptr,ipray,&p1,&p2,D)) && objectlist_Ptr->RayLength > plane_distance){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	objectlist_Ptr->ObjectType=ELLIPSE;
	objectlist_Ptr->EllipseObj= ellipse_Ptr;
	objectlist_Ptr->RayLength=plane_distance;
	objectlist_Ptr->rayLengthThroughObject=ipray->rayLengthThroughObject;

	{
	  static triplet	tmp;static double Tx,Ty,Tz;
	  Tx=objectlist_Ptr->EllipseObj->dimensions.x;
	  Ty=objectlist_Ptr->EllipseObj->dimensions.y;
	  Tz=objectlist_Ptr->EllipseObj->dimensions.z;
	  tmp=vector_copy2(1.0/(Tx*Tx),1.0/(Ty*Ty),1.0/(Tz*Tz));
	  objectlist_Ptr->normal=normalise(multiply_vector(vector_minus(objectlist_Ptr->real_intersection,objectlist_Ptr->EllipseObj->origin),tmp));
	}
	store_hit=1;
      }
      /* note dummies p1,p2 used to pass **
      ** local info			   */
      break;
    case SPHEROID:
      spheroid_Ptr=(Spheroid*)current_object->hit;
      if((hit = ray_on_spheroid(&(plane_distance),spheroid_Ptr,ipray,&p1,&p2,D)) && objectlist_Ptr->RayLength > plane_distance){
        objectlist_Ptr->intersection=objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
        objectlist_Ptr->ObjectType=SPHEROID;
        objectlist_Ptr->SpheroidObj= spheroid_Ptr;
        objectlist_Ptr->RayLength=plane_distance;
        objectlist_Ptr->rayLengthThroughObject=ipray->rayLengthThroughObject;

        {
          /* get the normal ... a bit complicated
          ** in essence, we need to find the foci in the plane of 
          ** the up vector (V1) and the vector from the intersection point
          ** to the centre
          */
          static triplet        IC,H,dv,f[2],v[2],vn[2],tmp,v0,v1;
          static double NX,NY,X,Y,a,b,d,lambda,num,dom;

          tmp = vector_minus(objectlist_Ptr->real_intersection,objectlist_Ptr->SpheroidObj->centre);
          IC = tmp;
          v1 = normalise(tmp);
          v0 = vector_cross(spheroid_Ptr->V1,v1);
          d = V_mod(v0);
          /* if its zero, then v1 is the normal */
          if(d==0){
            objectlist_Ptr->normal=v1;
          }else{
/*
** normal is quite tricksy
**
** the spheroid centre is C
**
** Consider the intersection point I and the foci f0, f1 in the plane
** that contains I-C and the primary axis of the spheroid V1.
*/

            /* first then get 2 orthogonal unit vectors in the plane of interest
            ** These are then spheroid_Ptr->V1 and v1
            */
            v0 = V_factor(v0,1./d);
            v1 = normalise(vector_cross(v0,spheroid_Ptr->V1));
            /* we can now consider an ellipse in the plane defined by
            ** spheroid_Ptr->V1 and v1
            ** This ellipse has semi-minor spheroid_Ptr->dimensions[0] (r)
            ** and semi-major length spheroid_Ptr->dimensions[2]  
            */
            /* now the focus is along the vector spheroid_Ptr->V1 from centre 
            ** iff it is oblate. We know this depending on d
            */
            if(objectlist_Ptr->SpheroidObj->a2>0){
              /* oblate */
              v0 = spheroid_Ptr->V1;
              v1 = v1;
              a = spheroid_Ptr->dimensions.z;
              b = spheroid_Ptr->dimensions.x;
            }else{
              v0 = v1;
              v1 = spheroid_Ptr->V1;
              b = spheroid_Ptr->dimensions.z;
              a = spheroid_Ptr->dimensions.x;
            }
            /* now project I-C onto plane */
            X = V_dot(IC,v0);
            Y = V_dot(IC,v1);
            /* normal direction is from http://people.maths.ox.ac.uk/earl/lecture11a.pdf */
            NX = b*X/a;
            NY = a*Y/b;
            /* project back */
            tmp = vector_plus(V_factor(v0,NX),V_factor(v1,NY));
            objectlist_Ptr->normal=normalise(tmp);
          }
          /* dummy at present for local coords */
          objectlist_Ptr->ancilliary[0]=objectlist_Ptr->ancilliary[1]= 0.0;
        }
        spheroid_Ptr->normal = objectlist_Ptr->normal;
        store_hit=1;
      }
      /* note dummies p1,p2 used to pass **
      ** local info                        */
      break;

    case DEM:
      dem_Ptr=(Dem *)current_object->hit;
      /* note dummies p1,p2 used to pass **
      ** local info			   */
      if(ray_dem_intersect(bb,&normal,&(objectlist_Ptr->RayLength),dem_Ptr,ipray,DEM,local_coords)){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,objectlist_Ptr->RayLength),ipray->origin);
	objectlist_Ptr->ObjectType=DEM;
	objectlist_Ptr->DemObj= dem_Ptr;
	objectlist_Ptr->normal=normal;
	objectlist_Ptr->ancilliary[0]=local_coords[0];
	objectlist_Ptr->ancilliary[1]=local_coords[1];
	objectlist_Ptr->rayLengthThroughObject=0.0;
	store_hit=1;
      }
      break;
    case SPHERICAL_DEM:
      spherical_dem_Ptr=(Spherical_Dem *)current_object->hit;
      /* note dummies p1,p2 used to pass **
      ** local info			   */
      if(ray_dem_intersect(bb,&normal,&(objectlist_Ptr->RayLength),spherical_dem_Ptr,ipray,SPHERICAL_DEM) && objectlist_Ptr->RayLength > plane_distance){
	objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,objectlist_Ptr->RayLength),ipray->origin);
	objectlist_Ptr->ObjectType=SPHERICAL_DEM;
	objectlist_Ptr->DemObj= (Dem *)spherical_dem_Ptr;
	objectlist_Ptr->normal=normal;
	objectlist_Ptr->ancilliary[0]=p1;
	objectlist_Ptr->ancilliary[1]=p2;
	objectlist_Ptr->rayLengthThroughObject=0.0;
	store_hit=1;
      }
      break;
    case FO:
      triangle_Ptr=(fFacet *)current_object->hit;
      if(ray_to_plane(&(plane_distance),&(triangle_Ptr->dw),&(triangle_Ptr->normal),ipray) && objectlist_Ptr->RayLength > plane_distance){
	if(point_in_triangle(&hit_point,triangle_Ptr,ipray,plane_distance,local_coords)){				
	  objectlist_Ptr->ObjectType=FO;
	  objectlist_Ptr->TriangleObj= triangle_Ptr;
	  objectlist_Ptr->RayLength=plane_distance;
	  objectlist_Ptr->normal=normalise(triangle_Ptr->normal);
	  objectlist_Ptr->real_intersection=vector_plus(V_factor(ipray->direction,plane_distance),ipray->origin);
	  /* store local coords of hit */
	  objectlist_Ptr->ancilliary[0]=local_coords[0];
	  objectlist_Ptr->ancilliary[1]=local_coords[1];
	  objectlist_Ptr->rayLengthThroughObject=0.0;
	  store_hit=1;
	}
      }
      break;
    case CLONE:
      clone_Ptr=(Clones *)current_object->hit;
      transformed_ray.sourceOfRay=ipray->sourceOfRay;
      transformed_ray.origin=vector_minus(ipray->origin,clone_Ptr->translation);
      transformed_ray.direction=ipray->direction;
      if(clone_Ptr->z_rotation_flag){
        { double mod=1.0,V_mod();
          triplet V_factor();
	transformed_ray.direction=rotateByMatrix(ipray->direction,clone_Ptr->matrix,1);
        mod = V_mod(transformed_ray.direction);
        transformed_ray.direction=V_factor(transformed_ray.direction,1.0/mod);
	transformed_ray.origin=rotateByMatrix(transformed_ray.origin,clone_Ptr->matrix,1);
        /* 
        ** July 8 2011 Lewis:
        ** this might seem odd 
        ** if the matrix has a scaling associated with it, it will shrink the location
        ** by a factor mod
        ** but actually, we need to expand by that anyway
        ** so we need to scale by mod^2
        */
        transformed_ray.origin=V_factor(transformed_ray.origin,1.0/(mod*mod));
        
	objectlist_Ptr->RayLength = objectlist_Ptr->RayLength/mod;
        if(ray_to_bbox(&(plane_distance),&L,clone_Ptr->bbox,&transformed_ray) && (objectlist_Ptr->RayLength) > plane_distance){
          
	  hit=intersect_objects(bb,materialbag,flag_Ptr,(clone_Ptr->bbox),&transformed_ray,objectlist_Ptr,0);
	  /* rotate normal */
	 
          /* July 8 2011 Lewis: normalise normal in case theres any scaling on the matrix */ 
	  if(hit){
	     objectlist_Ptr->normal=normalise(rotateByMatrix(objectlist_Ptr->normal,clone_Ptr->matrix,0));
             /*objectlist_Ptr->real_intersection = rotateByMatrix(objectlist_Ptr->real_intersection,clone_Ptr->matrix,0);
             objectlist_Ptr->real_intersection = vector_plus(objectlist_Ptr->real_intersection,clone_Ptr->translation);*/
	  }
	  /* something like this ... */
	}else hit=0;
        objectlist_Ptr->RayLength = objectlist_Ptr->RayLength*mod;
       }
      }else{
	if(ray_to_bbox(&(plane_distance),&L,clone_Ptr->bbox,&transformed_ray) && objectlist_Ptr->RayLength > plane_distance){
	  hit=intersect_objects(bb,materialbag,flag_Ptr,(clone_Ptr->bbox),&transformed_ray,objectlist_Ptr,0);
	}else hit=0; 
      }
      if(hit){
	store_hit=3;
	store_clone_Ptr=clone_Ptr;
      }
      break;
    }
    if(current_object->next_hit == NULL)quit_flag=1;
    else current_object=current_object->next_hit;
  }
  
  if(store_hit){
    objectlist_Ptr->intersection=vector_plus(V_factor(ipray->direction,objectlist_Ptr->RayLength),ipray->origin);
    if(store_hit==1){
      objectlist_Ptr->transform_flag=bbox_Ptr->transform_flag;
    }else{
      if(store_hit==3){		/* clone */
	;
      }
    }
    return(1);
  }
  return(0);
}

