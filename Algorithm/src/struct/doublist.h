#ifndef __DOUBLIST_H__
#define __DOUBLIST_H__

#include <stdbool.h>

#define SUCCESS		0
#define PARAM_ERR	-1
#define ALLOC_ERR   -2

typedef struct doubNode {
	int data; /*这里可扩展为任何数据，暂时简单就用一个 int*/
	struct doubNode * prev;
	struct doubNode * next;
} st_doubNode;

struct doubNode * gDoubHead;
struct doubNode * gDoubHead1;
struct doubNode * gDoubHead2;

int createDoubList(st_doubNode ** phead, int * array, int size);
void dumpDoubList(st_doubNode * head);
int getDoubListLen(st_doubNode * head);
st_doubNode* searchDoubListNode(st_doubNode* head, int num);
st_doubNode * findDoubListPos(st_doubNode * head, int pos);
st_doubNode * insertDoubListNode(st_doubNode** phead, int pos, int data);
void dumpDoubListReverse(st_doubNode * head);
st_doubNode * removeDoubListNode(st_doubNode** phead, int pos);
st_doubNode * getDouListTail(st_doubNode* head);
int quickSortDoubList(st_doubNode** phead, st_doubNode ** ptail);
int insertDoubLoopSortedListNode(st_doubNode** phead, int data);
int removeDoubLoopListNodeByData(st_doubNode ** phead, int data);
int removeDoubLoopListSameNode(st_doubNode** phead1, st_doubNode** phead2);


void testCreateDoubList(void);
void testgetDoubListLen(void);
void testSearchDoubList(void);
void testFindDoubListPos(void);
void testInsertDoubNode(void);
void testremoveDoubListNode(void);
void testquickSortDoubList(void);
void testinsertDoubLoopSortedListNode(void);
void testremoveDoubLoopListSameNode(void);





#endif


