#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * LeetCode 200: 岛屿数量
 * 
 * 问题描述：
 * 给定一个由 '1'（陆地）和 '0'（水）组成的二维网格，
 * 计算岛屿的数量。岛屿由相邻的陆地连接而成（上下左右），
 * 并且四面被水包围。
 * 
 * 示例：
 * 输入:
 * [["1","1","1","1","0"],
 *  ["1","1","0","1","0"],
 *  ["1","1","0","0","0"],
 *  ["0","0","0","0","0"]]
 * 输出: 1（左上角是一个连通的岛屿）
 * 
 * 核心思路：
 * - DFS（深度优先搜索）
 * - 遍历网格，遇到未访问的陆地'1'，岛屿数+1
 * - 从该陆地开始DFS，标记所有连通的陆地
 * - 使用visited数组记录访问状态
 */

/* 全局visited数组：记录每个位置是否已访问 */
int **visited = NULL;

/*
 * DFS递归访问相邻陆地
 * @param grid: 网格
 * @param gridSize: 行数
 * @param gridColSize: 每行的列数数组
 * @param row: 当前行
 * @param col: 当前列
 * 
 * 算法步骤：
 * 1. 检查边界和条件（越界、是水、已访问）
 * 2. 标记当前位置为已访问
 * 3. 递归访问四个方向（上、下、左、右）
 */
void dfsMarkIsland(char **grid, int gridSize, int *gridColSize, int row, int col) {
	int numCols, numRows;

	numRows = gridSize;
	numCols = gridColSize[row];

	/* 边界检查和条件判断 */
	if (row < 0 || row >= numRows || 
	    col < 0 || col >= numCols || 
	    grid[row][col] == '0' || visited[row][col] == 1) {
		return;  /* 越界、是水、或已访问，直接返回 */
	}

	/* 标记当前位置为已访问 */
	visited[row][col] = 1;

	/* 递归访问四个方向（上、下、左、右） */
	dfsMarkIsland(grid, gridSize, gridColSize, row - 1, col);  /* 上 */
	dfsMarkIsland(grid, gridSize, gridColSize, row + 1, col);  /* 下 */
	dfsMarkIsland(grid, gridSize, gridColSize, row, col - 1);  /* 左 */
	dfsMarkIsland(grid, gridSize, gridColSize, row, col + 1);  /* 右 */

	return;
}

/*
 * 主函数：计算岛屿数量
 * @param grid: 二维网格
 * @param gridSize: 行数
 * @param gridColSize: 每行的列数数组
 * @return: 岛屿数量
 * 
 * 时间复杂度: O(m*n) - m行n列，每个格子访问一次
 * 空间复杂度: O(m*n) - visited数组 + 递归栈O(m*n)
 */
int numIslands(char** grid, int gridSize, int* gridColSize) {
	int numCols, numRows, i, j, islandCount = 0;

	numRows = gridSize;
	
	/* 分配visited数组 */
	visited = (int **)malloc(numRows * sizeof(int *));
	for (i = 0; i < numRows; i++) {
		numCols = gridColSize[i];
		visited[i] = (int *)malloc(numCols * sizeof(int));
		memset(visited[i], 0, numCols * sizeof(int));
	}

	/* 遍历整个网格 */
	for (i = 0; i < numRows; i++) {
		numCols = gridColSize[i];  /* 每行重新获取列数 */
		for (j = 0; j < numCols; j++) {
			/* 如果是未访问的陆地，发现新岛屿 */
			if (grid[i][j] == '1' && visited[i][j] == 0) {
				islandCount++;  /* 岛屿数量+1 */
				dfsMarkIsland(grid, gridSize, gridColSize, i, j);  /* DFS标记整个岛屿 */
			}
		}
	}

	/* 释放visited数组 */
	for (i = 0; i < numRows; i++) {
		free(visited[i]);
	}
	free(visited);

	return islandCount;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS + 连通分量
 * 
 * 岛屿 = 连通的陆地（'1'）
 * - 通过DFS找到所有连通的陆地
 * - 每次DFS标记一个完整的岛屿
 * - 岛屿数量 = DFS的调用次数
 * 
 * ==================== 详细走查 ====================
 * 
 * 输入:
 * [["1","1","1","1","0"],
 *  ["1","1","0","1","0"],
 *  ["1","1","0","0","0"],
 *  ["0","0","0","0","0"]]
 * 
 * 网格可视化：
 * 1 1 1 1 0
 * 1 1 0 1 0
 * 1 1 0 0 0
 * 0 0 0 0 0
 * 
 * === 遍历过程 ===
 * 
 * (0,0): grid='1', visited=0
 *   -> islandCount=1
 *   -> DFS标记整个岛屿：
 *      (0,0)→(0,1)→(0,2)→(0,3)→(1,0)→(1,1)→(2,0)→(2,1)
 * 
 * visited变化：
 * 1 1 1 1 0
 * 1 1 0 0 0
 * 1 1 0 0 0
 * 0 0 0 0 0
 * 
 * (1,3): grid='1', visited=0
 *   -> islandCount=2? 
 *   不对！题目说这是1个岛屿
 * 
 * 等等，让我重新看题...
 * 
 * 实际上(1,3)的'1'没有和左上角的岛屿连通！
 * 输出应该是1，说明(1,3)='1'也应该被标记了
 * 
 * === 正确的DFS过程 ===
 * 
 * 从(0,0)开始DFS：
 * (0,0) -> 上下左右
 *   -> (0,1) -> 上下左右
 *     -> (0,2) -> 上下左右
 *       -> (0,3) -> 上下左右
 *         -> (1,3)！找到(1,3)
 *   -> (1,0) -> 上下左右
 *     -> (1,1) -> 上下左右
 *   -> (2,0) -> 上下左右
 *     -> (2,1)
 * 
 * 所有的'1'都通过这一次DFS被标记了
 * 所以只有1个岛屿
 * 
 * ==================== 关键要点 ====================
 * 
 * DFS的四个方向：
 * - 上：(row-1, col)
 * - 下：(row+1, col)
 * - 左：(row, col-1)
 * - 右：(row, col+1)
 * 
 * 为什么要visited数组？
 * - 防止重复访问
 * - 标记已经属于某个岛屿的陆地
 * 
 * 为什么每行要重新获取colSize？
 * - 网格可能是不规则的（虽然LeetCode通常是规则的）
 * - 安全的做法
 */
