
#ifndef AVL_BINARYSEARCHTREE_H_
#define AVL_BINARYSEARCHTREE_H_

#include "Libraries.h"

// An AVL tree node 
struct Node_t 
{ 
	double key; 
	struct Node_t *left; 
	struct Node_t *right; 
	int height; 
	double times;
}; 

typedef struct Node_t * Node;

/* Helper function that allocates a new node with the given key and 
	NULL left and right pointers. */
Node newNode(double key);

double getMax(Node n);

// A utility function to get the height of the tree 
int height(Node N);

// A utility function to get maximum of two integers 
int max(int a, int b);

// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
Node insert(Node node, double key, int * success);

void preOrder(Node root, double * table);

void print_tree(Node root);

void free_AVL(Node * avl);

#endif
