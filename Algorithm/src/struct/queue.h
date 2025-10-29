#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "list.h"

typedef struct queue{
	ListNode * head;  // 队头指针
	ListNode * tail;  // 队尾指针
} st_queue;

/* 全局队列指针（用于测试） */
st_queue * gqueue;

/* ===== 队列操作函数（已优化） ===== */

/* 队列基本操作 */
st_queue *create_new_queue(void);
int enqueue_element(st_queue *queue_ptr, int element_data);
ListNode *dequeue_element(st_queue *queue_ptr);
int get_queue_length(st_queue *queue_ptr);
bool is_queue_empty(st_queue *queue_ptr);
int display_queue_contents(st_queue *queue_ptr);

/* 兼容性宏：保持旧接口可用 */
#define createQueue    create_new_queue
#define inQueue        enqueue_element
#define deQueue        dequeue_element
#define getQueueLength get_queue_length
#define isQueueEmpty   is_queue_empty
#define dumpQueue      display_queue_contents

/* 测试函数 */
void testQueue(void);




#endif


