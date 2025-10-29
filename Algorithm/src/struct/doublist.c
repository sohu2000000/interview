#include <stdio.h>
#include <stdlib.h>
#include "doublist.h"

/**
 * 双向链表节点连接宏 - 简化链表节点的链接操作
 * 
 * 注意：只定义安全且常用的宏，复杂操作仍用显式代码以保证清晰性
 */

/* 将new_node插入到target_node之前 */
#define DOUBLY_INSERT_BEFORE(target_node, new_node) do { \
    (target_node)->prev->next = (new_node); \
    (new_node)->prev = (target_node)->prev; \
    (new_node)->next = (target_node); \
    (target_node)->prev = (new_node); \
} while(0)

/* 将new_node插入到target_node之后 */
#define DOUBLY_INSERT_AFTER(target_node, new_node) do { \
    (new_node)->next = (target_node)->next; \
    (new_node)->prev = (target_node); \
    if ((target_node)->next) (target_node)->next->prev = (new_node); \
    (target_node)->next = (new_node); \
} while(0)

/* 在node_before和node_after之间插入new_node */
#define DOUBLY_INSERT_BETWEEN(node_before, new_node, node_after) do { \
    (node_before)->next = (new_node); \
    (new_node)->prev = (node_before); \
    (new_node)->next = (node_after); \
    (node_after)->prev = (new_node); \
} while(0)

/* 设置节点为自循环（用于循环链表的第一个节点） */
#define DOUBLY_MAKE_SELF_LOOP(node) do { \
    (node)->next = (node); \
    (node)->prev = (node); \
} while(0)

/* 断开节点的所有连接 */
#define DOUBLY_DISCONNECT(node) do { \
    (node)->next = NULL; \
    (node)->prev = NULL; \
} while(0)

/* 将循环链表转换为普通链表（断开首尾连接） */
#define DOUBLY_BREAK_LOOP(head, tail) do { \
    (tail)->next = NULL; \
    (head)->prev = NULL; \
} while(0)

/* 将普通链表转换为循环链表（连接首尾） */
#define DOUBLY_MAKE_LOOP(head, tail) do { \
    (tail)->next = (head); \
    (head)->prev = (tail); \
} while(0)

/* 连接两个节点（node1 -> node2） */
#define DOUBLY_CONNECT(node1, node2) do { \
    (node1)->next = (node2); \
    (node2)->prev = (node1); \
} while(0)

/* 从循环链表中摘除节点（只重新连接前后节点，不修改node本身） */
#define DOUBLY_DETACH_FROM_LOOP(node) do { \
    (node)->prev->next = (node)->next; \
    (node)->next->prev = (node)->prev; \
} while(0)

/* ============================================================================
 * 【1. 创建和显示函数】
 * ============================================================================ */

/**
 * 创建双向链表 - 从整数数组创建双向链表
 * @param list_head_ptr: 链表头指针的指针（二级指针，用于返回新链表头）
 * @param source_array: 源数据数组
 * @param array_size: 数组大小
 * @return: 成功返回SUCCESS，失败返回错误码
 * 
 * 算法：尾插法
 * 1. 创建头节点
 * 2. 依次创建新节点并连接到链表尾部
 * 3. 维护双向指针（prev和next）
 */
int create_doubly_linked_list(st_doubNode **list_head_ptr, int *source_array, int array_size) {
	// 参数有效性检查
	if (source_array == NULL || array_size < 0) {
		printf("Error: %s parameter error\n", __func__);
		return PARAM_ERR;
	}	

	if (array_size == 0) {
		return SUCCESS;  // 空数组，无需创建
	}

	st_doubNode *list_head = NULL;
	st_doubNode *current_tail = NULL;
	st_doubNode *new_node = NULL;

	// 分配并初始化头节点
	list_head = (st_doubNode *)malloc(sizeof(st_doubNode));
	if (list_head == NULL) {
		printf("Error: %s malloc failed\n", __func__);
		return ALLOC_ERR;
	}	
	DOUBLY_DISCONNECT(list_head);

	current_tail = list_head;
	
	for (int array_index = 0; array_index < array_size; array_index++) {
		// 处理头节点（第一个元素）
		if (list_head->next == NULL) {
			current_tail->data = source_array[array_index];
			// 技巧：用next指向自己表示已赋值
			current_tail->next = list_head;
		} else {
			// 创建新节点
			new_node = (st_doubNode *)malloc(sizeof(st_doubNode));
			if (new_node == NULL) {
				printf("Error: %s malloc failed\n", __func__);
				return ALLOC_ERR;
			}
			
			new_node->data = source_array[array_index];
			new_node->prev = current_tail;
			new_node->next = NULL;
			current_tail->next = new_node;
			current_tail = new_node;
		}
	}
	
	*list_head_ptr = list_head;

	return SUCCESS;
}

