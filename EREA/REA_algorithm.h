#ifndef REA_ALGORITHM_H_
#define REA_ALGORITHM_H_

#include "Libraries.h"
#include "Heap.h"
#include "Key_volume.h"

typedef struct{
	Heap heap;
	double ** proba_distributions;
	unsigned int nb_dimensions;
	uint64 * size_dimensions;
	unsigned int * secret_key;
	uint128 * intervalI;
	double secret_key_proba;
}REA_h;

typedef REA_h * REA;

REA init_REA(double ** proba_distributions, unsigned int nb_dimensions, uint64 * size_dimensions, unsigned int * secret_key, int initial_heap_size);

uint128 * run_REA(REA rea, double nbSeconds);

void sort_probas(REA rea);
void normalize_probas(REA rea);

void merge_d(REA rea, uint64 maxSize);

void print_uint128(uint128 n);

void free_REA(REA * rea);
		

#endif
