#include "prat.h"

/*
**	sort bbox bounds aligned to axes
*/

void	calculate_bbox_limits(bounds,bbox_Ptr)
BBox	*bbox_Ptr;
double	*bounds;
{
	triplet	vmax(),vmin();
/*
**	0 -> x.max
**	1 -> y.max
**	2 -> z.max
**	3 -> x.min
**	4 -> y.min
**	5 -> z.min
*/

/*
	if(bbox_Ptr->transform_flag){	/ transform bbox  /
		v[0]=vector_copy2(bbox_Ptr->max.x,bbox_Ptr->max.y,bbox_Ptr->max.z);
		v[1]=vector_copy2(bbox_Ptr->max.x,bbox_Ptr->max.y,bbox_Ptr->min.z);
		v[2]=vector_copy2(bbox_Ptr->max.x,bbox_Ptr->min.y,bbox_Ptr->max.z);
		v[3]=vector_copy2(bbox_Ptr->max.x,bbox_Ptr->min.y,bbox_Ptr->min.z);
		v[4]=vector_copy2(bbox_Ptr->min.x,bbox_Ptr->max.y,bbox_Ptr->max.z);
		v[5]=vector_copy2(bbox_Ptr->min.x,bbox_Ptr->max.y,bbox_Ptr->min.z);
		v[6]=vector_copy2(bbox_Ptr->min.x,bbox_Ptr->min.y,bbox_Ptr->max.z);
		v[7]=vector_copy2(bbox_Ptr->min.x,bbox_Ptr->min.y,bbox_Ptr->min.z);
		bbox_Ptr->max=vector_copy2(-BIG,-BIG,-BIG);
		bbox_Ptr->min=vector_copy2(BIG,BIG,BIG);
		for(i=0;i<8;i++){
			bbox_Ptr->max=vmax(multiply_matrix4_vector3(bbox_Ptr->m,v[i],1),bbox_Ptr->max);
			bbox_Ptr->min=vmin(multiply_matrix4_vector3(bbox_Ptr->m,v[i],1),bbox_Ptr->min);
		}
	}
*/
		
	bounds[0]=bbox_Ptr->max.x;
	bounds[1]=bbox_Ptr->max.y;
	bounds[2]=bbox_Ptr->max.z;
	bounds[3]=bbox_Ptr->min.x;
	bounds[4]=bbox_Ptr->min.y;
	bounds[5]=bbox_Ptr->min.z;
}


/*
**	calculate cg of objects (obsolete)
*/

triplet	calculate_sphere_cg(obj,i)
Sphere *obj;int	i;
{
	return(obj[i].centre);
}

triplet	calculate_cylinder_cg(obj,i)
Cylinder *obj;int	i;
{
	return(vector_plus(obj[i].origin,V_factor(obj[i].normal,obj[i].height)));
}

/*
**
*/

int	sort_in_link_list(slot,timer_Ptr,base_hit,bounds,data)
Contents_order *base_hit[6],*data,*slot[6];
double	*bounds;
int	*timer_Ptr;
{
	int	l;
	Contents_order *current_hit,*prev_hit;
	
/*
**	setup bounds limits (max)
*/
	for(l=0;l<3;l++){
		prev_hit=NULL;
		current_hit=base_hit[l];
		if(*timer_Ptr == 1){	/* first entry to link-list */
			current_hit->type=data->type;
			current_hit->hit=data->hit;
			current_hit->D=bounds[l];
		}else{
			while(current_hit!=NULL&&bounds[l]<current_hit->D){
				prev_hit=current_hit;
				current_hit=current_hit->next_hit;
			}
	/* slot data in slot inbetween prev_hit and current_hit */
			if(prev_hit!=NULL)
				prev_hit->next_hit=slot[l];
			else
				base_hit[l]=slot[l];
			slot[l]->type=data->type;
			slot[l]->hit=data->hit;
			slot[l]->D=bounds[l];
			slot[l]->next_hit=current_hit;
		}
	}
/*
**	setup bounds limits (min)
*/
	for(l=3;l<6;l++){
		prev_hit=NULL;
		current_hit=base_hit[l];
		if(*timer_Ptr == 1){	/* first entry to link-list */
			current_hit->type=data->type;
			current_hit->hit=data->hit;
			current_hit->D=bounds[l];
		}else{
			while(current_hit!=NULL&&bounds[l]>current_hit->D){
				prev_hit=current_hit;
				current_hit=current_hit->next_hit;
			}
	/* slot data in slot inbetween prev_hit and current_hit */
			if(prev_hit!=NULL)
				prev_hit->next_hit=slot[l];
			else
				base_hit[l]=slot[l];
			slot[l]->type=data->type;
			slot[l]->hit=data->hit;
			slot[l]->D=bounds[l];
			slot[l]->next_hit=current_hit;
		}
	}
	return(1);
}

