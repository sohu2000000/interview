/*
Given a string s, find the length of the longest substring without duplicate characters.

 

Example 1:

Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3. Note that "bca" and "cab" are also correct answers.
Example 2:

Input: s = "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.
Example 3:

Input: s = "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3.
Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.
 

Constraints:

0 <= s.length <= 5 * 104
s consists of English letters, digits, symbols and spaces.
 */

#define HASH_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * 最长无重复子串 - 滑动窗口 + 哈希表
 * 
 * 核心思想：
 * - 维护窗口 [left, right]，保证窗口内无重复字符
 * - 用哈希表记录每个字符最后出现的位置
 * - 遇到重复字符时，移动left到重复字符上次位置的下一个
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1) - 哈希表大小固定256
 */
int lengthOfLongestSubstring(char* s) {
	int charLastPos[HASH_SIZE];  // 记录每个字符最后出现的位置
	int left = 0;                // 窗口左边界
	int right = 0;               // 窗口右边界
	int maxLen = 0;              // 最长长度

	if (NULL == s)
		return 0;

	// 初始化：-1表示字符未出现过
	memset(charLastPos, -1, sizeof(charLastPos));
	
	while (s[right] != '\0') {
		unsigned char currentChar = (unsigned char)s[right];
		
		// 如果当前字符之前出现过，且在当前窗口内
		if (charLastPos[currentChar] >= left) {
			// 移动left到重复字符上次位置的下一个
			left = charLastPos[currentChar] + 1;
		}

		// 更新最长长度
		maxLen = MAX(maxLen, right - left + 1);
		
		// 记录当前字符的位置
		charLastPos[currentChar] = right;
		
		// 移动右边界
		right++;
	}

	return maxLen;
}