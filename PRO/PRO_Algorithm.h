#ifndef PRO_ALGORITHM_H_
#define PRO_ALGORITHM_H_

#include "Libraries.h"

// An AVL tree node 
struct PRO_al
{
	double ** proba_distributions;
	double secret_key_proba;
	
	unsigned nb_dimensions;
	uint64 * size_dimensions;
	uint64 * secret_key;
	double * intervalI;
	uint64 alpha;
}; 

typedef struct PRO_al * PRO;

/*	Initializes the structure	*/
PRO init_PRO(double ** proba_distributions, unsigned nb_dimensions, uint64 * size_dimensions, uint64 * secret_key, uint64 alpha);

void sort_probas(PRO pro);

/*	Runs the algorithm	*/
double * run_PRO(PRO pro);

void normalize_probas(PRO pro);

/*	Calculates the distribution for the value "secret key proba", of 
	the generalized polynomial resulting from the multiplication of all proba distributions	*/
void distr(double secret_key_proba, double ** probas, uint64 *  size_dimensions, unsigned numSets, double * res);

/*	Multiplication of 2 polynomials (for sparse polynomials)	*/
double * mult_pol(double * a, double * b, uint128 na, uint128 nb, uint128 * nres);

void free_PRO(PRO pro);



#endif

