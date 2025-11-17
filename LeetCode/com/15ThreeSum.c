/*
Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.

Notice that the solution set must not contain duplicate triplets.

 

Example 1:

Input: nums = [-1,0,1,2,-1,-4]
Output: [[-1,-1,2],[-1,0,1]]
Explanation: 
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0.
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0.
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0.
The distinct triplets are [-1,0,1] and [-1,-1,2].
Notice that the order of the output and the order of the triplets does not matter.
Example 2:

Input: nums = [0,1,1]
Output: []
Explanation: The only possible triplet does not sum up to 0.
Example 3:

Input: nums = [0,0,0]
Output: [[0,0,0]]
Explanation: The only possible triplet sums up to 0.
 

Constraints:

3 <= nums.length <= 3000
-105 <= nums[i] <= 105
 */
/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

int cmp(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

/**
 * 三数之和 - 双指针法
 * 
 * 核心思想：
 * 1. 先排序数组
 * 2. 固定第一个数，用双指针找另外两个数
 * 3. 去重：跳过重复的数字
 * 
 * 时间复杂度：O(n²) - 外层O(n)，内层双指针O(n)
 * 空间复杂度：O(1) - 不计输出空间
 */
int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
	int i, left, right, currentSum;	
	int **result = NULL;

	// 边界检查
	if (NULL == nums || numsSize < 3) {
		*returnSize = 0;
		*returnColumnSizes = NULL;
		return NULL;
	}

	// 分配内存（三元组数量最坏情况是O(n²)，分配足够大的空间）
	int maxResults = numsSize * numsSize / 2;  // 保守估计
	result = (int **)malloc(maxResults * sizeof(int *));
	*returnColumnSizes = (int *)malloc(maxResults * sizeof(int));
	*returnSize = 0;

	// 排序（使结果有序，便于去重）
	qsort(nums, numsSize, sizeof(int), cmp);

	// 固定第一个数，用双指针找另外两个数
	for (i = 0; i < numsSize - 2; i++) {
		// 去重：跳过重复的第一个数
		if (i > 0 && nums[i] == nums[i - 1]) continue;

		// 双指针：left从i+1开始，right从末尾开始
		left = i + 1;
		right = numsSize - 1;
		
		while (left < right) {
			currentSum = nums[i] + nums[left] + nums[right];
			
			if (currentSum == 0) {
				// 找到一个三元组
				result[*returnSize] = (int *)malloc(3 * sizeof(int));
				result[*returnSize][0] = nums[i];
				result[*returnSize][1] = nums[left];
				result[*returnSize][2] = nums[right];
				(*returnColumnSizes)[*returnSize] = 3;
				(*returnSize)++;

				// 去重：跳过重复的left和right
				while (left < right && nums[left] == nums[left + 1]) left++;
				while (left < right && nums[right] == nums[right - 1]) right--;
				
				// 移动双指针
				left++;
				right--;
			} else if (currentSum < 0) {
				// 和太小，左指针右移（增大和）
				left++;
			} else {
				// 和太大，右指针左移（减小和）
				right--;
			}
		}
	}

	return result;
}