/**
 * 打印双向链表（正向遍历）- 从头到尾打印链表所有节点的数据
 * @param list_head: 链表头节点指针
 * 
 * 功能：正向遍历并打印链表的所有节点数据
 */
void display_doubly_linked_list_forward(st_doubNode *list_head) {
	if (list_head == NULL) {
		return;
	}

	st_doubNode *current_node = NULL;
	
	printf("========= 双向链表（正向）%p ===========\n\t", list_head);
	current_node = list_head;
	while (current_node != NULL) {
		printf(" %d ", current_node->data);
		current_node = current_node->next;
	}
	printf("\n");
	printf("===================================\n");
}

/**
 * 打印双向链表（反向遍历）- 从尾到头打印链表所有节点的数据
 * @param list_head: 链表头节点指针
 * 
 * 功能：先找到尾节点，然后反向遍历并打印
 */
void display_doubly_linked_list_backward(st_doubNode *list_head) {
	if (list_head == NULL) {
		return;
	}

	st_doubNode *current_node = NULL;
	st_doubNode *tail_node = NULL;
	
	// 找到尾节点
	tail_node = list_head;
	while (tail_node->next != NULL) {
		tail_node = tail_node->next;
	}

	// 从尾到头打印
	current_node = tail_node;
	printf("========= 双向链表（反向）%p ===========\n\t", current_node);
	while (current_node != NULL) {
		printf(" %d ", current_node->data);
		current_node = current_node->prev;
	}	
	
	printf("\n");
	printf("================================================\n");
}


/* ============================================================================
 * 【2. 查询操作函数】
 * ============================================================================ */

/**
 * 获取双向链表长度 - 计算链表中节点的个数
 * @param list_head: 链表头节点指针
 * @return: 链表长度（节点个数）
 */
int get_doubly_linked_list_length(st_doubNode *list_head) {
	int node_count = 0;
	st_doubNode *current_node = NULL;

	if (list_head == NULL) {
		return 0;
	}

	current_node = list_head;
	while (current_node != NULL) {
		node_count++;
		current_node = current_node->next;
	}

	return node_count;
}

/**
 * 按值搜索节点 - 在双向链表中查找第一个匹配指定值的节点
 * @param list_head: 链表头节点指针
 * @param target_value: 要查找的数据值
 * @return: 找到返回节点指针，否则返回NULL
 */
st_doubNode *search_doubly_list_node_by_value(st_doubNode *list_head, int target_value) {
	if (list_head == NULL) {
		return NULL;
	}

	st_doubNode *current_node = NULL;
	st_doubNode *found_node = NULL;

	current_node = list_head;
	while (current_node != NULL) {
		if (current_node->data == target_value) {
			found_node = current_node;
			break;
		}
		current_node = current_node->next;
	}
	
	return found_node;
}

/**
 * 按位置查找节点 - 查找双向链表中指定位置的节点
 * @param list_head: 链表头节点指针
 * @param target_position: 目标位置（从0开始）
 * @return: 找到返回节点指针，否则返回NULL
 */
st_doubNode *find_doubly_list_node_at_position(st_doubNode *list_head, int target_position) {
	if (list_head == NULL || target_position < 0) {
		return NULL;
	}

	if (target_position == 0) {
		return list_head;
	}

	st_doubNode *current_node = NULL;
	st_doubNode *target_node = NULL;
	int list_length = get_doubly_linked_list_length(list_head);

	if (target_position >= list_length) {
		printf("Error: Position exceeds list length!\n");
		return NULL;
	}

	current_node = list_head;
	int current_position = target_position;
	
	while (current_node != NULL) {
		if (current_position == 0) {
			target_node = current_node;
			break;
		}
		current_position--;
		current_node = current_node->next;
	}

	return target_node;
}

/**
 * 获取链表尾节点 - 查找双向链表的最后一个节点
 * @param list_head: 链表头节点指针
 * @return: 返回尾节点指针，失败返回NULL
 */
