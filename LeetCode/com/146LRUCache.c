/**
 * LRU Cache - 双向链表 + 哈希表
 * 
 * 核心思想：
 * - 哈希表：O(1)查找
 * - 双向链表：维护访问顺序（head=最久，tail=最新）
 * - get/put时移动到tail（最近使用）
 * - 容量满时删除head（最久未使用）
 * 
 * 时间复杂度：O(1) 所有操作
 * 空间复杂度：O(capacity)
 */

// 双向链表节点（同时作为哈希表节点）
typedef struct CacheNode {
	int key;
	int value;
	struct CacheNode *next;
	struct CacheNode *prev;
	UT_hash_handle hh;  // uthash句柄
} CacheNode;

typedef struct {
    CacheNode *head;  // 最久未使用（LRU）
    CacheNode *tail;  // 最近使用（MRU）
    int capacity;     // 容量
    int size;         // 当前大小
} LRUCache;

CacheNode *hashTable = NULL;  // 全局哈希表

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->head = NULL;
    cache->tail = NULL;
    cache->capacity = capacity;
    cache->size = 0;
    return cache;
}

// 辅助函数：将节点移动到尾部（标记为最近使用）
void moveToTail(LRUCache* cache, CacheNode *node) {
	if (node == cache->tail) return;  // 已经是尾部
	
	// 从原位置移除
	if (node->prev != NULL) {
		node->prev->next = node->next;
	} else {
		// node是head
		cache->head = node->next;
	}
	
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}

	// 添加到尾部
	cache->tail->next = node;
	node->prev = cache->tail;
	node->next = NULL;
	cache->tail = node;
}

int lRUCacheGet(LRUCache* obj, int key) {
    CacheNode *node = NULL;

    HASH_FIND_INT(hashTable, &key, node);
    if (node != NULL) {
        moveToTail(obj, node);  // 移动到尾部
        return node->value;
    }

    return -1;  // 未找到
}

// 辅助函数：添加节点到尾部
void addToTail(LRUCache* cache, CacheNode *node) {
	if (cache->tail == NULL) {
		// 空链表，第一个节点
		cache->head = node;
		cache->tail = node;
	} else {
		// 添加到尾部
		cache->tail->next = node;
		node->prev = cache->tail;
		cache->tail = node;
	}
	node->next = NULL;
}

// 辅助函数：删除头节点（最久未使用）
void removeHead(LRUCache* cache) {
	if (cache->head == NULL) return;
	
	CacheNode *oldHead = cache->head;
	cache->head = oldHead->next;
	
	if (cache->head != NULL) {
		cache->head->prev = NULL;
	} else {
		// 删除后链表空了
		cache->tail = NULL;
	}
	
	HASH_DEL(hashTable, oldHead);
	free(oldHead);
}

void lRUCachePut(LRUCache* obj, int key, int value) {
	CacheNode *node = NULL;

	HASH_FIND_INT(hashTable, &key, node);
	if (node != NULL) {
		// 键已存在，更新值并移动到尾部
		node->value = value;
		moveToTail(obj, node);
	} else {
		// 新键，创建新节点
		CacheNode *newNode = (CacheNode*)malloc(sizeof(CacheNode));
		newNode->key = key;
		newNode->value = value;
		newNode->prev = NULL;
		newNode->next = NULL;
		
		HASH_ADD_INT(hashTable, key, newNode);
		
		if (obj->size < obj->capacity) {
			// 容量未满，直接添加
			obj->size++;
			addToTail(obj, newNode);
		} else {
			// 容量满，删除head再添加
			removeHead(obj);
			addToTail(obj, newNode);
		}
	}
}

void lRUCacheFree(LRUCache* obj) {
    CacheNode *current = obj->head;
    while (current != NULL) {
        CacheNode *next = current->next;
        HASH_DEL(hashTable, current);
        free(current);
        current = next;
    }
    free(obj);
}

/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);
 
 * lRUCachePut(obj, key, value);
 
 * lRUCacheFree(obj);
*/