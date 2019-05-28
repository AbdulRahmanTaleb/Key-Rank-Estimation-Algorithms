#ifndef EREA_H_
#define EREA_H_

#include "Libraries.h"
#include "REA_algorithm.h"

typedef struct{
	REA rea;
	double time;
	uint128 samples;
	uint128 q;
	uint128 actual_samples;
	double mean;
	double * confidence_levels;
	double * confidence_zvalues;
}EREA_h;

typedef EREA_h * EREA;

EREA init_EREA(double ** proba_distributions, unsigned int nb_dimensions, uint64 * size_dimensions, unsigned int * secret_key, int initial_heap_size, uint64 samples, double time);

uint128 * run_EREA(EREA erea, double * time);

uint128 binomial(uint128 n, double proba);

double * compute_confidence_interval(EREA erea, double confidence_value);

void merge_dimensions(EREA erea, uint128 maxSize);

double get_zvalue(EREA erea, double confidence_level);

void free_EREA(EREA * erea);
		

#endif