/* BBox intercept code using inequality equations to solve for	**
** ray length limits from ray_origin in direction ray_direction	**
** when intesecting bbox (return(0) if fail)			*/

int	bbox_shuffle(tnear,tfar,min,max,origin,direction)
double	min,max,origin,direction,*tnear,*tfar;
{
	double	t1,t2;

	if(direction==0.0){
		if((origin<min)||(origin>max))return(0);
	}else{
		t1=(min-origin)/direction;
		t2=(max-origin)/direction;
		if(t1>t2){
			if(t2>*tnear)*tnear=t2;
			if(t1<*tfar)*tfar=t1;
		}else{
			if(t1>*tnear)*tnear=t1;
			if(t2<*tfar)*tfar=t2;
		}
		if(*tnear>*tfar)return(0);
		if(*tfar<0.0)return(0);
	}
	return(1);
}

int	bbox_infinite_shuffle(tnear,tfar,min,max,origin,direction,offset)
double	min,max,origin,direction,*tnear,*tfar,offset;
{
	double	t1,t2;

	if(direction==0.0){
		if((origin<min)||(origin>max))return(0);
	}else{
		t1=(min-origin)/direction;
		t2=(max-origin)/direction;
		if(t1>t2){
			if(t2>*tnear)*tnear=t2;
			if(t1<*tfar)*tfar=t1;
		}else{
			if(t1>*tnear)*tnear=t1;
			if(t2<*tfar)*tfar=t2;
		}
		if(*tnear>*tfar)return(0);
		if(*tfar<0.0)return(0);
	}
	return(1);
}

int testIntersection(double d,double origin,double direction,double bmin,double bmax){
  double intersection;
  intersection=origin + d * direction;
  if(intersection<bmin || intersection>bmax)
    return(0);
  return(1);
}

int intersectsOk(double d,struct Bbox *bbox,Ray *ray,int axis){

  switch(axis){
  case 0:
    /* test axes 1 & 2: y,z */
    return(testIntersection(d,ray->origin.y,ray->direction.y,bbox->min.y,bbox->max.y) || testIntersection(d,ray->origin.z,ray->direction.z,bbox->min.z,bbox->max.z));
    break;
 case 1:
    /* test axes 0 & 2: x,z */
    return(testIntersection(d,ray->origin.x,ray->direction.x,bbox->min.x,bbox->max.x) || testIntersection(d,ray->origin.z,ray->direction.z,bbox->min.z,bbox->max.z));
    break;
 case 2:
    /* test axes 0 & 1: x,y */
    return(testIntersection(d,ray->origin.y,ray->direction.y,bbox->min.y,bbox->max.y) || testIntersection(d,ray->origin.x,ray->direction.x,bbox->min.x,bbox->max.x));
    break;
  }
  return(0);
}

int intersectsOkd(double d,struct Bbox *bbox,D_Ray *ray,int axis){

  switch(axis){
  case 0:
    /* test axes 1 & 2: y,z */
    return(testIntersection(d,ray->origin.y,ray->direction.y,bbox->min.y,bbox->max.y) || testIntersection(d,ray->origin.z,ray->direction.z,bbox->min.z,bbox->max.z));
    break;
 case 1:
    /* test axes 0 & 2: x,z */
    return(testIntersection(d,ray->origin.x,ray->direction.x,bbox->min.x,bbox->max.x) || testIntersection(d,ray->origin.z,ray->direction.z,bbox->min.z,bbox->max.z));
    break;
 case 2:
    /* test axes 0 & 1: x,y */
    return(testIntersection(d,ray->origin.y,ray->direction.y,bbox->min.y,bbox->max.y) || testIntersection(d,ray->origin.x,ray->direction.x,bbox->min.x,bbox->max.x));
    break;
  }
  return(0);
}


