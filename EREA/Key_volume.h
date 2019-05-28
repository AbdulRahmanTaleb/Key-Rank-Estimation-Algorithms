#ifndef KEY_VOLUME_H_
#define KEY_VOLUME_H_

#include "Libraries.h"

typedef struct{
	unsigned int nb_dimensions;
	uint64 * size_dimensions;
	uint128 volume;
	unsigned * topCorner;
	unsigned * bottomCorner;
	unsigned * carvingPoint;
	int carved;				/*1 : volume is carved, 0: not carved*/
	int carved_part;		/*-1 : up, 0: equal, +1: down*/
	int carved_inside;		/*1 : the upper rectangle is carved, 0 :otherwise*/
	uint128 carvedVolume;	/*the amount of carved volume*/
	double * ratios;		/*variable to be used when split function is called*/
	
}Key_volume_h;

typedef Key_volume_h * KeyVolume;

KeyVolume init_KeyV(unsigned int nb_dimensions, uint64 * size_dimensions);

KeyVolume init_KeyV_points(unsigned * topC, unsigned * bottomC, int nb_dimensions);

int is_carved(KeyVolume k);

KeyVolume split(KeyVolume k);

uint128 get_volume(KeyVolume k);

int get_carved_part(KeyVolume k);

uint128 carve(double ** proba_distributions, KeyVolume k, double secret_key_proba, uint128 * intervalI);

void compute_volume(KeyVolume k);

/*return the index for the variable carved_part depending on the secret key proba and the carving point proba*/
int compute_carved_part(double secret_key_proba, double key_proba);

/*return proba of a given key*/
double getProba(KeyVolume k, double ** proba_distributions, unsigned * key);

/*clone a point*/
unsigned * copy_point(unsigned * point, uint128 size);

/*pick a random point in the corresponding key Volume*/
double pick_point_proba(KeyVolume k, double ** proba_distributions);

void free_KeyV(KeyVolume * k);
		

#endif
