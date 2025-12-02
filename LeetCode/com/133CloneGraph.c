#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

/**
 * Definition for a Node.
 * struct Node {
 *     int val;
 *     int numNeighbors;
 *     struct Node** neighbors;
 * };
 */

struct Node {
	int val;
	int numNeighbors;
	struct Node** neighbors;
};

/*
 * LeetCode 133: 克隆图
 * 
 * 问题描述：
 * 给定无向连通图中一个节点的引用，返回该图的深拷贝（克隆）
 * 
 * 图节点定义：
 * - val: 节点值
 * - neighbors: 邻居节点数组
 * 
 * 示例：
 * 输入: adjList = [[2,4],[1,3],[2,4],[1,3]]
 * 
 * 图结构：
 *   1 --- 2
 *   |     |
 *   4 --- 3
 * 
 * 核心思路：
 * - DFS遍历图
 * - 哈希表存储原节点→克隆节点的映射
 * - 关键：先创建克隆节点并加入哈希表，再克隆邻居（防止环导致无限递归）
 */

/* 哈希表节点：存储原节点→克隆节点的映射 */
typedef struct {
	struct Node *originalNode;  /* 原节点（作为key） */
	struct Node *clonedNode;    /* 克隆节点（作为value） */
	UT_hash_handle hh;
} CloneHashNode;

CloneHashNode *cloneHashMap = NULL;

/* 初始化哈希表 */
void initCloneHashMap() {
	cloneHashMap = NULL;
}

/* 添加映射：原节点 -> 克隆节点 */
void addCloneMapping(struct Node *originalNode, struct Node *clonedNode) {
	CloneHashNode *hashNode = (CloneHashNode*)malloc(sizeof(CloneHashNode));
	hashNode->originalNode = originalNode;
	hashNode->clonedNode = clonedNode;
	HASH_ADD(hh, cloneHashMap, originalNode, sizeof(struct Node*), hashNode);
}

/* 查找克隆节点：返回originalNode对应的克隆节点 */
struct Node *findClonedNode(struct Node *originalNode) {
	CloneHashNode *hashNode;
	HASH_FIND(hh, cloneHashMap, &originalNode, sizeof(struct Node*), hashNode);
	if (hashNode)
		return hashNode->clonedNode;  /* 返回克隆节点 */
	return NULL;
}

/* 释放哈希表 */
void freeCloneHashMap() {
	CloneHashNode *hashNode, *tmp;
	HASH_ITER(hh, cloneHashMap, hashNode, tmp) {
		HASH_DEL(cloneHashMap, hashNode);
		free(hashNode);
	}
	cloneHashMap = NULL;
}

/*
 * DFS克隆图节点
 * @param node: 当前原节点
 * @return: 克隆的节点
 * 
 * 算法步骤：
 * 1. 检查是否已克隆（哈希表查找）
 * 2. 创建克隆节点
 * 3. 立即加入哈希表（防止环导致无限递归）
 * 4. 递归克隆所有邻居
 */
struct Node *dfsClone(struct Node *node) {
	struct Node *clonedNode;
	int i;

	/* 步骤1: 如果已经克隆过，直接返回克隆节点 */
	clonedNode = findClonedNode(node);
	if (clonedNode) {
		return clonedNode;
	}

	/* 步骤2: 创建克隆节点 */
	clonedNode = (struct Node*)malloc(sizeof(struct Node));
	clonedNode->val = node->val;
	clonedNode->numNeighbors = node->numNeighbors;
	clonedNode->neighbors = (struct Node**)malloc(sizeof(struct Node*) * node->numNeighbors);
	
	/* 步骤3: 立即加入哈希表（关键：防止环形图导致无限递归） */
	addCloneMapping(node, clonedNode);

	/* 步骤4: 递归克隆所有邻居 */
	for (i = 0; i < node->numNeighbors; i++) {
		clonedNode->neighbors[i] = dfsClone(node->neighbors[i]);
	}

	return clonedNode;
}

/*
 * 主函数：克隆图
 * @param s: 图中的一个节点
 * @return: 克隆图中对应节点的引用
 * 
 * 时间复杂度: O(N+E) - N个节点，E条边
 * 空间复杂度: O(N) - 哈希表存储N个节点
 */
struct Node *cloneGraph(struct Node *s) {
	struct Node *clonedNode;

	if (NULL == s)
		return NULL;

	/* 初始化哈希表 */
	initCloneHashMap();
	
	/* DFS克隆整个图 */
	clonedNode = dfsClone(s);
	
	/* 释放哈希表 */
	freeCloneHashMap();

	return clonedNode;
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：DFS + 哈希表
 * 
 * 图的深拷贝挑战：
 * - 图可能有环（节点之间互相引用）
 * - 必须保证每个节点只克隆一次
 * - 需要维护原节点→克隆节点的映射
 * 
 * 为什么需要哈希表？
 * - 记录已克隆的节点（防止重复克隆）
 * - 存储原节点→克隆节点的映射
 * - 处理环形结构（节点可能被多次访问）
 * 
 * 为什么要立即加入哈希表？
 * - 在克隆邻居之前就加入
 * - 防止环形图导致无限递归
 * 
 * 例如：节点1 -> 节点2 -> 节点1（环）
 * 1. 克隆节点1，加入哈希表
 * 2. 克隆节点1的邻居（节点2）
 * 3. 克隆节点2，加入哈希表
 * 4. 克隆节点2的邻居（节点1）
 * 5. 查找节点1的克隆（在哈希表中找到）
 * 6. 直接返回，避免无限递归 ✓
 * 
 * ==================== HASH_ADD/HASH_FIND 的参数 ====================
 * 
 * 关键理解：指针作为键
 * 
 * HASH_ADD(hh, head, keyfield, keylen, item)
 * - keyfield: 字段名（originalNode）
 * - keylen: 键的大小 sizeof(struct Node*)（指针的大小）
 * 
 * HASH_FIND(hh, head, keyptr, keylen, out)
 * - keyptr: 指向键的指针
 * - 键是 struct Node* 类型
 * - 要传 &originalNode（指向指针的指针）
 * 
 * 为什么传 &node 而不是 node？
 * - node 是 struct Node*（指针）
 * - HASH_FIND 需要"指向键的指针"
 * - 所以要传 &node（struct Node**）
 * 
 * 类比：
 * - 如果键是 int key，传 &key
 * - 如果键是 struct Node* node，传 &node
 */