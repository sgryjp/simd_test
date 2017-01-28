#include "mul_add.h"

void mul_add_c(const float *a, const float *b, const float *c, size_t len, float *result)
{
	size_t i;

	for (i = 0; i<len; ++i) {
		result[i] = a[i] * b[i] + c[i];
	}
}

void mul_add_sse(const float *a, const float *b, const float *c, size_t len, float *result)
{
	size_t i;

	for (i = 0; i < len; i += 4) {
		__m128* A = (__m128*)(a+i);
		__m128* B = (__m128*)(b+i);
		__m128* C = (__m128*)(c+i);
		__m128* R = (__m128*)(result+i);
		*R = _mm_mul_ps(*A, *B);
		*R = _mm_add_ps(*R, *C);
	}
}

#ifdef USE_AVX
void mul_add_avx(const float *a, const float *b, const float *c, size_t len, float *result)
{
	size_t i;

	for (i = 0; i < len; i += 8) {
		__m256* A = (__m256*)(a+i);
		__m256* B = (__m256*)(b+i);
		__m256* C = (__m256*)(c+i);
		__m256* R = (__m256*)(result+i);
		*R = _mm256_mul_ps(*A, *B);
		*R = _mm256_add_ps(*R, *C);
	}
}
#endif
