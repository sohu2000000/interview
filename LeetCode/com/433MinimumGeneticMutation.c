#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uthash.h"

/*
 * LeetCode 433: 最小基因变化
 * 
 * 问题描述：
 * 基因序列由8个字符组成，每个字符是 'A', 'C', 'G', 'T' 之一
 * 一次基因变化是指基因序列中的一个字符发生变化
 * 给定起始基因startGene、目标基因endGene和基因库bank
 * 求从startGene变化到endGene所需的最少变化次数
 * 每次变化的结果必须在基因库中
 * 
 * 示例：
 * startGene = "AACCGGTT"
 * endGene = "AACCGGTA"
 * bank = ["AACCGGTA"]
 * 输出: 1（只需要变化一个字符）
 * 
 * 核心思路：
 * - BFS最短路径
 * - 每个基因是图的顶点
 * - 相差一个字符的基因之间有边
 * - 使用哈希表优化visited检查（O(1)）
 */

#define MAX_QUEUE_SIZE 1000
#define MAX_GENE_LENGTH 8
#define NUM_GENE_CHARS 4

/* 基因字符集 */
char geneCharSet[NUM_GENE_CHARS] = { 'A', 'C', 'G', 'T' };

/* 访问状态 */
typedef enum {
	UNVISITED,
	VISITED,
} VisitStatus;

/* 基因节点 */
typedef struct {
	char *geneSequence;  /* 基因序列 */
	VisitStatus status;  /* 访问状态 */
} GeneNode;

/* BFS队列 */
typedef struct {
	GeneNode *genes[MAX_QUEUE_SIZE];  /* 基因节点数组 */
	int front;
	int rear;
	int size;
} GeneQueue;

GeneQueue *bfsQueue = NULL;

/* 初始化队列 */
void initGeneQueue(GeneQueue **queuePtr) {
	GeneQueue *queue = NULL;
	int i;

	queue = (GeneQueue *)malloc(sizeof(GeneQueue));
	queue->front = 0;
	queue->rear = 0;
	queue->size = 0;
	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		queue->genes[i] = NULL;
	}

	*queuePtr = queue;
}

/* 释放队列 */
void freeGeneQueue(GeneQueue **queuePtr) {
	GeneQueue *queue = *queuePtr;
	int i;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (queue->genes[i] != NULL) {
			free(queue->genes[i]->geneSequence);
			free(queue->genes[i]);
		}
	}

	free(queue);
	*queuePtr = NULL;
}

/* 创建基因节点 */
GeneNode *createGeneNode(char *gene) {
	GeneNode *node = NULL;

	node = (GeneNode *)malloc(sizeof(GeneNode));
	node->geneSequence = strdup(gene);
	node->status = UNVISITED;
	return node;
}

/* 入队 */
void enqueueGene(GeneQueue *queue, GeneNode *geneNode) {
	queue->genes[queue->rear] = geneNode;
	queue->rear++;
	queue->size++;
}

/* 出队 */
GeneNode *dequeueGene(GeneQueue *queue) {
	GeneNode *geneNode = queue->genes[queue->front];
	queue->front++;
	queue->size--;
	return geneNode;
	}

/* 获取队列大小 */
int getQueueSize(GeneQueue *queue) {
	return queue->size;
}

/* 判断队列是否为空 */
bool isQueueEmpty(GeneQueue *queue) {
	return queue->size == 0;
}

/* 哈希表节点：用于O(1)检查visited */
typedef struct {
	char *geneSequence;  /* 基因序列（作为key） */
	UT_hash_handle hh;
} VisitedGeneHash;

VisitedGeneHash *visitedSet = NULL;

/* 初始化visited哈希表 */
void initVisitedSet() {
	visitedSet = NULL;
}

/* 释放visited哈希表 */
void freeVisitedSet() {
	VisitedGeneHash *current, *tmp;
	HASH_ITER(hh, visitedSet, current, tmp) {
		HASH_DEL(visitedSet, current);
		free(current->geneSequence);
		free(current);
	}
	visitedSet = NULL;
}

/* 标记基因为已访问 */
void markGeneVisited(char *gene) {
	VisitedGeneHash *item = NULL;

	item = (VisitedGeneHash *)malloc(sizeof(VisitedGeneHash));
	item->geneSequence = strdup(gene);

	HASH_ADD_STR(visitedSet, geneSequence, item);
}

/* 检查基因是否已访问（O(1)） */
bool isGeneVisited(char *gene) {
	VisitedGeneHash *item = NULL;
	HASH_FIND_STR(visitedSet, gene, item);
	return item != NULL;
}

/* 检查基因是否在基因库中 */
bool isGeneInBank(char **bank, int bankSize, char *gene) {
	int i;

	for (i = 0; i < bankSize; i++) {
		if (strcmp(bank[i], gene) == 0)
			return true;
	}

	return false;
}

