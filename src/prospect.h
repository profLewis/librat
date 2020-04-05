#ifndef PROSPECT_H
#define PROSPECT_H
typedef struct {
	double	chlorophyll;
	double	equivalentWaterThickness;
	double	LeafStructure;
} ProspectModel;

#define CHLOROPHYLL_CONCENTRATION	2
#define EQUIVALENT_WATER_THICKNESS	3
#define LEAF_STRUCTURE_PARAMETER	4
#endif
