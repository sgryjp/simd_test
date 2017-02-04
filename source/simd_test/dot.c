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
	__m128 accum;
	alignas(MALIGN) float buf[4];

	accum = _mm_setzero_ps();
	for (i = 0; i<len; i += 4) {
		__m128 va = _mm_load_ps(a + i);
		__m128 vb = _mm_load_ps(b + i);

		__m128 tmp = _mm_mul_ps(va, vb);
		accum = _mm_add_ps(accum, tmp);
	}

	_mm_store_ps(buf, accum);
	return buf[0] + buf[1] + buf[2] + buf[3];
}

/* DPPS instruction version (SSE 4.1) */
float dot_sse41_dp(const float *a, const float *b, size_t len)
{
	size_t i;
	__m128 accum;
	alignas(MALIGN) float buf[4];

	accum = _mm_setzero_ps();
	for (i = 0; i<len; i += 4) {
		__m128 va = _mm_load_ps(a + i);
		__m128 vb = _mm_load_ps(b + i);

		__m128 tmp = _mm_dp_ps(va, vb, 0xf1);
		accum = _mm_add_ps(accum, tmp);
	}

	_mm_store_ps(buf, accum);
	return buf[0];
}

#ifdef USE_AVX
/* VMULPS + VADDPS version */
float dot_avx(const float *a, const float *b, size_t len)
{
	size_t i;
	__m256 accum;
	alignas(MALIGN) float buf[8];

	accum = _mm256_setzero_ps();
	for (i = 0; i < len; i += 8) {
		__m256 va = _mm256_load_ps(a + i);
		__m256 vb = _mm256_load_ps(b + i);
		__m256 tmp = _mm256_mul_ps(va, vb);
		accum = _mm256_add_ps(accum, tmp);
	}

	_mm256_store_ps(buf, accum);
	return buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7];
}

/* VDPPS version */
float dot_avx_dp(const float *a, const float *b, size_t len)
{
	size_t i;
	__m256 accum;
	alignas(MALIGN) float buf[8];

	accum = _mm256_setzero_ps();
	for (i = 0; i<len; i += 8) {
		__m256 va = _mm256_load_ps(a + i);
		__m256 vb = _mm256_load_ps(b + i);
		__m256 tmp = _mm256_dp_ps(va, vb, 0xf1);
		accum = _mm256_add_ps(tmp, accum);
	}

	_mm256_store_ps(buf, accum);
	return buf[0] + buf[4];
}
#endif
