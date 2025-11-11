/*
Given an integer array nums, return an array answer such that answer[i] is equal to the product of all the elements of nums except nums[i].

The product of any prefix or suffix of nums is guaranteed to fit in a 32-bit integer.

You must write an algorithm that runs in O(n) time and without using the division operation.

 

Example 1:

Input: nums = [1,2,3,4]
Output: [24,12,8,6]
Example 2:

Input: nums = [-1,1,0,-3,3]
Output: [0,0,9,0,0]
 

Constraints:

2 <= nums.length <= 105
-30 <= nums[i] <= 30
The input is generated such that answer[i] is guaranteed to fit in a 32-bit integer.
 

Follow up: Can you solve the problem in O(1) extra space complexity? (The output array does not count as extra space for space complexity analysis.)

 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
	int i, left, right;
	int *answer;
    
	answer = (int *)malloc(sizeof(int) * numsSize);
	if (!answer) {
	    *returnSize = 0;
	    return NULL;
	}
    
	*returnSize = numsSize;
	
	// 第一遍：从左到右，累积左侧所有元素的乘积
	left = 1;
	for(i = 0; i < numsSize; i++) {
	    answer[i] = left;        // answer[i] 先存储左侧所有元素的乘积
	    left = left * nums[i];   // 修复：应该乘以 nums[i]，不是 answer[i]
	}
	
	// 第二遍：从右到左，累积右侧所有元素的乘积，并与左侧乘积相乘
	right = 1;
	for(i = numsSize - 1; i >= 0; i--){
	    answer[i] = right * answer[i];  // 左侧乘积 * 右侧乘积
	    right = right * nums[i];        // 修复：应该乘以 nums[i]，不是 answer[i]
	}
    
	return answer;
}