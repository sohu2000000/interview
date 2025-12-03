#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * LeetCode 399: 除法求值
 * 
 * 问题描述：
 * 给定方程组 equations 和对应的值 values，其中 equations[i] = [Ai, Bi]，values[i] 表示 Ai / Bi = values[i]
 * 给定查询 queries，queries[j] = [Cj, Dj]，表示查询 Cj / Dj = ?
 * 
 * 示例：
 * equations = [["a","b"],["b","c"]]
 * values = [2.0, 3.0]
 * queries = [["a","c"],["b","a"],["a","e"],["a","a"],["x","x"]]
 * 
 * 已知：a/b = 2.0, b/c = 3.0
 * 查询：
 * - a/c = (a/b) * (b/c) = 2.0 * 3.0 = 6.0
 * - b/a = 1 / (a/b) = 1 / 2.0 = 0.5
 * - a/e = -1.0 (e不存在)
 * - a/a = 1.0
 * - x/x = -1.0 (x不存在)
 * 
 * 核心思路：
 * - 构建有向带权图：变量是顶点，除法关系是边
 * - a/b = 2.0 表示：边 a->b 权重为2.0，边 b->a 权重为0.5
 * - 查询 a/c：在图中找从a到c的路径，累积路径权重
 * - 使用DFS遍历图，累积路径权重
 */

#define MAX_VERTICES 100

/* 边节点：邻接表的边 */
typedef struct EdgeNode {
	int adjacentVertex;  /* 相邻顶点的索引 */
	double weight;       /* 边的权重（除法结果） */
	struct EdgeNode *next;
} EdgeNode;

/* 顶点节点：存储变量名和出边链表 */
typedef struct VertexNode {
	char *variableName;  /* 变量名（如 "a", "b"） */
	struct EdgeNode *edgeList;  /* 出边链表 */
} VertexNode;

/* 邻接表图结构 */
typedef struct {
	VertexNode vertices[MAX_VERTICES];  /* 顶点数组 */
	int visited[MAX_VERTICES];          /* DFS访问标记 */
	int numVertices;                    /* 顶点数量 */
	int numEdges;                       /* 边数量 */
} Graph;

Graph *globalGraph = NULL;

/* 重置所有顶点的访问标记 */
void resetVisited(Graph *graph) {
	int i;

	for (i = 0; i < MAX_VERTICES; i++) {
		graph->visited[i] = 0;
	}
}

/* 初始化图 */
void initGraph(Graph **graphPtr) {
	Graph *graph = NULL;
	int i;

	graph = (Graph *)malloc(sizeof(Graph));
	*graphPtr = graph;
	graph->numVertices = 0;
	graph->numEdges = 0;
	for (i = 0; i < MAX_VERTICES; i++) {
		graph->vertices[i].variableName = NULL;
		graph->vertices[i].edgeList = NULL;
	}

	resetVisited(graph);
}

/* 释放图的内存 */
void freeGraph(Graph **graphPtr) {
	EdgeNode *edge = NULL, *nextEdge = NULL;
	Graph *graph = *graphPtr;
	VertexNode *vertex = NULL;
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		vertex = &graph->vertices[i];
		edge = vertex->edgeList;

		while (edge != NULL) {
			nextEdge = edge->next;
			free(edge);
			edge = nextEdge;
		}
	}

	free(graph);
	*graphPtr = NULL;
}

/* 查找变量在图中的索引 */
int findVertexIndex(Graph *graph, char *variableName) {
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		if (0 == strcmp(graph->vertices[i].variableName, variableName))
			return i;
	}

	return -1;
}

/* 添加顶点（变量） */
void addVertex(Graph *graph, char *variableName) {
	int index;

	index = findVertexIndex(graph, variableName);
	/* 如果顶点不存在，则添加顶点 */
	if (index == -1) {
		graph->vertices[graph->numVertices].variableName = strdup(variableName);
		graph->numVertices++;
	}
}

