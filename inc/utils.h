#ifndef _UTILS_H_
# define _UTILS_H_

#define SIZE_OF(a) (sizeof(a) / sizeof((a)[0]))

#define UNUSED(x) ((void)x)

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
    	__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
    	__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

#endif
