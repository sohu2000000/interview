/*
You are given an array of non-overlapping intervals intervals where intervals[i] = [starti, endi] represent the start and the end of the ith interval and intervals is sorted in ascending order by starti. You are also given an interval newInterval = [start, end] that represents the start and end of another interval.

Insert newInterval into intervals such that intervals is still sorted in ascending order by starti and intervals still does not have any overlapping intervals (merge overlapping intervals if necessary).

Return intervals after the insertion.

Note that you don't need to modify intervals in-place. You can make a new array and return it.

 

Example 1:

Input: intervals = [[1,3],[6,9]], newInterval = [2,5]
Output: [[1,5],[6,9]]
Example 2:

Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
Output: [[1,2],[3,10],[12,16]]
Explanation: Because the new interval [4,8] overlaps with [3,5],[6,7],[8,10].

 */
/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 * 
 * 插入区间 - 一次遍历
 * 
 * 核心思想：
 * - 遍历原区间数组，与newInterval比较
 * - 三种情况：在前面、在后面、重叠
 * - 重叠时合并，不重叠时直接添加
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(n) - 结果数组
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int** insert(int** intervals, int intervalsSize, int* intervalsColSize, int* newInterval, int newIntervalSize, int* returnSize, int** returnColumnSizes) {
	int i, currentStart, currentEnd;
	int mergeStart, mergeEnd;  // 待插入区间（可能合并后的）
	int **result;
	int resultCount;           // 结果数组中的区间数量
	int isInserted;            // 标记newInterval是否已插入
   
	// 分配结果数组（最多intervalsSize + 1个区间）
	result = (int **)malloc((intervalsSize + 1) * sizeof(int *));
	for (i = 0; i < intervalsSize + 1; i++) {
		result[i] = (int *)malloc(2 * sizeof(int));
	}

	*returnColumnSizes = (int *)malloc((intervalsSize + 1) * sizeof(int));
	for (i = 0; i < intervalsSize + 1; i++) {
		(*returnColumnSizes)[i] = 2;
	}

	resultCount = 0;
	mergeStart = newInterval[0];
	mergeEnd = newInterval[1];
	isInserted = 0;

	// 遍历所有区间
	for (i = 0; i < intervalsSize; i++) {
		currentStart = intervals[i][0];
		currentEnd = intervals[i][1];

		if (currentEnd < mergeStart) {
			// 情况1：当前区间在newInterval之前，不重叠
			result[resultCount][0] = currentStart;
			result[resultCount][1] = currentEnd;
			resultCount++;
		} else if (currentStart > mergeEnd) {
			// 情况2：当前区间在newInterval之后，不重叠
			if (!isInserted) {
				// 先插入合并后的区间
				result[resultCount][0] = mergeStart;
				result[resultCount][1] = mergeEnd;
				resultCount++;
				isInserted = 1;
			}
			// 复制当前区间
			result[resultCount][0] = currentStart;
			result[resultCount][1] = currentEnd;
			resultCount++;
		} else {
			// 情况3：重叠，合并区间
			mergeStart = MIN(mergeStart, currentStart);
			mergeEnd = MAX(mergeEnd, currentEnd);
		}
	}
	
	// 如果newInterval还没插入（在所有区间之后或与最后区间合并）
	if (!isInserted) {
		result[resultCount][0] = mergeStart;
		result[resultCount][1] = mergeEnd;
		resultCount++;
	}

	*returnSize = resultCount;
	return result;
}