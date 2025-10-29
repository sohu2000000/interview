#ifndef __STACK_H__
#define __STACK_H__

#include "list.h"

typedef struct stack{
	ListNode *top;     // 栈顶指针
	ListNode *bottom;  // 栈底指针
} st_stack;

/* 全局栈指针（用于测试） */
st_stack * gstack;

/* ===== 栈操作函数（已优化） ===== */

/* 栈基本操作 */
st_stack *create_new_stack(void);
int push_element_to_stack(st_stack *stack_ptr, int element_data);
ListNode *pop_element_from_stack(st_stack *stack_ptr);
int get_stack_length(st_stack *stack_ptr);
bool is_stack_empty(st_stack *stack_ptr);
int display_stack_contents(st_stack *stack_ptr);

/* 兼容性宏：保持旧接口可用 */
#define createStack    create_new_stack
#define pushStack      push_element_to_stack
#define popStack       pop_element_from_stack
#define getStackLength get_stack_length
#define isStackEmpty   is_stack_empty
#define dumpStack      display_stack_contents

/* 测试函数 */
void testStack(void);

#endif


