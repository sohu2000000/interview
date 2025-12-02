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

#define MAX_QUEUE_SIZE 10000
#define MAX_TREE_LEVELS 2000

/* 全局队列（用于BFS层序遍历） */
struct TreeNode* bfsQueue[MAX_QUEUE_SIZE];
int queueSize = 0;
int frontIndex = 0;
int rearIndex = 0;

/* 入队操作 */
void enqueueNode(struct TreeNode* node) {
	bfsQueue[rearIndex++] = node;
	queueSize++;
}

/* 出队操作 */
struct TreeNode* dequeueNode() {
	struct TreeNode *node = bfsQueue[frontIndex++];
	queueSize--;
	return node;
}

/* 判断队列是否为空 */
bool isQueueEmpty() {
	return queueSize == 0;
}

/* 获取队列当前大小 */
int getQueueSize() {
	return queueSize;
}

/*
 * LeetCode 102: 二叉树的层序遍历
 * 
 * 问题描述：
 * 给定一个二叉树，返回其节点值的层序遍历（逐层从左到右访问所有节点）
 * 
 * 示例：
 * 输入: [3,9,20,null,null,15,7]
 *     3
 *    / \
 *   9  20
 *     /  \
 *    15   7
 * 输出: [[3], [9,20], [15,7]]
 * 
 * 核心思路：
 * - BFS层序遍历
 * - 使用队列，记录每层的节点数量
 * - 返回二维数组
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
	int i, currentLevelSize, currentLevel;
	struct TreeNode *currentNode;

	/* 分配二维数组（只分配行指针，不分配每行的空间） */
	int** result = (int**)malloc(sizeof(int*) * MAX_TREE_LEVELS);

	*returnSize = 0;

	/* 分配每层的大小数组 */
	*returnColumnSizes = (int *)malloc(sizeof(int) * MAX_TREE_LEVELS);
	for (i = 0; i < MAX_TREE_LEVELS; i++) {
		(*returnColumnSizes)[i] = 0;
	}

	if (root == NULL)
		return result;
	
	/* 重置队列 */
	queueSize = 0;
	frontIndex = 0;
	rearIndex = 0;

	/* 根节点入队 */
	enqueueNode(root);
	currentLevel = 0;

	/* 层序遍历 */
	while (!isQueueEmpty()) {
		/* 步骤1: 记录当前层的节点数量 */
		currentLevelSize = getQueueSize();

		/* 步骤2: 按需分配当前层的空间（只分配需要的大小） */
		result[currentLevel] = (int *)malloc(sizeof(int) * currentLevelSize);

		/* 步骤3: 遍历当前层的所有节点 */
		for (i = 0; i < currentLevelSize; i++) {
			currentNode = dequeueNode();

			/* 步骤4: 记录当前节点的值 */
			result[currentLevel][i] = currentNode->val;
			(*returnColumnSizes)[currentLevel]++;
			
			/* 步骤5: 将子节点入队 */
			if (currentNode->left != NULL)
				enqueueNode(currentNode->left);
			if (currentNode->right != NULL)
				enqueueNode(currentNode->right);
		}

		currentLevel++;
	}

	*returnSize = currentLevel;
	
	return result;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：BFS层序遍历
 * 
 * 层序遍历（Level Order Traversal）：
 * - 按层从上到下遍历
 * - 每层从左到右遍历
 * - 使用队列实现（FIFO）
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入: [3,9,20,null,null,15,7]
 * 
 *     3
 *    / \
 *   9  20
 *     /  \
 *    15   7
 * 
 * === 初始化 ===
 * enqueue(3)
 * queue: [3]
 * queueSize: 1
 * 
 * === 第1层 ===
 * currentLevel = 0
 * currentLevelSize = 1
 * for (i=0; i<1; i++):
 *   i=0: dequeue() -> node=3
 *        result[0][0] = 3
 *        enqueue(9), enqueue(20)
 * queue: [9, 20]
 * result: [[3]]
 * 
 * === 第2层 ===
 * currentLevel = 1
 * currentLevelSize = 2
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=9
 *        result[1][0] = 9
 *   i=1: dequeue() -> node=20
 *        result[1][1] = 20
 *        enqueue(15), enqueue(7)
 * queue: [15, 7]
 * result: [[3], [9,20]]
 * 
 * === 第3层 ===
 * currentLevel = 2
 * currentLevelSize = 2
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=15
 *        result[2][0] = 15
 *   i=1: dequeue() -> node=7
 *        result[2][1] = 7
 * queue: []
 * result: [[3], [9,20], [15,7]] ✓
 * 
 * ==================== 关键要点 ====================
 * 
 * 为什么要记录 currentLevelSize？
 * - 区分不同层的节点
 * - 固定当前层的节点数量
 * - 在处理当前层时，新入队的节点属于下一层
 * 
 * 为什么要重置全局变量？
 * - LeetCode 会多次调用函数
 * - frontIndex 和 rearIndex 会一直递增
 * - 不重置会导致数组越界
 * 
 * 为什么队列大小设为10000？
 * - 题目中树的节点最多约2000个
 * - 队列最大大小约为树的最大宽度
 * - 完全二叉树最后一层可能有n/2个节点
 * - 10000是安全的上限
 */