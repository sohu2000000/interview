#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode 162: 寻找峰值
 * 
 * 问题描述：
 * 峰值元素是指其值严格大于左右相邻值的元素
 * 给定一个整数数组 nums，找到峰值元素并返回其索引
 * 假设 nums[-1] = nums[n] = -∞
 * 
 * 示例：
 * 输入：nums = [1,2,3,1]
 * 输出：2
 * 解释：3 是峰值元素，你的函数应该返回其索引 2
 * 
 * 核心思路：
 * - 二分查找
 * - 关键：比较 nums[mid] 和 nums[mid+1]
 * - 如果 nums[mid] > nums[mid+1]，峰值在左侧（包括mid）
 * - 如果 nums[mid] < nums[mid+1]，峰值在右侧（不包括mid）
 * - 必然存在峰值（数组两端为-∞）
 */

/*
 * 二分查找峰值元素
 * @param nums: 整数数组
 * @param arraySize: 数组大小
 * @return: 峰值元素的索引
 * 
 * 算法核心：
 * - 比较 nums[mid] 和 nums[mid+1]
 * - 沿着"上坡"方向移动，必然能找到峰值
 * 
 * 为什么一定能找到峰值？
 * - nums[-1] = -∞, nums[n] = -∞（虚拟边界）
 * - 如果 nums[mid] > nums[mid+1]：
 *   左侧必有峰值（最坏情况是nums[0]）
 * - 如果 nums[mid] < nums[mid+1]：
 *   右侧必有峰值（最坏情况是nums[n-1]）
 * 
 * 时间复杂度: O(logn)
 * 空间复杂度: O(1)
 */
int findPeakElement(int* nums, int arraySize) {
	int leftBound, rightBound, midIndex;

	leftBound = 0;
	rightBound = arraySize - 1;

	/* 二分查找峰值 */
	while (leftBound < rightBound) {
		midIndex = (leftBound + rightBound) / 2;

		/* 比较mid和mid+1，决定搜索方向 */
		if (nums[midIndex] > nums[midIndex + 1]) {
			/* 峰值在左侧（包括mid） */
			rightBound = midIndex;
		} else {
			/* 峰值在右侧（不包括mid） */
			leftBound = midIndex + 1;
		}
	}

	/* left == right，找到峰值 */
	return leftBound;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：二分查找 + 沿上坡方向
 * 
 * 为什么沿上坡方向能找到峰值？
 * 
 * 示例：nums = [1,2,1,3,5,6,4]
 * 
 * 第1次：left=0, right=6, mid=3
 *   nums[3]=3, nums[4]=5
 *   3 < 5，沿上坡向右
 *   left=4
 * 
 * 第2次：left=4, right=6, mid=5
 *   nums[5]=6, nums[6]=4
 *   6 > 4，沿下坡向左（峰值在左侧）
 *   right=5
 * 
 * 第3次：left=4, right=5, mid=4
 *   nums[4]=5, nums[5]=6
 *   5 < 6，向右
 *   left=5
 * 
 * left == right = 5，找到峰值索引5（值为6）
 * 
 * 关键：
 * - 不需要检查 nums[mid-1]
 * - 只需要比较 nums[mid] 和 nums[mid+1]
 * - 沿着上坡方向，必然到达峰值
 */