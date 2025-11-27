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
 * LeetCode 114: 二叉树展开为链表
 * 
 * 问题描述：
 * 给定一个二叉树，原地将它展开为"链表"（使用 right 指针连接）
 * 展开后的顺序应该是原二叉树的前序遍历顺序
 * 
 * 示例：
 *     1              1
 *    / \              \
 *   2   5      =>      2
 *  / \   \              \
 * 3   4   6              3
 *                         \
 *                          4
 *                           \
 *                            5
 *                             \
 *                              6
 */

/* ==================== 方法1：递归法 ==================== */

/*
 * 递归前序遍历，边遍历边连接
 * @param root: 当前节点
 * @param prevNode: 指向前一个节点的指针的指针
 * 
 * 时间复杂度: O(n)
 * 空间复杂度: O(h) - 递归栈
 */
void flattenRecursive(struct TreeNode *root, struct TreeNode **prevNode) {

	struct TreeNode *leftChild, *rightChild;

	if (root == NULL) return;
	
	/* 关键：先保存左右子树指针，避免在修改结构后丢失 */
	leftChild = root->left;
	rightChild = root->right;

	/* 将当前节点连接到前一个节点的右边 */
	(*prevNode)->right = root;
	(*prevNode)->left = NULL;  /* 展开为链表，左子树置空 */
	(*prevNode) = root;

	/* 递归处理左子树和右子树（使用保存的指针） */
	flattenRecursive(leftChild, prevNode);
	flattenRecursive(rightChild, prevNode);
	return;

}

/*
 * 递归法主入口（辅助函数）
 * 使用哑节点简化边界处理
 */
void flattenByRecursion(struct TreeNode* root) {

	struct TreeNode *prevNode = NULL;
	struct TreeNode *dummy = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	
	if (root == NULL) {
		free(dummy);
		return;
	}

	dummy->left = NULL;
	dummy->right = NULL;
	prevNode = dummy;

	flattenRecursive(root, &prevNode);
	free(dummy);

	return;
}


/* ==================== 方法2：Morris 遍历法（O(1)空间）==================== */

/*
 * Morris 遍历法展开二叉树
 * 
 * 核心思想：
 * 利用树中的空闲指针（右子树的最右节点的 right 指针）来保存信息
 * 避免使用递归栈或额外的数据结构
 * 
 * 算法步骤：
 * 对于每个节点 current：
 * 1. 如果 current 没有左子树，直接移动到右子树
 * 2. 如果 current 有左子树：
 *    a) 找到左子树的最右节点（predecessor）
 *    b) 将 current 的右子树接到 predecessor 的右边
 *    c) 将左子树移动到右边，左指针置空
 *    d) 继续处理（新的）右子树
 * 
 * 图解示例：处理节点1
 * 
 * 步骤1（原始）：        步骤2（找到predecessor=4，接上右子树）：
 *     1                      1
 *    / \                    / \
 *   2   5                  2   5
 *  / \   \                / \   \
 * 3   4   6              3   4   6
 *                             \
 *                              5
 *                               \
 *                                6
 * 
 * 步骤3（左子树移到右边，左指针置空）：
 *     1
 *      \
 *       2
 *      / \
 *     3   4
 *          \
 *           5
 *            \
 *             6
 * 
 * 时间复杂度: O(n) - 每个节点最多访问2次
 * 空间复杂度: O(1) - 只使用常数额外空间
 * 
 * @param root: 二叉树的根节点
 */
void flatten(struct TreeNode* root) {

	struct TreeNode *current = root;
	struct TreeNode *predecessor = NULL;  /* 左子树的最右节点 */

	if (root == NULL) return;

	while (current != NULL) {
		/* 情况1：当前节点没有左子树，直接移动到右子树 */
		if (!current->left) {
			current = current->right;
		} 
		/* 情况2：当前节点有左子树，需要重新组织结构 */
		else {
			/* 步骤1: 找到左子树的最右节点（predecessor） */
			predecessor = current->left;
			while (predecessor->right != NULL) {
				predecessor = predecessor->right;
			}

			/* 步骤2: 将 current 原来的右子树接到 predecessor 的右边 */
			/*        这样右子树就不会丢失 */
			predecessor->right = current->right;

			/* 步骤3: 将左子树整体移动到右边 */
			current->right = current->left;
			
			/* 步骤4: 清空左指针（展开为链表） */
			current->left = NULL;

			/* 步骤5: 继续处理（新的）右子树 */
			current = current->right;
		}
	}

	return;
}

/*
 * Morris 遍历详细走查示例：
 * 
 * 输入树：
 *     1
 *    / \
 *   2   5
 *  / \   \
 * 3   4   6
 * 
 * === 第1步：处理节点1 ===
 * current = 1, 有左子树
 * - 找 predecessor: 2 -> 4 (最右节点)
 * - predecessor->right = 5 (将5接到4的右边)
 * - 将左子树移到右边：
 *     1
 *      \
 *       2
 *      / \
 *     3   4
 *          \
 *           5
 *            \
 *             6
 * - current = 2
 * 
 * === 第2步：处理节点2 ===
 * current = 2, 有左子树
 * - 找 predecessor: 3 (最右节点)
 * - predecessor->right = 4 (将4接到3的右边)
 * - 将左子树移到右边：
 *     1
 *      \
 *       2
 *        \
 *         3
 *          \
 *           4
 *            \
 *             5
 *              \
 *               6
 * - current = 3
 * 
 * === 第3-6步：处理节点3,4,5,6 ===
 * 这些节点都没有左子树，直接往右走
 * 
 * 最终结果：1 -> 2 -> 3 -> 4 -> 5 -> 6
 */