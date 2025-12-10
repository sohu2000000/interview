#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 77: 组合
 * 
 * 问题描述：
 * 给定两个整数 n 和 k，返回范围 [1, n] 中所有可能的 k 个数的组合
 * 
 * 示例：
 * 输入：n = 4, k = 2
 * 输出：[[1,2],[1,3],[1,4],[2,3],[2,4],[3,4]]
 * 
 * 核心思路：
 * - DFS回溯算法
 * - 从start开始枚举每个数字
 * - 递归构建组合，到达k个数时保存
 * - 剪枝优化：如果剩余数字不够，提前返回
 */

#define MAX_COMBINATIONS 200000

/*
 * DFS回溯生成组合
 * @param n: 数字范围[1,n]
 * @param k: 组合大小
 * @param startNum: 当前可选的起始数字
 * @param currentCombination: 当前组合
 * @param combinationSize: 当前组合中的数字个数
 * @param allCombinations: 所有组合结果
 * @param totalCount: 结果数量
 * 
 * 算法步骤：
 * 1. 终止条件：组合大小达到k，保存结果
 * 2. 剪枝：剩余数字不够，提前返回
 * 3. 回溯三步骤：
 *    a) 选择：将数字加入组合
 *    b) 递归：处理下一个位置
 *    c) 回溯：撤销选择
 * 
 * 剪枝优化：
 * - 剩余数字数 = n - currentNum + 1（包含currentNum）
 * - 还需数字数 = k - combinationSize
 * - 如果剩余 < 还需，提前返回
 */
void dfsBacktrackCombinations(int n, int k, int startNum,
		 int *currentCombination, int *combinationSize,
		 int **allCombinations, int *totalCount) {
	
	int remainingNumbers, stillNeeded;
	int i, currentNum;

	/* 终止条件：达到k个数，保存组合 */
	if (*combinationSize == k) {
		memcpy(allCombinations[*totalCount], currentCombination, k * sizeof(int));
		(*totalCount)++;
		return;
	}

	/* 枚举从startNum到n的每个数字 */
	for(i = startNum; i <= n; i++) {
		currentNum = i;
		
		/* 剪枝优化：剩余数字不够 */
		remainingNumbers = n - currentNum + 1;  /* [currentNum, n]的个数 */
		stillNeeded = k - (*combinationSize);   /* 还需要几个数 */
	
		if (remainingNumbers < stillNeeded)
			return;  /* 剩余不够，后面的数字更不够 */

		/* 选择：将当前数字加入组合 */
		currentCombination[*combinationSize] = currentNum;
		(*combinationSize)++;

		/* 递归：处理下一个位置（从currentNum+1开始） */
		dfsBacktrackCombinations(n, k, currentNum + 1, currentCombination, combinationSize, allCombinations, totalCount);

		/* 回溯：撤销选择 */
		(*combinationSize)--;
}
}

/*
 * 主函数：生成所有组合
 * @param n: 数字范围[1,n]
 * @param k: 组合大小
 * @param returnSize: 返回数组大小
 * @param returnColumnSizes: 返回每行的列数
 * @return: 所有组合
 * 
 * 时间复杂度: O(C(n,k) × k)，C(n,k)是组合数
 * 空间复杂度: O(k)，递归栈深度
 */
int** combine(int n, int k, int* returnSize, int** returnColumnSizes) {
	int i, combinationSize;
	int *currentCombination = NULL;
	int totalCount = 0;
	int **allCombinations = NULL;

	/* 分配结果数组 */
	allCombinations = (int **)malloc(MAX_COMBINATIONS * sizeof(int *));
	for(i = 0; i < MAX_COMBINATIONS; i++){
		allCombinations[i] = (int *)malloc(k * sizeof(int));
	}

	/* 分配列大小数组 */
	*returnColumnSizes = (int *)malloc(MAX_COMBINATIONS * sizeof(int));
	for(i = 0; i < MAX_COMBINATIONS; i++){
		(*returnColumnSizes)[i] = k;
	}

	/* 分配当前组合数组 */
	currentCombination = (int *)malloc(k * sizeof(int));
	combinationSize = 0;

	/* DFS回溯生成所有组合 */
	dfsBacktrackCombinations(n, k, 1, currentCombination, &combinationSize, allCombinations, &totalCount);

	*returnSize = totalCount;
	free(currentCombination);
	
	return allCombinations;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯 + 剪枝优化
 * 
 * 回溯模板：
 * void backtrack(选择列表, 路径, 结果) {
 *     if (满足结束条件) {
 *         保存结果
 *         return
 *     }
 *     
 *     for (选择 in 选择列表) {
 *         // 剪枝
 *         if (不满足条件) continue;
 *         
 *         // 选择
 *         路径.add(选择)
 *         
 *         // 递归
 *         backtrack(新的选择列表, 路径, 结果)
 *         
 *         // 回溯
 *         路径.remove(选择)
 *     }
 * }
 * 
 * 本题示例：n=4, k=2
 * 
 * DFS树：
 *              []
 *        /     |    |    \
 *       1      2    3    4
 *      /|\     |\    |
 *     2 3 4    3 4   4
 * 
 * 生成：[1,2], [1,3], [1,4], [2,3], [2,4], [3,4]
 * 
 * 剪枝优化详解：
 * 
 * 为什么是 n - current_num + 1？
 * - 包含两端：[current_num, n]
 * - 例如：n=4, current=3
 *   剩余数字：[3,4] = 2个
 *   4 - 3 + 1 = 2 ✓
 * 
 * 为什么是 k - combinationSize？
 * - 已选：combinationSize个
 * - 还需：k - combinationSize个
 * - 例如：k=3, size=1
 *   还需：3 - 1 = 2个 ✓
 * 
 * 何时剪枝？
 * - 剩余数字 < 还需数字
 * - 例如：剩1个，还需2个 → 不可能 → 剪枝
 */