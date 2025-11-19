/*
Given an array of intervals where intervals[i] = [starti, endi], merge all overlapping intervals, and return an array of the non-overlapping intervals that cover all the intervals in the input.

 

Example 1:

Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlap, merge them into [1,6].
Example 2:

Input: intervals = [[1,4],[4,5]]
Output: [[1,5]]
Explanation: Intervals [1,4] and [4,5] are considered overlapping.
Example 3:

Input: intervals = [[4,7],[1,4]]
Output: [[1,7]]
Explanation: Intervals [1,4] and [4,7] are considered overlapping.
 

Constraints:

1 <= intervals.length <= 104
intervals[i].length == 2
0 <= starti <= endi <= 104
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 * 
 * 合并区间 - 排序 + 贪心合并
 * 
 * 核心思想：
 * 1. 按起始位置排序所有区间
 * 2. 遍历排序后的区间，判断是否与前一个区间重叠
 * 3. 重叠则合并（扩展end），不重叠则添加新区间
 * 
 * 时间复杂度：O(n log n) - 排序
 * 空间复杂度：O(n) - 结果数组
 */

// 比较函数：按区间起始位置排序
int compareIntervals(const void *a, const void *b) {
	// a和b是指向int*的指针，需要转换为int**再解引用
	int *interval1 = *(int **)a;
	int *interval2 = *(int **)b;
	return interval1[0] - interval2[0];  // 按起始位置排序
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
	int i, currentStart, currentEnd;
	int **result;
	int mergedCount;  // 已合并的区间数量

	// 按起始位置排序
	qsort(intervals, intervalsSize, sizeof(int *), compareIntervals);

	// 分配结果数组（最多有intervalsSize个区间）
	result = (int **)malloc(intervalsSize * sizeof(int *));
	for (i = 0; i < intervalsSize; i++) {
		result[i] = (int *)malloc(2 * sizeof(int));
	}

	// 分配列大小数组
	*returnColumnSizes = (int *)malloc(intervalsSize * sizeof(int));
	for (i = 0; i < intervalsSize; i++) {
		(*returnColumnSizes)[i] = 2;  // 每个区间有2个元素
	}

	// ===== 合并区间 =====
	mergedCount = 0;
	// 初始化第一个区间
	result[mergedCount][0] = intervals[0][0];
	result[mergedCount][1] = intervals[0][1];
	
	// 遍历剩余区间
	for (i = 1; i < intervalsSize; i++) {
		currentStart = intervals[i][0];
		currentEnd = intervals[i][1];

		// 判断是否重叠：当前区间的start <= 上一个区间的end
		if (currentStart <= result[mergedCount][1]) {
			// 重叠：合并区间，更新end为较大值
			if (currentEnd > result[mergedCount][1]) {
				result[mergedCount][1] = currentEnd;
			}
		} else {
			// 不重叠：添加新区间
			mergedCount++;
			result[mergedCount][0] = currentStart;
			result[mergedCount][1] = currentEnd;
		}
	}

	*returnSize = mergedCount + 1;
	return result;
}