int	double_bbox_shuffle(tnear,tfar,min,max,origin,direction,bbox,ray,axis)
     double	min,max,origin,direction,*tnear,*tfar;D_Ray *ray;struct Bbox *bbox;int axis;
{
	double	t1,t2, fabs();

	if(fabs(direction) <= BOXTOL){
		if((origin<min)||(origin>max))return(0);
	}else{
		t1=(min-origin)/direction;
		t2=(max-origin)/direction;
		if(t1>t2){
			if(t2>*tnear){
			  /* now check that it interestects the side */
			  if(intersectsOkd(t2,bbox,ray,axis))
			     *tnear=t2;
			}
			if(t1<*tfar)*tfar=t1;
		}else{
			if(t1>*tnear){
			  if(intersectsOkd(t1,bbox,ray,axis))
			     *tnear=t1;
			}
			if(t2<*tfar)*tfar=t2;
		}
		if(*tnear-TTOL>*tfar)return(0);
		if(*tfar<0.0)return(0);
	}
	return(1);
}


/* BBox intercept code using inequality equations to solve for	**
** ray length limits from ray_origin in direction ray_direction	**
** when intesecting bbox (return(0) if fail)			*/

int	far_bbox_shuffle(tnear,tfar,min,max,origin,direction)
double	min,max,origin,direction,*tnear,*tfar;
{
	double	t1,t2;

	if(direction==0.0){
		if((origin<min)||(origin>max))return(0);
	}else{
		t1=(min-origin)/direction;
		t2=(max-origin)/direction;
		if(t1>t2){
			if(t1<*tfar)*tfar=t1;
		}else{
			if(t2<*tfar)*tfar=t2;
		}
		if(*tnear>*tfar)return(0);
		if(*tfar<0.0)return(0);
	}
	return(1);
}

/*
**	return 2 points of intersection with bbox
*/

int	ray_to_bbox(tnear_Ptr,tfar_Ptr,bbox_Ptr,ray_Ptr)
double	*tnear_Ptr,*tfar_Ptr;
struct Bbox	*bbox_Ptr;
Ray	*ray_Ptr;
{
	if(bbox_Ptr->min.x==BIG)return(0);
	*tnear_Ptr= -BIG;
	*tfar_Ptr=BIG;
	if( 	bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.x,bbox_Ptr->max.x,ray_Ptr->origin.x,ray_Ptr->direction.x) &&
		bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.y,bbox_Ptr->max.y,ray_Ptr->origin.y,ray_Ptr->direction.y) &&
		bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.z,bbox_Ptr->max.z,ray_Ptr->origin.z,ray_Ptr->direction.z)){
		if(*tnear_Ptr <0.0) *tnear_Ptr=0.0;
		return(1);
	}
	return(0);	
}

int	ray_to_infinite_bbox(flagbag,tnear_Ptr,tfar_Ptr,bbox_Ptr,ray_Ptr,offset)
     triplet *offset;
double	*tnear_Ptr,*tfar_Ptr;
struct Bbox	*bbox_Ptr;
Ray	*ray_Ptr;
FlagBag *flagbag;
{
  if(bbox_Ptr->min.x==BIG)return(0);

  /* infinite extension:
  ** 
  */
  *tnear_Ptr= -BIG;
  *tfar_Ptr=BIG;
  if( 	bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.x,bbox_Ptr->max.x,ray_Ptr->origin.x,ray_Ptr->direction.x) &&
	bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.y,bbox_Ptr->max.y,ray_Ptr->origin.y,ray_Ptr->direction.y) &&
	bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.z,bbox_Ptr->max.z,ray_Ptr->origin.z,ray_Ptr->direction.z)){
    if(*tnear_Ptr <0.0) *tnear_Ptr=0.0;
    return(1);
  }
  return(0);	
}

