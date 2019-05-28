#include "Key_volume.h"

KeyVolume init_KeyV(unsigned int nb_dimensions, uint64 * size_dimensions){
	KeyVolume k = (KeyVolume)malloc(sizeof(Key_volume_h));
	
	k->nb_dimensions = nb_dimensions;
	k->size_dimensions = size_dimensions;
	k->carved = 0;
	k->carved_part = 0;
	k->carvedVolume = 0;
	k->ratios = (double *)malloc(nb_dimensions * sizeof(double));
	k->topCorner = (unsigned *)malloc(nb_dimensions * sizeof(unsigned));
	k->bottomCorner = (unsigned *)malloc(nb_dimensions * sizeof(unsigned));
	
	for(unsigned i=0; i< nb_dimensions; i++){
		k->topCorner[i] = 0;
		k->bottomCorner[i] = size_dimensions[i] - 1;
	}
	
	k->carvingPoint = copy_point(k->topCorner, k->nb_dimensions);
	compute_volume(k);
	return k;
}


KeyVolume init_KeyV_points(unsigned * topC, unsigned * bottomC, int nb_dimensions){
	KeyVolume k = (KeyVolume)malloc(sizeof(Key_volume_h));
	
	k->nb_dimensions = nb_dimensions;
	k->carved = 0;
	k->carved_part = 0;
	k->carvedVolume = 0;
	k->ratios = (double *)malloc(nb_dimensions * sizeof(double));
	k->topCorner = topC;
	k->bottomCorner = bottomC;
	
	k->carvingPoint = copy_point(k->topCorner, nb_dimensions);
	k->size_dimensions = NULL;
	compute_volume(k);
	return k;
}

unsigned * copy_point(unsigned * point, uint128 size){
	unsigned * p = (unsigned *)malloc(size * sizeof(unsigned));
	memcpy(p, point, size * sizeof(unsigned));
	return p;
}


int is_carved(KeyVolume k){
	return k->carved;
}


KeyVolume split(KeyVolume k){
	unsigned argMax = 0;
	//choose the dimension to maximize the resulting split volume
	double * ratios = k->ratios;
	if(k->carved_inside){
		for(unsigned i=0; i<k->nb_dimensions; i++){
			ratios[i] = ((k->bottomCorner[i] - k->carvingPoint[i])*1.0) / (k->bottomCorner[i] - k->topCorner[i] + 1 );
			if(ratios[i] > ratios[argMax]){
				argMax = i;
			}
		}
	}
	else{
		for(unsigned i=0; i<k->nb_dimensions; i++){
			ratios[i] = ((k->carvingPoint[i] - k->topCorner[i])*1.0) / (k->bottomCorner[i] - k->topCorner[i] + 1 );
			if(ratios[i] > ratios[argMax]){
				argMax = i;
			}
		}
	}
	
	//do the split by updating the top and bottom corners
	unsigned * topC = copy_point(k->topCorner, k->nb_dimensions);
	unsigned * botC = copy_point(k->bottomCorner, k->nb_dimensions);
	
	if(k->carved_inside){
		k->bottomCorner[argMax] = k->carvingPoint[argMax];
		topC[argMax] = (k->carvingPoint[argMax]) + 1;
	}
	else{
		k->topCorner[argMax] = k->carvingPoint[argMax];
		botC[argMax] = (k->carvingPoint[argMax]) - 1;
	}
	
	ratios[argMax] = 0;
	int test = 0;
	for(int i=0; i< k->nb_dimensions; i++){
		if(ratios[i] != 0){
			test++;
			argMax = i;
		}
	}
	//if there is only 1 split dimension left, the carved volume becomes a full one;
	if(test == 1){
		if(k->carved_inside){
			k->topCorner[argMax] = k->carvingPoint[argMax] + 1;
		}
		else{
			k->bottomCorner[argMax] = k->carvingPoint[argMax] - 1;
		}
		k->carved = 0;
		k->carvedVolume = 0;
		free(k->carvingPoint);
		k->carvingPoint = copy_point(k->topCorner, k->nb_dimensions);
	}
	//compute the new volume of k and return the other resulting keyVolume
	compute_volume(k);
	return init_KeyV_points(topC, botC, k->nb_dimensions);
}

