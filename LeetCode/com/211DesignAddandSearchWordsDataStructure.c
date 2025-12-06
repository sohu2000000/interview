#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uthash.h"

/*
 * LeetCode 211: 添加与搜索单词 - 数据结构设计
 * 
 * 问题描述：
 * 设计一个数据结构，支持添加单词和搜索单词（支持'.'通配符）
 * '.' 可以表示任何一个字母
 * 
 * 示例：
 * addWord("bad")
 * addWord("dad")
 * addWord("mad")
 * search("pad") -> false
 * search("bad") -> true
 * search(".ad") -> true（匹配bad, dad, mad）
 * search("b..") -> true（匹配bad）
 * 
 * 核心思路：
 * - Trie树 + DFS递归搜索
 * - 普通字符：直接查找子节点
 * - 通配符'.'：遍历所有子节点，递归匹配
 * - 使用哈希表存储子节点（O(1)查找）
 */

/* 前向声明：解决循环引用 */
struct TrieNode;

/* 哈希表节点：存储字符→子节点的映射 */
typedef struct {
	char character;              /* 字符（作为key） */
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
 * 创建Trie节点
 * @param ch: 字符
 * @return: 新创建的节点
 */
TrieNode *createTrieNode(char ch) {
	TrieNode *node;

	node = (TrieNode *)malloc(sizeof(TrieNode));
	node->character = ch;
	node->isWordEnd = false;
	node->isRoot = false;
	node->children = NULL;

	return node;
}

/*
 * 查找子节点
 * @param parentNode: 父节点
 * @param ch: 要查找的字符
 * @return: 子节点，不存在返回NULL
 */
TrieNode *findChildNode(TrieNode *parentNode, char ch) {
	TrieChildHash *childrenHashTable, *childHashNode;

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
	TrieChildHash *childrenHashTable, *childHashNode;

	childrenHashTable = parentNode->children;
	childHashNode = (TrieChildHash *)malloc(sizeof(TrieChildHash));
	childHashNode->character = childNode->character;
	childHashNode->childNode = childNode;
	HASH_ADD(hh, childrenHashTable, character, sizeof(char), childHashNode);
	parentNode->children = childrenHashTable;
}

/*
 * 递归释放Trie节点
 * @param nodePtr: 节点指针的地址
 */
void freeTrieNode(TrieNode **nodePtr) {
	TrieChildHash *childrenHashTable, *current, *tmp;
	TrieNode *node = *nodePtr;

	if (node == NULL)
		return;

	childrenHashTable = node->children;
	HASH_ITER(hh, childrenHashTable, current, tmp) {
		HASH_DEL(childrenHashTable, current);
		freeTrieNode(&(current->childNode));
		free(current);
	}

	free(node);
	*nodePtr = NULL;
}


/* 单词字典结构 */
typedef struct {
	TrieNode *root;  /* Trie树根节点 */
} WordDictionary;

/*
 * 创建单词字典
 * @return: 单词字典实例
 */
WordDictionary* wordDictionaryCreate() {
	WordDictionary *dict;
	TrieNode *rootNode;
	
	rootNode = createTrieNode('\0');
	rootNode->isRoot = true;

	dict = (WordDictionary *)malloc(sizeof(WordDictionary));
	dict->root = rootNode;
    
	return dict;
}

/*
 * DFS递归搜索（支持通配符）
 * @param node: 当前节点
 * @param word: 单词
 * @param wordLength: 单词长度
 * @param currentIndex: 当前处理的字符索引
 * @return: 找到返回true，否则返回false
 * 
 * 算法步骤：
 * 1. 如果到达单词末尾，检查是否是单词结尾
 * 2. 如果是'.'通配符，遍历所有子节点递归匹配
 * 3. 如果是普通字符，查找对应子节点递归匹配
 */
bool dfsSearchWithWildcard(TrieNode *node, char *word, int wordLength, int currentIndex) {
	TrieNode *currentNode, *childNode;
	TrieChildHash *childrenHashTable, *childHashNode, *tmp;
	char ch;

	/* 到达单词末尾，检查是否是单词结尾 */
	if (currentIndex == wordLength)
		return node->isWordEnd;
	
	currentNode = node;
	ch = word[currentIndex];
	childrenHashTable = currentNode->children;
	
	if (ch == '.') {
		/* 通配符：遍历所有子节点 */
		HASH_ITER(hh, childrenHashTable, childHashNode, tmp) {
			/* 任一子节点匹配成功就返回true */
			if (dfsSearchWithWildcard(childHashNode->childNode, word, wordLength, currentIndex + 1))
				return true;
		}
	} else {
		/* 普通字符：查找对应的子节点 */
		childNode = findChildNode(currentNode, ch);
		if (!childNode)
			return false;
		
		return dfsSearchWithWildcard(childNode, word, wordLength, currentIndex + 1);
	}
	
	return false;
}

/*
 * 添加单词
 * @param obj: 单词字典
 * @param word: 要添加的单词
 * 
 * 时间复杂度: O(L)，L是单词长度
 * 空间复杂度: O(L)
 */
void wordDictionaryAddWord(WordDictionary* obj, char* word) {
	TrieNode *currentNode, *childNode;
	int i, wordLength;

	wordLength = strlen(word);

	currentNode = obj->root;
	for (i = 0; i < wordLength; i++) {
		childNode = findChildNode(currentNode, word[i]);
		if (!childNode) {
			childNode = createTrieNode(word[i]);
			insertChildNode(currentNode, childNode);
		}
		currentNode = childNode;
	}
	currentNode->isWordEnd = true;
}

/*
 * 搜索单词（支持'.'通配符）
 * @param obj: 单词字典
 * @param word: 要搜索的单词（可含'.'通配符）
 * @return: 存在返回true，否则返回false
 * 
 * 时间复杂度: O(M)，M是Trie中的节点数（最坏情况）
 * 空间复杂度: O(L)，L是单词长度（递归栈）
 */
bool wordDictionarySearch(WordDictionary* obj, char* word) {
    return dfsSearchWithWildcard(obj->root, word, strlen(word), 0);
}

/*
 * 释放单词字典
 * @param obj: 单词字典
 */
void wordDictionaryFree(WordDictionary* obj) {
	freeTrieNode(&(obj->root));
	free(obj);
}

/*
 * ==================== 算法详解 ====================
 * 
 * 核心思想：Trie树 + DFS递归搜索
 * 
 * 通配符处理：
 * - 普通字符：直接查找对应子节点
 * - '.' 通配符：遍历所有子节点，递归匹配
 * 
 * 为什么用递归DFS？
 * - 通配符可能匹配多个分支
 * - 需要尝试所有可能的路径
 * - 找到一个匹配就成功
 * 
 * 示例：搜索 ".ad"
 * 
 * Trie:
 *   root
 *    ├─ b → a → d (bad)
 *    ├─ d → a → d (dad)
 *    └─ m → a → d (mad)
 * 
 * DFS搜索过程：
 * 1. ch='.'，遍历root的子节点[b, d, m]
 * 2. 尝试node_b：
 *    ch='a'，找到子节点a
 *    ch='d'，找到子节点d，is_end=true ✓
 *    返回true（找到"bad"）
 * 
 * 关键：
 * - HASH_ITER遍历所有子节点
 * - 找到一个匹配立即返回
 * - 所有子节点都失败才返回false
 */

/**
 * Your WordDictionary struct will be instantiated and called as such:
 * WordDictionary* obj = wordDictionaryCreate();
 * wordDictionaryAddWord(obj, word);
 
 * bool param_2 = wordDictionarySearch(obj, word);
 
 * wordDictionaryFree(obj);
*/