#include "frat.h"
#include "imagelib.h"

int phx=-1,phy=-1,hx=-1,hy=-1,ox=-1,oy=-1;
int copyToHdr();
void	enter_dem(verbose,data,hd_Ptr,dem_filename,type)
char *dem_filename;
struct  header  *hd_Ptr;
Data	*data;
int	verbose,type;
{
        GenericImage *dem;
        dem=allocateImageArray(1,NULL);
        setImageDefaults(dem,FALSE);
        setImageName(dem,dem_filename);
        setImageNameH(dem,getImageNameH(dem));
        readImage(dem,NULL);
        copyToHdr(hd_Ptr,dem);
        if( dem->format == PFFLOAT){
          data->fdata = dem->image;
          data->bdata = NULL;
        }else{
          data->bdata = dem->image;
          data->fdata = NULL;
        }
	if(verbose)switch(type){
		case SPHERICAL_DEM:
		fprintf(stderr,"[SPHERICAL_DEM %s]",dem_filename);
		break;
		case DEM:
		fprintf(stderr,"[DEM %s]",dem_filename);
		break;
	}

/*
**	exit on 0 frames,
**	ignore all but first frame
*/

	if(hd_Ptr->num_frame<1)
		error1("read_dem:\t 0 frames read in DEM file");
	if(hd_Ptr->num_frame>1)
		fprintf(stderr,"Warning:\nread_dem:\t%d frames read in DEM file %s\n\tignoring all but frame 0\n",hd_Ptr->num_frame,dem_filename);
	hd_Ptr->num_frame=1;

	return;
}

void	get_candidate_point(data,candidate_Ptr,i,j,hd_Ptr,dem_attributes_Ptr)
struct  header  *hd_Ptr;
Dem_Attributes	*dem_attributes_Ptr;
triplet		*candidate_Ptr;
int		i,j;
Data	*data;
{
	char	c_candidate=0;
	double	f_candidate=0.0;
	
/*
**	get z-height
*/
	switch(hd_Ptr->pixel_format){
		case PFFLOAT:
		f_candidate = *(data->fdata+hd_Ptr->cols*i+j);
		break;
		case PFBYTE:
		c_candidate = *(data->bdata+hd_Ptr->cols*i+j);
		f_candidate = (double)c_candidate;
		break;
	}
	candidate_Ptr->z = ((f_candidate*dem_attributes_Ptr->extent.z) + dem_attributes_Ptr->offset.z);

/*
**	x,y values (c,r)
*/

	candidate_Ptr->x = (( (double)j * dem_attributes_Ptr->extent.x ) + dem_attributes_Ptr->offset.x);
	candidate_Ptr->y = (( (double)i * dem_attributes_Ptr->extent.y ) + dem_attributes_Ptr->offset.y);

	return;

}


void	get_dem_bbox(verbose,data,hd_Ptr,bbox_max,bbox_min,dem_attributes_Ptr,type)
struct  header  *hd_Ptr;
Dem_Attributes	*dem_attributes_Ptr;
Data	*data;
triplet	*bbox_max,*bbox_min;
int	verbose,type;
{
	int	i,j;
	triplet	candidate,vmin(),vmax();

/*
**	calclate bbox
*/
	
	for(i=0;i<hd_Ptr->rows;i++)
	for(j=0;j<hd_Ptr->cols;j++){
		get_candidate_point(data,&candidate,i,j,hd_Ptr,dem_attributes_Ptr);
		*bbox_min=vmin(*bbox_min,candidate);
		*bbox_max=vmax(*bbox_max,candidate);
	}
	return;
}

int	dem_read(bounds,dem,verbose,local_coords,line,data,hd,bbox_max,bbox_min,dem_attributes_Ptr)
char	 *line;
Dem	*dem;
int	 verbose,local_coords;
triplet	 *bbox_max,*bbox_min;
Dem_Attributes	*dem_attributes_Ptr;
double	*bounds;
Data	*data;
struct header	*hd;
{
	char	*hold,temp[1000],filename[1000];
	void *calloc();
	int	filename_flag;
	double	zexag=1.0;
        triplet _bbox_min,_bbox_max;

        _bbox_min = vector_copy2(BIG,BIG,BIG);
        _bbox_max = vector_copy2(-BIG,-BIG,-BIG);


	dem->repeatMode=SQUARE_REPEAT;
	dem_attributes_Ptr->offset=vector_copy2(0.0,0.0,0.0);
	dem_attributes_Ptr->bbox_min=vector_copy2(0.0,0.0,0.0);
	dem_attributes_Ptr->extent=vector_copy2(1.0,1.0,1.0);
	dem_attributes_Ptr->inv_extent=vector_copy2(1.0,1.0,1.0);
	filename_flag=0;
	if(local_coords){
		if(!(dem->local=(triplet *)calloc(2,sizeof(triplet)))){
			fprintf(stderr,"error in core allocation\n");
			exit(1);
		}
		dem->local[0].x=0.0;
		dem->local[0].y=0.0;
		dem->local[1].x=1.0;
		dem->local[1].y=1.0;
	}else
		dem->local=NULL;
	hold=line;
	while( sscanf(hold,"%s",temp)==1 ){
		hold = strchr(hold,temp[0]) +  (strlen(temp) + 1);
		if(strcmp(temp,"-extent")==0){
			if( sscanf(hold,"%lf %lf",&(dem_attributes_Ptr->extent.x),&(dem_attributes_Ptr->extent.y))!=2)error1("dem_read:\terror in dem extent specification");
		}else if(strcmp(temp,"-triangularRepeat")==0){
			dem->repeatMode=TRIANGULAR_REPEAT;
		}else if(strcmp(temp,"-filename")==0){
			filename_flag=1;
			if( sscanf(hold,"%s",filename)!=1)error1("dem_read:\terror in dem filename specification");
		}else if(strcmp(temp,"-local")==0){
		  if(! dem->local ){
		    fprintf(stderr,"FATAL error: -local flag specified for DEM - require -l flag for prat\n");
		    exit(1);
		  }
			if( sscanf(hold,"%lf %lf %lf %lf",&dem->local[0].x,&dem->local[0].y,&dem->local[1].x,&dem->local[1].y)!=4)error1("dem_read:\terror in dem local coordinate system specification");
		}else if(strcmp(temp,"-origin")==0){
			/* attempt to read 3 values */
			if( sscanf(hold,"%lf %lf %lf",&(dem_attributes_Ptr->offset.x),&(dem_attributes_Ptr->offset.y),&(dem_attributes_Ptr->offset.z))!=3){
				if( sscanf(hold,"%lf %lf",&(dem_attributes_Ptr->offset.x),&(dem_attributes_Ptr->offset.y))!=2)error1("dem_read:\terror in dem origin specification");
				dem_attributes_Ptr->offset.z=0.0;
			}
		}else if(strcmp(temp,"-vertical_exageration")==0 || strcmp(temp,"-v")==0 || strcmp(temp,"-exag")==0){
			if( sscanf(hold,"%lf",&(zexag))!=1)error1("dem_read:\terror in dem vertical_exageration specification");
		}
	}
	if(filename_flag==0)error1("dem_read:\tno filename specified");
/*
**	enter data
*/
	enter_dem(verbose,data,hd,filename,DEM);
	
