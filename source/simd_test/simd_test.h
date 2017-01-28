#pragma once

#include <math.h>
#include <intrin.h>
#include <time.h>
#ifdef _MSC_VER
# include "win32_clock_override.h"
#endif

/* boolean type */
#if !defined(__cplusplus)
# if _MSC_VER < 1800
typedef int	bool;
#define true	1
#define false	0
# else
# include <stdbool.h>
# endif
#endif

/* inline function */
#if !defined(__cplusplus)
# ifdef _MSC_VER
#  define inline __inline
# endif
#endif

#if (1600 <= _MSC_VER)
# define USE_AVX
#endif

#ifdef _MSC_VER
#include <malloc.h>
# define aligned_alloc(align, size)		_aligned_malloc(size, align)
# define aligned_free(ptr)				_aligned_free(ptr)
# define alignas(alignment)				__declspec(align(alignment))
# define alignof(type)					__alignof(type)
#elif __STDC_VERSION__ >= 199901L
# include <stdalign.h>
# define aligned_free(ptr)				free(ptr)
# define alignof(type)					__alignof__(type)
#endif
#define MALIGN							32
