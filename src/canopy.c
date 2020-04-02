#include <stdio.h>
#include <math.h>
#include	"useful3.h"
#include	"vectors2.h"

#define USEAGE "canopy [-u_l Ul] [-v | -verbose] [-leaf_base leaf_base | -B leaf_base] [-BHR BHR | -base_to_height_ratio BHR] [-s sx sy sz | -extent sx sy sz] [-o ox oy oz | -origin ox oy oz] [-r seed | -random seed] [-m materialLibrary | -matlib materialLibrary] [-uniform | -spherical] [-l leafMaterial | -leaf leafMaterial] [-extend Nextension] [-soil] [-help | -useage]"

typedef struct{
	double	u_l;
	double	ox,oy,oz,sx,sy,sz;
	double	leaf_base,BHR;
}CanopyDescription;

typedef struct{
	double	delta;
	double	cx,cy;
}DerivedCanopyDescription;

void spherical(theta_l,phi_l)
double	*theta_l,*phi_l;
{
	double	si;
	si=Random();
	*phi_l=Random()*2.*M_PI;
	*theta_l=acos(si);
	return;
}

void uniform(theta_l,phi_l)
double	*theta_l,*phi_l;
{
	*phi_l=Random()*2.*M_PI;
	*theta_l=M_PI_2*Random();
	return;
}


