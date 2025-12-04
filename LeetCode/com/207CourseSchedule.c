#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * LeetCode 207: 课程表
 * 
 * 问题描述：
 * 你需要选n门课程，记为 0 到 n-1
 * 在选某些课程之前需要一些先修课程
 * prerequisites[i] = [ai, bi] 表示如果要学习课程ai，必须先学习课程bi
 * 判断是否可能完成所有课程的学习
 * 
 * 示例：
 * 输入: numCourses = 2, prerequisites = [[1,0]]
 * 输出: true
 * 解释: 先学课程0，再学课程1
 * 
 * 输入: numCourses = 2, prerequisites = [[1,0],[0,1]]
 * 输出: false
 * 解释: 0→1→0 形成环，无法完成
 * 
 * 核心思路：
 * - 拓扑排序问题
 * - 有向图环检测
 * - 使用DFS + 三色标记法
 * - 白色（UNVISITED）：未访问
 * - 灰色（VISITING）：正在访问（在当前DFS路径上）
 * - 黑色（VISITED）：已完成访问
 * - 如果访问到灰色节点，说明有环
 */

#define MAX_COURSES 2000

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
	CourseGraph *graph = NULL;
	int i;

	graph = (CourseGraph *)malloc(sizeof(CourseGraph));
	*graphPtr = graph;

	graph->numVertices = 0;
	graph->numEdges = 0;
	for (i = 0; i < MAX_COURSES; i++) {
		graph->vertices[i] = NULL;
	}
}

/* 释放课程图内存 */
void freeCourseGraph(CourseGraph **graphPtr) {
	EdgeNode *edge = NULL, *nextEdge = NULL;
	VertexNode *vertex = NULL;
	CourseGraph *graph = *graphPtr;
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

/* 检查课程是否已存在 */
bool hasCourse(CourseGraph *graph, int courseId) {
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		if (graph->vertices[i]->courseId == courseId)
			return true;
	}
	return false;
}

/* 检查先修关系是否已存在 */
bool hasPrerequisite(CourseGraph *graph, int prerequisite, int course) {
	VertexNode *vertex = NULL;
	EdgeNode *edge = NULL;
	int i;

	for (i = 0; i < graph->numVertices; i++) {
		vertex = graph->vertices[i];
		if (vertex->courseId == prerequisite) {
			edge = vertex->edgeList;
			while (edge) {
				if (edge->adjacentCourse == course)
					return true;
				edge = edge->next;
			}
		}
	}
	return false;
}

