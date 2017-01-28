#ifndef simd_test__win32_clock_override_h_
#define simd_test__win32_clock_override_h_

#include <time.h>

#ifdef _WIN32
#include <windows.h>

#define clock()			win32_clock_override_clock()

#define clock_t			__int64
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC	win32_clock_override_clocks_per_sec()

static __inline clock_t win32_clock_override_clock() {
	__int64 count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	return count;
}

static __inline clock_t win32_clock_override_clocks_per_sec() {
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return freq;
}
#endif

#endif /*simd_test__win32_clock_override_h_*/
