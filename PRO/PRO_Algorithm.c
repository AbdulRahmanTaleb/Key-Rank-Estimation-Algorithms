#include "PRO_Algorithm.h"
#include "AVL_BinarySearchTree.h"

PRO init_PRO(double ** proba_distributions, unsigned nb_dimensions, uint64 * size_dimensions, uint64 * secret_key, uint64 alpha){
	PRO p = (PRO)malloc(sizeof(struct PRO_al));
	
	if(p == NULL){
		fprintf(stderr, "error in allocation\n");
		exit(1);
	}
	
	p->proba_distributions = proba_distributions;
	p->nb_dimensions = nb_dimensions;
	p->size_dimensions = size_dimensions;
	p->secret_key = secret_key;
	p->alpha = alpha;
	p->intervalI = (double *)malloc(2 * sizeof(double));
	p->intervalI[0] = 0;
	p->intervalI[1] = 0;

	p->secret_key_proba = 1.0;
	normalize_probas(p);
	//sort_probas(p);
	
	for(unsigned i=0; i< p->nb_dimensions; i++){
		p->secret_key_proba =  p->secret_key_proba *  proba_distributions[i][secret_key[i]];
	}

	return p;
}

/*	Runs the algorithm	*/
double * run_PRO(PRO pro){
	double secret_key_proba_alpha = (pro->alpha) * log2(1/pro->secret_key_proba);
	
	Node avl = NULL;
	
	uint64 cpt = 0;
	int success;
	double tmp = 0;
			
	/*compute lower bound*/
	printf("computing Lower Bound\n");
	double ** proba_dist = (double **)malloc(pro->nb_dimensions * sizeof(double *));
	uint64 * size_dimensions = (uint64 *)malloc(pro->nb_dimensions * sizeof(uint64));
	
	for(unsigned i=0; i< pro->nb_dimensions; i++){
				
		for(uint64 j=0; j<pro->size_dimensions[i]; j++){
			
			tmp = ceil(pro->alpha * log2(1 / pro->proba_distributions[i][j]));
			success = 0;
			avl = insert(avl, tmp , &success);
			if(success == 1){
				cpt++;
			}
		}
		size_dimensions[i] = (uint64)(getMax(avl) + 1);
		proba_dist[i] = (double *)malloc(size_dimensions[i] * sizeof(double));
		memset(proba_dist[i], 0, size_dimensions[i] * sizeof(double));
		preOrder(avl, proba_dist[i]); 
		
		free_AVL(&avl);
		avl = NULL;
		cpt = 0;
	}
	
	printf("nb_dimensions = %u\n", pro->nb_dimensions);
	int s = 0;
	
	for(unsigned i = 0; i<pro->nb_dimensions; i++){
		printf("size of dimension %u = %llu\n", i, size_dimensions[i]);
	}
	distr(secret_key_proba_alpha, proba_dist, size_dimensions, pro->nb_dimensions, &pro->intervalI[0]);
	
	
	/*compute Upper bound*/
	printf("computing Upper Bound\n");

	memset(size_dimensions, 0, pro->nb_dimensions * sizeof(uint64));
	
	for(unsigned i=0; i< pro->nb_dimensions; i++){
				
		for(uint64 j=0; j<pro->size_dimensions[i]; j++){
			
			tmp = floor(pro->alpha * log2(1 / pro->proba_distributions[i][j]));
			//printf("tmp = %lf\n", tmp);
			success = 0;
			avl = insert(avl, tmp , &success);
			if(success == 1){
				cpt++;
			}
		}
		
		size_dimensions[i] = (uint64)(getMax(avl) + 1);
		proba_dist[i] = (double *)malloc(size_dimensions[i] * sizeof(double));
		memset(proba_dist[i], 0, size_dimensions[i] * sizeof(double));
		preOrder(avl, proba_dist[i]); 
		
		free_AVL(&avl);
		avl = NULL;
		cpt = 0;
	}
	
	printf("nb_dimensions = %u\n", pro->nb_dimensions);
	
	for(unsigned i = 0; i<pro->nb_dimensions; i++){
		printf("size of dimension %u = %llu\n", i, size_dimensions[i]);
	}
	
	distr(secret_key_proba_alpha, proba_dist, size_dimensions, pro->nb_dimensions, &pro->intervalI[1]);
	
	printf("End Computing\n\n");
	return pro->intervalI;
}

int compare(void const * a, void const *b){
	double const *const *pa = a;
	double const *const *pb = b;
	return *pa < *pb;
}

void sort_probas(PRO pro){
	for(uint128 i=0; i< pro->nb_dimensions; i++){
		qsort(pro->proba_distributions[i], pro->size_dimensions[i], sizeof(double), &compare);
	}
}


void normalize_probas(PRO pro){
	double tot;
	for(unsigned i=0; i< pro->nb_dimensions; i++){
		tot = 0;
		for(uint64 j=0; j< pro->size_dimensions[i]; j++){
			tot += pro->proba_distributions[i][j];
		}
		for(uint64 j=0; j< pro->size_dimensions[i]; j++){
			pro->proba_distributions[i][j] = (pro->proba_distributions[i][j]*1.0) / tot;
		}
	}
}

/*	Calculates the distribution for the value "secret key proba", of 
	the generalized polynomial resulting from the multiplication of all proba distributions	*/
void distr(double secret_key_proba, double ** probas, uint64 *  size_dimensions, unsigned numSets, double * res){
	uint128 deg_pol_res = 1;
	double * pol_res = (double *)malloc(sizeof(double));
	pol_res[0] = 1;
	
	uint128 dega, degb;
	double * a;
	clock_t start, end;
	double total;
	
	for(unsigned i=0; i< numSets; i++){
		a = probas[i];
		dega = size_dimensions[i] - 1;
		degb = deg_pol_res;
		pol_res = mult_pol(a, pol_res, dega, degb, &deg_pol_res);
	}
	
	for(uint128 j=0; j< deg_pol_res; j = j +1){
		if(j < secret_key_proba){
			*res = *res + (pol_res[j]);
		}
	} 	
}


/*	Multiplication of 2 polynomials (for sparse polynomials)	*/
double * mult_pol(double * a, double * b, uint128 na, uint128 nb, uint128 * nres){
	*nres = na + nb + 1;
	double * res = (double *)malloc(*nres * sizeof(double));
	memset(res,0, *nres * sizeof(double));
	
	for(uint128 i= 0; i<= na; i++){
		if(a[i] != 0){
			for(uint128 j=0; j<=  nb; j++){
				res[i+j] += a[i]*b[j];
			}	
		}
	}
	
	free(a);
	free(b);
	return res;
	
}

void free_PRO(PRO pro){
	free(pro);
}

