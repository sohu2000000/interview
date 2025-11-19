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

enum state_code {
	DEADDEAD = 2,
	DEADALIVE = 3,
	ALIVEDEAD = 4,
	ALIVEALIVE = 5,
};

int get_live_count(int** board, int boardSize, int* boardColSize,
	           int row, int col) {

	int live_count = 0;

	// 定义宏简化代码：检查是否为活细胞（初始1或编码后的4,5）
	#define IS_ALIVE(state) ((state) == 1 || (state) == ALIVEDEAD || (state) == ALIVEALIVE)

	// row - 1, col - 1
	if (row - 1 >= 0 && col - 1 >= 0 && IS_ALIVE(board[row - 1][col - 1]))
		live_count++;
	// row - 1, col
	if (row - 1 >= 0 && IS_ALIVE(board[row - 1][col]))
		live_count++;
	// row - 1, col + 1
	if (row - 1 >= 0 && col + 1 < *boardColSize && IS_ALIVE(board[row - 1][col + 1]))
		live_count++;
	// row, col - 1
	if (col - 1 >= 0 && IS_ALIVE(board[row][col - 1]))
		live_count++;
	// row, col + 1
	if (col + 1 < *boardColSize && IS_ALIVE(board[row][col + 1]))
		live_count++;
	// row + 1, col - 1
	if (row + 1 < boardSize && col - 1 >= 0 && IS_ALIVE(board[row + 1][col - 1]))
		live_count++;
	// row + 1, col
	if (row + 1 < boardSize && IS_ALIVE(board[row + 1][col]))
		live_count++;
	// row + 1, col + 1
	if (row + 1 < boardSize && col + 1 < *boardColSize && IS_ALIVE(board[row + 1][col + 1]))
		live_count++;

	#undef IS_ALIVE

	return live_count;
}

void apply_rule(int** board, int boardSize, int* boardColSize,
		int row, int col, int live_count) {
	
	// 检查当前细胞是否活着
	// 初始状态：1=活着，或编码后：ALIVEDEAD(2)、ALIVEALIVE(3)
	int isAlive = (board[row][col] == 1 || 
	               board[row][col] == ALIVEDEAD || 
	               board[row][col] == ALIVEALIVE);

	if (isAlive) {
		// 活细胞的规则
		if (live_count < 2 || live_count > 3) {
			board[row][col] = ALIVEDEAD;  // 活->死
		} else {
			board[row][col] = ALIVEALIVE;  // 修复：活->活（需要显式设置）
		}
	} else {
		// 死细胞的规则
		if (live_count == 3) {
			board[row][col] = DEADALIVE;  // 死->活
		} else {
			board[row][col] = DEADDEAD;   // 修复：死->死（需要显式设置）
		}
	}
}

void gameOfLife(int** board, int boardSize, int* boardColSize) {
   int i, j, live_count;

   // encode * apply rule 
   for (i = 0; i < boardSize; i++) {
	for (j = 0; j < *boardColSize; j++) {
		live_count = get_live_count(board, boardSize, boardColSize, i, j);
		apply_rule(board, boardSize, boardColSize, i, j, live_count);
	}
   }

   // decode
   for (i = 0; i < boardSize; i++) {
	for (j = 0; j < *boardColSize; j++) {
		if (board[i][j] == ALIVEDEAD || board[i][j] == DEADDEAD)
			board[i][j] = 0;
		else if (board[i][j] == DEADALIVE || board[i][j] == ALIVEALIVE)
			board[i][j] = 1;
	}
   }
}
