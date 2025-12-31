#include <stdlib.h>
#include <string.h>

#define INF_MAX 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * LeetCode 322: Coin Change
 * 
 * Given an array of coin denominations and a target amount,
 * return the fewest number of coins needed to make up that amount.
 * If that amount cannot be made up, return -1.
 * You may assume that you have an infinite number of each kind of coin.
 * 
 * Algorithm: Dynamic Programming (Complete Knapsack)
 * - State: dp[i] = minimum number of coins needed to make amount i
 * - Transition: dp[i] = min(dp[i], dp[i-coin] + 1) for each coin
 *   - Try using each coin if i >= coin
 *   - dp[i-coin] + 1 means: use one more coin based on solution for i-coin
 * - Base case: dp[0] = 0 (need 0 coins to make amount 0)
 * - Initial: dp[i] = INF_MAX (impossible initially)
 * 
 * Time Complexity: O(amount × coinsSize) - fill dp array
 * Space Complexity: O(amount) - dp array size
 * 
 * Example:
 * Input: coins = [1,2,5], amount = 11
 * Output: 3 (11 = 5+5+1)
 */
int coinChange(int* coins, int coinsSize, int amount) {
	int i, j, currentCoin, result;
	int *dp;

	// Allocate and initialize dp array
	dp = (int *)malloc((amount + 1) * sizeof(int));
	
	// Initialize all amounts as impossible (INF_MAX)
	for (i = 0; i <= amount; i++) 
		dp[i] = INF_MAX;

	// Base case: 0 coins needed to make amount 0
	dp[0] = 0;

	// For each amount from 1 to target amount
	for (i = 1; i <= amount; i++) {
		// Try each coin denomination
		for (j = 0; j < coinsSize; j++) {
			currentCoin = coins[j];
			
			// Skip if coin value exceeds current amount
			if (i - currentCoin < 0)
				continue;
			
			// Skip if dp[i-currentCoin] is unreachable
			if (dp[i - currentCoin] == INF_MAX)		
				continue;
			
			// Update: use this coin + solution for remaining amount
			dp[i] = MIN(dp[i], dp[i - currentCoin] + 1);
		}
	}

	// Return -1 if amount cannot be made, otherwise return coin count
	result = (dp[amount] == INF_MAX) ? -1 : dp[amount];
	free(dp);
	return result;
}