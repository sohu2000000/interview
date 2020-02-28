#ifndef __MAXSUBARRAY_H__
#define __MAXSUBARRAY_H__

#include <stdbool.h>

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2

int maxSubArray(int* nums, int numsSize);
void testmaxSubArray(void);


#endif
