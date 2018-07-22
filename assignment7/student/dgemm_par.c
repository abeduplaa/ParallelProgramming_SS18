#include"dgemm.h"
#include <immintrin.h>

void dotmul_intrins128(float *A, float *B, float *C, int SIZE)
{
  register __m128 *a, *b;

  register __m128 r1,r2,r2a,r3,r4; //,r4;
  //register __m128 ;
  register int k;
  r4 = _mm_setzero_ps();
    for (k = 0; k < SIZE; k += 4)
    {

      r1 = _mm_setzero_ps();
      r2 = _mm_setzero_ps();
      r3 = _mm_setzero_ps();


      a = (__m128*) &A[k];
      b = (__m128*) &B[k];

      r1 = _mm_mul_ps(*a, *b);
      r2 = _mm_hadd_ps(r1, r1);
      r3 = _mm_hadd_ps(r2, r2);

      r4 = _mm_add_ps(r4,r3);

    }
    _mm_store_ss(C, r4);
}

void dotmul_intrins(float *A, float *B, float *C, int SIZE)
{
  register __m256 a, b;

  register __m256 r1,r2, r2a, r3,r4, r6;
  //__m256 r6;

  //SIZE += 8;
  register int k;
  r6 = _mm256_setzero_ps();

    for (k = 0; k < SIZE; k += 8)
    {
      //r1 = _mm256_setzero_ps();
      //r2 = _mm256_setzero_ps();
      //r2a = _mm256_setzero_ps();
      r3 = _mm256_setzero_ps();
      r4 = _mm256_setzero_ps();

      a = _mm256_loadu_ps(&A[k]);
      b = _mm256_loadu_ps(&B[k]);

      r1 = _mm256_mul_ps(a,b); //multiply them together

      r2 = _mm256_hadd_ps(r1, _mm256_permute2f128_ps(r1,r1,1)); //add to get (12) and (34) added together

      r3 = _mm256_hadd_ps(r2, r2 ); //add to get (1234) and (5678)
      //r4 = _mm256_hadd_ps(r3, r3); //add to get (12345678)
      r4 = _mm256_hadd_ps(r3, r3); //add to get (12345678)

      //__m256 rd = _mm256_set_p s(2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0);
      //r4 = _mm256_div_ps(r4,rd);
      *C += r4[0];
    }
    //_mm256_storeu_ps(C, r6);

}



void dgemm(float *a, float *b, float *c, int n)
{
  for (int i = 0; i < n; ++i )
  {
    for (int j = 0 ; j<n; ++j )
    {
      //dotmul_intrins128(&a[i*n],&b[j*n],&c[i * n + j], n);
      dotmul_intrins(&a[i*n],&b[j*n],&c[i * n + j], n);
    }
  }
}