st_doubNode *get_doubly_list_tail_node(st_doubNode *list_head) {
	if (list_head == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return NULL;
	}

	st_doubNode *tail_node = NULL;
	
	tail_node = list_head;
	while (tail_node->next != NULL) {
		tail_node = tail_node->next;
	}

	return tail_node;
}


/* ============================================================================
 * 【3. 插入和删除函数】
 * ============================================================================ */

/**
 * 在指定位置插入节点 - 在双向链表的指定位置前插入新节点
 * @param list_head_ptr: 链表头指针的指针（二级指针，因为插入头部时会改变头指针）
 * @param insert_position: 插入位置（从0开始，在此位置之前插入）
 * @param node_data: 新节点的数据
 * @return: 成功返回新节点指针，失败返回NULL
 * 
 * 注意：
 * - 位置0表示在头部插入（成为新的头节点）
 * - 插入位置pos表示在pos节点之前插入
 */
st_doubNode *insert_doubly_list_node_at_position(st_doubNode **list_head_ptr, int insert_position, int node_data) {
	if (list_head_ptr == NULL || insert_position < 0) {
		printf("Error: %s parameter error\n", __func__);
		return NULL;
	}

	st_doubNode *list_head = *list_head_ptr;
	st_doubNode *position_node = NULL;
	st_doubNode *previous_node = NULL;
	st_doubNode *new_node = NULL;

	// 分配新节点
	new_node = (st_doubNode *)malloc(sizeof(st_doubNode));
	if (new_node == NULL) {
		printf("Error: %s memory allocation failed\n", __func__);
		return NULL;
	}
	new_node->data = node_data;
	new_node->prev = NULL;
	new_node->next = NULL;

	if (insert_position == 0) {
		// 在头部插入
		new_node->next = list_head;
		new_node->prev = NULL;
		list_head->prev = new_node;
		*list_head_ptr = new_node;  // 更新头指针
	} else {
		// 在中间位置插入
		position_node = find_doubly_list_node_at_position(list_head, insert_position);
		DOUBLY_INSERT_BEFORE(position_node, new_node);
	}

	return new_node;
}

/**
 * 删除指定位置的节点 - 从双向链表中删除指定位置的节点
 * @param list_head_ptr: 链表头指针的指针（二级指针，因为删除头部时会改变头指针）
 * @param remove_position: 要删除的节点位置（从0开始）
 * @return: 返回被删除的节点指针（需要调用者释放内存），失败返回NULL
 * 
 * 注意：返回的节点已从链表中摘除，但内存未释放，需要调用者free
 */
st_doubNode *remove_doubly_list_node_at_position(st_doubNode **list_head_ptr, int remove_position) {
	if (list_head_ptr == NULL || remove_position < 0) {
		printf("Error: %s parameter error\n", __func__);
		return NULL;
	}

	st_doubNode *list_head = *list_head_ptr;
	st_doubNode *next_node = NULL;
	st_doubNode *previous_node = NULL;
	st_doubNode *removed_node = NULL;

	if (remove_position == 0) {
		// 删除头节点
		removed_node = list_head;
		next_node = list_head->next;
		next_node->prev = NULL;

		// 清除被删除节点的指针
		DOUBLY_DISCONNECT(removed_node);

		// 更新头指针
		list_head = next_node;
		
	} else {
		// 删除中间或尾部节点
		removed_node = find_doubly_list_node_at_position(list_head, remove_position);
		next_node = removed_node->next;
		previous_node = removed_node->prev;

		// 重新连接前后节点
		previous_node->next = next_node;
		next_node->prev = previous_node;

		// 清除被删除节点的指针
		DOUBLY_DISCONNECT(removed_node);
	}

	*list_head_ptr = list_head;

	return removed_node;
}


/* ============================================================================
 * 【4. 高级操作函数】（排序、循环链表等）
 * ============================================================================ */

