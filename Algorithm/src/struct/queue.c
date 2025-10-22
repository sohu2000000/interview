#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "queue.h"


int dumpQueue(st_queue * queue){
	if(NULL == queue){
		return SUCCESS;
	}

	st_dataNode * iter = NULL;

	printf("========= Dump Queue %p ===========\n", queue);

	printf("\t head  = %p \n", queue->head);
	printf("\t tail  = %p \n\t", queue->tail);

	iter = queue->head;

	while(NULL != iter){
		printf(" %d ", iter->data);
		iter = iter->next;
	}

	printf("\n");
	printf("===================================\n");

	return SUCCESS;
}

st_queue * createQueue(void){
	st_queue * q = NULL;
	q = (st_queue *)malloc(sizeof(st_queue));
	if(NULL == q){
		printf("createQueue malloc failed\n");
		return NULL;
	}

	q->head = NULL;
	q->tail = NULL;

	return q;
}


int inQueue(st_queue * queue, int data){
	if(NULL == queue){
		printf("%s param error \n");
		return PARAM_ERR;
	}

	st_dataNode * node = createListNode(data);
	if(NULL == node){
		printf("%s createListNode error \n");
		return ALLOC_ERR;
	}

	if(NULL == queue->head){ /*队列第一个元素*/
		queue->head = node;
		queue->tail = node;
	} else { /*不是首节点，加到最后, 更新queue的tail*/
		queue->tail->next = node;
		queue->tail = node;
	}

	return SUCCESS;
}


st_dataNode * deQueue(st_queue * queue){
	if(NULL == queue){
		printf("%s param error \n");
		return NULL;
	}

	st_dataNode * p = NULL;

	/*队列头退出*/
	p = queue->head;

	/*只有一个节点*/
	if(queue->tail == queue->head){
		queue->tail = NULL;
		queue->head = NULL;
		goto out;
	}
	
	if(NULL == p){ /*空对列*/
		return NULL;
	}

	/*出队*/
	queue->head = p->next;

out:
	return p;
}

int getQueueLength(st_queue * queue){
	if(NULL == queue){
		printf("%s param error \n");
		return PARAM_ERR;
	}
	
	st_dataNode * p = NULL;
	int len = 0;

	len = getListLen(queue->head);

	return len;
}

bool isQueueEmpty(st_queue * q){
	bool rst = false;

	if(NULL == q->head && NULL == q->tail){
		rst = true;
	}

	return rst;
}

void testQueue(void){
	st_dataNode * p = NULL;
	int len = 0;
	bool rst = false;
	
	gqueue = createQueue();
	rst = isQueueEmpty(gqueue);
	if(rst){
		printf("Queue is NULL\n");
	} else {
		printf("Queue not NULL\n");
	}
	
	inQueue(gqueue, 22);
	inQueue(gqueue, 32);
	inQueue(gqueue, 19);
	inQueue(gqueue, 53);
	inQueue(gqueue, 0);
	inQueue(gqueue, 47);
	inQueue(gqueue, 29);
	len = getQueueLength(gqueue);
	printf("Queue len = %d\n", len);	
	dumpQueue(gqueue);

	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	len = getQueueLength(gqueue);
	printf("Queue len = %d\n", len);	
	dumpQueue(gqueue);	
	rst = isQueueEmpty(gqueue);
	if(rst){
		printf("Queue is NULL\n");
	} else {
		printf("Queue not NULL\n");
	}	

	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	p = deQueue(gqueue);
	if(NULL != p)
		printf("Dequeue p = %d\n", p->data);
	free(p);
	rst = isQueueEmpty(gqueue);
	if(rst){
		printf("Queue is NULL\n");
	} else {
		printf("Queue not NULL\n");
	}

	return;
}



