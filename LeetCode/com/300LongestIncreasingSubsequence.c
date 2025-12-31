#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * LeetCode 300: Longest Increasing Subsequence
 * 
 * Given an integer array nums, return the length of the longest strictly increasing subsequence.
 * A subsequence is derived from the array by deleting some or no elements without changing
 * the order of the remaining elements.
 * 
 * Algorithm: Dynamic Programming (O(n²) solution)
 * - State: dp[i] = length of longest increasing subsequence ending at nums[i]
 * - Transition: For each i, check all j < i:
 *               If nums[j] < nums[i], dp[i] = max(dp[i], dp[j] + 1)
 * - Base case: dp[i] = 1 (each element itself is a subsequence of length 1)
 * - Answer: max(dp[i]) for all i
 * 
 * Key Insight:
 * - dp[i] represents LIS *ending at* position i (must include nums[i])
 * - Final answer is the maximum among all dp values
 * 
 * Time Complexity: O(n²) - nested loops
 * Space Complexity: O(n) - dp array
 * 
 * Example:
 * Input: nums = [10,9,2,5,3,7,101,18]
 * Output: 4 (LIS is [2,5,7,101] or [2,3,7,101])
 */
int lengthOfLIS(int* nums, int numsSize) {
	int i, j, previousNum, maxLength;
	int *dp;

	// Handle edge cases
	if (nums == NULL || numsSize == 0)
		return 0;

	// Allocate and initialize dp array
	// dp[i] = length of LIS ending at nums[i]
	dp = (int *)malloc(numsSize * sizeof(int));
	for (i = 0; i < numsSize; i++)
		dp[i] = 1;  // Each element alone is a subsequence of length 1
	
	// For each position i, check all previous positions j
	for (i = 0; i < numsSize; i++) {
		// Try extending LIS from each previous position j
		for (j = 0; j < i; j++) {
			previousNum = nums[j];
			
			// Can only extend if strictly increasing
			if (previousNum >= nums[i])
				continue;
			
			// Extend LIS ending at j by including nums[i]
			dp[i] = MAX(dp[i], dp[j] + 1);
		}
	}

	// Find the maximum length among all ending positions
	maxLength = dp[0];
	for (i = 1; i < numsSize; i++)
		maxLength = MAX(maxLength, dp[i]);
	
	free(dp);
	return maxLength;
}