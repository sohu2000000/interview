/*
There are some spherical balloons taped onto a flat wall that represents the XY-plane. The balloons are represented as a 2D integer array points where points[i] = [xstart, xend] denotes a balloon whose horizontal diameter stretches between xstart and xend. You do not know the exact y-coordinates of the balloons.

Arrows can be shot up directly vertically (in the positive y-direction) from different points along the x-axis. A balloon with xstart and xend is burst by an arrow shot at x if xstart <= x <= xend. There is no limit to the number of arrows that can be shot. A shot arrow keeps traveling up infinitely, bursting any balloons in its path.

Given the array points, return the minimum number of arrows that must be shot to burst all balloons.

 

Example 1:

Input: points = [[10,16],[2,8],[1,6],[7,12]]
Output: 2
Explanation: The balloons can be burst by 2 arrows:
- Shoot an arrow at x = 6, bursting the balloons [2,8] and [1,6].
- Shoot an arrow at x = 11, bursting the balloons [10,16] and [7,12].
Example 2:

Input: points = [[1,2],[3,4],[5,6],[7,8]]
Output: 4
Explanation: One arrow needs to be shot for each balloon for a total of 4 arrows.
Example 3:

Input: points = [[1,2],[2,3],[3,4],[4,5]]
Output: 2
Explanation: The balloons can be burst by 2 arrows:
- Shoot an arrow at x = 2, bursting the balloons [1,2] and [2,3].
- Shoot an arrow at x = 4, bursting the balloons [3,4] and [4,5].
*/

/**
 * 用最少数量的箭引爆气球 - 贪心算法
 * 
 * 核心思想：
 * - 按气球结束位置排序
 * - 贪心策略：尽可能用一支箭射穿多个气球
 * - 在当前箭能覆盖的最右端射箭
 * 
 * 贪心正确性：
 * 按end排序后，如果气球重叠，必然与当前箭位置重叠
 * 
 * 时间复杂度：O(n log n) - 排序
 * 空间复杂度：O(1)
 */

// 比较函数：按气球结束位置升序排序
int compareBalloons(const void *a, const void *b) {
	int *balloon1 = *(int **)a;
	int *balloon2 = *(int **)b;

	// 避免整数溢出，不用减法
	if (balloon1[1] < balloon2[1]) return -1;
	if (balloon1[1] > balloon2[1]) return 1;
	return 0;
}

int findMinArrowShots(int** points, int pointsSize, int* pointsColSize) {
	int i, arrowCount, arrowPosition;
	int balloonStart, balloonEnd;
	
	// 按结束位置排序
	qsort(points, pointsSize, sizeof(int *), compareBalloons);

	// 初始化：第一个气球需要一支箭
	arrowCount = 1;
	arrowPosition = points[0][1];  // 箭的位置在第一个气球的结束点
	
	// 遍历剩余气球
	for (i = 1; i < pointsSize; i++) {
		balloonStart = points[i][0];
		balloonEnd = points[i][1];

		// 如果气球起始位置 <= 当前箭位置，则可以被当前箭射穿
		if (balloonStart <= arrowPosition) {
			continue;  // 不需要新箭
		}
		
		// 需要新箭：射在新气球的结束位置
		arrowPosition = balloonEnd;
		arrowCount++;
	}

	return arrowCount;
}