int	double_ray_to_bbox(tnear_Ptr,tfar_Ptr,bbox_Ptr,ray_Ptr)
double	*tnear_Ptr,*tfar_Ptr;
struct Bbox	*bbox_Ptr;
D_Ray	*ray_Ptr;
{
	if(bbox_Ptr->min.x==BIG)return(0);
	*tnear_Ptr= -BIG;
	*tfar_Ptr=BIG;
	if( 	double_bbox_shuffle(tnear_Ptr,tfar_Ptr,(double)bbox_Ptr->min.x,(double)bbox_Ptr->max.x,ray_Ptr->origin.x,ray_Ptr->direction.x,bbox_Ptr,ray_Ptr,0) &&
		double_bbox_shuffle(tnear_Ptr,tfar_Ptr,(double)bbox_Ptr->min.y,(double)bbox_Ptr->max.y,ray_Ptr->origin.y,ray_Ptr->direction.y,bbox_Ptr,ray_Ptr,1) &&
		double_bbox_shuffle(tnear_Ptr,tfar_Ptr,(double)bbox_Ptr->min.z,(double)bbox_Ptr->max.z,ray_Ptr->origin.z,ray_Ptr->direction.z,bbox_Ptr,ray_Ptr,2)){
	  /* we are inside the box */
	  if(*tnear_Ptr <0.0) *tnear_Ptr=0.0;	  
	  return(1);
	}
	return(0);	
}

/*
**	return 2 points of intersection with bbox.. 
**	.........coded to which side(s) they hit
*/

int	coded_ray_to_bbox(tnear_Ptr,tfar_Ptr,bbox_Ptr,ray_Ptr,near_code,far_code)
double	*tnear_Ptr,*tfar_Ptr;
struct Bbox	*bbox_Ptr;
Ray	*ray_Ptr;
int	*near_code,*far_code;
{
/*
**	codes: 
**	0-7:	(see clone.h)
**	-1:	thro' top or bottom of box
*/	

	double	d[2];

	if(bbox_Ptr->min.x==BIG)return(0);

	/* intersect z-planes */
	
	if(ray_Ptr->direction.z!=0.0){
		d[0]= (bbox_Ptr->min.z - ray_Ptr->origin.z)/ray_Ptr->direction.z;
		d[1]= (bbox_Ptr->max.z - ray_Ptr->origin.z)/ray_Ptr->direction.z;
	}
	

	*tnear_Ptr= -BIG;
	*tfar_Ptr=BIG;
	if( 	bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.x,bbox_Ptr->max.x,ray_Ptr->origin.x,ray_Ptr->direction.x) &&
		bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.y,bbox_Ptr->max.y,ray_Ptr->origin.y,ray_Ptr->direction.y) &&
		bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_Ptr->min.z,bbox_Ptr->max.z,ray_Ptr->origin.z,ray_Ptr->direction.z)){
		if(*tnear_Ptr <0.0) *tnear_Ptr=0.0;
		return(1);
	}
	return(0);	
}
/*
**	return far point of intersection with bbox
*/

int	far_ray_to_bbox(tnear_Ptr,tfar_Ptr,bbox_min,bbox_max,ray_origin_Ptr,ray_direction_Ptr)
double	*tnear_Ptr,*tfar_Ptr;
triplet	*bbox_min, *bbox_max;
triplet	*ray_origin_Ptr;
triplet	*ray_direction_Ptr;
{
	if(bbox_min->x==BIG)return(0);
	*tfar_Ptr= BIG;
	if( 	far_bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_min->x,bbox_max->x,ray_origin_Ptr->x,ray_direction_Ptr->x) &&
		far_bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_min->y,bbox_max->y,ray_origin_Ptr->y,ray_direction_Ptr->y) &&
		far_bbox_shuffle(tnear_Ptr,tfar_Ptr,bbox_min->z,bbox_max->z,ray_origin_Ptr->z,ray_direction_Ptr->z)){
		return(1);
	}
	return(0);	/* shouldn't end up here */	
}