	dem_attributes_Ptr->extent.x = (dem_attributes_Ptr->extent.x)/(double)(hd->cols - 1);
	dem_attributes_Ptr->extent.y = (dem_attributes_Ptr->extent.y)/(double)(hd->rows - 1);
	dem_attributes_Ptr->extent.z = zexag;
	dem_attributes_Ptr->inv_extent.x=1.0/dem_attributes_Ptr->extent.x;
	dem_attributes_Ptr->inv_extent.y=1.0/dem_attributes_Ptr->extent.y;
	dem_attributes_Ptr->inv_extent.z=1.0/dem_attributes_Ptr->extent.z;
/*
**	bbox
*/
	get_dem_bbox(verbose,data,hd,&_bbox_max,&_bbox_min,dem_attributes_Ptr,DEM);
	
	if(!(dem_attributes_Ptr->direction=(char *)calloc(hd->cols*hd->rows,sizeof(char))))
		error1("dem_read:\terror in core allocation\n");

	bounds[0]=_bbox_max.x;
	bounds[1]=_bbox_max.y;
	bounds[2]=_bbox_max.x;
	bounds[3]=_bbox_min.x;
	bounds[4]=_bbox_min.y;
	bounds[5]=_bbox_min.z;

	dem->objectSize = fabs((bounds[1]-bounds[4])*(bounds[0]-bounds[3]));
/*
**	dem bbox (world coords)
*/
	dem_attributes_Ptr->bbox_max = _bbox_max;
	dem_attributes_Ptr->bbox_min = _bbox_min;

	*bbox_min = _bbox_min;
	*bbox_max = _bbox_max;
	return(1);
}

int	spherical_dem_read(bounds,verbose,line,data,hd,bbox_max,bbox_min,spherical_dem_Ptr,vertices)
char	 *line;
int	 verbose;
triplet	 *bbox_max,*bbox_min;
double	*bounds;
Data	*data;
struct header	*hd;
Spherical_Dem *spherical_dem_Ptr;
VeRtIcEs	*vertices;
{
	char	*hold,temp[1000],filename[1000];
	int	v,block,vertex_in_block;
	double	zexag=1.0;
	triplet	*trip,vertex,r,bbox_max_orthographic,bbox_min_orthographic;
	Dem_Attributes	dem_attributes;
	void	vertex_to_block();

	spherical_dem_Ptr->partial_spherical_data.spherical_cell_bounds_flag=0;
	spherical_dem_Ptr->partial_spherical_data.extent.x=1.0/(2.0*M_PI);
	spherical_dem_Ptr->partial_spherical_data.extent.y=1.0/M_PI;
	spherical_dem_Ptr->partial_spherical_data.offset.x= -M_PI;
	spherical_dem_Ptr->partial_spherical_data.offset.y= -M_PI/2.0;

	bbox_max_orthographic=vector_copy2(-BIG,-BIG,-BIG);
	bbox_min_orthographic=vector_copy2(BIG,BIG,BIG);
	dem_attributes.offset=vector_copy2(0.0,0.0,0.0);
	dem_attributes.extent=vector_copy2(1.0,1.0,1.0);
	
	hold=line;
	if(sscanf(hold,"%d %lf %s",&v,&(spherical_dem_Ptr->base_sphere.radius),filename)!=3)error1("prat_wavefront_read:\terror in sph specification of SPHERICAL_DEM");
	if(spherical_dem_Ptr->base_sphere.radius < 0.0)error1("spherical_dem_read:\t -ve sphere radius specified (must be > 0)");
	spherical_dem_Ptr->base_sphere.r2 = spherical_dem_Ptr->base_sphere.radius*spherical_dem_Ptr->base_sphere.radius;
	spherical_dem_Ptr->objectSize = spherical_dem_Ptr->base_sphere.r2 * M_PI *4.;

/* relative */
	if(v<0)
		v+=vertices->no_of_vertices+1;

	vertex_to_block(&block,&vertex_in_block,v,vertices->blocksize);
	trip=vertices->vertices[block];
	vertex=trip[vertex_in_block];
	spherical_dem_Ptr->base_sphere.centre.x=vertex.x;
	spherical_dem_Ptr->base_sphere.centre.y=vertex.y;
	spherical_dem_Ptr->base_sphere.centre.z=vertex.z;

	while( sscanf(hold,"%s",temp)==1 ){
		hold = strchr(hold,temp[0]) +  (strlen(temp) + 1);
		if(strcmp(temp,"-extent")==0){
			if( sscanf(hold,"%lf %lf",&(spherical_dem_Ptr->partial_spherical_data.extent.x),&(spherical_dem_Ptr->partial_spherical_data.extent.y))!=2)error1("dem_read:\terror in dem extent specification");
			spherical_dem_Ptr->partial_spherical_data.extent.x=1.0/DTOR(spherical_dem_Ptr->partial_spherical_data.extent.x);
			spherical_dem_Ptr->partial_spherical_data.extent.y=1.0/DTOR(spherical_dem_Ptr->partial_spherical_data.extent.y);
		}else if(strcmp(temp,"-origin")==0){
			if( sscanf(hold,"%lf %lf",&(spherical_dem_Ptr->partial_spherical_data.offset.x),&(spherical_dem_Ptr->partial_spherical_data.offset.y))!=2)error1("dem_read:\terror in dem origin specification");
			spherical_dem_Ptr->partial_spherical_data.offset.x=DTOR(spherical_dem_Ptr->partial_spherical_data.offset.x);
			spherical_dem_Ptr->partial_spherical_data.offset.y=DTOR(spherical_dem_Ptr->partial_spherical_data.offset.y);
		}else if(strcmp(temp,"-vertical_exageration")==0 || strcmp(temp,"-v")==0 || strcmp(temp,"-exag")==0){
			if( sscanf(hold,"%lf",&(zexag))!=1)error1("dem_read:\terror in dem vertical_exageration specification");
		}
	}
/*
**	enter spherical dem data
*/
	enter_dem(verbose,data,hd,filename,SPHERICAL_DEM);

	spherical_dem_Ptr->partial_spherical_data.extent.x *= hd->cols;
	spherical_dem_Ptr->partial_spherical_data.extent.y *= hd->rows;
	spherical_dem_Ptr->partial_spherical_data.inv_extent.x = 1.0/spherical_dem_Ptr->partial_spherical_data.extent.x;
	spherical_dem_Ptr->partial_spherical_data.inv_extent.y = 1.0/spherical_dem_Ptr->partial_spherical_data.extent.y;

/*
**	bbox
*/
	get_dem_bbox(verbose,data,hd,&bbox_max_orthographic,&bbox_min_orthographic,&dem_attributes,SPHERICAL_DEM);
	spherical_dem_Ptr->bounding_sphere[0].radius=spherical_dem_Ptr->base_sphere.radius + bbox_min_orthographic.z;
	spherical_dem_Ptr->bounding_sphere[1].radius=spherical_dem_Ptr->base_sphere.radius + bbox_max_orthographic.z;
	spherical_dem_Ptr->bounding_sphere[0].centre=spherical_dem_Ptr->base_sphere.centre;
	spherical_dem_Ptr->bounding_sphere[1].centre=spherical_dem_Ptr->base_sphere.centre;
	spherical_dem_Ptr->bounding_sphere[0].r2=spherical_dem_Ptr->bounding_sphere[0].radius * spherical_dem_Ptr->bounding_sphere[0].radius;
	spherical_dem_Ptr->bounding_sphere[1].r2=spherical_dem_Ptr->bounding_sphere[1].radius * spherical_dem_Ptr->bounding_sphere[1].radius;

	r=vector_copy2(spherical_dem_Ptr->bounding_sphere[1].radius,spherical_dem_Ptr->bounding_sphere[1].radius,spherical_dem_Ptr->bounding_sphere[1].radius);

/* bbox */
	*bbox_min=vector_minus(vertex,r);
	*bbox_max=vector_plus(vertex,r);
	
	bounds[0]=bbox_max->x;
	bounds[1]=bbox_max->y;
	bounds[2]=bbox_max->x;
	bounds[3]=bbox_min->x;
	bounds[4]=bbox_min->y;
	bounds[5]=bbox_min->z;

	return(1);
}

