/*
According to Wikipedia's article: "The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970."

The board is made up of an m x n grid of cells, where each cell has an initial state: live (represented by a 1) or dead (represented by a 0). Each cell interacts with its eight neighbors (horizontal, vertical, diagonal) using the following four rules (taken from the above Wikipedia article):

Any live cell with fewer than two live neighbors dies as if caused by under-population.
Any live cell with two or three live neighbors lives on to the next generation.
Any live cell with more than three live neighbors dies, as if by over-population.
Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
The next state of the board is determined by applying the above rules simultaneously to every cell in the current state of the m x n grid board. In this process, births and deaths occur simultaneously.

Given the current state of the board, update the board to reflect its next state.

Note that you do not need to return anything.

 

Example 1:


Input: board = [[0,1,0],[0,0,1],[1,1,1],[0,0,0]]
Output: [[0,0,0],[1,0,1],[0,1,1],[0,1,0]]
Example 2:


Input: board = [[1,1],[1,0]]
Output: [[1,1],[1,1]]
 

Constraints:

m == board.length
n == board[i].length
1 <= m, n <= 25
board[i][j] is 0 or 1.
 

Follow up:

Could you solve it in-place? Remember that the board needs to be updated simultaneously: You cannot update some cells first and then use their updated values to update other cells.
In this question, we represent the board using a 2D array. In principle, the board is infinite, which would cause problems when the active area encroaches upon the border of the array (i.e., live cells reach the border). How would you address these problems?
 */

/**
 * 生命游戏 - 状态编码法
 * 
 * 核心思想：
 * - 用不同的值编码"当前状态"和"下一状态"
 * - 编码值避免与初始值0,1冲突，使用2,3,4,5
 * - 第一遍：根据规则编码下一状态
 * - 第二遍：解码为最终状态0或1
 * 
 * 状态编码：
 * 2 = 死->死, 3 = 死->活, 4 = 活->死, 5 = 活->活
 * 
 * 时间复杂度：O(m×n)
 * 空间复杂度：O(1) - 原地更新
 */

enum state_code {
	DEAD_TO_DEAD = 2,   // 死->死
	DEAD_TO_ALIVE = 3,  // 死->活
	ALIVE_TO_DEAD = 4,  // 活->死
	ALIVE_TO_ALIVE = 5, // 活->活
};

/**
 * 统计周围8个邻居中活细胞的数量
 */
int countLiveNeighbors(int** board, int boardSize, int* boardColSize,
	           int row, int col) {
	int liveNeighbors = 0;

	// 定义宏简化代码：检查是否为活细胞（初始1或编码后的4,5）
	#define IS_ALIVE(state) ((state) == 1 || (state) == ALIVE_TO_DEAD || (state) == ALIVE_TO_ALIVE)

	// row - 1, col - 1
	if (row - 1 >= 0 && col - 1 >= 0 && IS_ALIVE(board[row - 1][col - 1]))
		liveNeighbors++;
	// row - 1, col
	if (row - 1 >= 0 && IS_ALIVE(board[row - 1][col]))
		liveNeighbors++;
	// row - 1, col + 1
	if (row - 1 >= 0 && col + 1 < *boardColSize && IS_ALIVE(board[row - 1][col + 1]))
		liveNeighbors++;
	// row, col - 1
	if (col - 1 >= 0 && IS_ALIVE(board[row][col - 1]))
		liveNeighbors++;
	// row, col + 1
	if (col + 1 < *boardColSize && IS_ALIVE(board[row][col + 1]))
		liveNeighbors++;
	// row + 1, col - 1
	if (row + 1 < boardSize && col - 1 >= 0 && IS_ALIVE(board[row + 1][col - 1]))
		liveNeighbors++;
	// row + 1, col
	if (row + 1 < boardSize && IS_ALIVE(board[row + 1][col]))
		liveNeighbors++;
	// row + 1, col + 1
	if (row + 1 < boardSize && col + 1 < *boardColSize && IS_ALIVE(board[row + 1][col + 1]))
		liveNeighbors++;

	#undef IS_ALIVE

	return liveNeighbors;
}

/**
 * 根据生命游戏规则更新细胞状态（编码为过渡状态）
 */
void updateCellState(int** board, int boardSize, int* boardColSize,
		             int row, int col, int liveNeighbors) {

	// 检查当前细胞是否活着
	// 初始状态：1=活着，或编码后：ALIVE_TO_DEAD(4)、ALIVE_TO_ALIVE(5)
	int isCurrentlyAlive = (board[row][col] == 1 || 
	                        board[row][col] == ALIVE_TO_DEAD || 
	                        board[row][col] == ALIVE_TO_ALIVE);

	if (isCurrentlyAlive) {
		// 活细胞的规则
		if (liveNeighbors < 2 || liveNeighbors > 3) {
			board[row][col] = ALIVE_TO_DEAD;  // 活->死（人口过少或过多）
		} else {
			board[row][col] = ALIVE_TO_ALIVE;  // 活->活（人口适中）
		}
	} else {
		// 死细胞的规则
		if (liveNeighbors == 3) {
			board[row][col] = DEAD_TO_ALIVE;  // 死->活（繁殖）
		} else {
			board[row][col] = DEAD_TO_DEAD;   // 死->死
	}
	}
}

/**
 * 主函数：生命游戏的下一代
 * 
 * 步骤1：编码 - 根据规则计算下一状态并编码
 * 步骤2：解码 - 将编码状态解码为最终的0或1
 */
void gameOfLife(int** board, int boardSize, int* boardColSize) {
	int row, col, liveNeighbors;

	// ===== 步骤1：编码下一代状态 =====
	for (row = 0; row < boardSize; row++) {
		for (col = 0; col < *boardColSize; col++) {
			// 统计活邻居数量
			liveNeighbors = countLiveNeighbors(board, boardSize, boardColSize, row, col);
			// 根据规则更新状态（编码）
			updateCellState(board, boardSize, boardColSize, row, col, liveNeighbors);
	}
   }

	// ===== 步骤2：解码为最终状态 =====
	for (row = 0; row < boardSize; row++) {
		for (col = 0; col < *boardColSize; col++) {
			// 解码：死状态(2,4) → 0，活状态(3,5) → 1
			if (board[row][col] == ALIVE_TO_DEAD || board[row][col] == DEAD_TO_DEAD) {
				board[row][col] = 0;  // 死细胞
			} else if (board[row][col] == DEAD_TO_ALIVE || board[row][col] == ALIVE_TO_ALIVE) {
				board[row][col] = 1;  // 活细胞
			}
	}
   }
}