/* head 和 tail 的指针的排序的时候，都有可能发生变化，所以这里使用二级指针 */
int quickSortDoubList(st_doubNode** phead, st_doubNode ** ptail){
	if(NULL == phead || NULL == *phead || NULL == ptail || NULL == *ptail){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}

	st_doubNode * head = NULL;
	st_doubNode * tail = NULL;
	st_doubNode * p = NULL;
	st_doubNode * mov = NULL;
	st_doubNode * prev = NULL;
	st_doubNode * next = NULL;
	st_doubNode * pivotpos = NULL;
	int pivot = 0;

	head = *phead;
	tail = *ptail;

	/*迭代退出的条件, 只有一个元素，或者没有元素*/
	if(head == tail) {
		printf("\t exit resc head = %d \n", head->data);
		return SUCCESS;
	}
	
	pivotpos = head;
	pivot = pivotpos->data;
	p = head;

	while(NULL != p) {
		/*小于的移动到pivot之前*/
		if(p->data < pivot){
			mov = p; /*要移动的变量*/
			p = p->next; /*下一轮位置变量*/

			/*摘除节点：需要先判断是否尾节点，再摘除*/
			int is_tail_node = (mov->next == NULL);
			mov->prev->next = mov->next;
			if(mov->next != NULL) { 
				mov->next->prev = mov->prev;
			}
			
			/*如果是尾节点，更新tail*/
			if(is_tail_node) {
				tail = mov->prev;
			}
			
			/*挂载到head之前（成为新的head）*/
			mov->next = head;
			mov->prev = NULL;
			head->prev = mov;

			/*更新新的head*/
			head = mov;
			continue;
		}
		/*大于pivot不变*/
		p = p->next;
	}

#ifdef DEBUG
	st_doubNode * q = NULL;
	printf("\n---------------------------\n");
	printf("\t[ ");
	q = head;
	while(q != pivotpos){
		printf(" %d ", q->data);
		q = q->next;
	}
	printf("] ");
	printf(" pivot = %d ", q->data);
	printf(" [ ");
	q = pivotpos->next;
	while(q != tail){
		printf(" %d ", q->data);
		q = q->next;
	}
	printf(" %d ", tail->data);
	printf("] \n");
	
	printf("---------------------------\n");
#endif

	prev = pivotpos->prev;
	next = pivotpos->next;
	
	/*
	 * 对小的子链排序
	 * 注意 排序后，pivot 是第一个元素的时候，前面就没有元素了，不用排序了
	 * 这时候 prev 为 NULL，就不用在排序了
	 */	
	if(prev != NULL){
		quickSortDoubList(&head, &prev);
		// 重新连接：左子链排序后，prev可能变化，需要重新连接
		DOUBLY_CONNECT(prev, pivotpos);
	} else {
		// pivot是第一个元素，没有前驱
		pivotpos->prev = NULL;
	}
	
	/*
	 * 对大的子链排序
	 * 注意 排序后，pivot 是最后一个元素的时候，后面就没有元素了，不用排序了
	 * 这时候 next 为 NULL，就不用在排序了
	 */
	if (next != NULL){
		quickSortDoubList(&next, &tail);
		// 重新连接：右子链排序后，next可能变化，需要重新连接
		DOUBLY_CONNECT(pivotpos, next);
	} else {
		// pivot是最后一个元素，没有后继
		pivotpos->next = NULL;
	}

	*phead = head;
	*ptail = tail;
		
	return SUCCESS;
}

/**
 * 循环链表有序插入 - 在双向循环有序链表中插入新节点（保持有序）
 * @param phead: 链表头指针的指针
 * @param data: 要插入的数据
 * @return: 成功返回SUCCESS，失败返回错误码
 * 
 * 功能：在有序的双向循环链表中找到正确位置并插入新节点
 */