/*
**	all done as double
*/

int	intersect_bbox_contents(tnear,tfar,normal_Ptr,hit_point,ray_length_Ptr,ray_Ptr,grid_Ptr,ggrid_Ptr,dem_Ptr,type_of_cell,uv)
double	*ray_length_Ptr,*tnear,*tfar;
Ray	*ray_Ptr;
ipair	*grid_Ptr;
Pair	*ggrid_Ptr;
Dem	*dem_Ptr;
int	type_of_cell;
triplet	*hit_point;
triplet	*normal_Ptr;
double	*uv;
{
	triplet	T1[3],T2[3];
	fFacet *facet;
	double	localCoords[2][4],bounds[12],scale;
	void	precompute_facet_features(),double_precompute_facet_features();
	int	double_ray_to_plane(),double_point_in_triangle();
	static Vertex_locals VextexLocal;

  if(grid_Ptr->x==ox && grid_Ptr->y==oy){
    /* apparently same square as last time */
    ox=-1;oy=-1;
    /*return(0);*/
  }
	facet=&(dem_Ptr->facet);
        /* dont check if out of bounds */
        if (grid_Ptr->y >= dem_Ptr->hd.rows -1 || grid_Ptr->x >= dem_Ptr->hd.cols -1) return(0);
	switch(dem_Ptr->hd.pixel_format){
		case PFBYTE:
		T1[0]=vector_copy2((ggrid_Ptr->x),(ggrid_Ptr->y),( dem_Ptr->dem_attributes.extent.z * (double) *(dem_Ptr->data.bdata + (dem_Ptr->hd.cols*grid_Ptr->y) + grid_Ptr->x)+dem_Ptr->dem_attributes.offset.z));
		T1[1]=vector_copy2((ggrid_Ptr->x),(ggrid_Ptr->y+dem_Ptr->dem_attributes.extent.y),( dem_Ptr->dem_attributes.extent.z * (double) *(dem_Ptr->data.bdata + (dem_Ptr->hd.cols*(grid_Ptr->y+1)) + grid_Ptr->x)+dem_Ptr->dem_attributes.offset.z));
		T2[0]=vector_copy2((ggrid_Ptr->x+dem_Ptr->dem_attributes.extent.x),(ggrid_Ptr->y),( dem_Ptr->dem_attributes.extent.z * (double) *(dem_Ptr->data.bdata + (dem_Ptr->hd.cols*grid_Ptr->y) + (grid_Ptr->x+1))+dem_Ptr->dem_attributes.offset.z));
		T2[1]=vector_copy2((ggrid_Ptr->x+dem_Ptr->dem_attributes.extent.x),(ggrid_Ptr->y+dem_Ptr->dem_attributes.extent.y),( dem_Ptr->dem_attributes.extent.z * (double) *(dem_Ptr->data.bdata + (dem_Ptr->hd.cols*(grid_Ptr->y+1)) + (grid_Ptr->x+1))+dem_Ptr->dem_attributes.offset.z));
		break;
		case PFFLOAT:
		T1[0]=vector_copy2((ggrid_Ptr->x),(ggrid_Ptr->y),( dem_Ptr->dem_attributes.extent.z * *(dem_Ptr->data.fdata + (dem_Ptr->hd.cols*grid_Ptr->y) + grid_Ptr->x)+dem_Ptr->dem_attributes.offset.z));
		T1[1]=vector_copy2((ggrid_Ptr->x),(ggrid_Ptr->y+dem_Ptr->dem_attributes.extent.y),( dem_Ptr->dem_attributes.extent.z * *(dem_Ptr->data.fdata + (dem_Ptr->hd.cols*(grid_Ptr->y+1)) + grid_Ptr->x)+dem_Ptr->dem_attributes.offset.z));
		T2[0]=vector_copy2((ggrid_Ptr->x+dem_Ptr->dem_attributes.extent.x),(ggrid_Ptr->y),( dem_Ptr->dem_attributes.extent.z * *(dem_Ptr->data.fdata + (dem_Ptr->hd.cols*grid_Ptr->y) + (grid_Ptr->x+1))+dem_Ptr->dem_attributes.offset.z));
		T2[1]=vector_copy2((ggrid_Ptr->x+dem_Ptr->dem_attributes.extent.x),(ggrid_Ptr->y+dem_Ptr->dem_attributes.extent.y),( dem_Ptr->dem_attributes.extent.z * *(dem_Ptr->data.fdata + (dem_Ptr->hd.cols*(grid_Ptr->y+1)) + (grid_Ptr->x+1))+dem_Ptr->dem_attributes.offset.z));
		break;
	}
	if(dem_Ptr->local){
		localCoords[0][0]=(dem_Ptr->local[0].x)+(dem_Ptr->local[1].x - dem_Ptr->local[0].x)*grid_Ptr->x/(double)dem_Ptr->hd.cols;
		localCoords[1][0]=(dem_Ptr->local[0].y)+(dem_Ptr->local[1].y - dem_Ptr->local[0].y)*grid_Ptr->y/(double)dem_Ptr->hd.rows;
		localCoords[0][1]=(dem_Ptr->local[0].x)+(dem_Ptr->local[1].x - dem_Ptr->local[0].x)*grid_Ptr->x/(double)dem_Ptr->hd.cols;
		localCoords[1][1]=(dem_Ptr->local[0].y)+(dem_Ptr->local[1].y - dem_Ptr->local[0].y)*(grid_Ptr->y+1)/(double)dem_Ptr->hd.rows;
		localCoords[0][2]=(dem_Ptr->local[0].x)+(dem_Ptr->local[1].x - dem_Ptr->local[0].x)*(grid_Ptr->x+1)/(double)dem_Ptr->hd.cols;
		localCoords[1][2]=(dem_Ptr->local[0].y)+(dem_Ptr->local[1].y - dem_Ptr->local[0].y)*grid_Ptr->y/(double)dem_Ptr->hd.rows;
		localCoords[0][3]=(dem_Ptr->local[0].x)+(dem_Ptr->local[1].x - dem_Ptr->local[0].x)*(grid_Ptr->x+1)/(double)dem_Ptr->hd.cols;
		localCoords[1][3]=(dem_Ptr->local[0].y)+(dem_Ptr->local[1].y - dem_Ptr->local[0].y)*(grid_Ptr->y+1)/(double)dem_Ptr->hd.rows;

		facet->local_coords=&VextexLocal;
		facet->local_coords->local_du.x=localCoords[0][1]-localCoords[0][0];
		facet->local_coords->local_du.y=localCoords[1][1]-localCoords[1][0];
		facet->local_coords->vertex_local.x=localCoords[0][0];
		facet->local_coords->vertex_local.y=localCoords[1][0];
	}else{
		facet->local_coords=NULL;
	}
	switch(type_of_cell){
		case 0:
		T1[2]=T2[1];
		T2[2]=T1[0];
		if(dem_Ptr->local){
			facet->local_coords->local_dv.x=localCoords[0][3]-localCoords[0][0];
			facet->local_coords->local_dv.y=localCoords[1][3]-localCoords[1][0];
		}
		break;
		case 1:
		T1[2]=T2[0];
		T2[2]=T1[1];
		if(dem_Ptr->local){
			facet->local_coords->local_dv.x=localCoords[0][2]-localCoords[0][0];
			facet->local_coords->local_dv.y=localCoords[1][2]-localCoords[1][0];
		}
		break;
	}

	double_precompute_facet_features(&scale,bounds,T1,facet,TRUE);
	if(double_ray_to_plane(ray_length_Ptr,&(facet->dw),&(facet->normal),ray_Ptr)){int i;i=0;}
	if(*ray_length_Ptr > 0.0 && *ray_length_Ptr + RAY_TOL >= *tnear && *ray_length_Ptr - RAY_TOL <= *tfar)
	if(*ray_length_Ptr >= facet->size*0.0001 && double_point_in_triangle(&scale,hit_point,facet,ray_Ptr,*ray_length_Ptr,uv) ){
		normal_Ptr->x = facet->normal.x;
		normal_Ptr->y = facet->normal.y;
		normal_Ptr->z = facet->normal.z;
		phx=grid_Ptr->x;
		phy=grid_Ptr->y;
		if(dem_Ptr->local){
		  uv[0] = dem_Ptr->local[0].x + (dem_Ptr->local[1].x - dem_Ptr->local[0].x)*(hit_point->x-dem_Ptr->dem_attributes.offset.x)/(dem_Ptr->dem_attributes.extent.x * (dem_Ptr->hd.cols-1));
		  uv[1] = dem_Ptr->local[0].y + (dem_Ptr->local[1].y - dem_Ptr->local[0].y)*(hit_point->y-dem_Ptr->dem_attributes.offset.y)/(dem_Ptr->dem_attributes.extent.y* (dem_Ptr->hd.rows-1));
		}
		return(1);
	}
	if(dem_Ptr->local){

		facet->local_coords->local_du.x=localCoords[0][3]-localCoords[0][2];
		facet->local_coords->local_du.y=localCoords[1][3]-localCoords[1][2];
		facet->local_coords->vertex_local.x=localCoords[0][0];
		facet->local_coords->vertex_local.y=localCoords[1][0];
		switch(type_of_cell){
			case 0:
			facet->local_coords->local_dv.x=localCoords[0][0]-localCoords[0][0];
			facet->local_coords->local_dv.y=localCoords[1][0]-localCoords[1][0];
			break;
			case 1:
			facet->local_coords->local_dv.x=localCoords[0][1]-localCoords[0][0];
			facet->local_coords->local_dv.y=localCoords[1][1]-localCoords[1][0];
			break;
		}
	}
	double_precompute_facet_features(&scale,bounds,T2,facet,TRUE);
	if(double_ray_to_plane(ray_length_Ptr,&(facet->dw),&(facet->normal),ray_Ptr)){int i;i=0;}
	if(*ray_length_Ptr > 0.0 && *ray_length_Ptr + RAY_TOL >= *tnear && *ray_length_Ptr - RAY_TOL <= *tfar)
	if(*ray_length_Ptr >= facet->size*0.0001 && double_point_in_triangle(&scale,hit_point,facet,ray_Ptr,*ray_length_Ptr,uv) ){
		normal_Ptr->x = facet->normal.x;
		normal_Ptr->y = facet->normal.y;
		normal_Ptr->z = facet->normal.z;
		phx=grid_Ptr->x;
		phy=grid_Ptr->y;
		if(dem_Ptr->local){
		  uv[0] = dem_Ptr->local[0].x + (dem_Ptr->local[1].x - dem_Ptr->local[0].x)*(hit_point->x-dem_Ptr->dem_attributes.offset.x)/(dem_Ptr->dem_attributes.extent.x* (dem_Ptr->hd.cols-1));
		  uv[1] = dem_Ptr->local[0].y + (dem_Ptr->local[1].y - dem_Ptr->local[0].y)*(hit_point->y-dem_Ptr->dem_attributes.offset.y)/(dem_Ptr->dem_attributes.extent.y* (dem_Ptr->hd.rows-1));
		}
		return(1);
	}
	return(0);
}

