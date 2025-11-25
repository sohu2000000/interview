/*
Implement the RandomizedSet class:

RandomizedSet() Initializes the RandomizedSet object.
bool insert(int val) Inserts an item val into the set if not present. Returns true if the item was not present, false otherwise.
bool remove(int val) Removes an item val from the set if present. Returns true if the item was present, false otherwise.
int getRandom() Returns a random element from the current set of elements (it's guaranteed that at least one element exists when this method is called). Each element must have the same probability of being returned.
You must implement the functions of the class such that each function works in average O(1) time complexity.

 

Example 1:

Input
["RandomizedSet", "insert", "remove", "insert", "getRandom", "remove", "insert", "getRandom"]
[[], [1], [2], [2], [], [1], [2], []]
Output
[null, true, false, true, 2, true, false, 2]

Explanation
RandomizedSet randomizedSet = new RandomizedSet();
randomizedSet.insert(1); // Inserts 1 to the set. Returns true as 1 was inserted successfully.
randomizedSet.remove(2); // Returns false as 2 does not exist in the set.
randomizedSet.insert(2); // Inserts 2 to the set, returns true. Set now contains [1,2].
randomizedSet.getRandom(); // getRandom() should return either 1 or 2 randomly.
randomizedSet.remove(1); // Removes 1 from the set, returns true. Set now contains [2].
randomizedSet.insert(2); // 2 was already in the set, so return false.
randomizedSet.getRandom(); // Since 2 is the only number in the set, getRandom() will always return 2.
 

Constraints:

-231 <= val <= 231 - 1
At most 2 * 105 calls will be made to insert, remove, and getRandom.
There will be at least one element in the data structure when getRandom is called.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/**
 * ========== 核心思想 ==========
 * 
 * 要求所有操作都是 O(1)：
 * 1. insert/remove O(1) → 需要哈希表
 * 2. getRandom O(1) 且等概率 → 需要数组（可以用索引随机访问）
 * 
 * 解决方案：动态数组 + 哈希表
 * - 动态数组 (nums)：存储实际的值，用于 getRandom
 * - 哈希表 (map)：存储 val -> index 的映射，用于 O(1) 查找
 * 
 * 关键技巧（remove的精髓）：
 * 要删除元素时，把它和数组最后一个元素交换，然后删除末尾
 * 这样删除是 O(1) 的！
 * 
 * 例子：
 * nums = [1, 3, 5, 7]
 * map = {1:0, 3:1, 5:2, 7:3}
 * 
 * remove(3):
 * 1. 找到3的索引：1
 * 2. 把3和最后元素7交换：nums = [1, 7, 5, 3]
 * 3. 删除末尾：nums = [1, 7, 5]
 * 4. 更新map：{1:0, 7:1, 5:2}
 */

#define HASH_SIZE 10007  // 哈希表大小（质数，减少冲突）
#define INIT_CAPACITY 16  // 动态数组初始容量

// 哈希表节点（链地址法处理冲突）
typedef struct HashNode {
    int key;              // 存储的值
    int index;            // 在数组中的索引
    struct HashNode* next;
} HashNode;

// RandomizedSet 结构
 typedef struct {
    int* nums;            // 动态数组，存储实际的值
    int size;             // 当前元素个数
    int capacity;         // 数组容量
    HashNode** map;       // 哈希表，val -> index
 } RandomizedSet;
 
// 哈希函数
int hash(int key) {
    // 处理负数
    unsigned int k = (unsigned int)key;
    return k % HASH_SIZE;
}

// 创建 RandomizedSet
 RandomizedSet* randomizedSetCreate() {
    RandomizedSet* obj = (RandomizedSet*)malloc(sizeof(RandomizedSet));
    
    // 初始化动态数组
    obj->nums = (int*)malloc(INIT_CAPACITY * sizeof(int));
    obj->size = 0;
    obj->capacity = INIT_CAPACITY;
    
    // 初始化哈希表
    obj->map = (HashNode**)calloc(HASH_SIZE, sizeof(HashNode*));
    
    // 初始化随机数种子（只需要一次）
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    
    return obj;
}