int insertDoubLoopSortedListNode(st_doubNode** phead, int data){
	if(NULL == phead){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	st_doubNode * head = NULL;
	st_doubNode * iter = NULL;
	st_doubNode * new_node = NULL;
	
	// 统一分配新节点（避免重复代码）
	new_node = (st_doubNode *)malloc(sizeof(st_doubNode));
	if(NULL == new_node){
			printf("%s: alloc error\n",__func__);
			return ALLOC_ERR;
		}
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;

	head = *phead;
	
	// 情况1: 空链表，创建第一个节点（自循环）
	if(NULL == head){
		DOUBLY_MAKE_SELF_LOOP(new_node);
		*phead = new_node;
		return SUCCESS;
	}

	// 情况2: 在链表中间找到插入位置
	iter = head;
	while(iter->next != head){
		if(data < iter->data) {
			// 在iter之前插入
			DOUBLY_INSERT_BEFORE(iter, new_node);
			
			if(head == iter) { 
				// 在头之前插入，更新头指针
				head = new_node;
			}
			
			*phead = head;
			return SUCCESS;
		}
		iter = iter->next;
	}

	// 情况3: 处理尾节点位置
	// 此时iter指向尾节点
	if(data < iter->data){
		// 插在倒数第二个节点和尾节点之间
		DOUBLY_INSERT_BEFORE(iter, new_node);
	} else {
		// 插在最后（成为新的尾节点）
		DOUBLY_INSERT_BETWEEN(iter, new_node, head);
	}

	*phead = head;
	return SUCCESS;
}

st_doubNode * getDouLoopListTail(st_doubNode* head){
	if(NULL == head ){
		printf("%s: param error\n",__func__);
		return NULL;
	}
	
	st_doubNode * tail = NULL;
	tail = head;
	while(head != tail->next){
		tail = tail->next;
	}

	return tail;
}

int removeDoubLoopListNodeByData(st_doubNode ** phead, int data){
	if(NULL == phead ){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}		

	if(NULL == phead){
		return SUCCESS;
	}

	st_doubNode * head = *phead;
	st_doubNode * iter = NULL;
	st_doubNode * p = NULL;

	iter = head;
	do {
		/* 相同 */
		if(data == iter->data){
			/* 保存要删除的节点和下一个节点 */
			p = iter;
			iter = iter->next;  // 先保存下一个节点
			
			/* 摘除节点（重新连接前后节点） */
			DOUBLY_DETACH_FROM_LOOP(p);
			
			/*首节点，更新头*/
			if(p == head){
				head = iter;
			}
			
			free(p);
			p = NULL;
			continue;
		}
		
		iter = iter->next;
	}while(iter != head);

	*phead = head;
	
	return SUCCESS;
}

int removeDoubLoopListSameNode(st_doubNode** phead1, st_doubNode** phead2){
	if(NULL == phead1 || NULL == phead2){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

#define ARRAR_SIZE	100

	st_doubNode * head1 = *phead1;
	st_doubNode * head2 = *phead2;
	st_doubNode * tail1 = NULL, * tail2 = NULL;
	st_doubNode * iter1 = NULL, * iter2 = NULL;

	int samevals[ARRAR_SIZE] = {0};
	int cnt = 0, i = 0, data = 0;

	if(NULL == head1 || NULL == head2){
		return SUCCESS;
	}

	tail1 = getDouLoopListTail(head1);
	tail2 = getDouLoopListTail(head2);

	/*寻找相同节点*/	
	iter1 = head1;	
	/*
	 * 注意, 对于循环链表使用 do...while 结构，这样用 iter !=  head 可以处理的到头尾节点。      
	 */
	do {
		data = iter1->data;	
		iter2 = head2;
		do {
			if(iter2->data == data){
				samevals[cnt++] = data;
			}
			iter2 = iter2->next;
		} while (iter2 != head2);
		iter1 = iter1->next;
	} while (iter1 != head1);

#ifdef DEBUG
	printf("cnt = %d\n", cnt);
	for(i = 0; i < cnt; i++){
		printf("samevals[%d] = %d\n", i, samevals[i]);
	}
#endif

	// TODO: 删除相同数值节点
	for(i = 0; i < cnt; i++) {
		data = samevals[i];
		removeDoubLoopListNodeByData(&head1, data);
		removeDoubLoopListNodeByData(&head2, data);
	}

	*phead1 = head1;
	*phead2 = head2;

#undef ARRAR_SIZE

	return SUCCESS;
}


/* ============================================================================
 * 【5. 测试函数】
 * ============================================================================ */

void testCreateDoubList(void){
	/*初始化input数据*/
	int input[10] = {22,32,19,53,1,47,29,116,4,6};
	gDoubHead = NULL;


	printf("************  testCreateDoubList ************ \n");

	createDoubList(&gDoubHead, input, 10);

	dumpDoubList(gDoubHead);

	return;
}

void testgetDoubListLen(void){
	int len = 0;

	printf("************  testCreateDoubList ************ \n");
	
	len = getDoubListLen(gDoubHead);
	
	printf("len  = %d \n", len);

	return;
}

void testSearchDoubList(void){
	st_doubNode * rst = NULL;
	rst = searchDoubListNode(gDoubHead, 6);

	printf("\n************  testSearchDoubList ************ \n");
	
	if(NULL != rst){
		printf("node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found num 6 \n");
	}

	rst = searchDoubListNode(gDoubHead, 119);
	if(NULL != rst){
		printf("node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found num 119 \n");
	}	
}

void testFindDoubListPos(void){
	st_doubNode * rst = NULL;

	printf("\n************  testFindDoubListPos ************ \n");
	
	rst = findDoubListPos(gDoubHead, -1);
	if(NULL != rst){
		printf("find pos -1 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos -1 node \n");
	}	

	rst = findDoubListPos(gDoubHead, 0);
	if(NULL != rst){
		printf("find pos 0 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 0 node \n");
	}		

	rst = findDoubListPos(gDoubHead, 5);
	if(NULL != rst){
		printf("find pos 5 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 5 node \n");
	}	

	rst = findDoubListPos(gDoubHead, 12);
	if(NULL != rst){
		printf("find pos 12 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 12 node \n");
	}	

	return;
	
}

void testInsertDoubNode(void){
	st_doubNode * nw = NULL;

	printf("\n************  testInsertDoubNode ************ \n");
	
	nw = insertDoubListNode(&gDoubHead, 0, 70);	
	dumpDoubList(gDoubHead);
	
	insertDoubListNode(&gDoubHead, 5, 31);
	dumpDoubList(gDoubHead);
	
	insertDoubListNode(&gDoubHead, 11, 66);
	dumpDoubList(gDoubHead);
	dumpDoubListReverse(gDoubHead);

	return;
}

void testremoveDoubListNode(void){
	st_doubNode * nw = NULL;

	printf("\n************  testremoveDoubListNode ************ \n");
	
	nw = removeDoubListNode(&gDoubHead, 11);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);
	dumpDoubList(gDoubHead);
	
	nw = removeDoubListNode(&gDoubHead, 5);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);
	dumpDoubList(gDoubHead);
	
	nw = removeDoubListNode(&gDoubHead, 0);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);	
	dumpDoubList(gDoubHead);
	dumpDoubListReverse(gDoubHead);

	return;
}