int	intersect_spherical_dem_cell_contents(ray_length_Ptr,normal_Ptr,ray_Ptr,hit_point,d)
double	*ray_length_Ptr;
Ray	*ray_Ptr;
triplet	*normal_Ptr;
triplet	*hit_point;
triplet	*d;
{
	triplet	T1[3],T2[3],du,dv;
/*	MATRIX3	M,M_1,matrix3_ip(),matrix3_inverse();*/
	fFacet	facet;
	double	uv[2],scale;double Random();
	int	setup_simple_facet();
	int	double_ray_to_plane(),double_point_in_triangle(); 
	unsigned int facet_type;

	facet_type=(unsigned int)(Random()+0.5);

	T1[0].x=d[0].x;
	T1[0].y=d[0].y;
	T1[0].z=d[0].z;
	T1[1].x=d[1].x;
	T1[1].y=d[1].y;
	T1[1].z=d[1].z;
	T2[0].x=d[2].x;
	T2[0].y=d[2].y;
	T2[0].z=d[2].z;
	T2[1].x=d[3].x;
	T2[1].y=d[3].y;
	T2[1].z=d[3].z;

	switch(facet_type){
		case 0:
		T1[2]=T2[1];
		T2[2]=T1[1];
		break;
		case 1: default:
		T1[2]=T2[0];
		T2[2]=T1[0];
		break;
	}
	du=vector_minus(T1[1],T1[0]);
	dv=vector_minus(T1[2],T1[0]);
	facet.normal=normalise(vector_cross(du,dv));
/*	normal=normalise(vector_cross(du,dv));
	direction.x=ray_Ptr->direction.x;
	direction.y=ray_Ptr->direction.y;
	direction.z=ray_Ptr->direction.z;
	
	M = matrix3_ip((direction),du,dv);
	M_1 = matrix3_inverse(M);

	OB=vector_minus(ray_Ptr->origin,T1[0]);
	m = matrix3_mult(M_1,OB);
	if(m.y>=0 && m.z>=0 && (m.y+m.z)<=1){
		*ray_length_Ptr = -(m.x);
		normal_Ptr->x = normal.x;
		normal_Ptr->y = normal.y;
		normal_Ptr->z = normal.z;
		return(1);
	}
*/	

	setup_simple_facet(&scale,&facet,&du,&dv,&(T1[0]));
	if(double_ray_to_plane(ray_length_Ptr,&(facet.dw),&(facet.normal),ray_Ptr))
	if(double_point_in_triangle(&scale,hit_point,&facet,ray_Ptr,*ray_length_Ptr,uv)){
		*normal_Ptr = facet.normal;
		return(1);
	}

	du=vector_minus(T2[1],T2[0]);
	dv=vector_minus(T2[2],T2[0]);
	facet.normal=normalise(vector_cross(du,dv));
/*	normal=normalise(vector_cross(du,dv));
	
	M = matrix3_ip((direction),du,dv);
	M_1 = matrix3_inverse(M);

	OB=vector_minus(ray_Ptr->origin,T2[0]);
	m = matrix3_mult(M_1,OB);
	if(m.y>=0 && m.z>=0 && (m.y+m.z)<=1){
		*ray_length_Ptr = -m.x;
		normal_Ptr->x = normal.x;
		normal_Ptr->y = normal.y;
		normal_Ptr->z = normal.z;
		return(1);
	}
*/
	setup_simple_facet(&scale,&facet,&du,&dv,&(T2[0]));
	if(double_ray_to_plane(ray_length_Ptr,&(facet.dw),&(facet.normal),ray_Ptr))
	if(double_point_in_triangle(&scale,hit_point,&facet,ray_Ptr,*ray_length_Ptr,uv)){
		*normal_Ptr = facet.normal;
		return(1);
	}

	return(0);
}

