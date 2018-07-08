
void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N, int num_threads) {

    // perform loop fusion to transform this loop and parallelize it with OpenMP
		//long a1;
		//long a2;
		//long c1;
    for (int j = 1 ; j<N ; j++)
		{
			#pragma omp parallel for num_threads(num_threads) firstprivate(c,b)
      for (int i = 1 ; i<N ; i++)
      {
					//a1 = 2 * b[i][j-1]; //j-1
					//a2 = 2 * b[i][j+1]; //j+1
          a[i][j] = 2 * b[i][j];
          d[i][j] = a[i][j] * c[i][j];
      }
			#pragma omp barrier
			#pragma omp parallel for num_threads(num_threads) shared(c)
			for (int i = 1 ; i<N ; i++)
			{
				c[i][j-1] = 2 * b[i][j-1] - 2 * b[i][j+1]; //j-1
			}

    }
}
/*
		#pragma omp parallel for num_threads(num_threads) firstprivate(a) shared(c)
		for (int i = 1 ; i<N ; i++)
		{
        for (int j = 1 ; j<N ; j++)
        {
					c[i][j-1] = a[i][j - 1] - a[i][j + 1];

				}
		}
}
*/
