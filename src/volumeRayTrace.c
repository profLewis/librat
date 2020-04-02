#include "frat.h"
/* some potential hot spot stuff */

double HS_k1(G,G_,m,m_)
double G,G_,m,m_;
{
	return(sqrt(MAX(0.,G_*m*m_*G)));
}

double HS_k2(m,m_,cosAlpha)
double m,m_,cosAlpha;
{
	return(sqrt(MAX(0.,m*m+m_*m_+2.0*m*m_*cosAlpha)));
}

/* Kuusk's 1985 model */
double HS_effect(l,G,G_,m,m_,v,s)
double l,G,G_,m,m_;
triplet *v,*s;
{
	double cosAlpha;
	double f,k1,k2;

/* l = 0 -> no hs */
	if(l==0)return(0.0);

/* assume v,s normalized */
	cosAlpha=V_dot(*v,*s);
/* if dot product > 0, no effect */
	if(cosAlpha>0)return(0.0);

/* in hs direction */
	if(cosAlpha>-1.0001&&cosAlpha<-0.9999)	
		return(G_*m);

	k1=HS_k1(G,G_,m,m_);
	k2=HS_k2(m,m_,cosAlpha);

	f=(k1*l/k2)*(1 - EXPONENTIAL(-k2/l));
	return(f);
}

void sortGlut(Volume *volume,int n){
  int i,j,total=10000;
  double angle,sum;
  triplet leaf,view,readFileEqual();
  double projection;

  if(!(volume->Glut=(double *)calloc(n,sizeof(double)))){
    fprintf(stderr,"error in core allocation\n");
    exit(1);
  }
  volume->nSamples=n;
  
  view.x=0;
  for(j=0;j<n;j++){
    angle = j*M_PI_2/(n-1);
    view.y=sin(angle);
    view.z=cos(angle);
    sum=0;
    for(i=0;i<total;i++){
      leaf=readFileEqual(volume);
      projection = V_dot(leaf,view);
      sum += fabs(projection);
    }
    volume->Glut[j]=sum/(double)(total);
  }
}

double HS_term(l,G,G_,m,m_,v,s)
double l,G,G_,m,m_;
triplet *v,*s;
{
	double f;
	f=HS_effect(l,G,G_,m,m_,v,s);
	return(f/G_);
}

/* a utility fn to reallocate some facets */
fFacet *reallocateFacetBlock(int nFacets,int nFacetBlock,fFacet *facets){
	fFacet *out=NULL;
	int i;

	/* allocate the new block */
	if(!(out=(fFacet *)calloc(nFacets+nFacetBlock,sizeof(fFacet)))){
		fprintf(stderr,"error in core allocation\n");
		exit(1);
	}

	/* copy the nFacets old ones over */
	if(facets){
		for(i=0;i<nFacets;i++){
			out[i]=facets[i];
		}

		/* free the old block */
		free(facets);
	}

	/* return the new block */
	return(out);
}


/*
** 	v	viewing vector
**
**	uL	leaf area density (m^2/m^3)
**	
**	G	fn. that returns object normal
**
**	maxV	max distance that can be travelled
**			- returns -1 if pathlength > maxV
**
**	returns path length that can be travelled
** & if valid, a facet of the correct orientation
*/

fFacet *volumeRayTrace(bb,flag_Ptr,mv,v,volume,maxV,s)
     BigBag *bb;
triplet *v;
double maxV;
double *mv;
FlagBag *flag_Ptr;
Volume *volume;
triplet *s;	/* sun vector - set to NULL if viewing */
{
	double	nzenith,tol=0.01,diff,Gv,hs,m_,uL;
	double l,G,m,zenith;	
	/* leaf linear dimension, G of viewer, path length of viewer */
	triplet (*Normal)(),readFileEqual();
	int	izenith,iter=20;
	triplet V;
/* your actual facet ... chosen from the stack */
	fFacet *facet;

/* am i on a reset ? */
	if(volume==NULL || flag_Ptr==NULL || mv==NULL){
		bb->whichFacet=0;
		return(NULL);
	}

	if(!(volume->Glut)){
	  sortGlut(volume,91);
	}
	uL=volume->uL;
	l=volume->l;
	m=volume->m;
	Normal=volume->lad;
	if(!Normal)volume->lad=Normal=readFileEqual;

/* ok ... do I need to allocate any facets */
	if(bb->whichFacet>=bb->nFacets){
		/* reallocate the facets list */
		bb->facets=reallocateFacetBlock(bb->nFacets,bb->nFacetBlock,bb->facets);
		bb->nFacets+=bb->nFacetBlock;
	}
/* chose a nice new facet to work with */
	facet=&bb->facets[bb->whichFacet];
	bb->whichFacet++;

/* object normal */

	/* view zenith */
	zenith = atan2(sqrt(1-v->z * v->z),fabs(v->z));
	nzenith = (volume->nSamples-1)*(zenith/M_PI_2);
	izenith = (int)nzenith;
	if(izenith == volume->nSamples - 1){
	  Gv = volume->Glut[izenith];
	}else{
	  Gv = volume->Glut[izenith] + (nzenith-izenith)*(volume->Glut[izenith+1]-volume->Glut[izenith]);
	}
	facet->normal=Normal(volume);

	*mv=-log((double)Random())/(double)(Gv*uL);

/* hs term */
	if(s&&m&&V_mod(s)!=0){
		V=normalise(*v);
/* 
**	approximate m_ - converges to tol*m (e.g. 0.01*m) 
**
**	in fact, it usually converges to around 1e-5 after only 1 iteration
**	so this isnt expensive
*/
		diff=tol*2*m;
		hs=0.0;
		m_=m;	/* initial estimate */
/* calculate distance travelled before random element comes in */
		while(diff/m>tol&&iter>0){
/* new hs estimate */
			hs=HS_term(l,G,Gv,m,m_,&V,s);
/* recaluclate estimate of path length */
			diff=fabs(hs-m_);
			m_=hs;
			iter--;
		}
		*mv+=hs;
	}
/* at present, this is not quite correct -
** if the ray *misses the current object & hits,
** say, the soil, the jgap information is lost
**
** this is not correct in the hot spot direction,
** or if the intersected object is close to the 
** current object, as, in the hs direction,
** if it were able to pass thro' on the way down
** it must be able to pass thro' on the way up.
*/
/* ray exits bounding box before interaction */
/* only store values if you hit */
	if(!s||V_mod(s)==0){
/* store values */
		if(maxV>0&&*mv>maxV){
			volume->m=maxV;
		}else
			volume->m=*mv;
		volume->G=Gv;
		volume->s=normalise(*v);
	}else{
		volume->m=0;
		volume->G=0;
		volume->s=vector_copy2(0.,0.,0.);
	}
	if(maxV>0&&*mv>maxV)
		return(NULL);

/* set up facet */
	return(facet);
}

