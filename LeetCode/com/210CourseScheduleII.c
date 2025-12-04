#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * LeetCode 210: 课程表 II
 * 
 * 问题描述：
 * 现在你总共有 numCourses 门课要选，记为 0 到 numCourses - 1
 * 给你一个数组 prerequisites，其中 prerequisites[i] = [ai, bi]，
 * 表示在选修课程 ai 前必须先选修 bi
 * 返回你为了学完所有课程所安排的学习顺序（拓扑排序）
 * 如果不可能完成所有课程，返回空数组
 * 
 * 示例：
 * 输入: numCourses = 2, prerequisites = [[1,0]]
 * 输出: [0,1]
 * 解释: 先学课程0，再学课程1
 * 
 * 输入: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 * 输出: [0,1,2,3] 或 [0,2,1,3]
 * 
 * 核心思路：
 * - 拓扑排序（DFS + 后序遍历）
 * - 使用栈存储拓扑排序结果
 * - DFS后序遍历：先访问依赖，后访问课程
 * - 入栈顺序：依赖课程先入栈
 * - 出栈顺序：后进先出，得到拓扑排序
 */

#define MAX_COURSES 2000

/* 栈结构：存储拓扑排序结果 */
typedef struct {
	int courses[MAX_COURSES];  /* 课程数组 */
	int top;                   /* 栈顶索引 */
	int size;                  /* 栈中元素数量 */
} TopologicalStack;

TopologicalStack *topoStack = NULL;

/* 初始化栈 */
void initStack(TopologicalStack **stackPtr) {
	TopologicalStack *stack = (TopologicalStack *)malloc(sizeof(TopologicalStack));
	*stackPtr = stack;
	stack->top = -1;
	stack->size = 0;
}

/* 释放栈 */
void freeStack(TopologicalStack **stackPtr) {
	free(*stackPtr);
	*stackPtr = NULL;
}

/* 入栈 */
void pushCourse(TopologicalStack *stack, int course) {
	stack->courses[++stack->top] = course;
	stack->size++;
}

/* 出栈（LIFO） */
int popCourse(TopologicalStack *stack) {
	int course = stack->courses[stack->top--];
	stack->size--;
	return course;
}

/* 判断栈是否为空 */
bool isStackEmpty(TopologicalStack *stack) {
	return stack->size == 0;
}

/* 三色标记：DFS访问状态 */
typedef enum {
	UNVISITED,  /* 白色：未访问 */
	VISITING,   /* 灰色：正在访问（在当前路径上） */
	VISITED,    /* 黑色：已完成访问 */
} VisitStatus;

/* 边节点：表示先修课程关系 */
typedef struct EdgeNode {
	int adjacentCourse;  /* 邻接课程 */
	struct EdgeNode *next;
} EdgeNode;

/* 顶点节点：表示课程 */
typedef struct VertexNode {
	int courseId;        /* 课程编号 */
	VisitStatus status;  /* DFS访问状态 */
	EdgeNode *edgeList;  /* 先修课程链表 */
} VertexNode;

/* 邻接表图：课程依赖图 */
typedef struct {
	VertexNode *vertices[MAX_COURSES];  /* 课程数组 */
	int numVertices;                    /* 课程数量 */
	int numEdges;                       /* 依赖关系数量 */
} CourseGraph;

CourseGraph *courseGraph = NULL;

/* 初始化课程图 */
void initCourseGraph(CourseGraph **graphPtr) {
	int i;

	CourseGraph *graph = (CourseGraph *)malloc(sizeof(CourseGraph));
	*graphPtr = graph;
	graph->numVertices = 0;
	graph->numEdges = 0;
	for (i = 0; i < MAX_COURSES; i++)
		graph->vertices[i] = NULL;
}

/* 释放课程图内存 */
void freeCourseGraph(CourseGraph **graphPtr) {
	CourseGraph *graph = *graphPtr;
	VertexNode *vertex;
	EdgeNode *edge, *nextEdge;
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		vertex = graph->vertices[i];
		edge = vertex->edgeList;
		while (edge) {
			nextEdge = edge->next;
			free(edge);
			edge = nextEdge;
		}
		free(vertex);
	}

	free(graph);
	*graphPtr = NULL;
}

/* 查找课程节点 */
VertexNode *findCourseNode(CourseGraph *graph, int courseId) {
	VertexNode *vertex = NULL;
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		vertex = graph->vertices[i];
		if (vertex->courseId == courseId)
			return vertex;
	}

	return NULL;
}

/* 检查课程是否存在 */
bool hasCourse(CourseGraph *graph, int courseId) {
	VertexNode *vertex = findCourseNode(graph, courseId);
	return vertex != NULL;
}

/* 检查先修关系是否存在 */
bool hasPrerequisite(CourseGraph *graph, int prerequisite, int course) {
	VertexNode *vertex = findCourseNode(graph, prerequisite);
	if (vertex == NULL)
		return false;
	
	EdgeNode *edge = vertex->edgeList;
	while (edge) {
		if (edge->adjacentCourse == course)
			return true;
		edge = edge->next;
	}

	return false;
}

/* 添加课程节点 */
void addCourse(CourseGraph *graph, int courseId) {
	VertexNode *vertex;

	vertex = findCourseNode(graph, courseId);
	if (vertex != NULL)
		return;

	vertex = (VertexNode *)malloc(sizeof(VertexNode));
	vertex->courseId = courseId;
	vertex->edgeList = NULL;
	vertex->status = UNVISITED;

	graph->vertices[graph->numVertices] = vertex;
	graph->numVertices++;
}

