#include <stdlib.h>

int seed_randomise(int i){
    srand(i);
    return(1);
}


double Random(){
  return(1.*rand()/(RAND_MAX+1.0));
}
#ifdef MAIN

/* test driver	prints out either NMR_RAND values or a histogram	*/

#include <stdio.h>

#define NMR_RAND	5000
#define MAX_BINS	500

#ifdef NO_PROTO
void main(argc, argv)
int argc;
char **argv;
#else
void main(int argc, char **argv)
#endif
{
	int j,k,nmr_bins,seed;
	int bins[MAX_BINS];
	double randm, bin_inc;
	double bin_limit[MAX_BINS];

	if ( argc != 3 )
	{
		printf("Usage -- %s nmr_bins seed\n", argv[0]);
		exit(1);
	}

	nmr_bins = atoi( argv[1] );
	if ( nmr_bins > MAX_BINS )
	{
		printf("ERROR -- maximum number of bins is %d\n", MAX_BINS);
		exit(1);
	}

	seed = atoi( argv[2] );

	r250_init( seed );

	if ( nmr_bins < 1 )	/* just print out the numbers */
	{
		for (j = 0; j < NMR_RAND; j++)
			printf("%f\n", dr250() );
		exit(0);
	}
	
	bin_inc = 1.0 / nmr_bins;
	for (j = 0; j < nmr_bins; j++)	/* initialize bins to zero */
	{
		bins[j] = 0;
		bin_limit[j] = (j + 1) * bin_inc;
	}

	bin_limit[nmr_bins-1] = 1.0e7;	/* make sure all others are in last bin */

	for (j = 0; j < NMR_RAND; j++)
	{
		randm = r250() / (double)ALL_BITS;
		for (k = 0; k < nmr_bins; k++)
			if ( randm < bin_limit[k] )
			{
				bins[k]++;
				break;
			}
	}


	for (j = 0; j < nmr_bins; j++)
		printf("%d\n", bins[j]);
	
}

#endif

