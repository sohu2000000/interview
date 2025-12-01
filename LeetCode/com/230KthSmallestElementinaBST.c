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
 * LeetCode 230: 二叉搜索树中第K小的元素
 * 
 * 问题描述：
 * 给定一个二叉搜索树的根节点 root 和整数 k，
 * 返回该树中第 k 小的元素（k 从 1 开始计数）
 * 
 * 示例：
 * 输入: root = [3,1,4,null,2], k = 1
 *     3
 *    / \
 *   1   4
 *    \
 *     2
 * 输出: 1
 * 
 * 说明：中序遍历BST得到升序序列：1, 2, 3, 4
 *       第1小的元素是 1
 * 
 * 核心思路：
 * - BST的中序遍历 = 升序序列
 * - 中序遍历到第k个节点就是答案
 * - 使用全局计数器记录访问的节点数
 */

/* 全局计数器：记录已访问的节点数量 */
int visitedCount = 0;

/*
 * 中序遍历查找第k小的元素
 * @param root: 当前节点
 * @param k: 第k小（从1开始）
 * @param kthValue: 存储结果的指针
 * @return: true表示已找到，false表示未找到
 * 
 * 中序遍历顺序：左 -> 根 -> 右（升序）
 * 
 * 算法步骤：
 * 1. 递归遍历左子树
 * 2. 访问当前节点（计数并判断）
 * 3. 递归遍历右子树
 * 4. 找到后提前返回（剪枝优化）
 * 
 * 关键：先计数，后判断
 * - visitedCount++ 表示"当前是第几个节点"
 * - 然后检查是否等于k
 */
bool inorderTraversal(struct TreeNode* root, int k, int *kthValue) {

	if (!root)
		return false;
	
	/* 步骤1: 递归遍历左子树 */
	if (inorderTraversal(root->left, k, kthValue))
		return true;
	
	/* 步骤2: 访问当前节点 */
	visitedCount++;  /* 关键：先计数（表示这是第几个节点） */
	if (visitedCount == k) {  /* 然后判断是否是第k个 */
		*kthValue = root->val;
		return true;  /* 找到后立即返回（剪枝） */
	}

	/* 步骤3: 递归遍历右子树 */
	if (inorderTraversal(root->right, k, kthValue))
		return true;

	return false;
}

/*
 * 主函数：查找BST中第k小的元素
 * @param root: BST的根节点
 * @param k: 第k小（从1开始）
 * @return: 第k小的元素值
 * 
 * 时间复杂度: O(h+k) - 平均O(log n + k)，最坏O(n)
 * 空间复杂度: O(h) - 递归栈深度
 * 
 * 示例：
 * BST: [5,3,6,2,4,null,null,1]
 *       5
 *      / \
 *     3   6
 *    / \
 *   2   4
 *  /
 * 1
 * 
 * 中序遍历：1, 2, 3, 4, 5, 6
 * k=1 -> 1
 * k=3 -> 3
 * k=5 -> 5
 */
int kthSmallest(struct TreeNode* root, int k) {
	int kthValue = 0;
	
	/* 重置全局计数器（LeetCode可能多次调用） */
	visitedCount = 0;

	/* 中序遍历查找第k小元素 */
	inorderTraversal(root, k, &kthValue);
	
	return kthValue;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心理解：BST中序遍历 = 升序序列
 * 
 * 为什么用中序遍历？
 * - BST性质：左 < 根 < 右
 * - 中序遍历：先访问左（小的），再访问根，最后访问右（大的）
 * - 结果：按从小到大的顺序访问节点
 * 
 * 为什么k从1开始？
 * - 题目定义：第1小、第2小、第3小...
 * - 不是数组索引（从0开始）
 * - 所以 visitedCount 也要从1开始计数
 * 
 * 为什么先计数再判断？
 * - visitedCount++ 表示"当前节点是第几个"
 * - visitedCount=1 表示第1个节点
 * - 然后检查 visitedCount == k
 * 
 * 如果先判断再计数会怎样？
 * - 第1个节点：g_count=0，检查0==k失败，然后g_count++变成1
 * - 第2个节点：g_count=1，检查1==k...
 * - 错位！实际返回的是第(k+1)小的元素
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入: root = [3,1,4,null,2], k = 1
 * 
 *     3
 *    / \
 *   1   4
 *    \
 *     2
 * 
 * === 中序遍历过程 ===
 * 
 * 1. inorderTraversal(3, k=1)
 *    - 递归左子树 inorderTraversal(1)
 *    
 * 2. inorderTraversal(1, k=1)
 *    - 递归左子树 inorderTraversal(NULL) -> 返回false
 *    - 访问节点1：visitedCount++ = 1
 *    - 检查：1 == 1 (true)
 *    - *kthValue = 1
 *    - 返回 true ✓
 *    
 * 3. inorderTraversal(3) 收到 true
 *    - 立即返回 true（剪枝，不再访问右子树）
 * 
 * 最终结果：1 ✓
 * 
 * === 如果 k=2 ===
 * 
 * 1. 访问节点1：visitedCount=1, 1==2(false)
 * 2. 访问节点2：visitedCount=2, 2==2(true)
 *    - *kthValue = 2
 *    - 返回 true ✓
 * 
 * === 如果 k=3 ===
 * 
 * 1. 访问节点1：visitedCount=1
 * 2. 访问节点2：visitedCount=2
 * 3. 访问节点3：visitedCount=3, 3==3(true)
 *    - *kthValue = 3
 *    - 返回 true ✓
 */