/* 检查基因是否有效 */
bool isGeneValid(char *gene) {
	int i;

	if (strlen(gene) != MAX_GENE_LENGTH)
		return false;

	for (i = 0; i < MAX_GENE_LENGTH; i++) {
		if (gene[i] != 'A' && gene[i] != 'C' && gene[i] != 'G' && gene[i] != 'T')
			return false;	
	}

	return true;
}


/*
 * BFS搜索最短变化路径
 * @param startGene: 起始基因
 * @param endGene: 目标基因
 * @param bank: 基因库
 * @param bankSize: 基因库大小
 * @return: 最少变化次数，无法到达返回-1
 * 
 * 算法步骤：
 * 1. 起始基因入队并标记
 * 2. BFS按层遍历
 * 3. 对每个基因，枚举所有单字符变化
 * 4. 如果变化后的基因在bank中且未访问，入队
 * 5. 到达目标基因时返回步数
 * 
 * 时间复杂度: O(N×8×4×8) = O(256N)，N是bank大小
 * 空间复杂度: O(N)
 */
int bfsMinMutations(char *startGene, char *endGene, char **bank, int bankSize) {
	GeneNode *currentGene = NULL, *mutatedGene = NULL;
	int steps = 0, levelSize = 0;
	char *mutatedSequence = NULL;
	int i, position, charIndex;

	/* 分配临时基因序列 */
	mutatedSequence = (char *)malloc(sizeof(char) * (MAX_GENE_LENGTH + 1));

	/* 起始基因入队并标记 */
	currentGene = createGeneNode(startGene);
	enqueueGene(bfsQueue, currentGene);
	markGeneVisited(startGene);

	/* BFS按层遍历 */
	while (!isQueueEmpty(bfsQueue)) {
		levelSize = getQueueSize(bfsQueue);
		
		/* 处理当前层的所有基因 */
		for (i = 0; i < levelSize; i++) {
			currentGene = dequeueGene(bfsQueue);
			
			/* 到达目标基因 */
			if (!strcmp(currentGene->geneSequence, endGene)) {
				free(mutatedSequence);
				return steps;
			}

			/* 枚举所有可能的单字符变化 */
			strcpy(mutatedSequence, currentGene->geneSequence);
			
			for (position = 0; position < MAX_GENE_LENGTH; position++) {
				for (charIndex = 0; charIndex < NUM_GENE_CHARS; charIndex++) {
					mutatedSequence[position] = geneCharSet[charIndex];
					
					/* 检查变化后的基因是否有效 */
					if (isGeneInBank(bank, bankSize, mutatedSequence)) {
						if (!isGeneVisited(mutatedSequence)) {
							mutatedGene = createGeneNode(mutatedSequence);
							enqueueGene(bfsQueue, mutatedGene);
							markGeneVisited(mutatedSequence);
						}
					}
				}
				/* 恢复原字符，准备变化下一个位置 */
				strcpy(mutatedSequence, currentGene->geneSequence);
			}
		}
		steps++;  /* 每层结束，步数+1 */
	}

	free(mutatedSequence);
	return -1;  /* 无法到达 */
}

/*
 * 主函数：计算最小基因变化次数
 * @param startGene: 起始基因
 * @param endGene: 目标基因
 * @param bank: 基因库
 * @param bankSize: 基因库大小
 * @return: 最少变化次数
 * 
 * 时间复杂度: O(N×L×C)，N是bank大小，L是基因长度，C是字符集大小
 * 空间复杂度: O(N)
 */
int minMutation(char* startGene, char* endGene, char** bank, int bankSize) {
	int minSteps = 0;

	/* 边界检查 */
	if (!isGeneValid(startGene) || !isGeneValid(endGene))
		return -1;
	
	/* 目标基因必须在基因库中 */
	if (!isGeneInBank(bank, bankSize, endGene))
		return -1;

	/* 起始和目标相同 */
	if (!strcmp(startGene, endGene))
		return 0;
	
	/* 初始化队列和visited哈希表 */
	initGeneQueue(&bfsQueue);
	initVisitedSet();
	
	/* BFS搜索最短路径 */
	minSteps = bfsMinMutations(startGene, endGene, bank, bankSize);
	
	/* 释放内存 */
	freeVisitedSet();
	freeGeneQueue(&bfsQueue);

	return minSteps;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：BFS最短路径 + 哈希表优化
 * 
 * 问题建模：
 * - 基因序列 → 图的顶点
 * - 相差一个字符的基因 → 边
 * - 求最短路径：startGene → endGene
 * 
 * BFS特点：
 * - 按层遍历，每层表示一次变化
 * - 第一次到达目标就是最短路径
 * 
 * 优化：
 * - 使用哈希表记录visited：O(1)查询
 * - 不使用哈希表：需要遍历队列O(1000)
 * 
 * 枚举变化：
 * - 8个位置 × 4个字符 = 32种可能的单字符变化
 * - 只保留在bank中的基因
 */