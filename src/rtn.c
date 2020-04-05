#include "prat.h"
double         *distance = NULL, *direct = NULL, *diffuse = NULL;
double         *distanceTot = NULL, *directTot = NULL, *diffuseTot = NULL;
double         *directSum = NULL, *diffuseSum = NULL;
int            *nDirect = NULL, *nDiffuse = NULL;
double         *directBinTot = NULL, *diffuseBinTot = NULL;
RTN **RTNLeafArray, **RTNLeafSkyArray;
int nRTNLeafArray, nRTNLeafSkyArray;


int 
whichBin(Samples * samples, double distance)
{
	int             bin;
	double          scaled;
	if (!samples->binStep)
		return (-1);
	scaled = (distance - samples->startBinCount) / (1. * samples->binStep);
	bin = (int) scaled + 1;
	bin = MIN(MAX(bin, 0), samples->no_of_time_bins - 1);
	return (bin);
	/* first & last bin are for overflows */
}

void 
allocateRTDStore(Samples * samples)
{
	int             nNodes, nLevels, nBands, doLidar, rtd;

	rtd = MAX(1, MAX(samples->max_diffuse_sampling_depth, samples->max_specular_sampling_depth));
	nLevels = 2 + rtd;
	nBands = samples->nBands;
	nNodes = samples->cumulativeNoOfNodes[nLevels - 1] + samples->noOfNodes[nLevels - 1];
	if (samples->binStep)
		doLidar = TRUE;
	else
		doLidar = FALSE;

	if (!direct)
		direct = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!diffuse)
		diffuse = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!directTot)
		directTot = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!diffuseTot)
		diffuseTot = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!directSum)
		directSum = (double *) v_allocate(nBands, sizeof(double));
	if (!diffuseSum)
		diffuseSum = (double *) v_allocate(nBands, sizeof(double));
	if (!distance)
		distance = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!distanceTot)
		distanceTot = (double *) v_allocate(nBands * rtd, sizeof(double));
	if (!nDirect)
		nDirect = (int *) v_allocate(rtd, sizeof(int));
	if (!nDiffuse)
		nDiffuse = (int *) v_allocate(rtd, sizeof(int));
	if (doLidar) {
		if (!directBinTot)
			directBinTot = (double *) v_allocate(samples->no_of_time_bins * rtd * nBands, sizeof(double));
		if (!diffuseBinTot)
			diffuseBinTot = (double *) v_allocate(samples->no_of_time_bins * rtd * nBands, sizeof(double));
	}
	if (!RTNLeafArray)
		RTNLeafArray = (RTN **) v_allocate(nNodes, sizeof(RTN *));
	if (!RTNLeafSkyArray)
		RTNLeafSkyArray = (RTN **) v_allocate(nNodes, sizeof(RTN *));
	return;
}

/* calculate ray tree stats */
/*
 * at rtd level 1: * pick up ray distance v->1 *
 * 
 * if type == -1 node is junk *
 * 
 * nodes are also junk if they dont eventually lead to * a leaf (RTN_LEAF |
 * RTN_LEAF_SKY) *
 * 
 * at other levels i: * refl of material at interaction i-1 * ray length
 * (cumulative) from i-1 to i * refl/trans interaction at level i-1
 * (reflTrans) *
 * 
 * 
 * at level of type RTN_LEAF 2: IRRAD of DIRECT ORIGIN * have above info +
 * irradiance info * ray length now refers to from i-1 back to viewer *
 * 
 * 
 * at level of type RTN_LEAF_SKY 5: IRRAD of DIFFUSE ORIGIN * have above info +
 * irradiance info * ray length now refers to from i-1 back to viewer
 * 
 */

/*
 * HEALTH WARNING: * wont deal correctly with having > 1 sample of each type *
 * at the moment - need to know what sort of node we are * which is partially
 * available from the style flag * but probably more reliably obtained from
 * going to prev & * finding our current ptr in the nextShadow etc lists *
 * 
 * once this information is available, we need to divide contributions by *
 * the number of rays of these different types
 */
