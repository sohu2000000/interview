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
 * LeetCode 236: 二叉树的最近公共祖先
 * 
 * 问题描述：
 * 给定一个二叉树，找到该树中两个指定节点的最近公共祖先（LCA）
 * 
 * 最近公共祖先的定义：
 * 对于有根树T的两个节点p、q，最近公共祖先表示为一个节点x，
 * 满足x是p、q的祖先且x的深度尽可能大（一个节点也可以是它自己的祖先）
 * 
 * 示例：
 *         3
 *       /   \
 *      5     1
 *     / \   / \
 *    6   2 0   8
 *       / \
 *      7   4
 * 
 * LCA(5, 1) = 3
 * LCA(5, 4) = 5 (节点可以是自己的祖先)
 * LCA(6, 4) = 5
 * 
 * 核心思路：
 * - 使用后序遍历（左-右-根）
 * - 递归返回值表示"在该子树中是否找到p或q"
 * - 如果左右子树都找到，则当前节点就是LCA
 * - 如果只有一边找到，则返回那一边的结果
 */

/*
 * 递归查找最近公共祖先
 * @param root: 当前子树的根节点
 * @param p: 目标节点p
 * @param q: 目标节点q
 * @return: 返回LCA节点，或找到的p/q节点，或NULL
 * 
 * 返回值含义：
 * - NULL: 当前子树中既没有p也没有q
 * - p或q: 当前子树中找到了p或q（或它们的LCA）
 * - root: 当前节点是LCA
 * 
 * 算法步骤：
 * 1. 递归终止：root为空，返回NULL
 * 2. 递归终止：root是p或q，返回root
 * 3. 递归查找左子树
 * 4. 递归查找右子树
 * 5. 根据左右子树结果判断：
 *    - 左右都找到 -> root是LCA
 *    - 只有左找到 -> 返回左
 *    - 只有右找到 -> 返回右
 *    - 都没找到 -> 返回NULL
 * 
 * 时间复杂度: O(n) - 最坏情况访问所有节点
 * 空间复杂度: O(h) - 递归栈深度
 */
struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {

	struct TreeNode *leftResult = NULL, *rightResult = NULL;

	/* 情况1: 递归终止 - 节点为空 */
	if (root == NULL)
		return NULL;

	/* 情况2: 递归终止 - 找到目标节点 */
	if (root == p || root == q)
		return root;
	
	/* 步骤1: 在左子树中查找 */
	leftResult = lowestCommonAncestor(root->left, p, q);
	
	/* 步骤2: 在右子树中查找 */
	rightResult = lowestCommonAncestor(root->right, p, q);

	/* 步骤3: 根据左右子树的结果判断 */
	
	/* 情况3: 左右子树都找到了，说明p和q分别在左右子树，当前节点是LCA */
	if (leftResult != NULL && rightResult != NULL)
		return root;
	
	/* 情况4: 只在左子树找到，返回左子树的结果 */
	if (leftResult != NULL)
		return leftResult;
	
	/* 情况5: 只在右子树找到，返回右子树的结果 */
	if (rightResult != NULL)
		return rightResult;

	/* 情况6: 左右都没找到，返回NULL */
	return NULL;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心理解：后序遍历 + 信息向上传递
 * 
 * 后序遍历顺序：左 -> 右 -> 根
 * 这样可以先获取左右子树的信息，再在当前节点做判断
 * 
 * 返回值的含义：
 * - NULL: 子树中没有p和q
 * - 非NULL: 子树中有p、q或它们的LCA
 * 
 * ==================== 详细走查 ====================
 * 
 * 示例：找 LCA(5, 1)
 * 
 *         3
 *       /   \
 *      5     1
 *     / \   / \
 *    6   2 0   8
 *       / \
 *      7   4
 * 
 * === 递归过程（后序遍历）===
 * 
 * 1. lowestCommonAncestor(6, 5, 1)
 *    - 6 != 5 且 6 != 1
 *    - leftResult = NULL (6无子节点)
 *    - rightResult = NULL (6无子节点)
 *    - 返回 NULL
 * 
 * 2. lowestCommonAncestor(7, 5, 1)
 *    - 返回 NULL (同上)
 * 
 * 3. lowestCommonAncestor(4, 5, 1)
 *    - 返回 NULL (同上)
 * 
 * 4. lowestCommonAncestor(2, 5, 1)
 *    - 2 != 5 且 2 != 1
 *    - leftResult = NULL (节点7返回的)
 *    - rightResult = NULL (节点4返回的)
 *    - 返回 NULL
 * 
 * 5. lowestCommonAncestor(5, 5, 1)
 *    - root == 5，找到目标节点！
 *    - 返回 5
 * 
 * 6. lowestCommonAncestor(0, 5, 1)
 *    - 返回 NULL
 * 
 * 7. lowestCommonAncestor(8, 5, 1)
 *    - 返回 NULL
 * 
 * 8. lowestCommonAncestor(1, 5, 1)
 *    - root == 1，找到目标节点！
 *    - 返回 1
 * 
 * 9. lowestCommonAncestor(3, 5, 1)
 *    - 3 != 5 且 3 != 1
 *    - leftResult = 5 (左子树返回5)
 *    - rightResult = 1 (右子树返回1)
 *    - leftResult != NULL && rightResult != NULL
 *    - 返回 3 ✓ (这就是LCA！)
 * 
 * ==================== 关键场景分析 ====================
 * 
 * 场景1：p和q分别在左右子树
 *     root        leftResult=p, rightResult=q
 *     /  \        -> 返回 root (LCA)
 *    p    q
 * 
 * 场景2：p是q的祖先
 *     p           root==p，直接返回p
 *    /            leftResult=q, rightResult=NULL
 *   q             -> p就是LCA
 * 
 * 场景3：p和q都在左子树
 *     root        leftResult=LCA(p,q), rightResult=NULL
 *     /           -> 返回 leftResult
 *    ...
 *   /   \
 *  p     q
 * 
 * 场景4：都不在当前子树
 *     root        leftResult=NULL, rightResult=NULL
 *     /  \        -> 返回 NULL
 *    a    b
 */