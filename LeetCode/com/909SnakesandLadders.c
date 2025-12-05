#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * LeetCode 909: 蛇梯棋
 * 
 * 问题描述：
 * N×N的棋盘，格子从1到N²编号，采用之字形（Boustrophedon）编号
 * 某些格子有蛇或梯子，到达后自动跳转
 * 从格子1开始，掷骰子（1-6）前进，求到达格子N²的最少步数
 * 
 * 示例：
 * 输入: board = [[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,35,-1,-1,13,-1],[-1,-1,-1,-1,-1,-1],[-1,15,-1,-1,-1,-1]]
 * 输出: 4
 * 
 * 核心思路：
 * - 建图：格子是顶点，可到达关系是边
 * - BFS最短路径：从格子1到格子N²
 * - 之字形编号处理
 */

#define MAX_SQUARES 2000  /* 最大格子数 */
#define MAX_DICE 6        /* 骰子最大值 */

/* 之字形方向枚举 */
typedef enum {
	LEFT_TO_RIGHT,  /* 从左到右 */
	RIGHT_TO_LEFT,  /* 从右到左 */
} Direction;

/* BFS队列 */
typedef struct {
	int squares[MAX_SQUARES];  /* 格子编号数组 */
	int front;
	int rear;
	int size;
} BFSQueue;

BFSQueue *bfsQueue = NULL;

/* 初始化队列 */
void initQueue(BFSQueue **queuePtr) {
	BFSQueue *queue = NULL;

	queue = (BFSQueue *)malloc(sizeof(BFSQueue));
	queue->front = 0;
	queue->rear = 0;
	queue->size = 0;
	*queuePtr = queue;
}

/* 释放队列 */
void freeQueue(BFSQueue **queuePtr) {
	BFSQueue *queue = *queuePtr;
	free(queue);
	*queuePtr = NULL;
}

/* 入队 */
void enqueueSquare(BFSQueue *queue, int squareNum) {
	queue->squares[queue->rear] = squareNum;
	queue->rear++;
	queue->size++;
}

/* 出队 */
int dequeueSquare(BFSQueue *queue) {
	int squareNum = queue->squares[queue->front];
	queue->front++;
	queue->size--;
	return squareNum;
}

/* 获取队列大小 */
int getQueueSize(BFSQueue *queue) {
	return queue->size;
}

/* 判断队列是否为空 */
bool isQueueEmpty(BFSQueue *queue) {
	return queue->size == 0;
}

/* 访问状态枚举 */
typedef enum {
	UNVISITED,  /* 未访问 */
	VISITED,    /* 已访问 */
} VisitStatus;

/* 边节点：表示可到达的格子 */
typedef struct EdgeNode {
	int targetSquare;  /* 目标格子编号 */
	struct EdgeNode *next;
} EdgeNode;

/* 顶点节点：表示棋盘格子 */
typedef struct SquareNode {
	int squareNum;      /* 格子编号 */
	int numEdges;       /* 出边数量 */
	VisitStatus status; /* 访问状态 */
	EdgeNode *edgeList; /* 可到达的格子链表 */
} SquareNode;

/* 图结构：棋盘图 */
typedef struct {
	SquareNode *squares[MAX_SQUARES];  /* 格子数组 */
	int numSquares;                    /* 格子数量 */
	int numEdges;                      /* 边数量 */
} BoardGraph;

BoardGraph *boardGraph = NULL;

/* 初始化图 */
void initBoardGraph(BoardGraph **graphPtr) {
	BoardGraph *graph = NULL;
	int i;

	graph = (BoardGraph *)malloc(sizeof(BoardGraph));
	graph->numSquares = 0;
	graph->numEdges = 0;
	for (i = 0; i < MAX_SQUARES; i++) {
		graph->squares[i] = NULL;
	}
	*graphPtr = graph;
}

/* 释放图内存 */
void freeBoardGraph(BoardGraph **graphPtr) {
	BoardGraph *graph = *graphPtr;
	EdgeNode *edge, *nextEdge;
	SquareNode *square;
	int i;

	for (i = 0; i < graph->numSquares; i++) {
		square = graph->squares[i];
		edge = square->edgeList;
		while (edge != NULL) {
			nextEdge = edge->next;
			free(edge);
			edge = nextEdge;
		}
		free(square);
	}

	free(graph);
	*graphPtr = NULL;
}

