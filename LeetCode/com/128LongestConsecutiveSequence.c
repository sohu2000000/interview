/*
Given an unsorted array of integers nums, return the length of the longest consecutive elements sequence.

You must write an algorithm that runs in O(n) time.

 

Example 1:

Input: nums = [100,4,200,1,3,2]
Output: 4
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.
Example 2:

Input: nums = [0,3,7,2,5,8,4,6,0,1]
Output: 9
Example 3:

Input: nums = [1,0,1,2]
Output: 3
 

Constraints:

0 <= nums.length <= 105
-109 <= nums[i] <= 109
 */

/**
 * 最长连续序列 - 哈希集合法
 * 
 * 核心思想：
 * - 用哈希集合存储所有数字（O(1)查找）
 * - 只从序列起点开始向后查找
 * - 起点判断：num-1不存在，则num是起点
 * 
 * 关键优化：
 * 避免重复计算 - 如果num-1存在，说明num不是起点，跳过
 * 
 * 时间复杂度：O(n) - 每个数字最多被访问2次
 * 空间复杂度：O(n) - 哈希集合
 */

// 哈希集合节点
typedef struct HashNode {
	int value;          // 存储的数字
	UT_hash_handle hh;  // uthash句柄
} HashNode;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int longestConsecutive(int* nums, int numsSize) {
	HashNode *hashSet = NULL;    // 哈希集合
	HashNode *currentNode;       // 当前节点
	HashNode *foundNode;         // 查找结果
	int i, maxLength = 0;

	if (NULL == nums || numsSize == 0)
		return 0;

	// ===== 步骤1：将所有数字加入哈希集合（去重）=====
	for (i = 0; i < numsSize; i++) {
		int key = nums[i];
		HASH_FIND_INT(hashSet, &key, currentNode);
		if (currentNode == NULL) {
			currentNode = (HashNode *)malloc(sizeof(HashNode));
			currentNode->value = nums[i];
			HASH_ADD_INT(hashSet, value, currentNode);
		}
	}

	// ===== 步骤2：遍历哈希集合，找最长连续序列 =====
	for (currentNode = hashSet; currentNode != NULL; currentNode = currentNode->hh.next) {
		int currentNum = currentNode->value;
		int prevNum = currentNum - 1;
		
		// 关键优化：只从序列的起点开始计数
		// 如果currentNum-1存在，说明currentNum不是起点，跳过
		HASH_FIND_INT(hashSet, &prevNum, foundNode);
		if (foundNode != NULL) {
			continue;  // 不是起点，跳过
		}
		
		// currentNum是起点，向后查找连续序列
		int sequenceLength = 1;
		int nextNum = currentNum + 1;
		HASH_FIND_INT(hashSet, &nextNum, foundNode);
		
		while (foundNode != NULL) {
			sequenceLength++;
			nextNum++;
			HASH_FIND_INT(hashSet, &nextNum, foundNode);
		}
		
		// 更新最长长度
		maxLength = MAX(maxLength, sequenceLength);
	}

	return maxLength;
}