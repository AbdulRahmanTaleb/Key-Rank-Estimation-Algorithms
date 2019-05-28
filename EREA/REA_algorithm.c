#include "Libraries.h"
#include "REA_algorithm.h"
#include "Heap.h"
#include "Key_volume.h"

void print_uint128(uint128 n)
{
    if (n == 0) {
      return;
    }

    print_uint128(n/10);
    putchar(n%10+'0');
}

double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return (tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L));
}

REA init_REA(double ** proba_distributions, unsigned int nb_dimensions, uint64 * size_dimensions, unsigned int * secret_key, int initial_heap_size){
	REA r = (REA)malloc(sizeof(REA_h));
	
	if(r == NULL){
		fprintf(stderr, "Allocation Error\n");
		exit(1);
	}
	
	r->secret_key = secret_key;
	r->proba_distributions = proba_distributions;
	r-> nb_dimensions = nb_dimensions;
	r->size_dimensions = size_dimensions;
	r->heap = init_heap(initial_heap_size);
	
	r->intervalI = (uint128 *)malloc(3 * sizeof(uint128));
	r->intervalI[0] = 1;
	r->intervalI[1] = 0;
	r->intervalI[2] = 0;
	r->secret_key_proba = 1;
	normalize_probas(r);

	for(unsigned int i=0; i< r->nb_dimensions; i++){
		r->intervalI[0] = r->intervalI[0] * (r->size_dimensions[i]);
		r->secret_key_proba = r->secret_key_proba * proba_distributions[i][secret_key[i]];
	}
	//printf("key proba = %.100lf\n", r->secret_key_proba);
	return r;
}


void merge_d(REA rea, uint64 maxSize){
	unsigned int tmp = rea->nb_dimensions;
	uint64 size = 0;
	unsigned index = 0;
	double ** probas = (double **)malloc(tmp * sizeof(double *));
	
	for(unsigned i =0; i< tmp; i++){
		if(size == 0){
			probas[index] = (double *)malloc(maxSize * sizeof(double));
			for(uint64 j=0; j< rea->size_dimensions[i]; j++){
				probas[index][j] = rea->proba_distributions[i][j];
				size++;
			}
		}
		else if((rea->size_dimensions[i] * size) > maxSize){
			rea->size_dimensions[index] = size;
			index++;
			size = 0;
			probas[index] = (double *)malloc(maxSize * sizeof(double));
			for(uint64 j=0; j< rea->size_dimensions[i]; j++){
				probas[index][j] = rea->proba_distributions[i][j];
				size++;
			}
		}
		else{
			double * t = (double *)malloc(rea->size_dimensions[i] * size * sizeof(double));
			uint128 size_tmp = 0;
			for(uint64 j = 0; j< rea->size_dimensions[i]; j++){
				for(uint64 k=0; k<size; k++){
					t[size_tmp] = rea->proba_distributions[i][j] * probas[index][k];
					size_tmp++;
				}
			}
			size = size_tmp;
			free(probas[index]);
			probas[index] = t;
		}
		
		free(rea->proba_distributions[i]);
	}
	rea->size_dimensions[index] = size;
	free(rea->proba_distributions);
	rea->proba_distributions = probas;
	index++;
	uint64 * sizes = (uint64 *)malloc(index * sizeof(uint64));
	memcpy(sizes, rea->size_dimensions, index * sizeof(uint64));
	rea->nb_dimensions = index;
	free(rea->size_dimensions);
	rea->size_dimensions = sizes;
}

uint128 * run_REA(REA rea, double nbSeconds){
	
	sort_probas(rea);
	Heap h = rea->heap;
	push(h, init_KeyV(rea->nb_dimensions, rea->size_dimensions));
	double start = my_gettimeofday();
	int carved_part;
	KeyVolume k;
	while((! is_empty(h)) && ((my_gettimeofday() - start) < nbSeconds)){
		k = pop(h);
		
		if(! is_carved(k)){
			
			uint128 carvedVolume = carve(rea->proba_distributions, k, rea->secret_key_proba, rea->intervalI);
			
			carved_part = get_carved_part(k);
			
			if(carved_part < 0){
				rea->intervalI[carved_part + 1] -= carvedVolume;
			}
			else{
				rea->intervalI[carved_part + 1] += carvedVolume;
			}
			
			if(get_volume(k) > 0){
				push(h, k);
			}
			else{
				free_KeyV(&k);
				k = NULL;
			}
			
		}
		else{
			push(h, split(k));
			push(h, k);
		}
		
	}
	
	return rea->intervalI;
	
}

int compare(void const * a, void const *b){
	double const *const *pa = a;
	double const *const *pb = b;
	return *pa < *pb;
}

void sort_probas(REA rea){
	for(unsigned int i=0; i< rea->nb_dimensions; i++){
		qsort(rea->proba_distributions[i], rea->size_dimensions[i], sizeof(double), &compare);
	}
}

void normalize_probas(REA rea){
	double tot;
	for(unsigned int i=0; i< rea->nb_dimensions; i++){
		tot = 0;
		for(uint64 j=0; j< rea->size_dimensions[i]; j++){
			tot += rea->proba_distributions[i][j];
		}
		for(uint64 j=0; j< rea->size_dimensions[i]; j++){
			rea->proba_distributions[i][j] = (rea->proba_distributions[i][j]*1.0) / tot;
		}
	}
}

void free_REA(REA * rea){
	for(unsigned int i=0; i< (*rea)->nb_dimensions; i++){
		free((*rea)->proba_distributions[i]);
	}
	
	free((*rea)->proba_distributions);
	
	/*if((*rea)->secret_key){
		free((*rea)->secret_key);
	}*/
	
	free((*rea)->intervalI);

	free((*rea)->size_dimensions);

	
	free_heap(&((*rea)->heap));
	free(*rea);
	*rea = NULL;
}
