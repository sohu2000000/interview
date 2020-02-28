
#include <stdio.h>
#include "list.h"
#include "doublist.h"
#include "queue.h"
#include "tree.h"

int main(int argc, char ** argv){

#if 0
	/* 单链表 */
	testCreateList();
	testListLen();
	testSearchList();
	testFindListPos();
	testInsertNode();
	testRemoveNode();
	testReverseList();
	testGetListMidNode();
	testGetListMidNode2();
	testInsertSortList();
	testWhetherLopp();
	testMergeSortedList();
	testJosephusLoop();
#endif

#if 0
	/*双向链表*/
	testCreateDoubList();
	testgetDoubListLen();
	testSearchDoubList();
	testFindDoubListPos();
	testInsertDoubNode();
	testremoveDoubListNode();
	testquickSortDoubList();
	testinsertDoubLoopSortedListNode();
	testremoveDoubLoopListSameNode();
#endif

#if 0
	/* 队列 */
	testQueue();
#endif	


#if 0
	/*栈操作*/
	testStack();
#endif

#if 1
	/*二叉排序树*/
	testBSTree();
#endif	

	return SUCCESS;
}

