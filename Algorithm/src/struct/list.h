#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

#define SUCCESS       0
#define PARAM_ERROR  -1
#define MEMORY_ERROR -2

/**
 * 链表节点结构体
 */
typedef struct list_node {
    int data;                    // 节点数据
    struct list_node *next;      // 指向下一个节点的指针
} ListNode;

// 全局链表头指针（用于测试）
ListNode *global_head;
ListNode *global_head1;
ListNode *global_head2;

// 基本链表操作
ListNode *create_node(int data);
ListNode *create_list_from_array(int *array, int size);
void print_list(ListNode *head);
int get_list_length(ListNode *head);
void free_list(ListNode **head);

// 查找和定位
ListNode *search_node_by_value(ListNode *head, int value);
ListNode *find_node_by_position(ListNode *head, int position);
ListNode *find_middle_node(ListNode *head);
ListNode *find_middle_node_fast(ListNode *head);

// 插入和删除
ListNode *insert_node(ListNode **head, int position, int data);
ListNode *remove_node(ListNode **head, int position);

// 链表操作
int reverse_list(ListNode **head);
int sort_list_insertion(ListNode **head);
bool is_list_circular(ListNode *head);
int merge_sorted_lists(ListNode **head1, ListNode *head2);
int josephus_problem(ListNode *head, int *result, int length, int step);

// 测试函数
void test_create_list(void);
void test_list_length(void);
void test_search_node(void);
void test_find_position(void);
void test_insert_node(void);
void test_remove_node(void);
void test_reverse_list(void);
void test_find_middle_node(void);
void test_find_middle_node_fast(void);
void test_insertion_sort(void);
void test_circular_detection(void);
void test_merge_sorted_lists(void);
void test_josephus_problem(void);

#endif