/* 检查边是否已存在 */
bool hasEdge(Graph *graph, char *from, char *to) {
	VertexNode *vertex = NULL;
	EdgeNode *edge = NULL;
	int fromIndex, toIndex;

	fromIndex = findVertexIndex(graph, from);
	toIndex = findVertexIndex(graph, to);

	if (fromIndex == -1 || toIndex == -1)
		return false;

	vertex = &graph->vertices[fromIndex];
	edge = vertex->edgeList;
	while (edge != NULL) {
		if (edge->adjacentVertex == toIndex)
			return true;
		edge = edge->next;
	}

	return false;

}

/*
 * 添加边（双向）
 * @param graph: 图
 * @param from: 起点变量名
 * @param to: 终点变量名
 * @param weight: 边的权重（from/to的值）
 * 
 * 算法步骤：
 * 1. 确保from和to顶点存在（不存在则添加）
 * 2. 检查边是否已存在（避免重复添加）
 * 3. 添加双向边：from->to 和 to->from
 */
void addEdge(Graph *graph, char *from, char *to, double weight) {
	EdgeNode *edge = NULL;
	int fromIndex, toIndex;

	/* 步骤1: 确保from顶点存在 */
	fromIndex = findVertexIndex(graph, from);
	if (fromIndex == -1) {
		addVertex(graph, from);
	}

	/* 步骤2: 确保to顶点存在 */
	toIndex = findVertexIndex(graph, to);
	if (toIndex == -1) {
		addVertex(graph, to);
	}

	/* 步骤3: 检查边是否已存在，避免重复添加 */
	if (hasEdge(graph, from, to))
		return;

	/* 步骤4: 获取顶点索引（现在一定存在） */
	fromIndex = findVertexIndex(graph, from);
	toIndex = findVertexIndex(graph, to);

	/* 步骤5: 添加正向边 from -> to */
	edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	edge->adjacentVertex = toIndex;
	edge->weight = weight;
	edge->next = graph->vertices[fromIndex].edgeList;
	graph->vertices[fromIndex].edgeList = edge;
	graph->numEdges++;

	/* 步骤6: 添加反向边 to -> from（权重是倒数） */
	edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	edge->adjacentVertex = fromIndex;
	edge->weight = 1.0 / weight;
	edge->next = graph->vertices[toIndex].edgeList;
	graph->vertices[toIndex].edgeList = edge;
	graph->numEdges++;
}

/*
 * DFS搜索路径并计算累积权重
 * @param graph: 图
 * @param currentVertex: 当前顶点
 * @param accumulatedWeight: 从起点到当前顶点的累积权重
 * @param targetVariable: 目标变量名
 * @return: 找到返回累积权重，找不到返回-1.0
 * 
 * 算法步骤：
 * 1. 如果当前节点就是目标，返回累积权重
 * 2. 标记当前节点为已访问
 * 3. 遍历所有未访问的邻居
 * 4. 递归DFS，累积权重（乘以边的权重）
 * 5. 找到目标立即返回，否则返回-1.0
 */
double dfsSearch(Graph *graph, VertexNode *currentVertex, double accumulatedWeight, char *targetVariable) {
	VertexNode *neighborVertex = NULL;
	EdgeNode *edge = NULL;
	double resultWeight = 1.0;
	int currentIndex, neighborIndex;

	/* 空指针检查 */
	if (currentVertex == NULL || currentVertex->variableName == NULL || targetVariable == NULL)
		return -1.0;

	/* 步骤1: 找到目标，返回累积权重 */
	if (0 == strcmp(currentVertex->variableName, targetVariable))
		return accumulatedWeight;

	/* 步骤2: 标记当前节点为已访问 */
	currentIndex = findVertexIndex(graph, currentVertex->variableName);
	graph->visited[currentIndex] = 1;
	
	/* 步骤3: 遍历所有邻居 */
	edge = currentVertex->edgeList;
	while (edge != NULL) {
		neighborIndex = edge->adjacentVertex;
		neighborVertex = &graph->vertices[neighborIndex];
		
		/* 步骤4: 如果邻居未访问，递归DFS */
		if (graph->visited[neighborIndex] == 0) {
			resultWeight = dfsSearch(graph, neighborVertex, accumulatedWeight * edge->weight, targetVariable);
			
			/* 步骤5: 找到目标，立即返回 */
			if (resultWeight != -1.0)
				return resultWeight;
		}
		
		edge = edge->next;
	}
	
	/* 所有路径都试过了，找不到目标 */
	return -1.0;
}

