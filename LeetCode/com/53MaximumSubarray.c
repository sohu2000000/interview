#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode 53: 最大子数组和
 * 
 * 问题描述：
 * 给定一个整数数组 nums，找到一个具有最大和的连续子数组
 * 返回其最大和
 * 
 * 示例：
 * 输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
 * 输出：6
 * 解释：连续子数组 [4,-1,2,1] 的和最大，为 6
 * 
 * 核心思路：
 * - Kadane算法（动态规划）
 * - 核心思想：如果累积和为负，不如重新开始
 * - 状态转移：dp[i] = max(nums[i], dp[i-1] + nums[i])
 */

/*
 * Kadane算法：求最大子数组和
 * @param nums: 整数数组
 * @param numsSize: 数组大小
 * @return: 最大子数组和
 * 
 * 算法核心：
 * - 维护当前子数组和 currentSum
 * - 如果 currentSum < 0，从当前位置重新开始
 * - 否则，累加当前元素
 * - 随时更新全局最大值
 * 
 * 为什么 currentSum < 0 时重新开始？
 * - 负数和加上任何数都会变小
 * - 不如从下一个数重新开始
 * 
 * 时间复杂度: O(n)
 * 空间复杂度: O(1)
 */
int maxSubArray(int* nums, int numsSize) {

	int maxSum, currentSum;
	int maxStart, maxEnd, currentStart;
	int i, currentNum;

	/* 边界检查 */
	if (nums == NULL || numsSize == 0) {
		return 0;
	}

	/* 初始化：第一个元素 */
	maxSum = nums[0];
	currentSum = nums[0];
	currentNum = nums[0];
	currentStart = 0;
	maxStart = 0;
	maxEnd = 0;

	/* Kadane算法主循环 */
	for (i = 1; i < numsSize; i++) {
		currentNum = nums[i];
		
		/* 决策：重新开始 vs 继续累加 */
		if (currentSum + currentNum < currentNum) {  /* 等价于 currentSum < 0 */
			currentSum = currentNum;  /* 重新开始 */
			currentStart = i;
		} else {
			currentSum += currentNum;  /* 继续累加 */
		}

		/* 更新全局最大值 */
		if (currentSum > maxSum) {
			maxSum = currentSum;
			maxStart = currentStart;
			maxEnd = i;
		}
	}

	return maxSum;
}

/*
 * ==================== 算法详解 ====================
 * 
 * Kadane算法（动态规划）
 * 
 * 核心思想：
 * - 维护到当前位置的最大子数组和
 * - 每个位置有两个选择：
 *   1. 加入前面的子数组：dp[i-1] + nums[i]
 *   2. 单独作为新子数组：nums[i]
 * - 取较大者
 * 
 * 状态转移方程：
 * dp[i] = max(nums[i], dp[i-1] + nums[i])
 * 
 * 优化：
 * - 不需要dp数组，用一个变量currentSum
 * - 空间复杂度从O(n)降到O(1)
 * 
 * 示例：nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 * 
 * i=0: currentSum=-2, maxSum=-2
 * i=1: -2+1=-1 < 1, 重新开始, currentSum=1, maxSum=1
 * i=2: 1+(-3)=-2, currentSum=-2
 * i=3: -2+4=2 < 4, 重新开始, currentSum=4, maxSum=4
 * i=4: 4+(-1)=3, currentSum=3
 * i=5: 3+2=5, currentSum=5, maxSum=5
 * i=6: 5+1=6, currentSum=6, maxSum=6
 * i=7: 6+(-5)=1, currentSum=1
 * i=8: 1+4=5, currentSum=5
 * 
 * 结果：maxSum=6，对应子数组[4,-1,2,1]
 */