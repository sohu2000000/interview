#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "list.h"

typedef struct queue{
	st_dataNode * head;
	st_dataNode * tail;	
} st_queue;

st_queue * gqueue;

int dumpQueue(st_queue * queue);
st_queue * createQueue(void);
int inQueue(st_queue * queue,int data);
bool isQueueEmpty(st_queue * q);
st_dataNode * deQueue(st_queue * queue);


void testQueue(void);




#endif


