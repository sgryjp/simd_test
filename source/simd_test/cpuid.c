#include "simd_test.h"
#include <intrin.h>

#define SSE_BIT		(1 << 25)	/*F1_EDX*/
#define SSE41_BIT	(1 << 20)	/*F1_ECX*/
#define AVX_BIT		(1 << 28)	/*F1_ECX*/

#define F1_EAX	g_f1_info[0]
#define F1_EBX	g_f1_info[1]
#define F1_ECX	g_f1_info[2]
#define F1_EDX	g_f1_info[3]

static int g_max_func_id = -1;
static int g_f1_info[4];

static inline void get_cpuinfo()
{
	if (g_max_func_id == -1) {
		int f0_info[4];
		__cpuid(f0_info, 0);
		g_max_func_id = f0_info[0];
		if (0 < g_max_func_id) {
			__cpuid(g_f1_info, 1);
		}
	}
}

bool cpu_has_sse()
{
	get_cpuinfo();
	return (F1_EDX & SSE_BIT) != 0;
}

bool cpu_has_sse41()
{
	get_cpuinfo();
	return (F1_ECX & SSE41_BIT) != 0;
}

bool cpu_has_avx()
{
	get_cpuinfo();
	return (F1_ECX & AVX_BIT) != 0;
}
