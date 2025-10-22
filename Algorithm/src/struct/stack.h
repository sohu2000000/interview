#ifndef __STACK_H__
#define __STACK_H__

#include "list.h"

typedef struct stack{
	st_dataNode * top; /* tail */
	st_dataNode * btn; /* head */
} st_stack;

st_stack * gstack;

int dumpStack(st_stack * stack);
st_stack * createStack(void);
int pushStack(st_stack * stack, int data);
st_dataNode * popStack(st_stack * stack);
int getStackLength(st_stack * stack);

void testStack(void);

#endif