/* 查找格子节点 */
SquareNode *findSquare(BoardGraph *graph, int squareNum) {
	SquareNode *square = NULL;
	int i;

	for (i = 0; i < graph->numSquares; i++) {
		square = graph->squares[i];
		if (square->squareNum == squareNum)
			return square;
	}

	return NULL;
}

/* 检查格子是否存在 */
bool hasSquare(BoardGraph *graph, int squareNum) {
	SquareNode *square = findSquare(graph, squareNum);
	return square != NULL;
}

/* 检查边是否存在 */
bool hasEdge(BoardGraph *graph, int from, int to) {
	SquareNode *square = findSquare(graph, from);
	EdgeNode *edge;

	if (square == NULL)
		return false;
	
	edge = square->edgeList;
	while (edge != NULL) {
		if (edge->targetSquare == to)
			return true;
		edge = edge->next;
	}

	return false;
}

/* 添加格子节点 */
void addSquare(BoardGraph *graph, int squareNum) {
	SquareNode *square = findSquare(graph, squareNum);
	if (square != NULL)
		return;
	
	square = (SquareNode *)malloc(sizeof(SquareNode));
	square->squareNum = squareNum;
	square->numEdges = 0;
	square->status = UNVISITED;
	square->edgeList = NULL;

	graph->squares[graph->numSquares] = square;
	graph->numSquares++;
}

/* 添加边（格子之间的可达关系） */
void addMove(BoardGraph *graph, int fromSquare, int toSquare) {
	SquareNode *square = NULL;
	EdgeNode *edge = NULL;

	if (!hasSquare(graph, fromSquare))
		addSquare(graph, fromSquare);
	if (!hasSquare(graph, toSquare))
		addSquare(graph, toSquare);
	
	if (hasEdge(graph, fromSquare, toSquare))
		return;

	square = findSquare(graph, fromSquare);
	edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	edge->targetSquare = toSquare;
	edge->next = square->edgeList;
	square->edgeList = edge;
	square->numEdges++;
	graph->numEdges++;
}


/*
 * BFS搜索最短路径
 * @param graph: 棋盘图
 * @param startSquare: 起始格子
 * @param targetSquare: 目标格子
 * @return: 最少步数，无法到达返回-1
 * 
 * 算法步骤：
 * 1. 起始格子入队并标记
 * 2. 按层BFS遍历
 * 3. 到达目标立即返回步数
 * 4. 邻居入队时立即标记（防重复）
 * 
 * 时间复杂度: O(N²)
 * 空间复杂度: O(N²)
 */
int bfsShortestPath(BoardGraph *graph, int startSquare, int targetSquare) {
	int currentSquare = 0, steps = 0, levelSize = 0, i;
	SquareNode *square = NULL, *adjacentSquare = NULL;
	EdgeNode *edge = NULL;

	/* 起始格子入队并标记 */
	enqueueSquare(bfsQueue, startSquare);
	square = findSquare(graph, startSquare);
	square->status = VISITED;

	/* BFS按层遍历 */
	while (!isQueueEmpty(bfsQueue)) {
		levelSize = getQueueSize(bfsQueue);

		/* 处理当前层的所有格子 */
		for (i = 0; i < levelSize; i++) {
			currentSquare = dequeueSquare(bfsQueue);
			
			/* 到达目标，返回步数 */
			if (currentSquare == targetSquare)
				return steps;
			
			/* 遍历所有可到达的格子 */
			square = findSquare(graph, currentSquare);
			edge = square->edgeList;
		while (edge != NULL) {
				adjacentSquare = findSquare(graph, edge->targetSquare);
				if (adjacentSquare->status == UNVISITED) {
					adjacentSquare->status = VISITED;
					enqueueSquare(bfsQueue, edge->targetSquare);
			}
			edge = edge->next;
		}
	}
		steps++;  /* 每层结束，步数+1 */
	}

	return -1;  /* 无法到达 */
}

