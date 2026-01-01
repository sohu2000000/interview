#include <stdlib.h>

#define INF_MAX 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * LeetCode 120: Triangle
 * 
 * Given a triangle array, return the minimum path sum from top to bottom.
 * For each step, you may move to an adjacent number of the row below.
 * More formally, if you are on index i on the current row,
 * you may move to either index i or index i+1 on the next row.
 * 
 * Algorithm: Dynamic Programming
 * - State: dp[i][j] = minimum path sum to reach position (i,j)
 * - Transition:
 *   - First column (j=0): can only come from dp[i-1][0]
 *   - Last column (j=last): can only come from dp[i-1][j-1]
 *   - Middle: can come from dp[i-1][j-1] or dp[i-1][j], choose minimum
 * - Base case: dp[0][0] = triangle[0][0]
 * - Answer: min(dp[lastRow][j]) for all j
 * 
 * Time Complexity: O(n²) where n is number of rows
 * Space Complexity: O(n²) for dp array (can be optimized to O(n))
 * 
 * Example:
 * Input: triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
 * Output: 11 (path: 2→3→5→1)
 */
int minimumTotal(int** triangle, int triangleSize, int* triangleColSize) {
	int i, j, minPathSum, colSize;
	int **dp;

	// Allocate and initialize 2D dp array
	dp = (int **)malloc(triangleSize * sizeof(int *));
	for (i = 0; i < triangleSize; i++) {
		colSize = triangleColSize[i];
		dp[i] = (int *)malloc(colSize * sizeof(int));
		for (j = 0; j < colSize; j++)
			dp[i][j] = INF_MAX;
	}

	// Base case: starting point
	dp[0][0] = triangle[0][0];

	// Fill dp array row by row (starting from row 1)
	for (i = 1; i < triangleSize; i++) {
		colSize = triangleColSize[i];
		
		for (j = 0; j < colSize; j++) {
			// First column: can only come from dp[i-1][0]
			if (j == 0)
				dp[i][j] = dp[i - 1][j] + triangle[i][j];
			
			// Last column: can only come from dp[i-1][j-1]
			else if (j == colSize - 1)
				dp[i][j] = dp[i - 1][j - 1] + triangle[i][j];
			
			// Middle: choose minimum from two possible paths
			else
				dp[i][j] = MIN(dp[i - 1][j - 1] + triangle[i][j],
				              dp[i - 1][j] + triangle[i][j]);
		}
	}

	// Find minimum in the last row
	minPathSum = INF_MAX;
	colSize = triangleColSize[triangleSize - 1];
	for (j = 0; j < colSize; j++)
		minPathSum = MIN(minPathSum, dp[triangleSize - 1][j]);
	
	// Free memory
	for (i = 0; i < triangleSize; i++)
		free(dp[i]);
	free(dp);

	return minPathSum;
}