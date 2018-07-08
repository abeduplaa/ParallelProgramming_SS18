#include "familytree.h"

void traverse_1(tree *node){
	if(node != NULL)
	{
		node->IQ = compute_IQ(node->data);
		genius[node->id] = node->IQ;

		#pragma omp task firstprivate(node)
		traverse_1(node->right);

		#pragma omp task firstprivate(node)
		traverse_1(node->left);

	}
}


void traverse(tree *node, int numThreads){

	#pragma omp parallel shared(node) num_threads(numThreads)
	{
		#pragma omp single
		traverse_1(node);
	}

}
