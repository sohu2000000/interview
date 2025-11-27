#include <stdio.h>
#include <stdlib.h>

/**
 * Definition for a Node.
 * struct Node {
 *     int val;
 *     struct Node *left;
 *     struct Node *right;
 *     struct Node *next;
 * };
 */

struct Node {
	int val;
	struct Node *left;
	struct Node *right;
	struct Node *next;
};

/*
 * LeetCode 117: 填充每个节点的下一个右侧节点指针 II
 * 
 * 核心思路：
 * - 层序遍历，使用 next 指针连接同一层的节点
 * - 利用已建立的 next 指针遍历当前层
 * - 同时建立下一层节点的 next 连接
 * - O(1) 空间复杂度（不使用队列）
 */

/*
 * 递归连接每一层的节点
 * @param levelHead: 当前层的头节点
 * 
 * 算法步骤：
 * 1. 遍历当前层（通过已有的 next 指针）
 * 2. 将下一层的节点用 next 指针连接起来
 * 3. 递归处理下一层
 */
void connectLevel(struct Node* levelHead) {

	struct Node* current = levelHead;
	struct Node* nextLevelHead = NULL;  /* 下一层的头节点 */
	struct Node* nextLevelTail = NULL;  /* 下一层的尾节点（用于连接新节点） */

	/* 递归终止条件：当前层为空 */
	if (!levelHead) return;

	/* 遍历当前层的所有节点 */
	while (current) {
		/* 找到下一层的第一个节点（头节点） */
		if (!nextLevelHead)
			nextLevelHead = current->left ? current->left : current->right;
		
		/* 处理左子节点 */
		if (current->left) {
			if (nextLevelTail) {
				nextLevelTail->next = current->left;  /* 连接到下一层的链表 */
			}
			nextLevelTail = current->left;  /* 更新尾节点 */
		}

		/* 处理右子节点 */
		if (current->right) {
			if (nextLevelTail) {
				nextLevelTail->next = current->right;  /* 连接到下一层的链表 */
			}
			nextLevelTail = current->right;  /* 更新尾节点 */
		}

		/* 移动到当前层的下一个节点 */
		current = current->next;
	}

	/* 递归处理下一层 */
	connectLevel(nextLevelHead);
	return;
}

/*
 * 主函数：填充每个节点的下一个右侧节点指针
 * @param root: 二叉树的根节点
 * @return: 处理后的根节点
 * 
 * 时间复杂度: O(n) - 每个节点访问一次
 * 空间复杂度: O(h) - 递归栈深度，h为树的高度
 */
struct Node* connect(struct Node* root) {
	if (!root) return NULL;

	connectLevel(root);
	return root;
}

