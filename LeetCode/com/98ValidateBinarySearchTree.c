#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

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
 * LeetCode 98: 验证二叉搜索树
 * 
 * 问题描述：
 * 给定一个二叉树的根节点，判断其是否是一个有效的二叉搜索树（BST）
 * 
 * BST定义：
 * - 节点的左子树只包含小于当前节点的数
 * - 节点的右子树只包含大于当前节点的数
 * - 所有左子树和右子树自身也必须是BST
 * 
 * 示例：
 * 输入: [2,1,3]        输入: [5,1,4,null,null,3,6]
 *   2                    5
 *  / \                  / \
 * 1   3                1   4
 * 输出: true              / \
 *                       3   6
 *                     输出: false（4 < 5违反BST，节点6在根节点5的左子树中）
 * 
 * 核心思路：
 * - 使用递归，传递合法值的范围 (min, max)
 * - 每个节点的值必须在 (min, max) 开区间内
 * - 左子树：范围缩小为 (min, root->val)
 * - 右子树：范围缩小为 (root->val, max)
 * - 使用long类型避免INT_MIN/INT_MAX的边界问题
 */

/*
 * 递归验证BST，检查节点值是否在合法范围内
 * @param root: 当前节点
 * @param minValue: 当前节点值的下界（开区间）
 * @param maxValue: 当前节点值的上界（开区间）
 * @return: true表示合法BST，false表示不合法
 * 
 * 算法步骤：
 * 1. 空节点返回true
 * 2. 检查当前节点值是否在 (minValue, maxValue) 范围内
 * 3. 递归检查左子树（范围缩小为 (minValue, root->val)）
 * 4. 递归检查右子树（范围缩小为 (root->val, maxValue)）
 * 
 * 关键：使用long类型避免边界问题
 * - 当节点值为INT_MIN时，用int类型无法表示更小的下界
 * - 当节点值为INT_MAX时，用int类型无法表示更大的上界
 * 
 * 时间复杂度: O(n) - 访问每个节点一次
 * 空间复杂度: O(h) - 递归栈深度
 */
bool validateBSTRange(struct TreeNode* root, long minValue, long maxValue) {
	if (!root)
		return true;
	
	/* 步骤1: 检查当前节点值是否在合法范围内 */
	if (root->val <= minValue || root->val >= maxValue)
		return false;
	
	/* 步骤2: 递归检查左子树（所有节点都要 < root->val） */
	if (!validateBSTRange(root->left, minValue, root->val))
		return false;
	
	/* 步骤3: 递归检查右子树（所有节点都要 > root->val） */
	if (!validateBSTRange(root->right, root->val, maxValue))
		return false;

	return true;
}

/*
 * 主函数：验证是否为合法BST
 * @param root: 二叉树的根节点
 * @return: true表示合法BST，false表示不合法
 * 
 * 初始范围：(-∞, +∞)
 */
bool isValidBST(struct TreeNode* root) {
	/* 根节点的范围是 (-∞, +∞)，使用long避免边界问题 */
	return validateBSTRange(root, LONG_MIN, LONG_MAX);
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：范围约束递归传递
 * 
 * BST性质（重要！）：
 * - 不仅仅是"左子节点 < 根 < 右子节点"
 * - 而是"左子树所有节点 < 根 < 右子树所有节点"
 * 
 * 错误理解：
 *     10
 *    /
 *   5
 *    \
 *     12  ❌ 12 > 5（满足父节点），但12 > 10（违反根节点）
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入: [5,1,4,null,null,3,6]
 * 
 *     5
 *    / \
 *   1   4
 *      / \
 *     3   6
 * 
 * === 递归过程 ===
 * 
 * 1. validateBSTRange(5, LONG_MIN, LONG_MAX)
 *    范围：(-∞, +∞)
 *    检查：5 > -∞ && 5 < +∞ ✓
 *    
 *    检查左子树(1)：
 *    2. validateBSTRange(1, LONG_MIN, 5)
 *       范围：(-∞, 5)
 *       检查：1 > -∞ && 1 < 5 ✓
 *       无子节点，返回true
 *    
 *    检查右子树(4)：
 *    3. validateBSTRange(4, 5, LONG_MAX)
 *       范围：(5, +∞)
 *       检查：4 > 5 && 4 < +∞
 *            false && true = false ✗
 *       返回false
 * 
 * 返回 false（正确识别非法BST）✓
 * 
 * ==================== 范围传递图解 ====================
 * 
 *                 10 (-∞, +∞)
 *                /          \
 *               /            \
 *         5 (-∞, 10)      15 (10, +∞)
 *        /      \          /        \
 *       /        \        /          \
 *  3 (-∞,5)  7 (5,10) 12(10,15)  20(15,+∞)
 * 
 * 左子树：min继承，max=父节点值
 * 右子树：min=父节点值，max继承
 * 
 * ==================== 递归的调用顺序 vs 处理顺序 ====================
 * 
 * 理解递归的两个维度：
 * 
 * 1. 调用顺序（从上往下）：
 *    - 递归是从根节点往下"钻"到叶子
 *    - 调用栈不断深入：root → left → left's left → ...
 *    - 这时调用栈最深，但还没有真正"处理"节点
 * 
 * 2. 处理顺序（从下往上）：
 *    - 到达叶子后，开始返回
 *    - 叶子先处理完，返回给父节点
 *    - 父节点处理完，返回给祖父节点
 *    - 逐层向上，最后回到根
 * 
 * 对于本题（后序遍历风格）：
 * 
 * 调用顺序：root → left → right（递归下探）
 * 处理顺序：left 处理完 → right 处理完 → 最后处理 root（逐层返回）
 * 
 * 写在"子递归调用之后"的代码，执行顺序就是从叶子往上到root！
 * 
 * 例子：
 *     10
 *    /  \
 *   5   15
 * 
 * 调用顺序（深入）：
 * 1. validateBSTRange(10, ...)  [调用，未处理]
 * 2.   validateBSTRange(5, ...)  [调用，未处理]
 * 3.     到达叶子，返回true
 * 4.   validateBSTRange(15, ...) [调用，未处理]
 * 5.     到达叶子，返回true
 * 
 * 处理顺序（返回）：
 * 6.   节点5处理完成，返回true
 * 7.   节点15处理完成，返回true
 * 8. 节点10处理完成，返回true（最后处理）
 * 
 * 这就是"自底向上"的本质：
 * - 调用时：根→叶（往下钻）
 * - 返回时：叶→根（往上爬）
 * - 处理时：叶子先处理，根节点最后处理
 */