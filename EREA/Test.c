#include "Libraries.h"
#include "Heap.h"
#include "EREA.h"

typedef __uint128_t uint128;

#define HEAP_SIZE 100000

int main(int argc, char ** argv){

	srand(time(NULL));
	double ** proba_distributions;
	
	
	FILE * f = fopen(argv[1], "r");
	unsigned int it, jt;
	fscanf(f, " %d %d", &it, &jt);
	proba_distributions = (double **)malloc(it * sizeof(double *));
	uint64 * size_dimensions = (uint64 *)malloc(it * sizeof(uint64));
	
	for(int i=0; i< it; i++){
		size_dimensions[i] = jt;
		proba_distributions[i] = (double *)malloc(jt * sizeof(double));
		for(int j=0; j<jt; j++){
			fscanf(f, " %lf", &proba_distributions[i][j]);
		}
	}

	unsigned int secret_key[] = {0x4d, 0xfb, 0xe0, 0xf2, 0x72, 0x21, 0xfe, 0x10, 0xa7, 0x8d, 0x4a, 0xdc, 0x8e, 0x49, 0x04, 0x69};

	/*Number of samples	*/
	int samp = 24;
	uint64 samples = 1;
	
	samples = samples << samp;
	double time = atof(argv[2]);
	double confidence_value = 0.9;
	unsigned int nb_dimensions = 16;
	EREA erea = init_EREA(proba_distributions, nb_dimensions, size_dimensions, secret_key, HEAP_SIZE, samples, time);
	REA rea = erea->rea;
	
	
	/*Merging Dimensions*/
	uint128 max = 1;
	int powed = 24;
	max = max << powed;
	merge_dimensions(erea, max);
	
	printf("nb_dimensions = %d\n", rea->nb_dimensions);
	for(unsigned i = 0; i<rea->nb_dimensions; i++){
		printf("size = %llu\n", rea->size_dimensions[i]);
	}
	/*End*/
	
	/*Running EREA Basic Algorithm Without Computing Confidence Interval*/
	double dur;
	uint128 * inter = run_EREA(erea, &dur);

	printf("Interval (log2) : [ %lf, %lf, %lf ]\n", log((double)inter[2])/log(2.0),log((double)inter[1])/log(2.0),log((double)inter[0])/log(2.0));
	/* End */
	
	uint128 remain = inter[0] - inter[2] + 1;
		
	/*	Computing Confidence Intervals	*/
	double values[7] = {0.8, 0.85, 0.9, 0.95, 0.99, 0.995, 0.999};

	for(int i=0; i<7; i++){
		double * conf = compute_confidence_interval(erea, values[i]);
		printf("%.2lf%% Confidence Interval (log2) = [ %lf, %lf ]\n", values[i]*100, log2(conf[0]) , log2(conf[1]));
		
		free(conf);
	}
	
	free_EREA(&erea);
	return 0;
	
}