int
statsRTN(Samples * samples)
{
	RTN            *prevNode, *prevCore, *current;
	int             bin, i, j, k, l, nNodes, nLevels, rtd, nBands,
	                currentRTD, doLidar;
	double          irrad, refl[MAX_TREE_DEPTH];

	if (samples->binStep)
		doLidar = TRUE;
	else
		doLidar = FALSE;
	rtd = MAX(1, MAX(samples->max_diffuse_sampling_depth, samples->max_specular_sampling_depth));
	nLevels = 2 + rtd;
	nBands = samples->nBands;
	for (i = 0; i < nBands * rtd; i++) {
		distanceTot[i] = directTot[i] = diffuseTot[i] = 0.;
	}
	if (doLidar) {
		for (j = 0; j < samples->no_of_time_bins * rtd * nBands; j++) {
			directBinTot[j] = diffuseBinTot[j] = 0.;
		}
	}
	for (i = 0; i < nBands; i++) {
		diffuseSum[i] = directSum[i] = 0.;
	}
	nNodes = samples->cumulativeNoOfNodes[nLevels - 1] + samples->noOfNodes[nLevels - 1];
	/* find all illumination nodes */
	for (i = 0; i < nRTNLeafArray; i++) {
		current = RTNLeafArray[i];
		/* shouldnt need this */
		for (j = 0; j < nBands * rtd; j++)
			direct[j] = distance[j] = 0.;
		for (j = 0; j < nBands; j++) {
			/* irradiance */
			irrad = current->irrad[j];
			/* store reflectances */
			prevCore = current;
			for (k = current->rtd - 2; k >= 0; k--) {
				refl[k] = prevCore->refl[j];
				prevCore = prevCore->prev;
			}
			currentRTD = MAX(0, current->rtd - 2);
			direct[currentRTD + rtd * j] = irrad;
			distance[currentRTD + rtd * j] = current->rayLength + current->prev->rayLength;
			/* find appropraite distance bin and sum */
			bin = whichBin(samples, distance[currentRTD + rtd * j]);
			for (k = 0; k <= currentRTD; k++) {
				direct[currentRTD + rtd * j] *= refl[k];
			}
			directTot[currentRTD + rtd * j] += direct[currentRTD + rtd * j];
			if (bin >= 0 && doLidar)
				directBinTot[bin + currentRTD * samples->no_of_time_bins + j * samples->no_of_time_bins * rtd] += direct[currentRTD + rtd * j];
		}
		nDirect[currentRTD]++;
	}
	for (i = 0; i < nRTNLeafSkyArray; i++) {
		current = RTNLeafSkyArray[i];
		/* shouldnt need this */
		for (j = 0; j < nBands * rtd; j++)
			diffuse[j] = distance[j] = 0.;
		for (j = 0; j < nBands; j++) {
			/* irradiance */
			irrad = current->irrad[j];
			/* store reflectances */
			prevCore = current;
			for (k = current->rtd - 2; k >= 0; k--) {
				refl[k] = prevCore->refl[j];
				prevCore = prevCore->prev;
			}
			currentRTD = MAX(0, current->rtd - 2);
			distance[currentRTD + rtd * j] = current->rayLength + current->prev->rayLength;
			/* find appropraite distance bin and sum */
			bin = whichBin(samples, distance[currentRTD + rtd * j]);
			diffuse[currentRTD + rtd * j] = irrad;
			for (k = 0; k <= currentRTD; k++) {
				diffuse[currentRTD + rtd * j] *= refl[k];
			}
			diffuseTot[currentRTD + rtd * j] += diffuse[currentRTD + rtd * j];
			if (bin >= 0 && doLidar)
				diffuseBinTot[bin + currentRTD * samples->no_of_time_bins + j * samples->no_of_time_bins * rtd] += direct[currentRTD + rtd * j];
		}
		nDiffuse[currentRTD]++;
	}
	/* sum all levels */
	if(nRTNLeafArray)for (j = 0; j < nBands; j++) {
		for (k = 0; k < rtd; k++) {
			directSum[j] += directTot[k + rtd * j];
		}
	}
	if(nRTNLeafSkyArray)for (j = 0; j < nBands; j++) {
		for (k = 0; k < rtd; k++) {
			diffuseSum[j] += diffuseTot[k + rtd * j];
		}
	}
	return (1);
	/*
	 * information returned: nDiffuse[rtd], nDirect[rtd]   ... no of
	 * samples at each level directSum[waveband], diffuseSum[waveband]
	 * ... totals per waveband for this ray tree diffuseTot[rtd +
	 * nRTD*waveband], directTot[rtd + nRTD*waveband]... combined
	 * contributions for this ray tree
	 * diffuseBinTot[bin+rtd*nBins+waveband*nBins*nRTD],
	 * directBinTot[bin+rtd*nBins+waveband*nBins*nRTD] ... combined
	 * binned lidar contributions
	 */
	/*
	 * could also go for stats on path length as a fn of rtd which would
	 * be interesting
	 * 
	 * also if we store the material information (in the int material field)
	 * then we can work out the combination of interactions eg 3 leafR 2
	 * leafT and 2 soil interactions and store this along with the
	 * appropriate attenuation field information to provide a generic
	 * multiple scattering result
	 */
}


