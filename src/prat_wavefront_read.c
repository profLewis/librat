#include "frat.h"
#include <stdlib.h>
/*
 **
 **
 **	prat_wavefront_read.c
 ** 
 **	wavefront ip 
 **
 **
 **
 */

triplet *t_allocate();

triplet Vmin(old,new)
	triplet	old,new;
{
	triplet	out;
	out.x=MMIN(old.x,new.x);
	out.y=MMIN(old.y,new.y);
	out.z=MMIN(old.z,new.z);
	return(out);
}

triplet Vmax(old,new)
	triplet	old,new;
{
	triplet	out;
	out.x=MMAX(old.x,new.x);
	out.y=MMAX(old.y,new.y);
	out.z=MMAX(old.z,new.z);
	return(out);
}

triplet vmin(old,new)
	triplet	old,new;
{
	triplet	out;
	out.x=MMIN(old.x,new.x);
	out.y=MMIN(old.y,new.y);
	out.z=MMIN(old.z,new.z);
	return(out);
}

triplet vmax(old,new)
	triplet	old,new;
{
	triplet	out;
	out.x=MMAX(old.x,new.x);
	out.y=MMAX(old.y,new.y);
	out.z=MMAX(old.z,new.z);
	return(out);
}

void	vertex_to_block(block_Ptr,vertex_in_block_Ptr,vertex_number,blocksize)
	int	*block_Ptr,*vertex_in_block_Ptr,vertex_number,blocksize;
{
	*block_Ptr=(int)(vertex_number-1)/blocksize;
	*vertex_in_block_Ptr = vertex_number%blocksize;
	return;
}

int	initialise_wavefront_storage(blocksize,vertices)
	int	blocksize;VeRtIcEs *vertices;
{
	int	i;
	vertices->blocksize=blocksize;
	vertices->block=0;
	vertices->no_of_vertices=0;
	for(i=0;i<RAT_N_BLOCKS;i++)
		vertices->new[i]=1;
	return(1);
}

void	set_up_local_basis_system(vertex,local_coords)
	Vertex_locals	*local_coords;triplet	*vertex;
{
	matrix_2D	m,m1,inverse_2D();
	triplet		u,v;

	u = vector_minus(vertex[1],vertex[0]);
	v = vector_minus(vertex[2],vertex[0]);

	m.column[0] = local_coords->local_du;
	m.column[1] = local_coords->local_dv;
	m1 = inverse_2D(m);
	local_coords->basis[0] = normalise(vector_plus(V_factor(u,m1.column[0].x),V_factor(v,m1.column[0].y)));
	local_coords->basis[1] = normalise(vector_plus(V_factor(u,m1.column[1].x),V_factor(v,m1.column[1].y)));
	return;
}


/*
 **	read bzp info
 */
