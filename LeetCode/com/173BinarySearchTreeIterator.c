#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

/*
 * LeetCode 173: 二叉搜索树迭代器
 * 
 * 问题描述：
 * 实现一个二叉搜索树迭代器，实现以下功能：
 * - next(): 返回 BST 中下一个最小的数
 * - hasNext(): 判断是否还有下一个数
 * 
 * 要求：
 * - next() 和 hasNext() 的平均时间复杂度为 O(1)
 * - 空间复杂度为 O(h)，h 为树的高度
 * 
 * 核心思路：
 * - 使用栈模拟中序遍历（左-根-右）
 * - BST 的中序遍历结果是升序序列
 * - 按需遍历：只在调用 next() 时才推进到下一个节点
 */

#define STACK_SIZE 100

/* 栈数据结构：用于存储遍历路径上的节点 */
typedef struct {
	struct TreeNode *nodes[STACK_SIZE];  /* 节点数组 */
	int top;                             /* 栈顶索引（-1表示空栈） */
} Stack;

/* 初始化栈 */
void initStack(Stack *stack) {
	stack->top = -1;
}

/* 入栈：注意先递增再赋值（++top，避免访问[-1]） */
void pushStack(Stack *stack, struct TreeNode *node) {
	stack->nodes[++stack->top] = node;
}

/* 出栈：注意先取值再递减（top--，先访问[top]再减） */
struct TreeNode *popStack(Stack *stack) {
	return stack->nodes[stack->top--];
}

/* 判断栈是否为空 */
bool isStackEmpty(Stack *stack) {
	return stack->top == -1;
}

/* BST迭代器数据结构 */
typedef struct {
	Stack nodeStack;          /* 栈：存储遍历路径上的祖先节点 */
	struct TreeNode *current; /* 当前待访问的节点（指向下一个要处理的子树） */
} BSTIterator;

/*
 * 创建BST迭代器
 * @param root: BST的根节点
 * @return: 迭代器对象
 * 
 * 初始化：
 * - 栈为空
 * - current指向根节点
 */
BSTIterator* bSTIteratorCreate(struct TreeNode* root) {
	BSTIterator *iterator = (BSTIterator *)malloc(sizeof(BSTIterator));
	initStack(&iterator->nodeStack);
	iterator->current = root;
	return iterator;
}

/*
 * 返回BST中下一个最小的数
 * @param obj: 迭代器对象
 * @return: 下一个最小值
 * 
 * 算法步骤（模拟中序遍历）：
 * 1. 将current及其所有左子节点入栈（找到最左节点）
 * 2. 弹出栈顶节点（当前最小值）
 * 3. 将current指向该节点的右子树
 * 4. 返回节点值
 * 
 * 时间复杂度: 平均O(1)，最坏O(h)
 * 空间复杂度: O(h) - 栈空间
 */
int bSTIteratorNext(BSTIterator* obj) {
	struct TreeNode *node;
	int value;

	/* 步骤1: 将current及其所有左子节点入栈 */
	while (obj->current != NULL) { 
		pushStack(&obj->nodeStack, obj->current);
		obj->current = obj->current->left;
	}

	/* 步骤2: 弹出栈顶节点（当前最小值） */
	node = popStack(&obj->nodeStack);
	value = node->val;
	
	/* 步骤3: 移动到该节点的右子树 */
	obj->current = node->right;

	return value;
}

/*
 * 判断是否还有下一个元素
 * @param obj: 迭代器对象
 * @return: true表示还有元素，false表示已遍历完
 * 
 * 逻辑：
 * - current不为空：还有节点未访问
 * - 栈不为空：栈中还有待处理的父节点
 */
bool bSTIteratorHasNext(BSTIterator* obj) {
	return obj->current != NULL || !isStackEmpty(&obj->nodeStack);
}

/*
 * 释放迭代器内存
 * @param obj: 迭代器对象
 */
void bSTIteratorFree(BSTIterator* obj) {
	free(obj);
}
 
/**
 * Your BSTIterator struct will be instantiated and called as such:
 * BSTIterator* obj = bSTIteratorCreate(root);
 * int param_1 = bSTIteratorNext(obj);
 * bool param_2 = bSTIteratorHasNext(obj);
 * bSTIteratorFree(obj);
 */

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：受控中序遍历
 * 
 * 传统中序遍历：一次性遍历完整棵树
 * 迭代器方法：按需遍历，每次调用next()只返回一个元素
 * 
 * BST性质：中序遍历 = 升序序列
 * 
 * 示例BST：
 *     7
 *    / \
 *   3   15
 *      /  \
 *     9   20
 * 
 * 中序遍历顺序：3, 7, 9, 15, 20（升序）
 * 
 * ==================== 详细走查 ====================
 * 
 * === 初始化 ===
 * bSTIteratorCreate(root=7)
 * stack: []
 * current: 7
 * 
 * === 第1次 next() ===
 * 1. while循环：将7及其所有左子节点入栈
 *    push(7) -> stack: [7], current: 3
 *    push(3) -> stack: [7, 3], current: NULL
 * 2. pop() -> node = 3, stack: [7]
 * 3. current = 3->right = NULL
 * 4. 返回 3
 * 
 * 状态：stack: [7], current: NULL
 * 
 * === 第2次 next() ===
 * 1. while循环：current=NULL，不执行
 * 2. pop() -> node = 7, stack: []
 * 3. current = 7->right = 15
 * 4. 返回 7
 * 
 * 状态：stack: [], current: 15
 * 
 * === 第3次 next() ===
 * 1. while循环：将15及其所有左子节点入栈
 *    push(15) -> stack: [15], current: 9
 *    push(9) -> stack: [15, 9], current: NULL
 * 2. pop() -> node = 9, stack: [15]
 * 3. current = 9->right = NULL
 * 4. 返回 9
 * 
 * 状态：stack: [15], current: NULL
 * 
 * === 第4次 next() ===
 * 1. while循环：current=NULL，不执行
 * 2. pop() -> node = 15, stack: []
 * 3. current = 15->right = 20
 * 4. 返回 15
 * 
 * 状态：stack: [], current: 20
 * 
 * === 第5次 next() ===
 * 1. while循环：push(20) -> stack: [20], current: NULL
 * 2. pop() -> node = 20, stack: []
 * 3. current = 20->right = NULL
 * 4. 返回 20
 * 
 * 状态：stack: [], current: NULL
 * 
 * === hasNext() ===
 * current=NULL && stack为空 -> false
 * 
 * ==================== 关键理解 ====================
 * 
 * 为什么是平均O(1)？
 * - 每个节点恰好入栈一次，出栈一次
 * - n个节点，总共2n次操作
 * - 调用n次next()，摊还每次O(1)
 * 
 * 栈中存的是什么？
 * - 从根到当前节点的路径上的祖先节点
 * - 这些祖先节点的左子树已经访问完，等待访问它们自己
 * 
 * current的作用？
 * - 指向下一个要处理的子树的根
 * - 为NULL表示当前子树已访问完
 * 
 * 为什么能保证升序？
 * - BST性质：左 < 根 < 右
 * - 中序遍历：先访问左子树（较小），再访问根，最后访问右子树（较大）
 */