#include"dgemm.h"
#include <immintrin.h>


void dgemm(float *a, float *b, float *c, int n)
{
  register __m128 *a1, *b1;
  register __m128 r1,r2,r2a,r3,r4; //,r4;
  register int k;
  for (int i = 0; i < n; ++i )
  {
    for (int j = 0 ; j<n; ++j )
    {
      r4 = _mm_setzero_ps();
      for (k = 0; k < n; k += 4)
      {

        r1 = _mm_setzero_ps();
        r2 = _mm_setzero_ps();
        r3 = _mm_setzero_ps();


        a1 = (__m128*) &a[i*n + k];
        b1 = (__m128*) &b[j*n + k];

        r1 = _mm_mul_ps(*a1, *b1);
        r2 = _mm_hadd_ps(r1, r1);
        r3 = _mm_hadd_ps(r2, r2);

        r4 = _mm_add_ps(r4,r3);

      }
      _mm_store_ss(&c[i * n + j], r4);
    }
  }
}