int
clearRTN(Samples * samples, RTN * current, RTN * base, int level, int nLevels)
{
	RTN            *nextNode;
	int             i, j;


	if (!current)
		return (0);

	for (j = 0; j < samples->nBands; j++) {
		current->refl[j] = 0.;
		current->irrad[j] = 0.;
	}
	current->rayLength = 0.;
	current->attenuation = 0.;
	current->type = -1;
	current->material = -1;
	current->reflTrans = -1;

	if (!current->nextShadow)
		return (0);
	for (i = 0; i < samples->no_of_shadow_rays; i++) {
		nextNode = current->nextShadow[i];
		clearRTN(samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_diffuse_reflectance_samples; i++) {
		nextNode = current->nextDiffuseR[i];
		clearRTN(samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_diffuse_transmittance_samples; i++) {
		nextNode = current->nextDiffuseT[i];
		clearRTN(samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_specular_reflectance_samples; i++) {
		nextNode = current->nextSpecularR[i];
		clearRTN(samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_specular_transmittance_samples; i++) {
		nextNode = current->nextSpecularT[i];
		clearRTN(samples, nextNode, base, level + 1, nLevels);
	}
	return (1);
}

int
printRTN(FILE * fp, Samples * samples, RTN * current, RTN * base, int level, int nLevels)
{
	RTN            *nextNode;
	char            str[MAX_TREE_DEPTH + 1];
	int             i, j;

	if (!current)
		return (0);
	if (current->type == RTN_NULL)
		return (0);

	for (i = 0; i < level; i++)
		str[i] = '-';
	str[level] = '\0';

	fprintf(fp, "%sRTN level %d / %d\n", str, level, nLevels);
	fprintf(fp, "%sreflectance / %d : ", str, samples->nBands);
	for (j = 0; j < samples->nBands; j++)
		fprintf(fp, "%f ", current->refl[j]);
	fprintf(fp, "\n");
	if (current->type == RTN_LEAF || current->type == RTN_LEAF_SKY) {
		fprintf(fp, "%sirradiance / %d : ", str, samples->nBands);
		for (j = 0; j < samples->nBands; j++)
			fprintf(fp, "%f ", current->irrad[j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "%srtd : %d\n", str, current->rtd);
	fprintf(fp, "%sstyle : %d\n", str, current->style);
	fprintf(fp, "%srayLength : %f\n", str, current->rayLength);
	fprintf(fp, "%sattenuation : %f\n", str, current->attenuation);
	fprintf(fp, "%stype : %d\n", str, current->type);
	fprintf(fp, "%smaterial : %d\n", str, current->material);
	fprintf(fp, "%sreflTrans : %d\n", str, current->reflTrans);

	if (current->type == RTN_LEAF || current->type == RTN_LEAF_SKY)
		return (1);
	if (!current->nextShadow)
		return (0);
	for (i = 0; i < samples->no_of_shadow_rays; i++) {
		fprintf(fp, "%sRTN shadow ray %d\n", str, i);
		nextNode = current->nextShadow[i];
		printRTN(fp, samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_diffuse_reflectance_samples; i++) {
		fprintf(fp, "%sRTN diffuseR ray %d\n", str, i);
		nextNode = current->nextDiffuseR[i];
		printRTN(fp, samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_diffuse_transmittance_samples; i++) {
		fprintf(fp, "%sRTN diffuseT ray %d\n", str, i);
		nextNode = current->nextDiffuseT[i];
		printRTN(fp, samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_specular_reflectance_samples; i++) {
		fprintf(fp, "%sRTN specularR ray %d\n", str, i);
		nextNode = current->nextSpecularR[i];
		printRTN(fp, samples, nextNode, base, level + 1, nLevels);
	}
	for (i = 0; i < samples->no_of_specular_transmittance_samples; i++) {
		fprintf(fp, "%sRTN specularT ray %d\n", str, i);
		nextNode = current->nextSpecularT[i];
		printRTN(fp, samples, nextNode, base, level + 1, nLevels);
	}
	return (1);
}

int
setUpRTN(Samples * samples, RTN * current, RTN * base, int level, int nLevels)
{
	RTN            *nextNode;
	int             i, j;

	if (level == nLevels - 1)
		return (rtnIndex);
	if (!current->refl)
		current->refl = v_allocate(samples->nBands, sizeof(double));
	if (!current->irrad) {
		current->irrad = v_allocate(samples->nBands, sizeof(double));
		current->rtd = 1;
		current->style = 0;
	}
	if (samples->no_of_shadow_rays)
		current->nextShadow = (RTN **) v_allocate(samples->no_of_shadow_rays, sizeof(RTN *));
	for (i = 0; i < samples->no_of_shadow_rays; i++) {
		nextNode = &(base[++rtnIndex]);
		current->nextShadow[i] = nextNode;
		nextNode->prev = current;
		nextNode->refl = v_allocate(samples->nBands, sizeof(double));
		nextNode->irrad = v_allocate(samples->nBands, sizeof(double));
		nextNode->rtd = level + 1;
		nextNode->style = RTN_DIRECT;
		setUpRTN(samples, nextNode, base, level + 1, nLevels);
	}
	if (samples->no_of_diffuse_reflectance_samples)
		current->nextDiffuseR = (RTN **) v_allocate(samples->no_of_diffuse_reflectance_samples, sizeof(RTN *));
	for (i = 0; i < samples->no_of_diffuse_reflectance_samples; i++) {
		nextNode = &(base[++rtnIndex]);
		current->nextDiffuseR[i] = nextNode;
		nextNode->prev = current;
		nextNode->refl = v_allocate(samples->nBands, sizeof(double));
		nextNode->irrad = v_allocate(samples->nBands, sizeof(double));
		nextNode->rtd = level + 1;
		nextNode->style = RTN_DIFFUSE;
		setUpRTN(samples, nextNode, base, level + 1, nLevels);
	}
	if (samples->no_of_diffuse_transmittance_samples)
		current->nextDiffuseT = (RTN **) v_allocate(samples->no_of_diffuse_transmittance_samples, sizeof(RTN *));
	for (i = 0; i < samples->no_of_diffuse_transmittance_samples; i++) {
		nextNode = &(base[++rtnIndex]);
		current->nextDiffuseT[i] = nextNode;
		nextNode->prev = current;
		nextNode->refl = v_allocate(samples->nBands, sizeof(double));
		nextNode->irrad = v_allocate(samples->nBands, sizeof(double));
		nextNode->rtd = level + 1;
		nextNode->style = RTN_DIFFUSE;
		setUpRTN(samples, nextNode, base, level + 1, nLevels);
	}
	if (samples->no_of_specular_reflectance_samples)
		current->nextSpecularR = (RTN **) v_allocate(samples->no_of_specular_reflectance_samples, sizeof(RTN *));
	for (i = 0; i < samples->no_of_specular_reflectance_samples; i++) {
		nextNode = &(base[++rtnIndex]);
		current->nextSpecularR[i] = nextNode;
		nextNode->prev = current;
		nextNode->refl = v_allocate(samples->nBands, sizeof(double));
		nextNode->irrad = v_allocate(samples->nBands, sizeof(double));
		nextNode->rtd = level + 1;
		nextNode->style = RTN_SPECULAR;
		setUpRTN(samples, nextNode, base, level + 1, nLevels);
	}
	if (samples->no_of_specular_transmittance_samples)
		current->nextSpecularT = (RTN **) v_allocate(samples->no_of_specular_transmittance_samples, sizeof(RTN *));
	for (i = 0; i < samples->no_of_specular_transmittance_samples; i++) {
		nextNode = &(base[++rtnIndex]);
		current->nextSpecularT[i] = nextNode;
		nextNode->prev = current;
		nextNode->refl = v_allocate(samples->nBands, sizeof(double));
		nextNode->irrad = v_allocate(samples->nBands, sizeof(double));
		nextNode->rtd = level + 1;
		nextNode->style = RTN_SPECULAR;
		setUpRTN(samples, nextNode, base, level + 1, nLevels);
	}
	return (rtnIndex);
}


int
allocateRTN(Samples * samples, int nBands)
{
	int             i, j, k, nLevels, nNodes;
	RTN            *current, *nextRTN;

	nLevels = MAX(3, 2 + MAX(samples->max_diffuse_sampling_depth, samples->max_specular_sampling_depth));
	samples->nBands = nBands;
	samples->noOfNodes = (int *) v_allocate(nLevels, sizeof(int));
	samples->cumulativeNoOfNodes = (int *) v_allocate(nLevels, sizeof(int));
	for (i = 1; i < nLevels; i++) {
		samples->noOfNodes[i] = pow(samples->max_no_of_ray_samples, i - 1);
		/* cumulative - before rtd i */
		samples->cumulativeNoOfNodes[i] = samples->cumulativeNoOfNodes[i - 1] + samples->noOfNodes[i - 1];
	}

	nNodes = samples->cumulativeNoOfNodes[nLevels - 1] + samples->noOfNodes[nLevels - 1];

	samples->DirectBase = (RTN *) v_allocate(nNodes, sizeof(RTN));
	samples->DiffuseBase = (RTN *) v_allocate(nNodes, sizeof(RTN));

	current = samples->DirectBase;
	rtnIndex = 0;
	setUpRTN(samples, current, samples->DirectBase, 1, nLevels);
	current = samples->DiffuseBase;
	rtnIndex = 0;
	setUpRTN(samples, current, samples->DiffuseBase, 1, nLevels);
	return (1);
}

int
storeCurrentRTN(MaterialBag * materialbag, Material_table * materialPtr)
{
	int             i, whichMaterial;
	RTN            *current;
	Standard_Material_List *mat;

	whichMaterial = *(materialPtr->material);

	mat = &materialbag->material_list->material[whichMaterial];

	current = materialbag->samples->currentDirect;
	for (i = 0; i < materialbag->samples->nBands; i++) {
		current->refl[i] = 0;
		current->irrad[i] = 0;
	}
}
