/*
Given an input string s, reverse the order of the words.

A word is defined as a sequence of non-space characters. The words in s will be separated by at least one space.

Return a string of the words in reverse order concatenated by a single space.

Note that s may contain leading or trailing spaces or multiple spaces between two words. The returned string should only have a single space separating the words. Do not include any extra spaces.

 

Example 1:

Input: s = "the sky is blue"
Output: "blue is sky the"
Example 2:

Input: s = "  hello world  "
Output: "world hello"
Explanation: Your reversed string should not contain leading or trailing spaces.
Example 3:

Input: s = "a good   example"
Output: "example good a"
Explanation: You need to reduce multiple spaces between two words to a single space in the reversed string.
 

Constraints:

1 <= s.length <= 104
s contains English letters (upper-case and lower-case), digits, and spaces ' '.
There is at least one word in s.
 

Follow-up: If the string data type is mutable in your language, can you solve it in-place with O(1) extra space?
 */

#include <stdlib.h>
#include <string.h>

/**
 * 辅助函数：反转字符串的指定区间 [start, end]
 */
void reverseString(char *start, char *end) {
	char temp;
	while (start < end) {
		temp = *start;
		*start = *end;
		*end = temp;
		start++;
		end--;
	}
}

/**
 * 核心思想：反转两次
 * 
 * 步骤1：反转整个字符串
 * "the sky is blue" → "eulb si yks eht"
 * 
 * 步骤2：反转每个单词，同时压缩空格
 * "eulb si yks eht" → "blue is sky the"
 * 
 * 关键技巧：用双指针（读指针+写指针）处理空格
 * - readPos: 读取原字符
 * - writePos: 写入有效字符（压缩后的位置）
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(n) - 需要复制字符串
 */
/**
 * 核心思想：反转两次 + 双指针压缩空格
 * 
 * 步骤1：反转整个字符串
 * 步骤2：用双指针反转每个单词并压缩空格
 * 
 * 关键：
 * - read: 读指针，扫描所有字符
 * - write: 写指针，只写有效内容
 * - 利用已有空格，在单词后保留一个
 */
char* reverseWords(char* s) {
	int len = strlen(s);
	
	// 复制到新字符串（因为输入不可修改）
	char* result = (char*)malloc((len + 1) * sizeof(char));
	strcpy(result, s);
	
	// ===== 步骤1：反转整个字符串 =====
	reverseString(result, result + len - 1);
	
	// ===== 步骤2：反转每个单词，同时压缩空格 =====
	char* read = result;   // 读指针
	char* write = result;  // 写指针
	
	while (*read != '\0') {
		// 跳过前导空格
		while (*read == ' ') {
			read++;
		}
		
		// 如果读到末尾，退出
		if (*read == '\0') break;
		
		// 如果不是第一个单词，利用已有的空格（保留一个）
		if (write != result) {
			*write++ = ' '; 
		}
		
		// 记录单词起始位置
		char* wordStart = write;
		
		// 复制单词（边读边写，可能覆盖原位置）
		while (*read != ' ' && *read != '\0') {
			*write++ = *read++;
		}
		
		// 反转这个单词
		reverseString(wordStart, write - 1);
	}
	
	// 添加字符串结束符
	*write = '\0';
	
	return result;
}
