#include "dot.h"

float dot_c(const float *a, const float *b, size_t len)
{
	size_t i;
	float accum = 0.f;
	for (i = 0; i<len; ++i) {
		accum += a[i] * b[i];
	}
	return accum;
}

float dot_sse(const float *a, const float *b, size_t len)
{
	size_t i;
	__m128 *A, *B, tmp, accum;

	accum = _mm_setzero_ps();
	for (i = 0; i<len; i += 4) {
		A = (__m128*)(a + i);
		B = (__m128*)(b + i);

		tmp = _mm_mul_ps(*A, *B);
		accum = _mm_add_ps(accum, tmp);
	}
	return accum.m128_f32[0] + accum.m128_f32[1] + accum.m128_f32[2] + accum.m128_f32[3];
}

/* DPPS instruction version (SSE 4.1) */
float dot_sse41_dp(const float *a, const float *b, size_t len)
{
	size_t i;
	__m128 *A, *B, tmp;
	float accum = {0};

	for (i = 0; i<len; i += 4) {
		A = (__m128*)(a + i);
		B = (__m128*)(b + i);

		tmp = _mm_dp_ps(*A, *B, 0xf1);
		accum += tmp.m128_f32[0];
	}
	return accum;
}

#ifdef USE_AVX
float dot_avx(const float *a, const float *b, size_t len)
{
	alignas(MALIGN) float accum[8] = {0};

	for (; 0<len; len -= 8) {
		__m256 ymm = _mm256_load_ps(a);
		ymm = _mm256_mul_ps(ymm, *(__m256*)b);
		*(__m256*)accum = _mm256_add_ps(ymm, *(__m256*)accum);
		a += 8; b += 8;
	}
	return accum[0] + accum[1] + accum[2] + accum[3] + accum[4] + accum[5] + accum[6] + accum[7];
}

/* VDPPS instruction version */
float dot_avx_dp(const float *a, const float *b, size_t len)
{
	size_t	i;
	float accum = 0.f;
	__m256 dp;

	for (i = 0; i<len; i += 8) {
		dp = _mm256_dp_ps(*(__m256*)(a + i), *(__m256*)(b + i), 0xf1);
		accum += dp.m256_f32[0] + dp.m256_f32[4];
	}
	return accum;
}
#endif
