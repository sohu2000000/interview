#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * LeetCode 97: Interleaving String
 * 
 * Given strings s1, s2, and s3, find whether s3 is formed by an interleaving of s1 and s2.
 * An interleaving is formed by merging s1 and s2 while maintaining the relative order
 * of characters from each string.
 * 
 * Algorithm: 2D Dynamic Programming
 * - State: dp[i][j] = whether s1[0..i-1] and s2[0..j-1] can interleave to form s3[0..i+j-1]
 * - Transition:
 *   - i=0: only use s2, dp[0][j] = dp[0][j-1] if s2[j-1] == s3[j-1]
 *   - j=0: only use s1, dp[i][0] = dp[i-1][0] if s1[i-1] == s3[i-1]
 *   - i>0,j>0: dp[i][j] = (dp[i-1][j] && s1[i-1]==s3[i+j-1]) || 
 *                         (dp[i][j-1] && s2[j-1]==s3[i+j-1])
 * - Base case: dp[0][0] = true (empty strings interleave to empty)
 * 
 * Key Insight:
 * - Must use else if (i==0) not else if (i==0 && char_match)
 * - Because if i==0 but char doesn't match, shouldn't fall into else block
 * 
 * Time Complexity: O(m×n) where m=len(s1), n=len(s2)
 * Space Complexity: O(m×n) for dp table
 * 
 * Example:
 * Input: s1="aabcc", s2="dbbca", s3="aadbbcbcac"
 * Output: true
 */
bool isInterleave(char* s1, char* s2, char* s3) {
	int s1Len, s2Len, s3Len;
	bool **dp, result;
	int i, j;

	// Handle NULL pointers
	if (s1 == NULL || s2 == NULL || s3 == NULL)
		return false;

	s1Len = strlen(s1);
	s2Len = strlen(s2);
	s3Len = strlen(s3);

	// Early check: length must match
	if (s1Len + s2Len != s3Len)
		return false;
	
	// Allocate and initialize dp table
	dp = (bool **)malloc((s1Len + 1) * sizeof(bool *));
	for (i = 0; i <= s1Len; i++) {
		dp[i] = (bool *)malloc((s2Len + 1) * sizeof(bool));
		for (j = 0; j <= s2Len; j++) {
			dp[i][j] = false;
		}
	}

	// Base case: empty strings
	dp[0][0] = true;
	
	// Fill dp table
	for (i = 0; i <= s1Len; i++) {
		for (j = 0; j <= s2Len; j++) {
			// Skip starting point
			if (i == 0 && j == 0)
				continue;
			
			// First row: only use s2
			else if (i == 0) {
				if (s2[j - 1] == s3[i + j - 1])
					dp[i][j] = dp[i][j - 1];
			}
			// First column: only use s1
			else if (j == 0) {
				if (s1[i - 1] == s3[i + j - 1])
					dp[i][j] = dp[i - 1][j];
			}
			// Other cells: can use either s1 or s2
			else {
				if (s3[i + j - 1] == s1[i - 1] && dp[i - 1][j])
					dp[i][j] = true;
				else if (s3[i + j - 1] == s2[j - 1] && dp[i][j - 1])
					dp[i][j] = true;
				else
					dp[i][j] = false;
			}
		}
	}
	
	// Answer is at dp[s1Len][s2Len]
	result = dp[s1Len][s2Len];

	// Free memory
	for (i = 0; i <= s1Len; i++) {
		free(dp[i]);
	}
	free(dp);

	return result;
}