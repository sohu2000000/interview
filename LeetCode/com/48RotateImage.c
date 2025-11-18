/*
You are given an n x n 2D matrix representing an image, rotate the image by 90 degrees (clockwise).

You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

 

Example 1:


Input: matrix = [[1,2,3],[4,5,6],[7,8,9]]
Output: [[7,4,1],[8,5,2],[9,6,3]]
Example 2:


Input: matrix = [[5,1,9,11],[2,4,8,10],[13,3,6,7],[15,14,12,16]]
Output: [[15,13,2,5],[14,3,4,1],[12,6,8,9],[16,7,10,11]]
 

Constraints:

n == matrix.length == matrix[i].length
1 <= n <= 20
-1000 <= matrix[i][j] <= 1000
 */

/**
 * 旋转图像90度（顺时针）
 * 
 * 核心思想：两步变换
 * 1. 转置矩阵（沿主对角线翻转）
 * 2. 反转每一行（左右翻转）
 * 
 * 例子：
 * [1,2,3]    转置    [1,4,7]    反转每行    [7,4,1]
 * [4,5,6]    →      [2,5,8]    →          [8,5,2]
 * [7,8,9]           [3,6,9]                [9,6,3]
 * 
 * 原理说明：
 * 顺时针旋转90度的位置变换规律：
 * - 原位置：(row, col)
 * - 新位置：(col, n-1-row)
 * 
 * 分解为两步：
 * 步骤1 - 转置：(row, col) → (col, row)
 *   matrix[i][j] 和 matrix[j][i] 交换
 * 
 * 步骤2 - 反转每行：(col, row) → (col, n-1-row)
 *   每行的第j列变成第(n-1-j)列
 * 
 * 验证：
 * 位置(0,0) → 转置→(0,0) → 反转→(0,2) ✓ 对应顺时针90度
 * 位置(0,2) → 转置→(2,0) → 反转→(2,2) ✓
 * 位置(2,0) → 转置→(0,2) → 反转→(0,0) ✓
 * 位置(2,2) → 转置→(2,2) → 反转→(2,0) ✓
 * 
 * 时间复杂度：O(n²)
 * 空间复杂度：O(1) - 原地操作
 */

#define SWAP(a, b) do { \
	int temp = a; \
	a = b; \
	b = temp; \
} while (0)

/**
 * 辅助函数：转置矩阵（沿主对角线翻转）
 * matrix[i][j] 和 matrix[j][i] 交换
 */
void transposeMatrix(int** matrix, int size) {
	int row, col;
	
	// 只遍历上三角（避免重复交换）
	for (row = 0; row < size; row++) {
		for (col = row + 1; col < size; col++) {
			SWAP(matrix[row][col], matrix[col][row]);
		}
	}
}

/**
 * 辅助函数：反转矩阵的每一行（左右翻转）
 */
void reverseEachRow(int** matrix, int size) {
	int row, left, right;

	// 反转每一行
	for (row = 0; row < size; row++) {
		left = 0;
		right = size - 1;

		// 双指针交换
		while (left < right) {
			SWAP(matrix[row][left], matrix[row][right]);
			left++;
			right--;
		}
	}
}

/**
 * 主函数：旋转图像90度
 * 步骤1：转置
 * 步骤2：反转每行
 */
void rotate(int** matrix, int matrixSize, int* matrixColSize) {
	transposeMatrix(matrix, matrixSize);
	reverseEachRow(matrix, matrixSize);
	*matrixColSize = matrixSize;
}