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
 * LeetCode 103: 二叉树的锯齿形层序遍历
 * 
 * 问题描述：
 * 给定一个二叉树，返回其节点值的锯齿形层序遍历
 * - 第1层：从左到右
 * - 第2层：从右到左
 * - 第3层：从左到右
 * - 依此类推...
 * 
 * 示例：
 * 输入: [3,9,20,null,null,15,7]
 *     3
 *    / \
 *   9  20
 *     /  \
 *    15   7
 * 输出: [[3], [20,9], [15,7]]
 * 
 * 核心思路：
 * - BFS层序遍历 + 方向标记
 * - 奇数层：从左到右
 * - 偶数层：从右到左（逆序存储）
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

#define MAX_QUEUE_SIZE 10000
#define MAX_TREE_LEVELS 2000

/* 全局队列（用于BFS层序遍历） */
struct TreeNode* bfsQueue[MAX_QUEUE_SIZE];
int queueSize = 0;
int frontIndex = 0;
int rearIndex = 0;

/* 遍历方向枚举 */
typedef enum {
	LEFT_TO_RIGHT = 0,  /* 从左到右 */
	RIGHT_TO_LEFT = 1   /* 从右到左 */
} Direction;

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
 * 主函数：锯齿形层序遍历
 * @param root: 二叉树的根节点
 * @param returnSize: 返回的层数
 * @param returnColumnSizes: 每层的节点数量
 * @return: 锯齿形遍历结果（二维数组）
 * 
 * 时间复杂度: O(n) - 访问每个节点一次
 * 空间复杂度: O(w) - w是树的最大宽度
 */
int** zigzagLevelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {

	int i, currentLevelSize, currentLevel;
	struct TreeNode *currentNode;
	Direction currentDirection;

	/* 分配二维数组（只分配行指针） */
	int** result = (int**)malloc(sizeof(int*) * MAX_TREE_LEVELS);
	
	/* 分配每层的大小数组 */
	*returnColumnSizes = (int *)malloc(sizeof(int) * MAX_TREE_LEVELS);
	for (i = 0; i < MAX_TREE_LEVELS; i++) {
		(*returnColumnSizes)[i] = 0;
	}
	*returnSize = 0;

	if (root == NULL)
		return result;

	/* 重置队列 */
	queueSize = 0;
	frontIndex = 0;
	rearIndex = 0;

	/* 根节点入队 */
	enqueueNode(root);
	currentLevel = 0;
	currentDirection = LEFT_TO_RIGHT;  /* 第1层从左到右 */

	/* 层序遍历 */
	while (!isQueueEmpty()) {
		/* 步骤1: 记录当前层的节点数量 */
		currentLevelSize = getQueueSize();
		
		/* 步骤2: 按需分配当前层的空间 */
		result[currentLevel] = (int *)malloc(sizeof(int) * currentLevelSize);
		(*returnColumnSizes)[currentLevel] = currentLevelSize;

		/* 步骤3: 遍历当前层的所有节点 */
		for (i = 0; i < currentLevelSize; i++) {
			currentNode = dequeueNode();
			
			/* 步骤4: 根据方向存储节点值 */
			if (currentDirection == LEFT_TO_RIGHT) {
				/* 从左到右：正常顺序 */
				result[currentLevel][i] = currentNode->val;
			} else {
				/* 从右到左：逆序存储 */
				result[currentLevel][currentLevelSize - 1 - i] = currentNode->val;
			}

			/* 步骤5: 将子节点入队（始终先左后右） */
			if (currentNode->left != NULL) {
				enqueueNode(currentNode->left);
			}
			if (currentNode->right != NULL) {
				enqueueNode(currentNode->right);
			}
		}

		/* 步骤6: 进入下一层，切换方向 */
		currentLevel++;
		currentDirection = (currentDirection == LEFT_TO_RIGHT) ? RIGHT_TO_LEFT : LEFT_TO_RIGHT;
	}

	*returnSize = currentLevel;
	return result;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：BFS + 方向标记 + 逆序存储
 * 
 * 锯齿形遍历：
 * - 第1层（奇数层）：从左到右
 * - 第2层（偶数层）：从右到左
 * - 第3层（奇数层）：从左到右
 * - ...依此类推
 * 
 * 实现方法：
 * - 使用普通BFS（节点始终从左到右入队）
 * - 根据当前层的方向，决定存储顺序
 * - 从右到左时：逆序存储（result[currentLevel][currentLevelSize-1-i]）
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
 * currentDirection: LEFT_TO_RIGHT
 * 
 * === 第1层（从左到右）===
 * currentLevel = 0
 * currentLevelSize = 1
 * currentDirection = LEFT_TO_RIGHT
 * 
 * for (i=0; i<1; i++):
 *   i=0: dequeue() -> node=3
 *        result[0][0] = 3 (正常顺序)
 *        enqueue(9), enqueue(20)
 * 
 * queue: [9, 20]
 * result: [[3]]
 * currentDirection 切换为 RIGHT_TO_LEFT
 * 
 * === 第2层（从右到左）===
 * currentLevel = 1
 * currentLevelSize = 2
 * currentDirection = RIGHT_TO_LEFT
 * 
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=9
 *        result[1][2-1-0] = result[1][1] = 9 (逆序)
 *   i=1: dequeue() -> node=20
 *        result[1][2-1-1] = result[1][0] = 20 (逆序)
 *        enqueue(15), enqueue(7)
 * 
 * queue: [15, 7]
 * result: [[3], [20,9]]  (从右到左！)
 * currentDirection 切换为 LEFT_TO_RIGHT
 * 
 * === 第3层（从左到右）===
 * currentLevel = 2
 * currentLevelSize = 2
 * currentDirection = LEFT_TO_RIGHT
 * 
 * for (i=0; i<2; i++):
 *   i=0: dequeue() -> node=15
 *        result[2][0] = 15 (正常顺序)
 *   i=1: dequeue() -> node=7
 *        result[2][1] = 7 (正常顺序)
 * 
 * queue: []
 * result: [[3], [20,9], [15,7]] ✓
 * 
 * ==================== 关键要点 ====================
 * 
 * 逆序存储的技巧：
 * - 正常顺序：result[level][i]
 * - 逆序存储：result[level][size-1-i]
 * 
 * 为什么不改变入队顺序？
 * - 节点始终按左->右入队（保持树结构）
 * - 只在存储时改变顺序（更简单）
 * 
 * 方向切换：
 * - 每层结束后切换方向
 * - 使用三元运算符切换
 */

