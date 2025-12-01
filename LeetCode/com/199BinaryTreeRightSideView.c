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
 * LeetCode 199: 二叉树的右视图
 * 
 * 问题描述：
 * 给定一棵二叉树，想象自己站在它的右侧，
 * 按照从顶部到底部的顺序，返回从右侧所能看到的节点值
 * 
 * 示例：
 * 输入: [1,2,3,null,5,null,4]
 *     1            从右边看：
 *    / \           第1层：1
 *   2   3          第2层：3
 *    \   \         第3层：4
 *     5   4        
 * 输出: [1,3,4]
 * 
 * 核心思路：
 * - 层序遍历（BFS）
 * - 记录每层的最后一个节点（最右边的节点）
 * - 使用队列实现层序遍历
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#define MAX_QUEUE_SIZE 1000

/* 全局队列（用于BFS层序遍历） */
struct TreeNode *bfsQueue[MAX_QUEUE_SIZE];
int queueSize = 0;    /* 当前队列中的节点数量 */
int frontIndex = 0;   /* 队首索引 */
int rearIndex = 0;    /* 队尾索引 */

/* 入队操作 */
void enqueueNode(struct TreeNode* node) {
	bfsQueue[rearIndex++] = node;
	queueSize++;
}

/* 出队操作 */
struct TreeNode* dequeueNode() {
	struct TreeNode* node = bfsQueue[frontIndex++];
	queueSize--;
	return node;
}

/* 判断队列是否为空 */
bool isQueueEmpty() {
	return queueSize == 0;
}

/*
 * BFS层序遍历，记录每层的最右节点
 * @param root: 二叉树的根节点
 * @param result: 结果数组
 * @param returnSize: 结果数组的大小
 * 
 * 算法步骤：
 * 1. 根节点入队
 * 2. while队列不为空：
 *    a) 记录当前层的节点数量
 *    b) 遍历当前层的所有节点
 *    c) 记录最后一个节点的值（最右节点）
 *    d) 将子节点入队（下一层）
 * 
 * 时间复杂度: O(n) - 访问每个节点一次
 * 空间复杂度: O(w) - w是树的最大宽度
 */
void bfsLevelOrder(struct TreeNode* root, int* result, int* returnSize) {
	int currentLevelSize, i;
	struct TreeNode* currentNode;
	
	if (root == NULL)
		return;
	
	/* 重置队列（因为使用了全局变量，LeetCode可能多次调用） */
	queueSize = 0;
	frontIndex = 0;
	rearIndex = 0;
	
	/* 根节点入队 */
	enqueueNode(root);
	
	/* 层序遍历 */
	while (!isQueueEmpty()) {
		/* 步骤1: 记录当前层的节点数量（关键！） */
		currentLevelSize = queueSize;
		
		/* 步骤2: 遍历当前层的所有节点 */
		for (i = 0; i < currentLevelSize; i++) {
			currentNode = dequeueNode();
			
			/* 步骤3: 如果是当前层的最后一个节点（最右边），记录它的值 */
			if (i == currentLevelSize - 1) {
				result[(*returnSize)++] = currentNode->val;
			}
			
			/* 步骤4: 将下一层的节点入队（先左后右） */
			if (currentNode->left != NULL)
				enqueueNode(currentNode->left);
			if (currentNode->right != NULL)
				enqueueNode(currentNode->right);
		}
	}
}

/*
 * 主函数：返回二叉树的右视图
 * @param root: 二叉树的根节点
 * @param returnSize: 返回数组的大小（输出参数）
 * @return: 从右侧看到的节点值数组
 * 
 * 示例：
 * 输入: [1,2,3,null,5,null,4]
 *     1
 *    / \
 *   2   3
 *    \   \
 *     5   4
 * 输出: [1, 3, 4]
 */
int* rightSideView(struct TreeNode* root, int* returnSize) {

	int* result = (int*)malloc(sizeof(int) * MAX_QUEUE_SIZE);
	if (result == NULL)
		return NULL;

	*returnSize = 0;
	bfsLevelOrder(root, result, returnSize);
	return result;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：层序遍历 + 记录每层最右节点
 * 
 * 层序遍历（BFS）的关键：
 * - 使用队列存储待访问的节点
 * - 按层处理：先处理完当前层，再处理下一层
 * - 关键技巧：记录当前层的节点数量
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入: [1,2,3,null,5,null,4]
 * 
 *     1
 *    / \
 *   2   3
 *    \   \
 *     5   4
 * 
 * === 初始化 ===
 * enqueue(1)
 * queue: [1]
 * queueSize: 1
 * 
 * === 第1层 ===
 * currentLevelSize = 1
 * for (i=0; i<1; i++):
 *   i=0: dequeue() -> node=1
 *        i==0 (最后一个), 记录 1
 *        enqueue(2), enqueue(3)
 * queue: [2, 3]
 * queueSize: 2
 * result: [1]
 * 
 * === 第2层 ===
 * currentLevelSize = 2
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=2
 *        enqueue(5)
 *   i=1: dequeue() -> node=3
 *        i==1 (最后一个), 记录 3
 *        enqueue(4)
 * queue: [5, 4]
 * queueSize: 2
 * result: [1, 3]
 * 
 * === 第3层 ===
 * currentLevelSize = 2
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=5
 *   i=1: dequeue() -> node=4
 *        i==1 (最后一个), 记录 4
 * queue: []
 * queueSize: 0
 * result: [1, 3, 4] ✓
 * 
 * ==================== 关键要点 ====================
 * 
 * 为什么要记录 currentLevelSize？
 * - 层序遍历需要区分每一层
 * - currentLevelSize 固定了当前层有多少个节点
 * - 遍历 currentLevelSize 次后，队列中剩下的都是下一层
 * 
 * 如何识别每层的最右节点？
 * - 当前层的最后一个节点（i == currentLevelSize - 1）
 * - 因为是先左后右入队，所以最后出队的是最右节点
 * 
 * 为什么要重置全局变量？
 * - LeetCode 可能多次调用函数
 * - front 和 rear 会一直递增
 * - 不重置会导致数组越界
 */