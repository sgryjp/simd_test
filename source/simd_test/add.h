#ifndef simd_test__add_h_
#define simd_test__add_h_

#include "simd_test.h"

#ifdef __cplusplus
extern "C" {
#endif

void add_c(const float *a, const float *b, size_t len, float *result);
void add_sse(const float *a, const float *b, size_t len, float *result);
#ifdef USE_AVX
void add_avx(const float *a, const float *b, size_t len, float *result);
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*simd_test__add_h_*/
