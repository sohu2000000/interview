/*
Given an array of strings strs, group the anagrams together. You can return the answer in any order.

 

Example 1:

Input: strs = ["eat","tea","tan","ate","nat","bat"]

Output: [["bat"],["nat","tan"],["ate","eat","tea"]]

Explanation:

There is no string in strs that can be rearranged to form "bat".
The strings "nat" and "tan" are anagrams as they can be rearranged to form each other.
The strings "ate", "eat", and "tea" are anagrams as they can be rearranged to form each other.
Example 2:

Input: strs = [""]

Output: [[""]]

Example 3:

Input: strs = ["a"]

Output: [["a"]]

 

Constraints:

1 <= strs.length <= 104
0 <= strs[i].length <= 100
strs[i] consists of lowercase English letters.
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 * 
 * 字母异位词分组 - 哈希表法
 * 
 * 核心思想：
 * - 将每个字符串排序后作为键（异位词排序后相同）
 * - 用哈希表（uthash）存储每个键对应的字符串组
 * - 最后将所有组转换为结果数组
 * 
 * 时间复杂度：O(n×k log k) - n个字符串，每个长度k需要排序
 * 空间复杂度：O(n×k) - 存储所有字符串
 */

// 异位词组结构
typedef struct AnagramGroup {
	char sortedKey[101];     // 排序后的字符串作为键
	char **strings;          // 该组的字符串数组
	int count;               // 当前字符串数量
	int capacity;            // 数组容量
	UT_hash_handle hh;       // uthash句柄
} AnagramGroup;

// 字符比较函数（用于qsort）
int compareChar(const void *a, const void *b) {
	return (*(char *)a - *(char *)b);
}

/**
 * 生成字符串的键：将字符串排序
 * 异位词排序后会得到相同的键
 */
void generateSortedKey(char *str, char *sortedKey) {
	strcpy(sortedKey, str);
	qsort(sortedKey, strlen(sortedKey), sizeof(char), compareChar);
}

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
	AnagramGroup *groupTable = NULL;  // 哈希表（所有组）
	AnagramGroup *currentGroup;       // 当前处理的组
	
	// ===== 步骤1：将字符串分组 =====
	for (int i = 0; i < strsSize; i++) {
		char sortedKey[101];
		generateSortedKey(strs[i], sortedKey);

		// 在哈希表中查找该键
		HASH_FIND_STR(groupTable, sortedKey, currentGroup);
		
		if (currentGroup == NULL) {
			// 新建一个组
			currentGroup = (AnagramGroup *)malloc(sizeof(AnagramGroup));
			strcpy(currentGroup->sortedKey, sortedKey);
			currentGroup->count = 0;
			currentGroup->capacity = 10;
			currentGroup->strings = (char **)malloc(currentGroup->capacity * sizeof(char *));
			HASH_ADD_STR(groupTable, sortedKey, currentGroup);
		}
		
		// 如果容量不够，扩容
		if (currentGroup->count == currentGroup->capacity) {
			currentGroup->capacity *= 2;
			currentGroup->strings = (char **)realloc(currentGroup->strings, 
			                                          currentGroup->capacity * sizeof(char *));
		}
		
		// 将字符串添加到该组
		currentGroup->strings[currentGroup->count++] = strs[i];
	}

	// ===== 步骤2：将哈希表转换为结果数组 =====
	int groupCount = HASH_COUNT(groupTable);
	char ***result = (char ***)malloc(groupCount * sizeof(char **));
	int resultIndex = 0;
	*returnColumnSizes = (int *)malloc(groupCount * sizeof(int));

	// 遍历哈希表中的每个组
	for (currentGroup = groupTable; currentGroup != NULL; currentGroup = currentGroup->hh.next) {
		result[resultIndex] = (char **)malloc(currentGroup->count * sizeof(char *));
		memcpy(result[resultIndex], currentGroup->strings, 
		       currentGroup->count * sizeof(char *));
		(*returnColumnSizes)[resultIndex] = currentGroup->count;
		resultIndex++;
	}

	*returnSize = groupCount;
	return result;
}