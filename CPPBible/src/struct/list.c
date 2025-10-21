
#include <stdio.h>
#include <stdlib.h>
#include "list.h"



st_dataNode * createListNode(int data){
	st_dataNode * p = NULL;
	p = (st_dataNode * ) malloc (sizeof(st_dataNode));
	if(NULL == p){
		printf("%s memory alloc error\n", __func__);
		return NULL;
	}

	p->data = data;
	p->next = NULL;

	return p;
}




void dumpList(st_dataNode * head){
	if(NULL == head){
		return;
	}

	st_dataNode * p = NULL;
	
	printf("========= Dump List %p ===========\n\t", head);
	p = head;
	while (NULL != p){
		printf(" %d ", p->data);
		p = p->next;
	}
	printf("\n");
	printf("===================================\n");
}

st_dataNode* createList(st_dataNode* head, int * array, int size){

	if(NULL == array){
		printf("%s para error\n", __func__);
		return NULL;
	}

	int i = 0;
	unsigned int st_size = sizeof(st_dataNode); 
	st_dataNode * p = NULL;
	st_dataNode * q = NULL;

	if(NULL == head){
		head = (st_dataNode *) malloc(st_size);
		head->next = NULL;
	}

	/*初始化头结点*/
	for(i = 0; i < size; i++){
		if(NULL == head->next){ /*首节点*/
			head->data = array[i];
			p = head; 
			/* 注意这里是为了区分首节点和中间节点
			 * 如果只有一个节点，会在循环外面，修复p->next = NULL*/
			p->next = head; 
		} else { /*中间节点*/
			/* 创建节点 */
			q = (st_dataNode *) malloc(sizeof(st_dataNode));
			q->data = array[i];
			q->next = NULL;				

			/*连接节点*/
			p->next = q;
			p = q;
		}			
	}

	/*尾结点*/
	p->next = NULL;

	return head;
}


