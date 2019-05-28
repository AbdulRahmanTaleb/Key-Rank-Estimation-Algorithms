#include "AVL_BinarySearchTree.h"

/* Helper function that allocates a new node with the given key and 
	NULL left and right pointers. */

Node newNode(double key) 
{ 
	Node node = (Node)malloc(sizeof(struct Node_t)); 
	
	node->key =  key;
	node->left = NULL; 
	node->right = NULL; 
	node->height =  1;
	node->times = 1;
	
	return(node); 
} 

double getMax(Node n){
	while(n->right){
		n = n->right;
	}
	return n->key;
}

// A utility function to get the height of the tree 
int height(Node N) 
{ 
	if (N == NULL){ 
		return 0;
	}
	else{
		return N->height;
	} 
} 

// A utility function to get maximum of two integers 
int max(int a, int b)
{ 
	return (a > b) ? a : b;
} 

// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
Node insert(Node node, double key, int * success) 
{ 
	*success = 0;
	/* 1. Perform the normal BST insertion */
	if (node == NULL){ 
		*success = 1;
		return (newNode(key)); 
	}
	if (key < node->key) 
		node->left = insert(node->left, key, success); 
	else if (key > node->key) 
		node->right = insert(node->right, key, success); 
	else{ // Equal keys are not allowed in BST 
		*success = 0;
		node->times = node->times +1;
		return node; 
	}

	/* 2. Update height of this ancestor node */
	node->height = max(height(node->left), height(node->right)) + 1;
	
	/* return the (unchanged) node pointer */
	return node; 
}

// A utility function to print preorder traversal 
// of the tree. 
// The function also prints height of every node 
void preOrder(Node root, double * table)
{ 
	if(root != NULL) 
	{ 
		preOrder(root->left, table);
		table[(uint128)root->key] = root->times;
		preOrder(root->right, table); 
	} 
} 

void print_tree(Node root){
	if(root != NULL) 
	{ 
		int s = 0;
		printf("%lf\n", root->key);
		print_tree(root->left); 
		print_tree(root->right);
		 
	} 
}

void free_AVL(Node * avl){
	if(*avl){
		free_AVL(&(*avl)->left);
		free_AVL(&(*avl)->right);
		free(*avl);
		*avl = NULL;
	}
}