uint128 carve(double ** proba_distributions, KeyVolume k, double secret_key_proba, uint128 * intervalI){
	const unsigned trialsCredit = 600;
	const unsigned restartCredit = 6;
	double bestLogInterval = -1;

	/*	Hill-Climbing ALgorithm	*/
	for(unsigned iSide = 0; iSide<2; ++iSide){
		int fromTop = iSide;
		
		for(unsigned iRestart = restartCredit; iRestart-- > 0;){
			
			unsigned * trialCarvePoint = copy_point(fromTop ? k->topCorner: k->bottomCorner, k->nb_dimensions);
			
			int cornerSign = compute_carved_part(secret_key_proba, getProba(k, proba_distributions, trialCarvePoint));
			unsigned * nextCarvePoint = copy_point(trialCarvePoint, k->nb_dimensions);
			unsigned index;
			int increment;
			increment = 2 * fromTop - 1;
			unsigned dimIncrement[k->nb_dimensions];
			
			for(unsigned i=0; i< k->nb_dimensions; ++i){
				dimIncrement[i] = k->bottomCorner[i] - k->topCorner[i] + 1;
			}
			
			int improved = 1;
			int trials = trialsCredit;
			while(trials >= 0){
				
				index = rand() % k->nb_dimensions;
				nextCarvePoint[index] += (increment * dimIncrement[index]);
				
				improved = nextCarvePoint[index] >= k->topCorner[index]
							&& nextCarvePoint[index] <= k->bottomCorner[index]
							&& cornerSign == compute_carved_part(secret_key_proba, getProba(k, proba_distributions, nextCarvePoint));
				
				if(! improved){
					--trials;
					nextCarvePoint[index] -= (increment * dimIncrement[index]);
					dimIncrement[index] >>= 1;
					dimIncrement[index] |= 1;
					
				}
				else{
					trials = trialsCredit;
				}
			}
			
			improved = 0;
			for(unsigned i=0; i< k->nb_dimensions; i++){
				if(nextCarvePoint[i] != trialCarvePoint[i]){
					improved = 1;
					goto cnt;
				}
			}
			cnt:
			free(trialCarvePoint);
			trialCarvePoint = nextCarvePoint;
			
			uint128 trialVolume = 1;
			uint128 tmp;
			for(unsigned i=0; i< k->nb_dimensions; i++){
				trialVolume *= (fromTop ? (nextCarvePoint[i] - k->topCorner[i] + 1):(k->bottomCorner[i] - nextCarvePoint[i] + 1));
			}
			double trialLogInterval = 0;
			
			if(fromTop){
				trialLogInterval  =  log((double)(intervalI[0]));
				if(trialLogInterval < 0){
					trialLogInterval = 128;
				}
				trialLogInterval -= log((double)(intervalI[2] + trialVolume));
			}
			else{
				trialLogInterval = log((double)((intervalI[0]) - trialVolume)) - log((double)(intervalI[2]));
			}
			if(bestLogInterval < 0 || trialLogInterval < bestLogInterval){
				bestLogInterval = trialLogInterval;
				k->carved = 1;
				k->carved_part = cornerSign;
				k->carved_inside = fromTop;
				free(k->carvingPoint);
				k->carvingPoint = trialCarvePoint;
				iRestart = restartCredit;
			}
		}
		
	}
	compute_volume(k);
	uint128 carvedVolume = k->carvedVolume;
	unsigned numDiff = 0;
	for(unsigned i =0; i< k->nb_dimensions; ++i){
		numDiff += (k->carvingPoint[i] != ((k->carved_inside)? k->bottomCorner[i]: k->topCorner[i]));
	}
	if(numDiff == 1){
		unsigned index = 0;
		for(unsigned i=0; i< k->nb_dimensions; ++i){
			if(k->carvingPoint[i] != ((k->carved_inside) ? k->bottomCorner[i] : k->topCorner[i])){
				index = i;
			}
		}
		k->carved = 0;
		k->carvedVolume = 0;
		if(k->carved_inside){
			k->topCorner[index] = k->carvingPoint[index] + 1;
		}
		else{
			k->bottomCorner[index] = k->carvingPoint[index] - 1;
		}
		free(k->carvingPoint);
		k->carvingPoint = copy_point(k->topCorner, k->nb_dimensions);
		compute_volume(k);
	}
	return carvedVolume;
}

uint128 get_volume(KeyVolume k){
	return k->volume;
}

int get_carved_part(KeyVolume k){
	return k->carved_part;
}

void compute_volume(KeyVolume k){
	k->volume = 1;
	uint128 tmp;
	for(unsigned i =0; i< k->nb_dimensions; i++){
		k->volume *= (k->bottomCorner[i] - k->topCorner[i] + 1);
	}
	if(k->carved){
		k->carvedVolume = 1;
		if(k->carved_inside){
			for(unsigned i=0; i< k->nb_dimensions; i++){
				k->carvedVolume *= (k->carvingPoint[i] - k->topCorner[i] + 1);
			}
		}
		else{
			for(unsigned i=0; i< k->nb_dimensions; i++){
				k->carvedVolume *= (k->bottomCorner[i] - k->carvingPoint[i] + 1);
			}
		}
		
		k->volume -= k->carvedVolume;
	}
}


int compute_carved_part(double secret_key_proba, double key_proba){
	double diff = key_proba - secret_key_proba;
	return diff > 0 ? 1 : (diff < 0 ? -1 : 0);
}

/*return proba of a given key*/
double getProba(KeyVolume k, double ** proba_distributions, unsigned * key){
	
	double p = 1;
	for(unsigned i=0; i< k->nb_dimensions; i++){
		p = p * proba_distributions[i][key[i]];
	}
	return p;
	
}

/*pick a random point in the corresponding key Volume*/
double pick_point_proba(KeyVolume k, double ** proba_distributions){
	double proba = 1;
	for(unsigned i =0; i< k->nb_dimensions; i++){
		unsigned tmp = k->topCorner[i] + (rand() % (k->bottomCorner[i] - k->topCorner[i] + 1));
		proba *= proba_distributions[i][tmp];
	}
	return proba;
}

void free_KeyV(KeyVolume * k){
	if((*k)->topCorner){
		free((*k)->topCorner);
	}
	
	if((*k)->bottomCorner){
		free((*k)->bottomCorner);
	}
	
	if((*k)->carvingPoint){
		free((*k)->carvingPoint);
	}
	if((*k)->ratios){
		free((*k)->ratios);
	}
	
	free(*k);
}
