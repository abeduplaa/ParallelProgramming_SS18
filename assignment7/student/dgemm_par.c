#include"dgemm.h"
#include <immintrin.h>
#include <smmintrin.h>
#include <string.h>


void dotmul_intrins(float *A, float *B, float C, int SIZE)
{

  register __m128 *a, *b;
  a = (__m128*) &A;
  b = (__m128*) &B;

  ///*

  __m128 r1 = _mm_mul_ps(*a, *b);
  __m128 r2 = _mm_hadd_ps(r1, r1);
  __m128 r3 = _mm_hadd_ps(r2, r2);
  _mm_store_ss(&C, r3);

  //*/

  //_mm_store_ss(C, _mm_hadd_ps(_mm_hadd_ps(_mm_mul_ps(*a, *b), _mm_mul_ps(*a, *b)), _mm_hadd_ps(_mm_mul_ps(*a, *b), _mm_mul_ps(*a, *b))));

}



void dgemm(float *a, float *b, float *c, int n)
{
  float *a1, *b1;
  a1 = (float*)malloc(n);
  b1 = (float*)malloc(n);
  for (int i = 0; i < n; ++i )
  {
    //float *a1 = &a[i * n];
    memcpy(a1, &a[i*n], n*sizeof(*a));
    for (int j = 0 ; j<n; ++j )
    {
      //b1 = &b[j*n];
      memcpy(b1, &b[i*n], n*sizeof(*b));

      dotmul_intrins(a1,b1,c[i * n + j],n);

    }
  }
}
