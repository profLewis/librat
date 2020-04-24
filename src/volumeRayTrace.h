#ifndef VOLUMERAYTRACE_H
#define VOLUMERAYTRACE_H
double HS_k1(double G,double G_,double m,double m_);
double HS_k2(double m,double m_,double cosAlpha);
double HS_effect(double l,double G,double G_,double m,double m_,triplet *v,triplet *s);
void sortGlut(Volume *volume,int n);
double HS_term(double l,double G,double G_,double m,double m_,triplet *v,triplet *s);
fFacet *reallocateFacetBlock(int nFacets,int nFacetBlock,fFacet *facets);
fFacet *volumeRayTrace(RATobj *bb,FlagBag *flag_Ptr,double *mv,triplet *v,Volume *volume,double maxV,triplet *s);

#endif
