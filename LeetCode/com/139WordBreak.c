#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * LeetCode 139: Word Break
 * 
 * Given a string s and a dictionary of strings wordDict, return true if s can be
 * segmented into a space-separated sequence of one or more dictionary words.
 * Note: The same word in the dictionary may be reused multiple times.
 * 
 * Algorithm: Dynamic Programming
 * - State: dp[i] = true if s[0...i-1] can be segmented into dictionary words
 * - Transition: For each position i where dp[i] is true,
 *               try to match each word starting from position i
 *               If word matches, set dp[i + wordLen] = true
 * - Base case: dp[0] = true (empty string)
 * 
 * Key Insight:
 * - dp[i] corresponds to s[0...i-1] (first i characters)
 * - When dp[i] is true, next character to check is s[i] (not s[i-1])
 * - So we use s+i to match from position i
 * 
 * Time Complexity: O(n×m×k) where n=len(s), m=dictSize, k=avg word length
 * Space Complexity: O(n) for dp array
 * 
 * Example:
 * Input: s = "leetcode", wordDict = ["leet","code"]
 * Output: true (can be segmented as "leet code")
 */
bool wordBreak(char* s, char** wordDict, int wordDictSize) {
	int i, j, strLen, wordLen;
	char *currentWord;
	int *dp;
	bool result;

	strLen = strlen(s);

	// Allocate and initialize dp array
	dp = (int *)malloc((strLen + 1) * sizeof(int));
	memset(dp, 0, (strLen + 1) * sizeof(int));

	// Base case: empty string can be segmented
	dp[0] = 1;

	// For each position in string
	for (i = 0; i < strLen; i++) {
		// Skip if s[0...i-1] cannot be segmented
		if (dp[i] == 0)
			continue;
		
		// Try each word in dictionary
		for (j = 0; j < wordDictSize; j++) {
			currentWord = wordDict[j];
			wordLen = strlen(currentWord);
			
			// Skip if word extends beyond string length
			if (i + wordLen > strLen)
				continue;
			
			// If word matches starting from position i
			if (strncmp(s + i, currentWord, wordLen) == 0)
				dp[i + wordLen] = 1;  // s[0...i+wordLen-1] can be segmented
		}
	}

	// Check if entire string can be segmented
	result = (bool)dp[strLen];
	
	free(dp);
	return result;
}
