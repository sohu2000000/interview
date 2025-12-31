#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Dynamic programming with memoization to calculate max money
 * 
 * @param nums: array of money in each house
 * @param numsSize: number of houses
 * @param position: current house position
 * @param memo: memoization array to avoid recomputation
 * @return: maximum money that can be robbed from position 0 to position
 */
int robHelper(int* nums, int numsSize, int position, int *memo) {
	int maxMoney;

	// Base case: no houses
	if (position == -1)
		return 0;
	
	// Base case: only first house
	if (position == 0)
		return nums[0];
	
	// If already computed, return cached result
	if (memo[position] != -1)
		return memo[position];
	
	// Recurrence relation:
	// Either skip current house or rob it (can't rob adjacent)
	maxMoney = MAX(robHelper(nums, numsSize, position - 1, memo),
	              robHelper(nums, numsSize, position - 2, memo) + nums[position]);
	
	// Cache the result
	memo[position] = maxMoney;
	return maxMoney;
}

/**
 * LeetCode 198: House Robber
 * 
 * You are a robber planning to rob houses along a street.
 * Each house has a certain amount of money stashed.
 * All houses are arranged in a row.
 * Adjacent houses have security systems connected, so you cannot rob two adjacent houses.
 * 
 * Given an array representing the amount of money of each house,
 * determine the maximum amount of money you can rob without alerting the police.
 * 
 * Algorithm: Dynamic Programming with Memoization (Top-Down)
 * - State: dp[i] = max money that can be robbed from houses 0 to i
 * - Recurrence: dp[i] = max(dp[i-1], dp[i-2] + nums[i])
 *   - dp[i-1]: skip house i
 *   - dp[i-2] + nums[i]: rob house i (can't rob i-1)
 * - Memoization: cache computed results to avoid O(2^n) recomputation
 * 
 * Time Complexity: O(n) - each position computed once
 * Space Complexity: O(n) - memo array + recursion stack
 * 
 * Example:
 * Input: nums = [2,7,9,3,1]
 * Output: 12 (rob house 0, 2, 4 → 2+9+1=12)
 */
int rob(int* nums, int numsSize) {
	int *memo, maxMoney;

	// Handle edge case
	if (numsSize == 0)
		return 0;

	// Allocate and initialize memoization array
	memo = (int *)malloc(numsSize * sizeof(int));
	memset(memo, -1, numsSize * sizeof(int));
	
	// Compute result with memoization
	maxMoney = robHelper(nums, numsSize, numsSize - 1, memo);
	
	// Free memory
	free(memo);

	return maxMoney;
}