#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2

typedef struct dataNode {
	int data; /*这里可扩展为任何数据，暂时简单就用一个 int*/
	struct dataNode * next;
} st_dataNode;

struct dataNode * ghead;
struct dataNode * ghead1;
struct dataNode * ghead2;

st_dataNode * createListNode(int data);
st_dataNode* createList(st_dataNode* head, int * array, int size);
void dumpList(st_dataNode * head);
int getListLen(st_dataNode* head);
st_dataNode* searchListNode(st_dataNode* head, int num);
st_dataNode* findListPos(st_dataNode* head, int pos);
st_dataNode * insertListNode(st_dataNode** phead, int pos, int data);
st_dataNode * removeListNode(st_dataNode** phead, int pos);
int reverseList(st_dataNode** phead);
st_dataNode * getListMidNode(st_dataNode * head);
st_dataNode * getListMidNode2(st_dataNode * head);
int insertSortList(st_dataNode** phead);
bool whetherListLoop(st_dataNode * head);
int MergeSortedList(st_dataNode ** phead, st_dataNode * head2);
int JosephusLoop(st_dataNode * head, int * output, int len, int M);





void testCreateList(void);
void testListLen(void);
void testSearchList(void);
void testFindListPos(void);
void testInsertNode(void);
void testRemoveNode(void);
void testReverseList(void);
void testGetListMidNode(void);
void testGetListMidNode2(void);
void testInsertSortList(void);
void testWhetherLopp(void);
void testMergeSortedList(void);
void testJosephusLoop(void);







#endif

