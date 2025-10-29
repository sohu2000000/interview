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

/* 全局链表头指针（用于测试） */
struct doubNode * gDoubHead;
struct doubNode * gDoubHead1;
struct doubNode * gDoubHead2;

/* ===== 双向链表基本操作函数（已优化） ===== */

/* 创建和显示 */
int create_doubly_linked_list(st_doubNode **list_head_ptr, int *source_array, int array_size);
void display_doubly_linked_list_forward(st_doubNode *list_head);
void display_doubly_linked_list_backward(st_doubNode *list_head);

/* 查询操作 */
int get_doubly_linked_list_length(st_doubNode *list_head);
st_doubNode *search_doubly_list_node_by_value(st_doubNode *list_head, int target_value);
st_doubNode *find_doubly_list_node_at_position(st_doubNode *list_head, int target_position);
st_doubNode *get_doubly_list_tail_node(st_doubNode *list_head);

/* 插入和删除 */
st_doubNode *insert_doubly_list_node_at_position(st_doubNode **list_head_ptr, int insert_position, int node_data);
st_doubNode *remove_doubly_list_node_at_position(st_doubNode **list_head_ptr, int remove_position);

/* 兼容性宏：保持旧接口可用 */
#define createDoubList           create_doubly_linked_list
#define dumpDoubList             display_doubly_linked_list_forward
#define dumpDoubListReverse      display_doubly_linked_list_backward
#define getDoubListLen           get_doubly_linked_list_length
#define searchDoubListNode       search_doubly_list_node_by_value
#define findDoubListPos          find_doubly_list_node_at_position
#define insertDoubListNode       insert_doubly_list_node_at_position
#define removeDoubListNode       remove_doubly_list_node_at_position
#define getDouListTail           get_doubly_list_tail_node

/* 待优化的函数（暂保持原名称） */
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
