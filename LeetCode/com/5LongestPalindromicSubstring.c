#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Structure to track longest palindrome information
 */
typedef struct info {
	int start;    // Start index
	int end;      // End index
	int length;   // Length
} info_t;

/**
 * LeetCode 5: Longest Palindromic Substring
 * 
 * Given a string s, return the longest palindromic substring in s.
 * 
 * Algorithm: Dynamic Programming (O(n²) solution)
 * - State: dp[i][j] = true if substring s[i...j] is a palindrome
 * - Transition:
 *   - Length 1: dp[i][i] = true (single character)
 *   - Length 2: dp[i][i+1] = (s[i] == s[i+1])
 *   - Length ≥3: dp[i][j] = (s[i] == s[j]) && dp[i+1][j-1]
 * - Iterate by substring length: check all substrings of length 1, then 2, then 3, ...
 * - Track the longest palindrome found
 * 
 * Time Complexity: O(n²) - check all substrings
 * Space Complexity: O(n²) - dp table
 * 
 * Example:
 * Input: s = "babad"
 * Output: "bab" or "aba"
 */
char* longestPalindrome(char* s) {
	int i, j, str_len, sub_len;
	info_t max_info;
	char *result;
	bool **dp;

	// Handle edge cases
	if (NULL == s)
		return NULL;

	str_len = strlen(s);
	if (str_len == 1)
		return s;
	
	// Allocate and initialize dp table
	dp = (bool **)malloc(str_len * sizeof(bool *));
	for (i = 0; i < str_len; i++) {
		dp[i] = (bool *)malloc(str_len * sizeof(bool));
		for (j = 0; j < str_len; j++)
			dp[i][j] = true;
	}

	// Initialize
	dp[0][0] = true;
	max_info.start = 0;
	max_info.end = 0;
	max_info.length = 1;

	// Check all substrings by length (1, 2, 3, ..., str_len)
	for (sub_len = 1; sub_len <= str_len; sub_len++) {
		// Check all substrings of current length
		for (i = 0; i <= str_len - sub_len; i++) {
			j = i + sub_len - 1;  // End index
			
			// Check if s[i...j] is palindrome
			if (sub_len == 1)
				dp[i][j] = true;
			else if (sub_len == 2 && s[i] == s[j])
				dp[i][j] = true;
			else if (s[i] == s[j] && dp[i+1][j-1])
				dp[i][j] = true;
			else
				dp[i][j] = false;
	
			// Update longest palindrome if current is longer
			if (dp[i][j] && sub_len > max_info.length) {
				max_info.start = i;
				max_info.end = j;
				max_info.length = sub_len;
			}
		}
	}

	// Extract result substring
	result = (char *)malloc((max_info.length + 1) * sizeof(char));
	memcpy(result, s + max_info.start, max_info.length);
	result[max_info.length] = '\0';

	// Free dp table
	for (i = 0; i < str_len; i++)
		free(dp[i]);
	free(dp);

	return result;
}