/*
 * 主函数：蛇梯棋最少步数
 * @param board: 棋盘（-1表示普通格子，其他值表示蛇/梯子的目标）
 * @param boardSize: 棋盘大小（N×N）
 * @return: 最少步数，无法到达返回-1
 * 
 * 算法步骤：
 * 1. 构建squareMap：将棋盘坐标映射到格子编号（之字形）
 * 2. 构建图：每个格子可以走1-6步到达其他格子
 * 3. BFS搜索从格子1到格子N²的最短路径
 * 
 * 时间复杂度: O(N²)
 * 空间复杂度: O(N²)
 */
int snakesAndLadders(int** board, int boardSize, int* boardColSize) {
	Direction direction = LEFT_TO_RIGHT;
	int **squareMap = NULL;  /* 棋盘坐标→格子编号的映射 */
	int finalSquare = 1, minSteps = -1, i, j, k;
	int nextRow, nextCol, destinationSquare;

	/* 分配squareMap数组 */
	squareMap = (int **)malloc(sizeof(int *) * boardSize);
	for (i = 0; i < boardSize; i++) {
		squareMap[i] = (int *)malloc(sizeof(int) * boardColSize[i]);
		for (j = 0; j < boardColSize[i]; j++) {
			squareMap[i][j] = 0;
		}
	}

	/* 初始化队列和图 */
	initQueue(&bfsQueue);
	initBoardGraph(&boardGraph);

	/* 步骤1: 构建squareMap（之字形编号） */
	for (i = boardSize - 1; i >= 0; i--) {
		for (j = 0; j < boardColSize[i]; j++) {
			if (direction == LEFT_TO_RIGHT) {
				squareMap[i][j] = finalSquare;
				finalSquare++;
			} else {
				squareMap[i][boardColSize[i] - j - 1] = finalSquare;
				finalSquare++;
			}
		}
		direction = direction == LEFT_TO_RIGHT ? RIGHT_TO_LEFT : LEFT_TO_RIGHT;
	}
	finalSquare--;  /* 最终格子编号 */

	/* 步骤2: 建图 - 遍历每个格子编号 */
	for (int squareNum = 1; squareNum < finalSquare; squareNum++) {
		/* 从格子squareNum尝试走1-6步 */
		for (k = 1; k <= MAX_DICE && squareNum + k <= finalSquare; k++) {
			int nextSquare = squareNum + k;
			
			/* 在squareMap中查找nextSquare对应的坐标 */
			int found = 0;
			for (nextRow = 0; nextRow < boardSize && !found; nextRow++) {
				for (nextCol = 0; nextCol < boardColSize[nextRow] && !found; nextCol++) {
					if (squareMap[nextRow][nextCol] == nextSquare) {
						found = 1;
						break;
					}
				}
				if (found) break;
			}
			
			/* 检查目标格子是否有蛇/梯子 */
			if (board[nextRow][nextCol] != -1) {
				destinationSquare = board[nextRow][nextCol];  /* 有蛇/梯子，跳转 */
			} else {
				destinationSquare = nextSquare;  /* 普通格子，停留 */
			}
			
			addMove(boardGraph, squareNum, destinationSquare);
			}
		}

	/* 步骤3: BFS搜索最短路径 */
	minSteps = bfsShortestPath(boardGraph, 1, finalSquare);

	/* 释放内存 */
	freeBoardGraph(&boardGraph);
	freeQueue(&bfsQueue);

	for (i = 0; i < boardSize; i++) {
		free(squareMap[i]);
	}
	free(squareMap);

	return minSteps;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：图的建模 + BFS最短路径
 * 
 * 问题建模：
 * - 格子 → 图的顶点
 * - 掷骰子可到达的格子 → 边
 * - 蛇/梯子 → 跳转（修改目标格子）
 * 
 * 之字形编号（Boustrophedon）：
 * 
 * 6×6棋盘示例：
 * 36 35 34 33 32 31
 * 25 26 27 28 29 30
 * 24 23 22 21 20 19
 * 13 14 15 16 17 18
 * 12 11 10  9  8  7
 *  1  2  3  4  5  6
 * 
 * 从左下角(1)开始，第1行从左到右，第2行从右到左...
 * 
 * 建图规则：
 * - 从格子num，可以走1-6步
 * - 到达nextPos后，检查是否有蛇/梯子
 * - 有：跳到board[row][col]
 * - 无：停在nextPos
 * 
 * BFS最短路径：
 * - 从格子1开始
 * - 每层表示一次掷骰子
 * - 找到到达格子N²的最少步数
 */