void testquickSortDoubList(void){
	printf("\n************  testquickSortDoubList ************ \n");
	st_doubNode * tail = NULL;
	tail = getDouListTail(gDoubHead);
	
	quickSortDoubList(&gDoubHead, &tail);
	
	dumpDoubList(gDoubHead);
	dumpDoubListReverse(gDoubHead);
	return;
}

void testinsertDoubLoopSortedListNode(void){

	st_doubNode * tail = NULL;

	/*构建双向循环链表*/
	tail = getDouListTail(gDoubHead);
	DOUBLY_MAKE_LOOP(gDoubHead, tail);

	insertDoubLoopSortedListNode(&gDoubHead, 22);
	insertDoubLoopSortedListNode(&gDoubHead, 21);
	insertDoubLoopSortedListNode(&gDoubHead, 0);
	insertDoubLoopSortedListNode(&gDoubHead, 63);
	insertDoubLoopSortedListNode(&gDoubHead, 119);

	/*打开环，因为tail可能会变化，所以要重新取*/
	tail = getDouLoopListTail(gDoubHead);
	DOUBLY_BREAK_LOOP(gDoubHead, tail);

	dumpDoubList(gDoubHead);
	dumpDoubListReverse(gDoubHead);

	return;
}

void testremoveDoubLoopListSameNode(void){

	printf("************  testremoveDoubLoopListSameNode ************ \n");

	st_doubNode * tail = NULL;

	/*初始化input数据*/
	int input1[10] = {9,6,8,3,0,11,28,119,5,7};
	gDoubHead1 = NULL;
	createDoubList(&gDoubHead1, input1, 10);
	dumpDoubList(gDoubHead1);
	tail = getDouListTail(gDoubHead1);
	DOUBLY_MAKE_LOOP(gDoubHead1, tail);

	/*初始化input数据*/
	int input2[10] = {0,1,2,3,4,5,6,7,8,9};
	gDoubHead2 = NULL;
	createDoubList(&gDoubHead2, input2, 10);
	dumpDoubList(gDoubHead2);
	tail = getDouListTail(gDoubHead2);
	DOUBLY_MAKE_LOOP(gDoubHead2, tail);

	removeDoubLoopListSameNode(&gDoubHead1, &gDoubHead2);

	tail = getDouLoopListTail(gDoubHead1);
	DOUBLY_BREAK_LOOP(gDoubHead1, tail);

	tail = getDouLoopListTail(gDoubHead2);
	DOUBLY_BREAK_LOOP(gDoubHead2, tail);	

	dumpDoubList(gDoubHead1);
	dumpDoubListReverse(gDoubHead1);
	dumpDoubList(gDoubHead2);
	dumpDoubListReverse(gDoubHead2);

}

