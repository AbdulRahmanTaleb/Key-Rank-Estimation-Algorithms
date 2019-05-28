#include "EREA.h"

double my_get(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return (tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L));
}


EREA init_EREA(double ** proba_distributions, unsigned int nb_dimensions, uint64 * size_dimensions, unsigned int * secret_key, int initial_heap_size, uint64 samples, double time){
	EREA e = (EREA)malloc(sizeof(EREA_h));
	if(e == NULL){
		fprintf(stderr, "Allocation Error\n");
		exit(1);
	}
	e->samples = samples;
	e->time = time;
	
	e->rea = init_REA(proba_distributions, nb_dimensions, size_dimensions, secret_key, initial_heap_size);
	e->q = 0;
	e->actual_samples = 0;
	e->mean = 0;
	e->confidence_levels = (double *)malloc(7 * sizeof(double));
	e->confidence_zvalues = (double *)malloc(7 * sizeof(double));
	
	e->confidence_levels[0] = 0.8; e->confidence_levels[1] = 0.85; e->confidence_levels[2] = 0.9;
	e->confidence_levels[3] = 0.95; e->confidence_levels[4] = 0.99; e->confidence_levels[5] = 0.995; e->confidence_levels[6] = 0.999;
	
	e->confidence_zvalues[0] = 1.282; e->confidence_zvalues[1] = 1.44; e->confidence_zvalues[2] = 1.645;
	e->confidence_zvalues[3] = 1.96; e->confidence_zvalues[4] = 2.576; e->confidence_zvalues[5] = 2.807;
	e->confidence_zvalues[6] = 3.291;
	
	return e;
}


uint128 * run_EREA(EREA erea, double * time){
	/*run the REA algorithm for a given amount of time*/
	
	uint128 * interval = run_REA(erea->rea, erea->time);
	
	uint128 remaining = (erea->rea->intervalI[0] - erea->rea->intervalI[2] + 1);
	if(remaining==0){
		erea->q = 0; erea->mean=0;
		return interval;
	}
	uint128 nv;
	uint64 q = 0;
	KeyVolume k;
	uint128 total = 0;
	printf("starts sampling\n");
	unsigned cpt = 0;
	int bool = 0;
	uint128 s;
	
	//start the sampling and statistical process*/
	/*iterate over all remaining volumes*/
	double debut = my_get();
	while(! is_empty(erea->rea->heap)){
		k = pop(erea->rea->heap);
		while(is_carved(k)){
			cpt++;
			push(erea->rea->heap, split(k));
		}
		
		if(k->volume == 0){
			k->volume = k->volume - 1;
		}
		double tmp = ((double)(k->volume)) / ((double)remaining);
		nv = binomial(erea->samples, tmp);

		for(uint128 i=0; i< nv; i++){
			double p = pick_point_proba(k, erea->rea->proba_distributions);
			if(erea->rea->secret_key_proba <= p){
				q = q + 1;
			}
		}
		free_KeyV(&k);
		total += nv;
	}
	double fin = my_get();
	*time = fin - debut;
	if(q==0){
		erea->q = 0;
		erea->mean=0;	
		return interval;
	}
	double mean = (double)(q) /  (double)(total);
	erea->actual_samples = total;
	erea->q = q;
	erea->mean = mean;
	return interval;
}

double * compute_confidence_interval(EREA erea, double confidence_value){
	uint128 remaining = (erea->rea->intervalI[0] - erea->rea->intervalI[2] + 1);
	double * inter = (double *)malloc(2 * sizeof(double));

	if(erea->q == 0){
		inter[0] = erea->rea->intervalI[2];
		inter[1] = erea->rea->intervalI[0];
		return inter;
	}
	
	double z_value = get_zvalue(erea, confidence_value);
	if(z_value == -1){
		printf("Incorrect Confidence Level\n");
		return NULL;
	}
	
	double fact = 1.0 / (1.0 + ((1.0 / (double)(erea->samples))*z_value*z_value));
	
	double sqrt_fact = (1.0/ (double)(erea->samples)) * (erea->mean) * (1.0 - erea->mean);
	sqrt_fact = sqrt_fact + ((1.0 / (4.0 * (double)(erea->samples) * (double)(erea->samples)))*z_value*z_value);
	sqrt_fact = sqrt(sqrt_fact) * z_value;
	
	double value_tobe_summed = (1.0 / (2.0 * (double)(erea->samples))) * z_value * z_value;
	
	inter[0] = fact * (erea->mean + value_tobe_summed - sqrt_fact);
	inter[0] = ((double)(remaining) * inter[0]) + (double)(erea->rea->intervalI[2]);

	inter[1] = fact * (erea->mean + value_tobe_summed + sqrt_fact);
	inter[1] = ((double)(remaining) * inter[1]) + (double)(erea->rea->intervalI[2]);
	
	return inter;
}

void merge_dimensions(EREA erea, uint128 maxSize){
	merge_d(erea->rea, maxSize);
}

uint128 binomial(uint128 n, double proba){
	int flag = 0;
	if(proba > 0.5){
		proba = 1 - proba;
		flag = 1;
	}
	double log_q = log(1.0 - proba);
	uint128 x = 0;
	double sum = 0;
	for(;;) {
		sum += (log(((double) rand() / (RAND_MAX))) / (n - x));
		if(sum < log_q) {
			if(flag == 1){
				return n-x;
			}
			else{
				return x;
			}
		}
		x++;
	}
	printf("error binomial \n");
	return 0;
}

double get_zvalue(EREA erea, double confidence_level){
	for(int i=0; i<7; i++){
		if(erea->confidence_levels[i] == confidence_level){
			return erea->confidence_zvalues[i];
		}
	}
	
	return -1;
}


void free_EREA(EREA * erea){
	free_REA(&((*erea)->rea));
	free(*erea);
	*erea = NULL;
}
