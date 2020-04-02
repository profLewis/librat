#include "prat.h"

void	setup_supercell(supercell,bbox)
SuperCell	*supercell;
BBox		*bbox;
{
	TRIPLET	bbox_extent;

	supercell->object_bbox 	=  bbox;
	supercell->bbox 	= *bbox;

	supercell->bbox.min.x = bbox->max.x - supercell->dx;
	supercell->bbox.max.x = bbox->min.x + supercell->dx;
	supercell->bbox.min.y = bbox->max.y - supercell->dy;
	supercell->bbox.max.y = bbox->min.y + supercell->dy;
	
	bbox_extent = vector_minus(bbox->max,bbox->min);

	supercell->x_overlap = (int)(bbox_extent.x/dx);
	supercell->y_overlap = (int)(bbox_extent.y/dy);

	supercell->2x_overlap = 2*supercell->x_overlap + 1;
	supercell->2y_overlap = 2*supercell->y_overlap + 1;

	supercell->done = (double *)v_allocate(supercell->2x_overlap*supercell->2y_overlap,sizeof(double));

	return;
}

int	local_done_shift(supercell,direction)
SuperCell	*supercell;
int		direction;
{
	int	i,j;
	
	switch(direction){
		case DOWN:
		SHIFT_DOWN STET_ROW 	done(i,j) = done(i+1,j);
		CLEAR_DOWN break;
		case DOWN_RIGHT:
		SHIFT_DOWN SHIFT_RIGHT 	done(i,j) = done(i+1,j+1);
		CLEAR_DOWN CLEAR_RIGHT break;
		case RIGHT:
		STET_COL   SHIFT_RIGHT 	done(i,j) = done(i,j+1);
		CLEAR_RIGHT break;
		case UP_RIGHT:
		SHIFT_UP   SHIFT_RIGHT 	done(i,j) = done(i-1,j+1);
		CLEAR_UP CLEAR_RIGHT break;
		case UP:
		SHIFT_UP   STET_ROW	done(i,j) = done(i-1,j);
		CLEAR_UP break;
		case UP_LEFT:
		SHIFT_UP   SHIFT_LEFT 	done(i,j) = done(i-1,j-1);
		CLEAR_UP CLEAR_LEFT break;
		case LEFT:
		STET_COL   SHIFT_LEFT 	done(i,j) = done(i,j-1);
		CLEAR_LEFT break;
		case DOWN_LEFT:
		SHIFT_DOWN  SHIFT_LEFT 	done(i,j) = done(i+1,j-1);
		CLEAR_DOWN CLEAR_LEFT break;
		default:
		error1("local_done_shift:\tillegal direction code passed to function");
	}
	return(1);
}

void	setup_root_offset(root_offset,n,m,supercell)
TRIPLET		*root_offset;
int		n,m;
SuperCell	*supercell;
{
	root_offset->x = (supercell->object_bbox->min.x) + (n * supercell->dx);
	root_offset->y = (supercell->object_bbox->min.y) + (n * supercell->dy);
	root_offset->z = supercell->object_bbox->min.z;
	
	return;
}

void	find_root_cell(ray,supercell,col_Ptr,row_Ptr)
Ray		*ray;
SuperCell	*supercell;
int		*row_Ptr,*col_Ptr;
{
	TRIPLET	intersect;
	double	D[2],d;
	int	upper;
		
	if(ray->direction.x==0.0 && ray->direction.y==0.0){
		intersect.x = ray->direction.x;
		intersect.y = ray->direction.y;
		d=MMIN(fabs(supercell->bbox.max.z - ray->origin.z),fabs(supercell->bbox.min.z - ray->origin.z));
		upper=1;
	}else{
		D[0] = (supercell->bbox.max.z - ray->origin.z)/ray->direction.z;		
		D[1] = (supercell->bbox.min.z - ray->origin.z)/ray->direction.z;
		if(D[0]<D[1]){
			upper=1;
			d=D[0];
		}else{
			upper=0;
			d=D[1];
		}
		
		switch(upper){
			case 0:
			intersect.x=supercell->bbox.min.x - ray->origin.x)/ray->direction.x;			
			intersect.y=supercell->bbox.min.y - ray->origin.y)/ray->direction.y;
			break;
			case 1:
			intersect.x=supercell->bbox.max.x - ray->origin.x)/ray->direction.x;			
			intersect.y=supercell->bbox.max.y - ray->origin.y)/ray->direction.y;
			break;
		}
	}

	*col_Ptr = intersect.x/supercell->extent.x;
	*row_Ptr = intersect.y/supercell->extent.y;

	return;
}

int	find_next_cell(ray,supercell,col_Ptr,row_Ptr)
Ray		*ray;
SuperCell	*supercell;
int		*row_Ptr,*col_Ptr;
{
	int	coded_ray_to_bbox();

}
