#include "simd_test.h"
#include "add.h"
#include "mul.h"
#include "div.h"
#include "dot.h"
#include "mul_add.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>

#define ALLOWED_ERROR	0.0001
#define ASSERT(cond)	{ \
	if(!cond) { \
		fprintf(stderr, "%s(%d): Assertion failed.\n", __FILE__, __LINE__); \
		exit(1); \
	} \
}
#define ASSERT_EQUALS_FLOAT(expected, actual)	{ \
	if (ALLOWED_ERROR < fabs((expected) - (actual))) {\
		fprintf(stderr, "%s(%d): Expected %f but actually %f.\n", __FILE__, __LINE__, (expected), (actual)); \
		exit(1); \
	} \
}
#define ASSERT_EQUALS_FLOATS(expected, actual, len)	{ \
	unsigned int i_; \
	for (i_ = 0; i_ < (unsigned int)len; ++i_) { \
		if (ALLOWED_ERROR < fabs((expected[i_]) - (actual[i_]))) {\
			fprintf(stderr, "%s(%d): Expected %f but actually %f (%dth element).\n", \
					__FILE__, __LINE__, (double)(expected[i_]), (double)(actual[i_]), i_); \
			exit(1); \
		} \
	} \
}

void init_vectors(float *a, float *b, float *c, size_t len)
{
	size_t i;
	for (i = 0; i<len; ++i) {
		a[i] = (rand() / (float)RAND_MAX) - .5f;
		b[i] = (rand() / (float)RAND_MAX) - .5f;
		c[i] = (rand() / (float)RAND_MAX) - .5f;
	}
}

int main(int argc, char* argv[])
{
	int		rc;
	int		i, fi;	/*function index*/
	float	*a, *b, *c, *c_result, *simd_result;
	clock_t	s, duration;
	size_t	counts[32] = {0};
	size_t	len;
	const char* names[] = {
		"add_c", "add_sse"
#ifdef USE_AVX
		, "add_avx"
#endif
		, "mul_c", "mul_sse"
#ifdef USE_AVX
		, "mul_avx"
#endif
		, "div_c", "div_sse"
#ifdef USE_AVX
		, "div_avx"
#endif
		, "mul_add_c", "mul_add_sse"
#ifdef USE_AVX
		, "mul_add_avx"
#endif
		, "dot_c"
		, "dot_sse", "dot_sse41_dp"
#ifdef _WIN64
		, "dot_sse_asm"
#endif
#ifdef USE_AVX
		, "dot_avx", "dot_avx_dp"
#ifdef _WIN64
		, "dot_avx_asm"
#endif
#endif
	};

	len = 4096;
	duration = (clock_t)(0.5 * (double)CLOCKS_PER_SEC);
	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-l") == 0 && i+1 < argc) {
			len = (size_t)atoi(argv[i+1]);
			++i;
		}
		else if (strcmp(argv[i], "-d") == 0 && i+1 < argc) {
			duration = (clock_t)(atof(argv[i+1]) * (double)CLOCKS_PER_SEC);
			++i;
		}
	}

	a = (float*)aligned_alloc(MALIGN, sizeof(float)*len);
	b = (float*)aligned_alloc(MALIGN, sizeof(float)*len);
	c = (float*)aligned_alloc(MALIGN, sizeof(float)*len);
	c_result = (float*)aligned_alloc(MALIGN, sizeof(float)*len);
	simd_result = (float*)aligned_alloc(MALIGN, sizeof(float)*len);
	if (a == NULL || b == NULL || c_result == NULL || simd_result == NULL) {
		rc = ENOMEM;
		goto cleanup;
	}

	init_vectors(a, b, c, len);
	{
		fi = 0;

		/* ----------------------------------------------------------------- */
		printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			add_c(a, b, len, c_result);
		}

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			add_sse(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);

#ifdef USE_AVX
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			add_avx(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);
#endif

		/* ----------------------------------------------------------------- */
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_c(a, b, len, c_result);
		}

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_sse(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);

#ifdef USE_AVX
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_avx(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);
#endif

		/* ----------------------------------------------------------------- */
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			div_c(a, b, len, c_result);
		}

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			div_sse(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);

#ifdef USE_AVX
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			div_avx(a, b, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);
#endif

		/* ----------------------------------------------------------------- */
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_add_c(a, b, c, len, c_result);
		}

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_add_sse(a, b, c, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);

#ifdef USE_AVX
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			mul_add_avx(a, b, c, len, simd_result);
		}
		ASSERT_EQUALS_FLOATS(c_result, simd_result, len);
#endif

		/* ----------------------------------------------------------------- */
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			c_result[0] = dot_c(a, b, len);
		}

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_sse(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);

		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_sse41_dp(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);

#ifdef _WIN64
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_sse_asm(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);
#endif

#ifdef USE_AVX
		fi++; printf("Executing %s...\n", names[fi]); s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_avx(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);

		fi++; printf("Executing %s...\n", names[fi]);
		s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_avx_dp(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);

#ifdef _WIN64
		fi++; printf("Executing %s...\n", names[fi]);
		s = clock();
		for (; clock()-s < duration; ++counts[fi]) {
			simd_result[0] = dot_avx_asm(a, b, len);
		}
		ASSERT_EQUALS_FLOAT(c_result[0], simd_result[0]);
#endif
#endif
	}

	printf("\n");
	printf("Number of times each function ran in %g secs:\n", (duration / (double)CLOCKS_PER_SEC));
	for (i = 0; i<sizeof(names) / sizeof(const char*); ++i) {
		printf("%-20s:%8llu times\n", names[i], (unsigned long long)(counts[i]));
	}

	rc = 0;

cleanup:
	if (simd_result != NULL) aligned_free(simd_result);
	if (c_result != NULL) aligned_free(c_result);
	if (c != NULL)       aligned_free(c);
	if (b != NULL)       aligned_free(b);
	if (a != NULL)       aligned_free(a);

	return (rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}