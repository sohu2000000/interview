/*
Given an array of positive integers nums and a positive integer target, return the minimal length of a subarray whose sum is greater than or equal to target. If there is no such subarray, return 0 instead.

 

Example 1:

Input: target = 7, nums = [2,3,1,2,4,3]
Output: 2
Explanation: The subarray [4,3] has the minimal length under the problem constraint.
Example 2:

Input: target = 4, nums = [1,4,4]
Output: 1
Example 3:

Input: target = 11, nums = [1,1,1,1,1,1,1,1]
Output: 0
 

Constraints:

1 <= target <= 109
1 <= nums.length <= 105
1 <= nums[i] <= 104
 

Follow up: If you have figured out the O(n) solution, try coding another solution of which the time complexity is O(n log(n)).
 */

#define INT_MAX 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * 最小子数组长度 - 滑动窗口法
 * 
 * 核心思想：
 * - 维护一个窗口 [left, right]
 * - 扩展right直到sum >= target
 * - 收缩left直到sum < target
 * - 记录最小窗口长度
 * 
 * 时间复杂度：O(n) - 每个元素最多被访问2次
 * 空间复杂度：O(1)
 */
int minSubArrayLen(int target, int* nums, int numsSize) {
	int left = 0;         // 窗口左边界
	int right = 0;        // 窗口右边界
	int windowSum = 0;    // 当前窗口的和
	int minLen = INT_MAX; // 最小长度

	if (NULL == nums || numsSize == 0)
		return 0;
	
	// 滑动窗口
	while (right < numsSize) {
		// 扩展窗口：加入right位置的元素
		windowSum += nums[right];
		
		// 当窗口和满足条件时，尝试收缩窗口
		while (windowSum >= target) {
			// 更新最小长度
			minLen = MIN(minLen, right - left + 1);
			
			// 收缩窗口：移除left位置的元素
			windowSum -= nums[left];
			left++;
		}
		
		// 扩展窗口：移动right指针
		right++;
	}

	return minLen == INT_MAX ? 0 : minLen;
}