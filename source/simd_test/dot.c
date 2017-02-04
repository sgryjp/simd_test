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

/* VDPPS version, a litte faster one */
float dot_avx_dp2(const float *a, const float *b, size_t len)
{
	size_t i;
	__m256 accum;
	alignas(MALIGN) float buf[8];

	accum = _mm256_setzero_ps();
	for (i = 0; i<len; i += 8*4) {
		__m256 ma0 = _mm256_load_ps(a + i + 0);
		__m256 ma1 = _mm256_load_ps(a + i + 8);
		__m256 ma2 = _mm256_load_ps(a + i +16);
		__m256 ma3 = _mm256_load_ps(a + i +24);
		__m256 mb0 = _mm256_load_ps(b + i + 0);
		__m256 mb1 = _mm256_load_ps(b + i + 8);
		__m256 mb2 = _mm256_load_ps(b + i +16);
		__m256 mb3 = _mm256_load_ps(b + i +24);

		__m256 m0 = _mm256_dp_ps(ma0, mb0, 0xf1);	/* a - - - A - - - */
		__m256 m1 = _mm256_dp_ps(ma1, mb1, 0xf2);	/* - b - - - B - - */
		__m256 m2 = _mm256_dp_ps(ma2, mb2, 0xf4);	/* - - c - - - C - */
		__m256 m3 = _mm256_dp_ps(ma3, mb3, 0xf8);	/* - - - d - - - D */

		m0 = _mm256_blend_ps(m0, m1, 0xaa); /* 0b10101010 --> a b - - A B - - */
		m0 = _mm256_blend_ps(m0, m2, 0x44); /* 0b00100010 --> a b c - A B C - */
		m0 = _mm256_blend_ps(m0, m3, 0x88); /* 0b00010001 --> a b c d A B C D */
		accum = _mm256_add_ps(accum, m0);
	}

	_mm256_store_ps(buf, accum);
	return buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7];
}
#endif
