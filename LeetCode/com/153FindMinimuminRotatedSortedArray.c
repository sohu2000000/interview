/**
 * LeetCode 153: Find Minimum in Rotated Sorted Array
 * 
 * Given a rotated sorted array, find the minimum element.
 * Array was originally sorted in ascending order, then rotated at some pivot.
 * All elements are unique.
 * 
 * Algorithm:
 * 1. Quick check: if array is not rotated (nums[0] < nums[n-1]), return first element
 * 2. Use binary search to find the rotation point (which is the minimum)
 * 3. Compare mid with right boundary to determine which half to search
 *    - If nums[mid] > nums[right]: minimum is in right half
 *    - Otherwise: minimum is in left half (including mid)
 * 
 * Time Complexity: O(log n) - binary search
 * Space Complexity: O(1) - constant extra space
 * 
 * Example:
 * Input: [4,5,6,7,0,1,2]
 * Output: 0
 */
int findMin(int* nums, int numsSize) {
	int left, right, mid;

	left = 0;
	right = numsSize - 1;
	
	// Quick check: if array is not rotated, return first element
	if (nums[left] < nums[right])
		return nums[left];
	
	// Binary search for the minimum element
	while (left < right) {
		mid = left + (right - left) / 2;  // Avoid overflow

		// If mid > right, rotation point is in right half
		if (nums[mid] > nums[right])
			left = mid + 1;
		else
			// Otherwise, minimum is in left half (including mid)
			right = mid;
	}

	// left == right, pointing to the minimum element
	return nums[left];
}