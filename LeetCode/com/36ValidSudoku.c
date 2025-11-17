/*
Determine if a 9 x 9 Sudoku board is valid. Only the filled cells need to be validated according to the following rules:

Each row must contain the digits 1-9 without repetition.
Each column must contain the digits 1-9 without repetition.
Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without repetition.
Note:

A Sudoku board (partially filled) could be valid but is not necessarily solvable.
Only the filled cells need to be validated according to the mentioned rules.
 

Example 1:


Input: board = 
[["5","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
Output: true
Example 2:

Input: board = 
[["8","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
Output: false
Explanation: Same as Example 1, except with the 5 in the top left corner being modified to 8. Since there are two 8's in the top left 3x3 sub-box, it is invalid.
 

Constraints:

board.length == 9
board[i].length == 9
board[i][j] is a digit 1-9 or '.'.
 

 */

#include <string.h>
#include <stdbool.h>

#define BOARD_SIZE 9

/**
 * 有效的数独 - 哈希表法
 * 
 * 核心思想：
 * - 用三个布尔数组标记数字是否出现过
 * - rowUsed[i][num]：第i行是否使用了数字num
 * - colUsed[j][num]：第j列是否使用了数字num
 * - boxUsed[k][num]：第k个3x3方块是否使用了数字num
 * 
 * 时间复杂度：O(1) - 固定9×9
 * 空间复杂度：O(1) - 固定大小数组
 */

// 数字1-9需要10个位置（索引0-9，索引0不用）
bool rowUsed[BOARD_SIZE][BOARD_SIZE + 1] = {false};  // 行标记
bool colUsed[BOARD_SIZE][BOARD_SIZE + 1] = {false};  // 列标记
bool boxUsed[BOARD_SIZE][BOARD_SIZE + 1] = {false};  // 3x3方块标记

bool isValidSudoku(char** board, int boardSize, int* boardColSize) {
	int row, col;        // 当前位置
	int boxIndex;        // 3x3方块的索引
	int num;             // 当前数字

	// 重置全局数组（LeetCode会多次调用函数）
	memset(rowUsed, false, sizeof(rowUsed));
	memset(colUsed, false, sizeof(colUsed));
	memset(boxUsed, false, sizeof(boxUsed));

	// 遍历整个棋盘
	for (row = 0; row < BOARD_SIZE; row++) {
		for (col = 0; col < BOARD_SIZE; col++) {
			// 跳过空格
			if (board[row][col] == '.')
				continue;
			
			// 获取当前数字
			num = board[row][col] - '0';  // '1'-'0'=1, '9'-'0'=9
			
			// 计算当前格子所属的小方块索引
			// 棋盘被分为多个小方块，从左到右、从上到下编号：
			// 
			// [0] [1] [2]
			// [3] [4] [5]
			// [6] [7] [8]
			//
			// 公式：boxIndex = (行所在块) × (每行块数) + (列所在块)
			// 其中：块大小 = BOARD_SIZE / 块数 = 9/3 = 3
			boxIndex = (row / 3) * 3 + col / 3;

			// 检查是否重复
			if (rowUsed[row][num] || colUsed[col][num] || boxUsed[boxIndex][num])
				return false;

			// 标记已使用
			rowUsed[row][num] = true;
			colUsed[col][num] = true;
			boxUsed[boxIndex][num] = true;
		}
	}

	*boardColSize = BOARD_SIZE;
	return true;
}