int	is_even(number)
int	number;
{
	int	i;
	i=number/2;
	if(2*i == number)return(1);
	return(0);
}

/*
** assumes alternate pattern to tesselation
*/
int	type_of_cell(grid_Ptr,cols,direction)
ipair	*grid_Ptr;
int	cols;
char	*direction;
{
	int	prev=0;
	double 	Random();

	if(direction){
		prev= *(direction + grid_Ptr->y + (grid_Ptr->x)*cols);
	}
	if(prev==0){ /* set random direction for cell */
		prev=(char)(Random()+0.5) + 1;
		*(direction + grid_Ptr->y + (grid_Ptr->x)*cols)=prev;
	}

/* regular tesselation version (old version */
/*	a=is_even(grid_Ptr->x);
	b=is_even(grid_Ptr->y);
	if(a==b)return(1);
	return(0);
*/
	return(prev-1);
}

void	global2local_ray(global_ray_Ptr,local_ray_Ptr,dem_Ptr)
Dem	*dem_Ptr;
Ray	*global_ray_Ptr,*local_ray_Ptr;
{
	local_ray_Ptr->origin=multiply_vector(vector_minus(global_ray_Ptr->origin,dem_Ptr->dem_attributes.offset),dem_Ptr->dem_attributes.inv_extent);
	local_ray_Ptr->direction=normalise(multiply_vector(global_ray_Ptr->direction,dem_Ptr->dem_attributes.inv_extent));
	return;
}

pair	global2local_2D(coord,dem_Ptr)
Dem	*dem_Ptr;
pair	coord;
{
	pair	out;
	out.x = (coord.x - dem_Ptr->dem_attributes.offset.x)*dem_Ptr->dem_attributes.inv_extent.x;
	out.y = (coord.y - dem_Ptr->dem_attributes.offset.y)*dem_Ptr->dem_attributes.inv_extent.y;
	return(out);
}

pair	local2global_2D(coord,dem_Ptr)
Dem	*dem_Ptr;
ipair	coord;
{
	pair	out;
	out.x = ((double)coord.x *dem_Ptr->dem_attributes.extent.x) + dem_Ptr->dem_attributes.offset.x;
	out.y = ((double)coord.y *dem_Ptr->dem_attributes.extent.y) + dem_Ptr->dem_attributes.offset.y;
	return(out);
}

Pair	double_global2local_2D(coord,dem_Ptr)
Dem	*dem_Ptr;
Pair	coord;
{
	Pair	out;
	out.x = (coord.x - dem_Ptr->dem_attributes.offset.x)*dem_Ptr->dem_attributes.inv_extent.x;
	out.y = (coord.y - dem_Ptr->dem_attributes.offset.y)*dem_Ptr->dem_attributes.inv_extent.y;
	return(out);
}

Pair	double_local2global_2D(coord,dem_Ptr)
Dem	*dem_Ptr;
ipair	coord;
{
	Pair	out;
	out.x = ((double)coord.x *dem_Ptr->dem_attributes.extent.x) + dem_Ptr->dem_attributes.offset.x;
	out.y = ((double)coord.y *dem_Ptr->dem_attributes.extent.y) + dem_Ptr->dem_attributes.offset.y;
	return(out);
}

int	ray_within_local_bbox(tnear_Ptr,tfar_Ptr,ray_Ptr,grid_Ptr,ggrid_Ptr,dem_Ptr,ray_direction_code)
double	*tfar_Ptr,*tnear_Ptr;
Ray	*ray_Ptr;
ipair	*grid_Ptr;
Pair	*ggrid_Ptr;
Dem	*dem_Ptr;
int	*ray_direction_code;
{
	struct Bbox bbox;
	int	isHit,double_ray_to_bbox();
	triplet	hit;
	
	bbox.min.x	=ggrid_Ptr->x;
	bbox.min.y	=ggrid_Ptr->y;
	bbox.max.x	=(ggrid_Ptr->x +dem_Ptr->dem_attributes.extent.x);
	bbox.max.y	=(ggrid_Ptr->y +dem_Ptr->dem_attributes.extent.y);
	bbox.max.z	=dem_Ptr->dem_attributes.bbox_max.z;
	bbox.min.z	=dem_Ptr->dem_attributes.bbox_min.z;

        isHit = double_ray_to_bbox(tnear_Ptr,tfar_Ptr,&bbox,ray_Ptr);	
	if(isHit == 0)
		return(0);
/*
**	calculate next cell to hit (based on *tfar_Ptr)
*/
	hit=vector_plus(V_factor(ray_Ptr->direction,*tfar_Ptr),ray_Ptr->origin);
/* exit via z-plane -> end here */
	if( hit.z > bbox.max.z || hit.z < bbox.min.z)return(-1);
	return(1);
}

