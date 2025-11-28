#include <stdio.h>
#include <stdlib.h>

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
 * LeetCode 129: 求根节点到叶节点数字之和
 * 
 * 问题描述：
 * 给定一个二叉树，每个节点都包含数字 0-9
 * 从根节点到叶子节点的每条路径代表一个数字
 * 计算所有路径表示的数字之和
 * 
 * 示例：
 *     1              路径: 1->2->5 => 125
 *    / \                   1->3   => 13
 *   2   3            总和: 125 + 13 = 138
 * 
 * 核心思路：
 * - DFS前序遍历，累积路径数字
 * - 从根到当前节点的数字 = 父节点数字 * 10 + 当前节点值
 * - 到达叶子节点时，累加到总和
 */

/*
 * DFS递归遍历，累积路径数字
 * @param root: 当前节点
 * @param pathNumber: 从根到当前节点父节点的路径数字
 * @param totalSum: 指向总和的指针
 * 
 * 算法步骤：
 * 1. 计算当前路径数字：pathNumber * 10 + root->val
 * 2. 如果是叶子节点，累加到总和
 * 3. 否则，递归遍历左右子树
 * 
 * 时间复杂度: O(n) - 访问每个节点一次
 * 空间复杂度: O(h) - 递归栈深度，h为树的高度
 */
void dfsPathSum(struct TreeNode* root, int pathNumber, int* totalSum) {

	int currentNumber;

	/* 递归终止条件 */
	if (root == NULL) return;

	/* 步骤1: 计算从根到当前节点的路径数字 */
	currentNumber = pathNumber * 10 + root->val;

	/* 步骤2: 如果是叶子节点，累加到总和 */
	if (root->left == NULL && root->right == NULL) {
		*totalSum += currentNumber;
		return;
	}

	/* 步骤3: 递归遍历左右子树 */
	dfsPathSum(root->left, currentNumber, totalSum);
	dfsPathSum(root->right, currentNumber, totalSum);

	return;
}

/*
 * 主函数：求根节点到叶节点数字之和
 * @param root: 二叉树的根节点
 * @return: 所有路径数字之和
 * 
 * 示例：
 * 输入: [1,2,3]
 *       1
 *      / \
 *     2   3
 * 输出: 25
 * 解释: 路径 1->2 表示 12
 *       路径 1->3 表示 13
 *       总和 = 12 + 13 = 25
 */
int sumNumbers(struct TreeNode* root) {

	int totalSum = 0;

	/* DFS遍历，初始路径数字为0 */
	dfsPathSum(root, 0, &totalSum);

	return totalSum;
}

/*
 * 详细走查示例：
 * 
 * 输入树：
 *     4
 *    / \
 *   9   0
 *  / \
 * 5   1
 * 
 * === DFS遍历过程 ===
 * 
 * 访问节点4: pathNumber=0, currentNumber=0*10+4=4
 *   访问节点9: pathNumber=4, currentNumber=4*10+9=49
 *     访问节点5: pathNumber=49, currentNumber=49*10+5=495
 *       叶子节点，totalSum += 495 (totalSum=495)
 *     访问节点1: pathNumber=49, currentNumber=49*10+1=491
 *       叶子节点，totalSum += 491 (totalSum=986)
 *   访问节点0: pathNumber=4, currentNumber=4*10+0=40
 *     叶子节点，totalSum += 40 (totalSum=1026)
 * 
 * 最终结果：495 + 491 + 40 = 1026
 */