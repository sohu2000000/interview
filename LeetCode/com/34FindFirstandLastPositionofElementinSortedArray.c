#include <stdlib.h>

/**
 * Find the rightmost (last) position of target using binary search
 * When target is found, continue searching in the right half
 * Time: O(log n), Space: O(1)
 */
int findEndPosition(int* nums, int numsSize, int target) {
	int left, right, mid, endPosition;

	left = 0;
	right = numsSize - 1;
	endPosition = -1;

	while (left <= right) {
		mid = left + (right - left) / 2;  // Avoid overflow

		if (nums[mid] == target) {
			endPosition = mid;
			left = mid + 1;  // Continue searching right
		} else if (nums[mid] < target) {
			left = mid + 1;
		} else {  // nums[mid] > target
			right = mid - 1;
		}
	}

	return endPosition;
}

/**
 * Find the leftmost (first) position of target using binary search
 * When target is found, continue searching in the left half
 * Time: O(log n), Space: O(1)
 */
int findStartPosition(int* nums, int numsSize, int target) {
	int left, right, mid, startPosition;

	left = 0;
	right = numsSize - 1;
	startPosition = -1;
	
	while (left <= right) {
		mid = left + (right - left) / 2;  // Avoid overflow

		if (nums[mid] == target) {
			startPosition = mid;
			right = mid - 1;  // Continue searching left
		} else if (nums[mid] < target) {
			left = mid + 1;
		} else {  // nums[mid] > target
			right = mid - 1;
		}
	}

	return startPosition;
}

/**
 * LeetCode 34: Find First and Last Position of Element in Sorted Array
 * 
 * Given a sorted array, find the starting and ending position of a target value.
 * If target is not found, return [-1, -1].
 * 
 * Algorithm:
 * 1. Use binary search twice: once for start position, once for end position
 * 2. For start: when found, search left half to find earlier occurrence
 * 3. For end: when found, search right half to find later occurrence
 * 
 * Time Complexity: O(log n) - two binary searches
 * Space Complexity: O(1) - constant extra space
 * 
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* searchRange(int* nums, int numsSize, int target, int* returnSize) {
	int startPosition, endPosition;
	int *result;

	result = (int *)malloc(2 * sizeof(int));
	*returnSize = 2;
	
	// Handle edge cases: NULL pointer or empty array
	if (!nums || !numsSize) {
		result[0] = -1;
		result[1] = -1;
		return result;
	}

	// Find start and end positions using binary search
	startPosition = findStartPosition(nums, numsSize, target);
	endPosition = findEndPosition(nums, numsSize, target);

	result[0] = startPosition;
	result[1] = endPosition;

	return result;
}