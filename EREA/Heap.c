
#include "Heap.h"

Heap init_heap(uint128 initial_size){
	Heap h = (Heap)malloc(sizeof(heap_h));
	h->size = 0;
	h->capacity= initial_size;
	h->heap_array = (KeyVolume *)malloc(initial_size * sizeof(KeyVolume));
}

uint128 get_size(Heap h){
	return h->size;
}

void empty(Heap h){
	h->size = 0;
}

int is_empty(Heap h){
	return h->size == 0;
}

int has_parent(Heap h, uint128 index){
	return index>1;
}


int is_leaf(Heap h, uint128 index){
	return (index<<1) > (h->size);
}

int has_right_child(Heap h, uint128 index){
	return ((index<<1) + 1) <= (h->size);
}

void push(Heap h, KeyVolume i){
	if((h->size)+1 == h->capacity){
		h->capacity = (h->capacity)<<1;
		h->heap_array = (KeyVolume *)realloc(h->heap_array ,h->capacity * sizeof(KeyVolume));
	}
	
	uint128 index = h->size+1;
	h->size++;
	KeyVolume * arr = h->heap_array;
	arr[index] = i;
	KeyVolume tmp;
	while(has_parent(h,index) && (get_volume(arr[index]) > get_volume(arr[index >> 1]))){
		tmp = arr[index];
		arr[index] = arr[index >> 1];
		arr[index >> 1] = tmp;
		index = index >> 1;
	}
}

KeyVolume pop(Heap h){
	if(is_empty(h)){
		return NULL;
	}
	
	KeyVolume k = h->heap_array[1];
	h->heap_array[1] = h->heap_array[h->size];
	h->heap_array[h->size] = NULL;
	h->size--;
	
	update(h);
	return k;
	
}

void update(Heap h){
	uint128 index = 1;
	uint128 max;
	KeyVolume * arr = h->heap_array;
	KeyVolume  tmp;
	
	while(has_right_child(h, index)){
		if(get_volume(arr[index << 1]) > get_volume(arr[(index<<1)+1])){
			max = index << 1;
		}
		else{
			max = (index << 1) + 1;
		}
		
		if(get_volume(arr[index]) < get_volume(arr[max])){
			tmp = arr[index];
			arr[index] = arr[max];
			arr[max] = tmp;
			index = max;
		}
		else{
			return;
		}
	}
	
	if(! is_leaf(h, index)){
		if(get_volume(arr[index]) < get_volume(arr[index << 1])){
			tmp = arr[index];
			arr[index] = arr[index << 1];
			arr[index << 1] = tmp;
		}
	}
}
	
void free_heap(Heap * h){
	for(int i=0; i< (*h)->size; i++){
		free((*h)->heap_array[i]);
	}
	free((*h)->heap_array);
	free(*h);
}
	
	
	