int getListLen(st_dataNode* head){
	int len = 0;
	st_dataNode * p = NULL;

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


st_dataNode* searchListNode(st_dataNode* head, int num){
	if(NULL == head){
		return NULL;
	}

	st_dataNode* p = NULL;
	st_dataNode* q = NULL;

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


st_dataNode* findListPos(st_dataNode* head, int pos){
	if(NULL == head || pos < 0){
		return NULL;
	}

	if(0 == pos) {
		return head;
	}

	st_dataNode * p = NULL;
	st_dataNode * q = NULL;
	int len = getListLen(head);

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
 */
st_dataNode * insertListNode(st_dataNode** phead, int pos, int data){
	if(NULL == phead || pos < 0){
		printf("%s: param error\n",__func__);
		return NULL;
	}

	st_dataNode * head = *phead;
	st_dataNode * p = NULL;
	st_dataNode * q = NULL;
	st_dataNode * nw = NULL;

	nw = (st_dataNode *) malloc(sizeof(st_dataNode));
	if(NULL == nw){
		printf("%s: memory alloc failed\n", __func__);
		return NULL;
	}
	nw->data = data;

	if(0 == pos){ // 首节点
		p = head;
		nw->next = p;
		*phead = nw; /*插入后更新首节点的值*/
	} else { // 中间节点
		/* 注意，要找前面一个节点，单链表才能前后都链接上*/
		p = findListPos(head, pos - 1);
		q = p->next;
		p->next = nw;
		nw->next = q;	
	}

	return nw;
}


st_dataNode * removeListNode(st_dataNode** phead, int pos){
	if(NULL == phead || pos < 0){
		printf("%s: param error\n",__func__);
		return NULL;
	}

	st_dataNode * head = *phead;
	st_dataNode * p = NULL;
	st_dataNode * q = NULL;
	st_dataNode * nw = NULL;

	if(0 == pos) { /*首节点*/
		nw = head;
		p = head->next;
		nw->next = NULL;
		*phead = p;
	} else { /* 其他节点 */
		/* 注意，要找前面一个节点，单链表才能前后都链接上*/
		p = findListPos(head, pos - 1);
		nw = p->next;
		q = nw->next;
		p->next = q;
		nw->next = NULL;		
	}

	return nw;
}


int reverseList(st_dataNode** phead){
	if(NULL == phead || NULL == *phead){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	st_dataNode *head = *phead;
	st_dataNode *prev = NULL, *p = NULL, *next;

	p = head;
	while(p != NULL){
		next = p->next;
		p->next = prev;
		prev = p;
		p = next;
	}

	/*原来的最后一个节点，是现在的头结点，在prev里面*/
	*phead = prev;

	return SUCCESS;

}


st_dataNode * getListMidNode(st_dataNode * head){
	if(NULL == head){
		printf("%s: param error\n",__func__);
		return NULL;
	}	

	int len = 0;
	int m = 0;
	st_dataNode * mid = NULL;

	len = getListLen(head);
	m = (len / 2 - 1); // 注意应该建议，因为下标是从0开始的
	if(m == 0){
		return head;
	}
	
	mid = head;
	while(m > 0){
		mid = mid->next;
		m--;
	}

	return mid;	
}

/* 通过一次遍历来完成寻找中间节点 */
st_dataNode * getListMidNode2(st_dataNode * head){
	if(NULL == head){
		printf("%s: param error\n",__func__);
		return NULL;
	}	

	int step1 = 0, step2 = 0;
	st_dataNode * cur = NULL, * mid = NULL;

	cur = mid = head;
	while(NULL != cur){
		if(step2 /2 > step1){ /* cur 移动两步，mid 移动一步*/
			mid = mid->next;
			step1++;
		}

		cur = cur->next;
		step2++;
	}

	return mid;

}


int insertSortList(st_dataNode** phead){
	if(NULL == phead || NULL == *phead){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	st_dataNode * head = NULL;
	st_dataNode * prev = NULL, * cur = NULL, * next = NULL;  	/*遍历外循环使用*/
	st_dataNode * inprev = NULL, * incur = NULL, *innext = NULL, *intail = NULL;	/*遍历有序区使用*/

	head = *phead;
	cur = head->next; /*第一个元素默认是有序区，默认不排序*/
	prev = head;
	if(NULL == cur){ /* 就一个元素，排个毛序 */
		return SUCCESS;
	} 

	/*遍历无序区*/
	while(NULL != cur){
		
		next = cur->next;
#ifdef DEBUG
		if(NULL != next){
			printf("无序区 \n\t prev->data = %d, cur->data = %d, next->data = %d, head->data = %d \n", \
				prev->data, cur->data, next->data, head->data);
		}
		dumpList(head);
#endif
		/*
		 * 链表头处理
		 * 最小的元素插入到链表头
		 */
		if(cur->data < head->data){
#ifdef DEBUG
			printf("List Head \n\tprev->data = %d, cur->data = %d, next->data = %d, head->data = %d \n", \
				prev->data, cur->data, next->data, head->data);
			dumpList(head);
#endif		
			/* 摘掉 cur */
			prev->next = next;
			/* 插入到头 */
			cur->next = head;
			/* 更新头 */
			head = cur;
			
			/*更新cur, 因为摘除掉了cur, prev应该不变*/			
			cur = next;
#ifdef DEBUG
			dumpList(head);
#endif
			continue;
		}

		/*
		 * 中间元素
		 * 在有序区[head, pos, ...., cur-1]里面查找合适的插入位置
		 */
		inprev = head;
		incur = head->next;
		innext = incur->next;
		intail = cur;
#ifdef DEBUG
		printf("inprev->data = %d, incur->data = %d, innext->data = %d, intail->data = %d, cur->data = %d \n", \
			inprev->data, incur->data, innext->data, intail->data, cur->data);
#endif
		while(incur != intail){
			innext = incur->next;			
#ifdef DEBUG
			printf("Inner Loop \n");
			if(NULL != next){
				printf("\t inprev->data = %d, incur->data = %d, innext->data = %d, intail->data = %d, cur->data = %d, prev->data = %d, next->data = %d \n", \
					inprev->data, incur->data, innext->data, intail->data, cur->data, prev->data, next->data);
			}

			dumpList(head);
#endif

			if(cur->data < incur->data){
#ifdef DEBUG
				printf("Before insert mid nodes \n");
				if(NULL != next){
					printf("\t inprev->data = %d, incur->data = %d, innext->data = %d, intail->data = %d, cur->data = %d, prev->data = %d, next->data = %d \n", \
						inprev->data, incur->data, innext->data, intail->data, cur->data, prev->data, next->data);
				}

				dumpList(head);
#endif				
				/*摘掉cur*/
				prev->next = next;
			
				/*插入合适位置*/
				inprev->next = cur;
				cur->next = incur;	
#ifdef DEBUG
				printf("After insert mid nodes \n");
				dumpList(head);
#endif
				/*更新cur*/
				goto cont;
			}	

	
			inprev = incur;
			incur = innext;
		}

		/*移动到下一个*/
		prev = cur;	
cont:		
		cur = next;	
		
	}

	*phead = head;

	return SUCCESS;

}

bool whetherListLoop(st_dataNode * head){
	if(NULL == head){ /*空链表打环个毛线*/
		return false;
	}	

	bool rst = false; /* 默认不是打环的 */
	st_dataNode * pos1 = NULL, * pos2 = NULL;

	pos1 = pos2 = head;
	if(NULL == pos1->next){ /*head后一个节点且下一个为NULL，不可能打环*/
		goto out;
	}
	
	while(NULL != pos1 && NULL != pos1->next){
		/*一个走两步，一个走一步，看看能不能跟的上*/
		pos1 = pos1->next->next;
		pos2 = pos2->next;
		if(pos1 == pos2){
			rst = true;
			goto out;
		}
	}

	if(pos1 == pos2){
		rst = true;
		goto out;
	}

	
out:
	return rst;
}

int MergeSortedList(st_dataNode ** phead, st_dataNode * head2){
	if(NULL == phead || NULL == *phead || NULL == head2){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	

	st_dataNode * head = *phead;
	st_dataNode * pos = NULL, * pos2 = NULL;
	st_dataNode * prev = NULL, * next = NULL;
	st_dataNode * prev2 = NULL, * next2 = NULL;

	/*将list2合并到list*/
	pos = head;
	pos2 = head2;
	next = pos->next;	
	next2 = pos2->next;

	/*处理list的头结点*/
	if(pos2->data < pos->data){
		/*插入到头部*/
		pos2->next = pos;
		/*更新list头*/
		head = pos2;
		/*更新list2的指针*/
		pos2 = next2;
#ifdef DEBUG
		printf("head process\n");
		dumpList(head);
#endif		
	}

	prev = pos;	
	prev2 = pos2;
	next = pos->next;	
	next2 = pos2->next;
#ifdef DEBUG	
	if(NULL != next){
		printf("prev->data = %d, pos->data = %d, next->data = %d\n", prev->data, pos->data, next->data);
	} else {		
		printf("prev->data = %d, pos->data = %d\n", prev->data, pos->data);
	}
	if(NULL != next2){
		printf("prev2->data = %d, pos2->data = %d, next2->data = %d\n", prev2->data, pos2->data, next2->data);
	} else {		
		printf("prev2->data = %d, pos2->data = %d\n", prev2->data, pos2->data);
	}
#endif	

	while(NULL != pos && NULL != pos2){
#ifdef DEBUG	
		if(NULL != next){
			printf("prev->data = %d, pos->data = %d, next->data = %d\n", prev->data, pos->data, next->data);
		} else {		
			printf("prev->data = %d, pos->data = %d\n", prev->data, pos->data);
		}
		if(NULL != next2){
			printf("prev2->data = %d, pos2->data = %d, next2->data = %d\n", prev2->data, pos2->data, next2->data);
		} else {		
			printf("prev2->data = %d, pos2->data = %d\n", prev2->data, pos2->data);
		}
#endif	

		/*pos2 小于 pos， 插入其中*/
		if(pos2->data < pos->data){
			/*从 list2 上摘掉 pos2 */
			prev2->next = next2;
			
			/* pos2 插入 list */
			prev->next = pos2;
			pos2->next = pos;

			/*更新下一轮变量*/
			prev = pos2;
			// pos 不变，next不变	

			pos2 = next2;
			next2 = pos2->next;
			/*
			 * list2 首节点,   prev 指向新的头 
			 * 非list2 首节点 prev2 不变
			 */
			if(prev2 != pos2){  
				prev2 = pos2;
			} 	
			
#ifdef DEBUG	
			if(NULL != next){
				printf("prev->data = %d, pos->data = %d, next->data = %d\n", prev->data, pos->data, next->data);
			} else {		
				printf("prev->data = %d, pos->data = %d\n", prev->data, pos->data);
			}
			if(NULL != next2){
				printf("prev2->data = %d, pos2->data = %d, next2->data = %d\n", prev2->data, pos2->data, next2->data);
			} else {		
				printf("prev2->data = %d, pos2->data = %d\n", prev2->data, pos2->data);
			}
			dumpList(head);
#endif						
			continue;
		}

		/* pos2 大于 pos，查找下一个pos元素*/
		/*更新下一轮变量*/
		prev = pos;
		pos = next;
		if(NULL != pos){
			next = pos->next;		
		}

#ifdef DEBUG	
		printf("next round \n");
		if(NULL != pos){
			if(NULL != next){
				printf("\t prev->data = %d, pos->data = %d, next->data = %d\n", prev->data, pos->data, next->data);
			} else {		
				printf("\t prev->data = %d, pos->data = %d\n", prev->data, pos->data);
			}
			if(NULL != next2){
				printf("\t prev2->data = %d, pos2->data = %d, next2->data = %d\n", prev2->data, pos2->data, next2->data);
			} else {		
				printf("\t prev2->data = %d, pos2->data = %d\n", prev2->data, pos2->data);
			}
		} else {
			if(NULL != next){
				printf("\t prev->data = %d, pos->data NULL , next->data = %d\n", prev->data, next->data);
			} else {		
				printf("\t prev->data = %d, pos->data  NULL \n", prev->data);
			}
			if(NULL != next2){
				printf("\t prev2->data = %d, pos2->data = %d, next2->data = %d\n", prev2->data, pos2->data, next2->data);
			} else {		
				printf("\t prev2->data = %d, pos2->data = %d\n", prev2->data, pos2->data);
			}
		}

		dumpList(head);
#endif

		
	}

	/* list2 里面还有成员，需要加入到list中 */
	prev->next = prev2;

	/* list 如果没有遍历完毕，不需要处理，因为并入list */
	*phead = head;
	
	return SUCCESS;
}


int JosephusLoop(st_dataNode * head, int * output, int len, int M){
	if(NULL == head || NULL == output || len < 0){
		printf("%s: param error\n",__func__);
		return PARAM_ERR;
	}	


	st_dataNode * suicide = NULL;
	st_dataNode * prev = NULL;
	st_dataNode * next = NULL;
	
	int cnt = 0, k = 0;

	prev = head;
	suicide = head->next;
	next = suicide->next;
	
	while(suicide->next != suicide){
		cnt++;
		/*报数为M，那么自杀*/
		if(0 == cnt % M){
			/*记录到输出*/
			output[k++] = suicide->data;
			/* 自杀 */
			free(suicide);
		
			/*摘掉suicide*/
			prev->next = next;
			suicide = next;
			next = suicide->next;

			/*重新计数*/
			cnt = 0;
			continue;
		}

		prev = suicide;
		suicide = next;
		next = suicide->next;		
	}

	output[k++] = suicide->data;
	free(suicide);
	
	return SUCCESS;
}


void testJosephusLoop(void){

	int len = getListLen(ghead1);
	int i = 0;
	int M = 1768 % len;
	/*存储结果*/
	int output[100] = {0};

	/*形成环*/
	st_dataNode * tail = NULL;
	tail = ghead1;
	while(NULL != tail->next){
		tail = tail->next;
	}
	tail->next = ghead1;

	JosephusLoop(ghead1, output, 1000, M);
	printf(" [ ");
	for(i = 0; i < len; i++){
		printf(" %d ", output[i]);
	}
	printf(" ] \n");

	ghead1 = NULL;
		
	return;
}



void testMergeSortedList(void){		
		/*初始化input数据*/
		int input[10] = {9,6,8,3,0,11,28,119,5,7};
		ghead2 = NULL;	
		ghead2 = createList(ghead2, input, 10);
		insertSortList(&ghead2);

		/*初始化input数据*/
		int input1[10] = {22,32,19,53,1,47,29,116,4,6};
		ghead1 = NULL;	
		ghead1 = createList(ghead1, input1, 10);
		insertSortList(&ghead1);

		dumpList(ghead1);
		dumpList(ghead2);

		printf("************* testMergeSortedList **********************\n");

		MergeSortedList(&ghead1, ghead2);
		ghead2 = NULL; /*被合并掉了*/
		dumpList(ghead1);
	
		return;
}


void testWhetherLopp(void){
	printf("====== %s ===========\n", __func__);
	st_dataNode * tail = NULL;
	bool rst = false;

	rst = whetherListLoop(ghead);
	if(rst){
		printf("\t List Loop\n");
	} else {
		printf("\t Not Loop\n");
	
	}

	tail = ghead;
	while(tail->next != NULL){
		tail = tail->next;
	}

	tail->next = ghead;

	rst = whetherListLoop(ghead);
	if(rst){
		printf("\t List Loop\n");
	} else {
		printf("\t Not Loop\n");	
	}
	tail->next = NULL;
	dumpList(ghead);

	return;
	
}

void testInsertSortList(void){
	printf("====== %s ===========\n", __func__);
	insertSortList(&ghead);
	dumpList(ghead);

	return;
}

void testGetListMidNode2(void){
	st_dataNode * mid = NULL;
	mid = getListMidNode(ghead);
	printf("======== Mid2 Node %p, data: %d==========\n", mid, mid->data);
	return;
}


void testGetListMidNode(void){
	st_dataNode * mid = NULL;
	mid = getListMidNode(ghead);
	printf("======== Mid Node %p, data: %d==========\n", mid, mid->data);
	return;
}

void testReverseList(void){
	reverseList(&ghead);
	dumpList(ghead);

	reverseList(&ghead);
	dumpList(ghead);

	return;
}



void testRemoveNode(void){
	st_dataNode * nw = NULL;
	nw = removeListNode(&ghead, 12);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);
	dumpList(ghead);
	
	nw = removeListNode(&ghead, 5);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);
	dumpList(ghead);
	
	nw = removeListNode(&ghead, 0);
	if(NULL != nw)
		printf("Removed node %p: data %d\n", nw, nw->data);	
	dumpList(ghead);

	return;
}



void testInsertNode(void){
	st_dataNode * nw = NULL;
	nw = insertListNode(&ghead, 0, 70);	
	dumpList(ghead);
	
	insertListNode(&ghead, 5, 31);
	dumpList(ghead);
	
	insertListNode(&ghead, 12, 66);
	dumpList(ghead);

	return;
}



void testFindListPos(void){
	st_dataNode * rst = NULL;
	
	rst = findListPos(ghead, -1);
	if(NULL != rst){
		printf("find pos -1 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos -1 node \n");
	}	

	rst = findListPos(ghead, 0);
	if(NULL != rst){
		printf("find pos 0 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 0 node \n");
	}		

	rst = findListPos(ghead, 5);
	if(NULL != rst){
		printf("find pos 5 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 5 node \n");
	}	

	rst = findListPos(ghead, 12);
	if(NULL != rst){
		printf("find pos 12 node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found pos 12 node \n");
	}	

	return;
	
}

void testSearchList(void){
	st_dataNode * rst = NULL;
	rst = searchListNode(ghead, 6);
	if(NULL != rst){
		printf("node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found num 6 \n");
	}

	rst = searchListNode(ghead, 119);
	if(NULL != rst){
		printf("node： %p, data = %d\n", rst, rst->data);
	} else {
		printf("Can not found num 119 \n");
	}	
}


void testListLen(void){
	int len = 0;
	len = getListLen(ghead);
	printf("List length = %d\n", len);

	return;
}


void testCreateList(void){
	/*初始化input数据*/
	int input[10] = {22,32,19,53,0,47,29,116,4,6};
	ghead = NULL;

	ghead = createList(ghead, input, 10);

#ifdef DEBUG
	dumpList(ghead);
#endif

	return;
}





