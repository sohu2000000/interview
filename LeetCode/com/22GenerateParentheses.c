#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 22: 括号生成
 * 
 * 问题描述：
 * 数字 n 代表生成括号的对数，设计一个函数生成所有可能的并且有效的括号组合
 * 
 * 示例：
 * 输入：n = 3
 * 输出：["((()))","(()())","(())()","()(())","()()()"]
 * 
 * 核心思路：
 * - DFS回溯算法
 * - 有效括号条件：
 *   1. 左括号数量 ≤ n
 *   2. 右括号数量 < 左括号数量（保证匹配）
 * - 不需要visited，因为只需要记录左右括号数量
 */

#define MAX_COMBINATIONS 10000

/*
 * DFS回溯生成有效括号组合
 * @param numPairs: 括号对数
 * @param leftCount: 已使用的左括号数量
 * @param rightCount: 已使用的右括号数量
 * @param currentString: 当前字符串
 * @param currentLength: 当前字符串长度
 * @param allCombinations: 所有组合结果
 * @param totalCount: 结果数量
 * 
 * 算法步骤：
 * 1. 终止条件：左右括号都用完n个，保存结果
 * 2. 如果左括号 < n，可以添加左括号
 * 3. 如果右括号 < 左括号，可以添加右括号
 * 
 * 关键：
 * - 右括号数量必须 < 左括号数量（保证有效）
 * - 例如：不能先添加 ')'，必须有 '(' 才能匹配
 */
void dfsGenerateParentheses(int numPairs, int leftCount, int rightCount, 
                            char *currentString, int currentLength, 
                            char **allCombinations, int *totalCount) {

	/* 终止条件：左右括号都用完n个 */
	if (leftCount == numPairs && rightCount == numPairs) {
		currentString[currentLength] = '\0';
		memcpy(allCombinations[*totalCount], currentString, (currentLength + 1) * sizeof(char));
		(*totalCount)++;
		return;
	}

	/* 选择1：添加左括号（如果还有剩余） */
	if (leftCount < numPairs) {
		currentString[currentLength] = '(';
		currentLength++;
		dfsGenerateParentheses(numPairs, leftCount + 1, rightCount, currentString, currentLength, allCombinations, totalCount);
		currentLength--;
	}

	/* 选择2：添加右括号（如果能匹配） */
	if (rightCount < leftCount) {
		currentString[currentLength] = ')';
		currentLength++;
		dfsGenerateParentheses(numPairs, leftCount, rightCount + 1, currentString, currentLength, allCombinations, totalCount);
		currentLength--;
	}

	return;
}

/*
 * 主函数：生成所有有效括号组合
 * @param n: 括号对数
 * @param returnSize: 返回数组大小
 * @return: 所有有效括号组合
 * 
 * 时间复杂度: O(4^n / √n)，卡特兰数
 * 空间复杂度: O(n)，递归栈深度
 */
char** generateParenthesis(int n, int* returnSize) {

	char **allCombinations = NULL;
	char *currentString = NULL;
	int currentLength = 0, maxLength = 0, i;

	maxLength = 2 * n + 1;
	
	/* 分配结果数组 */
	allCombinations = (char **)malloc(MAX_COMBINATIONS * sizeof(char *));
	for (i = 0; i < MAX_COMBINATIONS; i++) {
		allCombinations[i] = (char *)malloc(maxLength * sizeof(char));
	}
	*returnSize = 0;

	/* 分配当前字符串 */
	currentString = (char *)malloc(maxLength * sizeof(char));
	memset(currentString, 0, maxLength * sizeof(char));
	currentLength = 0;

	/* DFS回溯生成所有组合 */
	dfsGenerateParentheses(n, 0, 0, currentString, currentLength, allCombinations, returnSize);

	/* 释放临时数组 */
	free(currentString);
	
	return allCombinations;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯 + 括号匹配规则
 * 
 * 有效括号的充要条件：
 * 1. 左括号数量 = 右括号数量 = n
 * 2. 任意前缀中，左括号数量 ≥ 右括号数量
 * 
 * DFS决策树（n=3）：
 *                  ""
 *                  ↓
 *                  (         ← 只能添加(
 *                 / \
 *                (   )       ← 可以添加(或)
 *               /|   |\
 *              ( )   ( )     ← 继续...
 * 
 * 为什么 right < left？
 * - 保证任意时刻左括号 ≥ 右括号
 * - 例如：")(" 不合法，因为第一个字符是 ')'
 * 
 * 与其他回溯题的区别：
 * - 不需要visited（只需要计数）
 * - 不需要start（不是从数组选择）
 * - 两个选择：添加'(' 或添加')'
 */