/* 添加课程节点 */
void addCourse(CourseGraph *graph, int courseId) {
	VertexNode *vertex = NULL;

	if (hasCourse(graph, courseId))
		return;

	vertex = (VertexNode *)malloc(sizeof(VertexNode));
	vertex->courseId = courseId;
	vertex->edgeList = NULL;
	vertex->status = UNVISITED;

	graph->vertices[graph->numVertices++] = vertex;
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

/* 添加先修关系（有向边） */
void addPrerequisite(CourseGraph *graph, int prerequisite, int course) {
	VertexNode *vertex = NULL;
	EdgeNode *edge = NULL;

	if (!hasCourse(graph, prerequisite))
		addCourse(graph, prerequisite);
	
	if (!hasCourse(graph, course))
		addCourse(graph, course);
	
	if (hasPrerequisite(graph, prerequisite, course))
		return;
	
	vertex = findCourseNode(graph, prerequisite);
	edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	edge->adjacentCourse = course;
	edge->next = vertex->edgeList;
	vertex->edgeList = edge;

	graph->numEdges++;
}

/*
 * DFS检测环（三色标记法）
 * @param graph: 课程图
 * @param courseId: 当前课程
 * @return: true表示无环，false表示有环
 * 
 * 三色标记：
 * - 白色（UNVISITED）：未访问
 * - 灰色（VISITING）：正在访问（在当前DFS路径上）
 * - 黑色（VISITED）：已完成访问（确认无环）
 * 
 * 环检测：
 * - 如果遇到灰色节点，说明回到了当前路径上的节点，形成环
 */
bool detectCycle(CourseGraph *graph, int courseId) {
	VertexNode *vertex = NULL;
	EdgeNode *edge = NULL;
	bool hasNoCycle = false;

	vertex = findCourseNode(graph, courseId);

	/* 情况1: 遇到灰色节点，有环 */
	if (vertex->status == VISITING)
		return false;
	
	/* 情况2: 已访问过的节点，无环 */
	if (vertex->status == VISITED)
		return true;
	
	/* 情况3: 标记为正在访问 */
	vertex->status = VISITING;
	
	/* 情况4: 递归检查所有依赖课程 */
	edge = vertex->edgeList;
	while (edge) {
		hasNoCycle = detectCycle(graph, edge->adjacentCourse);
		if (!hasNoCycle)
			return false;  /* 发现环，立即返回 */
		
		edge = edge->next;
	}

	/* 情况5: 标记为已完成访问 */
	vertex->status = VISITED;
	return true;

}

/*
 * 主函数：判断能否完成所有课程
 * @param numCourses: 课程总数
 * @param prerequisites: 先修课程关系
 * @param prerequisitesSize: 先修关系数量
 * @return: true表示能完成，false表示不能完成
 * 
 * 算法步骤：
 * 1. 构建课程依赖图
 * 2. 对每个未访问的课程进行DFS环检测
 * 3. 如果发现环，返回false
 * 4. 所有课程检查完无环，返回true
 * 
 * 时间复杂度: O(V+E) - V个课程，E个依赖关系
 * 空间复杂度: O(V+E) - 图的存储和递归栈
 */
bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {

	VertexNode *vertex = NULL;
	bool canComplete = true;
	int i;

	/* 没有先修关系，一定能完成 */
	if (prerequisitesSize == 0)
		return true;

	/* 初始化课程图 */
	initCourseGraph(&courseGraph);

	/* 步骤1: 构建课程依赖图 */
	for (i = 0; i < prerequisitesSize; i++) {
		/* prerequisites[i] = [ai, bi] 表示 bi -> ai（先学bi才能学ai） */
		addPrerequisite(courseGraph, prerequisites[i][1], prerequisites[i][0]);
	}

	/* 步骤2: 对每个未访问的课程进行环检测 */
	for (i = 0; i < courseGraph->numVertices; i++) {
		vertex = courseGraph->vertices[i];
		if (vertex->status == UNVISITED) {
			canComplete = detectCycle(courseGraph, vertex->courseId);
			if (!canComplete)
				break;  /* 发现环，无法完成 */
		}
	}

	/* 释放内存 */
	freeCourseGraph(&courseGraph);

	return canComplete;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：拓扑排序 + 环检测
 * 
 * 拓扑排序问题：
 * - 有向无环图（DAG）才能进行拓扑排序
 * - 如果有环，无法完成所有课程
 * 
 * 三色标记法（DFS环检测）：
 * - 白色（UNVISITED）：未访问
 * - 灰色（VISITING）：正在访问（在递归栈上）
 * - 黑色（VISITED）：已访问完成
 * 
 * 环检测原理：
 * - 如果DFS过程中遇到灰色节点，说明回到了当前路径上的节点
 * - 形成环！
 * 
 * 示例1：无环
 * 0 → 1 → 2
 * 
 * DFS(0):
 *   0: UNVISITED → VISITING
 *   DFS(1):
 *     1: UNVISITED → VISITING
 *     DFS(2):
 *       2: UNVISITED → VISITING → VISITED
 *     1: VISITING → VISITED
 *   0: VISITING → VISITED
 * 
 * 没有遇到VISITING节点，无环 ✓
 * 
 * 示例2：有环
 * 0 → 1 → 0
 * 
 * DFS(0):
 *   0: UNVISITED → VISITING
 *   DFS(1):
 *     1: UNVISITED → VISITING
 *     DFS(0):
 *       0: VISITING（灰色！）
 *       return false（检测到环）
 * 
 * 遇到VISITING节点，有环 ✗
 * 
 * 为什么不需要重置状态？
 * - 每个节点只需要DFS一次
 * - VISITED表示该节点及其所有依赖都已检查过
 * - 如果已经是VISITED，不需要重复检查
 * - VISITING状态在DFS返回时会变成VISITED，不会残留
 */