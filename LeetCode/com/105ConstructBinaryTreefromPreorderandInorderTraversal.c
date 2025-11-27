#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

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
 * LeetCode 105: 从前序与中序遍历构造二叉树
 * 
 * 核心思路：
 * - 前序遍历：[根节点, [左子树], [右子树]]
 * - 中序遍历：[[左子树], 根节点, [右子树]]
 * 
 * 算法步骤：
 * 1. 前序遍历的第一个节点是根节点
 * 2. 在中序遍历中找到根节点位置，左边是左子树，右边是右子树
 * 3. 递归构建左右子树
 */

/* 哈希表节点：用于快速查找中序遍历中节点的索引位置 */
struct InorderIndexNode {
	int val;          /* 节点值 */
	int index;        /* 在中序遍历中的索引位置 */
	UT_hash_handle hh;
};

struct InorderIndexNode *inorderHashMap = NULL;

/* 创建中序遍历的哈希映射：值 -> 索引 */
void createInorderHashMap(int* inorder, int inorderSize) {
	int i;
	for (i = 0; i < inorderSize; i++) {
		struct InorderIndexNode *node = (struct InorderIndexNode *)malloc(sizeof(struct InorderIndexNode));
		node->val = inorder[i];
		node->index = i;
		HASH_ADD_INT(inorderHashMap, val, node);
	}

	return;
}

/* 释放哈希映射内存 */
void freeHashMap() {
	struct InorderIndexNode *currentNode, *tmpNode;
	HASH_ITER(hh, inorderHashMap, currentNode, tmpNode) {
		HASH_DEL(inorderHashMap, currentNode);
		free(currentNode);
	}
	inorderHashMap = NULL;

	return;
}


/*
 * 递归构建二叉树
 * @param preorder: 前序遍历数组
 * @param inorder: 中序遍历数组
 * @param preStart: 前序遍历子树的起始索引
 * @param preEnd: 前序遍历子树的结束索引
 * @param inStart: 中序遍历子树的起始索引
 * @param inEnd: 中序遍历子树的结束索引
 * @param hashMap: 中序遍历的值->索引映射
 * @return: 构建的子树根节点
 */
struct TreeNode* buildTreeRecursive(int *preorder, int *inorder,
				    int preStart, int preEnd,
				    int inStart,  int inEnd,
				    struct InorderIndexNode *hashMap) {

	int preorderRootIdx, inorderRootIdx;
	int preLeftStart, preLeftEnd, inLeftStart, inLeftEnd, leftSize;
	int preRightStart, preRightEnd, inRightStart, inRightEnd, rightSize;
	int rootVal;
	struct TreeNode *root;
	struct InorderIndexNode *foundNode;

	/* 递归终止条件：子树为空 */
	if (preStart > preEnd)
		return NULL;
	
	/* 步骤1: 前序遍历的第一个元素是根节点 */
	preorderRootIdx = preStart;
	rootVal = preorder[preorderRootIdx];

	root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	root->val = rootVal;
	root->left = NULL;
	root->right = NULL;

	/* 步骤2: 在中序遍历中找到根节点位置 */
	HASH_FIND_INT(hashMap, &rootVal, foundNode);
	inorderRootIdx = foundNode->index;

	/* 步骤3: 计算左子树的范围 */
	leftSize = inorderRootIdx - inStart;  /* 左子树节点数量, 注意包含左子树的根节点*/
	inLeftStart = inStart;
	inLeftEnd = inorderRootIdx - 1;
	preLeftStart = preorderRootIdx + 1;
	preLeftEnd = preLeftStart + leftSize - 1;

	/* 步骤4: 计算右子树的范围 */
	rightSize = inEnd - inorderRootIdx;  /* 右子树节点数量, 注意包含右子树的根节点 */
	inRightStart = inorderRootIdx + 1;
	inRightEnd = inEnd;
	preRightStart = preLeftEnd + 1;
	preRightEnd = preEnd;

	/* 步骤5: 递归构建左右子树 */
	root->left = buildTreeRecursive(preorder, inorder, preLeftStart, preLeftEnd, inLeftStart, inLeftEnd, hashMap);
	root->right = buildTreeRecursive(preorder, inorder, preRightStart, preRightEnd, inRightStart, inRightEnd, hashMap);

	return root;
}

/*
 * 主函数：从前序和中序遍历构造二叉树
 * @param preorder: 前序遍历数组
 * @param preorderSize: 前序遍历数组大小
 * @param inorder: 中序遍历数组
 * @param inorderSize: 中序遍历数组大小
 * @return: 构建的二叉树根节点
 * 
 * 时间复杂度: O(n) - 哈希表优化查找
 * 空间复杂度: O(n) - 哈希表 + 递归栈
 */
struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize) {

	struct TreeNode *root;

	/* 边界条件检查 */
	if (NULL == preorder || NULL == inorder || preorderSize != inorderSize || preorderSize == 0)
		return NULL;
	
	/* 创建中序遍历的哈希映射，用于O(1)查找节点位置 */
	createInorderHashMap(inorder, inorderSize);

	/* 递归构建整棵树 */
	root = buildTreeRecursive(preorder, inorder, 0, preorderSize - 1, 0, inorderSize - 1, inorderHashMap);

	/* 释放哈希映射内存 */
	freeHashMap();

	return root;
}