/* 添加先修关系（有向边） */
void addPrerequisite(CourseGraph *graph, int prerequisite, int course) {
	VertexNode *vertex;
	EdgeNode *edge;

	if (hasPrerequisite(graph, prerequisite, course))
		return;

	if (!hasCourse(graph, prerequisite))
		addCourse(graph, prerequisite);
	if (!hasCourse(graph, course))
		addCourse(graph, course);

	vertex = findCourseNode(graph, prerequisite);

	edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	edge->adjacentCourse = course;
	edge->next = vertex->edgeList;
	vertex->edgeList = edge;
	graph->numEdges++;
}

/*
 * DFS检测环并构建拓扑排序
 * @param graph: 课程图
 * @param courseId: 当前课程
 * @return: true表示无环，false表示有环
 * 
 * 算法步骤：
 * 1. 检查是否遇到灰色节点（有环）
 * 2. 检查是否已访问过（黑色节点）
 * 3. 标记为灰色（正在访问）
 * 4. 递归访问所有依赖课程
 * 5. 标记为黑色（完成访问）
 * 6. 入栈（后序遍历顺序）
 */
bool detectCycleAndTopo(CourseGraph *graph, int courseId) {
	VertexNode *vertex;
	EdgeNode *edge;
	bool hasNoCycle;

	vertex = findCourseNode(graph, courseId);
	if (vertex == NULL)
		goto fail;
	
	/* 遇到灰色节点，有环 */
	if (vertex->status == VISITING)
		goto fail;
	
	/* 已访问过，无环 */
	if (vertex->status == VISITED)
		return true;
	
	/* 标记为灰色（正在访问） */
	vertex->status = VISITING;
	
	/* 递归访问所有依赖课程 */
	edge = vertex->edgeList;
	while (edge) {
		hasNoCycle = detectCycleAndTopo(graph, edge->adjacentCourse);
		if (!hasNoCycle)
			goto fail;
		edge = edge->next;
	}

	/* 标记为黑色（完成访问） */
	vertex->status = VISITED;
	
	/* 入栈（后序遍历顺序） */
	pushCourse(topoStack, courseId);
	return true;
	
fail:
	return false;
}

/*
 * 主函数：返回学习课程的顺序（拓扑排序）
 * @param numCourses: 课程总数
 * @param prerequisites: 先修课程关系
 * @param prerequisitesSize: 先修关系数量
 * @param returnSize: 返回数组的大小
 * @return: 拓扑排序结果，如果有环返回空数组
 * 
 * 算法步骤：
 * 1. 构建课程依赖图
 * 2. 添加所有课程（包括没有依赖的独立课程）
 * 3. 对每个未访问的课程进行DFS
 * 4. 从栈中弹出得到拓扑排序
 * 
 * 时间复杂度: O(V+E)
 * 空间复杂度: O(V+E)
 */
int* findOrder(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize, int* returnSize) {
	VertexNode *vertex;
	int *topoOrder;
	bool hasNoCycle;
	int i;

	/* 特殊情况：只有1门课程 */
	if (numCourses == 1) {
		topoOrder = malloc(sizeof(int) * numCourses);
		topoOrder[0] = 0;
		*returnSize = 1;
		return topoOrder;
	}

	/* 初始化栈和图 */
	initStack(&topoStack);
	initCourseGraph(&courseGraph);

	/* 步骤1: 添加所有先修关系（构建图） */
	for (i = 0; i < prerequisitesSize; i++)
		/* prerequisites[i] = [ai, bi] 表示 bi -> ai */
		addPrerequisite(courseGraph, prerequisites[i][1], prerequisites[i][0]);
	
	/* 步骤2: 添加所有课程（包括独立课程） */
	for (i = 0; i < numCourses; i++) {
		if (!hasCourse(courseGraph, i))
			addCourse(courseGraph, i);
	}
	
	/* 步骤3: DFS检测环并构建拓扑排序 */
	for (i = 0; i < courseGraph->numVertices; i++) {
		vertex = courseGraph->vertices[i];
		if (vertex->status == UNVISITED) {
			hasNoCycle = detectCycleAndTopo(courseGraph, vertex->courseId);
			if (!hasNoCycle) {
				/* 有环，无法完成 */
				*returnSize = 0;
				topoOrder = NULL;
				goto cleanup;
			}
		}
	}

	/* 步骤4: 从栈中弹出得到拓扑排序（后进先出） */
	*returnSize = topoStack->size;
	topoOrder = malloc(sizeof(int) * topoStack->size);
	for (i = 0; i < *returnSize; i++) {
		topoOrder[i] = popCourse(topoStack);
	}
	
cleanup:
	freeCourseGraph(&courseGraph);
	freeStack(&topoStack);
	return topoOrder;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS后序遍历 + 栈
 * 
 * 拓扑排序的实现方式：
 * 方法1：Kahn算法（BFS + 入度）
 * 方法2：DFS + 后序遍历（本题方法）
 * 
 * DFS后序遍历的特点：
 * - 先递归访问所有依赖（子节点）
 * - 再处理当前节点
 * - 入栈顺序：依赖先入栈
 * 
 * 为什么反序输出？
 * - DFS后序：依赖课程先完成，后入栈
 * - 栈是LIFO：后入先出
 * - 所以弹出顺序正好是拓扑排序
 * 
 * 示例：prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 * 
 * 图结构：
 *   0 → 1 → 3
 *   ↓   ↗
 *   2
 * 
 * DFS(0):
 *   DFS(1):
 *     DFS(3): push(3)
 *   push(1)
 *   DFS(2):
 *     DFS(3): 已访问，跳过
 *   push(2)
 * push(0)
 * 
 * 栈：[3, 1, 2, 0]
 * 弹出（LIFO）：[0, 2, 1, 3] ✓
 * 
 * 学习顺序：先学0，再学2和1，最后学3
 */