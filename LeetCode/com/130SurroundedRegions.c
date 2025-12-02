#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode 130: 被围绕的区域
 * 
 * 问题描述：
 * 给定一个二维矩阵，包含 'X' 和 'O'（字母O）
 * 找到所有被 'X' 围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充
 * 
 * 被围绕的区域：
 * - 不与边界连通的 'O'
 * - 从边界的 'O' 开始，通过上下左右相邻的 'O' 能到达的都不算被围绕
 * 
 * 示例：
 * 输入:
 * X X X X      输出:
 * X O O X      X X X X
 * X X O X  =>  X X X X
 * X O X X      X X X X
 *              X O X X  (最后一行的O与边界连通，保留)
 * 
 * 核心思路：
 * - 反向思维：找不被围绕的O（与边界连通的O）
 * - 从边界的O开始DFS，标记所有连通的O
 * - 剩余未标记的O就是被围绕的，改为X
 */

/* 全局数组 */
int **visited = NULL;     /* 记录是否已访问 */
int **borderConnected = NULL;  /* 标记与边界连通的O */

/*
 * DFS标记与边界连通的O
 * @param board: 棋盘
 * @param boardSize: 行数
 * @param boardColSize: 每行的列数
 * @param row: 当前行
 * @param col: 当前列
 */
void dfsMarkBorderConnected(char** board, int boardSize, int* boardColSize, int row, int col) {
	int numCols, numRows;
	
	numRows = boardSize;
	numCols = boardColSize[row];

	/* 边界检查和条件判断 */
	if (row >= 0 && row < numRows &&
	    col >= 0 && col < numCols &&
	    board[row][col] == 'O' && visited[row][col] == 0) {
		
		/* 标记为已访问和与边界连通 */
		visited[row][col] = 1;
		borderConnected[row][col] = 1;

		/* 递归访问四个方向 */
		dfsMarkBorderConnected(board, boardSize, boardColSize, row - 1, col);
		dfsMarkBorderConnected(board, boardSize, boardColSize, row + 1, col);
		dfsMarkBorderConnected(board, boardSize, boardColSize, row, col - 1);
		dfsMarkBorderConnected(board, boardSize, boardColSize, row, col + 1);
	}

	return;
}

/*
 * 主函数：捕获被围绕的区域
 * @param board: 棋盘
 * @param boardSize: 行数
 * @param boardColSize: 每行的列数
 * 
 * 算法步骤：
 * 1. 从边界的O开始DFS，标记所有与边界连通的O
 * 2. 遍历棋盘，将未标记的O改为X
 * 3. 释放内存
 * 
 * 时间复杂度: O(m*n)
 * 空间复杂度: O(m*n)
 */
void solve(char** board, int boardSize, int* boardColSize) {
	int i, j, numRows, numCols;

	numRows = boardSize;
	
	/* 分配visited数组 */
	visited = (int **)malloc(numRows * sizeof(int *));
	for (i = 0; i < boardSize; i++) {
		numCols = boardColSize[i];
		visited[i] = (int *)malloc(numCols * sizeof(int));
		memset(visited[i], 0, numCols * sizeof(int));
	}

	/* 分配borderConnected数组 */
	borderConnected = (int **)malloc(numRows * sizeof(int *));
	for (i = 0; i < numRows; i++) {
		numCols = boardColSize[i];
		borderConnected[i] = (int *)malloc(numCols * sizeof(int));
		memset(borderConnected[i], 0, numCols * sizeof(int));
	}

	/* 步骤1: 从四条边界开始DFS */
	
	/* 第一行 */
	numCols = boardColSize[0];
	for (j = 0; j < numCols; j++) {
		dfsMarkBorderConnected(board, boardSize, boardColSize, 0, j);
	}

	/* 最后一行 */
	numCols = boardColSize[numRows - 1];
	for (j = 0; j < numCols; j++) {
		dfsMarkBorderConnected(board, boardSize, boardColSize, numRows - 1, j);
	}
	
	/* 第一列 */
	for (i = 1; i < numRows - 1; i++) {
		dfsMarkBorderConnected(board, boardSize, boardColSize, i, 0);
	}

	/* 最后一列 */
	numCols = boardColSize[0];
	for (i = 1; i < numRows - 1; i++) {
		dfsMarkBorderConnected(board, boardSize, boardColSize, i, numCols - 1);
	}

	/* 步骤2: 遍历棋盘，将未标记的O改为X */
	for (i = 0; i < numRows; i++) {
		numCols = boardColSize[i];
		for (j = 0; j < numCols; j++) {
			if (board[i][j] == 'O' && borderConnected[i][j] == 0) {
				board[i][j] = 'X';  /* 关键：用 = 赋值，不是 == 比较 */
			}
		}
	}

	/* 步骤3: 释放内存 */
	for (i = 0; i < numRows; i++) {
		free(visited[i]);
	}
	free(visited);

	for (i = 0; i < numRows; i++) {
		free(borderConnected[i]);
	}
	free(borderConnected);

	return;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：反向思维 + DFS标记
 * 
 * 正向思考（困难）：
 * - 找被围绕的O（需要判断是否被X完全包围）
 * 
 * 反向思考（简单）：
 * - 找不被围绕的O（与边界连通的O）
 * - 剩下的O就是被围绕的
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入:
 * X X X X
 * X O O X
 * X X O X
 * X O X X
 * 
 * === 步骤1: 从边界DFS ===
 * 
 * 第1行：全是X，无O
 * 最后1行：(3,1)是O，开始DFS
 *   标记(3,1)，递归四个方向
 *   但(3,1)的四周都是X或边界，无法扩展
 * 
 * borderConnected标记：
 * 0 0 0 0
 * 0 0 0 0
 * 0 0 0 0
 * 0 1 0 0  ← 只有(3,1)与边界连通
 * 
 * === 步骤2: 将未标记的O改为X ===
 * 
 * (1,1): O且未标记 -> 改为X
 * (1,2): O且未标记 -> 改为X
 * (2,2): O且未标记 -> 改为X
 * (3,1): O且已标记 -> 保留O
 * 
 * 输出:
 * X X X X
 * X X X X
 * X X X X
 * X O X X  ✓
 * 
 * ==================== 关键要点 ====================
 * 
 * 反向思维的威力：
 * - 直接找被围绕的O很难（需要判断是否完全被X包围）
 * - 找不被围绕的O很简单（从边界开始DFS即可）
 * 
 * 为什么从边界开始？
 * - 与边界连通的O一定不被围绕
 * - 只需要标记这些O
 * - 剩下的O一定被围绕
 * 
 * 为什么需要两个数组？
 * - visited: 防止DFS重复访问（标准DFS操作）
 * - borderConnected: 标记与边界连通的O（业务逻辑）
 * 
 * 优化：可以合并为一个数组
 */