#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode 33: 搜索旋转排序数组
 * 
 * 问题描述：
 * 整数数组 nums 按升序排列，数组中的值互不相同
 * 在传递给函数之前，nums 在预先未知的某个下标 k 上进行了旋转
 * 例如：[0,1,2,4,5,6,7] 在下标 3 旋转后变为 [4,5,6,7,0,1,2]
 * 给定旋转后的数组和目标值 target，如果存在返回索引，否则返回 -1
 * 要求：时间复杂度 O(logn)
 * 
 * 示例：
 * 输入：nums = [4,5,6,7,0,1,2], target = 0
 * 输出：4
 * 
 * 核心思路：
 * - 二分查找
 * - 判断哪一半是有序的
 * - 在有序的一半中判断target是否在范围内
 * - 决定搜索方向
 */

/*
 * 搜索旋转排序数组
 * @param nums: 旋转排序数组
 * @param arraySize: 数组大小
 * @param target: 目标值
 * @return: 目标索引，不存在返回-1
 * 
 * 算法核心：
 * 1. 找到哪一半是有序的
 * 2. 判断target是否在有序的一半中
 * 3. 决定搜索左半还是右半
 * 
 * 关键判断：
 * - nums[left] <= nums[mid]：左半有序
 * - nums[mid] <= nums[right]：右半有序
 * - 至少一半是有序的！
 * 
 * 时间复杂度: O(logn)
 * 空间复杂度: O(1)
 */
int search(int* nums, int arraySize, int target) {
	int leftBound, rightBound, midIndex;

	leftBound = 0;
	rightBound = arraySize - 1;
	
	/* 二分查找 */
	while (leftBound <= rightBound) {
		midIndex = (leftBound + rightBound) / 2;

		/* 找到目标 */
		if (nums[midIndex] == target) {
			return midIndex;
		}

		/* 情况1：左半部分旋转，右半部分有序 */
		if (nums[leftBound] > nums[midIndex]) {
			/* 右半有序：[mid+1, right] */
			if (target > nums[midIndex] && target <= nums[rightBound])
				leftBound = midIndex + 1;  /* target在右半 */
			else
				rightBound = midIndex - 1;  /* target在左半 */
		}
		/* 情况2：右半部分旋转，左半部分有序 */
		else if (nums[rightBound] < nums[midIndex]) {
			/* 左半有序：[left, mid-1] */
			if (target < nums[midIndex] && target >= nums[leftBound])
				rightBound = midIndex - 1;  /* target在左半 */
			else
				leftBound = midIndex + 1;  /* target在右半 */
		}
		/* 情况3：两边都有序（没有旋转） */
		else {
			if (target > nums[midIndex])
				leftBound = midIndex + 1;
			else
				rightBound = midIndex - 1;
		}
	}

	return -1;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：改进的二分查找
 * 
 * 旋转数组特点：
 * - 分成两部分，每部分都有序
 * - 例如：[4,5,6,7,0,1,2]
 *   左半：[4,5,6,7]（有序）
 *   右半：[0,1,2]（有序）
 * 
 * 判断哪一半有序：
 * - nums[left] <= nums[mid]：左半有序
 * - nums[mid] <= nums[right]：右半有序
 * - 至少一半是有序的！
 * 
 * 示例：nums = [4,5,6,7,0,1,2], target = 0
 * 
 * 第1次：left=0, right=6, mid=3
 *   nums[0]=4, nums[3]=7, nums[6]=2
 *   4 <= 7，左半有序 [4,5,6,7]
 *   0 在 [4,7] 范围内吗？no
 *   搜索右半：left=4
 * 
 * 第2次：left=4, right=6, mid=5
 *   nums[4]=0, nums[5]=1, nums[6]=2
 *   0 <= 1 <= 2，两边有序
 *   0 < 1，搜索左半：right=4
 * 
 * 第3次：left=4, right=4, mid=4
 *   nums[4]=0 == target
 *   返回 4 ✓
 */