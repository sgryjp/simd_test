#ifndef simd_test__dot_h_
#define simd_test__dot_h_

#include "simd_test.h"

float dot_c(const float *a, const float *b, size_t len);
float dot_sse(const float *a, const float *b, size_t len);
float dot_sse41_dp(const float *a, const float *b, size_t len);
#ifdef _WIN64
extern float dot_sse_asm(const float *a, const float *b, size_t len);
#endif
#ifdef USE_AVX
extern float dot_avx(const float *a, const float *b, size_t len);
extern float dot_avx_dp(const float *a, const float *b, size_t len);
extern float dot_avx_dp2(const float *a, const float *b, size_t len);
# ifdef _WIN64
extern float dot_avx_asm(const float *a, const float *b, size_t len);
# endif
#endif

#endif /*simd_test__dot_h_*/
