#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode 918: 环形子数组的最大和
 * 
 * 问题描述：
 * 给定一个环形整数数组 nums，找到其最大的子数组和
 * 环形数组：数组的末尾连接到开头
 * 
 * 示例：
 * 输入：nums = [1,-2,3,-2]
 * 输出：3
 * 解释：子数组 [3] 有最大和 3
 * 
 * 输入：nums = [5,-3,5]
 * 输出：10
 * 解释：子数组 [5,5]（跨越首尾）有最大和 10
 * 
 * 核心思路：
 * - 情况1：最大子数组在中间（不跨越首尾）→ Kadane算法
 * - 情况2：最大子数组跨越首尾 → total_sum - 最小子数组和
 * - 取两种情况的最大值
 */

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
 * 环形子数组的最大和
 * @param nums: 整数数组
 * @param numsSize: 数组大小
 * @return: 最大子数组和
 * 
 * 算法核心：
 * - 同时维护最大子数组和 & 最小子数组和
 * - 情况1：最大在中间 → maxSubarraySum
 * - 情况2：最大跨越首尾 → totalSum - minSubarraySum
 * - 特殊情况：所有数都是负数 → 返回maxSubarraySum
 * 
 * 时间复杂度: O(n)
 * 空间复杂度: O(1)
 */
int maxSubarraySumCircular(int* nums, int numsSize) {
	int maxSubarraySum, minSubarraySum, totalSum;
	int maxStart, maxEnd, minStart, minEnd;
	int i, currentNum, maxStartTemp, minStartTemp, currentMinSum, currentMaxSum;

	/* 边界检查 */
	if (nums == NULL || numsSize == 0)
		return 0;
	
	/* 初始化 */
	maxSubarraySum = nums[0];
	currentMaxSum = nums[0];
	currentMinSum = nums[0];
	totalSum = nums[0];
	minSubarraySum = nums[0];
	currentNum = nums[0];
	maxStartTemp = 0;
	minStartTemp = 0;
	maxStart = 0;
	minStart = 0;
	maxEnd = 0;
	minEnd = 0;

	/* Kadane算法同时找最大和最小子数组和 */
	for (i = 1; i < numsSize; i++) {
		totalSum += nums[i];
		currentNum = nums[i];

		/* Kadane算法找最大子数组和 */
		if (currentMaxSum + currentNum < currentNum) {  /* 等价于 currentMaxSum < 0 */
			currentMaxSum = currentNum;
			maxStartTemp = i;
		} else {
			currentMaxSum += currentNum;
		}

		/* Kadane算法找最小子数组和（反向） */
		if (currentMinSum + currentNum > currentNum) {  /* 等价于 currentMinSum > 0 */
			currentMinSum = currentNum;
			minStartTemp = i;
		} else {
			currentMinSum += currentNum;
		}

		/* 更新全局最大子数组和 */
		if (currentMaxSum > maxSubarraySum) {
			maxSubarraySum = currentMaxSum;
			maxStart = maxStartTemp;
			maxEnd = i;
		}

		/* 更新全局最小子数组和 */
		if (currentMinSum < minSubarraySum) {
			minSubarraySum = currentMinSum;
			minStart = minStartTemp;
			minEnd = i;
		}
	}

	/* 特殊情况：所有数都是负数，最小子数组 = 整个数组 */
	if (totalSum == minSubarraySum)
		return maxSubarraySum;
	
	/* 返回两种情况的最大值 */
	return MAX(maxSubarraySum, totalSum - minSubarraySum);
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：Kadane算法变种
 * 
 * 环形数组的最大子数组和有两种情况：
 * 
 * 情况1：最大子数组在中间（不跨越首尾）
 * [x x x [====MAX====] x x x]
 * → 用Kadane算法求maxSubarraySum
 * 
 * 情况2：最大子数组跨越首尾
 * [===] x x x [====MIN====] x x x [===]
 * → totalSum - minSubarraySum
 * 
 * 为什么？
 * - 跨越首尾的最大和 = 总和 - 中间的最小和
 * - 相当于去掉中间最小的部分
 * 
 * 示例：nums = [5, -3, 5]
 * 
 * 情况1：maxSubarraySum = 5（单个5）
 * 情况2：
 *   totalSum = 7
 *   minSubarraySum = -3
 *   跨越首尾的和 = 7 - (-3) = 10 ✓
 * 
 * 结果：MAX(5, 10) = 10
 * 
 * 特殊情况：nums = [-3, -2, -1]
 * - minSubarraySum = -6 = totalSum
 * - 如果用情况2：-6 - (-6) = 0（空子数组，不合法）
 * - 返回maxSubarraySum = -1 ✓
 */