int	blp_read(vertex,verbose,line,vertices,normals,locals,bbox_max,bbox_min,normal,local,facet_Ptr,vertexStore)
	char	 *line;
	triplet	 *vertex;
	VeRtIcEs *vertices, *normals, *locals;
	int	 verbose,normal,local,vertexStore;
	triplet	 *bbox_max,*bbox_min;
	BiLinearPatch	*facet_Ptr;
{
	char	*hold;
	int	i,j,block,vertex_in_block;
	triplet	*trip;
	int	ivertex[4];

	hold=line;
	if(sscanf(hold,"%d %d %d %d",&ivertex[0],&ivertex[1],&ivertex[2],&ivertex[3])!=4)
		error1("prat_wavefront_read:\terror in fo specification");

	/* relative */
	if(ivertex[0]<0||ivertex[1]<0||ivertex[2]<0||ivertex[3]<0){
		for(i=0;i<4;i++)ivertex[i]+=vertices->no_of_vertices+1;
	}
	/* bbox */
	for(j=0;j<4;j++){
		if(vertexStore){
			ivertex[j]=(ivertex[j]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[j],vertices->blocksize);
		trip=vertices->vertices[block];
		facet_Ptr->corner[j]=vertex[j]=trip[vertex_in_block];
		*bbox_min=vmin(*bbox_min,vertex[j]);
		*bbox_max=vmax(*bbox_max,vertex[j]);
	}
	if(bbox_max->x == bbox_min->x)bbox_max->x += 0.001;
	if(bbox_max->y == bbox_min->y)bbox_max->y += 0.001;
	if(bbox_max->z == bbox_min->z)bbox_max->z += 0.001;
	if(normal){
		/* 
		 ** TODO: need to modify this for a bilinear patch 
		 */
		if((facet_Ptr->vertex_normals=(Vertex_normals *)calloc(1,sizeof(Vertex_normals)))==0)error1("fo_read:\terror in vertex normal memory allocation");
		/*
		 **	base normal
		 */
		if(vertexStore){
			ivertex[0]=(ivertex[0]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[0],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->vertex_normal=trip[vertex_in_block];
		/*
		 **	u - vector
		 */
		if(vertexStore){
			ivertex[1]=(ivertex[1]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[1],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->normal_du=vector_minus(trip[vertex_in_block],facet_Ptr->vertex_normals->vertex_normal);

		/*
		 **	v - vector
		 */
		if(vertexStore){
			ivertex[2]=(ivertex[2]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[2],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->normal_dv=vector_minus(trip[vertex_in_block],facet_Ptr->vertex_normals->vertex_normal);

	}else
		facet_Ptr->vertex_normals=NULL;
	if(local){
		/* 
		 ** TODO: need to modify this for a bilinear patch 
		 */
		if((facet_Ptr->local_coords=(Vertex_locals *)calloc(1,sizeof(Vertex_locals)))==0)error1("fo_read:\terror in vertex local_coord memory allocation");

		/*
		 **	base local
		 */
		if(vertexStore){
			ivertex[0]=(ivertex[0]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[0],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->vertex_local.x=trip[vertex_in_block].x;
		facet_Ptr->local_coords->vertex_local.y=trip[vertex_in_block].y;
		/*
		 **	u - vector
		 */
		if(vertexStore){
			ivertex[1]=(ivertex[1]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[1],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->local_du.x=trip[vertex_in_block].x-facet_Ptr->local_coords->vertex_local.x;
		facet_Ptr->local_coords->local_du.y=trip[vertex_in_block].y-facet_Ptr->local_coords->vertex_local.y;
		facet_Ptr->local_coords->du_length=V_mod(vector_minus(vertex[1],vertex[0]));

		/*
		 **	v - vector
		 */
		if(vertexStore){
			ivertex[2]=(ivertex[2]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,ivertex[2],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->local_dv.x=trip[vertex_in_block].x-facet_Ptr->local_coords->vertex_local.x;
		facet_Ptr->local_coords->local_dv.y=trip[vertex_in_block].y-facet_Ptr->local_coords->vertex_local.y;

		if(facet_Ptr->local_coords->local_dv.y==0 && facet_Ptr->local_coords->local_dv.x ==0 && facet_Ptr->local_coords->local_du.x ==0 && facet_Ptr->local_coords->local_du.y ==0){
			/*
			 **	assume an error & set up defaults
			 */
			facet_Ptr->local_coords->vertex_local.x=0.0;
			facet_Ptr->local_coords->vertex_local.y=0.0;
			facet_Ptr->local_coords->local_du.x=1.0;
			facet_Ptr->local_coords->local_dv.y=1.0;
			facet_Ptr->local_coords->du_length=1.0;
			facet_Ptr->local_coords->dv_length=1.0;

		}
		facet_Ptr->local_coords->dv_length=V_mod(vector_minus(vertex[2],vertex[0]));
		set_up_local_basis_system(vertex,(facet_Ptr->local_coords));
	}else
		facet_Ptr->local_coords=NULL;
	if(verbose){
		fprintf(stderr,"b");
		if(local)fprintf(stderr,"l");
		if(normal)fprintf(stderr,"n");
	}
	/* calculate size */
	{
		triplet du,dv;
		du = vector_minus(facet_Ptr->corner[0],facet_Ptr->corner[1]);
		dv = vector_minus(facet_Ptr->corner[1],facet_Ptr->corner[2]);
		facet_Ptr->objectSize = 0.5*fabs(V_mod(vector_cross(du,dv)));
		du = vector_minus(facet_Ptr->corner[2],facet_Ptr->corner[3]);
		dv = vector_minus(facet_Ptr->corner[3],facet_Ptr->corner[0]);
		facet_Ptr->objectSize += 0.5*fabs(V_mod(vector_cross(du,dv)));
	}
	return(1);
}

/*
 **	read fo info
 */
int	fo_read(vertex,verbose,line,vertices,normals,locals,bbox_max,bbox_min,normal,local,facet_Ptr,vertexStore)
	char	 *line;
	triplet	 *vertex;
	VeRtIcEs *vertices, *normals, *locals;
	int	 verbose,normal,local,vertexStore;
	triplet	 *bbox_max,*bbox_min;
	fFacet	*facet_Ptr;
{
	char	*hold;
	int	i,j,block,vertex_in_block;
	triplet	*trip;
	Ivect3	v;

	hold=line;
	if(sscanf(hold,"%d %d %d",&v.vertex[0],&v.vertex[1],&v.vertex[2])!=3)
		error1("prat_wavefront_read:\terror in fo specification");

	/* relative */
	if(v.vertex[0]<0||v.vertex[1]<0||v.vertex[2]<0)
		for(i=0;i<3;i++)v.vertex[i]+=vertices->no_of_vertices+1;
	/* bbox */
	for(j=0;j<3;j++){
		if(vertexStore){
			v.vertex[j]=(v.vertex[j]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[j],vertices->blocksize);
		trip=vertices->vertices[block];
		vertex[j]=trip[vertex_in_block];
		*bbox_min=vmin(*bbox_min,vertex[j]);
		*bbox_max=vmax(*bbox_max,vertex[j]);
	}
#ifdef DESPARATE
	for(j=0;j<3;j++){
		fprintf(stderr,"v %f %f %f\n",vertex[j].x,vertex[j].y,vertex[j].z);
	}
	fprintf(stderr,"fo -1 -2 -3\n");
#endif
	if(bbox_max->x == bbox_min->x)bbox_max->x += 0.001;
	if(bbox_max->y == bbox_min->y)bbox_max->y += 0.001;
	if(bbox_max->z == bbox_min->z)bbox_max->z += 0.001;
	if(normal){
		if((facet_Ptr->vertex_normals=(Vertex_normals *)calloc(1,sizeof(Vertex_normals)))==0)error1("fo_read:\terror in vertex normal memory allocation");
		/*
		 **	base normal
		 */
		if(vertexStore){
			v.vertex[0]=(v.vertex[0]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[0],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->vertex_normal=trip[vertex_in_block];
		/*
		 **	u - vector
		 */
		if(vertexStore){
			v.vertex[1]=(v.vertex[1]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[1],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->normal_du=vector_minus(trip[vertex_in_block],facet_Ptr->vertex_normals->vertex_normal);

		/*
		 **	v - vector
		 */
		if(vertexStore){
			v.vertex[2]=(v.vertex[2]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[2],vertices->blocksize);		
		trip=normals->vertices[block];
		facet_Ptr->vertex_normals->normal_dv=vector_minus(trip[vertex_in_block],facet_Ptr->vertex_normals->vertex_normal);

	}else
		facet_Ptr->vertex_normals=NULL;
	if(local){
		if((facet_Ptr->local_coords=(Vertex_locals *)calloc(1,sizeof(Vertex_locals)))==0)error1("fo_read:\terror in vertex local_coord memory allocation");

		/*
		 **	base local
		 */
		if(vertexStore){
			v.vertex[0]=(v.vertex[0]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[0],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->vertex_local.x=trip[vertex_in_block].x;
		facet_Ptr->local_coords->vertex_local.y=trip[vertex_in_block].y;
		/*
		 **	u - vector
		 */
		if(vertexStore){
			v.vertex[1]=(v.vertex[1]-1)%vertexStore+1;
		}
		vertex_to_block(&block,&vertex_in_block,v.vertex[1],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->local_du.x=trip[vertex_in_block].x-facet_Ptr->local_coords->vertex_local.x;
		facet_Ptr->local_coords->local_du.y=trip[vertex_in_block].y-facet_Ptr->local_coords->vertex_local.y;
		facet_Ptr->local_coords->du_length=V_mod(vector_minus(vertex[1],vertex[0]));

		/*
		 **	v - vector
		 */
		if(vertexStore){
			v.vertex[2]=(v.vertex[2]-1)%vertexStore+1;
		}	
		vertex_to_block(&block,&vertex_in_block,v.vertex[2],vertices->blocksize);		
		trip=locals->vertices[block];
		facet_Ptr->local_coords->local_dv.x=trip[vertex_in_block].x-facet_Ptr->local_coords->vertex_local.x;
		facet_Ptr->local_coords->local_dv.y=trip[vertex_in_block].y-facet_Ptr->local_coords->vertex_local.y;

		if(facet_Ptr->local_coords->local_dv.y==0 && facet_Ptr->local_coords->local_dv.x ==0 && facet_Ptr->local_coords->local_du.x ==0 && facet_Ptr->local_coords->local_du.y ==0){
			/*
			 **	assume an error & set up defaults
			 */
			facet_Ptr->local_coords->vertex_local.x=0.0;
			facet_Ptr->local_coords->vertex_local.y=0.0;
			facet_Ptr->local_coords->local_du.x=1.0;
			facet_Ptr->local_coords->local_dv.y=1.0;
			facet_Ptr->local_coords->du_length=1.0;
			facet_Ptr->local_coords->dv_length=1.0;

		}
		facet_Ptr->local_coords->dv_length=V_mod(vector_minus(vertex[2],vertex[0]));
		set_up_local_basis_system(vertex,(facet_Ptr->local_coords));
	}else
		facet_Ptr->local_coords=NULL;
	if(verbose){
		fprintf(stderr,"f");
		if(local)fprintf(stderr,"l");
		if(normal)fprintf(stderr,"n");
	}
	{
		triplet du,dv;
		du = vector_minus(vertex[0],vertex[1]);
		dv = vector_minus(vertex[1],vertex[2]);
		facet_Ptr->objectSize = 0.5*fabs(V_mod(vector_cross(du,dv)));
	}

	return(1);
}


/*
 **	read sphere info
 */
int	sph_read(bounds,sph,verbose,line,vertices,bbox_max,bbox_min,vertexStore)
	double	*bounds;
	Sphere	*sph;
	char	 *line;
	VeRtIcEs *vertices;
	int	 verbose,vertexStore;
	triplet	 *bbox_max,*bbox_min;
{
	char	*hold;
	int	block,vertex_in_block;
	triplet	*trip,vertex,r,rmin,rmax;
	int	v;

	hold=line;
#ifdef DOUBLEDEF
	if(sscanf(hold,"%d %lf",&v,&(sph->radius))!=2)error1("prat_wavefront_read:\terror in sph specification");
#else
	if(sscanf(hold,"%d %f",&v,&(sph->radius))!=2)error1("prat_wavefront_read:\terror in sph specification");
#endif

	if(sph->radius<=0.0)error1("prat_wavefront_read:\terror in sph specification:\n\tzero or -ve radius specified");

	/*
	 **	precompute radius ^ 2
	 */

	sph->r2=sph->radius*sph->radius;

	/* surface area */
	sph->objectSize = 4. * M_PI * sph->r2;

	r=vector_copy2(sph->radius,sph->radius,sph->radius);

	/* relative */
	if(v<0)
		v+=vertices->no_of_vertices+1;
	if(vertexStore){
		v=(v-1)%vertexStore+1;
	}	
	vertex_to_block(&block,&vertex_in_block,v,vertices->blocksize);
	trip=vertices->vertices[block];
	vertex=trip[vertex_in_block];
	sph->centre=vertex;

	/* bbox */
	rmin=vector_minus(vertex,r);
	rmax=vector_plus(vertex,r);
	*bbox_min=vmin(*bbox_min,rmin);
	*bbox_max=vmax(*bbox_max,rmax);
	bounds[0]=rmax.x;
	bounds[1]=rmax.y;
	bounds[2]=rmax.x;
	bounds[3]=rmin.x;
	bounds[4]=rmin.y;
	bounds[5]=rmin.z;
	if(verbose)fprintf(stderr,"s");
	return(1);
}


/*
 **	read cylinder info
 */
int	cyl_read(bounds,cyl,verbose,line,vertices,bbox_max,bbox_min,closed,vertexStore)
	double	*bounds;
	Cylinder *cyl;
	char	 *line;
	VeRtIcEs *vertices;
	int	 verbose,closed,vertexStore;
	triplet	 *bbox_max,*bbox_min;
{
	char	*hold;
	int	i,block,vertex_in_block;
	triplet	bboxmax,bboxmin,*trip,vertex[2],normal,V[4],R;
	int	v[2];
	double	radius;

	bboxmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	bboxmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	hold=line;
#ifdef DOUBLEDEF
	if(sscanf(hold,"%d %d %lf",&v[0],&v[1],&(radius))!=3)error1("prat_wavefront_read:\terror in cyl specification");
#else
	if(sscanf(hold,"%d %d %f",&v[0],&v[1],&(radius))!=3)error1("prat_wavefront_read:\terror in cyl specification");
#endif

	if(radius<=0.0)error1("prat_wavefront_read:\terror in cyl specification:\n\tzero or -ve radius specified");

	cyl->r2=radius*radius;

	/* relative */
	if(v[0]<0 || v[1]<0)
		for(i=0;i<2;i++)v[i]+=vertices->no_of_vertices+1;

	for(i=0;i<2;i++){
		if(vertexStore){
			v[i]=(v[i]-1)%vertexStore+1;
		}	
		vertex_to_block(&block,&vertex_in_block,v[i],vertices->blocksize);
		trip=vertices->vertices[block];
		vertex[i]=trip[vertex_in_block];
	}

	/*
	 **	sort data into structure
	 */

	normal=vector_minus(vertex[1],vertex[0]);
	cyl->height=MAX(0.000000000001,V_mod(normal));
	cyl->normal=V_factor(normal,1.0/(cyl->height));
	cyl->origin=vertex[0];
	/* surface area */        
	cyl->objectSize = 2 * M_PI * radius * cyl->height;

	/*
	 **	bounds 
	 */
	R=vector_copy2(radius,radius,radius);
	V[0]=vector_plus(R,vertex[0]);
	V[1]=vector_plus(R,vertex[1]);
	V[2]=vector_minus(vertex[0],R);
	V[3]=vector_minus(vertex[1],R);
	for(i=0;i<4;i++){
		bboxmin=vmin(bboxmin,V[i]);
		bboxmax=vmax(bboxmax,V[i]);
	}
	bounds[0]=bboxmax.x;
	bounds[1]=bboxmax.y;
	bounds[2]=bboxmax.z;
	bounds[3]=bboxmin.x;
	bounds[4]=bboxmin.y;
	bounds[5]=bboxmin.z;

	*bbox_max=vmax(*bbox_max,bboxmax);
	*bbox_min=vmin(*bbox_min,bboxmin);
	if(verbose&&!closed)fprintf(stderr,"c");
	if(verbose&&closed)fprintf(stderr,"C");
	if(closed){

		cyl->objectSize += 2. * M_PI * radius * radius;
		if((cyl->ends=(Disk *)calloc(2,sizeof(Disk)))==0){
			fprintf(stderr,"error in core allocation\n");
			exit(1);
		}
		cyl->ends[0].origin=vertex[0];
		cyl->ends[1].origin=vertex[1];
		cyl->ends[0].normal=cyl->ends[1].normal=cyl->normal;
		cyl->ends[0].r2=cyl->ends[1].r2=cyl->r2;
	}else{
		cyl->ends=NULL;
	}
	return(1);
}


/*
 **	read disk info
 */
int	disk_read(bounds,disk,verbose,line,vertices,bbox_max,bbox_min,vertexStore)
	double	*bounds;
	Disk *disk;
	char	 *line;
	VeRtIcEs *vertices;
	int	 verbose,vertexStore;
	triplet	 *bbox_max,*bbox_min;
{
	char	*hold;
	int	i,block,vertex_in_block;
	triplet	bboxmax,bboxmin,*trip,vertex[2],V[4],R;
	int	v[2];
	double	radius;
	/* disk, origin, normal */

	bboxmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	bboxmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	hold=line;
#ifdef DOUBLEDEF
	if(sscanf(hold,"%d %d %lf",&v[0],&v[1],&(radius))!=3)error1("prat_wavefront_read:\terror in disk specification");
#else
	if(sscanf(hold,"%d %d %f",&v[0],&v[1],&(radius))!=3)error1("prat_wavefront_read:\terror in disk specification");
#endif

	if(radius<=0.0)error1("prat_wavefront_read:\terror in disk specification:\n\tzero or -ve radius specified");

	disk->r2=radius*radius;

	disk->objectSize = M_PI * disk->r2;
	/* relative */
	if(v[0]<0 || v[1]<0)
		for(i=0;i<2;i++)v[i]+=vertices->no_of_vertices+1;

	for(i=0;i<2;i++){
		if(vertexStore){
			v[i]=(v[i]-1)%vertexStore+1;
		}	
		vertex_to_block(&block,&vertex_in_block,v[i],vertices->blocksize);
		trip=vertices->vertices[block];
		vertex[i]=trip[vertex_in_block];
	}
	disk->origin=vertex[0];
	/* disk normal */
	disk->normal=normalise(vertex[1]);

	/*
	 **	bounds 
	 */
	R=vector_copy2(radius,radius,radius);
	V[0]=vector_plus(R,vertex[0]);
	V[1]=vector_minus(vertex[0],R);
	for(i=0;i<2;i++){
		bboxmin=vmin(bboxmin,V[i]);
		bboxmax=vmax(bboxmax,V[i]);
	}
	bounds[0]=bboxmax.x;
	bounds[1]=bboxmax.y;
	bounds[2]=bboxmax.z;
	bounds[3]=bboxmin.x;
	bounds[4]=bboxmin.y;
	bounds[5]=bboxmin.z;

	*bbox_max=vmax(*bbox_max,bboxmax);
	*bbox_min=vmin(*bbox_min,bboxmin);
	if(verbose)fprintf(stderr,"d");
	return(1);
}

/*
 **	read ellipse info
 */
int	ellipse_read(bounds,ell,verbose,line,vertices,bbox_max,bbox_min,vertexStore)
	double	*bounds;
	Ellipse *ell;
	char	 *line;
	VeRtIcEs *vertices;
	int	 verbose,vertexStore;
	triplet	 *bbox_max,*bbox_min;
{
	char	*hold;
	int	i,block,vertex_in_block;
	triplet	bboxmax,bboxmin,*trip,V[4];
	int	v[1];
	double p=1.6075;

	bboxmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	bboxmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	hold=line;
	if(sscanf(hold,"%d %lf %lf %lf",&v[0],&ell->dimensions.x,&ell->dimensions.y,&ell->dimensions.z)!=4)error1("prat_wavefront_read:\terror in ell specification");

	if(ell->dimensions.x<=0.0||ell->dimensions.y<=0.0||ell->dimensions.z<=0.0)error1("prat_wavefront_read:\terror in ell specification:\n\tzero or -ve radius specified");

	/* ellipsoid surface area */
	/* approximation http://en.wikipedia.org/wiki/Ellipsoid */
	ell->objectSize = 4. * M_PI * pow((pow(ell->dimensions.x,p)*pow(ell->dimensions.y,p)+pow(ell->dimensions.x,p)*pow(ell->dimensions.z,p)+pow(ell->dimensions.y,p)*pow(ell->dimensions.z,p))/3.,1./p);	


	/* relative */
	if(v[0]<0)v[0]+=vertices->no_of_vertices+1;

	if(vertexStore){
		v[0]=(v[0]-1)%vertexStore+1;
	}
	vertex_to_block(&block,&vertex_in_block,v[0],vertices->blocksize);
	trip=vertices->vertices[block];
	/* origin in file is given as base so this sets it as the centre */
	ell->origin=vector_plus(trip[vertex_in_block],vector_copy2(0.0,0.0,ell->dimensions.z));


	/*
	 **	bounds 
	 */
	V[0]=vector_plus(ell->origin,ell->dimensions);
	V[1]=vector_minus(ell->origin,ell->dimensions);
	for(i=0;i<2;i++){
		bboxmin=vmin(bboxmin,V[i]);
		bboxmax=vmax(bboxmax,V[i]);
	}
	bounds[0]=bboxmax.x;
	bounds[1]=bboxmax.y;
	bounds[2]=bboxmax.z;
	bounds[3]=bboxmin.x;
	bounds[4]=bboxmin.y;
	bounds[5]=bboxmin.z;

	*bbox_max=vmax(*bbox_max,bboxmax);
	*bbox_min=vmin(*bbox_min,bboxmin);
	if(verbose)fprintf(stderr,"e");
	return(1);
}

/*
 **      read spheroid info
 */
int     spheroid_read(bounds,ell,verbose,line,vertices,bbox_max,bbox_min,vertexStore)
	double  *bounds;
	Spheroid *ell;
	char     *line;
	VeRtIcEs *vertices;
	int      verbose,vertexStore;
	triplet  *bbox_max,*bbox_min;
{
	char    *hold;
	int     i,j,block,vertex_in_block;
	triplet bboxmax,bboxmin,*trip,V[4],xx,yy;
	int     v[2];
	double p=1.6075,semilen,F;

	bboxmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	bboxmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	hold=line;
	if(sscanf(hold,"%d %d %lf",&v[0],&v[1],&ell->radius)!=3)error1("prat_wavefront_read:\terror in spheroid specification");

	if(ell->radius<=0.0)error1("prat_wavefront_read:\terror in spheroid specification:\n\tzero or -ve radius specified");

	/* relative */
	if(v[0]<0)v[0]+=vertices->no_of_vertices+1;
	if(v[1]<0)v[1]+=vertices->no_of_vertices+1;

	if(vertexStore){
		v[0]=(v[0]-1)%vertexStore+1;
		v[1]=(v[1]-1)%vertexStore+1;

	}
	vertex_to_block(&block,&vertex_in_block,v[0],vertices->blocksize);
	trip=vertices->vertices[block];
	ell->origin=trip[vertex_in_block];

	vertex_to_block(&block,&vertex_in_block,v[1],vertices->blocksize);
	trip=vertices->vertices[block];
	ell->tip=trip[vertex_in_block];

	ell->naxes[2] = vector_minus(ell->tip,ell->origin);
	semilen = V_mod(ell->naxes[2])*0.5;

	if(semilen<=0.0)error1("prat_wavefront_read:\terror in spheroid specification:\n\tzero or -ve base to tip specified");

	ell->naxes[2] = normalise(ell->naxes[2]);
	xx = vector_copy2(0.,0.,1.0);
	yy = vector_cross(ell->naxes[2],xx);
	if ( V_mod(yy) <= 0.){
		xx = vector_copy2(0.,1.,0.0);
		yy = vector_cross(ell->naxes[2],xx);
		if ( V_mod(yy) <= 0.){
			xx = vector_copy2(1.,0.,0.0);
			yy = vector_cross(ell->naxes[2],xx);
		}
	}
	yy = normalise(yy);
	xx = vector_cross(yy,ell->naxes[2]);
        /* up */
        ell->V1 = ell->naxes[2];

	ell->naxes[0] = V_factor(xx,1./ell->radius);
	ell->naxes[1] = V_factor(yy,1./ell->radius);
	ell->naxes[2] = V_factor(ell->naxes[2],1./semilen);

        ell->dimensions.x = ell->dimensions.y = (ell->radius);
        ell->dimensions.z = (semilen);

	/* ellipsoid surface area */
	/* approximation http://en.wikipedia.org/wiki/Ellipsoid */
	ell->objectSize = 4. * M_PI * pow((pow(ell->radius,p)*pow(ell->radius,p)+pow(ell->radius,p)*pow(semilen,p)+pow(ell->radius,p)*pow(semilen,p))/3.,1./p);

	/*
	 **      bounds ... add the radius on 
	 */
        for(j=0;j<3;j++){
	  xx = V_factor(ell->naxes[j],ell->radius);
	  V[0]=vector_plus(ell->origin,xx);
	  V[1]=vector_minus(ell->origin,xx);
	  V[2]=vector_plus(ell->tip,xx);
	  V[3]=vector_minus(ell->tip,xx);
    
	  for(i=0;i<4;i++){
		bboxmin=vmin(bboxmin,V[i]);
		bboxmax=vmax(bboxmax,V[i]);
	  }
        }
	bounds[0]=bboxmax.x;
	bounds[1]=bboxmax.y;
	bounds[2]=bboxmax.z;
	bounds[3]=bboxmin.x;
	bounds[4]=bboxmin.y;
	bounds[5]=bboxmin.z;

	*bbox_max=vmax(*bbox_max,bboxmax);
	*bbox_min=vmin(*bbox_min,bboxmin);

	ell->centre = V_factor(vector_plus(ell->origin,ell->tip),0.5);
        /* for the normal, we need the focus stored */
        ell->a2 = semilen*semilen-ell->radius*ell->radius;

	if(verbose)fprintf(stderr,"Sph");
	return(1);
}


triplet multiply_matrix3_vector3(double *m,triplet *translate,triplet *v){
	triplet out;

	out.x = m[0*3+0]*v->x + m[0*3+1]*v->y + m[0*3+2]*v->z;
	out.y = m[1*3+0]*v->x + m[1*3+1]*v->y + m[1*3+2]*v->z;
	out.z = m[2*3+0]*v->x + m[2*3+1]*v->y + m[2*3+2]*v->z;

	if(translate){
		out.x += translate->x;
		out.y += translate->y;
		out.z += translate->z;
	}
	return(out);

}

void	transform_bbox2(double scale,double *m,triplet *translate,triplet *bboxmin,triplet *bboxmax)
{
	triplet	outmin,outmax;
	triplet	corners[8];
	int	i;

	/*
	   July 8 2011
	   Lewis, UCL

	   add in correction for scale in case some people want everything bundled in the one matrix

	   Rather than passing the scale through, calculate it internally

	   it should be 1./cbrt(det(m))
	   [I think]
	 */
	double det;
	void Matrix_determinant();

	/*Matrix_determinant(m,&det,3);
	  if ( det == 0.0){
	  fprintf(stderr,"zero sized matrix iencountered in transform_bbox2 ... check your matrix definitions.");
	  det = 1.0;
	  }
	  scale = 1./cbrt(det); */
	/*fprintf(stderr,"%f %f\n",scale,det);*/

	corners[0] = vector_copy2(scale*bboxmin->x,scale*bboxmin->y,scale*bboxmin->z);
	corners[1] = vector_copy2(scale*bboxmin->x,scale*bboxmin->y,scale*bboxmax->z);
	corners[2] = vector_copy2(scale*bboxmin->x,scale*bboxmax->y,scale*bboxmin->z);
	corners[3] = vector_copy2(scale*bboxmin->x,scale*bboxmax->y,scale*bboxmax->z);
	corners[4] = vector_copy2(scale*bboxmax->x,scale*bboxmin->y,scale*bboxmin->z);
	corners[5] = vector_copy2(scale*bboxmax->x,scale*bboxmin->y,scale*bboxmax->z);
	corners[6] = vector_copy2(scale*bboxmax->x,scale*bboxmax->y,scale*bboxmin->z);
	corners[7] = vector_copy2(scale*bboxmax->x,scale*bboxmax->y,scale*bboxmax->z);

	outmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	outmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	for(i=0;i<8;i++){
		outmin = vmin( outmin, multiply_matrix3_vector3(m,translate,&corners[i]));
		outmax = vmax( outmax, multiply_matrix3_vector3(m,translate,&corners[i]));
	}
	*bboxmax=outmax;
	*bboxmin=outmin;
	return;
}


void	transform_bbox(Matrix4 m,triplet *bboxmin,triplet *bboxmax)
{
	triplet	outmin,outmax;
	triplet	corners[8];
	int	i;

	corners[0] = vector_copy2(bboxmin->x,bboxmin->y,bboxmin->z);
	corners[1] = vector_copy2(bboxmin->x,bboxmin->y,bboxmax->z);
	corners[2] = vector_copy2(bboxmin->x,bboxmax->y,bboxmin->z);
	corners[3] = vector_copy2(bboxmin->x,bboxmax->y,bboxmax->z);
	corners[4] = vector_copy2(bboxmax->x,bboxmin->y,bboxmin->z);
	corners[5] = vector_copy2(bboxmax->x,bboxmin->y,bboxmax->z);
	corners[6] = vector_copy2(bboxmax->x,bboxmax->y,bboxmin->z);
	corners[7] = vector_copy2(bboxmax->x,bboxmax->y,bboxmax->z);

	outmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	outmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	for(i=0;i<8;i++){
		outmin = vmin( outmin, multiply_matrix4_vector3(m,&corners[i]));
		outmax = vmax( outmax, multiply_matrix4_vector3(m,&corners[i]));
	}
	*bboxmax=outmax;
	*bboxmin=outmin;
	return;
}

void matrixCopy(double *m1,double *m0){
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			m0[i*3+j]=m1[i*3+j];
		}
	}
	return;
}


void matrixMult(double *out,double *a,double *b){
	int i,j,I;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			out[i*3+j]=0.;
			for(I=0;I<3;I++)out[i*3+j]+=a[i*3+I]*b[I*3+j];
		}
	}
	return;
}

double *setXRotation(double *matrix,double rotation){
	double   *d_allocate();
	double m[9],m0[9],c,s;
	int i;
	if(!matrix){matrix=d_allocate(9);for(i=0;i<9;i+=4)matrix[i]=1.;}
	matrixCopy(matrix,m0);
	for(i=0;i<9;i++)m[i]=0.;for(i=0;i<9;i+=4)m[i]=1.;
	c=cos(DTOR(rotation));
	s=sin(DTOR(rotation));

	m[4]=c;
	m[5]=-s;
	m[7]=s;
	m[8]=c;

	matrixMult(matrix,m,m0);

	return(matrix);
}


double *setYRotation(double *matrix,double rotation){
	double   *d_allocate();
	double m[9],m0[9],c,s;
	int i;
	if(!matrix){matrix=d_allocate(9);for(i=0;i<9;i+=4)matrix[i]=1.;}
	matrixCopy(matrix,m0);
	for(i=0;i<9;i++)m[i]=0.;for(i=0;i<9;i+=4)m[i]=1.;
	c=cos(DTOR(rotation));
	s=sin(DTOR(rotation));

	m[0]=c;
	m[2]=s;
	m[6]=-s;
	m[8]=c;

	matrixMult(matrix,m,m0);

	return(matrix);
}


double *setZRotation(double *matrix,double rotation){
	double   *d_allocate();
	double m[9],m0[9],c,s;
	int i;
	if(!matrix){matrix=d_allocate(9);for(i=0;i<9;i+=4)matrix[i]=1.;}
	matrixCopy(matrix,m0);
	for(i=0;i<9;i++)m[i]=0.;for(i=0;i<9;i+=4)m[i]=1.;
	c=cos(DTOR(rotation));
	s=sin(DTOR(rotation));

	m[0]=c;
	m[1]=-s;
	m[3]=s;
	m[4]=c;

	matrixMult(matrix,m,m0);

	return(matrix);
}

/*
 **	read clone info
 */
int	clone_read(bounds,clone,verbose,line,group_Ptr,bbox_Ptr,transformed_bbox_Ptr)
	double	*bounds;
	Clones 	*clone;
	char	*line;
	Group	*group_Ptr;
	int	verbose;
	BBox	*bbox_Ptr,*transformed_bbox_Ptr;
{
	char	*hold,a[1000],b[1000],c[1000],srotation[1000];
	int	i,j,found_group=0,dontFinish;
	triplet	bboxmax,bboxmin,tmp;
	double	rotation,*matrix=NULL;

	bboxmax=vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	bboxmin=vector_copy2((double)BIG,(double)BIG,(double)BIG);
	hold=line;
	if(sscanf(hold,"%s %s %s",a,b,c)!=3)error1("prat_wavefront_read:\terror in clone specification");
	hold = strchr(hold,a[0])+strlen(a);
	hold = strchr(hold,b[0])+strlen(b);
	hold = strchr(hold,c[0])+strlen(c)+1;
	hold[strlen(hold)-1]='\0';	/* group name */
	clone->translation.x = atof(a);		/* ... no checking here... */
	clone->translation.y = atof(b);
	clone->translation.z = atof(c);
	tmp=clone->translation;
	/* clone Z-rotation */
	clone->z_rotation_flag=0;
	dontFinish=1;
	while(dontFinish==1 && strlen(hold)>0 && sscanf(hold,"%s",a)==1){
		if(!strcmp(a,"Rx")){
			/* x - rotation */
			if(!(sscanf(hold,"%s %s",a,srotation)==2))error1("prat_wavefront_read:\terror in clone specification after Rx specification");
			rotation=atof(srotation);
			if(rotation!=0.0){
				clone->z_rotation_flag=1;
				matrix=setXRotation(matrix,rotation);
			}
			hold = strchr(hold,a[0])+strlen(a);
			hold = strchr(hold,srotation[0])+strlen(srotation);
		}else if(!strcmp(a,"Ry")){
			/* y - rotation */
			if(!(sscanf(hold,"%s %s",a,srotation)==2))error1("prat_wavefront_read:\terror in clone specification after Ry specification");
			rotation=atof(srotation);
			if(rotation!=0.0){
				clone->z_rotation_flag=1;
				matrix=setYRotation(matrix,rotation);
			}
			hold = strchr(hold,a[0])+strlen(a);
			hold = strchr(hold,srotation[0])+strlen(srotation);
		}else if(!strcmp(a,"Rz")){
			/* z - rotation */
			if(!(sscanf(hold,"%s %s",a,srotation)==2))error1("prat_wavefront_read:\terror in clone specification after Rz specification");
			rotation=atof(srotation);
			if(rotation!=0.0){
				clone->z_rotation_flag=1;
				matrix=setZRotation(matrix,rotation);
			}
			hold = strchr(hold,a[0])+strlen(a);
			hold = strchr(hold,srotation[0])+strlen(srotation);
		}else if(!strcmp(a,"Transform")){
			/* arbitrary 3 x 3 matrix */
			{
				char s[10][100];
				int i;
				double thisMatrix[9],m0[9],value;
				if(!(sscanf(hold,"%s %s %s %s %s %s %s %s %s %s",a,s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8])==10))error1("prat_wavefront_read:\terror in clone specification after Rz specification");
				hold = strchr(hold,a[0])+strlen(a);
				clone->z_rotation_flag=1;
				for(i=0;i<9;i++){
					value=atof(s[i]);
					thisMatrix[i]=value;
				}
				/* set up matrix if it doesnt exist */
				if(!matrix){matrix=d_allocate(9);for(i=0;i<9;i+=4)matrix[i]=1.;}
				/* copy matrix to m0 */
				matrixCopy(matrix,m0);
				/* multiply thisMatrix by m0 into matrix */
				matrixMult(matrix,thisMatrix,m0);
				for(i=0;i<9;i++){
					hold = strchr(hold,s[i][0])+strlen(s[i]);
				}
			}
		}else{
			rotation=atof(a);
			if(rotation!=0.0 || a[0]=='0'){
				clone->z_rotation_flag=1;
				matrix=setZRotation(matrix,rotation);
				hold = strchr(hold,a[0])+strlen(a)+1;
				tmp=clone->translation;
			}else
				dontFinish=0;
		}
	}
	/* get rid of prec blanks */
	j=0;
	for(i=0;i<strlen(hold);i++)if(hold[i]==' ')j++;else break;
	hold=&hold[j];
	if(matrix)clone->matrix=matrix;

	for(i=0;i<group_Ptr->no_of_groups;i++){
		if(strcmp(group_Ptr->group_names[i],hold)==0){
			found_group=1;
			clone->bbox=group_Ptr->group_bboxes[i];
			if(clone->bbox->hit[0]==NULL)
				error1("librat:\terror in plant definition - attempting to clone plant with inconsistent bounding box contents");
			i=group_Ptr->no_of_groups;
		}
	}
	if(!found_group){
		fprintf(stderr,"prat_wavefront_read:\tgroup %s not found for cloning\n",hold);exit(1);
	}
	/*
	 **	bounds 
	 */
	bboxmin=clone->bbox->min;
	bboxmax=clone->bbox->max;
	/* first do matrix multiplcation (if appropraite) */
	if(matrix){
		transform_bbox2(1.0,matrix,&(clone->translation),&bboxmin,&bboxmax);
	}else{
		bboxmin=vector_plus(clone->translation,bboxmin);
		bboxmax=vector_plus(clone->translation,bboxmax);
	}

	bounds[0]=bboxmax.x;
	bounds[1]=bboxmax.y;
	bounds[2]=bboxmax.z;
	bounds[3]=bboxmin.x;
	bounds[4]=bboxmin.y;
	bounds[5]=bboxmin.z;

	bbox_Ptr->max=vmax(bbox_Ptr->max,bboxmax);
	bbox_Ptr->min=vmin(bbox_Ptr->min,bboxmin);

	if(verbose)fprintf(stderr,"<clone %s>",hold);
	return(1);
}

/*
 **	read arbitrarily-inclined plane info
 */
int	plane_read(normal_coords,dw,verbose,line,vertices,vertexStore)
	char	 *line;
	triplet	 *normal_coords;
	double	 *dw;
	VeRtIcEs *vertices;
	int	 verbose,vertexStore;
{
	char	*hold;
	int	block,vertex_in_block;
	triplet	*trip,v;
	int	normal;
	int	point_in_plane;

	hold=line;
	if(sscanf(hold,"%d %d",&normal,&point_in_plane)!=2)error1("prat_wavefront_read:\terror in plane specification");

	/* relative */
	if(normal<0||point_in_plane<0){
		normal+=vertices->no_of_vertices+1;
		point_in_plane+=vertices->no_of_vertices+1;
	}
	if(vertexStore){
		normal=(normal-1)%vertexStore+1;
		point_in_plane=(point_in_plane-1)%vertexStore+1;
	}	
	vertex_to_block(&block,&vertex_in_block,normal,vertices->blocksize);
	trip=vertices->vertices[block];
	*normal_coords=normalise(trip[vertex_in_block]);

	vertex_to_block(&block,&vertex_in_block,point_in_plane,vertices->blocksize);
	trip=vertices->vertices[block];
	v=trip[vertex_in_block];
	*dw= -V_dot(v,*normal_coords);

	if(verbose)fprintf(stderr,"PLANE");
	return(1);
}

/*
 **	mtllib_read
 **
 */

int	mtllib_read(bb,verbose,line,material_list_Ptr,material_names,material_table)
	BigBag *bb;
	char	 *line;
	int	 verbose;
	Material_List	*material_list_Ptr;
	char	**material_names;
	Material_table	*material_table;
{
	static char	*filename=NULL;
	static int timer=0;
	void	read_material_definition_file();

	timer++;
	if(timer==1)filename=c_allocate(1024);
	if(sscanf(line,"%s",filename)!=1)error1("prat_wavefront_read:\terror in mtllib specification");
#ifdef DEBUG
        fprintf(stderr,"+--%s\n",line);
        fprintf(stderr," --%s\n",filename);
#endif


#ifdef PFAT
	/* for PFAT, we need the lut early on, so cant make it very efficient as in prat */
	allocate_partial_material_list();
#endif
	read_material_definition_file(bb,verbose,&filename,material_list_Ptr,material_names,&(material_list_Ptr->no_of_materials),material_table);
	if(verbose)fprintf(stderr,"\n");
	return(1);
}


/*
 **	current_material_read
 **
 */

Material_table	*current_material_read(verbose,line,material_names,materialList,material_table)
	char	 *line;
	int	 verbose;
	char	**material_names;
	Material_table	*material_table;
	Material_List	*materialList;
{
	Material_table	*current_mtl=NULL;
	char	material_name[200];
	int	i,j,found=0,*no_of_materials_Ptr,newMaterial;

	no_of_materials_Ptr=&materialList->no_of_materials;

	if(sscanf(line,"%s",material_name)!=1)error1("prat_wavefront_read:\terror in usemtl specification");
	for(i=0;i< *no_of_materials_Ptr;i++){
		if(strcmp(material_names[i],material_name)==0){
			current_mtl = &(material_table[i]);
			if(materialList->useage){
				/*
				 **	if checking on material useage,
				 **	insert new material information
				 */
				newMaterial=1;
				for(j=0;j<*no_of_materials_Ptr;j++){
					if(materialList->useage->materials[j]!=NULL){
						if((char *)materialList->useage->materials[j]==(char *)current_mtl)newMaterial=0;
					}else{
						if(newMaterial){
							materialList->useage->materials[j]=(char *)current_mtl;
							/*materialList->useage->names[j]=c_allocate(strlen(material_name)+1); */
							if(!(materialList->useage->names[j]=(char *)calloc(strlen(material_name)+1,sizeof(char)))){
								fprintf(stderr,"error in core allocation\n");
								exit(1);
							}
							strcpy(materialList->useage->names[j],material_name);
							(materialList->useage->no_of_materials)++;
							newMaterial=0;
						}
					}
				}
			}
			i = *no_of_materials_Ptr;
			found = 1;
			if(verbose)fprintf(stderr,"<%s>",material_name);
		}
	}
	if(!found){
		error1("error in obj file:\t material not specified");
	}
	return(current_mtl);
}

/*
 **	load transformation matrices
 */

int	load_transformation_matrix(line,bbox_Ptr,m_inv_reverse_Ptr,m_inverse_fwd_Ptr)
	char	*line;
	BBox	*bbox_Ptr;
	Matrix4	*m_inv_reverse_Ptr, *m_inverse_fwd_Ptr;
{
	char	*hold,transformation_keyword[100];
	Matrix4	matrix;
	double	scale,theta;
	triplet	fix_point,translation,axis,d_scale;

	hold=line;
	if( sscanf(hold,"%s",transformation_keyword)!=1)error1("prat_wavefront_read:\terror in transformation definition");

	/*
	 **	load identity matrix
	 */
	if(!bbox_Ptr->transform_flag){
		bbox_Ptr->m=load_identity_matrix4();
		bbox_Ptr->transform_flag=1;
	}

	hold=strchr(hold,transformation_keyword[0])+strlen(transformation_keyword);
	if(strcmp(transformation_keyword,"scale")==0){
#ifdef DOUBLEDEF
		if(sscanf(hold,"%lf",&scale)!=1)error1("prat_wavefront_read:\terror in scale transformation definition");
#else
		if(sscanf(hold,"%f",&scale)!=1)error1("prat_wavefront_read:\terror in scale transformation definition");
#endif
		matrix = load_scaling_matrix4(scale);
	}else if(strcmp(transformation_keyword,"transformation_matrix")==0){
		matrix = load_identity_matrix4();
#ifdef DOUBLEDEF
		if(sscanf(hold,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&matrix.data[0][0],&matrix.data[1][0],&matrix.data[2][0],&matrix.data[0][1],&matrix.data[1][1],&matrix.data[2][1],&matrix.data[0][2],&matrix.data[1][2],&matrix.data[2][2],&matrix.data[0][3],&matrix.data[1][3],&matrix.data[2][3])!=12)error1("prat_wavefront_read:\terror in transformation_matrix definition");
	}else if(strcmp(transformation_keyword,"scale_differential")==0){
		if(sscanf(hold,"%lf %lf %lf",&fix_point.x,&fix_point.y,&fix_point.z)!=3)error1("prat_wavefront_read:\terror in scale_differential transformation definition");
		matrix = load_differential_scaling_matrix4(fix_point);
	}else if(strcmp(transformation_keyword,"translate")==0){
		if(sscanf(hold,"%lf %lf %lf",&translation.x,&translation.y,&translation.z)!=3)error1("prat_wavefront_read:\terror in translation transformation definition");
		matrix = load_translation_matrix4(translation);
	}else if(strcmp(transformation_keyword,"rotate_about_x_axis")==0){
		if(sscanf(hold,"%lf",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_x_axis transformation definition");
		matrix = load_x_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_y_axis")==0){
		if(sscanf(hold,"%lf",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_y_axis transformation definition");
		matrix = load_y_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_z_axis")==0){
		if(sscanf(hold,"%lf",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_z_axis transformation definition");
		matrix = load_z_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_arbitrary_axis")==0){
		if(sscanf(hold,"%lf %lf %lf %lf %lf %lf %lf",&axis.x,&axis.y,&axis.z,&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=7)error1("prat_wavefront_read:\terror in rotate_about_arbitrary_axis transformation definition");
	}else if(strcmp(transformation_keyword,"scale_fix_point")==0){
		if(sscanf(hold,"%lf %lf %lf %lf",&scale,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in scale_fix_point transformation definition");
		matrix = load_scaling_fix_point_matrix4(scale,fix_point);
	}else if(strcmp(transformation_keyword,"scale_differential_fix_point")==0){
		if(sscanf(hold,"%lf %lf %lf %lf %lf %lf",&d_scale.x,&d_scale.y,&d_scale.z,&fix_point.x,&fix_point.y,&fix_point.z)!=6)error1("prat_wavefront_read:\terror in scale_differential_fix_point transformation definition");
		matrix = load_differential_scaling_fix_point_matrix4(d_scale,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_x_axis_fix_point")==0){
		if(sscanf(hold,"%lf %lf %lf %lf",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_x_axis_fix_point transformation definition");
		matrix = load_x_axis_rotation_fix_point_matrix4(theta,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_y_axis_fix_point")==0){
		if(sscanf(hold,"%lf %lf %lf %lf",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_y_axis_fix_point transformation definition");
		matrix = load_y_axis_rotation_fix_point_matrix4(theta,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_z_axis_fix_point")==0){
		if(sscanf(hold,"%lf %lf %lf %lf",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_z_axis_fix_point transformation definition");
#else
		if(sscanf(hold,"%f %f %f %f %f %f %f %f %f %f %f %f",&matrix.data[0][0],&matrix.data[1][0],&matrix.data[2][0],&matrix.data[0][1],&matrix.data[1][1],&matrix.data[2][1],&matrix.data[0][2],&matrix.data[1][2],&matrix.data[2][2],&matrix.data[0][3],&matrix.data[1][3],&matrix.data[2][3])!=12)error1("prat_wavefront_read:\terror in transformation_matrix definition");
	}else if(strcmp(transformation_keyword,"scale_differential")==0){
		if(sscanf(hold,"%f %f %f",&fix_point.x,&fix_point.y,&fix_point.z)!=3)error1("prat_wavefront_read:\terror in scale_differential transformation definition");
		matrix = load_differential_scaling_matrix4(fix_point);
	}else if(strcmp(transformation_keyword,"translate")==0){
		if(sscanf(hold,"%f %f %f",&translation.x,&translation.y,&translation.z)!=3)error1("prat_wavefront_read:\terror in translation transformation definition");
		matrix = load_translation_matrix4(translation);
	}else if(strcmp(transformation_keyword,"rotate_about_x_axis")==0){
		if(sscanf(hold,"%f",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_x_axis transformation definition");
		matrix = load_x_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_y_axis")==0){
		if(sscanf(hold,"%f",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_y_axis transformation definition");
		matrix = load_y_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_z_axis")==0){
		if(sscanf(hold,"%f",&theta)!=1)error1("prat_wavefront_read:\terror in rotate_about_z_axis transformation definition");
		matrix = load_z_axis_rotation_matrix4(theta);
	}else if(strcmp(transformation_keyword,"rotate_about_arbitrary_axis")==0){
		if(sscanf(hold,"%f %f %f %f %f %f %f",&axis.x,&axis.y,&axis.z,&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=7)error1("prat_wavefront_read:\terror in rotate_about_arbitrary_axis transformation definition");
	}else if(strcmp(transformation_keyword,"scale_fix_point")==0){
		if(sscanf(hold,"%f %f %f %f",&scale,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in scale_fix_point transformation definition");
		matrix = load_scaling_fix_point_matrix4(scale,fix_point);
	}else if(strcmp(transformation_keyword,"scale_differential_fix_point")==0){
		if(sscanf(hold,"%f %f %f %f %f %f",&d_scale.x,&d_scale.y,&d_scale.z,&fix_point.x,&fix_point.y,&fix_point.z)!=6)error1("prat_wavefront_read:\terror in scale_differential_fix_point transformation definition");
		matrix = load_differential_scaling_fix_point_matrix4(d_scale,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_x_axis_fix_point")==0){
		if(sscanf(hold,"%f %f %f %f",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_x_axis_fix_point transformation definition");
		matrix = load_x_axis_rotation_fix_point_matrix4(theta,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_y_axis_fix_point")==0){
		if(sscanf(hold,"%f %f %f %f",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_y_axis_fix_point transformation definition");
		matrix = load_y_axis_rotation_fix_point_matrix4(theta,fix_point);
	}else if(strcmp(transformation_keyword,"rotate_about_z_axis_fix_point")==0){
		if(sscanf(hold,"%f %f %f %f",&theta,&fix_point.x,&fix_point.y,&fix_point.z)!=4)error1("prat_wavefront_read:\terror in rotate_about_z_axis_fix_point transformation definition");
#endif
		matrix = load_z_axis_rotation_fix_point_matrix4(theta,fix_point);
	}

	bbox_Ptr->m		      = multiply_matrix4_matrix4(matrix,bbox_Ptr->m);
	bbox_Ptr->m_inverse_transpose = inverse_matrix4(bbox_Ptr->m);
	bbox_Ptr->m_inverse 	      = transpose4(bbox_Ptr->m_inverse_transpose);

	/*	*m_inv_reverse_Ptr=multiply_matrix4_matrix4(bbox_Ptr->m_inverse, *m_inv_reverse_Ptr);

	 *m_inverse_fwd_Ptr=multiply_matrix4_matrix4( *m_inverse_fwd_Ptr,bbox_Ptr->m);

	 bbox_Ptr->m_inverse = multiply_matrix4_matrix4( *m_inverse_fwd_Ptr,multiply_matrix4_matrix4(bbox_Ptr->m_inverse, *m_inv_reverse_Ptr));
	 bbox_Ptr->m_inverse_transpose = transpose4(bbox_Ptr->m_inverse);
	 */
	return(1);
}

/*
 **	read vertex info
 */
int	vertex_read(line,vertices,normals,locals,normal,local,vertexStore)
	char	 *line;
	VeRtIcEs *vertices,*normals, *locals;
	int	 normal,local,vertexStore;
{	
	triplet	*trip,vertex;
	int	block,vertex_in_block,vertexNumber;

#ifdef DOUBLEDEF
	if(sscanf(line,"%lf %lf %lf",&vertex.x,&vertex.y,&vertex.z)!=3)error1("error in vertex specification");
#else
	if(sscanf(line,"%f %f %f",&vertex.x,&vertex.y,&vertex.z)!=3)error1("error in vertex specification");
#endif
	if(vertexStore){
		vertexNumber=(vertices->no_of_vertices)%vertexStore+1;
	}else{
		vertexNumber=(vertices->no_of_vertices)+1;
	}
	vertex_to_block(&block,&vertex_in_block,vertexNumber,vertices->blocksize);
	if(vertices->new[vertices->block]==1){
		vertices->new[vertices->block]=0;
		vertices->vertices[vertices->block]=t_allocate(vertices->blocksize);
		if(normal){
			normals->new[vertices->block]=0;
			normals->vertices[vertices->block]=t_allocate(vertices->blocksize);
		}
		if(local){
			locals->new[vertices->block]=0;
			locals->vertices[vertices->block]=t_allocate(vertices->blocksize);
		}
	}else if(vertex_in_block==0){
		vertices->block++;
		vertices->vertices[vertices->block]=t_allocate(vertices->blocksize);
		if(normal){
			normals->block++;
			normals->vertices[vertices->block]=t_allocate(vertices->blocksize);
		}
		if(local){
			locals->block++;
			locals->vertices[vertices->block]=t_allocate(vertices->blocksize);
		}
	}
	trip=vertices->vertices[vertices->block];
	trip[vertex_in_block]=vertex;
	vertices->no_of_vertices++;
	return(1);
}

/*
 **	read vertex normal info
 */
int	normal_read(line,vertices,normals,normal,vertexStore)
	char	 *line;
	VeRtIcEs *vertices,*normals;
	int	 normal,vertexStore;
{	
	char	*hold,tmp[100];
	triplet	*trip,vertex;
	int	block,vertex_in_block,vertex_no,atoi(),vertexNumber;

	if(!normal)return(0);

	hold=line;
	/*
	 **	read vertex pointer and normal info
	 */	
#ifdef DOUBLEDEF
	if(sscanf(hold,"%s %lf %lf %lf",tmp,&vertex.x,&vertex.y,&vertex.z)!=4)error1("error in vertex normal specification");
#else
	if(sscanf(hold,"%s %f %f %f",tmp,&vertex.x,&vertex.y,&vertex.z)!=4)error1("error in vertex normal specification");
#endif
	vertex_no=atoi(tmp);


	/* relative */
	if(vertex_no<0)
		vertex_no+=vertices->no_of_vertices+1;
	if(vertexStore){
		vertexNumber=(vertex_no-1)%vertexStore+1;
	}else{
		vertexNumber=(vertex_no)+1;
	}
	/*
	 **	find current block
	 */
	vertex_to_block(&block,&vertex_in_block,vertexNumber,vertices->blocksize);

	trip=normals->vertices[block];
	trip[vertex_in_block]=normalise(vertex);
	return(1);
}

/*
 **	read vertex normal info
 */
int	local_coord_read(line,vertices,locals,local,vertexStore)
	char	 *line;
	VeRtIcEs *vertices,*locals;
	int	 local,vertexStore;
{	
	char	*hold,tmp[100];
	triplet	*trip,vertex;
	int	block,vertex_in_block,vertex_no,atoi(),vertexNumber;

	if(!local)return(0);

	hold=line;
	/*
	 **	read vertex pointer and local info
	 */	
#ifdef DOUBLEDEF
	if(sscanf(hold,"%s %lf %lf",tmp,&vertex.x,&vertex.y)!=3)error1("error in vertex local_coordinate specification");
#else
	if(sscanf(hold,"%s %f %f",tmp,&vertex.x,&vertex.y)!=3)error1("error in vertex local_coordinate specification");
#endif
	vertex_no=atoi(tmp);

	/* relative */
	if(vertex_no<0)
		vertex_no+=vertices->no_of_vertices+1;
	if(vertexStore){
		vertexNumber=(vertex_no-1)%vertexStore+1;
	}else{
		vertexNumber=(vertex_no)+1;
	}
	/*
	 **	find current block
	 */
	vertex_to_block(&block,&vertex_in_block,vertexNumber,vertices->blocksize);

	trip=locals->vertices[block];
	trip[vertex_in_block]=vertex;
	return(1);
}

void	add_total_box_contents(total_box_contents_Ptr,contents_Ptr)
	Contents	*total_box_contents_Ptr,*contents_Ptr;
{
	contents_Ptr->no_of_clones += total_box_contents_Ptr->no_of_clones;
	contents_Ptr->no_of_triangles += total_box_contents_Ptr->no_of_triangles;
	contents_Ptr->no_of_beziers += total_box_contents_Ptr->no_of_beziers;
	contents_Ptr->no_of_spheres += total_box_contents_Ptr->no_of_spheres;
	contents_Ptr->no_of_cylinders += total_box_contents_Ptr->no_of_cylinders;
	contents_Ptr->no_of_ellipses += total_box_contents_Ptr->no_of_ellipses;
	contents_Ptr->no_of_spheroids += total_box_contents_Ptr->no_of_spheroids;

	return;
}


int	type_of_element(option)
	char	*option;
{
	if(strcmp(option,SPHER)==0)return(SPHER_RET);
	if(strcmp(option,OPEN_BBOX)==0)return(OPEN_BBOX_RET);
	if(strcmp(option,CLOSE_BBOX)==0)return(CLOSE_BBOX_RET);
	if(strcmp(option,GROUP)==0)return(GROUP_RET);
	if(strcmp(option,VERTEX)==0)return(VERTEX_RET);
	if(strcmp(option,LOCAL)==0)return(LOCAL_RET);
	if(strcmp(option,NORMAL1)==0||strcmp(option,NORMAL2)==0)return(NORMAL_RET);
	if(strcmp(option,DEFINE1)==0||strcmp(option,DEFINE2)==0)return(DEFINE_RET);
	if(strcmp(option,TRIANGLE1)==0||strcmp(option,TRIANGLE2)==0)
		return(TRIANGLE_RET);
	if(strcmp(option,SPH)==0)return(SPHERE_RET);
	if(strcmp(option,VOLUME_SP)==0)return(VOLUME_SPHERE_RET);
	if(strcmp(option,CYL)==0)return(CYLINDER_RET);
	if(strcmp(option,VOLUME_CYL)==0)return(VOLUME_CYL_RET);
	if(strcmp(option,CLOSED_CYL)==0)return(CLOSED_CYL_RET);
	if(strcmp(option,ELL)==0)return(ELLIPSE_RET);
	if(strcmp(option,VOLUME_ELLIPSE)==0)return(VOLUME_ELLIPSE_RET);
	if(strcmp(option,DISK1)==0||strcmp(option,DISK2)==0)return(DISK_RET);
	if(strcmp(option,DEM1)==0||strcmp(option,DEM2)==0||strcmp(option,DEM3)==0||strcmp(option,DEM4)==0||strcmp(option,DEM5)==0||strcmp(option,DEM6)==0)return(DEM_RET);
	if(strcmp(option,SPHERICAL_DEM1)==0||strcmp(option,SPHERICAL_DEM2)==0||strcmp(option,SPHERICAL_DEM3)==0||strcmp(option,SPHERICAL_DEM4)==0||strcmp(option,SPHERICAL_DEM5)==0||strcmp(option,SPHERICAL_DEM6)==0)return(SPHERICAL_DEM_RET);
	if(strcmp(option,PLANE1)==0||strcmp(option,PLANE2)==0||strcmp(option,PLANE3)==0||strcmp(option,PLANE4)==0||strcmp(option,PLANE5)==0||strcmp(option,PLANE6)==0)return(PLANE_RET);
	if(strcmp(option,USEMTL)==0)return(USEMTL_RET);
	if(strcmp(option,MTLLIB)==0)return(MTLLIB_RET);
	if(strcmp(option,TRANS)==0)return(TRANS_RET);
	if(strcmp(option,CLONE1)==0||strcmp(option,CLONE2)==0||strcmp(option,CLONE3)==0)return(CLONE_RET);
	if(strcmp(option,BILINEARPATCH0)==0||strcmp(option,BILINEARPATCH1)==0||strcmp(option,BILINEARPATCH2)==0||strcmp(option,BILINEARPATCH3)==0||strcmp(option,BILINEARPATCH4)==0||strcmp(option,BILINEARPATCH5)==0||strcmp(option,BILINEARPATCH6)==0||strcmp(option,BILINEARPATCH7)==0||strcmp(option,BILINEARPATCH8)==0)return(BILINEARPATCH_RET);
	return(UNKNOWN_TYPE);
}


Dem	*find_dem(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Dem	*current_dem,*prev_dem;
	int	first_time=1;

	current_dem=bbox_Ptr->contents.dem;
	prev_dem=NULL;
	/* consider first dem object in list */
	while(current_dem!=NULL){
		prev_dem=current_dem;
		current_dem=current_dem->next_dem;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_dem->next_dem=(Dem *)calloc(1,sizeof(Dem))))error1("find_dem:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_dems)++;
		return(prev_dem->next_dem);
	}else{
		if(!(bbox_Ptr->contents.dem=(Dem *)calloc(1,sizeof(Dem))))error1("find_dem:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_dems)++;
		return(bbox_Ptr->contents.dem);
	}
}

Spherical_Dem	*find_spherical_dem(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Spherical_Dem	*current_dem,*prev_dem;
	int	first_time=1;

	current_dem=bbox_Ptr->contents.spherical_dem;
	prev_dem=NULL;
	/* consider first dem object in list */
	while(current_dem!=NULL){
		prev_dem=current_dem;
		current_dem=current_dem->next_dem;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_dem->next_dem=(Spherical_Dem *)calloc(1,sizeof(Spherical_Dem))))error1("find_spherical_dem:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spherical_dems)++;
		return(prev_dem->next_dem);
	}else{
		if(!(bbox_Ptr->contents.spherical_dem=(Spherical_Dem *)calloc(1,sizeof(Spherical_Dem))))error1("find_spherical_dem:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spherical_dems)++;
		return(bbox_Ptr->contents.spherical_dem);
	}
}

Cylinder	*find_cylinder(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Cylinder	*current_cylinder,*prev_cylinder;
	int	first_time=1;

	current_cylinder=bbox_Ptr->contents.cylinder;
	prev_cylinder=NULL;
	/* consider first cylinder object in list */
	while(current_cylinder!=NULL){
		prev_cylinder=current_cylinder;
		current_cylinder=current_cylinder->next_cylinder;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_cylinder->next_cylinder=(Cylinder *)calloc(1,sizeof(Cylinder))))error1("find_cylinder:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_cylinders)++;
		return(prev_cylinder->next_cylinder);
	}else{
		if(!(bbox_Ptr->contents.cylinder=(Cylinder *)calloc(1,sizeof(Cylinder))))error1("find_cylinder:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_cylinders)++;
		return(bbox_Ptr->contents.cylinder);
	}
}

Spheroid *find_spheroid(bbox_Ptr)
	BBox    *bbox_Ptr;
{
	Spheroid *current_spheroid,*prev_spheroid;
	int     first_time=1;

	current_spheroid=bbox_Ptr->contents.spheroid;
	prev_spheroid=NULL;
	/* consider first spheroid object in list */
	while(current_spheroid!=NULL){
		prev_spheroid=current_spheroid;
		current_spheroid=current_spheroid->next_spheroid;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_spheroid->next_spheroid=(Spheroid *)calloc(1,sizeof(Spheroid))))error1("find_spheroid:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spheroids)++;
		return(prev_spheroid->next_spheroid);
	}else{
		if(!(bbox_Ptr->contents.spheroid=(Spheroid *)calloc(1,sizeof(Spheroid))))error1("find_spheroid:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spheroids)++;
		return(bbox_Ptr->contents.spheroid);
	}
}

Ellipse	*find_ellipse(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Ellipse	*current_ellipse,*prev_ellipse;
	int	first_time=1;

	current_ellipse=bbox_Ptr->contents.ellipse;
	prev_ellipse=NULL;
	/* consider first ellipse object in list */
	while(current_ellipse!=NULL){
		prev_ellipse=current_ellipse;
		current_ellipse=current_ellipse->next_ellipse;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_ellipse->next_ellipse=(Ellipse *)calloc(1,sizeof(Ellipse))))error1("find_ellipse:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_ellipses)++;
		return(prev_ellipse->next_ellipse);
	}else{
		if(!(bbox_Ptr->contents.ellipse=(Ellipse *)calloc(1,sizeof(Ellipse))))error1("find_ellipse:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_ellipses)++;
		return(bbox_Ptr->contents.ellipse);
	}
}

fFacet	*find_facet(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	fFacet	*current_triangle,*prev_triangle;
	int	first_time=1;

	current_triangle=bbox_Ptr->contents.ttriangle;
	prev_triangle=NULL;
	/* consider first triangle object in list */
	while(current_triangle!=NULL){
		prev_triangle=current_triangle;
		current_triangle=current_triangle->next_facet;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_triangle->next_facet=(fFacet *)calloc(1,sizeof(fFacet))))error1("find_facet:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_triangles)++;
		return(prev_triangle->next_facet);
	}else{
		if(!(bbox_Ptr->contents.ttriangle=(fFacet *)calloc(1,sizeof(fFacet))))error1("find_facet:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_triangles)++;
		return(bbox_Ptr->contents.ttriangle);
	}
}

fFacet	*delete_facet(BBox *bbox_Ptr,fFacet *fo){
	fFacet	*current_triangle,*prev_triangle;
	int	first_time=1;

	current_triangle=bbox_Ptr->contents.ttriangle;
	prev_triangle=NULL;
	/* if its the 1st one, set to NULL */
	if(bbox_Ptr->contents.ttriangle==fo)bbox_Ptr->contents.ttriangle=NULL;
	/* consider first triangle object in list */
	while(current_triangle!=NULL){
		if(current_triangle == fo){
			/* delete it if found */
			if(prev_triangle)prev_triangle->next_facet=fo->next_facet;
			(bbox_Ptr->contents.no_of_triangles)--;
			free(fo);
			fprintf(stderr,"facet deleted\n");
			return(NULL);
		}
		prev_triangle=current_triangle;
		current_triangle=current_triangle->next_facet;
		first_time=0;
	}
	/* return it if you dont find it */
	return(fo);
}

BiLinearPatch	*find_blp(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	BiLinearPatch	*current_blp,*prev_blp;
	int	first_time=1;

	current_blp=bbox_Ptr->contents.blp;
	prev_blp=NULL;
	/* consider first blp object in list */
	while(current_blp!=NULL){
		prev_blp=current_blp;
		current_blp=current_blp->next_BiLinearPatch;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_blp->next_BiLinearPatch=(BiLinearPatch *)calloc(1,sizeof(BiLinearPatch))))error1("find_BiLinearPatch:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_blps)++;
		return(prev_blp->next_BiLinearPatch);
	}else{
		if(!(bbox_Ptr->contents.blp=(BiLinearPatch *)calloc(1,sizeof(BiLinearPatch))))error1("find_BiLinearPatch:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_blps)++;
		return(bbox_Ptr->contents.blp);
	}
}

Plane	*find_plane(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Plane	*current_plane,*prev_plane;
	int	first_time=1;

	current_plane=bbox_Ptr->contents.plane;
	prev_plane=NULL;
	/* consider first plane object in list */
	while(current_plane!=NULL){
		prev_plane=current_plane;
		current_plane=current_plane->next_plane;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_plane->next_plane=(Plane *)calloc(1,sizeof(Plane))))error1("find_plane:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_planes)++;
		return(prev_plane->next_plane);
	}else{
		if(!(bbox_Ptr->contents.plane=(Plane *)calloc(1,sizeof(Plane))))error1("find_plane:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_planes)++;
		return(bbox_Ptr->contents.plane);
	}
}

Sphere	*find_sphere(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Sphere	*current_sphere,*prev_sphere;
	int	first_time=1;

	current_sphere=bbox_Ptr->contents.sphere;
	prev_sphere=NULL;
	/* consider first sphere object in list */
	while(current_sphere!=NULL){
		prev_sphere=current_sphere;
		current_sphere=current_sphere->next_sphere;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_sphere->next_sphere=(Sphere *)calloc(1,sizeof(Sphere))))error1("find_sphere:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spheres)++;
		return(prev_sphere->next_sphere);
	}else{
		if(!(bbox_Ptr->contents.sphere=(Sphere *)calloc(1,sizeof(Sphere))))error1("find_sphere:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_spheres)++;
		return(bbox_Ptr->contents.sphere);
	}
}

Disk	*find_disk(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Disk	*current_disk,*prev_disk;
	int	first_time=1;

	current_disk=bbox_Ptr->contents.disk;
	prev_disk=NULL;
	/* consider first disk object in list */
	while(current_disk!=NULL){
		prev_disk=current_disk;
		current_disk=current_disk->next_disk;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_disk->next_disk=(Disk *)calloc(1,sizeof(Disk))))error1("find_disk:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_disks)++;
		return(prev_disk->next_disk);
	}else{
		if(!(bbox_Ptr->contents.disk=(Disk *)calloc(1,sizeof(Disk))))error1("find_disk:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_disks)++;
		return(bbox_Ptr->contents.disk);
	}
}

Clones	*find_clone(bbox_Ptr)
	BBox	*bbox_Ptr;
{
	Clones	*current_clone,*prev_clone;
	int	first_time=1;

	current_clone=bbox_Ptr->contents.clone;
	prev_clone=NULL;
	/* consider first clone object in list */
	while(current_clone!=NULL){
		prev_clone=current_clone;
		current_clone=current_clone->next_clone;
		first_time=0;
	}
	if(!first_time){
		if(!(prev_clone->next_clone=(Clones *)calloc(1,sizeof(Clones))))error1("find_clone:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_clones)++;
		return(prev_clone->next_clone);
	}else{
		if(!(bbox_Ptr->contents.clone=(Clones *)calloc(1,sizeof(Clones))))error1("find_clone:\tmemory allocation error");
		(bbox_Ptr->contents.no_of_clones)++;
		return(bbox_Ptr->contents.clone);
	}
}

int	find_current_hit(out,timer_Ptr,base_hit_Ptr)
	Contents_order	*base_hit_Ptr[6];
	int		*timer_Ptr;
	Contents_order	*out[6];
{
	int	i;

	for(i=0;i<6;i++){
		if(!(out[i]=(Contents_order *)calloc(1+100,sizeof(Contents_order))))error1("find_current_hit:\tmemory allocation error");
		if(*timer_Ptr==0)base_hit_Ptr[i]=out[i];

	}
	(*timer_Ptr)++;
	return(1);
}

#ifdef MATCHK

void	check_material(current_mtl)
	Material_table	*current_mtl;
{
	if(current_mtl->no_of_materials > 256)
		error1("mtllib:\terror in facet material definition");
	return;
}

#endif

/* function to approximate contents of a bbox */
int approximateContents(bbox_Ptr,angleToler,distanceTol,sizeTol,deleted)
	BBox	*bbox_Ptr;
	double  angleToler,distanceTol,sizeTol;
	void **deleted;
{
	int i,j,k,l,minNumber,retval=0;
	Cylinder *cylinders[2],*prevCylinder;
	triplet normalise(),p[4],vector_plus(),V_factor(),vector_minus();
	double angleTol,V_mod(),V_dot(),d[4],minDistance,minAngle;
	BBox *bbox[2],*prevBBox;
	struct Clone *clones[2];
	fFacet *triangles[2];
	Bezier4 *beziers[2];
	Sphere *spheres[2];
	Ellipse *ellipses[2];
	Spheroid *spheroids[2];
	Disk *disks[2];
	Dem *dems[2];
	Spherical_Dem *spherical_dems[2];
	Plane *planes[2];
	BiLinearPatch *blps[2];
	int sum,timer=2,found;
	Contents_order *current_hit[6],*hit,*prevHit;
	double	bounds[6];
	int  sort_in_link_list();
	/* see if any bboxes are empty */
	*deleted=NULL;
	prevBBox=NULL;
	bbox[0]=bbox_Ptr->contents.bbox;
	for(i=0;i<bbox_Ptr->contents.no_of_bboxes;i++){
		if(!bbox[0]){
			i=bbox_Ptr->contents.no_of_bboxes;
		}else{
			sum=bbox[0]->contents.no_of_bboxes+bbox[0]->contents.no_of_clones +bbox[0]->contents.no_of_triangles +bbox[0]->contents.no_of_beziers +bbox[0]->contents.no_of_spheres +bbox[0]->contents.no_of_cylinders +bbox[0]->contents.no_of_ellipses + bbox[0]->contents.no_of_spheroids + bbox[0]->contents.no_of_disks +bbox[0]->contents.no_of_dems +bbox[0]->contents.no_of_spherical_dems +bbox[0]->contents.no_of_planes + bbox[0]->contents.no_of_blps;
			if(sum==0){
				/* delete */
				if(prevBBox==NULL){
					bbox_Ptr->contents.bbox=bbox_Ptr->contents.bbox->next_bbox;
				}else{
					prevBBox->next_bbox=bbox[0]->next_bbox;	
				}
				bbox_Ptr->contents.no_of_bboxes--;	
				free((void *)bbox[0]);
				*deleted=(void *)bbox[0];
				return(1);
			}
			prevBBox=bbox[0];
			bbox[0]=bbox[0]->next_bbox;
		}
	}
	angleTol=cos(DTOR(angleToler));
	for(i=0;i<2;i++){
		clones[i]=NULL;
		triangles[i]=NULL;
		cylinders[i]=NULL;
		beziers[i]=NULL;
		spheres[i]=NULL;
		ellipses[i]=NULL;
		spheroids[i]=NULL;
		disks[i]=NULL;
		dems[i]=NULL;
		spherical_dems[i]=NULL;
		planes[i]=NULL;
		blps[i]=NULL;
	}
	prevBBox=bbox_Ptr->contents.bbox;
	bbox[0]=bbox_Ptr->contents.bbox;
	for(i=0;i<bbox_Ptr->contents.no_of_bboxes-1;i++){
		/* if bboxes are the same space, combine */    
		/* if bboxes are essentially the same, attempt to combine them */
		bbox[1]=bbox[0]->next_bbox;
		p[0]=bbox[0]->min;
		p[1]=bbox[0]->max;
		/* compare each bbox with every other */
		for(j=i+1;j<bbox_Ptr->contents.no_of_bboxes;j++){
			if(! bbox[0] || ! bbox[1] ){
				j=i=bbox_Ptr->contents.no_of_bboxes;
			}else{
				p[2]=bbox[1]->min;
				p[3]=bbox[1]->max;
				if((d[0]=V_mod(vector_minus(p[0],p[2])))<=distanceTol&&(d[1]=V_mod(vector_minus(p[1],p[3])))<=distanceTol){
					retval++;
					bbox[0]->min=vmin(bbox[0]->min,bbox[1]->min);
					bbox[0]->max=vmax(bbox[0]->max,bbox[1]->max);
					bbox_Ptr->contents.no_of_bboxes--;
					/* clones */
					bbox[0]->contents.no_of_clones+=bbox[1]->contents.no_of_clones;
					if(bbox[0]->contents.no_of_clones){
						clones[0]=bbox[0]->contents.clone;
						if(clones[0]){
							clones[1]=clones[0]->next_clone;
							while(clones[1]){
								clones[0]=clones[1];
								clones[1]=clones[0]->next_clone;
							}
						}
					}
					if(clones[0]){
						clones[0]->next_clone=bbox[1]->contents.clone;
					}else{
						bbox[0]->contents.clone=bbox[1]->contents.clone;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.clone){
						/* find clone in bbox[1]->hit */	    
						clones[0]=bbox[1]->contents.clone;
						while(clones[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)clones[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							clones[0]=clones[0]->next_clone;
						}
					}
					/* triangles */
					bbox[0]->contents.no_of_triangles+=bbox[1]->contents.no_of_triangles;
					if(bbox[0]->contents.no_of_triangles){
						triangles[0]=bbox[0]->contents.ttriangle;
						if(triangles[0]){
							triangles[1]=triangles[0]->next_facet;
							while(triangles[1]){
								triangles[0]=triangles[1];
								triangles[1]=triangles[0]->next_facet;
							}
						}
					}
					if(triangles[0]){
						triangles[0]->next_facet=bbox[1]->contents.ttriangle;
					}else{
						bbox[0]->contents.ttriangle=bbox[1]->contents.ttriangle;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.ttriangle){
						/* find clone in bbox[1]->hit */	    
						triangles[0]=bbox[1]->contents.ttriangle;
						while(triangles[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)triangles[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							triangles[0]=triangles[0]->next_facet;
						}
					}
					/* bezier4 */
					bbox[0]->contents.no_of_beziers+=bbox[1]->contents.no_of_beziers;
					if(bbox[0]->contents.no_of_beziers){
						beziers[0]=bbox[0]->contents.bezier;
						if(beziers[0]){
							beziers[1]=beziers[0]->next_bezier4;
							while(beziers[1]){
								beziers[0]=beziers[1];
								beziers[1]=beziers[0]->next_bezier4;
							}
						}
					}
					if(beziers[0]){
						beziers[0]->next_bezier4=bbox[1]->contents.bezier;
					}else{
						bbox[0]->contents.bezier=bbox[1]->contents.bezier;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.bezier){
						/* find clone in bbox[1]->hit */	    
						beziers[0]=bbox[1]->contents.bezier;
						while(beziers[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)beziers[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							beziers[0]=beziers[0]->next_bezier4;
						}
					}
					/* sphere */
					bbox[0]->contents.no_of_spheres+=bbox[1]->contents.no_of_spheres;
					if(bbox[0]->contents.no_of_spheres){
						spheres[0]=bbox[0]->contents.sphere;
						if(spheres[0]){
							spheres[1]=spheres[0]->next_sphere;
							while(spheres[1]){
								spheres[0]=spheres[1];
								spheres[1]=spheres[0]->next_sphere;
							}
						}
					}
					if(spheres[0]){
						spheres[0]->next_sphere=bbox[1]->contents.sphere;
					}else{
						bbox[0]->contents.sphere=bbox[1]->contents.sphere;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.sphere){
						/* find clone in bbox[1]->hit */	    
						spheres[0]=bbox[1]->contents.sphere;
						while(spheres[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)spheres[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							spheres[0]=spheres[0]->next_sphere;
						}
					}
					/* cylinder */
					bbox[0]->contents.no_of_cylinders+=bbox[1]->contents.no_of_cylinders;
					if(bbox[0]->contents.no_of_cylinders){
						cylinders[0]=bbox[0]->contents.cylinder;
						if(cylinders[0]){
							cylinders[1]=cylinders[0]->next_cylinder;
							while(cylinders[1]){
								cylinders[0]=cylinders[1];
								cylinders[1]=cylinders[0]->next_cylinder;
							}
						}
					}
					if(cylinders[0]){
						cylinders[0]->next_cylinder=bbox[1]->contents.cylinder;
					}else{
						bbox[0]->contents.cylinder=bbox[1]->contents.cylinder;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.cylinder){
						/* find clone in bbox[1]->hit */	    
						cylinders[0]=bbox[1]->contents.cylinder;
						while(cylinders[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)cylinders[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							cylinders[0]=cylinders[0]->next_cylinder;
						}
					}

					/* ellipse */
					bbox[0]->contents.no_of_ellipses+=bbox[1]->contents.no_of_ellipses;
					if(bbox[0]->contents.no_of_ellipses){
						ellipses[0]=bbox[0]->contents.ellipse;
						if(ellipses[0]){
							ellipses[1]=ellipses[0]->next_ellipse;
							while(ellipses[1]){
								ellipses[0]=ellipses[1];
								ellipses[1]=ellipses[0]->next_ellipse;
							}
						}
					}
					if(ellipses[0]){
						ellipses[0]->next_ellipse=bbox[1]->contents.ellipse;
					}else{
						bbox[0]->contents.ellipse=bbox[1]->contents.ellipse;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.ellipse){
						/* find clone in bbox[1]->hit */	    
						ellipses[0]=bbox[1]->contents.ellipse;
						while(ellipses[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)ellipses[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							ellipses[0]=ellipses[0]->next_ellipse;
						}
					}


					/* spheroid */
					bbox[0]->contents.no_of_spheroids+=bbox[1]->contents.no_of_spheroids;
					if(bbox[0]->contents.no_of_spheroids){
						spheroids[0]=bbox[0]->contents.spheroid;
						if(spheroids[0]){
							spheroids[1]=spheroids[0]->next_spheroid;
							while(spheroids[1]){
								spheroids[0]=spheroids[1];
								spheroids[1]=spheroids[0]->next_spheroid;
							}
						}
					}
					if(spheroids[0]){
						spheroids[0]->next_spheroid=bbox[1]->contents.spheroid;
					}else{
						bbox[0]->contents.spheroid=bbox[1]->contents.spheroid;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.spheroid){
						/* find clone in bbox[1]->hit */
						spheroids[0]=bbox[1]->contents.spheroid;
						while(spheroids[0]){
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)spheroids[0]){
										found=1;
									}
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							spheroids[0]=spheroids[0]->next_spheroid;
						}
					}

					/* disk */
					bbox[0]->contents.no_of_disks+=bbox[1]->contents.no_of_disks;
					if( bbox[0]->contents.no_of_disks){	   
						disks[0]=bbox[0]->contents.disk;
						if(disks[0]){
							disks[1]=disks[0]->next_disk;
							while(disks[1]){
								disks[0]=disks[1];
								disks[1]=disks[0]->next_disk;
							}
						}
					}
					if(disks[0]){
						disks[0]->next_disk=bbox[1]->contents.disk;
					}else{
						bbox[0]->contents.disk=bbox[1]->contents.disk;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.disk){
						/* find clone in bbox[1]->hit */	    
						disks[0]=bbox[1]->contents.disk;
						while(disks[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)disks[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							disks[0]=disks[0]->next_disk;
						}
					}
					/* dem */
					bbox[0]->contents.no_of_dems+=bbox[1]->contents.no_of_dems;
					if(bbox[0]->contents.no_of_dems){
						dems[0]=bbox[0]->contents.dem;
						if(dems[0]){
							dems[1]=dems[0]->next_dem;
							while(dems[1]){
								dems[0]=dems[1];
								dems[1]=dems[0]->next_dem;
							}
						}
					}
					if(dems[0]){
						dems[0]->next_dem=bbox[1]->contents.dem;
					}else{
						bbox[0]->contents.dem=bbox[1]->contents.dem;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.dem){
						/* find clone in bbox[1]->hit */	    
						dems[0]=bbox[1]->contents.dem;
						while(dems[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)dems[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							dems[0]=dems[0]->next_dem;
						}
					}
					/* spherical_dem */
					bbox[0]->contents.no_of_spherical_dems+=bbox[1]->contents.no_of_spherical_dems;
					if(bbox[0]->contents.no_of_spherical_dems){
						spherical_dems[0]=bbox[0]->contents.spherical_dem;
						if(spherical_dems[0]){
							spherical_dems[1]=spherical_dems[0]->next_dem;
							while(spherical_dems[1]){
								spherical_dems[0]=spherical_dems[1];
								spherical_dems[1]=spherical_dems[0]->next_dem;
							}
						}
					}
					if(spherical_dems[0]){
						spherical_dems[0]->next_dem=bbox[1]->contents.spherical_dem;
					}else{
						bbox[0]->contents.spherical_dem=bbox[1]->contents.spherical_dem;;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.spherical_dem){
						/* find clone in bbox[1]->hit */	    
						spherical_dems[0]=bbox[1]->contents.spherical_dem;
						while(spherical_dems[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)spherical_dems[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							spherical_dems[0]=spherical_dems[0]->next_dem;
						}
					}
					/* plane */
					bbox[0]->contents.no_of_planes+=bbox[1]->contents.no_of_planes;
					if(bbox[0]->contents.no_of_planes){
						planes[0]=bbox[0]->contents.plane;
						if(planes[0]){
							planes[1]=planes[0]->next_plane;
							while(planes[1]){
								planes[0]=planes[1];
								planes[1]=planes[0]->next_plane;
							}
						}
					}
					if(planes[0]){
						planes[0]->next_plane=bbox[1]->contents.plane;
					}else{
						bbox[0]->contents.plane=bbox[1]->contents.plane;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.plane){
						/* find clone in bbox[1]->hit */	    
						planes[0]=bbox[1]->contents.plane;
						while(planes[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)planes[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							planes[0]=planes[0]->next_plane;
						}
					}
					/* blp */
					bbox[0]->contents.no_of_blps+=bbox[1]->contents.no_of_blps;
					if( bbox[0]->contents.no_of_blps){
						blps[0]=bbox[0]->contents.blp;
						if(blps[0]){
							blps[1]=blps[0]->next_BiLinearPatch;
							while(blps[1]){
								blps[0]=blps[1];
								blps[1]=blps[0]->next_BiLinearPatch;
							}
						}
					}
					if(blps[0]){
						blps[0]->next_BiLinearPatch=bbox[1]->contents.blp;
					}else{
						bbox[0]->contents.blp=bbox[1]->contents.blp;
					}
					/* reprocess hits for all objects transfered from bbox[1] to bbox[0] */
					if(bbox[1]->contents.blp){
						/* find clone in bbox[1]->hit */	    
						blps[0]=bbox[1]->contents.blp;
						while(blps[0]){		
							for(l=0;l<6;l++){
								hit=bbox[1]->hit[l];
								found=0;
								while(!found&&hit){
									if((void *)(hit->hit)==(void *)blps[0]){
										found=1;
									} 
									if(!found){
										/* step through */
										hit=hit->next_hit;
									}
								}	      
								if(!found){
									fprintf(stderr,"fatal error in object restructuring (try switching off -objectTol)\n");
									exit(1);
								}
								bounds[l]=hit->D;
							}
							find_current_hit(&(current_hit[0]),&timer,NULL);
							sort_in_link_list(&(current_hit[0]),&timer,&(bbox[0]->hit[0]),&(bounds[0]),hit);
							blps[0]=blps[0]->next_BiLinearPatch;
						}
					}
					/* tidy up */
					prevBBox->next_bbox=bbox[1]->next_bbox;
					/* free the spare bbox */
					for(l=0;l<6;l++){
						hit=(bbox[1]->hit[l]);
						while(hit){
							prevHit=hit->next_hit;
							free((void *)hit);
							hit=prevHit;	    
						}
					}
					free((void *)bbox[1]);
					*deleted=(void *)bbox[1];
					return(retval);
				}
			}
			if(bbox[1]){
				bbox[1]=bbox[1]->next_bbox;
			}
			if(! bbox[0] || ! bbox[1] ){
				j=i=bbox_Ptr->contents.no_of_bboxes;
			}
		}
		bbox[0]=bbox[0]->next_bbox;
		if(!bbox[0]){
			i=bbox_Ptr->contents.no_of_bboxes;
		}
		prevBBox=bbox[0];
	}
	for(i=0;i<bbox_Ptr->contents.no_of_clones;i++){
		/* if clones are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_triangles;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_beziers;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_spheres;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}

	prevCylinder=bbox_Ptr->contents.cylinder;
	if(prevCylinder){
		cylinders[0]=prevCylinder;
		if(cylinders[0]->height<sizeTol || sqrt(cylinders[0]->r2)<sizeTol){
			bbox_Ptr->contents.cylinder=bbox_Ptr->contents.cylinder->next_cylinder;free((void *)cylinders[0]);
			bbox_Ptr->contents.no_of_cylinders--;
			*deleted=(void *)cylinders[0];
			return(1);
		}
	}
	for(i=0;i<bbox_Ptr->contents.no_of_cylinders-1;i++){
		/* if cylinders are essentially the same, attempt to combine them */
		if(cylinders[0]&& cylinders[0]->next_cylinder){
			cylinders[1]=cylinders[0]->next_cylinder;

			/* check cylinder sizes against minimum dimension */
			if(cylinders[0]->height<sizeTol || sqrt(cylinders[0]->r2)<sizeTol){
				bbox_Ptr->contents.cylinder=cylinders[1];free((void *)cylinders[0]);
				bbox_Ptr->contents.no_of_cylinders--;
				*deleted=(void *)cylinders[0];
				return(1);
			}
			p[0]=cylinders[0]->origin;
			p[1]=vector_plus(cylinders[0]->origin,V_factor(cylinders[0]->normal,cylinders[0]->height));
			/* compare each cylinder with every other */
			for(j=i+1;j<bbox_Ptr->contents.no_of_cylinders;j++){
				if(! cylinders[0] && ! cylinders[1] )j=i=bbox_Ptr->contents.no_of_cylinders;
				else{
					/* material */
					if(cylinders[0]->material==cylinders[1]->material&&((cylinders[0]->ends&&cylinders[1]->ends)||(!cylinders[0]->ends && !cylinders[1]->ends))){
						/* radius tolerance */
						if(fabs(sqrt(cylinders[0]->r2) - sqrt(cylinders[1]->r2))<=distanceTol){
							/* angle tolerance */
							minAngle=V_dot(cylinders[0]->normal,cylinders[1]->normal);
							if(fabs(minAngle)>=angleTol){
								p[2]=cylinders[1]->origin;
								p[3]=vector_plus(cylinders[1]->origin,V_factor(cylinders[1]->normal,cylinders[1]->height));
								d[0]=fabs(V_mod(vector_minus(p[0],p[2])));d[1]=fabs(V_mod(vector_minus(p[0],p[3])));d[2]=fabs(V_mod(vector_minus(p[1],p[2])));d[3]=fabs(V_mod(vector_minus(p[1],p[3])));
								minNumber= -1;
								minDistance= distanceTol*2.;
								for(k=0;k<4;k++){
									if(d[k]<=distanceTol && d[k]<minDistance){
										minDistance=d[k];minNumber=k;
									}
								}
								if(minNumber>=0){
									/* should combine objects */
									retval++;
									switch(minNumber){
										case 0:
											cylinders[0]->r2=0.5*(cylinders[0]->r2+cylinders[1]->r2);cylinders[0]->origin=p[1];cylinders[0]->height=d[3];
											if(cylinders[0]->ends){
												free((void *)&cylinders[0]->ends[0]);cylinders[0]->ends[0]=cylinders[0]->ends[1];
												free((void *)&cylinders[1]->ends[0]);cylinders[0]->ends[1]=cylinders[1]->ends[1];
											}		  
											cylinders[0]->normal=normalise(vector_minus(p[3],p[1]));
											break;
										case 1:
											cylinders[0]->r2=0.5*(cylinders[0]->r2+cylinders[1]->r2);cylinders[0]->origin=p[1];cylinders[0]->height=d[2];
											if(cylinders[0]->ends){
												free((void *)&cylinders[0]->ends[0]);cylinders[0]->ends[0]=cylinders[0]->ends[1];
												free((void *)&cylinders[1]->ends[1]);cylinders[0]->ends[1]=cylinders[1]->ends[0];
											}
											cylinders[0]->normal=normalise(vector_minus(p[2],p[1]));
											break;
										case 2:
											cylinders[0]->r2=0.5*(cylinders[0]->r2+cylinders[1]->r2);cylinders[0]->origin=p[0];cylinders[0]->height=d[1];
											if(cylinders[0]->ends){
												free((void *)&cylinders[0]->ends[1]);cylinders[0]->ends[0]=cylinders[0]->ends[0];
												free((void *)&cylinders[1]->ends[1]);cylinders[0]->ends[1]=cylinders[1]->ends[0];
											}		  
											cylinders[0]->normal=normalise(vector_minus(p[3],p[0]));
											break;
										case 3:
											cylinders[0]->r2=0.5*(cylinders[0]->r2+cylinders[1]->r2);cylinders[0]->origin=p[0];cylinders[0]->height=d[0];
											if(cylinders[0]->ends){
												free((void *)&cylinders[0]->ends[1]);cylinders[0]->ends[0]=cylinders[1]->ends[0];
												free((void *)&cylinders[1]->ends[1]);cylinders[0]->ends[1]=cylinders[1]->ends[0];
											}
											cylinders[0]->normal=normalise(vector_minus(p[2],p[0]));
											break;
									}
									prevCylinder->next_cylinder=cylinders[1]->next_cylinder;free((void *)cylinders[1]);
									(bbox_Ptr->contents.no_of_cylinders)--;
									*deleted=(void *)cylinders[1];
									return(retval);
								}
							}
						}
					}
					prevCylinder=cylinders[1];
					cylinders[1]=cylinders[1]->next_cylinder;   
					if(!cylinders[1])break;
				} 
			}
		}
		if(cylinders[0]){
			cylinders[0]=cylinders[0]->next_cylinder;
			prevCylinder=cylinders[0];
		}
		if(! cylinders[0] || ! cylinders[0]->next_cylinder ){
			i=bbox_Ptr->contents.no_of_cylinders;
		}
	}
	for(i=0;i<bbox_Ptr->contents.no_of_spheroids;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_ellipses;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_disks;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_dems;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_spherical_dems;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_planes;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	for(i=0;i<bbox_Ptr->contents.no_of_blps;i++){
		/* if  are essentially the same, attempt to combine them */
		;
	}
	*deleted=NULL;
	return(retval);
}

void loadObjectInfo(BigBag *bb,char *materialName,int objectType,double size){
	int i,whichOne=-1;
	if(!materialName)return;
	/* check to see if already loaded */
	for(i=0;i<bb->nObjectTypes;i++){
		if(bb->objectTypes[i]==objectType && bb->objectMaterialNames[i]==materialName){
			whichOne=i;
			break;
		}
	}
	if(whichOne==-1){
		/* new one */
		if(!bb->objectTypes){
			bb->nObjectTypes=0;
			bb->objectTypes=i_allocate(MAX_OBJECT_TYPES*bb->material_list.no_of_materials);
			bb->objectMaterialNames=(char **)v_allocate(MAX_OBJECT_TYPES*bb->material_list.no_of_materials,sizeof(char *));
			bb->objectTypeCapacity=MAX_OBJECT_TYPES*bb->material_list.no_of_materials;
			bb->minObjectSize=d_allocate(bb->objectTypeCapacity);
			bb->maxObjectSize=d_allocate(bb->objectTypeCapacity);
		}
		bb->objectTypes[bb->nObjectTypes]=objectType;
		bb->objectMaterialNames[bb->nObjectTypes]=materialName;
		bb->minObjectSize[bb->nObjectTypes]=bb->maxObjectSize[bb->nObjectTypes]=size;
		(bb->nObjectTypes)++;
	}else{
		/* old one */
		bb->minObjectSize[whichOne]=MIN(bb->minObjectSize[whichOne],size);
		bb->maxObjectSize[whichOne]=MAX(bb->maxObjectSize[whichOne],size);
	}
	return;
}
char *getMaterialName(BigBag *bb,int type,void *object){
	char *name=NULL;
	int matNo=-1;

	switch(type){
		case FO:
			matNo = *(((fFacet *)object)->material->material);
			break;
		case SPHERE:
		case VOLUMETRIC_SPHERE:
			matNo = *(((Sphere *)object)->material->material);
			break;
		case BILINEARPATCH:
			matNo = *(((BiLinearPatch *)object)->material->material);
			break;
		case DEM:
			matNo = *(((Dem *)object)->material->material);
			break;
		case SPHERICAL_DEM:
			matNo = *(((Spherical_Dem *)object)->material->material);
			break;
		case ELLIPSE:
		case VOLUMETRIC_ELLIPSE:
			matNo = *(((Ellipse *)object)->material->material);
			break;
		case SPHEROID:
		case VOLUMETRIC_SPHEROID:
			matNo = *(((Spheroid *)object)->material->material);
			break;
		case CYLINDER:
		case VOLUMETRIC_CYLINDER:
		case CLOSED_CYLINDER:
			matNo = *(((Cylinder *)object)->material->material);
			break;
		case DISK:
			matNo = *(((Disk *)object)->material->material);
			break;
	}
	if(matNo != -1){
		name = bb->material_name[matNo];
	}
	return(name);
}


double getObjectSize(BigBag *bb,int type,void *object){
	double size=0.0;

	switch(type){
		case FO:
			size= (((fFacet *)object)->objectSize);	
			break;
		case SPHERE:
		case VOLUMETRIC_SPHERE:
			size= (((Sphere *)object)->objectSize);	
			break;
		case BILINEARPATCH:
			size= (((BiLinearPatch *)object)->objectSize);     
			break;
		case DEM:
			size= (((Dem *)object)->objectSize);
			break;
		case SPHERICAL_DEM:
			size= (((Spherical_Dem *)object)->objectSize);
			break;
		case ELLIPSE:
		case VOLUMETRIC_ELLIPSE:
			size= (((Ellipse *)object)->objectSize);
			break;
		case SPHEROID:
		case VOLUMETRIC_SPHEROID:
			size= (((Spheroid *)object)->objectSize);
			break;
		case CLOSED_CYLINDER:
		case CYLINDER:
		case VOLUMETRIC_CYLINDER:
			size= (((Cylinder *)object)->objectSize);
			break;
		case DISK:
			size= (((Disk *)object)->objectSize);
			break;
	}
	return(size);
}

void loadObjectSizeType(BigBag *bb,Contents_order *hit){
	double size;
	char *materialName;
	int objectType;

	objectType = hit->type;	
	materialName = getMaterialName(bb,(int)hit->type,hit->hit);
	size = getObjectSize(bb,(int)hit->type,hit->hit);

	loadObjectInfo(bb,materialName,objectType,size);
	return;	
}

int	parse_prat_wavefront_data(bb,verbose,top_root_bbox_Ptr,bbox_Ptr,fp,level,group_Ptr,current_mtl,vertices,normals,locals,normal,local_coords,m_inv_reverse_prev,m_inverse_fwd_prev,material_names,material_list_Ptr,material_table,vertexStore,angleTol,distanceTol,sizeTol)
	BigBag *bb;
	BBox	*bbox_Ptr;
	BBox	*top_root_bbox_Ptr;
	FILE	*fp;
	int	verbose;
	int	*level,vertexStore;
	Group	*group_Ptr;
	VeRtIcEs	*vertices;
	VeRtIcEs	*normals,*locals;
	Material_table	*current_mtl;
	Matrix4		*m_inv_reverse_prev,*m_inverse_fwd_prev;
	int		normal,local_coords;
	Material_List	*material_list_Ptr;
	char	**material_names;
	Material_table	*material_table;
	double angleTol,distanceTol,sizeTol;
{	
	char dum1[10],dum2[10],dum3[10],dum4[10],material[1000];
	int	l,retval,approx,define_flag=0,len,timer=0,bbox_closed=0,hit_check;
	Matrix4	m_inv_reverse, m_inverse_fwd;
	triplet	bmax,bmin;
	BBox	*current_bbox,transformed_bbox,*prev_bbox;
	double	bounds[6];
	char	*liner,line[1000],option[1000],*optioner;
	triplet	tri[3];
	fFacet	*fo;
	BiLinearPatch	*blp;
	Sphere	*sph;
	Cylinder *cyl;
	Clones	*clone;
	Disk	*disk;
	Dem	*dem;
	Ellipse	*ellipse;
	Spheroid *spheroid;
	Spherical_Dem	*spherical_dem;
	Plane	*plane_Ptr;
	Contents_order	*current_hit[6],hit,*hitter,*prevHitter;
	void	calculate_bbox_limits(),dem_read(),spherical_dem_read();
	int  sort_in_link_list(),precompute_facet_features();
	void *deleted;

	m_inv_reverse = *m_inv_reverse_prev;
	m_inverse_fwd = *m_inverse_fwd_prev;
	bbox_Ptr->min = vector_copy2((double)BIG,(double)BIG,(double)BIG);
	bbox_Ptr->max = vector_copy2((double)(-BIG),(double)(-BIG),(double)(-BIG));
	prev_bbox=NULL;current_bbox=NULL;

	while(fgets(line,1000,fp)){
#ifdef DEBUG
		fprintf(stderr,">>>%s\n",line);
#endif
		liner=line;
		hit_check=1;
		if(sscanf(liner,"%s",option)==1){
			liner=strchr(liner,option[0])+strlen(option);
			/* special #include flag 
			 */
			if(!strcmp(option,"#include")){
				char filename[1000];
				FILE *fp2=NULL;
				if(sscanf(liner,"%s",filename)==1){
					fp2=fp;
                                        fp=open_file_for_read(filename);
					if(verbose)fprintf(stderr,"#include %s",filename); 
					parse_prat_wavefront_data(bb,verbose,top_root_bbox_Ptr,bbox_Ptr,fp,level,group_Ptr,current_mtl,vertices,normals,locals,normal,local_coords,m_inv_reverse_prev,m_inverse_fwd_prev,material_names,material_list_Ptr,material_table,vertexStore,angleTol,distanceTol,sizeTol); 
					fclose(fp);
					fp=fp2;
				}
				hit_check=0;
			}else
				/*
				 **	info re contents of total bbox stored in root_bbox
				 */

				switch(type_of_element(option)){
					case OPEN_BBOX_RET:
						if(verbose && type_of_element(option)==OPEN_BBOX_RET)fprintf(stderr,"%c%d",123,*level);
						(*level)++;
						prev_bbox=current_bbox;
						if(current_bbox==NULL){
							if(!(bbox_Ptr->contents.bbox=(BBox *)calloc(1,sizeof(BBox))))
								error1("read_prat_wavefront_line:\terror allocating core for new bbox");
							current_bbox=bbox_Ptr->contents.bbox;
						}else{
							if(!(current_bbox->next_bbox=(BBox *)calloc((size_t)1,(size_t)sizeof(BBox))))error1("read_prat_wavefront_line:\terror allocating core for new bbox");
							current_bbox=current_bbox->next_bbox;
						}
						parse_prat_wavefront_data(bb,verbose,top_root_bbox_Ptr,current_bbox,fp,level,group_Ptr,current_mtl,vertices,normals,locals,normal,local_coords,m_inv_reverse_prev,m_inverse_fwd_prev,material_names,material_list_Ptr,material_table,vertexStore,angleTol,distanceTol,sizeTol);
						/* sum contents of new_root_bbox into current root_bbox */
						bbox_closed=1;
						if(current_bbox->max.x >0)bmax.x=1.0001*current_bbox->max.x;else bmax.x=0.9999*current_bbox->max.x;
						if(current_bbox->max.y >0)bmax.y=1.0001*current_bbox->max.y;else bmax.y=0.9999*current_bbox->max.y;
						if(current_bbox->max.x >0)bmax.z=1.0001*current_bbox->max.z;else bmax.z=0.9999*current_bbox->max.z;
						if(current_bbox->min.x >0)bmin.x=1.0001*current_bbox->min.x;else bmin.x=0.9999*current_bbox->min.x;
						if(current_bbox->min.y >0)bmin.y=1.0001*current_bbox->min.y;else bmin.y=0.9999*current_bbox->min.y;
						if(current_bbox->min.z >0)bmin.z=1.0001*current_bbox->min.z;else bmin.z=0.9999*current_bbox->min.z;

						if(current_bbox->transform_flag){
							transform_bbox(current_bbox->m,&bmin,&bmax);
						}
						if(current_bbox->define_flag){
							hit_check=0;
							current_bbox=prev_bbox;
						}else{
							bbox_Ptr->max=vmax(bbox_Ptr->max,bmax);
							bbox_Ptr->min=vmin(bbox_Ptr->min,bmin);
							calculate_bbox_limits(bounds,bbox_Ptr);
							if(bounds[0]!=-BIG){	/* weed out zero bbox */
								hit.type=BBOX;
								hit.hit=(BBox *)current_bbox;
							}else{
								hit_check=0;	/* dont include data */
								current_bbox=prev_bbox;
							}
							/* dont include 'defined' data */
							if(hit_check)(bbox_Ptr->contents.no_of_bboxes)++;
						}
						break;
					case CLOSE_BBOX_RET:
						approx=0;
						while((retval=approximateContents(bbox_Ptr,angleTol,distanceTol,sizeTol,&deleted))){
							approx++;
							/* remove deleted from hit */
							for(l=0;l<6;l++){
								hitter=bbox_Ptr->hit[l];
								prevHitter=NULL;
								while(hitter){
									if(hitter->hit==(void *)deleted){
										if(prevHitter){
											prevHitter->next_hit=hitter->next_hit;
										}else{ /* first one */
											bbox_Ptr->hit[l]=hitter->next_hit;
										}
										free((void *)hitter);
										hitter=NULL;
									}else{
										prevHitter=hitter;
										hitter=hitter->next_hit;
									}
								}
							}
						}
						if(approx&&verbose){
							fprintf(stderr,"*%d*",approx);
						}
						if(verbose){
							if(bbox_Ptr->max.x!=-BIG)
								fprintf(stderr,"[BBox: (%.2f,%.2f,%.2f) (%.2f,%.2f,%.2f)]",bbox_Ptr->min.x,bbox_Ptr->min.y,bbox_Ptr->min.z,bbox_Ptr->max.x,bbox_Ptr->max.y,bbox_Ptr->max.z);
							else
								fprintf(stderr,"[BBox: (UNDEFINED)]");
						}
						(*level)--;
						if(verbose){
							if(*level==-1)fprintf(stderr,"GLOBAL}");
							else fprintf(stderr,"%d}",*level);
						}
						return(define_flag);
						break;
					case GROUP_RET:
						optioner=liner+1;
						len=strlen(optioner)-1;
						optioner[len]='\0';
						if(group_Ptr->no_of_groups>=bb->max_number_of_groups){
							fprintf(stderr,"Fatal error in reading wavefront file: Hey dummy ... you're trying to use more than %d groups ... you can increase this by e.g.\n\tsetenv MAX_GROUPS 100000\n",bb->max_number_of_groups);
							exit(1);
						}
						group_Ptr->group_bboxes[group_Ptr->no_of_groups]=bbox_Ptr;
						strcpy(group_Ptr->group_names[group_Ptr->no_of_groups],optioner);
						(group_Ptr->no_of_groups)++;
						hit_check=0;
						if(verbose)fprintf(stderr,"[%s]",optioner);
#ifdef DESPARATE
						fprintf(stderr,"g %s\n",optioner);
#endif
						break;
					case VERTEX_RET:
						vertex_read(liner,vertices,normals,locals,normal,local_coords,vertexStore);
						hit_check=0;
						break;
					case LOCAL_RET:
						local_coord_read(liner,vertices,locals,local_coords,vertexStore);
						hit_check=0;
						break;
					case NORMAL_RET:
						normal_read(liner,vertices,normals,normal,vertexStore);
						hit_check=0;
						break;
					case DEFINE_RET:
						bbox_Ptr->define_flag=1;
						hit_check=0;
						if(verbose)fprintf(stderr,"(DEFINE)");
						break;
					case TRIANGLE_RET:
						fo= find_facet(bbox_Ptr);
#ifdef MATCHK
						check_material(current_mtl);
#endif
						fo->material= current_mtl;
						fo_read(tri,verbose,liner,vertices,normals,locals,&bbox_Ptr->max,&bbox_Ptr->min,normal,local_coords,fo,vertexStore);
						if(precompute_facet_features(bounds,tri,fo)==1){
							hit.type=FO;
							hit.hit=(fFacet *)fo;
						}else{
							fo=delete_facet(bbox_Ptr,fo);
						}
						break;
					case BILINEARPATCH_RET:
						blp = find_blp(bbox_Ptr);
#ifdef MATCHK
						check_material(current_mtl);
#endif
						blp->material= current_mtl;
						/*blp_read(tri,verbose,liner,vertices,normals,locals,&bbox_Ptr->max,&bbox_Ptr->min,normal,local_coords,blp);
						precompute_facet_features(bounds,tri,fo); ... TODO ... for blp */
						hit.type=BILINEARPATCH;
						hit.hit=(BiLinearPatch *)blp;
						break;
					case SPHERE_RET:
						sph= find_sphere(bbox_Ptr);
						sph->material= current_mtl;
						sph_read(bounds,sph,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						hit.type=SPHERE;
						hit.hit=(Sphere *)sph;
						break;
					case DISK_RET:
						disk= find_disk(bbox_Ptr);
						disk->material= current_mtl;
						disk_read(bounds,disk,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						hit.type=DISK;
						hit.hit=(Disk *)disk;
						break;
					case VOLUME_SPHERE_RET:
						sph= find_sphere(bbox_Ptr);
						sph_read(bounds,sph,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						sscanf(liner,"%s %s %s",dum1,dum2,material);
						sph->material=(Material_table *)current_material_read(verbose,material,material_names,material_list_Ptr,material_table);
						hit.type=VOLUMETRIC_SPHERE;
						hit.hit=(Sphere *)sph;
						break;
					case VOLUME_CYL_RET:
						cyl= find_cylinder(bbox_Ptr);
						cyl_read(bounds,cyl,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,1,vertexStore);
						sscanf(liner,"%s %s %s %s",dum1,dum2,dum3,material);
						cyl->material=(Material_table *)current_material_read(verbose,material,material_names,material_list_Ptr,material_table);
						hit.type=VOLUMETRIC_CYLINDER;
						hit.hit=(Cylinder *)cyl;
						break;
					case CLOSED_CYL_RET:
						cyl= find_cylinder(bbox_Ptr);
						cyl->material= current_mtl;
						cyl_read(bounds,cyl,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,1,vertexStore);
						hit.type=CLOSED_CYLINDER;
						hit.hit=(Cylinder *)cyl;
						break;
					case CYLINDER_RET:
						cyl= find_cylinder(bbox_Ptr);
						cyl->material= current_mtl;
						cyl_read(bounds,cyl,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,0,vertexStore);
						hit.type=CYLINDER;
						hit.hit=(Cylinder *)cyl;
						break;
					case ELLIPSE_RET:
						ellipse= find_ellipse(bbox_Ptr);
						ellipse->material= current_mtl;
						ellipse_read(bounds,ellipse,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						hit.type=ELLIPSE;
						hit.hit=(Ellipse *)ellipse;
						break;
					case SPHER_RET:
						spheroid = find_spheroid(bbox_Ptr);
						spheroid->material= current_mtl;
						spheroid_read(bounds,spheroid,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						hit.type=SPHEROID;
						hit.hit=(Spheroid *)spheroid;
						break;
					case VOLUME_SPHEROID_RET:
						spheroid= find_spheroid(bbox_Ptr);
						spheroid_read(bounds,spheroid,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						sscanf(liner,"%s %s %s",dum1,dum4,material);
						spheroid->material=(Material_table *)current_material_read(verbose,material,material_names,material_list_Ptr,material_table);
						hit.type=VOLUMETRIC_SPHEROID;
						hit.hit=(Spheroid *)spheroid;
						break;
					case VOLUME_ELLIPSE_RET:
						ellipse= find_ellipse(bbox_Ptr);
						ellipse_read(bounds,ellipse,verbose,liner,vertices,&bbox_Ptr->max,&bbox_Ptr->min,vertexStore);
						sscanf(liner,"%s %s %s %s %s",dum1,dum2,dum3,dum4,material);
						ellipse->material=(Material_table *)current_material_read(verbose,material,material_names,material_list_Ptr,material_table);
						hit.type=VOLUMETRIC_ELLIPSE;
						hit.hit=(Ellipse *)ellipse;
						break;
					case CLONE_RET:
						clone = find_clone(bbox_Ptr);
						transformed_bbox = *bbox_Ptr;
						clone_read(bounds,clone,verbose,liner,group_Ptr,bbox_Ptr,&transformed_bbox);
						hit.type=CLONE;
						hit.hit=(Clone *)clone;
						break;
					case DEM_RET:
						dem = find_dem(bbox_Ptr);
						dem_read(bounds,dem,verbose,local_coords,liner,&(dem->data),&(dem->hd),&bbox_Ptr->max,&bbox_Ptr->min,&(dem->dem_attributes));
						dem->material= current_mtl;
						hit.type=DEM;
						hit.hit=(Dem *)dem;
						break;
					case SPHERICAL_DEM_RET:
						spherical_dem = find_spherical_dem(bbox_Ptr);
						spherical_dem_read(bounds,verbose,liner,&(spherical_dem->data),&(spherical_dem->hd),&bbox_Ptr->max,&bbox_Ptr->min,spherical_dem,vertices);
						spherical_dem->material= current_mtl;
						hit.type=SPHERICAL_DEM;
						hit.hit=(Spherical_Dem *)spherical_dem;
						break;
					case PLANE_RET:
						/* store infinite object at top level */
						plane_Ptr=find_plane(top_root_bbox_Ptr);
						plane_read(&(plane_Ptr->normal),&(plane_Ptr->dw),verbose,liner,vertices,vertexStore);
						plane_Ptr->material= current_mtl;
						hit_check=0;	/* not for local box */
						break;
					case USEMTL_RET:
						current_mtl=current_material_read(verbose,liner,material_names,material_list_Ptr,material_table);
						hit_check=0;
						break;
					case MTLLIB_RET:
						mtllib_read(bb,verbose,liner,material_list_Ptr,material_names,material_table);
						/* allocate objectType info */
						bb->nObjectTypes=0;
						bb->objectTypes=i_allocate(MAX_OBJECT_TYPES*material_list_Ptr->no_of_materials);
						bb->objectMaterialNames=(char **)v_allocate(MAX_OBJECT_TYPES*material_list_Ptr->no_of_materials,sizeof(char *));
						bb->objectTypeCapacity=MAX_OBJECT_TYPES*material_list_Ptr->no_of_materials;
						bb->minObjectSize=d_allocate(bb->objectTypeCapacity);
						bb->maxObjectSize=d_allocate(bb->objectTypeCapacity);
						hit_check=0;
						break;
					case TRANS_RET:
						load_transformation_matrix(liner,bbox_Ptr,&m_inv_reverse,&m_inverse_fwd);
						hit_check=0;
						if(verbose)fprintf(stderr,"[M]");
						break;
					default:
						hit_check=0;
						break;
				}
			/*
			 **	sort order of object in current box
			 */
			if(hit_check!=0){
				/* sort size and material use info */
				loadObjectSizeType(bb,&hit);
				find_current_hit(&(current_hit[0]),&timer,&(bbox_Ptr->hit[0]));
				sort_in_link_list(&(current_hit[0]),&timer,&(bbox_Ptr->hit[0]),&(bounds[0]),&hit);
			}

		}
	}
	return(FILE_END);
}


