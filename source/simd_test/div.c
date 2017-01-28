#include "div.h"

void div_c(const float *a, const float *b, size_t len, float *result)
{
	size_t i;

	for (i = 0; i<len; ++i) {
		result[i] = a[i] / b[i];
	}
}

void div_sse(const float *a, const float *b, size_t len, float *result)
{
	size_t i;

	for (i = 0; i < len; i += 4) {
		__m128* A = (__m128*)(a+i);
		__m128* B = (__m128*)(b+i);
		__m128* R = (__m128*)(result+i);
		*R = _mm_div_ps(*A, *B);
	}
}

#ifdef USE_AVX
void div_avx(const float *a, const float *b, size_t len, float *result)
{
	size_t i;

	for (i = 0; i < len; i += 8) {
		__m256* A = (__m256*)(a+i);
		__m256* B = (__m256*)(b+i);
		__m256* R = (__m256*)(result+i);
		*R = _mm256_div_ps(*A, *B);
	}
}
#endif
