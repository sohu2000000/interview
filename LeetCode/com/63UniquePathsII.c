#include <stdlib.h>

/**
 * LeetCode 63: Unique Paths II
 * 
 * A robot is located at the top-left corner of a m×n grid.
 * The robot can only move either down or right at any point in time.
 * The robot is trying to reach the bottom-right corner.
 * Now consider if some obstacles are added to the grids. How many unique paths would there be?
 * An obstacle and space is marked as 1 and 0 respectively in the grid.
 * 
 * Algorithm: Dynamic Programming with Obstacle Handling
 * - State: dp[i][j] = number of unique paths to reach cell (i,j)
 * - Transition:
 *   - If cell has obstacle: dp[i][j] = 0
 *   - First row: dp[0][j] = dp[0][j-1] (can only come from left)
 *   - First column: dp[i][0] = dp[i-1][0] (can only come from top)
 *   - Other cells: dp[i][j] = dp[i-1][j] + dp[i][j-1]
 * - Base case: dp[0][0] = 1 (if no obstacle)
 * - Early termination: return 0 if start or end has obstacle
 * 
 * Time Complexity: O(m×n)
 * Space Complexity: O(m×n) for dp array (can be optimized to O(n))
 * 
 * Example:
 * Input: obstacleGrid = [[0,0,0],[0,1,0],[0,0,0]]
 * Output: 2 (two paths avoiding the obstacle)
 */
int uniquePathsWithObstacles(int** obstacleGrid, int obstacleGridSize, int* obstacleGridColSize) {
	int i, j, colSize, numPaths;
	int **dp;

	// Early termination: check if start or end has obstacle
	if (obstacleGrid[0][0] == 1)
		return 0;
	
	colSize = obstacleGridColSize[obstacleGridSize - 1];
	if (obstacleGrid[obstacleGridSize - 1][colSize - 1] == 1)
		return 0;

	// Allocate and initialize dp array
	dp = (int **)malloc(obstacleGridSize * sizeof(int *));
	for (i = 0; i < obstacleGridSize; i++) {
		colSize = obstacleGridColSize[i];
		dp[i] = (int *)malloc(colSize * sizeof(int));
		for (j = 0; j < colSize; j++)
			dp[i][j] = 0;
	}

	// Base case: starting point
	dp[0][0] = 1;

	// Fill dp array
	for (i = 0; i < obstacleGridSize; i++) {
		colSize = obstacleGridColSize[i];
		
		for (j = 0; j < colSize; j++) {
			// Skip starting point
			if (i == 0 && j == 0)
				continue;
			
			// If obstacle, no paths through this cell
			if (obstacleGrid[i][j] == 1)
				dp[i][j] = 0;
			else {
				// First row: only from left
				if (i == 0)
					dp[i][j] = dp[i][j-1];
				
				// First column: only from top
				else if (j == 0)
					dp[i][j] = dp[i-1][j];
				
				// Other cells: sum of paths from top and left
				else
					dp[i][j] = dp[i-1][j] + dp[i][j-1];
			}
		}
	}

	// Answer is at bottom right corner
	colSize = obstacleGridColSize[obstacleGridSize - 1];
	numPaths = dp[obstacleGridSize - 1][colSize - 1];

	// Free memory
	for (i = 0; i < obstacleGridSize; i++)
		free(dp[i]);
	free(dp);

	return numPaths;
}