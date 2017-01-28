#ifndef simd_test__div_h_
#define simd_test__div_h_

#include "simd_test.h"

extern void div_c(const float *a, const float *b, size_t len, float *result);
extern void div_sse(const float *a, const float *b, size_t len, float *result);
#ifdef USE_AVX
extern void div_avx(const float *a, const float *b, size_t len, float *result);
#endif

#endif /*simd_test__div_h_*/
