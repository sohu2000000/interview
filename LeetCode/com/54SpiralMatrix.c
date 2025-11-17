/*
Given an m x n matrix, return all elements of the matrix in spiral order.

 

Example 1:


Input: matrix = [[1,2,3],[4,5,6],[7,8,9]]
Output: [1,2,3,6,9,8,7,4,5]
Example 2:


Input: matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
Output: [1,2,3,4,8,12,11,10,9,5,6,7]
 

Constraints:

m == matrix.length
n == matrix[i].length
1 <= m, n <= 10
-100 <= matrix[i][j] <= 100
 */


/**
 * Note: The returned array must be malloced, assume caller calls free().
 * 
 * 螺旋矩阵 - 模拟法
 * 
 * 核心思想：
 * - 用四个边界（上下左右）控制螺旋范围
 * - 按顺序遍历：右 → 下 → 左 → 上
 * - 每完成一条边，收缩对应边界
 * - 注意：左和上需要额外判断（避免重复遍历）
 * 
 * 时间复杂度：O(m×n) - 遍历所有元素一次
 * 空间复杂度：O(1) - 不计输出数组
 */
int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {
	// 四个边界
	int top = 0;                      // 上边界
	int bottom = matrixSize - 1;      // 下边界
	int left = 0;                     // 左边界
	int right = *matrixColSize - 1;   // 右边界
	
	int pos = 0;     // 结果数组的写入位置
	int col, row;    // 循环变量
	int *result = NULL;

	// 分配结果数组
	result = (int *)malloc(matrixSize * (*matrixColSize) * sizeof(int));
	if (result == NULL) return NULL;

	// 螺旋遍历
	while (top <= bottom && left <= right) {
		// 方向1：从左到右（沿着上边界）
		for (col = left; col <= right; col++) {
			result[pos++] = matrix[top][col];
		}
		top++;  // 上边界向下收缩

		// 方向2：从上到下（沿着右边界）
		for (row = top; row <= bottom; row++) {
			result[pos++] = matrix[row][right];
		}
		right--;  // 右边界向左收缩

		// 方向3：从右到左（沿着下边界）
		// 检查：确保还有行可以遍历（防止top超过bottom）
		// 场景：单行矩阵，方向1已遍历完，避免重复
		if (top <= bottom) {
			for (col = right; col >= left; col--) {
				result[pos++] = matrix[bottom][col];
			}
			bottom--;  // 下边界向上收缩
		}

		// 方向4：从下到上（沿着左边界）
		// 检查：确保还有列可以遍历（防止left超过right）
		// 场景：单列矩阵，方向2已遍历完，避免重复
		if (left <= right) {
			for (row = bottom; row >= top; row--) {
				result[pos++] = matrix[row][left];
			}
			left++;  // 左边界向右收缩
		}
	}

	*returnSize = pos;
	return result;
}
