
#ifndef HEAP_H_
#define HEAP_H_

#include "Libraries.h"
#include "Key_volume.h"

typedef struct{
	KeyVolume * heap_array;
	uint128 size;
	uint128 capacity;
}heap_h;

typedef heap_h * Heap;

Heap init_heap(uint128 initial_size);

uint128 get_size(Heap h);

void empty(Heap h);

int is_empty(Heap h);

int has_parent(Heap h, uint128 index);
int is_leaf(Heap h, uint128 index);
int has_right_child(Heap h, uint128 index);

void push(Heap h, KeyVolume i);

KeyVolume pop(Heap h);

void update(Heap h);

void free_heap(Heap * h);
		

#endif