int	dem_intersect(bb,normal_Ptr,dem_Ptr,global_ray_Ptr,ray_length_Ptr,uv)
     BigBag *bb;
Dem		*dem_Ptr;
Ray		*global_ray_Ptr;
double		*ray_length_Ptr;
triplet		*normal_Ptr;
double		*uv;
{
  int     isHit;
  int	starty,ray_direction_code[2],intersect=0,double_ray_to_bbox();
  double	D,Dmax,lambda,norm,cx,cy,dx,dy,ox,oy,tnear,tfar,ray_length;
  ipair	igrid,iout_grid,grid,out_grid;
  Pair	p_near,p_far,ggrid,gout_grid,double_global2local_2D(),double_local2global_2D();
  Ray	ray;
  struct Bbox	bbox;
  triplet	hit_point;
  FlagBag *flagbag;

  flagbag=bb->flagbag;

/* DEM direction memory */
/* convert ray to local coords */
  ray.origin.x    = global_ray_Ptr->origin.x;
  ray.origin.y    = global_ray_Ptr->origin.y;
  ray.origin.z    = global_ray_Ptr->origin.z;
  ray.direction.x = global_ray_Ptr->direction.x;
  ray.direction.y = global_ray_Ptr->direction.y;
  ray.direction.z = global_ray_Ptr->direction.z;
  ray.ray_length  = global_ray_Ptr->ray_length;

  if(ray.direction.x > 0.0)
    ray_direction_code[0]=1;	/* x +ve */
  else
    ray_direction_code[0]=0;	/* x -ve */
  if(ray.direction.y > 0.0)
    ray_direction_code[1]=1;	/* y +ve */
  else
    ray_direction_code[1]=0;	/* y -ve */

/*	intersect dem bbox */

  bbox.min=dem_Ptr->dem_attributes.bbox_min;
  bbox.max=dem_Ptr->dem_attributes.bbox_max;
  if(!(double_ray_to_bbox(&tnear,&tfar,&bbox,&ray)))
    return(0);

/* set up local grid cell */

  p_near.x=ray.origin.x + (ray.direction.x * (tnear + flagbag->ray_length_tol));
  p_near.y=ray.origin.y + (ray.direction.y * (tnear + flagbag->ray_length_tol));
  p_far.x=ray.origin.x + (ray.direction.x * (tfar - flagbag->ray_length_tol));
  p_far.y=ray.origin.y + (ray.direction.y * (tfar - flagbag->ray_length_tol));
  ggrid = double_global2local_2D(p_near,dem_Ptr);
  gout_grid = double_global2local_2D(p_far,dem_Ptr);
  grid.x = (int)ggrid.x;
  grid.y = (int)ggrid.y;
  out_grid.x = (int)gout_grid.x;
  out_grid.y = (int)gout_grid.y;
  p_far=p_near;

  ggrid = double_local2global_2D(grid,dem_Ptr);	
  if(grid.x<0 || grid.y <0 || grid.x >= dem_Ptr->hd.cols -1 || grid.y >= dem_Ptr->hd.rows -1)
    return(0);

  Dmax = (dem_Ptr->dem_attributes.extent.x*dem_Ptr->dem_attributes.extent.x + dem_Ptr->dem_attributes.extent.y*dem_Ptr->dem_attributes.extent.y); 
  ox = dem_Ptr->dem_attributes.extent.x * 0.5 - ray.origin.x;
  oy = dem_Ptr->dem_attributes.extent.y * 0.5 - ray.origin.y;
  dx = ray.direction.x;
  dy = ray.direction.y;
  norm = sqrt(dx*dx + dy*dy); 
  if(norm>0.){
    dx /= norm;
    dy /= norm;
  }
  out_grid.x=MAX(0,MIN(out_grid.x,dem_Ptr->hd.cols -1));
  out_grid.y=MAX(0,MIN(out_grid.y,dem_Ptr->hd.rows -1));

  igrid.x=MIN(grid.x,out_grid.x);
  igrid.y=starty=MIN(grid.y,out_grid.y);
  iout_grid.x=MAX(grid.x,out_grid.x);
  iout_grid.y=MAX(grid.y,out_grid.y);  
  if(iout_grid.x>=dem_Ptr->hd.cols-2)iout_grid.x=dem_Ptr->hd.cols-1;
  if(iout_grid.y>=dem_Ptr->hd.rows-2)iout_grid.y=dem_Ptr->hd.rows-1;
  for(;igrid.x<=iout_grid.x;igrid.x++){
    for(igrid.y=starty;igrid.y<=iout_grid.y;igrid.y++){
      ggrid = double_local2global_2D(igrid,dem_Ptr);
      cx = ggrid.x + ox;
      cy = ggrid.y + oy;
      lambda = cx*dx + cy*dy;
      if((D = cx*cx + cy*cy - lambda*lambda) <= Dmax){
        isHit = ray_within_local_bbox(&tnear,&tfar,&ray,&igrid,&ggrid,dem_Ptr,ray_direction_code);
        if(isHit != 0){
          ray_length= *ray_length_Ptr;
          if(intersect_bbox_contents(&tnear,&tfar,normal_Ptr,&hit_point,&ray_length,&ray,&igrid,&ggrid,dem_Ptr,type_of_cell(&igrid,dem_Ptr->hd.cols,dem_Ptr->dem_attributes.direction),uv)){
            *ray_length_Ptr = ray_length;
            intersect=1;
          }
        }
      }
    }   
  }
  return(intersect);
}

void	setup_grid(grid,ggrid,dem_Ptr)
ipair	*grid;
Pair	ggrid;
Spherical_Dem	*dem_Ptr;
{
	int	i;

	grid[0].x=(int)ggrid.x;
	grid[0].y=(int)ggrid.y;

	grid[1].x=grid[0].x+1;
	grid[1].y=grid[0].y;
	grid[2].x=grid[0].x+1;
	grid[2].y=grid[0].y+1;
	grid[3].x=grid[0].x;
	grid[3].y=grid[0].y+1;

	for(i=0;i<4;i++){
		while(grid[i].x>dem_Ptr->hd.cols)grid[i].x -= dem_Ptr->hd.cols;
		while(grid[i].y>dem_Ptr->hd.rows)grid[i].y -= dem_Ptr->hd.rows;
		while(grid[i].x<0.0             )grid[i].x += dem_Ptr->hd.cols;
		while(grid[i].y<0.0             )grid[i].y += dem_Ptr->hd.rows;
	}
	
}

