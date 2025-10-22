#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "stack.h"


int dumpStack(st_stack * stack){
	if(NULL == stack){
		return SUCCESS;
	}

	st_dataNode * iter = NULL;

	printf("========= Dump Stack %p ===========\n", stack);

	printf("\t button  = %d \n", stack->btn->data);
	printf("\t top  = %d \n\t", stack->top->data);

	iter = stack->btn;

	while(NULL != iter){
		printf(" %d ", iter->data);
		iter = iter->next;
	}

	printf("\n");
	printf("===================================\n");

	return SUCCESS;
}

st_stack * createStack(void){
	st_stack * q = NULL;
	q = (st_stack *)malloc(sizeof(st_stack));
	if(NULL == q){
		printf("createStack malloc failed\n");
		return NULL;
	}

	q->top = NULL;
	q->btn = NULL;

	return q;
}


int pushStack(st_stack * stack, int data){
	if(NULL == stack){
		printf("%s param error \n");
		return PARAM_ERR;
	}

	st_dataNode * node = createListNode(data);
	if(NULL == node){
		printf("%s createListNode error \n");
		return ALLOC_ERR;
	}

	if(NULL == stack->btn){ /*队列第一个元素*/
		stack->top = node;
		stack->btn = node;
	} else { /*不是首节点，加到最后, 更新stack的top*/
		stack->top->next = node;
		stack->top = node;
	}

	return SUCCESS;
}


st_dataNode * popStack(st_stack * stack){
	if(NULL == stack){
		printf("%s param error \n");
		return NULL;
	}

	st_dataNode * p = NULL;
	st_dataNode * iter = NULL;

	/*从top出队，需要得到top的prev*/
	iter = stack->btn;

	/*只剩下一个节点的时候，让top和btn都为NULL，然后pop最后一个节点*/
	if(iter == stack->top){ 
		p = iter;
		stack->btn = NULL;
		stack->top = NULL;
		goto out;
	}
	
	while(iter->next != stack->top){
		iter = iter->next;
	}

	p = stack->top;
	if(NULL == p){ /*空对列*/
		return NULL;
	}

	/*出栈*/
	iter->next = NULL;
	stack->top = iter;

out:
	return p;
}

int getStackLength(st_stack * stack){
	if(NULL == stack){
		printf("%s param error \n");
		return PARAM_ERR;
	}
	
	st_dataNode * p = NULL;
	int len = 0;

	len = getListLen(stack->btn);

	return len;
}

bool isStackEmpty(st_stack * stack){
	bool flag = false;

	if(NULL == stack->btn && NULL == stack->top){
		flag = true;
	}

	return flag;
}


void testStack(void){
	st_dataNode * p = NULL;
	int len = 0;
	bool rst = 0;
	
	gstack = createStack();
	rst = isStackEmpty(gstack);
	if(rst){
		printf("Stack is empty\n");
	} else {
		printf("Stack is not empty\n");
	}

	pushStack(gstack, 22);
	pushStack(gstack, 32);
	pushStack(gstack, 19);
	pushStack(gstack, 53);
	pushStack(gstack, 0);
	pushStack(gstack, 47);
	pushStack(gstack, 29);
	len = getStackLength(gstack);
	printf("Stack len = %d\n", len);	
	dumpStack(gstack);

	p = popStack(gstack);
	if(NULL != p)
		printf("popStack p = %d\n", p->data);
	free(p);
	p = popStack(gstack);
	if(NULL != p)
		printf("popStack p = %d\n", p->data);
	free(p);
	p = popStack(gstack);
	if(NULL != p)
		printf("popStack p = %d\n", p->data);
	free(p);
	len = getStackLength(gstack);
	printf("Stack len = %d\n", len);	
	dumpStack(gstack);	

	rst = isStackEmpty(gstack);
	if(rst){
		printf("Stack is empty\n");
	} else {
		printf("Stack is not empty\n");
	}

	p = popStack(gstack);
	free(p);
	p = popStack(gstack);
	free(p);
	p = popStack(gstack);
	free(p);
	printf("1\n");
	p = popStack(gstack);
	printf("2\n");
	free(p);	
	printf("3\n");
	
	rst = isStackEmpty(gstack);
	if(rst){
		printf("Stack is empty\n");
	} else {
		printf("Stack is not empty\n");
	}	

	return;
}









