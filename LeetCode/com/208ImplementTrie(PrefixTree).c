#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uthash.h"

/*
 * LeetCode 208: 实现 Trie (前缀树)
 * 
 * 问题描述：
 * 实现一个 Trie (前缀树)，包含 insert、search 和 startsWith 操作
 * 
 * 示例：
 * Trie trie = new Trie();
 * trie.insert("apple");
 * trie.search("apple");   // 返回 true
 * trie.search("app");     // 返回 false
 * trie.startsWith("app"); // 返回 true
 * trie.insert("app");   
 * trie.search("app");     // 返回 true
 * 
 * 核心思路：
 * - 树形结构：每个节点表示一个字符
 * - 使用哈希表存储子节点（O(1)查找）
 * - is_end标记：标识是否是单词结尾
 * - 前向声明处理循环引用
 */

/* 前向声明：解决循环引用 */
struct TrieNode;

/* 哈希表节点：存储字符→子节点的映射 */
typedef struct {
	char character;           /* 字符（作为key） */
	struct TrieNode *childNode;  /* 子节点 */
	UT_hash_handle hh;
} TrieChildHash;

/* Trie节点 */
typedef struct TrieNode {
	char character;           /* 当前字符 */
	bool isWordEnd;          /* 是否是单词结尾 */
	bool isRoot;             /* 是否是根节点 */
	TrieChildHash *children; /* 子节点哈希表 */
} TrieNode;

/*
 * 查找子节点
 * @param parentNode: 父节点
 * @param ch: 要查找的字符
 * @return: 子节点，不存在返回NULL
 */
TrieNode *findChildNode(TrieNode *parentNode, char ch) {
	TrieChildHash *childHashNode = NULL;
	TrieChildHash *childrenHashTable = NULL;

	childrenHashTable = parentNode->children;
	HASH_FIND(hh, childrenHashTable, &ch, sizeof(char), childHashNode);

	if (childHashNode)
		return childHashNode->childNode;

	return NULL;
}

/*
 * 插入子节点到父节点
 * @param parentNode: 父节点
 * @param childNode: 子节点
 */
void insertChildNode(TrieNode *parentNode, TrieNode *childNode) {
	TrieChildHash *childHashNode = NULL;
	TrieChildHash *childrenHashTable = NULL;

	childHashNode = (TrieChildHash *)malloc(sizeof(TrieChildHash));
	childHashNode->character = childNode->character;
	childHashNode->childNode = childNode;

	childrenHashTable = parentNode->children;
	HASH_ADD(hh, childrenHashTable, character, sizeof(char), childHashNode);
	parentNode->children = childrenHashTable;
}

/*
 * 创建Trie节点
 * @param ch: 字符
 * @return: 新创建的节点
 */
TrieNode *createTrieNode(char ch) {
	TrieNode *node = NULL;

	node = (TrieNode *)malloc(sizeof(TrieNode));
	node->character = ch;
	node->isWordEnd = false;
	node->isRoot = false;
	node->children = NULL;
	
	return node;
}

/*
 * 递归释放Trie节点
 * @param nodePtr: 节点指针的地址
 */
void freeTrieNode(TrieNode **nodePtr) {
	TrieNode *node = *nodePtr;
	TrieChildHash *current, *tmp;

	if (node == NULL)
		return;
	
	/* 递归释放所有子节点 */
	if (node->children != NULL) {
		HASH_ITER(hh, node->children, current, tmp) {
			HASH_DEL(node->children, current);
			freeTrieNode(&(current->childNode));
			free(current);
		}
	}

	free(node);
	*nodePtr = NULL;
}

/* Trie树结构 */
typedef struct {
	TrieNode *root;  /* 根节点 */
} Trie;

/*
 * 创建Trie树
 * @return: Trie树实例
 * 
 * 时间复杂度: O(1)
 * 空间复杂度: O(1)
 */
Trie* trieCreate() {
	TrieNode *rootNode = NULL;
	Trie *trie = NULL;

	rootNode = createTrieNode('\0');
	rootNode->isRoot = true;

	trie = (Trie *)malloc(sizeof(Trie));
	trie->root = rootNode;

	return trie;
}

