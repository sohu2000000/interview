#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "queue.h"

/**
 * 显示队列内容 - 打印队列中所有元素（从队头到队尾）
 * @param queue_ptr: 队列指针
 * @return: 成功返回SUCCESS
 * 
 * 功能：遍历并打印队列的所有元素
 */
int display_queue_contents(st_queue *queue_ptr) {
	if (queue_ptr == NULL) {
		return SUCCESS;
	}

	ListNode *current_node = NULL;

	printf("========= 队列内容 %p ===========\n", queue_ptr);
	printf("\t 队头(head) = %p \n", queue_ptr->head);
	printf("\t 队尾(tail) = %p \n\t", queue_ptr->tail);

	current_node = queue_ptr->head;

	while (current_node != NULL) {
		printf(" %d ", current_node->data);
		current_node = current_node->next;
	}

	printf("\n");
	printf("===================================\n");

	return SUCCESS;
}

/**
 * 创建队列 - 分配并初始化一个新的队列
 * @return: 成功返回队列指针，失败返回NULL
 * 
 * 功能：创建空队列，头尾指针都为NULL
 */
st_queue *create_new_queue(void) {
	st_queue *new_queue = NULL;
	
	new_queue = (st_queue *)malloc(sizeof(st_queue));
	if (new_queue == NULL) {
		printf("Error: create_new_queue malloc failed\n");
		return NULL;
	}

	new_queue->head = NULL;
	new_queue->tail = NULL;

	return new_queue;
}

/**
 * 入队操作 - 将元素添加到队列尾部
 * @param queue_ptr: 队列指针
 * @param element_data: 要入队的数据
 * @return: 成功返回SUCCESS，失败返回错误码
 * 
 * 算法：尾插法
 * - 第一个元素：head和tail都指向它
 * - 后续元素：追加到tail后面，更新tail
 */
int enqueue_element(st_queue *queue_ptr, int element_data) {
	if (queue_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return PARAM_ERROR;
	}

	ListNode *new_node = create_node(element_data);
	if (new_node == NULL) {
		printf("Error: %s create_node failed\n", __func__);
		return MEMORY_ERROR;
	}

	if (queue_ptr->head == NULL) {
		// 队列为空，第一个元素
		queue_ptr->head = new_node;
		queue_ptr->tail = new_node;
	} else {
		// 队列非空，追加到尾部
		queue_ptr->tail->next = new_node;
		queue_ptr->tail = new_node;
	}

	return SUCCESS;
}

/**
 * 出队操作 - 从队列头部移除并返回元素
 * @param queue_ptr: 队列指针
 * @return: 返回出队的节点指针（需调用者释放），队列空返回NULL
 * 
 * 算法：头删法
 * - 取出队头元素
 * - 更新队头指针
 * - 如果队列变空，同时更新队尾指针
 */
ListNode *dequeue_element(st_queue *queue_ptr) {
	if (queue_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return NULL;
	}

	ListNode *dequeued_node = NULL;

	// 队列为空
	if (queue_ptr->head == NULL) {
		return NULL;
	}

	// 取出队头节点
	dequeued_node = queue_ptr->head;

	// 只有一个节点的情况
	if (queue_ptr->tail == queue_ptr->head) {
		queue_ptr->tail = NULL;
		queue_ptr->head = NULL;
	} else {
		// 多个节点，更新队头
		queue_ptr->head = dequeued_node->next;
	}

	return dequeued_node;
}

/**
 * 获取队列长度 - 计算队列中元素的个数
 * @param queue_ptr: 队列指针
 * @return: 队列长度，错误返回PARAM_ERR
 */
int get_queue_length(st_queue *queue_ptr) {
	if (queue_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return PARAM_ERROR;
	}
	
	int element_count = 0;

	element_count = get_list_length(queue_ptr->head);

	return element_count;
}

/**
 * 检查队列是否为空
 * @param queue_ptr: 队列指针
 * @return: 空返回true，非空返回false
 */
bool is_queue_empty(st_queue *queue_ptr) {
	if (queue_ptr == NULL) {
		return true;
	}

	return (queue_ptr->head == NULL && queue_ptr->tail == NULL);
}

void testQueue(void){
	ListNode *dequeued_node = NULL;
	int queue_length = 0;
	bool is_empty = false;
	
	gqueue = createQueue();
	is_empty = isQueueEmpty(gqueue);
	if(is_empty){
		printf("队列为空\n");
	} else {
		printf("队列不为空\n");
	}
	
	inQueue(gqueue, 22);
	inQueue(gqueue, 32);
	inQueue(gqueue, 19);
	inQueue(gqueue, 53);
	inQueue(gqueue, 0);
	inQueue(gqueue, 47);
	inQueue(gqueue, 29);
	queue_length = getQueueLength(gqueue);
	printf("队列长度 = %d\n", queue_length);	
	dumpQueue(gqueue);

	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	queue_length = getQueueLength(gqueue);
	printf("队列长度 = %d\n", queue_length);	
	dumpQueue(gqueue);	
	
	is_empty = isQueueEmpty(gqueue);
	if(is_empty){
		printf("队列为空\n");
	} else {
		printf("队列不为空\n");
	}	

	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	dequeued_node = deQueue(gqueue);
	if(dequeued_node != NULL)
		printf("出队元素 = %d\n", dequeued_node->data);
	free(dequeued_node);
	
	is_empty = isQueueEmpty(gqueue);
	if(is_empty){
		printf("队列为空\n");
	} else {
		printf("队列不为空\n");
	}

	return;
}