main(argc,argv)
int argc;
char **argv;
{
	CanopyDescription	canopy;
	DerivedCanopyDescription	field;
	double	leafArea,x,y,z,D,theta_l,phi_l;
	double	NleavesPerM3,NleavesPerM,canopyVolume,b1,h1,h2,area=0.0,Asum=0.0;
	void	(* distribution)();
	triplet	shift,V,u,v,t[3],p,i,j,n,d,min,max,dim;
	int	INumber[3],Number[3],Nleaves,soil=0,clone,zclone,row,level,N=0,I,J,K,verbose=0,randomSeed=1,extend=0;
	char	matlib[1000];
	char	leafMaterial[100];

	strcpy(leafMaterial,"leaf");
	strcpy(matlib,"/ps/data/callisto/alpha/plewis/canopy/plants.matlib");
	canopy.leaf_base=0.1;
	canopy.u_l=0.5;
	canopy.BHR=2./sqrt(3.); /* equilateral */
	canopy.sx=canopy.sy=5.;
	canopy.sz=10.;
	canopy.ox= -canopy.sx/2.;
	canopy.oy= -canopy.sy/2.;
	canopy.oz=0.0;
	distribution=spherical;

	for(I=1;I<argc;I++){
		if(strcmp(argv[I],"-uniform")==0){
			distribution=uniform;
		}else if(strcmp(argv[I],"-spherical")==0){
			distribution=spherical;
		}else if(strcmp(argv[I],"-u_l")==0){
			canopy.u_l=atof(argv[++I]);
		}else if(strcmp(argv[I],"-m")==0||strcmp(argv[I],"-matlib")==0){
			strcpy(matlib,argv[++I]);
		}else if(strcmp(argv[I],"-l")==0||strcmp(argv[I],"-leaf")==0){
			strcpy(leafMaterial,argv[++I]);
		}else if(strcmp(argv[I],"-v")==0||strcmp(argv[I],"-verbose")==0){
			verbose=1;
		}else if(strcmp(argv[I],"-extend")==0){
			extend=atoi(argv[++I]);
		}else if(strcmp(argv[I],"-leaf_base")==0||strcmp(argv[I],"-B")==0){
			canopy.leaf_base=atof(argv[++I]);
		}else if(strcmp(argv[I],"-BHR")==0||strcmp(argv[I],"-base_to_height_ratio")==0){
			canopy.BHR=atof(argv[++I]);
		}else if(strcmp(argv[I],"-r")==0||strcmp(argv[I],"-random")==0){
			randomSeed=atoi(argv[++I]);
		}else if(strcmp(argv[I],"-e")==0||strcmp(argv[I],"-extent")==0){
			canopy.sx=atof(argv[++I]);
			canopy.sy=atof(argv[++I]);
			canopy.sz=atof(argv[++I]);
			canopy.ox= -canopy.sx/2.;
			canopy.oy= -canopy.sy/2.;
			canopy.oz=0.0;
		}else if(strcmp(argv[I],"-soil")==0){
			soil=1;
		}else if(strcmp(argv[I],"-o")==0||strcmp(argv[I],"-origin")==0){
			canopy.ox=atof(argv[++I]);
			canopy.oy=atof(argv[++I]);
			canopy.oz=atof(argv[++I]);
		}else if(strcmp(argv[I],"-help")==0||strcmp(argv[I],"-useage")==0){
			fprintf(stderr,USEAGE);exit(1);
		}else{
			fprintf(stderr,"unrecognised option %s\n",argv[I]);
			fprintf(stderr,USEAGE);exit(1);
		}
	}

	fprintf(stdout,"mtllib %s\n",matlib);

	seed_randomise(randomSeed);
	leafArea=0.5*canopy.leaf_base*canopy.leaf_base/canopy.BHR;
	NleavesPerM3=canopy.u_l/leafArea;
	NleavesPerM=cbrt(NleavesPerM3);
	field.delta=1.0/NleavesPerM;
	canopyVolume=canopy.sx*canopy.sy*canopy.sz;
	Nleaves=(int)(canopy.u_l*canopyVolume/leafArea);
	Number[0]=(int)(canopy.sx/field.delta+ 1.0 -1e-6);
	Number[1]=(int)(canopy.sy/field.delta+ 1.0 -1e-6);
	Number[2]=(int)(canopy.sz/field.delta+ 1.0 -1e-6);
	canopy.sx=Number[0]*field.delta;
	canopy.sy=Number[1]*field.delta;
	canopy.sz=Number[2]*field.delta;

	field.cx=0.5*canopy.leaf_base;
	field.cy=(1/3.)*canopy.leaf_base/canopy.BHR;
	b1=canopy.leaf_base;
	h1=canopy.leaf_base/canopy.BHR;

	if(verbose){
		fprintf(stderr,"generating real canopy\nwith origin {%lf %lf %lf}\nof extent {%lf %lf %lf}\n",canopy.ox,canopy.oy,canopy.oz,canopy.sx,canopy.sy,canopy.sz);
		fprintf(stderr,"lad = %lf\n",canopy.u_l);
		fprintf(stderr,"lai = %lf\n",canopy.u_l*canopy.sz);
		fprintf(stderr,"leaf area = %lf\n",leafArea);
		fprintf(stderr,"average leaf spacing = %lf\n",field.delta);
		fprintf(stderr,"real canopy volume = %lf\n",canopyVolume);
		fprintf(stderr,"number of real leaves = %d\n",Number[0]*Number[1]*Number[2]);
		if(soil)fprintf(stderr,"with underlying soil\n");
	}

	if(soil)printf("v 0.000000 0.000000 %lf\nv 0.000000 0.000000 1.000000\nusemtl soil\nplane -1 -2\n",canopy.oz-MAX(canopy.leaf_base,canopy.leaf_base/canopy.BHR));

	printf("!{\n");
	if(extend)printf("#define\ng\trealCanopy\n");
	fprintf(stdout,"usemtl %s\n",leafMaterial);

	for(level=0,z=canopy.oz,INumber[2]=0;INumber[2]<Number[2];z-=field.delta,level++,INumber[2]++){
		printf("!{\n");
		printf("g\tlevel %d\n",level);
		/* introduce a randon x-y shift */
		shift=vector_copy2((Random()-.5)*field.delta,(Random()-.5)*field.delta,0.0);
		for(row=0,x=canopy.ox,INumber[0]=0;INumber[0]<Number[0];x+=field.delta,row++,INumber[0]++){
			printf("!{\n");
			printf("g\trow %d\n",row);
			for(y=canopy.oy,INumber[1]=0;INumber[1]<Number[1];y+=field.delta,INumber[1]++){
				N++;
				distribution(&theta_l,&phi_l);
				n=spherical_to_cartesian(vector_copy2(1.0,phi_l,M_PI_2-theta_l),1);
				i=spherical_to_cartesian(vector_copy2(1.0,phi_l,-theta_l),1);
				i=rotate_about(i,n,Random()*2.*M_PI);
				j=vector_cross(n,i);
				/* get bbox 0f leaf */
				t[0]=vector_copy2(0.,0.,0.);
				t[1]=vector_copy2(b1*j.x,b1*j.y,b1*j.z);
				t[2]=vector_copy2((b1/2.)*j.x+h1*i.x,(b1/2.)*j.y+h1*i.y,(b1/2.)*j.z+h1*i.z);
				max.x=MAX(MAX(t[0].x,t[1].x),t[2].x);
				max.y=MAX(MAX(t[0].y,t[1].y),t[2].y);
				max.z=MAX(MAX(t[0].z,t[1].z),t[2].z);
				min.x=MIN(MIN(t[0].x,t[1].x),t[2].x);
				min.y=MIN(MIN(t[0].y,t[1].y),t[2].y);
				min.z=MIN(MIN(t[0].z,t[1].z),t[2].z);
				dim=vector_minus(max,min);
				/* use above for attempt to get non-overlap */
				/* but results in clumping ...? */
				dim=vector_copy2(0.,0.,0.);

					d=vector_copy2(MAX(0.,field.delta-dim.x)*Random(),MAX(0.,field.delta-dim.y)*Random(),MAX(0.,field.delta-dim.z)*Random());
					t[0]=vector_minus(t[0],vector_plus(max,d));
					t[1]=vector_minus(t[1],vector_plus(max,d));
					t[2]=vector_minus(t[2],vector_plus(max,d));
					u=vector_minus(t[0],t[1]);
					v=vector_minus(t[2],t[1]);
					n=vector_cross(u,v);
					area+=0.5*V_mod(n);
					Asum+=fabs(0.5*n.z);
					printf("v %lf %lf %lf\n",t[0].x+x+field.delta +shift.x,t[0].y+y+field.delta+shift.y,t[0].z+z);
					printf("v %lf %lf %lf\n",t[1].x+x+field.delta +shift.x,t[1].y+y+field.delta+shift.x,t[1].z+z);
					printf("v %lf %lf %lf\n",t[2].x+x+field.delta +shift.x,t[2].y+y+field.delta+shift.x,t[2].z+z);
					printf("f -1 -2 -3\n");
			}
			printf("!}\n");
		}
		printf("!}\n");
	}

	printf("!}\n");

	if(extend){
		printf("!{\n");
		printf("g\tclonedCanopy\n");
		J=extend/2;
		d=vector_copy2(-J*canopy.sx,-J*canopy.sy,0.0);
		if(soil)d.z=1;else d.z=extend;
		for(K=0,zclone=0;K<d.z;K++,zclone++){
			printf("!{\n");
			printf("g\tcloneGroupDepth %d\n",zclone);
			/* introduce a random shift in the cloned layers */
			shift=vector_copy2((Random()-.5)*canopy.sx,(Random()-.5)*canopy.sy,0.0);
			for(I=0,clone=0;I<extend;I++,clone++){
				printf("!{\n");
				printf("g\tcloneGroupRow %d\n",clone);
				for(J=0;J<extend;J++){
					printf("clone %lf %lf %lf 0.0 realCanopy\n",d.x+J*canopy.sx+shift.x,d.y+I*canopy.sy+shift.y,-K*canopy.sz);
				}
				printf("!}\n");
			}
			printf("!}\n");
		}
		printf("!}\n");

	}

	if(!soil)z=canopy.sz;
	else z=canopy.sz*extend;
	if(verbose){
		if(extend)if(!soil){
			fprintf(stderr,"extending %d times to canopy\nwith origin {%lf %lf %lf}\nof extent {%lf %lf %lf}\n",extend,d.x+canopy.ox,d.y+canopy.oy,d.z+canopy.oz,canopy.sx*extend,canopy.sy*extend,z*extend);
			fprintf(stderr,"%d original leaves in canopy\n",N);
			fprintf(stderr,"%d virtual copies\n",N*extend*extend*extend);
		}else{
			fprintf(stderr,"extending %d times to canopy\nwith origin {%lf %lf %lf}\nof extent {%lf %lf %lf}\n",extend,d.x+canopy.ox,d.y+canopy.oy,d.z+canopy.oz,canopy.sx*extend,canopy.sy*extend,z);
			fprintf(stderr,"%d original leaves in canopy\n",N);
			fprintf(stderr,"%d virtual copies\n",N*extend*extend);
		}
		fprintf(stderr,"G(0.0) = %lf\n",Asum/area);
		fprintf(stderr,"leaf area density = %lf\n",area/(canopy.sx*canopy.sy*canopy.sz));
		fprintf(stderr,"leaf area index = %lf\n",area/(canopy.sx*canopy.sy));
	}

}