double	get_grid_value(grid,data,hd)
ipair	*grid;
Data	*data;
struct header *hd;
{
	double	f_candidate=0.0;
	char	c_candidate=0;

	switch(hd->pixel_format){
		case PFFLOAT:
		f_candidate = *(data->fdata+hd->cols*(grid->y%hd->rows)+(grid->x%hd->cols));
		break;
		case PFBYTE:
		c_candidate = *(data->bdata+hd->cols*grid->y+grid->x);
		f_candidate = (double)c_candidate;
		break;
	}
	return(f_candidate);
}

int	setup_simple_facet(scale_Ptr,facet_Ptr,du,dv,base)
fFacet	*facet_Ptr;triplet	*du,*dv,*base;double	*scale_Ptr;
{
	Pair Du,Dv;
	double	tmp;

	if(!(du->x==0.0 || dv->x == 0.0)){		/* dx unuseable */
		facet_Ptr->orientation=0;	
		facet_Ptr->base.x=base->y;
		facet_Ptr->base.y=base->z;
		Du.x=du->y;
		Du.y=du->z;
		Dv.x=dv->y;
		Dv.y=dv->z;
	}else if(!(du->z==0.0 || dv->z == 0.0)){	/* dz unuseable */
		facet_Ptr->orientation=2;	
		facet_Ptr->base.x=base->x;
		facet_Ptr->base.y=base->y;
		Du.x=du->x;
		Du.y=du->y;
		Dv.x=dv->x;
		Dv.y=dv->y;
	}else  if(!(du->y==0.0 || dv->y == 0.0)){	/* dy unuseable */
		facet_Ptr->orientation=1;	/* y - max */
		facet_Ptr->base.x=base->z;
		facet_Ptr->base.y=base->x;
		Du.x=du->z;
		Du.y=du->x;
		Dv.x=dv->z;
		Dv.y=dv->x;
	}else{
		facet_Ptr->orientation=2;	
		facet_Ptr->base.x=base->x;
		facet_Ptr->base.y=base->y;
		Du.x=du->x;
		Du.y=du->y;
		Dv.x=dv->x;
		Dv.y=dv->y;
	}

	tmp=(Du.x * Dv.y - Dv.x * Du.y);
	while(tmp==0.0){
		switch(facet_Ptr->orientation){
			case 0:
			facet_Ptr->orientation=1;	
			facet_Ptr->base.x=base->z;
			facet_Ptr->base.y=base->x;
			Du.x=du->z;
			Du.y=du->x;
			Dv.x=dv->z;
			Dv.y=dv->x;
			break;
			case 1:
			facet_Ptr->orientation=2;	
			facet_Ptr->base.x=base->x;
			facet_Ptr->base.y=base->y;
			Du.x=du->x;
			Du.y=du->y;
			Dv.x=dv->x;
			Dv.y=dv->y;
			break;
			case 2:
			facet_Ptr->orientation=0;	
			facet_Ptr->base.x=base->y;
			facet_Ptr->base.y=base->z;
			Du.x=du->y;
			Du.y=du->z;
			Dv.x=dv->y;
			Dv.y=dv->z;
			break;
		}

		tmp=(Du.x * Dv.y - Dv.x * Du.y);

	}
	if(tmp==0.0)	
		error1("precompute_facet_features\t:error setting up facet");
	*scale_Ptr = 1.0/tmp;

	facet_Ptr->store[0]=(Dv.y);
	facet_Ptr->store[1]=(-Dv.x);
	facet_Ptr->store[2]=(-Du.y);
	facet_Ptr->store[3]=(Du.x);
/*
**	distance from origin to plane
*/
	facet_Ptr->dw= -v_dot(*base,facet_Ptr->normal);

	return(1);
}

int	spherical_cell_intersect(grid,dem_Ptr,ray,normal_Ptr,tmin_Ptr,next_direction_Ptr)
ipair	*grid;
Spherical_Dem	*dem_Ptr;
Ray	*ray;
triplet	*normal_Ptr;
double	*tmin_Ptr;
int	*next_direction_Ptr;
{
	double	zmin,zmax,z_value[4],tmax,D,uv[2],scale;
	Pair	ggrid;
	triplet	d[4],d_world[4],du,dv,t_hit_point;
	int	test_contents,i,j,double_ray_to_plane(),double_point_in_triangle();
	fFacet	facet[4];

	zmin=BIG;
	zmax= -BIG;
	for(i=0;i<4;i++){
		z_value[i]= dem_Ptr->base_sphere.radius + get_grid_value(&(grid[i]),&(dem_Ptr->data),&(dem_Ptr->hd));
		ggrid.x = (grid[i].x / dem_Ptr->partial_spherical_data.extent.x)+dem_Ptr->partial_spherical_data.offset.x;
		ggrid.y = (grid[i].y / dem_Ptr->partial_spherical_data.extent.y)+dem_Ptr->partial_spherical_data.offset.y;
		d[i]=spherical_to_cartesian(vector_copy2(z_value[i],ggrid.x,ggrid.y),1);
		d_world[i]=spherical_to_cartesian(vector_copy2(dem_Ptr->bounding_sphere[1].radius,ggrid.x,ggrid.y),1);
	}
	*tmin_Ptr=BIG;
	tmax= -BIG;
	*next_direction_Ptr= -1;
	test_contents=0;
	for(i=0;i<4;i++){
		du=d_world[i];
		if(i==3)j=0;else j=i+1;
		dv=d_world[j];
		facet[i].normal=normalise(Vector_cross(du,dv));
		setup_simple_facet(&scale,&facet[i],&du,&dv,&(dem_Ptr->base_sphere.centre));
		if(double_ray_to_plane(&D,&(facet[i].dw),&(facet[i].normal),&(ray->origin),&(ray->direction)) && D < ray->ray_length){
			if(double_point_in_triangle(&scale,&t_hit_point,&(facet[i]),ray,D,&(uv[0]))){
				*tmin_Ptr=MMIN(*tmin_Ptr,D);
				tmax=MMAX(tmax,D);
				if(tmax==D)*next_direction_Ptr=i;
				/* test contents */
				test_contents=1;
			}
		}
	}

	return(0);
}

int getIntersectionLimits(hit_points,ray,dem_Ptr)
Ray *ray;
Spherical_Dem *dem_Ptr;
triplet *hit_points;
{
	int	ray_in_sphere_data();
	double	tnear,tfar,tvnear,tvfar;
/*	intersect outer bounding spheres */

	if(!(ray_in_sphere_data(&tnear,&tfar,&(dem_Ptr->bounding_sphere[1]),&(ray->origin),&(ray->direction))))return(0);

/* check intersection with inner sphere */
	if(ray_in_sphere_data(&tvnear,&tvfar,&(dem_Ptr->bounding_sphere[0]),&(ray->origin),&(ray->direction)))tfar=MMIN(tvnear,tfar);


/* find hit point on outer sphere (in coords local to sphere centre */

	hit_points[0]=vector_minus(vector_plus(V_factor(ray->direction,tnear),ray->origin),dem_Ptr->bounding_sphere[1].centre);

/* find hit point on 2nd hit (in coords local to sphere centre */

	hit_points[1]=vector_minus(vector_plus(V_factor(ray->direction,tfar),ray->origin),dem_Ptr->bounding_sphere[1].centre);

	return(1);
}

