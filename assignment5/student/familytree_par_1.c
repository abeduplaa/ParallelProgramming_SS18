#include "familytree.h"

#define int count 0


void traverse(tree *node, int numThreads){
	#pragma omp parallel shared(node, count)
	{
		#pragma omp single
		{
			if(node != NULL)
			{
				node->IQ = compute_IQ(node->data);
				genius[node->id] = node->IQ;

				#pragma omp task firstprivate(node) final(count>3)
				traverse(node->right, numThreads);

				#pragma omp task firstprivate(node) final(count>3) //TODOfind a way to put something at final
				traverse(node->left, numThreads);
			}
		}
	}

}
