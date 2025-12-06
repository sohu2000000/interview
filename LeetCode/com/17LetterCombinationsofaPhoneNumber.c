#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 17: 电话号码的字母组合
 * 
 * 问题描述：
 * 给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合
 * 
 * 数字到字母的映射（与电话按键相同）：
 * 2: abc, 3: def, 4: ghi, 5: jkl
 * 6: mno, 7: pqrs, 8: tuv, 9: wxyz
 * 
 * 示例：
 * 输入：digits = "23"
 * 输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
 * 
 * 核心思路：
 * - DFS回溯算法
 * - 对每个数字，尝试其对应的所有字母
 * - 递归构建组合，到达末尾时保存结果
 */

#define MAX_DIGITS 8 
#define MAX_COMBINATIONS (1 << 12)  /* 最多4^8种组合 */

/* 数字到字母的映射表 */
char *digitToLetters[] = {
    "abc",   /* 2 */
    "def",   /* 3 */
    "ghi",   /* 4 */
    "jkl",   /* 5 */
    "mno",   /* 6 */
    "pqrs",  /* 7 */
    "tuv",   /* 8 */
    "wxyz"   /* 9 */
};

/*
 * 获取数字对应的字母
 * @param digit: 数字字符（'2'-'9'）
 * @return: 对应的字母字符串
 */
char *getLettersForDigit(char digit) {
	return digitToLetters[digit - '2'];
}

/*
 * DFS回溯生成组合
 * @param digits: 输入的数字字符串
 * @param numDigits: 数字字符串长度
 * @param currentDepth: 当前处理的数字位置
 * @param currentPath: 当前路径（正在构建的组合）
 * @param combinations: 结果数组
 * @param combinationCount: 结果数量
 * 
 * 算法步骤：
 * 1. 到达末尾：保存当前路径到结果
 * 2. 获取当前数字对应的字母
 * 3. 对每个字母：
 *    a) 添加到路径
 *    b) 递归处理下一个数字
 *    c) 回溯（恢复状态）
 */
void dfsBacktrack(char *digits, int numDigits, int currentDepth,
		 char *currentPath, char **combinations, int *combinationCount) {
	char *letterChoices = NULL;
	int i, numLetters;

	/* 终止条件：到达数字末尾，保存组合 */
	if (currentDepth == numDigits) {
		combinations[*combinationCount] = strdup(currentPath);
		(*combinationCount)++;
		return;
	}

	/* 获取当前数字对应的字母 */
	letterChoices = getLettersForDigit(digits[currentDepth]);
	numLetters = strlen(letterChoices);
	
	/* 遍历所有可能的字母 */
	for (i = 0; i < numLetters; i++) {
		/* 选择：添加字母到路径 */
		currentPath[currentDepth] = letterChoices[i];
		
		/* 递归：处理下一个数字 */
		dfsBacktrack(digits, numDigits, currentDepth + 1,
			    currentPath, combinations, combinationCount);
		
		/* 回溯：清除当前字母（为下次尝试做准备） */
		currentPath[currentDepth] = '\0';
	}

	return;
}

/*
 * 主函数：生成电话号码的所有字母组合
 * @param digits: 数字字符串
 * @param returnSize: 返回数组的大小
 * @return: 所有可能的字母组合
 * 
 * 时间复杂度: O(4^N)，N是数字个数（最坏情况，如7和9有4个字母）
 * 空间复杂度: O(N)，递归栈深度
 */
char** letterCombinations(char* digits, int* returnSize) {
	int numDigits, i;
	char **combinations = NULL;
	char *currentPath = NULL;

	/* 边界检查 */
	if (digits == NULL || strlen(digits) == 0) {
		*returnSize = 0;
		return NULL;
	}

	/* 分配结果数组 */
	combinations = (char **)malloc(MAX_COMBINATIONS * sizeof(char *));
	for (i = 0; i < MAX_COMBINATIONS; i++)
		combinations[i] = NULL;
	*returnSize = 0;

	/* 获取数字长度 */
	numDigits = strlen(digits);

	/* 分配路径数组 */
	currentPath = (char *)malloc((numDigits + 1) * sizeof(char));
	memset(currentPath, 0, (numDigits + 1) * sizeof(char));

	/* DFS回溯生成所有组合 */
	dfsBacktrack(digits, numDigits, 0, currentPath, combinations, returnSize);
	
	/* 释放临时数组 */
	free(currentPath);
	
	return combinations;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯算法
 * 
 * 回溯三步骤：
 * 1. 选择：path[i] = choices[j]
 * 2. 递归：dfs(i+1)
 * 3. 回溯：path[i] = '\0'
 * 
 * 示例：digits = "23"
 * 
 * 数字到字母：
 * '2' -> "abc"
 * '3' -> "def"
 * 
 * DFS树：
 *          root
 *        /  |  \
 *       a   b   c    (选择'2'的字母)
 *      /|\ /|\ /|\
 *     d e f d e f d e f  (选择'3'的字母)
 * 
 * 结果：ad, ae, af, bd, be, bf, cd, ce, cf
 * 
 * 回溯过程：
 * path=""
 * 选择'a': path="a"
 *   选择'd': path="ad" -> 保存
 *   回溯: path="a"
 *   选择'e': path="ae" -> 保存
 *   回溯: path="a"
 *   选择'f': path="af" -> 保存
 *   回溯: path="a"
 * 回溯: path=""
 * 选择'b': path="b"
 *   ...
 */