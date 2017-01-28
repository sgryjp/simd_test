#ifndef simd_test__mul_add_h_
#define simd_test__mul_add_h_

#include "simd_test.h"

void mul_add_c(const float *a, const float *b, const float *c, size_t len, float *result);
void mul_add_sse(const float *a, const float *b, const float *c, size_t len, float *result);
#ifdef USE_AVX
void mul_add_avx(const float *a, const float *b, const float *c, size_t len, float *result);
#endif


#endif /*simd_test__mul_add_h_*/
