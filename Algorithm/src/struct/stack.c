#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "stack.h"

/**
 * 显示栈内容 - 打印栈中所有元素（从栈底到栈顶）
 * @param stack_ptr: 栈指针
 * @return: 成功返回SUCCESS
 * 
 * 功能：遍历并打印栈的所有元素
 */
int display_stack_contents(st_stack *stack_ptr) {
	if (stack_ptr == NULL) {
		return SUCCESS;
	}

	ListNode *current_node = NULL;

	printf("========= 栈内容 %p ===========\n", stack_ptr);
	printf("\t 栈底(bottom) = %d \n", stack_ptr->bottom->data);
	printf("\t 栈顶(top)    = %d \n\t", stack_ptr->top->data);

	current_node = stack_ptr->bottom;

	while (current_node != NULL) {
		printf(" %d ", current_node->data);
		current_node = current_node->next;
	}

	printf("\n");
	printf("===================================\n");

	return SUCCESS;
}

/**
 * 创建栈 - 分配并初始化一个新的栈
 * @return: 成功返回栈指针，失败返回NULL
 * 
 * 功能：创建空栈，栈顶和栈底指针都为NULL
 */
st_stack *create_new_stack(void) {
	st_stack *new_stack = NULL;
	
	new_stack = (st_stack *)malloc(sizeof(st_stack));
	if (new_stack == NULL) {
		printf("Error: create_new_stack malloc failed\n");
		return NULL;
	}

	new_stack->top = NULL;
	new_stack->bottom = NULL;

	return new_stack;
}

/**
 * 入栈操作（压栈）- 将元素压入栈顶
 * @param stack_ptr: 栈指针
 * @param element_data: 要压栈的数据
 * @return: 成功返回SUCCESS，失败返回错误码
 * 
 * 算法：尾插法（栈顶在链表尾部）
 * - 第一个元素：top和bottom都指向它
 * - 后续元素：追加到top后面，更新top
 */
int push_element_to_stack(st_stack *stack_ptr, int element_data) {
	if (stack_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return PARAM_ERROR;
	}

	ListNode *new_node = create_node(element_data);
	if (new_node == NULL) {
		printf("Error: %s create_node failed\n", __func__);
		return MEMORY_ERROR;
	}

	if (stack_ptr->bottom == NULL) {
		// 栈为空，第一个元素
		stack_ptr->top = new_node;
		stack_ptr->bottom = new_node;
	} else {
		// 栈非空，追加到栈顶
		stack_ptr->top->next = new_node;
		stack_ptr->top = new_node;
	}

	return SUCCESS;
}


/**
 * 出栈操作（弹栈）- 从栈顶移除并返回元素
 * @param stack_ptr: 栈指针
 * @return: 返回出栈的节点指针（需调用者释放），栈空返回NULL
 * 
 * 算法：从尾部删除（栈顶在链表尾部）
 * - 需要找到栈顶的前一个节点
 * - 更新栈顶指针
 * - 如果栈变空，同时更新栈底指针
 */
ListNode *pop_element_from_stack(st_stack *stack_ptr) {
	if (stack_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return NULL;
	}

	ListNode *popped_node = NULL;
	ListNode *iterator = NULL;

	// 栈为空
	if (stack_ptr->bottom == NULL) {
		return NULL;
	}

	// 只有一个元素的情况
	if (stack_ptr->bottom == stack_ptr->top) {
		popped_node = stack_ptr->top;
		stack_ptr->bottom = NULL;
		stack_ptr->top = NULL;
		return popped_node;
	}
	
	// 找到栈顶的前一个节点
	iterator = stack_ptr->bottom;
	while (iterator->next != stack_ptr->top) {
		iterator = iterator->next;
	}

	// 取出栈顶节点
	popped_node = stack_ptr->top;

	// 更新栈顶指针
	iterator->next = NULL;
	stack_ptr->top = iterator;

	return popped_node;
}

/**
 * 获取栈长度 - 计算栈中元素的个数
 * @param stack_ptr: 栈指针
 * @return: 栈长度，错误返回PARAM_ERROR
 */
int get_stack_length(st_stack *stack_ptr) {
	if (stack_ptr == NULL) {
		printf("Error: %s parameter error\n", __func__);
		return PARAM_ERROR;
	}
	
	int element_count = 0;

	element_count = get_list_length(stack_ptr->bottom);

	return element_count;
}

/**
 * 检查栈是否为空
 * @param stack_ptr: 栈指针
 * @return: 空返回true，非空返回false
 */
bool is_stack_empty(st_stack *stack_ptr) {
	if (stack_ptr == NULL) {
		return true;
	}

	return (stack_ptr->bottom == NULL && stack_ptr->top == NULL);
}


void testStack(void){
	ListNode *popped_node = NULL;
	int stack_length = 0;
	bool is_empty = false;
	
	gstack = createStack();
	is_empty = isStackEmpty(gstack);
	if(is_empty){
		printf("栈为空\n");
	} else {
		printf("栈不为空\n");
	}

	pushStack(gstack, 22);
	pushStack(gstack, 32);
	pushStack(gstack, 19);
	pushStack(gstack, 53);
	pushStack(gstack, 0);
	pushStack(gstack, 47);
	pushStack(gstack, 29);
	stack_length = getStackLength(gstack);
	printf("栈长度 = %d\n", stack_length);	
	dumpStack(gstack);

	popped_node = popStack(gstack);
	if(popped_node != NULL)
		printf("出栈元素 = %d\n", popped_node->data);
	free(popped_node);
	
	popped_node = popStack(gstack);
	if(popped_node != NULL)
		printf("出栈元素 = %d\n", popped_node->data);
	free(popped_node);
	
	popped_node = popStack(gstack);
	if(popped_node != NULL)
		printf("出栈元素 = %d\n", popped_node->data);
	free(popped_node);
	
	stack_length = getStackLength(gstack);
	printf("栈长度 = %d\n", stack_length);	
	dumpStack(gstack);	

	is_empty = isStackEmpty(gstack);
	if(is_empty){
		printf("栈为空\n");
	} else {
		printf("栈不为空\n");
	}

	popped_node = popStack(gstack);
	free(popped_node);
	popped_node = popStack(gstack);
	free(popped_node);
	popped_node = popStack(gstack);
	free(popped_node);
	printf("继续出栈...\n");
	popped_node = popStack(gstack);
	free(popped_node);	
	printf("出栈完成\n");
	
	is_empty = isStackEmpty(gstack);
	if(is_empty){
		printf("栈为空\n");
	} else {
		printf("栈不为空\n");
	}	

	return;
}
