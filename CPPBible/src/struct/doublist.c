#include <stdio.h>
#include <stdlib.h>
#include "doublist.h"

void dumpDoubList(st_doubNode * head){
	if(NULL == head){
		return;
	}

	st_doubNode * p = NULL;
	
	printf("========= Dump Double List %p ===========\n\t", head);
	p = head;
	while (NULL != p){
		printf(" %d ", p->data);
		p = p->next;
	}
	printf("\n");
	printf("===================================\n");
}


void dumpDoubListReverse(st_doubNode * head){
	if(NULL == head){
		return;
	}

	st_doubNode * p = NULL;
	st_doubNode * tail = NULL;
	
	tail = head;
	while (NULL != tail->next){
		tail = tail->next;
	}

	p = tail;
	printf("========= dump DoubList Reverse %p ===========\n\t", p);
	while(NULL != p){
		printf(" %d ", p->data);
		p = p->prev;
	}	
	
	printf("\n");
	printf("================================================\n");
}


int createDoubList(st_doubNode ** phead, int * array, int size){

	if(NULL == array || size < 0){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	if(0 == size){ /*没有数据，初始化个毛线*/
		return;
	}

	st_doubNode * head = * phead;
	st_doubNode * p = NULL, *q = NULL;
	int i = 0;

	/*分配并初始化头结点*/
	head = (st_doubNode * ) malloc (sizeof(st_doubNode));
	if(NULL == head){
		printf("%s: malloc error\n",__func__);
		return ALLOC_ERR;
	}	
	head->next = NULL;
	head->prev = NULL;

	p = head;	
	for(i = 0; i < size; i++){	
		/*首节点，未赋值*/
		if(NULL == head->next){
			p->data = array[i];
			/*这个做个小技巧，把next指向头自己，表示已经赋值了*/
			p->next = head; 
		} else {
			q = (st_doubNode * ) malloc (sizeof(st_doubNode));
			if(NULL == q){
				printf("%s: malloc error\n", __func__);
				return ALLOC_ERR;
			}	
			q->data = array[i];
			q->prev = p;
			q->next = NULL;
			p->next = q;
			p = q;
		}
	}
	
	*phead = head;

	return SUCCESS;
}


int getDoubListLen(st_doubNode * head){
	int len = 0;
	st_doubNode * p = NULL;

	if(NULL == head){
		goto out;
	}

	p = head;
	while(NULL != p){
		len++;
		p = p->next;
	}
		
out:
	return len;
}


st_doubNode* searchDoubListNode(st_doubNode* head, int num){
	if(NULL == head){
		return NULL;
	}

	st_doubNode* p = NULL;
	st_doubNode* q = NULL;

	p = head;
	while(NULL != p){
		if(p->data == num){
			q = p;
			break;
		}

		p = p->next;
	}
	
	return q;
}

st_doubNode * findDoubListPos(st_doubNode * head, int pos){
	if(NULL == head || pos < 0){
		return NULL;
	}

	if(0 == pos) {
		return head;
	}

	st_doubNode * p = NULL;
	st_doubNode * q = NULL;
	int len = getDoubListLen(head);

	if(pos > len) {
		printf("超过链表长度了！\n");
		return NULL;
	}

	p = head;
	while(p != NULL){
		if(pos-- == 0){
			q = p;
			break;
		}
		p = p->next;
	}

	return q;
}


/*
 * 传入head是二重指针，是因为插入头结点的时候，会改变head的指向到新的节点
 * 这里在PoS前面插入，注意这里的pos是按照数组方式来的，从0开始，所以12个插入，pos传11
 */
st_doubNode * insertDoubListNode(st_doubNode** phead, int pos, int data){
	if(NULL == phead || pos < 0){
		printf("%s: param error\n",__func__);
		return NULL;
	}

	st_doubNode * head = *phead;
	st_doubNode * p = NULL;
	st_doubNode * prev = NULL;
	st_doubNode * nw = NULL;

	nw = (st_doubNode *) malloc(sizeof(st_doubNode));
	if(NULL == nw){
		printf("%s: memory alloc failed\n", __func__);
		return NULL;
	}
	nw->data = data;
	nw->prev = NULL;
	nw->next = NULL;

	if(0 == pos){ // 首节点
		head->prev = nw;
		nw->next = head;		
		*phead = nw;  /*插入后更新首节点的值*/
	} else { // 中间节点
		p = findDoubListPos(head, pos);
		prev = p->prev;
		prev->next = nw;
		nw->prev = prev;
		nw->next = p;
		p->prev = nw;		
	}

	return nw;
}


st_doubNode * removeDoubListNode(st_doubNode** phead, int pos){
	if(NULL == phead || pos < 0){
		printf("%s: param error\n",__func__);
		return NULL;
	}

	st_doubNode * head = *phead;
	st_doubNode * p = NULL;
	st_doubNode * q = NULL;
	st_doubNode * nw = NULL;

	if(0 == pos) { /*首节点*/
		/*摘掉*/
		nw = head;
		p = head->next;
		p->prev = NULL;

		/*清除*/
		nw->next = NULL;
		nw->prev = NULL;

		/*更新头*/
		head = p;
		
	} else { /* 其他节点 */
		nw = findDoubListPos(head, pos);
		p = nw->next;
		q = nw->prev;

		p->prev = q;
		q->next = p;

		nw->next = NULL;		
		nw->prev = NULL;
	}

	*phead = head;

	return nw;
}


st_doubNode * getDouListTail(st_doubNode* head){
	if(NULL == head ){
		printf("%s: param error\n",__func__);
		return NULL;
	}
	
	st_doubNode * tail = NULL;
	tail = head;
	while(NULL != tail->next){
		tail = tail->next;
	}

	return tail;
}


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
	if(head == tail){
		printf("\t exit resc head = %d \n", head->data);
		return SUCCESS;
	}
	
	pivotpos = head;
	pivot = pivotpos->data;
	p = head;

	while(NULL != p){
		/*小于的移动到pivot之前*/
		if(p->data < pivot){
			mov = p; /*要移动的变量*/
			p = p->next; /*下一轮位置变量*/

			/*摘除*/
			mov->prev->next = mov->next;
			if(NULL != mov->next){ 
				mov->next->prev = mov->prev;
			} else { /*尾结点处理，mov是尾结点的时候*/
				/*更新tail, 为新的尾结点*/
				tail = mov->prev;
			}
			
			/*挂载*/
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
	}
	
	/*
	 * 对大的子链排序
	 * 注意 排序后，pivot 是最后一个元素的时候，后面就没有元素了，不用排序了
	 * 这时候 next 为 NULL，就不用在排序了
	 */
	if (next != NULL){
		quickSortDoubList(&next, &tail);
	}

	/*因为next可能变化，重新连接*/
	pivotpos->next = next;
	next->prev = pivotpos;

	*phead = head;
	*ptail = tail;
		
	return SUCCESS;
}


int insertDoubLoopSortedListNode(st_doubNode** phead, int data){
	if(NULL == phead){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	st_doubNode * head = NULL;
	st_doubNode * iter = NULL;
	st_doubNode * p = NULL;
	
	head = *phead;
	if(NULL == head){ /*新加入的节点*/
		p = (st_doubNode * ) malloc (sizeof(st_doubNode));
		if(NULL == p){
			printf("%s: alloc error\n",__func__);
			return ALLOC_ERR;
		}
		p->data = data;
		p->next = p;
		p->prev = p;	
		head = p;
	}

	iter = head;
	while(iter->next != head){
		if(data < iter->data){
			/*分配节点*/
			p = (st_doubNode * ) malloc (sizeof(st_doubNode));
			if(NULL == p){
				printf("%s: alloc error\n",__func__);
				return ALLOC_ERR;
			}		
			p->data = data;
			p->next = NULL;
			p->prev = NULL;

			/*插入*/
			iter->prev->next = p;
			p->prev = iter->prev;
			iter->prev = p;
			p->next = iter;			
			
			if(head == iter){ /*头之前插入，更新头*/
				head = p;
			}
			
			goto out;
		}
		iter = iter->next;
	}

	/*尾巴处理, 此时iter指向尾巴*/
	if(NULL == p){ 
		/*分配节点*/
		p = (st_doubNode * ) malloc (sizeof(st_doubNode));
		if(NULL == p){
			printf("%s: alloc error\n",__func__);
			return ALLOC_ERR;
		}
		p->data = data;
		p->next = NULL;
		p->prev = NULL;		

		/*插在倒数第二个节点*/
		if(data < iter->data){
			iter->prev->next = p;
			p->prev = iter->prev;
			p->next = iter;
			iter->prev = p;
		} else { /*插在最后一个节点*/
			iter->next = p;
			p->prev = iter;
			p->next = head;			
		}		
	}

out:
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
			/* 摘除点 */
			p = iter;
			iter->prev->next = iter->next;
			iter->next->prev = iter->prev;
			/* 下一循环点 */
			iter = iter->next;
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


void testremoveDoubLoopListSameNode(void){

	printf("************  testremoveDoubLoopListSameNode ************ \n");

	st_doubNode * tail = NULL;

	/*初始化input数据*/
	int input1[10] = {9,6,8,3,0,11,28,119,5,7};
	gDoubHead1 = NULL;
	createDoubList(&gDoubHead1, input1, 10);
	dumpDoubList(gDoubHead1);
	tail = getDouListTail(gDoubHead1);
	tail->next = gDoubHead1;
	gDoubHead1->prev = tail;	

	/*初始化input数据*/
	int input2[10] = {0,1,2,3,4,5,6,7,8,9};
	gDoubHead2 = NULL;
	createDoubList(&gDoubHead2, input2, 10);
	dumpDoubList(gDoubHead2);
	tail = getDouListTail(gDoubHead2);
	tail->next = gDoubHead2;
	gDoubHead2->prev = tail;

	removeDoubLoopListSameNode(&gDoubHead1, &gDoubHead2);

	tail = getDouLoopListTail(gDoubHead1);
	tail->next = NULL;
	gDoubHead1->prev = NULL;	

	tail = getDouLoopListTail(gDoubHead2);
	tail->next = NULL;
	gDoubHead2->prev = NULL;	

	dumpDoubList(gDoubHead1);
	dumpDoubListReverse(gDoubHead1);
	dumpDoubList(gDoubHead2);
	dumpDoubListReverse(gDoubHead2);

}


void testinsertDoubLoopSortedListNode(void){

	st_doubNode * tail = NULL;

	/*构建双向循环链表*/
	tail = getDouListTail(gDoubHead);
	tail->next = gDoubHead;
	gDoubHead->prev = tail;

	insertDoubLoopSortedListNode(&gDoubHead, 22);
	insertDoubLoopSortedListNode(&gDoubHead, 21);
	insertDoubLoopSortedListNode(&gDoubHead, 0);
	insertDoubLoopSortedListNode(&gDoubHead, 63);
	insertDoubLoopSortedListNode(&gDoubHead, 119);

	/*打开换，因为tail可能会变化，所以要重新取*/
	tail = getDouLoopListTail(gDoubHead);
	tail->next = NULL;
	gDoubHead->prev = NULL;

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


void testgetDoubListLen(void){
	int len = 0;

	printf("************  testCreateDoubList ************ \n");

	len = getDoubListLen(gDoubHead);

	printf("len  = %d \n", len);

	return;
}



void testCreateDoubList(void){
	/*初始化input数据*/
	int input[10] = {22,32,19,53,1,47,29,116,4,6};
	gDoubHead = NULL;


	printf("************  testCreateDoubList ************ \n");

	createDoubList(&gDoubHead, input, 10);

	dumpDoubList(gDoubHead);

	return;
}