/*
 * 主函数：计算除法表达式的值
 * @param equations: 方程数组
 * @param equationsSize: 方程数量
 * @param values: 方程的值
 * @param queries: 查询数组
 * @param queriesSize: 查询数量
 * @param returnSize: 返回数组的大小
 * @return: 查询结果数组
 * 
 * 时间复杂度: O((E+Q)*(V+E)) - E个方程，Q个查询，V个变量
 * 空间复杂度: O(V+E) - 图的存储
 */
double* calcEquation(char*** equations, int equationsSize, int* equationsColSize, double* values, int valuesSize, char*** queries, int queriesSize, int* queriesColSize, int* returnSize) {

	int fromIndex = -1, toIndex = -1;
	char *fromVariable = NULL, *toVariable = NULL;
	VertexNode *startVertex = NULL;
	double *resultArray = NULL;
	int i;

	/* 分配结果数组 */
	resultArray = (double *)malloc(sizeof(double) * queriesSize);
	*returnSize = queriesSize;

	/* 初始化图 */
	initGraph(&globalGraph);
	
	/* 检查初始化是否成功 */
	if (globalGraph == NULL)
		return resultArray;

	/* 步骤1: 构建图（添加所有方程的边） */
	for (i = 0; i < equationsSize; i++) {
		addEdge(globalGraph, equations[i][0], equations[i][1], values[i]);
	}

	/* 步骤2: 处理每个查询 */
	for (i = 0; i < queriesSize; i++) {
		fromVariable = queries[i][0];
		toVariable = queries[i][1];
		
		/* 查找变量在图中的索引 */
		fromIndex = findVertexIndex(globalGraph, fromVariable);
		toIndex = findVertexIndex(globalGraph, toVariable);

		/* 情况1: 变量不存在，返回-1.0 */
		if (-1 == fromIndex || -1 == toIndex) {
			resultArray[i] = -1.0;
			continue;
		}

		/* 情况2: 相同变量，a/a = 1.0 */
		if (0 == strcmp(fromVariable, toVariable)) {
			resultArray[i] = 1.0;
			continue;
		}

		/* 情况3: 使用DFS在图中搜索路径 */
		startVertex = &globalGraph->vertices[fromIndex];
		resetVisited(globalGraph);
		resultArray[i] = dfsSearch(globalGraph, startVertex, 1.0, toVariable); 
	}

	/* 释放图的内存 */
	freeGraph(&globalGraph);
	
	return resultArray;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：图的建模 + DFS路径搜索
 * 
 * 问题建模：
 * - 变量 → 图的顶点
 * - 除法关系 → 图的边（带权重）
 * - a/b = 2.0 → 边 a->b 权重2.0，边 b->a 权重0.5
 * 
 * 查询求解：
 * - a/c = ? → 找从a到c的路径，累积权重
 * - 如果路径是 a->b->c，累积权重 = (a/b) * (b/c)
 * 
 * 示例：
 * equations = [["a","b"],["b","c"]]
 * values = [2.0, 3.0]
 * 
 * 图结构：
 *   a --2.0--> b --3.0--> c
 *   a <-0.5--- b <-0.33-- c
 * 
 * 查询 a/c：
 * - DFS从a开始
 * - a -> b（权重2.0）
 * - b -> c（权重3.0）
 * - 累积权重：2.0 * 3.0 = 6.0
 * 
 * 关键要点：
 * - 双向边：a/b=2.0 → a->b权重2.0，b->a权重0.5
 * - DFS标记visited：防止环导致无限循环
 * - 每次查询前重置visited：独立搜索
 * - 路径累积：沿着边累乘权重
 */