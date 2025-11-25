/*
Given a 1-indexed array of integers numbers that is already sorted in non-decreasing order, find two numbers such that they add up to a specific target number. Let these two numbers be numbers[index1] and numbers[index2] where 1 <= index1 < index2 <= numbers.length.

Return the indices of the two numbers, index1 and index2, added by one as an integer array [index1, index2] of length 2.

The tests are generated such that there is exactly one solution. You may not use the same element twice.

Your solution must use only constant extra space.

 

Example 1:

Input: numbers = [2,7,11,15], target = 9
Output: [1,2]
Explanation: The sum of 2 and 7 is 9. Therefore, index1 = 1, index2 = 2. We return [1, 2].
Example 2:

Input: numbers = [2,3,4], target = 6
Output: [1,3]
Explanation: The sum of 2 and 4 is 6. Therefore index1 = 1, index2 = 3. We return [1, 3].
Example 3:

Input: numbers = [-1,0], target = -1
Output: [1,2]
Explanation: The sum of -1 and 0 is -1. Therefore index1 = 1, index2 = 2. We return [1, 2].
 

Constraints:

2 <= numbers.length <= 3 * 104
-1000 <= numbers[i] <= 1000
numbers is sorted in non-decreasing order.
-1000 <= target <= 1000
The tests are generated such that there is exactly one solution.
 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 * 
 * 双指针法（Two Pointers）
 * 
 * 核心思想：
 * - 利用数组已排序的特性
 * - 左指针从头开始，右指针从尾开始
 * - 如果和太大，右指针左移（减小和）
 * - 如果和太小，左指针右移（增大和）
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
	int left = 0;              // 左指针：从数组开头开始
	int right = numbersSize - 1;  // 右指针：从数组末尾开始
	int currentSum;               // 当前两数之和
	int* result = (int *)malloc(2 * sizeof(int));

	// 双指针向中间移动
	while (left < right) {
		currentSum = numbers[left] + numbers[right];

		if (currentSum == target) {
			// 找到答案
			break;
		} else if (currentSum > target) {
			// 和太大，右指针左移（减小和）
			right--;
		} else {
			// 和太小，左指针右移（增大和）
			left++;
	}
	}

	// 返回结果（题目要求1-indexed，所以+1）
	result[0] = left + 1;
	result[1] = right + 1;
	*returnSize = 2;
	
	return result;
}