// 在哈希表中查找 val，返回索引（如果不存在返回-1）
int mapGet(HashNode** map, int val) {
    int h = hash(val);
    HashNode* node = map[h];
    
    while (node) {
        if (node->key == val) {
            return node->index;
        }
        node = node->next;
    }
    
    return -1;  // 不存在
 }
 
// 在哈希表中插入或更新 val -> index
void mapPut(HashNode** map, int val, int index) {
    int h = hash(val);
    HashNode* node = map[h];
    
    // 如果已存在，更新索引
    while (node) {
        if (node->key == val) {
            node->index = index;
            return;
        }
        node = node->next;
    }
    
    // 不存在，创建新节点（头插法）
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = val;
    newNode->index = index;
    newNode->next = map[h];
    map[h] = newNode;
}

// 从哈希表中删除 val
void mapRemove(HashNode** map, int val) {
    int h = hash(val);
    HashNode* node = map[h];
    HashNode* prev = NULL;
    
    while (node) {
        if (node->key == val) {
            if (prev) {
                prev->next = node->next;
            } else {
                map[h] = node->next;
            }
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

// 插入元素
bool randomizedSetInsert(RandomizedSet* obj, int val) {
    // 如果已存在，返回 false
    if (mapGet(obj->map, val) != -1) {
        return false;
    }
    
    // 如果数组满了，扩容
    if (obj->size == obj->capacity) {
        obj->capacity *= 2;
        obj->nums = (int*)realloc(obj->nums, obj->capacity * sizeof(int));
    }
    
    // 添加到数组末尾
    obj->nums[obj->size] = val;
    
    // 在哈希表中记录 val -> index
    mapPut(obj->map, val, obj->size);
    
    obj->size++;
    return true;
}

/**
 * 删除元素（关键技巧！）
 * 
 * 要删除数组中间的元素是 O(n)，怎么做到 O(1)？
 * 
 * 技巧：把要删除的元素和数组最后一个元素交换，然后删除末尾
 * 
 * 例子：删除 3
 * nums = [1, 3, 5, 7]  删除索引1的元素
 *         ↓     ↓
 * step1: [1, 7, 5, 3]  交换索引1和索引3
 * step2: [1, 7, 5]     删除末尾
 * 
 * 时间复杂度：O(1)
 */
bool randomizedSetRemove(RandomizedSet* obj, int val) {
    // 查找元素索引
    int index = mapGet(obj->map, val);
    if (index == -1) {
        return false;  // 不存在
    }
    
    // 获取最后一个元素
    int lastElement = obj->nums[obj->size - 1];
    
    // 把最后一个元素移到要删除的位置
    obj->nums[index] = lastElement;
    
    // 更新最后一个元素在哈希表中的索引
    mapPut(obj->map, lastElement, index);
    
    // 从哈希表中删除 val
    mapRemove(obj->map, val);
    
    // 减小 size（相当于删除了末尾）
    obj->size--;
    
    return true;
}

// 随机获取一个元素
int randomizedSetGetRandom(RandomizedSet* obj) {
    // 随机选择一个索引 [0, size-1]
    int randomIndex = rand() % obj->size;
    return obj->nums[randomIndex];
}

// 释放内存
void randomizedSetFree(RandomizedSet* obj) {
    // 释放动态数组
    free(obj->nums);
    
    // 释放哈希表
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = obj->map[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(obj->map);
    
    // 释放对象本身
    free(obj);
}

/**
 * Your RandomizedSet struct will be instantiated and called as such:
 * RandomizedSet* obj = randomizedSetCreate();
 * bool param_1 = randomizedSetInsert(obj, val);
 * bool param_2 = randomizedSetRemove(obj, val);
 * int param_3 = randomizedSetGetRandom(obj);
 * randomizedSetFree(obj);
*/
