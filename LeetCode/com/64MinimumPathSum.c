#include <stdlib.h>

#define INF_MAX 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * LeetCode 64: Minimum Path Sum
 * 
 * Given a m×n grid filled with non-negative numbers,
 * find a path from top left to bottom right which minimizes the sum of all numbers along its path.
 * Note: You can only move either down or right at any point in time.
 * 
 * Algorithm: Dynamic Programming
 * - State: dp[i][j] = minimum path sum to reach cell (i,j)
 * - Transition:
 *   - First row (i=0): can only come from left → dp[0][j] = dp[0][j-1] + grid[0][j]
 *   - First column (j=0): can only come from top → dp[i][0] = dp[i-1][0] + grid[i][0]
 *   - Other cells: can come from left or top, choose minimum
 *     dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j]
 * - Base case: dp[0][0] = grid[0][0]
 * - Answer: dp[m-1][n-1] (bottom right corner)
 * 
 * Time Complexity: O(m×n) where m=rows, n=cols
 * Space Complexity: O(m×n) for dp array (can be optimized to O(n))
 * 
 * Example:
 * Input: grid = [[1,3,1],[1,5,1],[4,2,1]]
 * Output: 7 (path: 1→3→1→1→1)
 */
int minPathSum(int** grid, int gridSize, int* gridColSize) {
	int i, j, minSum, colSize;
	int **dp;

	// Allocate and initialize 2D dp array
	dp = (int **)malloc(gridSize * sizeof(int *));
	for (i = 0; i < gridSize; i++) {
		colSize = gridColSize[i];
		dp[i] = (int *)malloc(colSize * sizeof(int));
		for (j = 0; j < colSize; j++)
			dp[i][j] = INF_MAX;
	}

	// Base case: starting point
	dp[0][0] = grid[0][0];

	// Fill dp array
	for (i = 0; i < gridSize; i++) {
		colSize = gridColSize[i];
		
		for (j = 0; j < colSize; j++) {
			// Skip starting point (already initialized)
			if (i == 0 && j == 0)
				continue;
			
			// First row: can only come from left
			if (i == 0)
				dp[i][j] = dp[i][j - 1] + grid[i][j];
			
			// First column: can only come from top
			else if (j == 0)
				dp[i][j] = dp[i - 1][j] + grid[i][j];
			
			// Other cells: choose minimum from left or top
			else
				dp[i][j] = MIN(dp[i][j - 1] + grid[i][j], 
				              dp[i - 1][j] + grid[i][j]);
		}
	}

	// Answer is at bottom right corner
	colSize = gridColSize[gridSize - 1];
	minSum = dp[gridSize - 1][colSize - 1];

	// Free memory
	for (i = 0; i < gridSize; i++)
		free(dp[i]);
	free(dp);

	return minSum;
}