/*
 * 插入单词到Trie树
 * @param obj: Trie树
 * @param word: 要插入的单词
 * 
 * 算法步骤：
 * 1. 从根节点开始
 * 2. 对每个字符，查找子节点
 * 3. 不存在则创建新节点
 * 4. 最后标记单词结尾
 * 
 * 时间复杂度: O(L)，L是单词长度
 * 空间复杂度: O(L)
 */
void trieInsert(Trie* obj, char* word) {
	TrieNode *currentNode = NULL, *childNode = NULL;
	int i, wordLength;
	char ch;

	if (obj == NULL || word == NULL)
		return;

	wordLength = strlen(word);
	currentNode = obj->root;
	
	for (i = 0; i < wordLength; i++) {
		ch = word[i];

		childNode = findChildNode(currentNode, ch);
		if (childNode == NULL) {
			childNode = createTrieNode(ch);
			childNode->isRoot = false;
			insertChildNode(currentNode, childNode);
		}

		currentNode = childNode;
	}

	currentNode->isWordEnd = true;
}

/*
 * 搜索单词是否存在
 * @param obj: Trie树
 * @param word: 要搜索的单词
 * @return: 存在返回true，否则返回false
 * 
 * 算法步骤：
 * 1. 从根节点开始
 * 2. 对每个字符，查找子节点
 * 3. 不存在则返回false
 * 4. 检查最后节点是否是单词结尾
 * 
 * 时间复杂度: O(L)
 * 空间复杂度: O(1)
 */
bool trieSearch(Trie* obj, char* word) {
	TrieNode *currentNode = NULL, *childNode = NULL;
	int i, wordLength;
	char ch;

	wordLength = strlen(word);
	currentNode = obj->root;
	
	for (i = 0; i < wordLength; i++) {
		ch = word[i];
		childNode = findChildNode(currentNode, ch);
		if (childNode == NULL) {
			return false;
		}

		currentNode = childNode;
	}

	return currentNode->isWordEnd;
}

/*
 * 检查前缀是否存在
 * @param obj: Trie树
 * @param prefix: 前缀字符串
 * @return: 存在返回true，否则返回false
 * 
 * 算法步骤：
 * 1. 从根节点开始
 * 2. 对每个字符，查找子节点
 * 3. 不存在则返回false
 * 4. 全部找到返回true（不需要检查isWordEnd）
 * 
 * 时间复杂度: O(L)
 * 空间复杂度: O(1)
 */
bool trieStartsWith(Trie* obj, char* prefix) {
	TrieNode *currentNode = NULL, *childNode = NULL;
	int i, prefixLength;
	char ch;

	prefixLength = strlen(prefix);
	currentNode = obj->root;

	for (i = 0; i < prefixLength; i++) {
		ch = prefix[i];

		childNode = findChildNode(currentNode, ch);
		if (childNode == NULL) {
			return false;
		}

		currentNode = childNode;
	}

	return true;
}

/*
 * 释放Trie树
 * @param obj: Trie树
 */
void trieFree(Trie* obj) {
	freeTrieNode(&(obj->root));
	free(obj);
}

/*
 * ==================== 算法详解 ====================
 * 
 * Trie树（前缀树/字典树）特点：
 * - 根节点不包含字符
 * - 每个节点的子节点共享相同前缀
 * - 从根到某节点的路径构成一个字符串
 * - 使用哈希表存储子节点：O(1)查找
 * 
 * 示例：插入 "app", "apple", "bat"
 * 
 *        root
 *       /    \
 *      a      b
 *      |      |
 *      p      a
 *      |      |
 *      p*     t*
 *      |
 *      l
 *      |
 *      e*
 * 
 * (*表示单词结尾)
 * 
 * 前向声明的作用：
 * - hash_child_node 包含 trie_node*
 * - trie_node 包含 hash_child_node_t*
 * - 循环引用！需要前向声明 struct trie_node
 * 
 * 使用哈希表的优势：
 * - 查找子节点：O(1)
 * - 传统数组方式：需要26个指针（假设只有小写字母）
 * - 哈希表方式：只存储存在的子节点，节省空间
 */

/**
 * Your Trie struct will be instantiated and called as such:
 * Trie* obj = trieCreate();
 * trieInsert(obj, word);
 
 * bool param_2 = trieSearch(obj, word);
 
 * bool param_3 = trieStartsWith(obj, prefix);
 
 * trieFree(obj);
*/