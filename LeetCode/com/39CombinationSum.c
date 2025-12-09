#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 39: 组合总和
 * 
 * 问题描述：
 * 给定一个无重复元素的正整数数组 candidates 和一个目标正整数 target
 * 找出 candidates 中所有可以使数字和为 target 的组合
 * candidates 中的同一个数字可以无限制重复被选取
 * 
 * 示例：
 * 输入：candidates = [2,3,6,7], target = 7
 * 输出：[[2,2,3],[7]]
 * 
 * 核心思路：
 * - DFS回溯算法
 * - 允许重复选择同一元素（递归传i，不是i+1）
 * - 剪枝：当前和 > target 时提前返回
 */

#define MAX_COMBINATIONS 10000

/*
 * DFS回溯生成组合
 * @param candidates: 候选数字数组
 * @param numCandidates: 候选数字数量
 * @param targetSum: 目标和
 * @param startIndex: 当前可选的起始索引
 * @param currentSum: 当前路径的和
 * @param currentPath: 当前路径
 * @param pathLength: 当前路径长度
 * @param allCombinations: 所有组合结果
 * @param totalCount: 结果数量
 * @param columnSizes: 每个组合的长度
 * 
 * 算法步骤：
 * 1. 终止条件：当前和 == target，保存组合
 * 2. 剪枝：当前和 > target，提前返回
 * 3. 回溯三步骤：
 *    a) 选择：添加数字到路径，累加和
 *    b) 递归：从当前索引继续（允许重复）
 *    c) 回溯：移除数字，减去和
 * 
 * 与普通组合的区别：
 * - 普通组合：递归传i+1（每个数字只用一次）
 * - 本题：递归传i（可以重复使用同一数字）
 */
void dfsBacktrackCombinationSum(int *candidates, int numCandidates, int targetSum,
		 int startIndex, int currentSum,
		 int *currentPath, int *pathLength,
		 int **allCombinations, int *totalCount, int **columnSizes) {
	int i;

	/* 终止条件：找到目标和 */
	if (currentSum == targetSum) {
		memcpy(allCombinations[*totalCount], currentPath, *pathLength * sizeof(int));
		(*columnSizes)[*totalCount] = *pathLength;
		(*totalCount)++;
		return;
	}

	/* 剪枝：和超过目标 */
	if (currentSum > targetSum)
		return;

	/* 枚举从startIndex开始的所有候选数字 */
	for (i = startIndex; i < numCandidates; i++) {
		/* 选择：添加到路径，累加和 */
		currentSum += candidates[i];
		currentPath[*pathLength] = candidates[i];
		(*pathLength)++;

		/* 递归：从i继续（允许重复使用） */
		dfsBacktrackCombinationSum(candidates, numCandidates, targetSum, i, currentSum, currentPath, pathLength, allCombinations, totalCount, columnSizes);

		/* 回溯：移除路径，减去和 */
		(*pathLength)--;
		currentSum -= candidates[i];
	}

	return;
}

/*
 * 主函数：找出所有和为target的组合
 * @param candidates: 候选数字数组
 * @param candidatesSize: 数组大小
 * @param target: 目标和
 * @param returnSize: 返回数组大小
 * @param returnColumnSizes: 每个组合的长度
 * @return: 所有组合
 * 
 * 时间复杂度: O(N^(T/M))，N是candidates数量，T是target，M是最小候选数
 * 空间复杂度: O(T/M)，递归栈深度
 */
int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {
	int **allCombinations = NULL;
	int pathLength = 0;
	int *currentPath = NULL;
	int i;

	/* 分配结果数组 */
	allCombinations = (int **)malloc(MAX_COMBINATIONS * sizeof(int *));
	for (i = 0; i < MAX_COMBINATIONS; i++) {
		allCombinations[i] = (int *)malloc(target * sizeof(int));
	}

	/* 分配列大小数组 */
	*returnColumnSizes = (int *)malloc(MAX_COMBINATIONS * sizeof(int));
	*returnSize = 0;

	/* 分配当前路径数组 */
	currentPath = (int *)malloc(target * sizeof(int));
	memset(currentPath, 0, target * sizeof(int));
	pathLength = 0;

	/* DFS回溯生成所有组合 */
	dfsBacktrackCombinationSum(candidates, candidatesSize, target, 0, 0, currentPath, &pathLength, allCombinations, returnSize, returnColumnSizes);

	/* 释放临时数组 */
	free(currentPath);
	
	return allCombinations;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯 + 允许重复
 * 
 * 与普通组合的区别：
 * - 普通组合（77题）：每个数字只用一次
 *   递归：dfs(i+1, ...)
 * 
 * - 本题：每个数字可以重复使用
 *   递归：dfs(i, ...)（传i，不是i+1）
 * 
 * 示例：candidates = [2,3], target = 5
 * 
 * DFS树（允许重复）：
 *           []
 *        /     \
 *       2       3
 *      / \      |
 *     2   3     3
 *    /|   |
 *   2 3   (5)
 *  
 * path=[2,2] → sum=4 < 5 → 继续
 * path=[2,2,2] → sum=6 > 5 → 剪枝
 * path=[2,3] → sum=5 ✓ → 保存
 * path=[3,3] → sum=6 > 5 → 剪枝
 * 
 * 结果：[[2,3]] (只有顺序[2,3]，没有[3,2]，因为用了start)
 */