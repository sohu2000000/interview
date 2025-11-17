/*
You are given an integer array height of length n. There are n vertical lines drawn such that the two endpoints of the ith line are (i, 0) and (i, height[i]).

Find two lines that together with the x-axis form a container, such that the container contains the most water.

Return the maximum amount of water a container can store.

Notice that you may not slant the container.

 

Example 1:


Input: height = [1,8,6,2,5,4,8,3,7]
Output: 49
Explanation: The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.
Example 2:

Input: height = [1,1]
Output: 1
 

Constraints:

n == height.length
2 <= n <= 105
0 <= height[i] <= 104
 */

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * 盛最多水的容器 - 双指针法
 * 
 * 核心思想：
 * - 容器面积 = 宽度 × 高度
 * - 宽度 = right - left
 * - 高度 = min(height[left], height[right]) （短板效应）
 * 
 * 贪心策略：
 * - 从最宽的容器开始（left=0, right=n-1）
 * - 每次移动较短的那条边（可能增大高度）
 * - 移动较长的边只会让容器变小（宽度减小，高度不增）
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int maxArea(int* height, int heightSize) {
	int left = 0;                  // 左指针（左边界）
	int right = heightSize - 1;    // 右指针（右边界）
	int maxWater = 0;              // 最大容水量
	int currentWater;              // 当前容水量

	// 边界检查
	if (NULL == height || heightSize < 2)
		return 0;
	
	// 双指针向中间移动
	while (left < right) {
		// 计算当前容器的容水量
		// 宽度 × 高度（取较短的边）
		int width = right - left;
		int minHeight = MIN(height[left], height[right]);
		currentWater = width * minHeight;
		
		// 更新最大容水量
		if (currentWater > maxWater) {
			maxWater = currentWater;
		}
		
		// 贪心：移动较短的那条边（可能找到更高的边）
		if (height[left] < height[right]) {
			left++;   // 左边更短，移动左指针
		} else {
			right--;  // 右边更短或相等，移动右指针
		}
	}

	return maxWater;
}