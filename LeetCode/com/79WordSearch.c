#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * LeetCode 79: 单词搜索
 * 
 * 问题描述：
 * 给定一个 m×n 的字符网格 board 和一个字符串 word
 * 如果 word 存在于网格中，返回 true；否则，返回 false
 * 单词必须按照字母顺序，通过相邻的单元格内的字母构成
 * 相邻单元格：水平或垂直相邻，同一单元格内的字母不允许被重复使用
 * 
 * 示例：
 * board = [["A","B","C","E"],
 *          ["S","F","C","S"],
 *          ["A","D","E","E"]]
 * word = "ABCCED"
 * 输出：true
 * 
 * 核心思路：
 * - DFS回溯+visited标记
 * - 从每个格子开始尝试匹配
 * - 四个方向递归搜索
 * - 回溯时恢复visited状态
 */

typedef enum {
	UNVISITED = 0,
	VISITED = 1,
} VisitStatus;

/*
 * DFS搜索单词
 * @param board: 字符网格
 * @param numRows: 行数
 * @param numCols: 每行的列数
 * @param targetWord: 目标单词
 * @param wordLength: 单词长度
 * @param currentIndex: 当前匹配到的字符索引
 * @param isVisited: 访问标记数组
 * @param currentRow: 当前行
 * @param currentCol: 当前列
 * @return: 找到返回true
 * 
 * 算法步骤：
 * 1. 终止条件：匹配完所有字符
 * 2. 边界检查：越界或已访问
 * 3. 字符匹配：当前格子字符 == word[index]
 * 4. 标记访问，递归四个方向
 * 5. 回溯：恢复未访问状态
 */
bool dfsSearchWord(char **board, int numRows, int *numCols,
		 char *targetWord, int wordLength, int currentIndex,
		 int **isVisited, int currentRow, int currentCol) {

	/* 终止条件：匹配完所有字符 */
	if (currentIndex == wordLength)
		return true;
	
	/* 边界检查：越界或已访问 */
	if (currentRow < 0 || currentRow >= numRows || 
	    currentCol < 0 || currentCol >= numCols[currentRow] || 
	    isVisited[currentRow][currentCol] == VISITED)
		return false;
	
	/* 字符匹配检查 */
	if (targetWord[currentIndex] != board[currentRow][currentCol])
		return false;

	/* 标记为已访问 */
	isVisited[currentRow][currentCol] = VISITED;

	/* 递归搜索四个方向 */
	if (dfsSearchWord(board, numRows, numCols, targetWord, wordLength, currentIndex + 1, isVisited, currentRow - 1, currentCol))
		return true;  /* 上 */
	if (dfsSearchWord(board, numRows, numCols, targetWord, wordLength, currentIndex + 1, isVisited, currentRow + 1, currentCol))
		return true;  /* 下 */
	if (dfsSearchWord(board, numRows, numCols, targetWord, wordLength, currentIndex + 1, isVisited, currentRow, currentCol - 1))
		return true;  /* 左 */
	if (dfsSearchWord(board, numRows, numCols, targetWord, wordLength, currentIndex + 1, isVisited, currentRow, currentCol + 1))
		return true;  /* 右 */

	/* 回溯：恢复未访问状态 */
	isVisited[currentRow][currentCol] = UNVISITED;
	return false;

}

/*
 * 主函数：判断单词是否存在于网格中
 * @param board: 字符网格
 * @param boardSize: 行数
 * @param boardColSize: 每行的列数
 * @param word: 目标单词
 * @return: 存在返回true
 * 
 * 时间复杂度: O(m×n×4^L)，m行n列，L是单词长度
 * 空间复杂度: O(m×n + L)，visited数组 + 递归栈
 */
bool exist(char** board, int boardSize, int* boardColSize, char* word) {

	int **isVisited = NULL;
	bool found = false;
	int i, j;

	/* 分配visited数组 */
	isVisited = (int **)malloc(boardSize * sizeof(int *));
	for (i = 0; i < boardSize; i++) {
		isVisited[i] = (int *)malloc(boardColSize[i] * sizeof(int));
		memset(isVisited[i], 0, boardColSize[i] * sizeof(int));
	}

	/* 遍历所有起始位置 */
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardColSize[i]; j++) {
			if (dfsSearchWord(board, boardSize, boardColSize, word, strlen(word), 0, isVisited, i, j)) {
				found = true;
				goto cleanup;
			}
		}
	}
	
cleanup:
	/* 释放visited数组 */
	for (i = 0; i < boardSize; i++)
		free(isVisited[i]);
	free(isVisited);
	
	return found;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS回溯 + visited标记
 * 
 * 与普通DFS的区别：
 * - 需要回溯visited状态
 * - 同一个格子在一条路径中只能使用一次
 * - 但不同路径可以重复使用同一格子
 * 
 * 示例：board = [["A","B"],["C","D"]], word = "ABCD"
 * 
 * 从(0,0)开始：
 * A → B → ?（B的邻居没有C）
 *     回溯，B恢复未访问
 * A → ?（A的其他邻居）
 * 
 * 回溯的重要性：
 * - 如果不回溯，visited[0][1]永远是VISITED
 * - 从其他起点可能需要访问(0,1)
 * 
 * 四个方向：
 * - 上：row-1
 * - 下：row+1
 * - 左：col-1
 * - 右：col+1
 */