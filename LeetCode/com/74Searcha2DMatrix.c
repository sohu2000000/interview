#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * LeetCode 74: 搜索二维矩阵
 * 
 * 问题描述：
 * 编写一个高效的算法来判断 m×n 矩阵中，是否存在一个目标值
 * 矩阵特点：
 * - 每行中的整数从左到右按升序排列
 * - 每行的第一个整数大于前一行的最后一个整数
 * 
 * 示例：
 * matrix = [[1,3,5,7],
 *           [10,11,16,20],
 *           [23,30,34,60]]
 * target = 3
 * 输出：true
 * 
 * 核心思路：
 * - 把二维矩阵看作一维有序数组
 * - 二分查找
 * - 索引转换：index → (row, col)
 */

/*
 * 搜索二维矩阵
 * @param matrix: 二维矩阵
 * @param numRows: 行数
 * @param numCols: 每行列数
 * @param target: 目标值
 * @return: 找到返回true
 * 
 * 算法步骤：
 * 1. 将二维矩阵视为一维有序数组
 * 2. 使用二分查找
 * 3. 索引转换：
 *    - row = mid / numCols
 *    - col = mid % numCols
 * 
 * 时间复杂度: O(log(m×n))
 * 空间复杂度: O(1)
 */
bool searchMatrix(int** matrix, int numRows, int* numCols, int target) {
	int rowIndex, colIndex, totalElements, numColsInRow;
	int left, right, mid;

	/* 边界检查 */
	if (matrix == NULL || numRows == 0 || numCols == NULL || *numCols == 0)
		return false;
	
	/* 计算总元素数 */
	numColsInRow = numCols[0];
	totalElements = numRows * numColsInRow;

	/* 二分查找 */
	left = 0;
	right = totalElements - 1;
	while (left <= right) {
		mid = (left + right) / 2;
		
		/* 一维索引转换为二维坐标 */
		rowIndex = mid / numColsInRow;
		colIndex = mid % numColsInRow;
		
		if (target == matrix[rowIndex][colIndex])
			return true;
		
		if (target > matrix[rowIndex][colIndex])
			left = mid + 1;
		else
			right = mid - 1;
	}

	return false;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：二维矩阵的一维化 + 二分查找
 * 
 * 索引转换：
 * - 一维索引 → 二维坐标
 *   row = index / numCols
 *   col = index % numCols
 * 
 * 示例：3×4矩阵
 * 
 * 二维表示：
 * [0][0] [0][1] [0][2] [0][3]
 * [1][0] [1][1] [1][2] [1][3]
 * [2][0] [2][1] [2][2] [2][3]
 * 
 * 一维表示：
 * 0  1  2  3  4  5  6  7  8  9  10 11
 * 
 * 索引5 → row=5/4=1, col=5%4=1 → [1][1] ✓
 * 索引9 → row=9/4=2, col=9%4=1 → [2][1] ✓
 * 
 * 为什么可以这样做？
 * - 矩阵的特点：每行递增，下一行 > 上一行
 * - 相当于一个完全有序的一维数组
 * - 可以直接用二分查找
 */