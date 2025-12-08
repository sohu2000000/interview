#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 46: 全排列
 * 
 * 问题描述：
 * 给定一个不含重复数字的数组 nums，返回其所有可能的全排列
 * 
 * 示例：
 * 输入：nums = [1,2,3]
 * 输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
 * 
 * 核心思路：
 * - DFS回溯算法
 * - 使用visited数组标记已使用的数字
 * - 枚举每个未使用的数字，递归构建排列
 * - 到达n个数时保存结果
 */

#define MAX_PERMUTATIONS 100000

/*
 * DFS回溯生成全排列
 * @param numbers: 输入数组
 * @param arraySize: 数组大小
 * @param isUsed: 标记数组（记录数字是否已使用）
 * @param currentPath: 当前路径（正在构建的排列）
 * @param currentLength: 当前路径长度
 * @param allPermutations: 所有排列结果
 * @param totalCount: 结果数量
 * 
 * 算法步骤：
 * 1. 终止条件：路径长度达到n，保存排列
 * 2. 回溯三步骤：
 *    a) 选择：标记已使用，添加到路径
 *    b) 递归：继续构建排列
 *    c) 回溯：取消标记，移除路径
 * 
 * 与组合的区别：
 * - 组合：从start开始，避免重复
 * - 排列：从0开始，用visited避免重复
 */
void dfsBacktrackPermutations(int *numbers, int arraySize, int *isUsed, 
		 int *currentPath, int *currentLength,
		 int **allPermutations, int *totalCount) {

	int i;

	/* 终止条件：达到n个数，保存排列 */
	if (*currentLength == arraySize) { 
		memcpy(allPermutations[*totalCount], currentPath, arraySize * sizeof(int));
		(*totalCount)++;
		return;
	}

	/* 枚举所有数字 */
	for (i = 0; i < arraySize; i++) {
		/* 跳过已使用的数字 */
		if (isUsed[i] == 1)
			continue;
		
		/* 选择：标记已使用，添加到路径 */
		isUsed[i] = 1;
		currentPath[*currentLength] = numbers[i];
		(*currentLength)++;

		/* 递归：继续构建排列 */
		dfsBacktrackPermutations(numbers, arraySize, isUsed,
			    currentPath, currentLength, 
			    allPermutations, totalCount);
		
		/* 回溯：取消标记，移除路径 */
		isUsed[i] = 0;
		(*currentLength)--;
	}

	return;
}

/*
 * 主函数：生成所有全排列
 * @param nums: 输入数组
 * @param numsSize: 数组大小
 * @param returnSize: 返回数组大小
 * @param returnColumnSizes: 返回每行的列数
 * @return: 所有全排列
 * 
 * 时间复杂度: O(n! × n)，n!个排列，每个复制需要O(n)
 * 空间复杂度: O(n)，递归栈深度 + visited数组
 */
int** permute(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
	int *isUsed = NULL;
	int **allPermutations = NULL;
	int *currentPath = NULL;
	int i, currentLength;

	/* 分配结果数组 */
	allPermutations = (int **)malloc(MAX_PERMUTATIONS * sizeof(int *));
	for (i = 0; i < MAX_PERMUTATIONS; i++) {
		allPermutations[i] = (int *)malloc(numsSize * sizeof(int));
	}

	/* 分配列大小数组 */
	*returnColumnSizes = (int *)malloc(MAX_PERMUTATIONS * sizeof(int));
	for (i = 0; i < MAX_PERMUTATIONS; i++) {
		(*returnColumnSizes)[i] = numsSize;
	}

	*returnSize = 0;
	
	/* 分配visited数组 */
	isUsed = (int *)malloc(numsSize * sizeof(int));
	memset(isUsed, 0, numsSize * sizeof(int));

	/* 分配当前路径数组 */
	currentPath = (int *)malloc(numsSize * sizeof(int));
	memset(currentPath, 0, numsSize * sizeof(int));
	currentLength = 0;

	/* DFS回溯生成所有排列 */
	dfsBacktrackPermutations(nums, numsSize, isUsed, currentPath, &currentLength, allPermutations, returnSize);

	/* 释放临时数组 */
	free(isUsed);
	free(currentPath);

	return allPermutations;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯 + visited标记
 * 
 * 回溯模板：
 * for (选择 in 所有选择) {
 *     if (已选择) continue;
 *     
 *     // 选择
 *     标记已选择
 *     路径.add(选择)
 *     
 *     // 递归
 *     backtrack(...)
 *     
 *     // 回溯
 *     取消标记
 *     路径.remove(选择)
 * }
 * 
 * 示例：nums = [1,2,3]
 * 
 * DFS树：
 *              []
 *        /      |      \
 *       1       2       3
 *      / \     / \     / \
 *     2   3   1   3   1   2
 *     |   |   |   |   |   |
 *     3   2   3   1   2   1
 * 
 * 生成6个排列：
 * [1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]
 * 
 * 与组合的区别：
 * - 组合：[1,2] = [2,1]（顺序无关）
 *   用start控制只从后面选
 * - 排列：[1,2] ≠ [2,1]（顺序相关）
 *   用visited标记避免重复使用
 */