void    getRowColNumbers(row,col,intersectionPoints,ray,demPtr)
int             *row;
int             *col;
triplet         *intersectionPoints;
Ray             *ray;
Spherical_Dem    *demPtr;
{
        triplet sph[2];
        double   *theta1,*theta2,*phi1,*phi2,frow1,frow2,fcol1,fcol2;
/*
**      convert intersection points to polar coords from Cartesian
*/
        sph[0]=cartesian_to_spherical(intersectionPoints[0]);
        sph[1]=cartesian_to_spherical(intersectionPoints[1]);

        theta1=&(sph[0].z);        /* theta */
        theta2=&(sph[1].z);        /* theta */

        phi1=&(sph[0].y);        /* phi */
        phi2=&(sph[1].y);        /* phi */

        frow1=(M_PI_2 - *theta1)*(demPtr->hd.rows-1)/(M_PI);
        frow2=(M_PI_2 - *theta2)*(demPtr->hd.rows-1)/(M_PI);

        row[0]=(int)frow1;
        row[1]=(int)frow2;

        fcol1=(*phi1 - M_PI - M_PI_2)*(demPtr->hd.cols-1)/(2.0*M_PI);
        fcol2=(*phi2 - M_PI - M_PI_2)*(demPtr->hd.cols-1)/(2.0*M_PI);

        col[0]=(int)fcol1;
        col[1]=(int)fcol2;

        return;
}

void	grid2Cartesian(d,r,c,dem_Ptr)
triplet	*d;
int	r,c;
Spherical_Dem    *dem_Ptr;
{
	double	h,R,D,theta,phi;
	int	Cc,Rr;

	Cc=c%dem_Ptr->hd.cols;while(Cc<0)Cc+=dem_Ptr->hd.cols;
	Rr=r%dem_Ptr->hd.rows;while(Rr<0)Rr+=dem_Ptr->hd.rows;

	h = *(dem_Ptr->data.fdata + Cc + Rr*dem_Ptr->hd.cols);
	R = h + dem_Ptr->base_sphere.radius;

	theta = M_PI_2 - (r*M_PI)/(dem_Ptr->hd.rows-1);
	phi = M_PI + M_PI_2 + (c*2*M_PI)/(dem_Ptr->hd.cols-1);

	D = R * cos(theta);

	d->x= (dem_Ptr->base_sphere.centre.x) + D*cos(phi);
	d->y= (dem_Ptr->base_sphere.centre.y) + D*sin(phi);
	d->z= (dem_Ptr->base_sphere.centre.z) + R*sin(theta);
}

void	getCornerValues(d,r,c,movementR,movementC,dem_Ptr)
triplet	*d;
int	r,c,movementR,movementC;
Spherical_Dem    *dem_Ptr;
{
	grid2Cartesian(&d[0],r,c,dem_Ptr);
	grid2Cartesian(&d[1],r,c+movementC,dem_Ptr);
	grid2Cartesian(&d[2],r+movementR,c+movementC,dem_Ptr);
	grid2Cartesian(&d[3],r+movementR,c,dem_Ptr);

	return;
}

int	spherical_dem_intersect(bb,normal_Ptr,dem_Ptr,ray)
Spherical_Dem	*dem_Ptr;
Ray		*ray;
triplet		*normal_Ptr;
BigBag *bb;
{
	int	r,c,row[2],col[2],ray_in_sphere_data();
	double	ray_length;
	triplet	hit_point,hit_points[2],d[4],normal;
        int     movementR,movementC,nRows,nCols,countR,countC;
        int     hit=0;

/*
**      find first and second intersections with bounding spheres
**      (hit_points[0],hit_points[1], respectively)
*/

	if(!getIntersectionLimits(hit_points,ray,dem_Ptr))return(0);

/*
**      convert intersection info. to first/last rows
**      for intersection tests
*/
	getRowColNumbers(&(row[0]),&(col[0]),&(hit_points[0]),ray,dem_Ptr);

	if(row[1]>row[0])movementR=1;else movementR= -1;
        nRows=abs(row[1]-row[0])+1;

	if(col[1]>col[0])movementC=1;else movementC= -1;
        nCols=abs(col[1]-col[0])+1;

        for(r=row[0],countR=0;countR<nRows;r+=movementR,countR++){
                for(c=col[0],countC=0;countC<nCols;c+=movementC,countC++){
			getCornerValues(&(d[0]),r,c,movementR,movementC,dem_Ptr);
#ifdef DEBUGGER
	/*printf("r:%d c:%d\t",r,c);*/
#endif
                       if(intersect_spherical_dem_cell_contents(&ray_length,&normal,ray,&hit_point,&(d[0])) && ray_length<ray->ray_length){
                        /*        
                        **      store hit DEM contents information
                        */
				*normal_Ptr=normal;
				ray->ray_length=ray_length;
                                hit=TRUE;
                                countC=nCols;
                                countR=nRows;
                        }
                }
	}

	return(hit);

}

int	ray_dem_intersect(bb,normal_Ptr,distance_Ptr,dem_Ptr,ray,type,uv)
BigBag *bb;
triplet	*normal_Ptr; 
double	*distance_Ptr;
void	*dem_Ptr;
Ray	*ray;
int	type;
double	*uv;
{
	Ray	dray;
	int	hit;
	triplet	dnormal;
	double	distance=BIG;

	switch(type){
		case DEM:
		ray->ray_length=  *distance_Ptr;
		hit=dem_intersect(bb,normal_Ptr,dem_Ptr,ray,&distance,uv);
		if(hit){
		  if(distance < *distance_Ptr){
			*distance_Ptr = distance;
			ox=phx;
			oy=phy;
			return(1);
		  }else return(0);
                }else return(0);
		break;
		case SPHERICAL_DEM:
		dray.direction.x= ray->direction.x;
		dray.direction.y= ray->direction.y;
		dray.direction.z= ray->direction.z;
		dray.origin.x= ray->origin.x;
		dray.origin.y= ray->origin.y;
		dray.origin.z= ray->origin.z;

		dray.ray_length=  *distance_Ptr;
		
		hit=spherical_dem_intersect(bb,&dnormal,dem_Ptr,&dray);
		if(hit && ray->ray_length < *distance_Ptr){
			normal_Ptr->x=dnormal.x;
			normal_Ptr->y=dnormal.y;
			normal_Ptr->z=dnormal.z;
			*distance_Ptr = dray.ray_length;
			return(1);
		}else return(0);
		break;
	}

	return(0);
}


