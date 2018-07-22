#include"dgemm.h"
#include <immintrin.h>


void dgemm(float *a, float *b, float *c, int n)
{
  register __m256 a1, b1;
  register __m256 r1,r2, r2a, r3,r4, r6;
  register int k;
  for (int i = 0; i < n; ++i )
  {
    for (int j = 0 ; j<n; ++j )
    {
      r6 = _mm256_setzero_ps();

      for (k = 0; k < n; k += 8)
      {
        r1 = _mm256_setzero_ps();
        r2 = _mm256_setzero_ps();
        r2a = _mm256_setzero_ps();
        r3 = _mm256_setzero_ps();
        r4 = _mm256_setzero_ps();
        a1 = _mm256_loadu_ps(&a[k]);
        b1 = _mm256_loadu_ps(&b[k]);

        r1 = _mm256_mul_ps(a1,b1); //multiply them together

        r2 = _mm256_hadd_ps(r1, r1); //add to get (12) and (34) added together

        r2a = _mm256_permute2f128_ps(r2,r2,1); //shuffle to get 5,6,7,8 in 1,2,3,4 position

        r3 = _mm256_hadd_ps(r2, r2a ); //add to get (1234) and (5678)
        //r4 = _mm256_hadd_ps(r3, r3); //add to get (12345678)
        r4 = _mm256_hadd_ps(r3, r3); //add to get (12345678)

        //__m256 rd = _mm256_set_ps(2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0);
        //r4 = _mm256_div_ps(r4,rd);
        r6 = _mm256_add_ps(r6,r4);
      }
      _mm256_storeu_ps(&c[i * n + j], r6);
    }
  }
}
