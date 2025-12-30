#include <stdlib.h>
#include <string.h>

/**
 * LeetCode 70: Climbing Stairs
 * 
 * You are climbing a staircase. It takes n steps to reach the top.
 * Each time you can either climb 1 or 2 steps.
 * In how many distinct ways can you climb to the top?
 * 
 * Algorithm: Dynamic Programming (Fibonacci-like)
 * - State: dp[i] = number of ways to reach step i
 * - Transition: dp[i] = dp[i-1] + dp[i-2]
 *   - From step i-1, climb 1 step to reach i
 *   - From step i-2, climb 2 steps to reach i
 * - Base case: dp[0] = 1, dp[1] = 1
 * 
 * Time Complexity: O(n) - iterate through all steps
 * Space Complexity: O(n) - dp array of size n+1
 * 
 * Example:
 * Input: n = 3
 * Output: 3 (1+1+1, 1+2, 2+1)
 */
int climbStairs(int n) {
	int i, result, *dp;

	// Handle edge cases
	if (n == 0) return 1;
	if (n == 1) return 1;

	// Initialize dp array: dp[i] = ways to reach step i
	dp = (int *)malloc((n + 1) * sizeof(int));
	memset(dp, 0, (n + 1) * sizeof(int));
	
	// Base cases
	dp[0] = 1;  // One way to stay at ground (do nothing)
	dp[1] = 1;  // One way to reach step 1 (climb 1 step)

	// Fill dp array: calculate ways for each step
	for (i = 2; i <= n; i++) {
		// Transition equation: dp[i] = dp[i-1] + dp[i-2]
		// Can reach step i from step i-1 (climb 1) or step i-2 (climb 2)
		dp[i] = dp[i - 1] + dp[i - 2];
	}

	result = dp[n];
	free(dp);

	return result;
}