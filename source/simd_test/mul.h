#ifndef simd_test__mul_h_
#define simd_test__mul_h_

#include "simd_test.h"

void mul_c(const float *a, const float *b, size_t len, float *result);
void mul_sse(const float *a, const float *b, size_t len, float *result);
#ifdef USE_AVX
void mul_avx(const float *a, const float *b, size_t len, float *result);
#endif


#endif /*simd_test__mul_h_*/
