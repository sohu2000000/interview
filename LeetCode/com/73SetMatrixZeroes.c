/*
73. Set Matrix Zeroes
Medium
Topics
premium lock icon
Companies
Hint
Given an m x n integer matrix matrix, if an element is 0, set its entire row and column to 0's.

You must do it in place.

 

Example 1:


Input: matrix = [[1,1,1],[1,0,1],[1,1,1]]
Output: [[1,0,1],[0,0,0],[1,0,1]]
Example 2:


Input: matrix = [[0,1,2,0],[3,4,5,2],[1,3,1,5]]
Output: [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
 

Constraints:

m == matrix.length
n == matrix[0].length
1 <= m, n <= 200
-231 <= matrix[i][j] <= 231 - 1
 

Follow up:

A straightforward solution using O(mn) space is probably a bad idea.
A simple improvement uses O(m + n) space, but still not the best solution.
Could you devise a constant space solution?
 */


void setRowZero(int** matrix, int *matrixColSize, int row) {
	int i;

	for (i = 0; i < *matrixColSize; i++)
		matrix[row][i] = 0;

	return;
}

void setColZero(int** matrix, int matrixSize, int col) {
	int i;

	for (i = 0; i < matrixSize; i++)
		matrix[i][col] = 0;

	return;
}

void setZeroes(int** matrix, int matrixSize, int* matrixColSize) {
	int i, j;
	int firstRowHasZero = 0;  // 修复：第0行是否有0
	int firstColHasZero = 0;  // 修复：第0列是否有0

	if (matrix == NULL || matrixSize == 0 || matrixColSize == NULL)
		return;
	
	// 检查第0行是否有0
	for (j = 0; j < *matrixColSize; j++) {
		if (matrix[0][j] == 0) {
			firstRowHasZero = 1;
			break;
		}
	}

	// 检查第0列是否有0
	for (i = 0; i < matrixSize; i++) {
		if (matrix[i][0] == 0) {
			firstColHasZero = 1;
			break;
		}
	}

	// 用第0行和第0列作为标记，检查其他元素
	for (i = 1; i < matrixSize; i++) {
		for (j = 1; j < *matrixColSize; j++) {
			if (matrix[i][j] == 0) {
				matrix[i][0] = 0;  // 标记该行需要清零
				matrix[0][j] = 0;  // 标记该列需要清零
			}
		}
	}

	// 根据第0列的标记，清零对应的行（从第1行开始）
	for (i = 1; i < matrixSize; i++) {
		if (matrix[i][0] == 0) {
			setRowZero(matrix, matrixColSize, i);
		}
	}
	
	// 根据第0行的标记，清零对应的列（从第1列开始）
	for (j = 1; j < *matrixColSize; j++) {
		if (matrix[0][j] == 0) {
			setColZero(matrix, matrixSize, j);
		}
	}

	// 最后处理第0行和第0列本身
	if (firstRowHasZero) {
		setRowZero(matrix, matrixColSize, 0);
	}
	if (firstColHasZero) {
		setColZero(matrix, matrixSize, 0);
	}
}