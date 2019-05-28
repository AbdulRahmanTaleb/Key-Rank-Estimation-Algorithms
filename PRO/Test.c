#include "Libraries.h"
#include "AVL_BinarySearchTree.h"
#include "PRO_Algorithm.h"

double wtime()
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	return (double)ts.tv_sec + ts.tv_usec / 1E6;
}

int main(int argc, char ** argv){
	
	
	srand(time(NULL));
	double ** proba_distributions;
	
	
	FILE * f = fopen(argv[1], "r");
	unsigned it, jt;
	fscanf(f, " %d %d", &it, &jt);
	proba_distributions = (double **)malloc(it * sizeof(double *));
	uint64 * size_dimensions = (uint64 *)malloc(it * sizeof(uint64 ));
	
	for(int i=0; i< it; i++){
		size_dimensions[i] = jt;
		proba_distributions[i] = (double *)malloc(jt * sizeof(double));
		for(int j=0; j<jt; j++){
			fscanf(f, " %lf", &proba_distributions[i][j]);
		}
	}
	fclose(f);
	f = NULL;
	uint64 secret_key[] = {0x4d, 0xfb, 0xe0, 0xf2, 0x72, 0x21, 0xfe, 0x10, 0xa7, 0x8d, 0x4a, 0xdc, 0x8e, 0x49, 0x04, 0x69};
	
	/* Accuracy Parameter	*/
	uint64 alpha = 1;
	alpha = alpha << 13;
	printf("alpha = %lld\n", alpha);
	unsigned nb_dimensions = 16;
	
	PRO pro = init_PRO(proba_distributions, nb_dimensions, size_dimensions, secret_key, alpha);
	double debut = wtime();
	double * inter = run_PRO(pro);
	if(inter[0] == 0){
		inter[0] = 1;
	}
	double fin = wtime();
	printf("Interval (log2) = [%lf,  %lf]\n", log((double)inter[0])/log(2.0), log((double)inter[1])/log(2.0));
	
	free(inter);
	for(int i=0; i< it; i++){
		free(proba_distributions[i]);
	}
	free(proba_distributions);
	free(size_dimensions);
	free_PRO(pro);
	return 0;
}
