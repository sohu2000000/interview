# LeetCode 解题经验与技巧总结

本文档总结了59道LeetCode题目的核心算法思想、常见技巧和易错点。

---

## 📚 目录

1. [C语言工具库API](#1-c语言工具库api)
2. [双指针技巧](#2-双指针技巧)
3. [滑动窗口](#3-滑动窗口)
4. [贪心算法](#4-贪心算法)
5. [哈希表应用](#5-哈希表应用)
6. [矩阵操作](#6-矩阵操作)
7. [数据结构设计](#7-数据结构设计)
8. [数组技巧](#8-数组技巧)
9. [链表操作](#9-链表操作)
10. [二叉树](#10-二叉树)
11. [图论/DFS/BFS](#11-图论dfsbfs)
12. [数据结构设计](#12-数据结构设计)
13. [C语言常见陷阱](#13-c语言常见陷阱)

---

## 1. C语言工具库API

### 8.1 qsort - 快速排序

**函数原型**：
```c
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
```

**参数说明**：
- `base`：数组首地址
- `nmemb`：元素个数
- `size`：**每个元素的字节数**（关键！）
- `compar`：比较函数

**比较函数返回值**：
```
< 0（负数）：a排在b前面
= 0：        a和b相等，顺序不变
> 0（正数）：a排在b后面
```

#### 示例1：整数数组升序

```c
int arr[] = {5, 2, 8, 1, 9};

int compareAsc(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    
    // 升序：a小则返回负数，a排前面
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

qsort(arr, 5, sizeof(int), compareAsc);
// 结果：[1, 2, 5, 8, 9]
```

#### 示例2：整数数组降序

```c
int compareDesc(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    
    // 降序：a大则返回负数，a排前面
    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

qsort(arr, 5, sizeof(int), compareDesc);
// 结果：[9, 8, 5, 2, 1]
```

#### 示例3：二维数组排序

```c
int** intervals = [[7,10], [1,5], [3,6]];

int compareByStart(const void *a, const void *b) {
    // a和b是指向int*的指针
    int *interval1 = *(int**)a;  // 解引用得到int*
    int *interval2 = *(int**)b;
    
    // 按第一个元素（起始位置）升序
    if (interval1[0] < interval2[0]) return -1;
    if (interval1[0] > interval2[0]) return 1;
    return 0;
}

// 关键：元素是指针，大小是sizeof(int*)
qsort(intervals, 3, sizeof(int*), compareByStart);
// 结果：[[1,5], [3,6], [7,10]]
```

#### 示例4：结构体排序

```c
typedef struct {
    char name[50];
    int age;
} Person;

Person people[] = {
    {"Alice", 25},
    {"Bob", 30},
    {"Charlie", 20}
};

int compareByAge(const void *a, const void *b) {
    Person *p1 = (Person*)a;
    Person *p2 = (Person*)b;
    
    if (p1->age < p2->age) return -1;
    if (p1->age > p2->age) return 1;
    return 0;
}

qsort(people, 3, sizeof(Person), compareByAge);
// 结果：Charlie(20), Alice(25), Bob(30)
```

#### 升降序记忆口诀

| 写法 | 结果 | 记忆 |
|-----|------|------|
| `return a - b` | 升序 ↑ | "小的在前"（自然顺序）|
| `return b - a` | 降序 ↓ | "大的在前"（反过来）|
| `if (a < b) return -1` | 升序 ↑ | a小排前 |
| `if (a > b) return -1` | 降序 ↓ | a大排前 |

**⚠️ 重要提醒**：大数值范围必须用if判断，不能用减法（会溢出）！

---

### 8.2 uthash - 哈希表库

**uthash简介**：
- C语言的宏哈希表库
- 头文件only，无需链接
- 支持int、string、自定义键

#### 基本结构

```c
#include "uthash.h"

// 定义哈希节点
typedef struct HashNode {
    int key;               // 键
    int value;             // 值
    UT_hash_handle hh;     // 必须包含！uthash句柄
} HashNode;

HashNode *hashTable = NULL;  // 哈希表头（初始NULL）
```

#### API 1：HASH_ADD_INT（整数键）

```c
// 语法：HASH_ADD_INT(head, keyfield_name, item_ptr)
HashNode *node = (HashNode*)malloc(sizeof(HashNode));
node->key = 42;
node->value = 100;
HASH_ADD_INT(hashTable, key, node);  
//            ↑        ↑    ↑
//          表头   字段名  节点指针
```

**注意**：第2参数是**字段名**（key），不是值！

#### API 2：HASH_FIND_INT（查找）

```c
// 语法：HASH_FIND_INT(head, key_ptr, output_ptr)
int searchKey = 42;
HashNode *found;
HASH_FIND_INT(hashTable, &searchKey, found);
//            ↑          ↑          ↑
//          表头       键的地址    输出指针

if (found) {
    printf("Value: %d\n", found->value);
} else {
    printf("Not found\n");
}
```

**注意**：第2参数是**地址**（&key），不是值！

#### API 3：HASH_ADD_STR（字符串键）

```c
typedef struct {
    char name[50];         // 字符串键
    int age;
    UT_hash_handle hh;
} Person;

Person *people = NULL;
Person *p = (Person*)malloc(sizeof(Person));
strcpy(p->name, "Alice");
p->age = 25;
HASH_ADD_STR(people, name, p);  // name是字段名
```

#### API 4：HASH_FIND_STR（查找字符串）

```c
Person *found;
HASH_FIND_STR(people, "Alice", found);  // 直接传字符串
if (found) {
    printf("Age: %d\n", found->age);
}
```

#### API 5：HASH_DEL（删除）

```c
HashNode *found;
int key = 42;
HASH_FIND_INT(hashTable, &key, found);
if (found) {
    HASH_DEL(hashTable, found);  // 从表中删除
    free(found);                  // 释放内存
}
```

#### API 6：HASH_COUNT（计数）

```c
unsigned int count = HASH_COUNT(hashTable);
printf("Total: %u items\n", count);
```

#### API 7：HASH_ITER（遍历）

```c
HashNode *current, *tmp;
HASH_ITER(hh, hashTable, current, tmp) {
    printf("key=%d, value=%d\n", current->key, current->value);
    // 可以在遍历中安全删除current
}

// 或者简单遍历（不能删除）
for (current = hashTable; current != NULL; current = current->hh.next) {
    printf("key=%d\n", current->key);
}
```

#### 完整示例：集合操作

```c
#include "uthash.h"

typedef struct {
    int num;
    UT_hash_handle hh;
} IntSet;

IntSet *set = NULL;

// 添加
void add(int num) {
    IntSet *node;
    HASH_FIND_INT(set, &num, node);
    if (node == NULL) {
        node = (IntSet*)malloc(sizeof(IntSet));
        node->num = num;
        HASH_ADD_INT(set, num, node);
    }
}

// 查找
bool contains(int num) {
    IntSet *found;
    HASH_FIND_INT(set, &num, found);
    return found != NULL;
}

// 删除
void removeNum(int num) {
    IntSet *found;
    HASH_FIND_INT(set, &num, found);
    if (found) {
        HASH_DEL(set, found);
        free(found);
    }
}

// 使用
add(1);
add(2);
add(3);
if (contains(2)) printf("2 exists\n");
removeNum(2);
printf("Count: %u\n", HASH_COUNT(set));
```

#### uthash 常见错误对照表

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| `HASH_FIND_INT(h, key, f)` | `HASH_FIND_INT(h, &key, f)` | 需要取地址 |
| `HASH_ADD_INT(h, 42, n)` | `HASH_ADD_INT(h, key, n)` | 第2参数是字段名 |
| `HASH_ADD_STR(h, str, n)` | `HASH_ADD_STR(h, key, n)` | 第2参数是字段名 |
| 缺少`UT_hash_handle hh` | 必须包含 | uthash必需字段 |
| 头指针未初始化 | `HashNode *h = NULL;` | 必须初始化为NULL |

#### uthash vs 手动哈希表

| 特性 | 手动实现 | uthash |
|-----|---------|--------|
| 代码量 | 多（100+行）| 少（几行）|
| 冲突处理 | 需要实现 | 自动处理 |
| 内存管理 | 需要自己管理 | 简化但仍需free |
| 性能 | 可控 | 优秀 |
| 适用场景 | LeetCode面试 | 两者都可以 |

---

## 17. 双指针技巧

### 8.1 对撞指针（Two Sum II - 167）

**适用场景**：有序数组，找两个数的和

**核心思路**：
- left从头开始，right从尾开始
- sum太大→right--，sum太小→left++
- O(n)时间复杂度

```c
while (left < right) {
    sum = nums[left] + nums[right];
    if (sum == target) return;
    else if (sum > target) right--;
    else left++;
}
```

**关键点**：
- ✅ 利用有序性质
- ✅ 两个指针从两端向中间移动
- ⚠️ 注意 `left < right`，不能相等

### 8.2 三指针（Three Sum - 15）

**核心思路**：固定一个数 + 双指针

```c
for (i = 0; i < n - 2; i++) {
    // 固定nums[i]，在[i+1, n-1]中用双指针找另外两个数
    left = i + 1;
    right = n - 1;
    while (left < right) {
        // 双指针逻辑
    }
}
```

**去重技巧**：
```c
// 跳过重复的第一个数
if (i > 0 && nums[i] == nums[i-1]) continue;

// 跳过重复的left和right
while (left < right && nums[left] == nums[left+1]) left++;
while (left < right && nums[right] == nums[right-1]) right--;
```

### 8.3 贪心双指针（Container With Most Water - 11）

**核心思路**：移动较短的边

```c
while (left < right) {
    area = (right - left) * min(height[left], height[right]);
    maxArea = max(maxArea, area);
    
    if (height[left] < height[right])
        left++;   // 移动较短的边
    else
        right--;
}
```

**为什么？**
- 移动较短的边：可能找到更高的边
- 移动较长的边：宽度减小，高度不增，面积必然减小

---

## 17. 滑动窗口

### 8.1 固定窗口 vs 可变窗口

| 类型 | 特点 | 例题 |
|-----|------|------|
| 可变窗口 | 窗口大小动态调整 | 最长无重复子串(3), 最小子数组和(209) |
| 固定窗口 | 窗口大小固定 | - |

### 8.2 最长无重复子串（3）

**核心模板**：
```c
int charLastPos[256];  // 哈希表
memset(charLastPos, -1, sizeof(charLastPos));

int left = 0, maxLen = 0;
for (int right = 0; s[right] != '\0'; right++) {
    // 如果字符重复，且在窗口内
    if (charLastPos[s[right]] >= left) {
        left = charLastPos[s[right]] + 1;  // 移动到重复位置+1
    }
    
    maxLen = max(maxLen, right - left + 1);
    charLastPos[s[right]] = right;
}
```

**关键点**：
- ✅ 用哈希表记录字符最后位置
- ✅ `>= left` 检查防止left后退
- ⚠️ 必须检查是否在窗口内

### 8.3 最小子数组和（209）

**可变窗口模板**：
```c
int left = 0, windowSum = 0;
for (int right = 0; right < n; right++) {
    windowSum += nums[right];  // 扩展窗口
    
    while (windowSum >= target) {  // 满足条件
        minLen = min(minLen, right - left + 1);
        windowSum -= nums[left];  // 收缩窗口
        left++;
    }
}
```

**关键点**：
- ✅ 外层循环扩展right
- ✅ 内层while收缩left
- ✅ 每个元素最多访问2次，O(n)

---

## 17. 贪心算法

### 8.1 Jump Game（55, 45）

**核心思想**：维护最远可达位置

```c
int farthest = 0;
for (int i = 0; i < n; i++) {
    if (i > farthest) return false;  // 到不了
    farthest = max(farthest, i + nums[i]);
}
return farthest >= n - 1;
```

**Jump Game II（最少跳跃次数）**：
- **BFS思想**：把数组看成多层
- `currentEnd`：当前层的边界
- `farthest`：下一层的边界
- 到达层边界时，steps++

**关键定理**：
> 不需要模拟跳跃过程，只需要知道"能到达多远"

### 8.2 Gas Station（134）

**关键定理**：
> 如果从A到不了B，那么[A, B)之间的任何点也到不了B

**证明**：
```
sum(C, B) = sum(A, B) - sum(A, C)
          < 0 - 0  (因为sum(A,B)<0, sum(A,C)>=0)
          < 0
```

**代码实现**：
```c
if (currentTank < 0) {
    start = i + 1;  // 直接跳过[start, i]所有点
    currentTank = 0;
}
```

**注意**：
- ⚠️ `total_tank` 检查要在循环**外面**
- ⚠️ 不能在循环中提前返回-1

### 8.3 Merge Intervals（56）

**核心思路**：排序 + 贪心合并

```c
// 1. 排序
qsort(intervals, n, sizeof(int*), compareIntervals);

// 2. 合并
if (currentStart <= lastEnd) {
    // 重叠：扩展end
    lastEnd = max(lastEnd, currentEnd);
} else {
    // 不重叠：添加新区间
}
```

**易错点**：
- ❌ `qsort` 元素大小写成 `2*sizeof(int)` → 应该是 `sizeof(int*)`
- ❌ cmp函数类型转换错误 → 需要 `*(int**)a`

### 8.4 Insert Interval（57）

**核心思路**：一次遍历，三种情况

```c
for (i = 0; i < n; i++) {
    if (intervals[i][1] < newStart) {
        // 情况1：当前区间在newInterval之前
        添加当前区间
    } else if (intervals[i][0] > newEnd) {
        // 情况2：当前区间在newInterval之后
        先插入newInterval（如果还没插入）
        添加当前区间
    } else {
        // 情况3：重叠，合并
        newStart = min(newStart, intervals[i][0]);
        newEnd = max(newEnd, intervals[i][1]);
    }
}

// 最后检查newInterval是否已插入
if (!inserted) 添加newInterval;
```

**关键点**：
- ✅ 不需要排序（已排序）
- ✅ 用flag标记是否已插入
- ⚠️ 运算符优先级：`(n+1)*sizeof(int*)` 要加括号

### 8.5 Minimum Arrows to Burst Balloons（452）

**核心思路**：按结束位置排序 + 贪心射箭

```c
// 1. 按结束位置排序
qsort(points, n, sizeof(int*), cmp);

// 2. 贪心：在当前箭覆盖的最右端射箭
arrowPosition = points[0][1];  // 第一支箭
arrowCount = 1;

for (i = 1; i < n; i++) {
    if (points[i][0] <= arrowPosition) {
        continue;  // 当前箭能射穿这个气球
    }
    // 需要新箭：射在新气球的结束位置
    arrowPosition = points[i][1];
    arrowCount++;
}
```

**为什么按end排序？**
- 在最早结束的位置射箭
- 能覆盖尽可能多的后续气球
- 贪心策略：尽早射箭，为后面留更多空间

**易错点**：
- ❌ cmp函数用减法 → 大数值会整数溢出
- ✅ 用if判断比较：`if (a[1] < b[1]) return -1;`

---

## 17. 哈希表应用

### 8.1 Group Anagrams（49）

**核心思想**：排序后的字符串作为键

```c
// 生成键
char key[101];
strcpy(key, str);
qsort(key, strlen(key), sizeof(char), cmp);

// 用uthash分组
HASH_FIND_STR(groups, key, group);
```

**技巧**：
- ✅ 异位词排序后相同
- ✅ 用uthash库（方便的C哈希表）

### 8.2 Longest Consecutive Sequence（128）

**核心优化**：只从序列起点计数

```c
// 检查是否起点
int prev = num - 1;
HASH_FIND_INT(hashSet, &prev, found);
if (found != NULL) continue;  // 不是起点，跳过

// 从起点向后查找
while (found != NULL) {
    length++;
    // 查找下一个数
}
```

**关键点**：
- ✅ 避免重复计算同一序列
- ✅ 每个数字最多访问2次，O(n)
- ⚠️ uthash: `HASH_FIND_INT` 第2参数需要取地址

### 8.3 uthash API 完整说明

**uthash简介**：
- C语言的哈希表宏库
- 使用简单，性能优秀
- 支持多种键类型（int, string, 自定义）

#### 基本结构定义

```c
#include "uthash.h"

// 哈希节点结构
typedef struct HashNode {
    int key;           // 键（可以是其他类型）
    int value;         // 值（可以是任意数据）
    UT_hash_handle hh; // 必须：uthash句柄
} HashNode;

HashNode *hashTable = NULL;  // 哈希表（初始为NULL）
```

#### 常用API

**1. HASH_ADD_INT - 添加（整数键）**

```c
// 语法：HASH_ADD_INT(head, keyfield, item_ptr)
HashNode *node = (HashNode*)malloc(sizeof(HashNode));
node->key = 100;
node->value = 200;
HASH_ADD_INT(hashTable, key, node);  // 第2参数是字段名
```

**2. HASH_FIND_INT - 查找（整数键）**

```c
// 语法：HASH_FIND_INT(head, key_ptr, out_ptr)
int searchKey = 100;
HashNode *found;
HASH_FIND_INT(hashTable, &searchKey, found);  // 第2参数是地址
if (found) {
    printf("Found: %d\n", found->value);
}
```

**3. HASH_DEL - 删除**

```c
HashNode *found;
int key = 100;
HASH_FIND_INT(hashTable, &key, found);
if (found) {
    HASH_DEL(hashTable, found);  // 从表中删除
    free(found);                  // 释放内存
}
```

**4. HASH_COUNT - 获取数量**

```c
unsigned int count = HASH_COUNT(hashTable);
```

**5. HASH_ITER - 遍历**

```c
HashNode *current, *tmp;
HASH_ITER(hh, hashTable, current, tmp) {
    printf("key=%d, value=%d\n", current->key, current->value);
}
```

**6. HASH_ADD_STR - 添加（字符串键）**

```c
typedef struct {
    char key[50];      // 字符串键
    int value;
    UT_hash_handle hh;
} StrNode;

StrNode *node = (StrNode*)malloc(sizeof(StrNode));
strcpy(node->key, "hello");
node->value = 123;
HASH_ADD_STR(strTable, key, node);  // 第2参数是字段名
```

**7. HASH_FIND_STR - 查找（字符串键）**

```c
StrNode *found;
HASH_FIND_STR(strTable, "hello", found);  // 第2参数是字符串
if (found) {
    printf("Found: %d\n", found->value);
}
```

#### 完整示例：整数集合

```c
#include "uthash.h"

typedef struct {
    int value;
    UT_hash_handle hh;
} IntSet;

// 添加元素
void addToSet(IntSet **set, int val) {
    IntSet *node;
    HASH_FIND_INT(*set, &val, node);
    if (node == NULL) {  // 不存在才添加
        node = (IntSet*)malloc(sizeof(IntSet));
        node->value = val;
        HASH_ADD_INT(*set, value, node);
    }
}

// 查找元素
bool contains(IntSet *set, int val) {
    IntSet *found;
    HASH_FIND_INT(set, &val, found);
    return found != NULL;
}

// 使用
IntSet *mySet = NULL;
addToSet(&mySet, 1);
addToSet(&mySet, 2);
addToSet(&mySet, 3);

if (contains(mySet, 2)) {
    printf("2 is in set\n");
}
```

#### 完整示例：字符串分组（Group Anagrams）

```c
typedef struct AnagramGroup {
    char sortedKey[101];     // 排序后的字符串作为键
    char **strings;          // 该组的字符串数组
    int count;
    UT_hash_handle hh;
} AnagramGroup;

// 添加字符串到组
void addString(AnagramGroup **groups, char *sortedKey, char *str) {
    AnagramGroup *group;
    HASH_FIND_STR(*groups, sortedKey, group);
    
    if (group == NULL) {
        // 创建新组
        group = (AnagramGroup*)malloc(sizeof(AnagramGroup));
        strcpy(group->sortedKey, sortedKey);
        group->count = 0;
        group->strings = (char**)malloc(10 * sizeof(char*));
        HASH_ADD_STR(*groups, sortedKey, group);
    }
    
    group->strings[group->count++] = str;
}
```

#### 常见错误

| 错误 | 正确 | 说明 |
|-----|------|------|
| `HASH_FIND_INT(h, key, f)` | `HASH_FIND_INT(h, &key, f)` | 第2参数需要地址 |
| `HASH_ADD_INT(h, value, n)` | `HASH_ADD_INT(h, key, n)` | 第2参数是字段名 |
| `HASH_ADD_STR(h, "key", n)` | `HASH_ADD_STR(h, key, n)` | 第2参数是字段名 |
| 忘记 `UT_hash_handle hh` | 必须包含此字段 | uthash必需 |

#### API速查表

| 操作 | 整数键 | 字符串键 | 自定义键 |
|-----|-------|---------|---------|
| 添加 | `HASH_ADD_INT` | `HASH_ADD_STR` | `HASH_ADD` |
| 查找 | `HASH_FIND_INT` | `HASH_FIND_STR` | `HASH_FIND` |
| 删除 | `HASH_DEL` | `HASH_DEL` | `HASH_DEL` |
| 计数 | `HASH_COUNT` | `HASH_COUNT` | `HASH_COUNT` |
| 遍历 | `HASH_ITER` | `HASH_ITER` | `HASH_ITER` |

#### qsort 的元素大小

## 17. 矩阵操作

### 8.1 Valid Sudoku（36）

**技巧**：三个哈希表分别标记行/列/方块

```c
bool rowUsed[9][10];     // [9][10]，支持索引1-9
bool colUsed[9][10];
bool boxUsed[9][10];

// 方块索引计算
boxIndex = (row / 3) * 3 + col / 3;
```

**易错点**：
- ❌ 数组大小 `[9][9]` → 应该 `[9][10]`（数字1-9需要10个位置）
- ❌ 全局变量未重置 → LeetCode多次调用会有问题

### 8.2 Rotate Image（48）

**核心思想**：转置 + 反转每行

```
位置变换：(row, col) → (col, n-1-row)

分解：
步骤1：转置 (row, col) → (col, row)
步骤2：反转 (col, row) → (col, n-1-row)
```

**为什么分解？**
- 直接变换需要额外空间
- 分解后每步都可以原地操作

### 8.3 Spiral Matrix（54）

**核心技巧**：四边界螺旋遍历

```c
while (top <= bottom && left <= right) {
    // 右 → 上边界
    // 下 → 右边界
    
    if (top <= bottom) {  // 防止单行重复
        // 左 → 下边界
    }
    
    if (left <= right) {  // 防止单列重复
        // 上 → 左边界
    }
}
```

**关键点**：
- ✅ 方向3和4需要额外检查
- ✅ 避免单行/单列情况下重复遍历

### 8.4 Set Matrix Zeroes（73）

**O(1)空间技巧**：用首行首列作标记

```c
// 1. 记录首行首列是否有0
firstRowHasZero, firstColHasZero

// 2. 用首行首列标记其他行列
if (matrix[i][j] == 0) {
    matrix[i][0] = 0;  // 标记该行
    matrix[0][j] = 0;  // 标记该列
}

// 3. 根据标记清零
// 4. 最后处理首行首列本身
```

**易错点**：
- ❌ 变量名弄反（`row0`检查的却是列）

### 8.5 Game of Life（289）

**状态编码技巧**：

```c
DEAD_TO_DEAD = 2
DEAD_TO_ALIVE = 3
ALIVE_TO_DEAD = 4
ALIVE_TO_ALIVE = 5
```

**关键点**：
- ✅ 编码值避开0和1
- ✅ 第一遍：编码下一状态
- ✅ 第二遍：解码为0或1
- ⚠️ 检查邻居时要同时检查**初始值1**和编码值

---

## 17. 贪心算法核心

### 8.1 贪心的本质

**三个要素**：
1. **贪心选择**：每步做局部最优选择
2. **无后悔**：不回溯
3. **期望**：局部最优导致全局最优

### 8.2 贪心正确性的两个条件

1. **最优子结构**：问题可以分解为子问题
2. **贪心选择性质**：局部最优能导致全局最优

### 8.3 经典例子

**Jump Game**：
- 不需要知道"怎么跳"
- 只需要知道"能到哪"
- 维护最远可达位置

**Gas Station**：
- 不需要逐个尝试起点
- 发现失败立即跳过一段
- 基于数学定理保证正确性

---

## 11. 链表操作

### 11.1 Add Two Numbers（2）

**核心思想**：模拟小学加法，逐位相加

```c
int carry = 0;  // 进位
while (p1 != NULL || p2 != NULL) {
    digitSum = carry;
    if (p1) digitSum += p1->val;
    if (p2) digitSum += p2->val;
    
    digit = digitSum % 10;   // 当前位
    carry = digitSum / 10;    // 进位
    
    // 创建新节点
    newNode = createNode(digit);
    // 添加到结果链表
}

// 最后处理进位
if (carry > 0) {
    newNode = createNode(carry);
}
```

**关键点**：
- ✅ 从低位到高位（链表头到尾）
- ✅ 两个链表可能长度不同
- ✅ 记得处理最后的进位
- ⚠️ `carry = sum / 10`，不是 `sum % 10`

**易错点**：
- ❌ 忘记最后的进位（如 5+5=10）
- ❌ 没有处理不同长度的链表

### 11.2 Remove Nth Node From End（19）

**核心思想**：快慢指针，保持固定间隔

```c
// 哑节点简化边界处理
dummy->next = head;
slow = fast = dummy;

// 快指针先走n+1步
for (i = 0; i < n+1; i++) {
    fast = fast->next;
}

// 快慢指针一起走
while (fast != NULL) {
    slow = slow->next;
    fast = fast->next;
}

// slow指向要删除节点的前一个
slow->next = slow->next->next;
```

**为什么是n+1步？**
```
删除倒数第n个 = 找到倒数第n+1个（前一个）

从某节点到NULL的距离 = 该节点到末尾的步数
如果slow到NULL需要n+1步，则slow在倒数第n+1个位置

所以：fast比slow快n+1步
```

**关键点**：
- ✅ 哑节点处理删除头节点的情况
- ✅ 快指针先走n+1步（不是n步）
- ✅ 间隔保证slow停在要删除节点的前一个
- ⚠️ 释放dummy前先保存结果

**易错点**：
- ❌ fast走n步 → slow停在要删除的节点（不是前一个）
- ❌ `free(dummy)` 后访问 `dummy->next` → use-after-free

### 11.3 Remove Duplicates from Sorted List II（82）

**核心思想**：删除所有重复节点（一个不留）

```c
dummy->next = head;
prev = dummy;  // 最后一个确定保留的节点
current = head;

while (current != NULL) {
    currentValue = current->val;
    nextNode = current->next;
    
    // 跳过所有值为currentValue的节点
    while (nextNode != NULL && nextNode->val == currentValue) {
        nextNode = nextNode->next;
    }
    
    // 判断是否有重复
    if (current->next != nextNode) {
        // 有重复：整组删除
        prev->next = nextNode;
    } else {
        // 无重复：保留，prev前进
        prev = current;
    }
    
    current = nextNode;
}
```

**关键判断**：`current->next != nextNode`
- 如果相等：current后面没有重复，保留
- 如果不等：current到nextNode之间有重复，全部删除

**对比19题**：

| 题目 | 目标 | 技巧 |
|-----|------|------|
| 19题 | 删除特定位置 | 快慢指针（固定间隔）|
| 82题 | 删除所有重复 | 判断重复组 + 跳过 |

**易错点**：
- ❌ 只删除重复的副本，保留一个 → 应该全部删除
- ❌ 没有使用哑节点 → 删除头节点时需要特殊处理

### 11.4 Rotate List（61）

**核心思想**：快慢指针找到旋转点

```c
// 1. 处理k（可能>length）
length = getListLength(head);
k = k % length;

// 2. 快指针先走k步
fast = head;
for (i = 0; i < k; i++) {
    fast = fast->next;
}

// 3. 快慢指针一起走到fast到最后
while (fast->next != NULL) {
    slow = slow->next;
    fast = fast->next;
}

// 4. 重新连接
newHead = slow->next;      // 新头
oldTail->next = head;      // 旧尾连旧头（成环）
slow->next = NULL;         // 新尾断开（断环）
```

**图解**：`[1,2,3,4,5], k=2`
```
原链表：1 -> 2 -> 3 -> 4 -> 5
旋转2：4 -> 5 -> 1 -> 2 -> 3

操作：
1. 找到新头：4（倒数第k个）
2. 找到新尾：3（倒数第k+1个）
3. 旧尾5连旧头1
4. 新尾3断开
```

**关键点**：
- ✅ k取模避免重复旋转
- ✅ 快指针先走k步（不是k+1）
- ✅ while条件是 `fast->next != NULL`（到最后一个节点）
- ⚠️ 注意 `k=0` 和空链表的边界情况

**易错点**：
- ❌ 忘记 `k % length`
- ❌ fast走到NULL而不是最后一个节点

### 11.5 Partition List（86）

**核心思想**：双链表分离 + 合并

```c
// 两个哑节点管理子链表
lessDummy, greaterDummy

// 两个尾指针
lessTail = lessDummy;
greaterTail = greaterDummy;

// 遍历原链表，分配节点
while (current != NULL) {
    if (current->val < x) {
        lessTail->next = current;
        lessTail = lessTail->next;
    } else {
        greaterTail->next = current;
        greaterTail = greaterTail->next;
    }
    current = current->next;
}

// 关键：断开greater尾部
greaterTail->next = NULL;

// 连接两个链表
lessTail->next = greaterDummy->next;
return lessDummy->next;
```

**为什么要断开尾部？**
```
原链表：1 -> 4 -> 3 -> 2
分配后：
less: 1 -> 2
greater: 4 -> 3
但3->next还指向2！

如果不断开：
1 -> 2 -> 4 -> 3 -> 2 -> ... (成环！超时)
        ↑______________|
```

**关键点**：
- ✅ 两个哑节点简化操作
- ✅ 原地操作（不创建新节点）
- ✅ 保持相对顺序
- ⚠️ **必须断开greater尾部**

**易错点**：
- ❌ 忘记 `greaterTail->next = NULL` → 成环导致超时

---

## 10. 二叉树

### 10.1 验证二叉搜索树（98）

**核心思想**：范围约束递归传递

**问题描述**：
给定一个二叉树的根节点，判断其是否是一个有效的二叉搜索树（BST）。

**BST定义**：
- 节点的左子树只包含**小于**当前节点的数
- 节点的右子树只包含**大于**当前节点的数
- 所有左子树和右子树自身也必须是BST

**关键理解**：
```
❌ 错误理解：只检查直接子节点
   左子节点 < 根 < 右子节点

✅ 正确理解：检查整个子树
   左子树所有节点 < 根 < 右子树所有节点
```

**示例**：

```
合法BST:         非法BST:
    2               5
   / \             / \
  1   3           1   4  ❌ 4 < 5，违反BST
                     / \
                    3   6  ❌ 6在根节点5的左子树中
```

**算法实现**：

```c
bool validateBSTRange(struct TreeNode* root, long minValue, long maxValue) {
    if (!root) return true;
    
    // 步骤1: 检查当前节点是否在合法范围内
    if (root->val <= minValue || root->val >= maxValue)
        return false;
    
    // 步骤2: 检查左子树（范围：(minValue, root->val)）
    if (!validateBSTRange(root->left, minValue, root->val))
        return false;
    
    // 步骤3: 检查右子树（范围：(root->val, maxValue)）
    if (!validateBSTRange(root->right, root->val, maxValue))
        return false;
    
    return true;
}

bool isValidBST(struct TreeNode* root) {
    // 根节点范围：(-∞, +∞)
    return validateBSTRange(root, LONG_MIN, LONG_MAX);
}
```

**范围传递图解**：

```
                 10 (-∞, +∞)
                /          \
               /            \
         5 (-∞, 10)      15 (10, +∞)
        /      \          /        \
       /        \        /          \
  3 (-∞,5)  7 (5,10) 12(10,15)  20(15,+∞)

规则：
- 左子树：min继承，max=父节点值
- 右子树：min=父节点值，max继承

为什么这样？
- 左子树所有节点要 < 根节点
- 右子树所有节点要 > 根节点
- 约束是传递的！
```

**详细走查**：输入 `[5,1,4,null,null,3,6]`

```
    5
   / \
  1   4
     / \
    3   6

=== 递归过程 ===

validateBSTRange(5, LONG_MIN, LONG_MAX)
  范围：(-∞, +∞)
  检查：5 > -∞ && 5 < +∞ ✓
  
  检查左子树：
  validateBSTRange(1, LONG_MIN, 5)
    范围：(-∞, 5)
    检查：1 > -∞ && 1 < 5 ✓
    返回 true
  
  检查右子树：
  validateBSTRange(4, 5, LONG_MAX)
    范围：(5, +∞)
    检查：4 > 5? false ✗
    返回 false

整体返回 false（正确识别非法BST）✓
```

**为什么用long类型？**

```
边界问题：

测试用例：[INT_MAX]

用int：
  validateBSTRange(INT_MAX, INT_MIN, INT_MAX)
  检查：INT_MAX < INT_MAX? false ✗
  错误返回false

用long：
  validateBSTRange(INT_MAX, LONG_MIN, LONG_MAX)
  检查：INT_MAX < LONG_MAX? true ✓
  正确返回true

同理，[INT_MIN]也需要用long处理
```

**易错点总结**：

**🐛 Bug #1: 只检查直接子节点**
```c
// ❌ 错误：只检查父子关系
bool isValidBST(struct TreeNode* root) {
    if (root->left && root->left->val >= root->val)
        return false;
    if (root->right && root->right->val <= root->val)
        return false;
    return isValidBST(root->left) && isValidBST(root->right);
}
// 无法检测深层违规（如孙子节点违反祖父约束）

// ✅ 正确：传递范围约束
bool validateBSTRange(root, minValue, maxValue) {
    if (root->val <= minValue || root->val >= maxValue)
        return false;
    // 范围会逐层传递
}
```

**🐛 Bug #2: 使用int类型**
```c
// ❌ 错误：边界值INT_MIN/INT_MAX会失败
bool validateBSTRange(struct TreeNode* root, int min, int max)

// ✅ 正确：使用long类型
bool validateBSTRange(struct TreeNode* root, long minValue, long maxValue)
```

**🐛 Bug #3: 先检查子树，后检查当前节点**
```c
// ❌ 效率低：即使当前节点不合法，仍会递归子树
if (!validateBSTRange(root->left, ...)) return false;
if (!validateBSTRange(root->right, ...)) return false;
if (root->val <= minValue || root->val >= maxValue)
    return false;

// ✅ 高效：先检查当前节点（提前剪枝）
if (root->val <= minValue || root->val >= maxValue)
    return false;
if (!validateBSTRange(root->left, ...)) return false;
if (!validateBSTRange(root->right, ...)) return false;
```

**递归的调用顺序 vs 处理顺序（深度理解）**：

从"调用顺序"上看：
- 递归是从根节点往下"钻"到叶子
- 调用栈不断深入：root → left → left's left → ...
- 这时调用栈最深，但还没有真正"处理"节点

从"处理顺序"上看：
- 到达叶子后，开始返回
- 叶子先处理完，返回给父节点
- 父节点再处理完，返回给祖父节点
- 逐层向上，最后回到根
- 这就是后序遍历的"自底向上"效果

**图解递归过程**：

```
    10
   /  \
  5   15

调用顺序（往下钻）：
1. validateBSTRange(10, ...)  [调用，未处理]
2.   validateBSTRange(5, ...)  [调用，未处理]
3.     NULL（叶子），返回true
4.     NULL（叶子），返回true
5.   validateBSTRange(15, ...) [调用，未处理]
6.     NULL（叶子），返回true
7.     NULL（叶子），返回true

处理顺序（往上爬）：
8.   节点5处理完成，返回true
9.   节点15处理完成，返回true
10. 节点10处理完成，返回true（最后处理）

写在"子递归调用之后"的代码，执行顺序就是从叶子往上到root！
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 访问每个节点 | O(n) | 每个节点检查一次 |
| 每个节点的检查 | O(1) | 常数时间 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 递归栈 | O(h) | 树的高度 |
| **总计** | **O(h)** | h从log(n)到n |

**测试用例**：

```c
// 用例1：合法BST
输入：[2,1,3]
输出：true

// 用例2：非法BST（值不对）
输入：[5,1,4,null,null,3,6]
输出：false（4 < 5，违反右子树 > 根）

// 用例3：边界值
输入：[2147483647]（INT_MAX）
输出：true（必须用long才能通过）

// 用例4：边界值
输入：[-2147483648]（INT_MIN）
输出：true（必须用long才能通过）
```

**关键要点**：
- ✅ 范围约束是递归传递的（不是只看直接子节点）
- ✅ 必须使用long类型（避免INT_MIN/INT_MAX边界问题）
- ✅ 左子树：min继承，max=root->val
- ✅ 右子树：min=root->val，max继承
- ✅ 先检查当前节点可以提前剪枝（优化）

**记忆技巧**：

```
BST范围约束：
左子树：(min, root->val)  - min继承
右子树：(root->val, max)  - max继承

边界值：必须用long
- LONG_MIN < INT_MIN
- LONG_MAX > INT_MAX
```

### 10.2 二叉树的层序遍历（102）

**核心思想**：BFS + 队列

**问题描述**：
给定一个二叉树，返回其节点值的层序遍历（逐层从左到右访问所有节点）。

**示例**：

```
输入: [3,9,20,null,null,15,7]
    3
   / \
  9  20
    /  \
   15   7
输出: [[3], [9,20], [15,7]]
```

**层序遍历标准模板**：

```c
int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    if (root == NULL) return NULL;
    
    // 初始化队列
    queueSize = 0;
    frontIndex = 0;
    rearIndex = 0;
    
    enqueueNode(root);
    int currentLevel = 0;
    
    while (!isQueueEmpty()) {
        // 关键：记录当前层节点数量
        int currentLevelSize = getQueueSize();
        
        // 遍历当前层的所有节点
        for (int i = 0; i < currentLevelSize; i++) {
            struct TreeNode* node = dequeueNode();
            
            // 记录节点值
            result[currentLevel][i] = node->val;
            
            // 入队子节点
            if (node->left) enqueueNode(node->left);
            if (node->right) enqueueNode(node->right);
        }
        
        currentLevel++;
    }
    
    return result;
}
```

**关键要点**：
- ✅ 使用队列（FIFO）实现BFS
- ✅ `currentLevelSize = queueSize` 固定当前层大小
- ✅ for循环只处理当前层，不处理新入队的节点
- ✅ 必须重置全局队列变量（避免越界）
- ✅ 队列大小设为10000（足够大，避免越界）

**时间复杂度**：O(n) - 每个节点访问一次  
**空间复杂度**：O(w) - w是树的最大宽度

**应用场景**：
- 二叉树的层序遍历（102）- 返回每层节点
- 二叉树的右视图（199）- 记录每层最右节点
- 二叉树的锯齿形遍历（103）- 奇偶层不同顺序
- 二叉树的最大深度（104）- 统计层数
- 填充next指针（117）- 连接每层节点

### 10.3 二叉树的锯齿形层序遍历（103）

**核心思想**：BFS + 方向标记 + 逆序存储

**问题描述**：
给定一个二叉树，返回其节点值的锯齿形层序遍历（奇数层从左到右，偶数层从右到左）。

**示例**：

```
输入: [3,9,20,null,null,15,7]
    3
   / \
  9  20
    /  \
   15   7
   
输出: [[3], [20,9], [15,7]]

第1层：从左到右 -> [3]
第2层：从右到左 -> [20,9]
第3层：从左到右 -> [15,7]
```

**算法实现**：

```c
int** zigzagLevelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    if (root == NULL) return result;
    
    // 初始化队列和方向
    enqueueNode(root);
    Direction currentDirection = LEFT_TO_RIGHT;
    
    while (!isQueueEmpty()) {
        int currentLevelSize = getQueueSize();
        
        // 按需分配当前层空间
        result[currentLevel] = malloc(sizeof(int) * currentLevelSize);
        
        // 遍历当前层
        for (int i = 0; i < currentLevelSize; i++) {
            struct TreeNode* node = dequeueNode();
            
            // 根据方向存储
            if (currentDirection == LEFT_TO_RIGHT) {
                result[currentLevel][i] = node->val;  // 正序
            } else {
                result[currentLevel][currentLevelSize-1-i] = node->val;  // 逆序
            }
            
            // 入队子节点（始终先左后右）
            if (node->left) enqueueNode(node->left);
            if (node->right) enqueueNode(node->right);
        }
        
        // 切换方向
        currentDirection = (currentDirection == LEFT_TO_RIGHT) ? 
                          RIGHT_TO_LEFT : LEFT_TO_RIGHT;
        currentLevel++;
    }
    
    return result;
}
```

**详细走查**：输入 `[3,9,20,null,null,15,7]`

```
=== 第1层（LEFT_TO_RIGHT）===
queue: [3]
出队: 9
存储: result[0][0] = 3 (正序)
入队: [9, 20]
结果: [[3]]

=== 第2层（RIGHT_TO_LEFT）===
queue: [9, 20]
i=0: 出队9, 存储到 result[1][2-1-0] = result[1][1] = 9
i=1: 出队20, 存储到 result[1][2-1-1] = result[1][0] = 20
入队: [15, 7]
结果: [[3], [20,9]]  (逆序！)

=== 第3层（LEFT_TO_RIGHT）===
queue: [15, 7]
i=0: 出队15, 存储到 result[2][0] = 15
i=1: 出队7, 存储到 result[2][1] = 7
结果: [[3], [20,9], [15,7]] ✓
```

**逆序存储技巧**：

```
正常存储（i=0,1,2）：
result[level][i] = node->val
存储位置：[0], [1], [2]

逆序存储（i=0,1,2，size=3）：
result[level][size-1-i] = node->val
i=0 -> [2]
i=1 -> [1]
i=2 -> [0]
存储位置：[2], [1], [0]（逆序！）
```

**方向切换**：

```c
// 方法1：三元运算符
currentDirection = (currentDirection == LEFT_TO_RIGHT) ? 
                   RIGHT_TO_LEFT : LEFT_TO_RIGHT;

// 方法2：取反（如果用bool）
leftToRight = !leftToRight;

// 方法3：异或（如果用0/1）
currentDirection ^= 1;
```

**易错点总结**：

**🐛 Bug #1: 改变入队顺序**
```c
// ❌ 错误：改变入队顺序会破坏树结构
if (currentDirection == LEFT_TO_RIGHT) {
    enqueue(node->left);
    enqueue(node->right);
} else {
    enqueue(node->right);  // 错误！
    enqueue(node->left);
}

// ✅ 正确：始终先左后右入队，只改变存储顺序
enqueue(node->left);
enqueue(node->right);
```

**🐛 Bug #2: 逆序计算错误**
```c
// ❌ 错误：索引计算错误
result[currentLevel][currentLevelSize - i] = node->val;  // 会越界

// ✅ 正确：减1
result[currentLevel][currentLevelSize - 1 - i] = node->val;
```

**🐛 Bug #3: 方向初始化错误**
```c
// ❌ 错误：从RIGHT_TO_LEFT开始
Direction currentDirection = RIGHT_TO_LEFT;

// ✅ 正确：第1层从左到右
Direction currentDirection = LEFT_TO_RIGHT;
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 访问每个节点 | O(n) | BFS遍历 |
| 每个节点的处理 | O(1) | 常数时间 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 队列 | O(w) | w是树的最大宽度 |
| 结果数组 | O(n) | 存储所有节点 |
| **总计** | **O(n)** | 线性空间 |

**测试用例**：

```c
// 用例1：完全二叉树
输入：[3,9,20,null,null,15,7]
输出：[[3], [20,9], [15,7]]

// 用例2：右偏树
输入：[1,null,2,null,3]
输出：[[1], [2], [3]]

// 用例3：对称树
输入：[1,2,2,3,4,4,3]
输出：[[1], [2,2], [3,4,4,3]]

// 用例4：单节点
输入：[1]
输出：[[1]]
```

**与102题对比**：

| 特性 | 102题（层序遍历） | 103题（锯齿形遍历） |
|-----|-----------------|-------------------|
| 遍历方式 | 每层从左到右 | 奇偶层方向不同 |
| 存储方式 | `result[level][i]` | 根据方向决定 |
| 入队顺序 | 先左后右 | 先左后右（相同） |
| 额外变量 | 无 | Direction 枚举 |

**关键要点**：
- ✅ 使用BFS层序遍历框架
- ✅ 添加方向标记（LEFT_TO_RIGHT / RIGHT_TO_LEFT）
- ✅ 从右到左时逆序存储：`result[level][size-1-i]`
- ✅ 节点始终先左后右入队（不改变入队顺序）
- ✅ 每层结束后切换方向

**记忆技巧**：

```
锯齿形 = 层序遍历 + 逆序存储

正序：result[level][i]
逆序：result[level][size-1-i]

方向切换：
LEFT -> RIGHT -> LEFT -> RIGHT ...
```

### 10.4 从前序与中序遍历构造二叉树（105）

**核心思想**：利用遍历特性递归构建

**遍历特点**：
- 前序遍历：`[根节点, [左子树], [右子树]]`
- 中序遍历：`[[左子树], 根节点, [右子树]]`

**算法步骤**：
```c
buildTreeRecursive(preorder, inorder, preStart, preEnd, inStart, inEnd, hashMap) {
    // 1. 终止条件
    if (preStart > preEnd) return NULL;
    
    // 2. 前序第一个元素是根节点
    preorderRootIdx = preStart;
    rootVal = preorder[preorderRootIdx];
    
    // 3. 在中序中找到根节点位置（哈希表O(1)）
    inorderRootIdx = hashMap[rootVal].index;
    
    // 4. 计算左子树大小
    leftSize = inorderRootIdx - inStart;
    
    // 5. 计算左子树范围
    preLeftStart = preorderRootIdx + 1;
    preLeftEnd = preLeftStart + leftSize - 1;
    inLeftStart = inStart;
    inLeftEnd = inorderRootIdx - 1;
    
    // 6. 计算右子树范围
    preRightStart = preLeftEnd + 1;
    preRightEnd = preEnd;
    inRightStart = inorderRootIdx + 1;
    inRightEnd = inEnd;
    
    // 7. 递归构建左右子树
    root->left = buildTreeRecursive(...左子树范围...);
    root->right = buildTreeRecursive(...右子树范围...);
}
```

**示例走查**：`preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]`

```
第1次递归：
  rootVal = 3, inorderRootIdx = 1
  leftSize = 1
  左子树: preorder[1:1], inorder[0:0] → [9], [9]
  右子树: preorder[2:4], inorder[2:4] → [20,15,7], [15,20,7]

第2次递归（左）：
  rootVal = 9, 左右子树为空

第3次递归（右）：
  rootVal = 20, inorderRootIdx = 3
  leftSize = 1
  左子树: [15]
  右子树: [7]
```

**变量命名体系**：

| 前缀 | 含义 | 示例 |
|------|------|------|
| `pre` | 前序遍历相关 | `preStart`, `preLeftEnd` |
| `in` | 中序遍历相关 | `inStart`, `inRightEnd` |
| `Start` | 范围起始索引 | `preStart` |
| `End` | 范围结束索引 | `preEnd` |
| `Idx` | 单个索引位置 | `preorderRootIdx` |
| `Size` | 节点数量 | `leftSize` |

**关键计算公式**：

```c
// 左子树大小
leftSize = inorderRootIdx - inStart;

// 左子树前序范围
preLeftStart = preorderRootIdx + 1;
preLeftEnd = preLeftStart + leftSize - 1;  // ⚠️ 注意 -1

// 右子树前序范围
preRightStart = preLeftEnd + 1;
preRightEnd = preEnd;
```

**易错点总结**：

**🐛 Bug #1: 索引越界**
```c
// ❌ 错误：多加了1
leftPreorderEnd = leftPreorderStart + leftSize;

// ✅ 正确：包含 leftSize 个元素
leftPreorderEnd = leftPreorderStart + leftSize - 1;
```

**原因分析**：
```
leftSize = 3 (3个节点)
leftPreorderStart = 2 (起始索引)
3个节点占用索引: [2, 3, 4]
结束位置: 2 + 3 - 1 = 4 ✓
```

**🐛 Bug #2: 未释放哈希表**
```c
// ❌ 错误
struct TreeNode* buildTree(...) {
    createInorderHashMap(inorder, inorderSize);
    root = buildTreeRecursive(...);
    return root;  // 忘记释放
}

// ✅ 正确
struct TreeNode* buildTree(...) {
    createInorderHashMap(inorder, inorderSize);
    root = buildTreeRecursive(...);
    freeHashMap();  // 释放内存
    return root;
}
```

**优化技巧**：

**1. 哈希表优化查找**：
```c
// 未优化：O(n) 线性查找
for (i = inStart; i <= inEnd; i++) {
    if (inorder[i] == rootVal) {
        inorderRootIdx = i;
        break;
    }
}
// 总时间复杂度：O(n²)

// 优化：O(1) 哈希表查找
HASH_FIND_INT(hashMap, &rootVal, foundNode);
inorderRootIdx = foundNode->index;
// 总时间复杂度：O(n) ✓
```

**2. 索引传递而非数组切片**：
```c
// ❌ 低效：每次创建新数组 O(n)
int* leftPreorder = copyArray(preorder, preLeftStart, preLeftEnd);

// ✅ 高效：传递索引 O(1)
buildTreeRecursive(preorder, inorder, preLeftStart, preLeftEnd, ...);
```

**3. 全局变量简化参数**：
```c
// 全局哈希表
struct InorderIndexNode *inorderHashMap = NULL;

// 递归函数无需传递 hashMap 参数
struct TreeNode* buildTreeRecursive(...) {
    HASH_FIND_INT(inorderHashMap, &rootVal, foundNode);
    ...
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 构建哈希表 | O(n) | 遍历中序数组 |
| 递归构建树 | O(n) | 每个节点访问一次 |
| 哈希表查找 | O(1) | 查找根节点位置 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 哈希表 | O(n) | 存储n个节点 |
| 递归栈 | O(h) | 树高，最坏O(n) |
| **总计** | **O(n)** | 线性空间 |

**数据结构设计**：

```c
/* 哈希表节点：用于快速查找中序遍历中节点的索引位置 */
struct InorderIndexNode {
    int val;          /* 节点值 */
    int index;        /* 在中序遍历中的索引位置 */
    UT_hash_handle hh;
};

/* 主函数流程 */
struct TreeNode* buildTree(int* preorder, int preorderSize, 
                          int* inorder, int inorderSize) {
    // 1. 边界检查
    if (preorder == NULL || inorder == NULL || 
        preorderSize != inorderSize || preorderSize == 0)
        return NULL;
    
    // 2. 创建哈希映射
    createInorderHashMap(inorder, inorderSize);
    
    // 3. 递归构建树
    root = buildTreeRecursive(preorder, inorder, 
                             0, preorderSize - 1, 
                             0, inorderSize - 1, 
                             inorderHashMap);
    
    // 4. 释放哈希表
    freeHashMap();
    
    return root;
}
```

**测试用例**：

```c
// 用例1：完全二叉树
preorder = [3,9,20,15,7]
inorder  = [9,3,15,20,7]
输出：     3
        /   \
       9    20
           /  \
          15   7

// 用例2：左偏树
preorder = [1,2,3]
inorder  = [3,2,1]
输出：   1
       /
      2
     /
    3

// 用例3：右偏树
preorder = [1,2,3]
inorder  = [1,2,3]
输出：   1
          \
           2
            \
             3

// 用例4：单节点
preorder = [1]
inorder  = [1]
输出：   1
```

**扩展思考**：

**1. 从中序和后序构造二叉树（106）**：
```c
// 后序遍历：[[左子树], [右子树], 根节点]
// 根节点在后序遍历的最后
rootVal = postorder[postEnd];  // 最后一个元素

// 其他逻辑类似
```

**2. 能否用前序和后序构造二叉树？**
- 不能唯一确定（除非是满二叉树）
- 原因：无法确定左右子树的分界
- 例如：`前序=[1,2], 后序=[2,1]`
  - 可能是：`1→left(2)` 或 `1→right(2)`

**3. 二叉树遍历总结**：

| 遍历 | 顺序 | 根节点位置 | 用途 |
|-----|------|-----------|------|
| 前序 | 根-左-右 | 最前 | 复制树结构 |
| 中序 | 左-根-右 | 中间 | BST升序遍历 |
| 后序 | 左-右-根 | 最后 | 释放树内存 |

**关键要点**：
- ✅ 前序遍历确定根节点
- ✅ 中序遍历划分左右子树
- ✅ 哈希表优化查找 O(1)
- ✅ 准确计算索引范围
- ✅ 注意内存释放

### 10.5 从中序与后序遍历构造二叉树（106）

**核心思想**：后序遍历根节点在末尾

**遍历特点**：
- 后序遍历：`[[左子树], [右子树], 根节点]`
- 中序遍历：`[[左子树], 根节点, [右子树]]`

**与105题的对比**：

| 题目 | 遍历组合 | 根节点位置 | 关键区别 |
|-----|---------|-----------|---------|
| 105 | 前序+中序 | 前序第一个 | 根节点在前序开头 |
| 106 | 中序+后序 | 后序最后一个 | 根节点在后序末尾 |

**算法步骤**：
```c
buildTreeRecursive(inorder, postorder, inStart, inEnd, postStart, postEnd, hashMap) {
    // 1. 终止条件
    if (inStart > inEnd) return NULL;
    
    // 2. 后序最后一个元素是根节点（关键区别！）
    postorderRootIdx = postEnd;  // ⚠️ 不是 postStart
    rootVal = postorder[postorderRootIdx];
    
    // 3. 在中序中找到根节点位置（与105相同）
    inorderRootIdx = hashMap[rootVal].index;
    
    // 4. 计算左子树大小（与105相同）
    leftSize = inorderRootIdx - inStart;
    
    // 5. 计算左子树范围
    inLeftStart = inStart;
    inLeftEnd = inorderRootIdx - 1;
    postLeftStart = postStart;
    postLeftEnd = postLeftStart + leftSize - 1;
    
    // 6. 计算右子树范围
    inRightStart = inorderRootIdx + 1;
    inRightEnd = inEnd;
    postRightStart = postLeftEnd + 1;
    postRightEnd = postEnd - 1;  // ⚠️ 减1，因为根节点在末尾
    
    // 7. 递归构建左右子树
    root->left = buildTreeRecursive(...左子树范围...);
    root->right = buildTreeRecursive(...右子树范围...);
}
```

**示例走查**：`inorder = [9,3,15,20,7], postorder = [9,15,7,20,3]`

```
第1次递归：
  rootVal = 3 (postorder最后一个)
  inorderRootIdx = 1
  leftSize = 1
  左子树: inorder[0:0], postorder[0:0] → [9], [9]
  右子树: inorder[2:4], postorder[1:3] → [15,20,7], [15,7,20]

第2次递归（左）：
  rootVal = 9
  左右子树为空

第3次递归（右）：
  rootVal = 20 (postorder[1:3]的最后一个)
  inorderRootIdx = 3
  leftSize = 1
  左子树: [15]
  右子树: [7]
```

**变量命名体系**（与105题保持一致）：

| 前缀 | 含义 | 示例 |
|------|------|------|
| `post` | 后序遍历相关 | `postStart`, `postLeftEnd` |
| `in` | 中序遍历相关 | `inStart`, `inRightEnd` |
| `Start` | 范围起始索引 | `postStart` |
| `End` | 范围结束索引 | `postEnd` |
| `Idx` | 单个索引位置 | `postorderRootIdx` |
| `Size` | 节点数量 | `leftSize` |

**关键计算公式**：

```c
// 根节点位置（关键区别）
postorderRootIdx = postEnd;  // 在末尾！

// 左子树大小（相同）
leftSize = inorderRootIdx - inStart;

// 左子树后序范围
postLeftStart = postStart;
postLeftEnd = postLeftStart + leftSize - 1;

// 右子树后序范围
postRightStart = postLeftEnd + 1;
postRightEnd = postEnd - 1;  // ⚠️ 减1，排除根节点
```

**索引关系图解**：

```
后序遍历数组：[左子树 | 右子树 | 根]
索引关系：    postStart...postLeftEnd  postRightStart...postEnd-1  postEnd
                   │                           │                      │
                   └─ leftSize 个元素          └─ rightSize 个元素    └─ 根节点

中序遍历数组：[左子树 | 根 | 右子树]
索引关系：    inStart...inLeftEnd  inorderRootIdx  inRightStart...inEnd
```

**易错点总结**：

**🐛 Bug #1: 根节点位置错误**
```c
// ❌ 错误：用了前序的思路
rootVal = postorder[postStart];

// ✅ 正确：后序根节点在末尾
rootVal = postorder[postEnd];
```

**🐛 Bug #2: 右子树范围计算错误**
```c
// ❌ 错误：没有排除根节点
postRightEnd = postEnd;

// ✅ 正确：减1排除根节点
postRightEnd = postEnd - 1;
```

**105题 vs 106题对比**：

**相同点**：
- ✅ 都需要中序遍历划分左右子树
- ✅ 都使用哈希表优化查找 O(1)
- ✅ 左子树大小计算方式相同
- ✅ 时间复杂度都是 O(n)

**不同点**：

| 特性 | 105题（前序+中序） | 106题（中序+后序） |
|-----|------------------|------------------|
| 根节点位置 | `preorder[preStart]` | `postorder[postEnd]` |
| 根节点在哪 | 遍历数组开头 | 遍历数组末尾 |
| 右子树结束 | `preEnd` | `postEnd - 1` |
| 参数顺序 | preorder, inorder | inorder, postorder |

**完整代码框架**：

```c
/* 哈希表节点 */
struct InorderIndexNode {
    int val;
    int index;
    UT_hash_handle hh;
};

struct InorderIndexNode *inorderHashMap = NULL;

/* 创建哈希映射 */
void createInorderHashMap(int* inorder, int inorderSize) {
    for (i = 0; i < inorderSize; i++) {
        node = malloc(sizeof(struct InorderIndexNode));
        node->val = inorder[i];
        node->index = i;
        HASH_ADD_INT(inorderHashMap, val, node);
    }
}

/* 递归构建 */
struct TreeNode *buildTreeRecursive(...) {
    if (inStart > inEnd) return NULL;
    
    // 关键：根节点在末尾
    rootVal = postorder[postEnd];
    
    // 查找 + 计算范围
    HASH_FIND_INT(hashMap, &rootVal, foundNode);
    inorderRootIdx = foundNode->index;
    leftSize = inorderRootIdx - inStart;
    
    // 左子树
    postLeftStart = postStart;
    postLeftEnd = postLeftStart + leftSize - 1;
    
    // 右子树
    postRightStart = postLeftEnd + 1;
    postRightEnd = postEnd - 1;  // 减1！
    
    // 递归
    root->left = buildTreeRecursive(...);
    root->right = buildTreeRecursive(...);
    
    return root;
}

/* 主函数 */
struct TreeNode* buildTree(int* inorder, int inorderSize, 
                          int* postorder, int postorderSize) {
    createInorderHashMap(inorder, inorderSize);
    root = buildTreeRecursive(inorder, postorder, inorderHashMap, 
                             0, inorderSize - 1, 
                             0, postorderSize - 1);
    freeHashMap();
    return root;
}
```

**测试用例**：

```c
// 用例1：完全二叉树
inorder   = [9,3,15,20,7]
postorder = [9,15,7,20,3]
输出：     3
        /   \
       9    20
           /  \
          15   7

// 用例2：左偏树
inorder   = [3,2,1]
postorder = [3,2,1]
输出：   1
       /
      2
     /
    3

// 用例3：右偏树
inorder   = [1,2,3]
postorder = [1,2,3]
输出：   1
          \
           2
            \
             3
```

**记忆技巧**：

```
前序（Pre-order）：根在前面 → preorder[preStart]
后序（Post-order）：根在后面 → postorder[postEnd]
中序（In-order）：根在中间 → 用于划分左右
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 构建哈希表 | O(n) | 遍历中序数组 |
| 递归构建树 | O(n) | 每个节点访问一次 |
| 哈希表查找 | O(1) | 查找根节点位置 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 哈希表 | O(n) | 存储n个节点 |
| 递归栈 | O(h) | 树高，最坏O(n) |
| **总计** | **O(n)** | 线性空间 |

**关键要点**：
- ✅ 后序遍历根节点在末尾（与前序相反）
- ✅ 右子树范围要减1排除根节点
- ✅ 其他逻辑与105题完全相同
- ✅ 掌握一题，另一题只需改根节点位置

### 10.6 二叉树展开为链表（114）

**核心思想**：Morris 遍历 + O(1) 空间

**问题描述**：
给定一个二叉树，原地将它展开为"链表"（使用 right 指针连接），展开后的顺序应该是原二叉树的**前序遍历**顺序。

**示例**：

```
输入：
    1
   / \
  2   5
 / \   \
3   4   6

输出：
1
 \
  2
   \
    3
     \
      4
       \
        5
         \
          6

前序遍历顺序：1 -> 2 -> 3 -> 4 -> 5 -> 6
```

**方法对比**：

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|-----|-----------|-----------|------|
| 递归法 | O(n) | O(h) | 简单直观，使用递归栈 |
| Morris遍历 | O(n) | O(1) | 巧妙，真正的O(1)空间 |

**方法1：递归法**

```c
void flattenRecursive(struct TreeNode *root, struct TreeNode **prevNode) {
    struct TreeNode *leftChild, *rightChild;
    
    if (root == NULL) return;
    
    // 关键：先保存左右子树指针
    leftChild = root->left;
    rightChild = root->right;
    
    // 将当前节点连接到前一个节点的右边
    (*prevNode)->right = root;
    (*prevNode)->left = NULL;
    (*prevNode) = root;
    
    // 递归处理左子树和右子树
    flattenRecursive(leftChild, prevNode);
    flattenRecursive(rightChild, prevNode);
}
```

**易错点**：
```c
// ❌ 错误：直接使用 root->left，在递归中会丢失
flattenRecursive(root->left, prevNode);
flattenRecursive(root->right, prevNode);

// ✅ 正确：先保存指针
leftChild = root->left;
rightChild = root->right;
flattenRecursive(leftChild, prevNode);
flattenRecursive(rightChild, prevNode);
```

**方法2：Morris 遍历法（推荐）**

**核心思想**：
利用树中的空闲指针（左子树最右节点的 right 指针）来保存信息。

**算法步骤**：

```c
void flatten(struct TreeNode* root) {
    struct TreeNode *current = root;
    struct TreeNode *predecessor = NULL;
    
    while (current != NULL) {
        // 情况1：没有左子树，直接往右走
        if (!current->left) {
            current = current->right;
        }
        // 情况2：有左子树，需要重组结构
        else {
            // 步骤1: 找到左子树的最右节点
            predecessor = current->left;
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }
            
            // 步骤2: 将右子树接到predecessor右边
            predecessor->right = current->right;
            
            // 步骤3: 将左子树移到右边
            current->right = current->left;
            current->left = NULL;
            
            // 步骤4: 继续往右走
            current = current->right;
        }
    }
}
```

**Morris 遍历详细走查**：

输入：`[1,2,5,3,4,null,6]`

```
=== 初始状态 ===
    1
   / \
  2   5
 / \   \
3   4   6

=== 第1步：处理节点1 ===
current = 1, 有左子树
1. 找 predecessor: 2 -> 4 (左子树最右节点)
2. predecessor->right = 5 (将5接到4右边)
3. 将左子树移到右边：
   1
    \
     2
    / \
   3   4
        \
         5
          \
           6

=== 第2步：处理节点2 ===
current = 2, 有左子树
1. 找 predecessor: 3 (左子树最右节点)
2. predecessor->right = 4 (将4接到3右边)
3. 将左子树移到右边：
   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6

=== 第3-6步 ===
节点3,4,5,6都没有左子树，直接往右走

最终结果：1 -> 2 -> 3 -> 4 -> 5 -> 6
```

**Morris 遍历图解**：

```
步骤1（原始）：        步骤2（连接右子树）：    步骤3（左树移右边）：
    current               current                 current
       / \                   / \                      \
   left  right          left  right                 left
    /                    /                           /  \
   ...                  ...                        ...  right
    \                    \
  predecessor          predecessor
                           \
                          right
  
操作流程：
1. 找到 predecessor（左子树的最右节点）
2. predecessor->right = right（将右子树接到predecessor）
3. current->right = left（左子树移到右边）
4. current->left = NULL（清空左指针）
```

**关键变量说明**：

| 变量名 | 作用 | 说明 |
|--------|------|------|
| `current` | 当前处理的节点 | 从root开始，沿着right移动 |
| `predecessor` | 左子树的最右节点 | 用于连接右子树 |
| `leftChild` | 保存的左子树指针 | 递归法使用 |
| `rightChild` | 保存的右子树指针 | 递归法使用 |

**为什么Morris遍历是O(1)空间？**

```
传统遍历：需要栈/队列存储节点 → O(h) 或 O(n)
Morris遍历：利用树本身的空指针 → O(1)

关键洞察：
- 左子树的最右节点的 right 指针本来是 NULL
- 我们用它来临时存储信息
- 不需要额外的数据结构
```

**复杂度分析**：

**时间复杂度**：O(n)
- 每个节点最多访问2次
- 第1次：处理该节点
- 第2次：寻找predecessor时可能经过该节点
- 总操作次数：O(n)

**空间复杂度**：
- 递归法：O(h) - 递归栈，h是树的高度
- Morris法：O(1) - 只使用几个指针变量

**易错点总结**：

**🐛 Bug #1: 忘记保存子树指针**
```c
// ❌ 错误：修改结构后再访问
(*prevNode)->right = root;
(*prevNode) = root;
flattenRecursive(root->left, prevNode);  // root->left可能已改变

// ✅ 正确：先保存
leftChild = root->left;
rightChild = root->right;
(*prevNode)->right = root;
flattenRecursive(leftChild, prevNode);
```

**🐛 Bug #2: predecessor 查找错误**
```c
// ❌ 错误：可能找到错误的节点
predecessor = current->left;
while (predecessor->right != NULL && predecessor->right != current) {
    predecessor = predecessor->right;
}

// ✅ 正确：在这题中不需要检查环
predecessor = current->left;
while (predecessor->right != NULL) {
    predecessor = predecessor->right;
}
```

**🐛 Bug #3: 忘记清空左指针**
```c
// ❌ 错误：只移动右指针，没有清空左指针
current->right = current->left;

// ✅ 正确：必须清空左指针
current->right = current->left;
current->left = NULL;
```

**测试用例**：

```c
// 用例1：普通二叉树
输入：[1,2,5,3,4,null,6]
输出：1->2->3->4->5->6

// 用例2：左偏树
输入：[1,2,null,3,null,4]
输出：1->2->3->4

// 用例3：右偏树
输入：[1,null,2,null,3]
输出：1->2->3

// 用例4：单节点
输入：[1]
输出：1

// 用例5：空树
输入：[]
输出：[]
```

**Morris 遍历的应用**：

1. **前序遍历**（本题）
2. **中序遍历**：经典Morris遍历
3. **后序遍历**：需要额外处理
4. **任何需要O(1)空间遍历二叉树的场景**

**记忆技巧**：

```
Morris遍历口诀：
找到前驱（predecessor）
接上右树（right subtree）
左树右移（move left to right）
清空左边（set left NULL）
继续右行（go right）
```

**关键要点**：
- ✅ Morris遍历是真正的O(1)空间复杂度
- ✅ 核心是利用左子树最右节点的空指针
- ✅ 递归法必须先保存左右子树指针
- ✅ 时间复杂度虽然看起来是O(n²)，实际是O(n)
- ✅ 每个节点的right指针最多被修改2次

### 10.7 填充每个节点的下一个右侧节点指针 II（117）

**核心思想**：层序遍历 + O(1) 空间

**问题描述**：
给定一个二叉树：
```c
struct Node {
    int val;
    struct Node *left;
    struct Node *right;
    struct Node *next;  // 指向右侧节点
};
```

填充每个 `next` 指针，使其指向下一个右侧节点。如果没有右侧节点，则 `next` 指针设为 `NULL`。

**示例**：

```
输入：
      1
     / \
    2   3
   / \   \
  4   5   7

输出：
      1 -> NULL
     / \
    2 ->3 -> NULL
   / \   \
  4->5-> 7 -> NULL
```

**核心技巧**：利用已建立的 next 指针遍历

**算法思路**：

```c
void connectLevel(struct Node* levelHead) {
    struct Node* current = levelHead;        // 当前层的遍历指针
    struct Node* nextLevelHead = NULL;       // 下一层的头节点
    struct Node* nextLevelTail = NULL;       // 下一层的尾节点
    
    if (!levelHead) return;
    
    // 遍历当前层（通过 next 指针）
    while (current) {
        // 1. 找到下一层的第一个节点
        if (!nextLevelHead) {
            nextLevelHead = current->left ? current->left : current->right;
        }
        
        // 2. 连接左子节点
        if (current->left) {
            if (nextLevelTail) {
                nextLevelTail->next = current->left;
            }
            nextLevelTail = current->left;
        }
        
        // 3. 连接右子节点
        if (current->right) {
            if (nextLevelTail) {
                nextLevelTail->next = current->right;
            }
            nextLevelTail = current->right;
        }
        
        // 4. 移动到当前层的下一个节点
        current = current->next;
    }
    
    // 5. 递归处理下一层
    connectLevel(nextLevelHead);
}
```

**详细走查**：`[1,2,3,4,5,null,7]`

```
=== 第1层：处理节点1 ===
current = 1
nextLevelHead = 2 (第一个子节点)
nextLevelTail = 2

处理1的左子节点2：
  nextLevelTail = 2

处理1的右子节点3：
  nextLevelTail->next = 3  (即 2->next = 3)
  nextLevelTail = 3

结果：2 -> 3 -> NULL

=== 第2层：处理节点2和3 ===
current = 2
nextLevelHead = 4 (2的左子节点)
nextLevelTail = 4

处理2的左子节点4：
  nextLevelTail = 4

处理2的右子节点5：
  nextLevelTail->next = 5  (即 4->next = 5)
  nextLevelTail = 5

current = 3 (通过next指针移动)

处理3的右子节点7：
  nextLevelTail->next = 7  (即 5->next = 7)
  nextLevelTail = 7

结果：4 -> 5 -> 7 -> NULL

=== 第3层：处理节点4, 5, 7 ===
都是叶子节点，nextLevelHead = NULL，递归终止
```

**关键变量说明**：

| 变量名 | 作用 | 说明 |
|--------|------|------|
| `levelHead` | 当前层的头节点 | 递归参数 |
| `current` | 当前层的遍历指针 | 通过 next 移动 |
| `nextLevelHead` | 下一层的头节点 | 记录第一个子节点 |
| `nextLevelTail` | 下一层的尾节点 | 用于连接新节点 |

**图解连接过程**：

```
当前层：  A -> B -> C -> NULL
         /\   /    /\
下一层： D  E F   G  H

处理A：
  nextLevelTail = D
  nextLevelTail = E (D->next = E)
  
处理B：
  nextLevelTail = F (E->next = F)
  
处理C：
  nextLevelTail = G (F->next = G)
  nextLevelTail = H (G->next = H)

结果：D -> E -> F -> G -> H -> NULL
```

**与116题的区别**：

| 特性 | 116题 | 117题 |
|-----|-------|-------|
| 树类型 | 完美二叉树 | 任意二叉树 |
| 子节点 | 每个节点都有2个或0个子节点 | 可能只有1个子节点 |
| 难度 | 较简单 | 较复杂 |
| 核心区别 | 可以假设每层都是满的 | 需要动态找下一层的头节点 |

**易错点总结**：

**🐛 Bug #1: 返回值错误**
```c
// ❌ 错误：返回了下一层的节点
struct Node* connectLevel(struct Node* levelHead) {
    ...
    return connectLevel(nextLevelHead);
}

// ✅ 正确：返回当前层的头节点
void connectLevel(struct Node* levelHead) {
    ...
    connectLevel(nextLevelHead);
    return;
}

struct Node* connect(struct Node* root) {
    connectLevel(root);
    return root;  // 返回 root
}
```

**🐛 Bug #2: 忘记更新 nextLevelTail**
```c
// ❌ 错误：只连接不更新尾节点
if (current->left) {
    if (nextLevelTail) {
        nextLevelTail->next = current->left;
    }
    // 忘记更新 nextLevelTail
}

// ✅ 正确：必须更新尾节点
if (current->left) {
    if (nextLevelTail) {
        nextLevelTail->next = current->left;
    }
    nextLevelTail = current->left;  // 更新尾节点
}
```

**🐛 Bug #3: nextLevelHead 初始化错误**
```c
// ❌ 错误：可能跳过某些节点
nextLevelHead = current->left;

// ✅ 正确：第一个非空子节点
if (!nextLevelHead) {
    nextLevelHead = current->left ? current->left : current->right;
}
```

**优化技巧**：

**1. 迭代实现（避免递归栈）**：
```c
struct Node* connect(struct Node* root) {
    struct Node* levelHead = root;
    
    while (levelHead) {
        struct Node* current = levelHead;
        struct Node* nextLevelHead = NULL;
        struct Node* nextLevelTail = NULL;
        
        while (current) {
            // 处理左右子节点
            if (!nextLevelHead) {
                nextLevelHead = current->left ? current->left : current->right;
            }
            
            if (current->left) {
                if (nextLevelTail) nextLevelTail->next = current->left;
                nextLevelTail = current->left;
            }
            
            if (current->right) {
                if (nextLevelTail) nextLevelTail->next = current->right;
                nextLevelTail = current->right;
            }
            
            current = current->next;
        }
        
        levelHead = nextLevelHead;  // 移动到下一层
    }
    
    return root;
}
```

**2. 使用哑节点简化逻辑**：
```c
struct Node dummy = {0};
struct Node* tail = &dummy;

// 连接时不需要检查 tail 是否为 NULL
tail->next = current->left;
tail = tail->next;
```

**复杂度分析**：

| 实现方式 | 时间复杂度 | 空间复杂度 | 说明 |
|---------|-----------|-----------|------|
| 递归 | O(n) | O(h) | h 是树的高度 |
| 迭代 | O(n) | O(1) | 真正的常数空间 |
| 队列BFS | O(n) | O(w) | w 是树的最大宽度 |

**测试用例**：

```c
// 用例1：普通二叉树
输入：[1,2,3,4,5,null,7]
输出：1->NULL, 2->3->NULL, 4->5->7->NULL

// 用例2：左偏树
输入：[1,2,null,3,null,4]
输出：1->NULL, 2->NULL, 3->NULL, 4->NULL

// 用例3：右偏树
输入：[1,null,2,null,3]
输出：1->NULL, 2->NULL, 3->NULL

// 用例4：空树
输入：[]
输出：[]

// 用例5：单节点
输入：[1]
输出：1->NULL
```

**关键要点**：
- ✅ 利用已建立的 next 指针遍历当前层
- ✅ 同时建立下一层的 next 连接
- ✅ 动态查找下一层的头节点（可能是左或右子节点）
- ✅ 迭代实现可以达到 O(1) 空间
- ✅ 必须更新 nextLevelTail 才能正确连接

**记忆技巧**：

```
当前层 → 下一层
  ↓        ↓
通过next  建立next
遍历      连接
```

**应用场景**：
- 层序遍历不使用队列
- 利用树的结构信息进行优化
- 原地修改数据结构

### 10.8 求根节点到叶节点数字之和（129）

**核心思想**：DFS路径累积

**问题描述**：
给定一个二叉树，每个节点都包含数字 0-9，从根节点到叶子节点的每条路径代表一个数字，计算所有路径表示的数字之和。

**示例**：

```
输入：
    1              路径: 1->2 => 12
   / \                   1->3 => 13
  2   3            总和: 12 + 13 = 25

输入：
    4              路径: 4->9->5 => 495
   / \                   4->9->1 => 491
  9   0                  4->0   => 40
 / \              总和: 495 + 491 + 40 = 1026
5   1
```

**算法思路**：

```c
void dfsPathSum(struct TreeNode* root, int pathNumber, int* totalSum) {
    if (root == NULL) return;
    
    // 1. 计算当前路径数字
    int currentNumber = pathNumber * 10 + root->val;
    
    // 2. 如果是叶子节点，累加到总和
    if (root->left == NULL && root->right == NULL) {
        *totalSum += currentNumber;
        return;
    }
    
    // 3. 递归遍历左右子树
    dfsPathSum(root->left, currentNumber, totalSum);
    dfsPathSum(root->right, currentNumber, totalSum);
}

int sumNumbers(struct TreeNode* root) {
    int totalSum = 0;
    dfsPathSum(root, 0, &totalSum);
    return totalSum;
}
```

**详细走查**：

输入：`[4,9,0,5,1]`

```
    4
   / \
  9   0
 / \
5   1

=== DFS遍历过程 ===

访问节点4: pathNumber=0
  currentNumber = 0 * 10 + 4 = 4
  
  访问节点9: pathNumber=4
    currentNumber = 4 * 10 + 9 = 49
    
    访问节点5: pathNumber=49
      currentNumber = 49 * 10 + 5 = 495
      叶子节点，totalSum += 495
      totalSum = 495
    
    访问节点1: pathNumber=49
      currentNumber = 49 * 10 + 1 = 491
      叶子节点，totalSum += 491
      totalSum = 986
  
  访问节点0: pathNumber=4
    currentNumber = 4 * 10 + 0 = 40
    叶子节点，totalSum += 40
    totalSum = 1026

最终结果：495 + 491 + 40 = 1026
```

**关键变量说明**：

| 变量名 | 作用 | 说明 |
|--------|------|------|
| `pathNumber` | 从根到父节点的路径数字 | 传递给子节点 |
| `currentNumber` | 从根到当前节点的路径数字 | pathNumber * 10 + root->val |
| `totalSum` | 所有路径数字之和 | 指针参数，累加结果 |

**核心公式**：

```
从根到当前节点的数字 = 从根到父节点的数字 × 10 + 当前节点值

示例：
根节点4: 0 * 10 + 4 = 4
节点9:   4 * 10 + 9 = 49
节点5:   49 * 10 + 5 = 495
```

**递归过程图解**：

```
    4 (pathNumber=0, currentNumber=4)
   / \
  9   0 (pathNumber=4, currentNumber=49/40)
 / \
5   1 (pathNumber=49, currentNumber=495/491)

遍历顺序（前序DFS）：
4 -> 9 -> 5（累加495）-> 回溯 -> 1（累加491）-> 回溯 -> 0（累加40）
```

**易错点总结**：

**🐛 Bug #1: 所有节点都累加**
```c
// ❌ 错误：在每个节点都累加
void dfsPathSum(struct TreeNode* root, int pathNumber, int* totalSum) {
    if (root == NULL) return;
    int currentNumber = pathNumber * 10 + root->val;
    *totalSum += currentNumber;  // 错误！包括非叶子节点
    dfsPathSum(root->left, currentNumber, totalSum);
    dfsPathSum(root->right, currentNumber, totalSum);
}

// ✅ 正确：只在叶子节点累加
if (root->left == NULL && root->right == NULL) {
    *totalSum += currentNumber;
}
```

**🐛 Bug #2: 忘记乘10**
```c
// ❌ 错误：直接相加
currentNumber = pathNumber + root->val;  // 错误！

// ✅ 正确：乘10再加
currentNumber = pathNumber * 10 + root->val;
```

**🐛 Bug #3: 使用root->val而不是currentNumber**
```c
// ❌ 错误：只累加当前节点值
if (root->left == NULL && root->right == NULL) {
    *totalSum += root->val;  // 错误！
}

// ✅ 正确：累加整个路径数字
if (root->left == NULL && root->right == NULL) {
    *totalSum += currentNumber;
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 访问每个节点 | O(n) | DFS遍历 |
| 计算路径数字 | O(1) | 每个节点 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 递归栈 | O(h) | 树的高度 |
| 其他变量 | O(1) | 常数空间 |
| **总计** | **O(h)** | h=log(n)到n |

**测试用例**：

```c
// 用例1：平衡树
输入：[1,2,3]
输出：25 (12 + 13)

// 用例2：左偏树
输入：[1,2,null,3]
输出：123 (1->2->3)

// 用例3：右偏树
输入：[1,null,2,null,3]
输出：123 (1->2->3)

// 用例4：单节点
输入：[0]
输出：0

// 用例5：包含0的树
输入：[4,9,0,5,1]
输出：1026 (495 + 491 + 40)
```

**变体问题**：

**1. 求路径数字的最大值**：
```c
void dfsMaxPath(struct TreeNode* root, int pathNumber, int* maxSum) {
    if (root == NULL) return;
    int currentNumber = pathNumber * 10 + root->val;
    
    if (root->left == NULL && root->right == NULL) {
        if (currentNumber > *maxSum) {
            *maxSum = currentNumber;
        }
        return;
    }
    
    dfsMaxPath(root->left, currentNumber, maxSum);
    dfsMaxPath(root->right, currentNumber, maxSum);
}
```

**2. 统计路径数字个数**：
```c
void dfsCountPaths(struct TreeNode* root, int pathNumber, int* count) {
    if (root == NULL) return;
    int currentNumber = pathNumber * 10 + root->val;
    
    if (root->left == NULL && root->right == NULL) {
        (*count)++;
        return;
    }
    
    dfsCountPaths(root->left, currentNumber, count);
    dfsCountPaths(root->right, currentNumber, count);
}
```

**3. 返回所有路径数字（不累加）**：
```c
void dfsCollectPaths(struct TreeNode* root, int pathNumber, int* result, int* index) {
    if (root == NULL) return;
    int currentNumber = pathNumber * 10 + root->val;
    
    if (root->left == NULL && root->right == NULL) {
        result[(*index)++] = currentNumber;
        return;
    }
    
    dfsCollectPaths(root->left, currentNumber, result, index);
    dfsCollectPaths(root->right, currentNumber, result, index);
}
```

**迭代实现（使用栈）**：

```c
int sumNumbers(struct TreeNode* root) {
    if (root == NULL) return 0;
    
    int totalSum = 0;
    struct StackNode {
        struct TreeNode* node;
        int pathNumber;
    } stack[1000];
    int top = 0;
    
    stack[top++] = (struct StackNode){root, 0};
    
    while (top > 0) {
        struct StackNode current = stack[--top];
        int currentNumber = current.pathNumber * 10 + current.node->val;
        
        // 叶子节点
        if (current.node->left == NULL && current.node->right == NULL) {
            totalSum += currentNumber;
            continue;
        }
        
        // 压栈（注意顺序：右->左，保证左子树先处理）
        if (current.node->right) {
            stack[top++] = (struct StackNode){current.node->right, currentNumber};
        }
        if (current.node->left) {
            stack[top++] = (struct StackNode){current.node->left, currentNumber};
        }
    }
    
    return totalSum;
}
```

**关键要点**：
- ✅ DFS前序遍历，累积路径数字
- ✅ 路径数字 = 父数字 × 10 + 当前值
- ✅ 只在叶子节点累加到总和
- ✅ 使用指针参数传递累加结果
- ✅ 可以用递归或迭代实现

**记忆技巧**：

```
路径累积公式：
父数字 × 10 + 当前值

类比十进制：
123 = 12 × 10 + 3
    = (1 × 10 + 2) × 10 + 3
```

**应用场景**：
- 树的路径问题
- 数字累积计算
- DFS遍历应用
- 路径相关统计

### 10.9 二叉搜索树迭代器（173）

**核心思想**：栈模拟中序遍历 + 按需推进

**问题描述**：
实现一个二叉搜索树迭代器，支持 `next()` 和 `hasNext()` 操作。

**要求**：
- `next()` 和 `hasNext()` 的平均时间复杂度为 **O(1)**
- 空间复杂度为 **O(h)**，h 为树的高度

**示例**：

```
BST:
    7
   / \
  3   15
     /  \
    9   20

调用序列：
next()    -> 3
next()    -> 7
hasNext() -> true
next()    -> 9
hasNext() -> true
next()    -> 15
hasNext() -> true
next()    -> 20
hasNext() -> false

中序遍历顺序（升序）：3, 7, 9, 15, 20
```

**核心数据结构**：

```c
typedef struct {
    Stack nodeStack;          /* 栈：存储遍历路径上的祖先节点 */
    struct TreeNode *current; /* 当前待访问的节点 */
} BSTIterator;
```

**算法实现**：

```c
/* 创建迭代器 */
BSTIterator* bSTIteratorCreate(struct TreeNode* root) {
    BSTIterator *iterator = malloc(sizeof(BSTIterator));
    initStack(&iterator->nodeStack);
    iterator->current = root;
    return iterator;
}

/* 返回下一个最小值 */
int bSTIteratorNext(BSTIterator* obj) {
    struct TreeNode *node;
    
    // 步骤1: 将current及其所有左子节点入栈
    while (obj->current != NULL) {
        pushStack(&obj->nodeStack, obj->current);
        obj->current = obj->current->left;
    }
    
    // 步骤2: 弹出栈顶节点（当前最小值）
    node = popStack(&obj->nodeStack);
    
    // 步骤3: 移动到右子树
    obj->current = node->right;
    
    return node->val;
}

/* 判断是否还有下一个元素 */
bool bSTIteratorHasNext(BSTIterator* obj) {
    return obj->current != NULL || !isStackEmpty(&obj->nodeStack);
}
```

**详细走查**：输入 `[7,3,15,null,null,9,20]`

```
BST:
    7
   / \
  3   15
     /  \
    9   20

=== 初始化 ===
stack: []
current: 7

=== 第1次 next() ===
1. while: push(7), push(3)
   stack: [7, 3]
   current: NULL
2. pop() -> node = 3
   stack: [7]
3. current = NULL
返回: 3

=== 第2次 next() ===
1. while: current=NULL，跳过
2. pop() -> node = 7
   stack: []
3. current = 15
返回: 7

=== 第3次 next() ===
1. while: push(15), push(9)
   stack: [15, 9]
   current: NULL
2. pop() -> node = 9
   stack: [15]
3. current = NULL
返回: 9

=== 第4次 next() ===
1. while: 跳过
2. pop() -> node = 15
   stack: []
3. current = 20
返回: 15

=== 第5次 next() ===
1. while: push(20)
   stack: [20]
   current: NULL
2. pop() -> node = 20
   stack: []
3. current = NULL
返回: 20

=== hasNext() ===
current=NULL && stack=[] -> false
```

**关键理解**：

**1. 为什么用栈？**
- 中序遍历需要记住"路径"
- 访问完左子树后，需要回到父节点
- 栈保存了这些等待访问的父节点

**2. current的作用？**
- 指向下一个要处理的子树
- NULL 表示当前分支已访问完，需要从栈中取父节点

**3. 为什么是O(1)平均时间？**
```
n个节点，调用n次next()
总操作：
- 每个节点入栈一次：n次
- 每个节点出栈一次：n次
- 总计：2n次操作
摊还：2n / n = O(1)
```

**易错点总结**：

**🐛 Bug #1: push/pop的++/--顺序错误**
```c
// ❌ 错误：top=-1时，先用后加会访问[-1]
void push(Stack *stack, struct TreeNode *node) {
    stack->nodes[stack->top++] = node;  // 访问[-1]
}

// ✅ 正确：先加后用
void pushStack(Stack *stack, struct TreeNode *node) {
    stack->nodes[++stack->top] = node;  // 先变0，再访问[0]
}

// ❌ 错误：top=0时，先减后用会访问[-1]
struct TreeNode *pop(Stack *stack) {
    return stack->nodes[--stack->top];
}

// ✅ 正确：先用后减
struct TreeNode *popStack(Stack *stack) {
    return stack->nodes[stack->top--];  // 先访问[0]，再变-1
}
```

**记忆口诀**：
```
入栈：++top  （先增后用）
出栈：top--  （先用后减）
```

**🐛 Bug #2: 使用错误的指针**
```c
// ❌ 错误：obj->current可能是NULL
node = pop(&obj->nodeStack);
obj->current = obj->current->right;  // NULL->right崩溃

// ✅ 正确：使用pop出来的node
node = popStack(&obj->nodeStack);
obj->current = node->right;  // 使用node
```

**🐛 Bug #3: hasNext() 逻辑错误**
```c
// ❌ 错误：只检查栈
bool hasNext(BSTIterator* obj) {
    return !isStackEmpty(&obj->nodeStack);
}

// ✅ 正确：同时检查current和栈
bool bSTIteratorHasNext(BSTIterator* obj) {
    return obj->current != NULL || !isStackEmpty(&obj->nodeStack);
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| next() | 平均O(1) | 摊还分析 |
| next() | 最坏O(h) | 需要遍历整条左链 |
| hasNext() | O(1) | 只检查变量 |
| **空间** | **O(h)** | **栈的最大深度** |

**摊还分析详解**：

```
假设BST有n个节点：
- 每个节点入栈1次，出栈1次
- 总操作次数：2n
- 调用next()的次数：n次
- 平均每次next()：2n/n = 2 = O(1)

单次可能O(h)，但n次总计O(n)，摊还O(1)
```

**与普通遍历的对比**：

| 方法 | 空间 | 特点 |
|-----|------|------|
| 完整中序遍历 | O(n) | 一次性存储所有结果 |
| 迭代器 | O(h) | 按需访问，节省空间 |

**测试用例**：

```c
// 用例1：完全二叉树
输入：[7,3,15,9,20]
输出：3,7,9,15,20

// 用例2：左偏树
输入：[3,2,null,1]
输出：1,2,3

// 用例3：右偏树
输入：[1,null,2,null,3]
输出：1,2,3

// 用例4：单节点
输入：[1]
输出：1
```

**关键要点**：
- ✅ 使用栈模拟中序遍历
- ✅ BST中序遍历 = 升序序列
- ✅ 按需推进，不一次性遍历
- ✅ push用++top，pop用top--（先后顺序很重要）
- ✅ 平均O(1)时间，通过摊还分析证明

**记忆技巧**：

```
栈的作用：记住"回家的路"
- 沿着左子树往下走（入栈）
- 访问完左子树后，通过栈"回家"（出栈）
- 然后访问右子树

中序遍历 = 左-根-右 = BST升序
```

### 10.10 二叉树的右视图（199）

**核心思想**：BFS层序遍历 + 记录每层最右节点

**问题描述**：
给定一棵二叉树，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。

**示例**：

```
输入: [1,2,3,null,5,null,4]
    1            从右边看到：
   / \           第1层：1（最右）
  2   3          第2层：3（最右）
   \   \         第3层：4（最右）
    5   4        
输出: [1,3,4]

输入: [1,2,3,4]
    1            从右边看到：
   / \           第1层：1
  2   3          第2层：3
 /               第3层：4
4
输出: [1,3,4]
```

**算法实现**：

```c
int* rightSideView(struct TreeNode* root, int* returnSize) {
    int* result = malloc(sizeof(int) * MAX_SIZE);
    *returnSize = 0;
    
    if (root == NULL) return result;
    
    // 重置队列
    queueSize = 0;
    frontIndex = 0;
    rearIndex = 0;
    
    enqueueNode(root);
    
    while (!isQueueEmpty()) {
        // 关键：记录当前层的节点数量
        int currentLevelSize = queueSize;
        
        // 遍历当前层的所有节点
        for (int i = 0; i < currentLevelSize; i++) {
            struct TreeNode* node = dequeueNode();
            
            // 如果是当前层的最后一个节点（最右边）
            if (i == currentLevelSize - 1) {
                result[(*returnSize)++] = node->val;
            }
            
            // 将子节点入队（先左后右）
            if (node->left) enqueueNode(node->left);
            if (node->right) enqueueNode(node->right);
        }
    }
    
    return result;
}
```

**详细走查**：输入 `[1,2,3,null,5,null,4]`

```
    1
   / \
  2   3
   \   \
    5   4

=== 初始化 ===
queue: [1]
queueSize: 1

=== 第1层 ===
currentLevelSize = 1
遍历1个节点：
  i=0: node=1
       i==0（最后一个）-> 记录1
       入队2, 3
queue: [2, 3]
result: [1]

=== 第2层 ===
currentLevelSize = 2
遍历2个节点：
  i=0: node=2, 入队5
  i=1: node=3
       i==1（最后一个）-> 记录3
       入队4
queue: [5, 4]
result: [1, 3]

=== 第3层 ===
currentLevelSize = 2
遍历2个节点：
  i=0: node=5
  i=1: node=4
       i==1（最后一个）-> 记录4
queue: []
result: [1, 3, 4] ✓
```

**关键技巧**：

**1. 层序遍历的标准模板**
```c
while (!isQueueEmpty()) {
    int currentLevelSize = queueSize;  // 记录当前层大小
    
    for (int i = 0; i < currentLevelSize; i++) {
        // 处理当前层的节点
    }
}
```

**2. 识别每层最右节点**
```c
if (i == currentLevelSize - 1) {
    // 这是当前层的最后一个节点
}
```

**为什么先左后右入队能保证最右？**
```
队列FIFO（先进先出）：
入队顺序：左子节点 -> 右子节点
出队顺序：左子节点 -> 右子节点
所以当前层的最后一个出队的就是最右节点
```

**易错点总结**：

**🐛 Bug #1: 判断队列为空来识别最右节点**
```c
// ❌ 错误：isQueueEmpty()不能识别每层的最右节点
while (!isQueueEmpty()) {
    node = dequeue();
    if (isQueueEmpty()) {  // 只有最后一个节点时才为true
        result[(*returnSize)++] = node->val;
    }
    enqueue(node->left);
    enqueue(node->right);
}

// ✅ 正确：记录每层的节点数量
while (!isQueueEmpty()) {
    int currentLevelSize = queueSize;
    for (int i = 0; i < currentLevelSize; i++) {
        node = dequeue();
        if (i == currentLevelSize - 1) {
            result[(*returnSize)++] = node->val;
        }
        // ...
    }
}
```

**🐛 Bug #2: 全局变量不重置**
```c
// ❌ 错误：不重置，多次调用会越界
int frontIndex = 0;   // 全局变量
int rearIndex = 0;

void bfsLevelOrder(...) {
    enqueueNode(root);  // rearIndex继续递增
    // 多次调用后 rearIndex 会超过数组大小
}

// ✅ 正确：函数开始时重置
void bfsLevelOrder(...) {
    queueSize = 0;
    frontIndex = 0;
    rearIndex = 0;
    // ...
}
```

**🐛 Bug #3: 循环条件错误**
```c
// ❌ 错误：使用队列当前大小
int currentLevelSize = queueSize;
while (queueSize > 0) {  // queueSize在变化！
    node = dequeue();
    // ...
}

// ✅ 正确：使用for循环固定次数
int currentLevelSize = queueSize;
for (int i = 0; i < currentLevelSize; i++) {
    node = dequeue();
    // ...
}
```

**方法对比**：

| 方法 | 时间 | 空间 | 特点 |
|-----|------|------|------|
| BFS（本题） | O(n) | O(w) | w是树的最大宽度 |
| DFS+深度 | O(n) | O(h) | 记录每层第一次访问的节点 |

**DFS实现（右-左顺序）**：

```c
void dfsRightView(struct TreeNode* root, int depth, 
                  int* result, int* returnSize) {
    if (root == NULL) return;
    
    // 如果是第一次访问这一层，记录节点值
    if (depth == *returnSize) {
        result[(*returnSize)++] = root->val;
    }
    
    // 先访问右子树，再访问左子树（关键！）
    dfsRightView(root->right, depth + 1, result, returnSize);
    dfsRightView(root->left, depth + 1, result, returnSize);
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 访问每个节点 | O(n) | BFS遍历 |
| 每个节点的处理 | O(1) | 常数时间 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 队列 | O(w) | w是树的最大宽度 |
| 结果数组 | O(h) | h是树的高度（层数） |
| **总计** | **O(w)** | w最大为n/2 |

**测试用例**：

```c
// 用例1：右偏树
输入：[1,null,3,null,4]
输出：[1,3,4]

// 用例2：左偏树（左节点也要看到）
输入：[1,2]
输出：[1,2]

// 用例3：满二叉树
输入：[1,2,3,4,5,6,7]
输出：[1,3,7]

// 用例4：单节点
输入：[1]
输出：[1]

// 用例5：复杂情况
输入：[1,2,3,null,5,null,4]
输出：[1,3,4]
```

**关键要点**：
- ✅ BFS层序遍历
- ✅ 记录每层的节点数量（currentLevelSize）
- ✅ 识别每层的最后一个节点（i == currentLevelSize - 1）
- ✅ 必须重置全局队列变量
- ✅ 先左后右入队，保证最右节点最后出队

**记忆技巧**：

```
层序遍历模板：
while (队列非空) {
    currentLevelSize = queueSize;  // 固定当前层大小
    for (i = 0 到 currentLevelSize) {
        处理节点
        if (i == currentLevelSize-1) {
            // 这是当前层最后一个节点
        }
    }
}
```

### 10.11 二叉搜索树中第K小的元素（230）

**核心思想**：中序遍历 + 计数 + 剪枝

**问题描述**：
给定一个二叉搜索树的根节点和整数 k，返回树中第 k 小的元素（k 从 1 开始计数）。

**示例**：

```
输入: root = [3,1,4,null,2], k = 1
    3
   / \
  1   4
   \
    2
输出: 1

中序遍历：1, 2, 3, 4（升序）
第1小的元素：1
第2小的元素：2
第3小的元素：3
```

**核心理解**：**BST的中序遍历 = 升序序列**

**算法实现**：

```c
int visitedCount = 0;  // 全局计数器

bool inorderTraversal(struct TreeNode* root, int k, int *kthValue) {
    if (!root) return false;
    
    // 1. 递归遍历左子树
    if (inorderTraversal(root->left, k, kthValue))
        return true;
    
    // 2. 访问当前节点
    visitedCount++;  // 关键：先计数
    if (visitedCount == k) {  // 然后判断
        *kthValue = root->val;
        return true;  // 找到后立即返回（剪枝）
    }
    
    // 3. 递归遍历右子树
    if (inorderTraversal(root->right, k, kthValue))
        return true;
    
    return false;
}

int kthSmallest(struct TreeNode* root, int k) {
    int kthValue = 0;
    visitedCount = 0;  // 重置计数器
    inorderTraversal(root, k, &kthValue);
    return kthValue;
}
```

**详细走查**：输入 `root = [5,3,6,2,4,null,null,1], k = 3`

```
BST:
      5
     / \
    3   6
   / \
  2   4
 /
1

中序遍历顺序：1, 2, 3, 4, 5, 6

=== 查找第3小 (k=3) ===

访问节点1: visitedCount++ = 1, 1==3? false
访问节点2: visitedCount++ = 2, 2==3? false
访问节点3: visitedCount++ = 3, 3==3? true
  -> 返回3，停止遍历 ✓
```

**为什么先计数，后判断？**

```
关键理解：k从1开始，不是从0开始

错误顺序（先判断后计数）：
visitedCount = 0
访问节点1: 检查0==1(false), count++变成1
访问节点2: 检查1==1(true), 返回节点2 ❌
结果：k=1返回第2小的元素（错位！）

正确顺序（先计数后判断）：
visitedCount = 0
访问节点1: count++变成1, 检查1==1(true), 返回节点1 ✓
结果：k=1返回第1小的元素（正确！）
```

**计数逻辑对比**：

| k值 | 含义 | visitedCount | 判断时机 |
|-----|------|-------------|---------|
| 1 | 第1小 | 先++变成1 | 然后检查1==1 |
| 2 | 第2小 | 先++变成2 | 然后检查2==2 |
| 3 | 第3小 | 先++变成3 | 然后检查3==3 |

**剪枝优化**：

```c
// 找到后立即返回，不再遍历后续节点
if (visitedCount == k) {
    *kthValue = root->val;
    return true;  // 剪枝：提前终止
}
```

**为什么能剪枝？**
- BST中序遍历是升序的
- 找到第k小后，后面的都更大
- 不需要继续遍历

**易错点总结**：

**🐛 Bug #1: 先判断后计数（最常见错误）**
```c
// ❌ 错误：计数和判断顺序错误
if (visitedCount == k) {
    *kthValue = root->val;
    return true;
}
visitedCount++;

// ✅ 正确：先计数，后判断
visitedCount++;
if (visitedCount == k) {
    *kthValue = root->val;
    return true;
}
```

**🐛 Bug #2: 没有重置全局变量**
```c
// ❌ 错误：不重置，LeetCode多次调用会出错
int visitedCount = 0;  // 全局变量

int kthSmallest(...) {
    inorderTraversal(...);  // visitedCount继续累加
}

// ✅ 正确：每次调用前重置
int kthSmallest(...) {
    visitedCount = 0;  // 重置
    inorderTraversal(...);
}
```

**🐛 Bug #3: 没有剪枝优化**
```c
// ❌ 效率低：找到后仍继续遍历
void inorderTraversal(...) {
    inorderTraversal(root->left, ...);
    
    visitedCount++;
    if (visitedCount == k) {
        *kthValue = root->val;
    }
    
    inorderTraversal(root->right, ...);  // 仍会执行
}

// ✅ 高效：找到后立即返回
bool inorderTraversal(...) {
    if (inorderTraversal(root->left, ...))
        return true;  // 提前返回
    
    visitedCount++;
    if (visitedCount == k) {
        *kthValue = root->val;
        return true;  // 立即返回
    }
    
    if (inorderTraversal(root->right, ...))
        return true;  // 提前返回
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 平均情况 | O(h+k) | h是树高，k是目标位置 |
| 最好情况 | O(h) | k=1，只访问最左路径 |
| 最坏情况 | O(n) | k=n，遍历整棵树 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 递归栈 | O(h) | 树的高度 |
| 其他变量 | O(1) | 常数空间 |
| **总计** | **O(h)** | h从log(n)到n |

**优化方案**：

**方法1：迭代实现（避免递归）**
```c
int kthSmallest(struct TreeNode* root, int k) {
    struct TreeNode* stack[1000];
    int top = -1;
    int count = 0;
    
    struct TreeNode* current = root;
    
    while (current || top >= 0) {
        // 一直往左走
        while (current) {
            stack[++top] = current;
            current = current->left;
        }
        
        // 访问节点
        current = stack[top--];
        count++;
        if (count == k) {
            return current->val;
        }
        
        // 转向右子树
        current = current->right;
    }
    
    return -1;
}
```

**方法2：Morris遍历（O(1)空间）**
- 不使用栈或递归
- 利用线索二叉树的思想
- 空间复杂度O(1)

**测试用例**：

```c
// 用例1：找第1小
输入：root=[3,1,4,null,2], k=1
输出：1

// 用例2：找第3小
输入：root=[5,3,6,2,4], k=3
输出：4

// 用例3：左偏树
输入：root=[1,null,2], k=2
输出：2

// 用例4：完全二叉树
输入：root=[2,1,3], k=2
输出：2
```

**关键要点**：
- ✅ BST中序遍历 = 升序序列
- ✅ k从1开始计数（不是0）
- ✅ 先计数，后判断（visitedCount++在前）
- ✅ 找到后立即返回（剪枝优化）
- ✅ 必须重置全局计数器

**记忆技巧**：

```
BST + 中序遍历 = 升序
第k小 = 升序序列的第k个

计数顺序：
先++ （当前是第几个）
后判断（是否是第k个）
```

### 10.12 二叉树的最近公共祖先（236）

**核心思想**：后序遍历 + 信息向上传递

**问题描述**：
给定一个二叉树，找到该树中两个指定节点的最近公共祖先（LCA - Lowest Common Ancestor）。

**最近公共祖先定义**：
对于树T的两个节点p、q，最近公共祖先是一个节点x，满足：
- x是p、q的祖先
- x的深度尽可能大
- 一个节点也可以是它自己的祖先

**示例**：

```
         3
       /   \
      5     1
     / \   / \
    6   2 0   8
       / \
      7   4

LCA(5, 1) = 3  (5和1分别在左右子树)
LCA(5, 4) = 5  (5是4的祖先，也是自己的祖先)
LCA(6, 4) = 5  (6和4都在5的子树中)
LCA(7, 4) = 2  (7和4都在2的子树中)
```

**算法实现**：

```c
struct TreeNode* lowestCommonAncestor(struct TreeNode* root, 
                                     struct TreeNode* p, 
                                     struct TreeNode* q) {
    struct TreeNode *leftResult, *rightResult;
    
    // 情况1: 空节点
    if (root == NULL) return NULL;
    
    // 情况2: 找到目标节点
    if (root == p || root == q) return root;
    
    // 步骤1: 在左子树查找
    leftResult = lowestCommonAncestor(root->left, p, q);
    
    // 步骤2: 在右子树查找
    rightResult = lowestCommonAncestor(root->right, p, q);
    
    // 步骤3: 根据结果判断
    if (leftResult != NULL && rightResult != NULL) {
        return root;  // 左右都找到，root是LCA
    }
    
    // 只有一边找到，返回那一边
    return leftResult != NULL ? leftResult : rightResult;
}
```

**返回值含义**：

| 返回值 | 含义 | 说明 |
|--------|------|------|
| NULL | 子树中没有p和q | 两个节点都不在这个子树 |
| p或q | 找到了目标节点 | 可能是p/q本身，也可能是它们的LCA |
| root | root是LCA | 左右子树都找到了 |

**详细走查**：查找 `LCA(5, 1)`

```
树结构：
         3
       /   \
      5     1
     / \   / \
    6   2 0   8

=== 递归过程（后序遍历：左->右->根）===

处理节点6: 返回NULL
处理节点7: 返回NULL
处理节点4: 返回NULL
处理节点2: 左右都NULL，返回NULL

处理节点5: root==5，返回5 ✓

处理节点0: 返回NULL
处理节点8: 返回NULL

处理节点1: root==1，返回1 ✓

处理节点3:
  leftResult = 5  (左子树返回5)
  rightResult = 1 (右子树返回1)
  左右都不为NULL -> 返回3 ✓

答案：3
```

**关键场景分析**：

**场景1：p和q分别在左右子树**
```
     root
     /  \
    p    q

leftResult = p
rightResult = q
-> 返回 root (root就是LCA)
```

**场景2：p是q的祖先**
```
     p
    /
   q

处理p节点：root==p，返回p
处理p的父节点：
  leftResult = p (包含p和q)
  rightResult = NULL
  -> 返回 p (p就是LCA)
```

**场景3：p和q在同一子树**
```
     root
     /
   subtree
   /   \
  p     q

leftResult = LCA(p,q) (子树中的LCA)
rightResult = NULL
-> 返回 leftResult
```

**四种返回情况**：

```c
// 1. 左右都找到 -> root是LCA
if (leftResult != NULL && rightResult != NULL)
    return root;

// 2. 只在左边找到 -> 返回左边结果
if (leftResult != NULL)
    return leftResult;

// 3. 只在右边找到 -> 返回右边结果
if (rightResult != NULL)
    return rightResult;

// 4. 都没找到 -> 返回NULL
return NULL;
```

**简化版本**：
```c
// 可以简化为
if (leftResult && rightResult) return root;
return leftResult ? leftResult : rightResult;
```

**易错点总结**：

**🐛 Bug #1: 只判断值相等，不判断节点相等**
```c
// ❌ 错误：判断值相等（树中可能有重复值）
if (root->val == p->val || root->val == q->val)
    return root;

// ✅ 正确：判断节点指针相等
if (root == p || root == q)
    return root;
```

**🐛 Bug #2: 找到一个就返回，没有继续查找**
```c
// ❌ 错误：找到p就返回，没有继续找q
if (root == p) return root;
leftResult = lowestCommonAncestor(root->left, p, q);
if (leftResult != NULL) return leftResult;  // 提前返回

// ✅ 正确：都查找完再判断
if (root == p || root == q) return root;
leftResult = lowestCommonAncestor(root->left, p, q);
rightResult = lowestCommonAncestor(root->right, p, q);
// 统一在最后判断
```

**🐛 Bug #3: 没有考虑节点是自己祖先的情况**
```c
// 这个算法自动处理了这种情况
// 当root==p时，直接返回p
// 后续判断会正确处理
```

**为什么用后序遍历？**

```
前序（根-左-右）：
- 先处理根节点，无法知道子树的信息
- ❌ 无法判断p、q的位置

中序（左-根-右）：
- 处理根时，只知道左子树信息
- ❌ 无法同时知道左右子树的情况

后序（左-右-根）：
- 处理根时，已知左右子树的信息
- ✅ 可以根据左右结果判断LCA
```

**递归返回值的妙用**：

```
返回值有两重含义：
1. 在该子树中是否找到p或q
2. 如果找到，返回的可能是：
   - p或q本身
   - p和q的LCA

这种"双重含义"巧妙地解决了问题
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 访问每个节点 | O(n) | 最坏情况遍历所有节点 |
| 每个节点的处理 | O(1) | 常数时间判断 |
| **总计** | **O(n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 递归栈 | O(h) | 树的高度 |
| 其他变量 | O(1) | 常数空间 |
| **总计** | **O(h)** | h从log(n)到n |

**测试用例**：

```c
// 用例1：p和q在不同子树
输入：root=[3,5,1,6,2,0,8], p=5, q=1
输出：3

// 用例2：p是q的祖先
输入：root=[3,5,1,6,2,0,8], p=5, q=4
输出：5

// 用例3：p和q在同一子树
输入：root=[3,5,1,6,2,0,8], p=6, q=4
输出：5

// 用例4：两个节点是叶子节点
输入：root=[3,5,1,6,2,0,8], p=7, q=4
输出：2

// 用例5：根节点是LCA
输入：root=[1,2,3], p=2, q=3
输出：1
```

**相关题目**：

**LeetCode 235: 二叉搜索树的最近公共祖先**
- BST有序性质，可以通过比较值来判断
- 不需要遍历整棵树，O(h)时间

```c
struct TreeNode* lowestCommonAncestorBST(struct TreeNode* root, 
                                        struct TreeNode* p, 
                                        struct TreeNode* q) {
    // 利用BST性质：左<根<右
    while (root) {
        if (p->val < root->val && q->val < root->val) {
            root = root->left;  // p、q都在左子树
        } else if (p->val > root->val && q->val > root->val) {
            root = root->right; // p、q都在右子树
        } else {
            return root;  // 分叉点就是LCA
        }
    }
    return NULL;
}
```

**关键要点**：
- ✅ 使用后序遍历（左-右-根）
- ✅ 递归返回值表示"是否找到目标节点"
- ✅ 左右都找到 -> 当前节点是LCA
- ✅ 只有一边找到 -> 返回那边的结果
- ✅ 节点可以是自己的祖先
- ✅ 信息从下往上传递

**记忆技巧**：

```
后序遍历口诀：
左右根（Left-Right-Root）
先知子树，后判自己

LCA判断：
两边都有 -> 我是祖先
一边有 -> 传上去
都没有 -> 返回空
```

**应用场景**：
- 树的最近公共祖先问题
- 信息向上传递的递归模式
- 后序遍历的应用
- 节点关系判断

---

## 12. 数据结构设计

### 12.1 LRU Cache（146）

**核心组合**：双向链表 + 哈希表

```c
typedef struct CacheNode {
    int key, value;
    struct CacheNode *prev, *next;
    UT_hash_handle hh;  // 哈希表句柄
} CacheNode;

typedef struct {
    CacheNode *head;  // 最久未使用（LRU）
    CacheNode *tail;  // 最近使用（MRU）
    int capacity, size;
} LRUCache;
```

**核心操作**：

```c
// Get: 查找并移到尾部
int get(LRUCache* cache, int key) {
    HASH_FIND_INT(hashTable, &key, node);
    if (node) {
        moveToTail(cache, node);  // 标记为最近使用
        return node->value;
    }
    return -1;
}

// Put: 更新或插入
void put(LRUCache* cache, int key, int value) {
    HASH_FIND_INT(hashTable, &key, node);
    if (node) {
        // 已存在：更新值，移到尾部
        node->value = value;
        moveToTail(cache, node);
    } else {
        // 不存在：创建新节点
        if (cache->size == cache->capacity) {
            // 容量满：删除头节点（LRU）
            removeHead(cache);
        }
        // 添加新节点到尾部
        addToTail(cache, newNode);
    }
}
```

**关键辅助函数**：

1. **moveToTail**：将节点移到尾部（最近使用）
2. **addToTail**：添加节点到尾部
3. **removeHead**：删除头节点（最久未使用）

**为什么需要双向链表？**
```
单向链表：删除节点需要找前驱节点 O(n) ❌
双向链表：通过prev直接访问前驱 O(1) ✓
```

**关键点**：
- ✅ head=LRU（最久），tail=MRU（最近）
- ✅ get/put都要moveToTail
- ✅ 容量满时removeHead

**易错点**：
- ❌ 第一次插入忘记检查tail=NULL
- ❌ capacity=1时，删除head后tail变野指针
- ❌ 全局hashTable在多个实例间共享（LeetCode可能测试多实例）

### 12.2 Insert Delete GetRandom O(1)（380）

**核心组合**：动态数组 + 哈希表

```c
typedef struct {
    int* nums;      // 动态数组：支持随机访问
    HashNode** map; // 哈希表：val -> index映射
    int size;
    int capacity;
} RandomizedSet;
```

**关键技巧**：删除时交换到末尾

```c
// 删除操作：O(1)
int index = mapGet(val);
int lastElement = nums[size-1];

nums[index] = lastElement;      // 交换
mapPut(lastElement, index);     // 更新哈希表
mapRemove(val);                 // 删除
size--;                         // 缩小
```

**为什么？**
- 数组中间删除是O(n)
- 交换到末尾再删除是O(1)

### 14.2 Copy List with Random Pointer（138）

**交织法**（O(1)空间）：

```
步骤1：A → B → C 变成 A → A' → B → B' → C → C'
步骤2：设置random：A'.random = A.random.next
步骤3：分离两个链表
```

**关键洞察**：
- 新节点紧跟在原节点后面
- 通过 `original.next` 访问对应的新节点
- 不需要哈希表！

### 11.6 Reverse Linked List II（92）

**核心思想**：一次遍历，记录关键节点 + 反转区间

```c
while (current != NULL) {
    nextNode = current->next;  // 保存next
    position++;
    
    // 记录4个关键节点
    if (position == left - 1) beforeLeft = current;
    if (position == right + 1) afterRight = current;
    if (position == left) leftNode = current;
    if (position == right) rightNode = current;
    
    // 反转区间内的指针
    if (position >= left && position <= right) {
        current->next = prev;
    }
    
    prev = current;
    current = nextNode;
}

// 重新连接
beforeLeft->next = rightNode;  // before -> 反转后的头
leftNode->next = afterRight;    // 反转后的尾 -> after
return (left == 1) ? rightNode : head;
```

**关键理解**：
- ✅ leftNode反转后变成**尾部**
- ✅ rightNode反转后变成**头部**
- ✅ 保存next避免断链

**易错点**：
- ❌ 连接错误：`right_ptr->next = right_end` → 应该是 `left_ptr->next`
- ❌ 返回值：left=1时头节点变了，需要返回 `rightNode`

---

## 9. 栈的应用

### 9.1 Simplify Path（71）

**核心思想**：用栈处理Unix路径

```c
char **dirStack;  // 栈（存储目录名）
int stackSize = 0;

// 用strtok分割路径
token = strtok(pathCopy, "/");
while (token) {
    if (strcmp(token, "..") == 0) {
        // 返回上级：弹栈
        if (stackSize > 0) stackSize--;
    } else if (strcmp(token, ".") != 0 && strlen(token) > 0) {
        // 有效目录名：入栈
        dirStack[stackSize++] = token;
    }
    // "." 跳过
    token = strtok(NULL, "/");
}

// 从栈底到栈顶拼接路径
result[0] = '/';
for (i = 0; i < stackSize; i++) {
    // 拼接目录名和 '/'
}
```

**关键点**：
- ✅ `strtok` 分割字符串
- ✅ 栈处理".."（返回上级）
- ✅ "..."是有效目录名（不是特殊符号）

**易错点**：
- ❌ `token = strtok(NULL, "/")` 忘记赋值 → 死循环
- ❌ `*top++` → 应该是 `(*top)++`（运算符优先级）

### 9.2 Evaluate Reverse Polish Notation（150）

**核心思想**：用栈计算后缀表达式

```c
int stack[MAX_SIZE];
int top = -1;

for (i = 0; i < tokensSize; i++) {
    if (isOperator(tokens[i])) {
        // 操作符：弹出两个操作数
        int right = stack[top--];
        int left = stack[top--];
        stack[++top] = operate(left, right, tokens[i]);
    } else {
        // 数字：入栈
        stack[++top] = atoi(tokens[i]);
    }
}

return stack[top];
```

**关键点**：
- ✅ 后缀表达式：操作数在前，操作符在后
- ✅ 遇到操作符就弹出两个数计算
- ✅ 结果入栈继续使用

**易错点**：
- ❌ `isOperator` 只检查第一个字符 → 负数"-11"会误判
- ✅ 必须检查长度：`strlen(op) == 1 && (*op == '-' || ...)`
- ❌ `stack[top+1] = result` → 应该 `stack[++top] = result`

**区分负数和减号**：
```c
"-"   → strlen=1, 是操作符 ✓
"-11" → strlen=3, 是负数 ✓
```

### 9.3 Min Stack（155）

**核心思想**：双栈同步，minStack记录每个状态的最小值

```c
typedef struct {
   int *stack;      // 主栈
   int *minStack;   // 最小值栈（同步）
   int top;         // 统一的栈顶索引
} MinStack;

// Push操作
void push(MinStack* obj, int val) {
    obj->top++;
    obj->stack[obj->top] = val;
    
    // 更新最小值栈
    if (obj->top == 0) {
        obj->minStack[0] = val;  // 第一个元素
    } else {
        int prevMin = obj->minStack[obj->top - 1];
        obj->minStack[obj->top] = (val < prevMin) ? val : prevMin;
    }
}

// GetMin操作：O(1)
int getMin(MinStack* obj) {
    return obj->minStack[obj->top];  // 直接返回
}
```

**关键点**：
- ✅ 两个栈同步（top相同）
- ✅ minStack[i] 存储"前i个元素的最小值"
- ✅ pop后自动恢复到上一个最小值

**易错点**：
- ❌ 维护单独的 `min_value` 字段 → pop后忘记更新，导致错误
- ✅ 直接用 `minStack[top]` → 自动正确

**为什么需要minStack？**
```
只用一个min变量：
push(3), push(2), push(5)
min = 2
pop() → 栈=[3,2]，但min如何恢复到3？需要遍历 O(n) ❌

用minStack：
push(3): minStack=[3]
push(2): minStack=[3,2]  (前2个元素的最小值是2)
push(5): minStack=[3,2,2]  (前3个元素的最小值还是2)
pop(): minStack=[3,2]，getMin()=2 ✓
pop(): minStack=[3]，getMin()=3 ✓
```

---

## 13. 数组技巧

### 13.1 Product Except Self（238）

**核心技巧**：左右累积乘积

```c
// 第一遍：左侧乘积
answer[i] = left;
left *= nums[i];

// 第二遍：右侧乘积
answer[i] *= right;
right *= nums[i];
```

**易错点**：
- ❌ `left *= answer[i]` → 应该是 `left *= nums[i]`
- ❌ `right *= answer[i]` → 应该是 `right *= nums[i]`

### 13.2 H-Index（274）

**排序法**（最优）：

```c
qsort(citations, n, sizeof(int), compare_desc);  // 降序

for (int i = 0; i < n; i++) {
    if (citations[i] >= i + 1) {
        h = i + 1;
    } else {
        break;
    }
}
```

**理解**：
- 排序后 `citations[i]` 是第 `i+1` 大的引用数
- 如果 `citations[i] >= i+1`，说明至少有 `i+1` 篇论文被引用 >= `i+1` 次

---

## 11. 图论/DFS/BFS

### 11.1 被围绕的区域（130）

**核心思想**：反向思维 + 从边界DFS

**问题描述**：
给定一个二维矩阵，包含 'X' 和 'O'，找到所有被 'X' 围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充。

**被围绕的定义**：
- 不与边界连通的 'O'
- 从边界的 'O' 开始，通过上下左右相邻的 'O' 能到达的都不算被围绕

**示例**：

```
输入:
X X X X
X O O X
X X O X
X O X X

输出:
X X X X
X X X X
X X X X
X O X X  ← 最后一行的O与边界连通，保留
```

**核心技巧：反向思维**

```
❌ 正向思考（困难）：
   找被围绕的O（需要判断是否被X完全包围）

✅ 反向思考（简单）：
   找不被围绕的O（与边界连通的O）
   剩下的O就是被围绕的
```

**算法实现**：

```c
int **visited = NULL;
int **borderConnected = NULL;

void dfsMarkBorderConnected(char** board, ..., int row, int col) {
    // 边界检查
    if (row < 0 || row >= numRows || col < 0 || col >= numCols ||
        board[row][col] == 'X' || visited[row][col] == 1) {
        return;
    }
    
    // 标记为与边界连通
    visited[row][col] = 1;
    borderConnected[row][col] = 1;
    
    // 递归四个方向
    dfsMarkBorderConnected(board, ..., row-1, col);
    dfsMarkBorderConnected(board, ..., row+1, col);
    dfsMarkBorderConnected(board, ..., row, col-1);
    dfsMarkBorderConnected(board, ..., row, col+1);
}

void solve(char** board, int boardSize, int* boardColSize) {
    // 1. 从四条边界开始DFS
    for (j = 0; j < numCols; j++) {
        dfsMarkBorderConnected(board, ..., 0, j);          // 第一行
        dfsMarkBorderConnected(board, ..., numRows-1, j);  // 最后一行
    }
    for (i = 1; i < numRows-1; i++) {
        dfsMarkBorderConnected(board, ..., i, 0);          // 第一列
        dfsMarkBorderConnected(board, ..., i, numCols-1);  // 最后一列
    }
    
    // 2. 将未标记的O改为X
    for (i = 0; i < numRows; i++) {
        for (j = 0; j < numCols; j++) {
            if (board[i][j] == 'O' && borderConnected[i][j] == 0) {
                board[i][j] = 'X';
            }
        }
    }
}
```

**详细走查**：

```
输入:
X X X X
X O O X
X X O X
X O X X

=== 步骤1: 从边界DFS ===

边界O：只有(3,1)
从(3,1)开始DFS，标记所有连通的O
但(3,1)四周都是X或边界，无法扩展

borderConnected:
0 0 0 0
0 0 0 0
0 0 0 0
0 1 0 0

=== 步骤2: 将未标记的O改为X ===

(1,1): O且未标记 -> 改为X
(1,2): O且未标记 -> 改为X
(2,2): O且未标记 -> 改为X
(3,1): O且已标记 -> 保留O

输出:
X X X X
X X X X
X X X X
X O X X  ✓
```

**易错点总结**：

**🐛 Bug #1: 使用 == 而不是 =**
```c
// ❌ 错误：比较运算符，不会修改board
board[i][j] == 'X';

// ✅ 正确：赋值运算符
board[i][j] = 'X';
```

**🐛 Bug #2: 只检查四条边的角**
```c
// ❌ 错误：漏掉了边界上的其他位置
dfsMarkBorderConnected(board, ..., 0, 0);      // 左上角
dfsMarkBorderConnected(board, ..., 0, n-1);    // 右上角
// ...

// ✅ 正确：遍历整条边
for (j = 0; j < numCols; j++) {
    dfsMarkBorderConnected(board, ..., 0, j);  // 整个第一行
}
```

**🐛 Bug #3: 忘记标记内部的连通区域**
```c
// 从边界(3,1)的O开始DFS
// 如果(3,1)与(2,2)连通，也应该标记(2,2)
// 这就是DFS的作用：标记整个连通区域
```

**优化方案**：

**方法1：合并visited和borderConnected数组**
```c
// 只用一个数组
int **borderConnected = NULL;

void dfsMarkBorderConnected(...) {
    if (...  || borderConnected[row][col] == 1) {
        return;
    }
    borderConnected[row][col] = 1;
    // ...
}
```

**方法2：原地标记（不使用额外数组）**
```c
// 用特殊字符标记
void dfsMarkBorderConnected(...) {
    if (board[row][col] != 'O') return;
    
    board[row][col] = '#';  // 临时标记
    // DFS四个方向
}

// 最后恢复
for (i = 0; i < numRows; i++) {
    for (j = 0; j < numCols; j++) {
        if (board[i][j] == 'O') board[i][j] = 'X';  // 未标记的O
        if (board[i][j] == '#') board[i][j] = 'O';  // 已标记的恢复
    }
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| DFS标记 | O(m×n) | 每个格子最多访问一次 |
| 遍历修改 | O(m×n) | 遍历整个棋盘 |
| **总计** | **O(m×n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| visited数组 | O(m×n) | 记录访问状态 |
| borderConnected数组 | O(m×n) | 标记边界连通 |
| 递归栈 | O(m×n) | 最坏情况 |
| **总计** | **O(m×n)** | 可优化到O(1) |

**测试用例**：

```c
// 用例1：中间的O被围绕
输入：[["X","X","X","X"],
      ["X","O","O","X"],
      ["X","X","O","X"],
      ["X","O","X","X"]]
输出：[["X","X","X","X"],
      ["X","X","X","X"],
      ["X","X","X","X"],
      ["X","O","X","X"]]

// 用例2：全是X
输入：[["X"]]
输出：[["X"]]

// 用例3：边界都是O
输入：[["O","O","O"],
      ["O","X","O"],
      ["O","O","O"]]
输出：[["O","O","O"],
      ["O","X","O"],
      ["O","O","O"]]
```

**关键要点**：
- ✅ 反向思维：找不被围绕的，剩下的就是被围绕的
- ✅ 从边界开始DFS
- ✅ 标记与边界连通的O
- ✅ 注意 = 和 == 的区别
- ✅ 记得释放内存

**记忆技巧**：

```
被围绕的区域 = 反向思维

从边界出发 -> DFS标记 -> 剩余未标记的就是被围绕的
```

### 11.2 克隆图（133）

**核心思想**：DFS + 哈希表

**问题描述**：
给定无向连通图中一个节点的引用，返回该图的深拷贝（克隆）。

**图节点定义**：
```c
struct Node {
    int val;
    int numNeighbors;
    struct Node** neighbors;  // 邻居数组
};
```

**示例**：

```
输入: adjList = [[2,4],[1,3],[2,4],[1,3]]

图结构：
  1 --- 2
  |     |
  4 --- 3

节点1的邻居：[2, 4]
节点2的邻居：[1, 3]
节点3的邻居：[2, 4]
节点4的邻居：[1, 3]
```

**核心挑战**：图有环！

```
1 -> 2 -> 1（环）

如果简单递归：
克隆1 -> 克隆1的邻居2 -> 克隆2的邻居1 -> 克隆1的邻居2 -> ...
无限递归！❌
```

**算法实现**：

```c
/* 哈希表：原节点 -> 克隆节点的映射 */
CloneHashNode *cloneHashMap = NULL;

struct Node *dfsClone(struct Node *node) {
    struct Node *clonedNode;
    
    // 步骤1: 检查是否已克隆
    clonedNode = findClonedNode(node);
    if (clonedNode) {
        return clonedNode;  // 已克隆，直接返回
    }
    
    // 步骤2: 创建克隆节点
    clonedNode = malloc(sizeof(struct Node));
    clonedNode->val = node->val;
    clonedNode->numNeighbors = node->numNeighbors;
    clonedNode->neighbors = malloc(sizeof(struct Node*) * node->numNeighbors);
    
    // 步骤3: 立即加入哈希表（关键！）
    addCloneMapping(node, clonedNode);
    
    // 步骤4: 递归克隆所有邻居
    for (int i = 0; i < node->numNeighbors; i++) {
        clonedNode->neighbors[i] = dfsClone(node->neighbors[i]);
    }
    
    return clonedNode;
}

struct Node *cloneGraph(struct Node *s) {
    if (s == NULL) return NULL;
    
    initCloneHashMap();
    struct Node *clonedNode = dfsClone(s);
    freeCloneHashMap();
    
    return clonedNode;
}
```

**为什么要立即加入哈希表？**

```
图结构：1 <-> 2（互相连接）

正确顺序（先加入哈希表）：
1. 克隆节点1
2. 加入哈希表：1 -> clone1
3. 克隆节点1的邻居（节点2）
   a. 克隆节点2
   b. 加入哈希表：2 -> clone2
   c. 克隆节点2的邻居（节点1）
   d. 查哈希表，找到clone1
   e. 直接返回clone1 ✓（避免无限递归）

错误顺序（后加入哈希表）：
1. 克隆节点1
2. 克隆节点1的邻居（节点2）
   a. 克隆节点2
   b. 克隆节点2的邻居（节点1）
   c. 查哈希表，找不到节点1
   d. 再次克隆节点1
   e. 无限递归！❌
```

**哈希表的键值对**：

| 原节点地址 | 克隆节点地址 | 说明 |
|-----------|------------|------|
| 0x1000 (node1) | 0x2000 (clone1) | 节点1的克隆 |
| 0x1100 (node2) | 0x2100 (clone2) | 节点2的克隆 |
| 0x1200 (node3) | 0x2200 (clone3) | 节点3的克隆 |

**HASH_ADD/HASH_FIND 参数详解**：

**关键问题：为什么传 `&node` 而不是 `node`？**

```c
// node 是 struct Node* 类型（指针）
// 我们想用指针的值（地址）作为键

HASH_ADD(hh, cloneHashMap, originalNode, sizeof(struct Node*), hashNode);
//                         ↑字段名      ↑指针的大小

HASH_FIND(hh, cloneHashMap, &originalNode, sizeof(struct Node*), hashNode);
//                          ↑要传地址     ↑指针的大小
```

**为什么？**
- `HASH_FIND` 的第3个参数是 `keyptr`（指向键的指针）
- 键的类型是 `struct Node*`（指针）
- 所以要传 `&originalNode`（指向指针的指针，类型是 `struct Node**`）

**类比**：
```c
// 如果键是 int
int key = 42;
HASH_FIND_INT(head, &key, found);  // 传 &key

// 如果键是 struct Node*
struct Node* key = node;
HASH_FIND(hh, head, &key, sizeof(struct Node*), found);  // 传 &key
```

**易错点总结**：

**🐛 Bug #1: 哈希表只存原节点，不存克隆节点**
```c
// ❌ 错误：只存原节点
typedef struct {
    struct Node *node;  // 只有一个字段
    UT_hash_handle hh;
} HashNode;

// ✅ 正确：存储映射关系
typedef struct {
    struct Node *originalNode;  // 原节点（key）
    struct Node *clonedNode;    // 克隆节点（value）
    UT_hash_handle hh;
} CloneHashNode;
```

**🐛 Bug #2: 在克隆邻居后才加入哈希表**
```c
// ❌ 错误：先克隆邻居，后加入哈希表
clonedNode = malloc(...);
for (i = 0; i < numNeighbors; i++) {
    clonedNode->neighbors[i] = dfsClone(node->neighbors[i]);
}
addCloneMapping(node, clonedNode);  // 太晚了！

// ✅ 正确：先加入哈希表，后克隆邻居
clonedNode = malloc(...);
addCloneMapping(node, clonedNode);  // 立即加入
for (i = 0; i < numNeighbors; i++) {
    clonedNode->neighbors[i] = dfsClone(node->neighbors[i]);
}
```

**🐛 Bug #3: HASH_FIND参数错误**
```c
// ❌ 错误：传 node（指针本身）
HASH_FIND(hh, cloneHashMap, node, sizeof(struct Node*), hashNode);

// ✅ 正确：传 &node（指向指针的指针）
HASH_FIND(hh, cloneHashMap, &node, sizeof(struct Node*), hashNode);
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| DFS遍历 | O(N) | N个节点 |
| 克隆邻居 | O(E) | E条边 |
| **总计** | **O(N+E)** | 节点数+边数 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 哈希表 | O(N) | 存储N个节点的映射 |
| 递归栈 | O(N) | 最坏情况 |
| 克隆的图 | O(N+E) | N个节点+E条边 |
| **总计** | **O(N+E)** | 线性空间 |

**测试用例**：

```c
// 用例1：环形图
输入：[[2,4],[1,3],[2,4],[1,3]]
输出：克隆的图（不是原图的引用）

// 用例2：单节点
输入：[[]]
输出：[[]]

// 用例3：空图
输入：[]
输出：[]

// 用例4：线性图
输入：[[2],[3],[]]
输出：[[2],[3],[]]
```

**关键要点**：
- ✅ 哈希表存储原节点→克隆节点的映射
- ✅ 先创建克隆节点并加入哈希表，再克隆邻居
- ✅ HASH_FIND传 `&node`（指向指针的指针）
- ✅ 键的大小是 `sizeof(struct Node*)`（指针的大小）
- ✅ 处理环形图（查哈希表避免重复克隆）

**记忆技巧**：

```
克隆图 = DFS + 哈希表

哈希表作用：
1. 记录已克隆的节点
2. 映射原节点→克隆节点
3. 防止环导致无限递归

关键顺序：
创建 -> 加入哈希表 -> 克隆邻居
```

### 11.3 岛屿数量（200）

**核心思想**：DFS标记连通分量

**问题描述**：
给定一个由 '1'（陆地）和 '0'（水）组成的二维网格，计算岛屿的数量。岛屿由相邻的陆地连接而成（上下左右），并且四面被水包围。

**示例**：

```
输入:
[["1","1","1","1","0"],
 ["1","1","0","1","0"],
 ["1","1","0","0","0"],
 ["0","0","0","0","0"]]
 
网格可视化：
1 1 1 1 0
1 1 0 1 0
1 1 0 0 0
0 0 0 0 0

输出: 1（这是一个连通的岛屿）
```

**算法实现**：

```c
int **visited = NULL;  // 全局visited数组

void dfsMarkIsland(char **grid, int gridSize, int *gridColSize, 
                   int row, int col) {
    int numRows = gridSize;
    int numCols = gridColSize[row];
    
    // 边界检查和条件判断
    if (row < 0 || row >= numRows || 
        col < 0 || col >= numCols || 
        grid[row][col] == '0' || visited[row][col] == 1) {
        return;
    }
    
    // 标记当前位置为已访问
    visited[row][col] = 1;
    
    // 递归访问四个方向
    dfsMarkIsland(grid, gridSize, gridColSize, row - 1, col);  // 上
    dfsMarkIsland(grid, gridSize, gridColSize, row + 1, col);  // 下
    dfsMarkIsland(grid, gridSize, gridColSize, row, col - 1);  // 左
    dfsMarkIsland(grid, gridSize, gridColSize, row, col + 1);  // 右
}

int numIslands(char** grid, int gridSize, int* gridColSize) {
    int islandCount = 0;
    
    // 分配visited数组
    visited = malloc(...);
    
    // 遍历整个网格
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            // 发现未访问的陆地
            if (grid[i][j] == '1' && visited[i][j] == 0) {
                islandCount++;  // 发现新岛屿
                dfsMarkIsland(grid, gridSize, gridColSize, i, j);
            }
        }
    }
    
    return islandCount;
}
```

**算法步骤**：
1. 遍历整个网格
2. 遇到未访问的陆地'1'，岛屿数+1
3. 从该陆地开始DFS，标记整个连通区域
4. 继续遍历，已标记的陆地不再计数

**DFS的四个方向**：
```
      上(row-1, col)
         ↑
左 ← (row,col) → 右
(row,col-1)    (row,col+1)
         ↓
    下(row+1, col)
```

**关键变量说明**：

| 变量名 | 作用 | 说明 |
|--------|------|------|
| `visited` | 访问标记数组 | 防止重复访问 |
| `islandCount` | 岛屿数量 | DFS调用次数 |
| `numRows` | 行数 | gridSize |
| `numCols` | 列数 | gridColSize[i] |

**易错点总结**：

**🐛 Bug #1: 提前标记**
```c
// ❌ 错误：在检查条件前标记
visited[row][col] = 1;
if (row >= 0 && row < numRows && ...) {
    // 处理
}

// ✅ 正确：先检查条件，再标记
if (row >= 0 && row < numRows && ...) {
    visited[row][col] = 1;
    // 处理
}
```

**🐛 Bug #2: colSize固定使用**
```c
// ❌ 错误：所有行用同一个colSize
colSize = gridColSize[0];
for (i = 0; i < numRows; i++) {
    for (j = 0; j < colSize; j++) {  // 错误
        ...
    }
}

// ✅ 正确：每行重新获取列数
for (i = 0; i < numRows; i++) {
    colSize = gridColSize[i];  // 每行重新获取
    for (j = 0; j < colSize; j++) {
        ...
    }
}
```

**🐛 Bug #3: 忘记释放visited数组**
```c
// ❌ 错误：忘记释放，内存泄漏
visited = malloc(...);
// 使用visited
return islandCount;

// ✅ 正确：释放内存
visited = malloc(...);
// 使用visited
for (i = 0; i < numRows; i++) {
    free(visited[i]);
}
free(visited);
return islandCount;
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 遍历网格 | O(m×n) | m行n列 |
| DFS访问 | O(m×n) | 每个格子最多访问一次 |
| **总计** | **O(m×n)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| visited数组 | O(m×n) | 记录访问状态 |
| 递归栈 | O(m×n) | 最坏情况 |
| **总计** | **O(m×n)** | 线性空间 |

**优化方案**：

**方法1：原地标记（节省空间）**
```c
// 不使用visited数组，直接修改grid
void dfsMarkIsland(char **grid, ...) {
    if (grid[row][col] == '0') return;
    
    grid[row][col] = '0';  // 标记为水
    // 递归四个方向
}
```

**方法2：BFS实现**
```c
// 使用队列代替递归
int numIslands(char** grid, ...) {
    int islandCount = 0;
    Queue queue;
    
    for (i = 0; i < numRows; i++) {
        for (j = 0; j < numCols; j++) {
            if (grid[i][j] == '1') {
                islandCount++;
                // BFS标记整个岛屿
                enqueue({i, j});
                while (!isEmpty()) {
                    (r, c) = dequeue();
                    // 标记并入队四个方向
                }
            }
        }
    }
    
    return islandCount;
}
```

**测试用例**：

```c
// 用例1：单个岛屿
输入：[["1","1","1"],
      ["0","1","0"],
      ["1","1","1"]]
输出：1

// 用例2：多个岛屿
输入：[["1","1","0","0","0"],
      ["1","1","0","0","0"],
      ["0","0","1","0","0"],
      ["0","0","0","1","1"]]
输出：3

// 用例3：全是水
输入：[["0","0","0"],
      ["0","0","0"]]
输出：0

// 用例4：全是陆地
输入：[["1","1"],
      ["1","1"]]
输出：1
```

**关键要点**：
- ✅ DFS标记整个连通区域
- ✅ 岛屿数 = DFS调用次数
- ✅ visited数组防止重复访问
- ✅ 每行重新获取列数（安全）
- ✅ 记得释放visited数组

**记忆技巧**：

```
岛屿数量 = 连通分量数量

DFS流程：
发现陆地 -> 计数+1 -> DFS标记整片 -> 继续寻找
```

**应用场景**：
- 连通分量问题
- 图的遍历
- 矩阵中的区域问题
- 泛洪填充算法

### 11.4 课程表（207）

**核心思想**：拓扑排序 + 三色标记法检测环

**问题描述**：
你需要选n门课程，在选某些课程之前需要先修课程。判断是否可能完成所有课程的学习。

**示例**：

```
输入: numCourses = 2, prerequisites = [[1,0]]
输出: true
解释: 先学课程0，再学课程1

输入: numCourses = 2, prerequisites = [[1,0],[0,1]]
输出: false
解释: 0→1→0 形成环，无法完成
```

**核心理解**：拓扑排序 = 环检测

```
有向无环图（DAG） → 可以拓扑排序 → 可以完成课程
有向有环图 → 无法拓扑排序 → 无法完成课程

本题本质：检测有向图中是否有环
```

**三色标记法（DFS环检测）**：

```
白色（UNVISITED）：未访问
灰色（VISITING）：正在访问（在当前DFS路径上）
黑色（VISITED）：已完成访问

环检测规则：
如果访问到灰色节点 → 回到当前路径 → 有环！
```

**算法实现**：

```c
typedef enum {
    UNVISITED,  // 白色
    VISITING,   // 灰色
    VISITED,    // 黑色
} VisitStatus;

bool detectCycle(CourseGraph *graph, int courseId) {
    VertexNode *vertex = findCourseNode(graph, courseId);
    
    // 情况1: 遇到灰色节点，有环
    if (vertex->status == VISITING)
        return false;
    
    // 情况2: 已访问过（黑色），无环
    if (vertex->status == VISITED)
        return true;
    
    // 情况3: 标记为灰色（正在访问）
    vertex->status = VISITING;
    
    // 情况4: 递归检查所有邻居
    EdgeNode *edge = vertex->edgeList;
    while (edge) {
        if (!detectCycle(graph, edge->adjacentCourse))
            return false;  // 发现环
        edge = edge->next;
    }
    
    // 情况5: 标记为黑色（完成访问）
    vertex->status = VISITED;
    return true;
}

bool canFinish(int numCourses, int** prerequisites, ...) {
    // 1. 构建课程依赖图
    for (i = 0; i < prerequisitesSize; i++) {
        addPrerequisite(graph, prerequisites[i][1], prerequisites[i][0]);
    }
    
    // 2. 对每个未访问的课程DFS检测环
    for (i = 0; i < graph->numVertices; i++) {
        if (vertex->status == UNVISITED) {
            if (!detectCycle(graph, vertex->courseId))
                return false;  // 有环
        }
    }
    
    return true;  // 无环
}
```

**详细走查**：

```
示例：0→1→2（链式图）

初始状态：所有节点白色（UNVISITED）

DFS(0):
  0: 白色 → 灰色（VISITING）
  检查邻居1:
    DFS(1):
      1: 白色 → 灰色（VISITING）
      检查邻居2:
        DFS(2):
          2: 白色 → 灰色（VISITING）
          无邻居
          2: 灰色 → 黑色（VISITED）
        返回true
      1: 灰色 → 黑色（VISITED）
    返回true
  0: 灰色 → 黑色（VISITED）
返回true

最终：所有节点黑色，无VISITING残留
```

**为什么VISITING不会残留？**

```
关键：DFS返回前，节点从灰色变黑色

vertex->status = VISITING;  // 进入时：灰色
// 递归访问邻居...
vertex->status = VISITED;   // 离开时：黑色

唯一不变黑的情况：发现环提前返回
但此时整个函数返回false，不会继续处理其他节点
```

**为什么不需要重置状态？**

```
场景：图有两个连通分量
  0→1    2→3

第1次DFS(0):
  0,1都变成黑色（VISITED）

第2次遍历到节点1:
  status == VISITED（第175-176行）
  直接return true，跳过

第3次DFS(2):
  2,3都变成黑色

时间复杂度：O(V+E)
- 每个节点最多访问一次
- 每条边最多访问一次
```

**易错点总结**：

**🐛 Bug #1: 每次循环都重置状态**
```c
// ❌ 错误：O(V²)复杂度
for (i = 0; i < numVertices; i++) {
    resetStatus(graph);  // 每次O(V)
    dfs(graph, i);       // 每次O(V+E)
}

// ✅ 正确：O(V+E)复杂度
for (i = 0; i < numVertices; i++) {
    if (vertex->status == UNVISITED) {
        if (!detectCycle(graph, i))
            return false;
    }
}
```

**🐛 Bug #2: 条件判断反了**
```c
// ❌ 错误：!=
if (vertex->status != UNVISITED) {
    dfs(...);
}

// ✅ 正确：==
if (vertex->status == UNVISITED) {
    dfs(...);
}
```

**🐛 Bug #3: result初始化错误**
```c
// ❌ 错误
bool result = false;

// ✅ 正确：默认为true，只有发现环才false
bool result = true;
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 构建图 | O(E) | E个先修关系 |
| DFS环检测 | O(V+E) | 每个节点和边访问一次 |
| **总计** | **O(V+E)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 图存储 | O(V+E) | 顶点和边 |
| 递归栈 | O(V) | DFS深度 |
| **总计** | **O(V+E)** | 线性空间 |

**测试用例**：

```c
// 用例1：无环
输入：numCourses=2, prerequisites=[[1,0]]
输出：true

// 用例2：有环
输入：numCourses=2, prerequisites=[[1,0],[0,1]]
输出：false

// 用例3：链式
输入：numCourses=4, prerequisites=[[1,0],[2,1],[3,2]]
输出：true

// 用例4：无先修关系
输入：numCourses=1, prerequisites=[]
输出：true
```

**关键要点**：
- ✅ 三色标记法检测环
- ✅ 灰色节点表示在当前DFS路径上
- ✅ 遇到灰色节点说明有环
- ✅ 不需要重置状态（VISITED不会变回VISITING）
- ✅ 时间复杂度O(V+E)

**记忆技巧**：

```
三色标记：
白色 - 未访问
灰色 - 正在访问（在路径上）
黑色 - 已完成

遇到灰色 → 有环 → 无法完成
```

### 11.5 除法求值（399）

**核心思想**：带权有向图 + DFS路径搜索

**问题描述**：
给定方程组和对应的值，以及若干查询，计算查询的结果。

**示例**：

```
方程：a/b = 2.0, b/c = 3.0
查询：
- a/c = ? → (a/b) * (b/c) = 2.0 * 3.0 = 6.0
- b/a = ? → 1 / (a/b) = 1 / 2.0 = 0.5
- a/e = ? → e不存在 → -1.0
- a/a = ? → 1.0
```

**问题建模**：

```
变量 → 图的顶点
除法关系 → 图的边（带权重）

a/b = 2.0 表示：
- 边 a → b，权重 = 2.0
- 边 b → a，权重 = 0.5（倒数）

图结构：
  a --2.0--> b --3.0--> c
  a <-0.5--- b <-0.33-- c
```

**算法实现**：

```c
/* 图结构 */
typedef struct {
    VertexNode vertices[MAX_VERTICES];  // 顶点数组
    int visited[MAX_VERTICES];          // DFS访问标记
    int numVertices;                    // 顶点数量
    int numEdges;                       // 边数量
} Graph;

/* DFS搜索路径并累积权重 */
double dfsSearch(Graph *graph, VertexNode *current, 
                double accumulatedWeight, char *target) {
    // 1. 找到目标，返回累积权重
    if (strcmp(current->variableName, target) == 0)
        return accumulatedWeight;
    
    // 2. 标记为已访问
    graph->visited[currentIndex] = 1;
    
    // 3. 遍历所有邻居
    EdgeNode *edge = current->edgeList;
    while (edge != NULL) {
        if (graph->visited[edge->adjacentVertex] == 0) {
            // 4. 递归DFS，累积权重
            double result = dfsSearch(graph, neighbor, 
                                     accumulatedWeight * edge->weight, 
                                     target);
            if (result != -1.0)
                return result;
        }
        edge = edge->next;
    }
    
    // 5. 所有路径都找不到
    return -1.0;
}
```

**关键步骤**：

1. **构建图**：
   - 为每个变量创建顶点
   - 为每个方程创建双向边
   - a/b = 2.0 → a→b (权重2.0)，b→a (权重0.5)

2. **处理查询**：
   - 特殊情况：变量不存在 → -1.0
   - 特殊情况：相同变量 a/a → 1.0
   - 一般情况：DFS搜索路径，累积权重

**详细走查**：

```
方程：a/b = 2.0, b/c = 3.0
查询：a/c = ?

构建图：
  a → b (2.0)
  b → a (0.5)
  b → c (3.0)
  c → b (0.33)

DFS从a到c：
1. 从a开始，accumulatedWeight = 1.0
2. 访问邻居b，累积权重 = 1.0 * 2.0 = 2.0
3. 从b开始，标记visited[b]=1
4. 访问邻居c，累积权重 = 2.0 * 3.0 = 6.0
5. 到达c，返回6.0 ✓
```

**易错点总结**：

**🐛 Bug #1: getVertexIndex返回值错误**
```c
// ❌ 错误：循环后i不是-1
int i = -1;
for (i = 0; i < numVertices; i++) { ... }
return i;  // 返回numVertices（错误）

// ✅ 正确：直接返回-1
for (i = 0; i < numVertices; i++) { ... }
return -1;
```

**🐛 Bug #2: 覆盖参数weight**
```c
// ❌ 错误：覆盖参数
weight = dfsSearch(..., weight * edge->weight, ...);

// ✅ 正确：使用新变量
double result = dfsSearch(..., weight * edge->weight, ...);
```

**🐛 Bug #3: 添加反向边时索引错误**
```c
// ❌ 错误：反向边添加到错误的顶点
edge->adjacentVertex = i;
edge->next = graph->vertices[j].edgeList;
graph->vertices[i].edgeList = edge;  // 错误

// ✅ 正确：添加到顶点j
graph->vertices[j].edgeList = edge;
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 构建图 | O(E) | E个方程 |
| 每次查询DFS | O(V+E) | V个顶点，E条边 |
| **总计** | **O(E+Q×(V+E))** | Q个查询 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 图存储 | O(V+E) | 顶点和边 |
| 递归栈 | O(V) | DFS深度 |
| **总计** | **O(V+E)** | 线性空间 |

**关键要点**：
- ✅ 除法关系建模为带权有向图
- ✅ 双向边：a/b=2.0 → a→b(2.0)，b→a(0.5)
- ✅ DFS搜索路径并累积权重
- ✅ 每次查询前重置visited
- ✅ 特殊情况处理：不存在、相同变量

**记忆技巧**：

```
除法求值 = 图的路径权重累积

建图：方程 → 双向边
查询：DFS → 累积权重
```

---

## 12. 二分查找进阶

### 12.1 查找元素的第一个和最后一个位置（34）

**核心思想**：两次二分查找 + 边界搜索

**问题描述**：
给定一个按升序排列的整数数组 `nums` 和一个目标值 `target`，找出给定目标值在数组中的开始位置和结束位置。如果数组中不存在目标值，返回 `[-1, -1]`。

**要求**：时间复杂度必须是 O(log n)。

**示例**：

```
输入: nums = [5,7,7,8,8,10], target = 8
输出: [3,4]

输入: nums = [5,7,7,8,8,10], target = 6
输出: [-1,-1]

输入: nums = [], target = 0
输出: [-1,-1]
```

**核心理解**：

```
普通二分查找：找到目标值就返回
本题二分查找：找到目标值后继续搜索边界

查找起始位置：找到后向左继续搜索
查找结束位置：找到后向右继续搜索
```

**算法实现**：

**查找起始位置（最左边）**：

```c
int findStartPosition(int* nums, int numsSize, int target) {
	int left, right, mid, startPosition;

	left = 0;
	right = numsSize - 1;
	startPosition = -1;
	
	while (left <= right) {
		mid = left + (right - left) / 2;  // 避免溢出

		if (nums[mid] == target) {
			startPosition = mid;
			right = mid - 1;  // 关键：继续向左搜索
		} else if (nums[mid] < target) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return startPosition;
}
```

**查找结束位置（最右边）**：

```c
int findEndPosition(int* nums, int numsSize, int target) {
	int left, right, mid, endPosition;

	left = 0;
	right = numsSize - 1;
	endPosition = -1;

	while (left <= right) {
		mid = left + (right - left) / 2;  // 避免溢出

		if (nums[mid] == target) {
			endPosition = mid;
			left = mid + 1;  // 关键：继续向右搜索
		} else if (nums[mid] < target) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return endPosition;
}
```

**详细走查**：输入 `nums = [5,7,7,8,8,10], target = 8`

```
=== 查找起始位置 ===

初始: left=0, right=5, startPosition=-1

第1轮:
  mid = 0 + (5-0)/2 = 2
  nums[2] = 7 < 8
  left = 3

第2轮:
  mid = 3 + (5-3)/2 = 4
  nums[4] = 8 == 8  ✓ 找到了
  startPosition = 4
  right = 3  // 继续向左搜索

第3轮:
  mid = 3 + (3-3)/2 = 3
  nums[3] = 8 == 8  ✓ 找到更左边的
  startPosition = 3
  right = 2  // 继续向左搜索

第4轮:
  left > right，循环结束
  
结果: startPosition = 3 ✓

=== 查找结束位置 ===

初始: left=0, right=5, endPosition=-1

第1轮:
  mid = 2, nums[2] = 7 < 8
  left = 3

第2轮:
  mid = 4, nums[4] = 8 == 8  ✓ 找到了
  endPosition = 4
  left = 5  // 继续向右搜索

第3轮:
  mid = 5, nums[5] = 10 > 8
  right = 4

第4轮:
  left > right，循环结束
  
结果: endPosition = 4 ✓

最终答案: [3, 4]
```

**关键对比**：

| 特性 | 查找起始位置 | 查找结束位置 |
|------|------------|------------|
| 找到后的操作 | `right = mid - 1` | `left = mid + 1` |
| 搜索方向 | 向左 ← | 向右 → |
| 目的 | 找更小的索引 | 找更大的索引 |

**图解搜索过程**：

```
数组: [5, 7, 7, 8, 8, 10]
索引:  0  1  2  3  4  5

查找 8 的起始位置：
找到 nums[4]=8 后，继续向左查找
最终找到 nums[3]=8（最左边的8）

查找 8 的结束位置：
找到 nums[4]=8 后，继续向右查找
发现 nums[5]=10 > 8，确定 nums[4] 就是最右边的8
```

**易错点总结**：

**🐛 Bug #1: 中间值溢出**
```c
// ❌ 错误：left + right 可能溢出
mid = (left + right) / 2;

// ✅ 正确：避免溢出
mid = left + (right - left) / 2;
```

**🐛 Bug #2: 找到后直接返回**
```c
// ❌ 错误：找到就返回（普通二分查找）
if (nums[mid] == target) {
    return mid;
}

// ✅ 正确：找到后继续搜索边界
if (nums[mid] == target) {
    startPosition = mid;
    right = mid - 1;  // 继续搜索
}
```

**🐛 Bug #3: 搜索方向错误**
```c
// ❌ 错误：查找起始位置时向右搜索
if (nums[mid] == target) {
    startPosition = mid;
    left = mid + 1;  // 错误！应该向左
}

// ✅ 正确：查找起始位置时向左搜索
if (nums[mid] == target) {
    startPosition = mid;
    right = mid - 1;  // 正确
}
```

**🐛 Bug #4: 未处理边界情况**
```c
// ❌ 错误：没有检查空数组
int* searchRange(...) {
    startPosition = findStartPosition(nums, numsSize, target);
    // 如果 numsSize=0，会访问越界
}

// ✅ 正确：先检查边界
if (!nums || !numsSize) {
    result[0] = -1;
    result[1] = -1;
    return result;
}
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 查找起始位置 | O(log n) | 二分查找 |
| 查找结束位置 | O(log n) | 二分查找 |
| **总计** | **O(log n)** | 两次二分，仍是对数时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 局部变量 | O(1) | 常数空间 |
| 返回数组 | O(1) | 固定2个元素 |
| **总计** | **O(1)** | 常数空间 |

**测试用例**：

```c
// 用例1：找到多个
输入：nums=[5,7,7,8,8,10], target=8
输出：[3,4]

// 用例2：找到单个
输入：nums=[5,7,7,8,8,10], target=5
输出：[0,0]

// 用例3：未找到
输入：nums=[5,7,7,8,8,10], target=6
输出：[-1,-1]

// 用例4：空数组
输入：nums=[], target=0
输出：[-1,-1]

// 用例5：全是目标值
输入：nums=[1,1,1,1], target=1
输出：[0,3]
```

**变体问题**：

**问题1：只需要找起始位置**
```c
// 只调用一次 findStartPosition
int firstPosition = findStartPosition(nums, numsSize, target);
```

**问题2：统计目标值出现次数**
```c
// 利用起始和结束位置计算
int count = (endPosition == -1) ? 0 : (endPosition - startPosition + 1);
```

**问题3：查找大于等于 target 的第一个位置（lower_bound）**
```c
int lowerBound(int* nums, int numsSize, int target) {
    int left = 0, right = numsSize - 1, result = numsSize;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            result = mid;
            right = mid - 1;  // 继续向左
        } else {
            left = mid + 1;
        }
    }
    return result;
}
```

**关键要点**：
- ✅ 两次二分查找，分别找起始和结束位置
- ✅ 找到目标后不返回，继续搜索边界
- ✅ 起始位置向左搜索（right = mid - 1）
- ✅ 结束位置向右搜索（left = mid + 1）
- ✅ 避免溢出：使用 `left + (right - left) / 2`

**记忆技巧**：

```
普通二分：找到就返回
边界二分：找到继续找

起始位置 → 向左找 → right = mid - 1
结束位置 → 向右找 → left = mid + 1

口诀：找起始往左找，找结束往右找
```

**应用场景**：
- 查找排序数组中的元素范围
- 统计元素出现次数
- 二分查找的边界问题
- lower_bound 和 upper_bound 实现

---

### 12.2 寻找旋转排序数组中的最小值（153）

**核心思想**：二分查找 + 比较mid与right判断方向

**问题描述**：
假设按照升序排序的数组在预先未知的某个点上进行了旋转。例如，数组 `[0,1,2,4,5,6,7]` 可能变为 `[4,5,6,7,0,1,2]`。请找出其中最小的元素。假设数组中不存在重复元素。

**要求**：时间复杂度必须是 O(log n)。

**示例**：

```
输入: [3,4,5,1,2]
输出: 1

输入: [4,5,6,7,0,1,2]
输出: 0

输入: [1,2,3,4,5]
输出: 1  (未旋转的情况)
```

**核心理解**：

```
旋转排序数组的特点：
1. 存在一个旋转点，旋转点是最小值
2. 旋转点左边是递增的，右边也是递增的
3. 但左边的所有值 > 右边的所有值

例如：[4,5,6,7,0,1,2]
     [递增段] [递增段]
      左边      右边
     左边>右边

未旋转情况：[1,2,3,4,5]
整个数组递增，第一个元素就是最小值
```

**算法实现**：

```c
int findMin(int* nums, int numsSize) {
	int left, right, mid;

	left = 0;
	right = numsSize - 1;
	
	// 优化：如果数组未旋转，直接返回第一个元素
	if (nums[left] < nums[right])
		return nums[left];
	
	// 二分查找最小元素
	while (left < right) {
		mid = left + (right - left) / 2;  // 避免溢出

		// 关键判断：比较mid与right
		if (nums[mid] > nums[right])
			// mid在左半段（较大的段），最小值在右边
			left = mid + 1;
		else
			// mid在右半段（较小的段）或就是最小值
			right = mid;
	}

	// left == right，指向最小元素
	return nums[left];
}
```

**关键判断解析**：

```c
if (nums[mid] > nums[right])
```

**为什么比较 mid 与 right，而不是与 left？**

```
情况1：nums[mid] > nums[right]
   [4, 5, 6, 7, 0, 1, 2]
         ↑        ↑
        mid     right
   
   mid=7 > right=2
   说明：mid在左半段（大的段），旋转点在右边
   操作：left = mid + 1

情况2：nums[mid] < nums[right]
   [4, 5, 6, 7, 0, 1, 2]
               ↑     ↑
              mid  right
   
   mid=1 < right=2
   说明：mid在右半段（小的段），最小值可能是mid或在左边
   操作：right = mid（不能是mid-1，因为mid可能就是答案）
```

**为什么 `right = mid` 而不是 `right = mid - 1`？**

```
反例：[3, 4, 5, 1, 2]
     left=0, right=4

第1轮：mid=2, nums[2]=5 > nums[4]=2
      left = 3

第2轮：left=3, right=4, mid=3
      nums[3]=1 < nums[4]=2
      right = mid = 3  ← 如果right=mid-1=2，会错过答案1

第3轮：left=3, right=3，退出
      返回 nums[3]=1 ✓

如果使用 right=mid-1，会错误地排除掉最小值！
```

**详细走查**：输入 `[4,5,6,7,0,1,2]`

```
初始: left=0, right=6

快速检查：nums[0]=4 > nums[6]=2，需要二分查找

第1轮:
  mid = 0 + (6-0)/2 = 3
  nums[3]=7 > nums[6]=2  ✓ mid在左段
  left = 4

第2轮:
  left=4, right=6
  mid = 4 + (6-4)/2 = 5
  nums[5]=1 < nums[6]=2  ✓ mid在右段
  right = 5

第3轮:
  left=4, right=5
  mid = 4 + (5-4)/2 = 4
  nums[4]=0 < nums[5]=1  ✓ mid在右段
  right = 4

第4轮:
  left=4, right=4，退出循环
  
返回: nums[4]=0 ✓
```

**图解过程**：

```
[4, 5, 6, 7, 0, 1, 2]
 ↑        ↑        ↑
left     mid     right

Step 1: nums[mid]=7 > nums[right]=2
        → left = mid+1 = 4

    [4, 5, 6, 7, 0, 1, 2]
                ↑  ↑  ↑
              left mid right

Step 2: nums[mid]=1 < nums[right]=2
        → right = mid = 5

    [4, 5, 6, 7, 0, 1, 2]
                ↑  ↑
              left=mid=right-1
              
Step 3: nums[mid]=0 < nums[right]=1
        → right = mid = 4

    [4, 5, 6, 7, 0, 1, 2]
                ↑
           left=right → 找到最小值0
```

**易错点总结**：

**🐛 Bug #1: 使用 right = mid - 1**
```c
// ❌ 错误：可能跳过最小值
if (nums[mid] < nums[right])
    right = mid - 1;  // 错误！

// ✅ 正确：mid可能就是最小值
if (nums[mid] < nums[right])
    right = mid;
```

**🐛 Bug #2: 比较 mid 与 left**
```c
// ❌ 错误：比较mid和left无法准确判断
if (nums[mid] > nums[left])
    left = mid + 1;

// ✅ 正确：必须比较mid和right
if (nums[mid] > nums[right])
    left = mid + 1;
```

**🐛 Bug #3: 循环条件错误**
```c
// ❌ 错误：使用 left <= right
while (left <= right) {
    // 可能死循环
}

// ✅ 正确：使用 left < right
while (left < right) {
    // left 和 right 会收敛到同一位置
}
```

**🐛 Bug #4: 忘记处理未旋转情况**
```c
// ❌ 效率低：即使未旋转也要二分查找
// 直接进入二分

// ✅ 优化：提前判断
if (nums[left] < nums[right])
    return nums[left];  // 未旋转，直接返回
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 快速检查 | O(1) | 比较首尾元素 |
| 二分查找 | O(log n) | 每次排除一半 |
| **总计** | **O(log n)** | 对数时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 局部变量 | O(1) | 只用了几个变量 |
| **总计** | **O(1)** | 常数空间 |

**测试用例**：

```c
// 用例1：旋转一次
输入：[4,5,6,7,0,1,2]
输出：0

// 用例2：未旋转
输入：[1,2,3,4,5]
输出：1

// 用例3：旋转到最后
输入：[2,1]
输出：1

// 用例4：单个元素
输入：[1]
输出：1

// 用例5：旋转多次
输入：[3,4,5,1,2]
输出：1
```

**相关题目**：

**LeetCode 33: 搜索旋转排序数组**
- 在旋转数组中查找特定值
- 也需要判断哪半边是有序的

**LeetCode 154: 寻找旋转排序数组中的最小值 II**
- 允许重复元素
- 需要额外处理 nums[mid] == nums[right] 的情况

```c
// 154题的关键区别
if (nums[mid] > nums[right]) {
    left = mid + 1;
} else if (nums[mid] < nums[right]) {
    right = mid;
} else {
    // 相等时无法判断，只能线性缩小范围
    right--;  // 最坏情况O(n)
}
```

**关键要点**：
- ✅ 提前判断未旋转情况（优化）
- ✅ 比较 mid 与 right（不是left）
- ✅ nums[mid] > nums[right] → 左半段，left = mid + 1
- ✅ nums[mid] <= nums[right] → 右半段，right = mid
- ✅ right = mid 而不是 mid - 1（mid可能是答案）
- ✅ 循环条件是 left < right（不是 <=）

**记忆技巧**：

```
比较对象：mid vs right（记住：向右看）

判断方向：
  mid > right → 在左段 → 最小值在右边 → left = mid + 1
  mid < right → 在右段 → 最小值可能是mid → right = mid

口诀：大在右找，小在左守（mid大说明最小值在右边要找，mid小说明可能就是最小值要守住）
```

**应用场景**：
- 旋转数组问题
- 二分查找变体
- 查找数组中的特殊点（旋转点）
- 分段有序数组

---

## 13. 贪心/边界问题

### 13.1 统计被其他建筑包围的建筑（3531）

**核心思想**：边界标记法 + O(n) 时间复杂度

**问题描述**：
给定一个 n×n 的网格和若干建筑物的坐标 `[x, y]`，统计有多少建筑物被其他建筑物完全包围。

**被包围的定义**：
一个建筑物被包围，当且仅当：
- 在它所在的行中，它不是最左或最右的建筑
- 在它所在的列中，它不是最上或最下的建筑

**示例**：

```
输入: n = 3, buildings = [[1,2],[2,2],[3,2],[2,1],[2,3]]

网格可视化（x=列, y=行）：
     x: 1   2   3
y: 1       ●
   2   ●   ●   ●
   3       ●

形成十字形，中心是 [2,2]

分析：
- [2,2] 在第2行：不在边界（列范围1~3，它是2）✓
- [2,2] 在第2列：不在边界（行范围1~3，它是2）✓
- 其他4个建筑都在各自行或列的边界上

输出: 1 (只有中心建筑被包围)
```

**算法实现**：

```c
/**
 * 初始化边界数组
 */
void initializeBoundaries(int gridSize, int **buildings, int buildingsSize,
                         int *rowMaxCols, int *rowMinCols, 
                         int *colMaxRows, int *colMinRows) {
	int i, currentRow, currentCol;

	// 初始化为极值
	for (i = 0; i <= gridSize; i++) {
		rowMaxCols[i] = 0;                // 每行的最大列号
		rowMinCols[i] = gridSize + 1;    // 每行的最小列号
		colMaxRows[i] = 0;                // 每列的最大行号
		colMinRows[i] = gridSize + 1;    // 每列的最小行号
	}

	// 遍历所有建筑，更新边界
	for (i = 0; i < buildingsSize; i++) {
		currentCol = buildings[i][0];  // x坐标
		currentRow = buildings[i][1];  // y坐标

		// 更新该行的列边界
		if (currentCol > rowMaxCols[currentRow])
			rowMaxCols[currentRow] = currentCol;
		if (currentCol < rowMinCols[currentRow])
			rowMinCols[currentRow] = currentCol;
		
		// 更新该列的行边界
		if (currentRow > colMaxRows[currentCol])
			colMaxRows[currentCol] = currentRow;
		if (currentRow < colMinRows[currentCol])
			colMinRows[currentCol] = currentRow;
	}
}

/**
 * 统计被完全包围的建筑
 */
int countFullyCoveredBuildings(int gridSize, int **buildings, int buildingsSize,
                               int *rowMaxCols, int *rowMinCols,
                               int *colMaxRows, int *colMinRows) {
	int i, currentRow, currentCol, coveredCount = 0;
	
	for (i = 0; i < buildingsSize; i++) {
		currentCol = buildings[i][0];
		currentRow = buildings[i][1];

		// 判断是否在两个方向都不在边界
		if (currentCol > rowMinCols[currentRow] && 
		    currentCol < rowMaxCols[currentRow] &&
		    currentRow > colMinRows[currentCol] && 
		    currentRow < colMaxRows[currentCol]) {
			coveredCount++;
		}
	}

	return coveredCount;
}
```

**详细走查**：输入 `n=3, buildings=[[1,2],[2,2],[3,2],[2,1],[2,3]]`

```
=== 步骤1: 初始化边界数组 ===

初始值（i=0到3）：
rowMaxCols = [0, 0, 0, 0]
rowMinCols = [4, 4, 4, 4]
colMaxRows = [0, 0, 0, 0]
colMinRows = [4, 4, 4, 4]

=== 步骤2: 遍历建筑，更新边界 ===

建筑1: [1,2]（第2行第1列）
  rowMaxCols[2] = 1, rowMinCols[2] = 1
  colMaxRows[1] = 2, colMinRows[1] = 2

建筑2: [2,2]（第2行第2列）
  rowMaxCols[2] = 2, rowMinCols[2] = 1
  colMaxRows[2] = 2, colMinRows[2] = 2

建筑3: [3,2]（第2行第3列）
  rowMaxCols[2] = 3, rowMinCols[2] = 1
  colMaxRows[3] = 2, colMinRows[3] = 2

建筑4: [2,1]（第1行第2列）
  rowMaxCols[1] = 2, rowMinCols[1] = 2
  colMaxRows[2] = 2, colMinRows[2] = 1

建筑5: [2,3]（第3行第2列）
  rowMaxCols[3] = 2, rowMinCols[3] = 2
  colMaxRows[2] = 3, colMinRows[2] = 1

最终边界：
第1行：列范围 [2, 2]
第2行：列范围 [1, 3]
第3行：列范围 [2, 2]
第1列：行范围 [2, 2]
第2列：行范围 [1, 3]
第3列：行范围 [2, 2]

=== 步骤3: 检查每个建筑 ===

[1,2]: 列2在[2,2]边界上 → 不被包围
[2,2]: 列2在(1,3)内 ✓，行2在(1,3)内 ✓ → 被包围！
[3,2]: 列3在[3,3]边界上 → 不被包围
[2,1]: 行1在[1,1]边界上 → 不被包围
[2,3]: 行3在[3,3]边界上 → 不被包围

结果: coveredCount = 1
```

**关键变量说明**：

| 变量名 | 含义 | 说明 |
|--------|------|------|
| `rowMaxCols[i]` | 第i行的最大列号 | 该行最右边的建筑 |
| `rowMinCols[i]` | 第i行的最小列号 | 该行最左边的建筑 |
| `colMaxRows[j]` | 第j列的最大行号 | 该列最下边的建筑 |
| `colMinRows[j]` | 第j列的最小行号 | 该列最上边的建筑 |

**判断条件解析**：

```c
// 建筑在 (currentCol, currentRow)
// 被包围需要满足：

// 条件1: 在行方向不在边界
currentCol > rowMinCols[currentRow]  // 不是该行最左
currentCol < rowMaxCols[currentRow]  // 不是该行最右

// 条件2: 在列方向不在边界
currentRow > colMinRows[currentCol]  // 不是该列最上
currentRow < colMaxRows[currentCol]  // 不是该列最下
```

**易错点总结**：

**🐛 Bug #1: 数组大小错误**
```c
// ❌ 错误：坐标范围是1到n，但数组只分配n
int *rowMaxCols = malloc(n * sizeof(int));

// ✅ 正确：需要n+1个位置（索引0不用）
int *rowMaxCols = malloc((n + 1) * sizeof(int));
```

**🐛 Bug #2: 初始化值错误**
```c
// ❌ 错误：使用0xffffffff（-1）
memset(rowMinCols, 0xffffffff, n * sizeof(int));

// ✅ 正确：使用循环初始化为n+1
for (i = 0; i <= n; i++) {
    rowMinCols[i] = n + 1;
}
```

**🐛 Bug #3: 坐标理解错误**
```c
// buildings[i][0] 是 x 坐标（列）
// buildings[i][1] 是 y 坐标（行）

// ❌ 错误：颠倒
currentRow = buildings[i][0];
currentCol = buildings[i][1];

// ✅ 正确
currentCol = buildings[i][0];  // x = 列
currentRow = buildings[i][1];  // y = 行
```

**🐛 Bug #4: 边界判断使用 >= 而不是 >**
```c
// ❌ 错误：使用 >=，边界上的也算被包围
if (currentCol >= rowMinCols[currentRow] && 
    currentCol <= rowMaxCols[currentRow])

// ✅ 正确：使用 >，排除边界
if (currentCol > rowMinCols[currentRow] && 
    currentCol < rowMaxCols[currentRow])
```

**复杂度分析**：

| 操作 | 时间复杂度 | 说明 |
|-----|-----------|------|
| 初始化边界 | O(n) | 遍历n个格子 |
| 更新边界 | O(m) | 遍历m个建筑 |
| 统计被包围 | O(m) | 遍历m个建筑 |
| **总计** | **O(n+m)** | 线性时间 |

| 空间 | 空间复杂度 | 说明 |
|-----|-----------|------|
| 边界数组 | O(n) | 4个长度为n的数组 |
| 其他变量 | O(1) | 常数空间 |
| **总计** | **O(n)** | 线性空间 |

**测试用例**：

```c
// 用例1：十字形（中心被包围）
输入：n=3, buildings=[[1,2],[2,2],[3,2],[2,1],[2,3]]
输出：1

// 用例2：L形（无被包围）
输入：n=3, buildings=[[1,1],[1,2],[2,1]]
输出：0

// 用例3：3×3方形（中心被包围）
输入：n=3, buildings=[[1,1],[1,2],[1,3],
                     [2,1],[2,2],[2,3],
                     [3,1],[3,2],[3,3]]
输出：1

// 用例4：两个建筑
输入：n=2, buildings=[[1,1],[2,2]]
输出：0

// 用例5：单个建筑
输入：n=1, buildings=[[1,1]]
输出：0
```

**优化方案**：

**方法1：原地标记（节省空间）**
- 如果可以修改输入，可以直接在网格上标记
- 不需要额外的边界数组

**方法2：哈希表优化（稀疏矩阵）**
- 如果网格很大但建筑很少，可以用哈希表
- 空间复杂度从O(n)降到O(m)

**关键要点**：
- ✅ 坐标系理解：x=列，y=行
- ✅ 边界数组：分别记录行和列的最大最小值
- ✅ 判断条件：严格大于/小于（不能等于）
- ✅ 数组大小：n+1（坐标从1开始）
- ✅ 初始化：用循环而不是memset

**记忆技巧**：

```
被包围 = 两个方向都不在边界

行方向：minCol < 当前列 < maxCol
列方向：minRow < 当前行 < maxRow

边界数组命名规则：
rowMaxCols = 行的最大列
colMaxRows = 列的最大行
交叉命名，避免混淆
```

**应用场景**：
- 二维网格的边界问题
- 区间重叠判断
- 空间范围分析
- 贪心算法应用

---

## 14. C语言常见陷阱

### 11.1 指针相关

#### 运算符优先级陷阱（重要！）

**问题**：`*ptr++` 不等于 `(*ptr)++`

```c
int value = 5;
int *ptr = &value;

// 错误写法：*ptr++
*ptr++;  // 等价于 *(ptr++)
// 1. ptr++ → ptr指针移动
// 2. * → 解引用移动后的位置（未定义行为！）
// value 还是 5，没有改变！❌

// 正确写法：(*ptr)++
(*ptr)++;  
// 1. *ptr → 解引用，获取value
// 2. ++ → value增加
// value 变成 6 ✓
```

**运算符优先级表**：

| 表达式 | 等价于 | 操作 |
|-------|--------|------|
| `*ptr++` | `*(ptr++)` | 指针后移再解引用 ❌ |
| `(*ptr)++` | `(*ptr)++` | 值增加 ✅ |
| `++*ptr` | `++(*ptr)` | 值增加 ✅ |
| `*++ptr` | `*(++ptr)` | 指针先移再解引用 ❌ |

**在栈操作中的应用**：
```c
void push(int *top, int val) {
    (*top)++;  // ✅ 栈顶索引增加
    // *top++; ❌ 错误！移动指针
}
```

#### strlen 不包含 `\0`

```c
int len = strlen(s);           // 不包含 \0
char* result = malloc(len + 1); // +1 留给 \0
```

#### 指针解引用

```c
// 错误
returnColumnSizes = (int*)malloc(...);  // int** = int*，类型不匹配

// 正确
*returnColumnSizes = (int*)malloc(...); // *int** = int*
```

### 11.2 数组相关

#### 数组大小计算

```c
// 如果索引范围是 [min, max]
数组大小 = max - min + 1

// 数独：数字1-9
bool used[9][10];  // 需要10个位置，索引0不用
```

#### VLA（可变长度数组）

```c
char* rows[numRows];  // VLA，在栈上

// 可以free rows[i]（malloc的）
free(rows[i]);

// 不能free rows本身（栈上的）
// free(rows);  ❌ 导致buffer overflow
```

### 11.3 排序相关

#### qsort API 完整说明

**函数原型**：
```c
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
```

**参数说明**：
- `base`：数组首地址
- `nmemb`：元素个数
- `size`：**每个元素的字节数**
- `compar`：比较函数

**比较函数规则**：
```
返回负数（< 0）：a排在b前面
返回0：         a和b顺序不变
返回正数（> 0）：a排在b后面
```

#### 示例1：整数数组升序

```c
int arr[] = {5, 2, 8, 1, 9};

int compareAsc(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    
    // 升序：return a - b
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

qsort(arr, 5, sizeof(int), compareAsc);
// 结果：[1, 2, 5, 8, 9]
```

#### 示例2：整数数组降序

```c
int arr[] = {5, 2, 8, 1, 9};

int compareDesc(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    
    // 降序：return b - a
    if (val_a > val_b) return -1;  // a大，排前面
    if (val_a < val_b) return 1;
    return 0;
}

qsort(arr, 5, sizeof(int), compareDesc);
// 结果：[9, 8, 5, 2, 1]
```

#### 示例3：二维数组（指针数组）排序

```c
int** intervals = [[7,10], [1,5], [3,6]];

int compareIntervals(const void *a, const void *b) {
    // a和b是 int** 类型（指向int*的指针）
    int *arr1 = *(int**)a;  // 解引用得到int*
    int *arr2 = *(int**)b;
    
    // 按第一个元素升序
    if (arr1[0] < arr2[0]) return -1;
    if (arr1[0] > arr2[0]) return 1;
    return 0;
}

// 注意：元素大小是指针大小
qsort(intervals, 3, sizeof(int*), compareIntervals);
// 结果：[[1,5], [3,6], [7,10]]
```

#### 示例4：结构体数组排序

```c
typedef struct {
    char name[50];
    int score;
} Student;

Student students[] = {
    {"Alice", 85},
    {"Bob", 92},
    {"Charlie", 78}
};

int compareByScore(const void *a, const void *b) {
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    
    // 按分数降序
    if (s1->score > s2->score) return -1;
    if (s1->score < s2->score) return 1;
    return 0;
}

qsort(students, 3, sizeof(Student), compareByScore);
// 结果：Bob(92), Alice(85), Charlie(78)
```

#### 升序 vs 降序记忆法

| 代码模式 | 结果 | 理解 |
|---------|------|------|
| `return a - b` | 升序 ↑ | a小返回负，a排前 |
| `return b - a` | 降序 ↓ | a大返回负，a排前 |
| `if (a < b) return -1` | 升序 ↑ | a小排前 |
| `if (a > b) return -1` | 降序 ↓ | a大排前 |

#### qsort 的元素大小

```c
// 错误：对int**数组排序
qsort(intervals, n, 2*sizeof(int), cmp);  // ❌

// 正确：元素是指针
qsort(intervals, n, sizeof(int*), cmp);   // ✅
```

#### 比较函数的类型转换

```c
// int**数组的比较函数
int cmp(const void *a, const void *b) {
    int *arr1 = *(int**)a;  // ✅ 先转int**，再解引用
    int *arr2 = *(int**)b;
    return arr1[0] - arr2[0];
}
```

#### 比较函数的整数溢出陷阱（重要！）

```c
// 危险：大数值会溢出
int cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;  // ❌
}

// 测试用例：
a = -2147483645, b = 2147483647
a - b = -4294967292  // 超出int范围！溢出！
```

**安全的写法**：

```c
// 方法1：if判断（推荐）
int cmp(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

// 方法2：技巧写法
int cmp(const void *a, const void *b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    return (val_a > val_b) - (val_a < val_b);
}
```

**适用场景**：
- 小范围（0-1000）：`return a - b` 可以
- **大范围（int全范围）：必须用if判断**
- **不确定：总是用if判断最安全**

### 11.4 全局变量陷阱

```c
bool rowUsed[9][10] = {false};  // 全局变量

void function() {
    // LeetCode多次调用时，全局变量保留上次的值！
    memset(rowUsed, false, sizeof(rowUsed));  // ✅ 必须重置
}
```

### 11.5 strcat 的性能陷阱

```c
// 多次strcat：O(n²)
for (int i = 0; i < n; i++) {
    strcat(result, str[i]);  // 每次都要找 \0
}

// 手动维护pos：O(n)
int pos = 0;
for (int i = 0; i < n; i++) {
    while (*str) result[pos++] = *str++;
}
```

---

## 15. 字符串处理

### 15.1 反转字符串中的单词（151）

**双指针压缩空格**：

```c
char *read = result;   // 读指针
char *write = result;  // 写指针

while (*read != '\0') {
    while (*read == ' ') read++;  // 跳过空格
    
    if (write != result) *write++ = ' ';  // 单词间加空格
    
    while (*read != ' ' && *read != '\0') {
        *write++ = *read++;  // 复制单词
    }
}
*write = '\0';
```

### 15.2 Integer to Roman（12）

**贪心 + 数据驱动**：

```c
int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

for (int i = 0; i < 13; i++) {
    while (num >= values[i]) {
        // 添加symbols[i]
        num -= values[i];
    }
}
```

**关键点**：
- ✅ 包含特殊值（4, 9, 40, 90, 400, 900）
- ✅ 从大到小贪心选择
- ⚠️ 不要用数字作为数组索引（稀疏浪费）

---

## 16. 复杂度分析技巧

### 16.1 摊还分析

**动态数组扩容**：
- 单次realloc：O(n)
- 但倍增策略下，摊还O(1)

```
插入n个元素的总成本：
拷贝：1 + 2 + 4 + ... + n/2 = n-1
插入：n
总计：2n-1 = O(n)
摊还：O(n)/n = O(1)
```

### 16.2 BFS的隐式实现

**Jump Game II**：
- 传统BFS需要队列（O(n)空间）
- 利用连续性，用边界变量代替队列（O(1)空间）

```c
// 隐式BFS
int currentEnd = 0;  // 当前层边界
int farthest = 0;    // 下一层边界

for (i = 0; i < n-1; i++) {
    farthest = max(farthest, i + nums[i]);
    
    if (i == currentEnd) {  // 到达层边界
        steps++;
        currentEnd = farthest;  // 进入下一层
    }
}
```

---

## 17. 易错点总结

### 17.1 数组越界

| 问题 | 原因 | 解决 |
|-----|------|------|
| 数独数组 | 数字1-9，但数组只到8 | `[9][10]`，索引0不用 |
| 动态分配不够 | Three Sum结果可能O(n²) | 分配 `n²/2` 空间 |

### 17.2 边界条件

| 场景 | 检查 | 原因 |
|-----|------|------|
| 螺旋矩阵方向3 | `if (top <= bottom)` | 避免单行重复 |
| 滑动窗口 | `charLastPos[ch] >= left` | 防止left后退 |
| Jump Game II | `i < n-1` | 不要遍历到终点 |

### 17.3 变量命名

| 不好 | 更好 | 为什么 |
|-----|------|--------|
| `sum` | `currentSum` | 更明确 |
| `i, j` | `row, col` | 更清晰 |
| `hash` | `charLastPos` | 说明用途 |
| `cmp` | `compareIntervals` | 说明比较什么 |

---

## 18. 面试技巧

### 18.1 思路讲解

**推荐顺序**：
1. 重述问题（确保理解）
2. 举例说明（画图辅助）
3. 提出思路（从暴力到优化）
4. 分析复杂度
5. 讨论边界情况
6. 开始编码

### 18.2 代码规范

**变量命名**：
- ✅ 用完整单词：`maxLength` 而不是 `maxLen`
- ✅ 驼峰命名：`currentSum` 而不是 `current_sum`
- ✅ 语义明确：`liveNeighbors` 而不是 `count`

**注释规范**：
- ✅ 算法思想和复杂度
- ✅ 关键步骤说明
- ✅ 边界情况注释

### 18.3 常见优化思路

| 从 | 到 | 技巧 |
|---|----|----|
| O(n²) | O(n) | 哈希表、双指针、滑动窗口 |
| O(n²) | O(n log n) | 排序 |
| O(n)空间 | O(1)空间 | 原地操作、状态编码、交织法 |

---

## 19. 题目分类速查

### 看到这些关键词 → 想到这些方法

| 关键词 | 方法 | 例题 |
|-------|------|------|
| 有序数组 + 两数之和 | 双指针 | 167 |
| 无序数组 + 三数之和 | 排序 + 双指针 | 15 |
| 最长/最短子串 | 滑动窗口 | 3, 209 |
| 无重复 | 哈希表 + 滑动窗口 | 3 |
| 连续序列 | 哈希集合 | 128 |
| 异位词 | 排序作为键 + 哈希表 | 49 |
| 矩阵旋转 | 转置 + 反转 | 48 |
| 原地更新 | 状态编码 | 73, 289 |
| O(1)随机访问 | 数组 + 哈希表 | 380 |
| 最少步数 | BFS思想 | 45 |
| 跳过不可能 | 贪心 + 数学定理 | 134 |
| 验证BST | 范围约束递归传递 + long | 98 |
| 层序遍历 | BFS + 队列 | 102 |
| 锯齿形层序遍历 | BFS + 方向标记 + 逆序 | 103 |
| 前序+中序构造树 | 递归 + 哈希表 | 105 |
| 中序+后序构造树 | 递归 + 哈希表 | 106 |
| 二叉树展开为链表 | Morris遍历 + O(1)空间 | 114 |
| 填充next指针 | 层序遍历 + next指针 | 117 |
| 根到叶路径数字和 | DFS路径累积 | 129 |
| BST迭代器 | 栈模拟中序遍历 | 173 |
| 二叉树右视图 | BFS层序遍历 + 记录最右 | 199 |
| BST第K小元素 | 中序遍历 + 计数 + 剪枝 | 230 |
| 最近公共祖先 | 后序遍历 + 信息向上传递 | 236 |
| 被围绕的区域 | 反向思维 + 从边界DFS | 130 |
| 克隆图 | DFS + 哈希表 + 先克隆后递归 | 133 |
| 岛屿数量 | DFS标记连通分量 | 200 |
| 课程表 | 拓扑排序 + 三色标记检测环 | 207 |
| 课程表II | DFS后序遍历 + 栈反序输出 | 210 |
| 除法求值 | 带权图 + DFS路径搜索 | 399 |
| 最小基因变化 | BFS + 哈希表O(1)优化 | 433 |
| 蛇梯棋 | BFS最短路径 + 之字形编号 | 909 |
| 统计被包围的建筑 | 边界标记法 + 行列最大最小值 | 3531 |
| Trie前缀树 | 哈希表存储子节点 + 前向声明 | 208 |
| Trie通配符搜索 | DFS递归 + 通配符遍历所有子节点 | 211 |
| 电话号码字母组合 | DFS回溯 + 选择递归回溯 | 17 |
| 括号生成 | DFS回溯 + 括号匹配规则 | 22 |
| 组合总和 | DFS回溯 + 允许重复选择 | 39 |
| 全排列 | DFS回溯 + visited标记 | 46 |
| 组合 | DFS回溯 + 剪枝优化 | 77 |
| 单词搜索 | DFS回溯 + 四方向搜索 | 79 |
| 排序链表 | 归并排序 + 快慢指针找中点 | 148 |
| 建立四叉树 | 分治递归 + 区域检查 | 427 |
| 最大子数组和 | Kadane算法 + 动态规划 | 53 |
| 环形子数组最大和 | Kadane变种 + totalSum-minSum | 918 |
| 搜索二维矩阵 | 二分查找 + 一维化索引转换 | 74 |
| 寻找峰值 | 二分查找 + 沿上坡方向 | 162 |
| 搜索旋转排序数组 | 二分查找 + 判断有序半边 | 33 |
| 查找元素首尾位置 | 二分查找 + 边界搜索 + 找到后继续搜索 | 34 |
| 旋转数组中的最小值 | 二分查找 + 比较mid与right判断方向 | 153 |

---

## 回溯算法核心理解

### 组合 vs 全排列：为什么用start vs visited？

**组合（Combinations）**：选出k个数，**顺序无关**
- `[1,2]` 和 `[2,1]` 是**同一个组合**
- **用start避免重复**

```c
void dfs_combination(int n, int k, int start, ...) {
    for (int i = start; i <= n; i++) {  // 从start开始
        path[pathSize] = i;
        dfs_combination(n, k, i + 1, ...);  // 下次从i+1开始
    }
}

// 为什么这样能避免重复？
// 保证 path[0] < path[1] < path[2] < ...
// [1,2] 会生成，但 [2,1] 不会（选2后，start=3，不会回头选1）
```

**DFS树示例（n=4, k=2，用start）**：
```
           []
    /      |     |    \
   1       2     3     4
  /|\      |\     |
 2 3 4     3 4    4

生成：[1,2], [1,3], [1,4], [2,3], [2,4], [3,4]
✅ 无重复：没有[2,1], [3,1], [3,2]等
```

**全排列（Permutations）**：排列n个数，**顺序相关**
- `[1,2]` 和 `[2,1]` 是**不同的排列**
- **用visited避免重复使用同一数字**

```c
void dfs_permutation(int *nums, int n, int *visited, ...) {
    for (int i = 0; i < n; i++) {  // 每次都从0开始
        if (visited[i]) continue;  // 跳过已使用的
        
        visited[i] = 1;
        path[pathSize] = nums[i];
        dfs_permutation(nums, n, visited, ...);
        visited[i] = 0;
    }
}

// 为什么这样能生成所有排列？
// 每层都可以选任何未使用的数字
// [1,2] 和 [2,1] 都会生成
```

**DFS树示例（nums=[1,2,3]，用visited）**：
```
              []
        /      |      \
       1       2       3
      / \     / \     / \
     2   3   1   3   1   2
     |   |   |   |   |   |
     3   2   3   1   2   1

生成6个排列：
[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]
✅ [1,2,3] ≠ [2,1,3]（顺序不同）
```

### 关键对比

| 特征 | 组合 | 全排列 |
|-----|------|--------|
| 顺序 | 无关 | 相关 |
| [1,2]和[2,1] | 相同 | 不同 |
| 避免重复方式 | start | visited |
| for循环 | `for(i=start; i<=n; i++)` | `for(i=0; i<n; i++)` |
| 递归参数 | `dfs(i+1, ...)` | `dfs(...)` |
| 数量 | C(n,k) | n! |

### 记忆技巧

```
组合 = 选数字（顺序无关）
      → 保证递增（start）
      → [1,2]只出现一次

排列 = 排数字（顺序相关）
      → 标记已用（visited）
      → [1,2]和[2,1]都要
```

**核心理解**：
- **start控制"方向"**：只能往后选，不能回头
- **visited控制"使用"**：标记用过的，可以任意选

---

## 20. 学习建议

### 16.1 从简单到困难

1. **双指针**：Two Sum II → Three Sum → Container With Water
2. **滑动窗口**：Minimum Subarray → Longest Substring
3. **贪心**：Jump Game I → Jump Game II → Gas Station
4. **矩阵**：Valid Sudoku → Rotate Image → Spiral Matrix

### 16.2 重点掌握

**必须理解的概念**：
- ✅ 双指针的三种类型（对撞、快慢、固定间距）
- ✅ 滑动窗口的扩展和收缩
- ✅ 贪心算法的正确性证明
- ✅ 哈希表的时间复杂度（平均O(1)）
- ✅ 摊还分析（动态数组）

**必须练习的技巧**：
- ✅ 原地操作（状态编码、首行首列标记）
- ✅ 空间优化（交织法、双指针）
- ✅ 去重技巧（排序 + 跳过重复）

### 16.3 调试技巧

**常见bug**：
1. 数组越界 → 仔细计算索引范围
2. 全局变量未重置 → 函数开始时清零
3. 指针解引用错误 → 注意 `*` 的位置
4. 类型转换错误 → qsort的cmp函数

**调试方法**：
- 用简单用例手动模拟
- 打印中间状态
- 检查边界条件（空、单元素、全相同）

---

## 21. 总结

### 核心思想

**双指针**：
> 利用有序性质或特定规律，用两个指针高效遍历

**滑动窗口**：
> 维护一个动态窗口，扩展和收缩以满足条件

**贪心算法**：
> 每步做局部最优选择，基于数学证明保证全局最优

**哈希表**：
> 空间换时间，O(1)查找和插入

**状态编码**：
> 用不同的值编码多个状态，实现原地更新

### 最重要的能力

1. **问题转换**：把复杂问题转换为简单问题
   - Jump Game：从"如何跳"转换为"能到哪"
   - Rotate Image：从"旋转"转换为"转置+反转"

2. **数据结构选择**：根据操作特点选择合适的数据结构
   - 需要随机访问 → 数组
   - 需要快速查找 → 哈希表
   - 两者都需要 → 组合使用

3. **复杂度分析**：理解摊还分析、空间时间权衡

4. **代码质量**：清晰的变量名、充分的注释、处理边界情况

---

**最后的话**：

> 算法学习不是记忆模板，而是理解思想。
> 同一个技巧（如双指针、贪心）在不同问题中有不同的应用。
> 关键是理解"为什么这样做"，而不是"怎么做"。

**持续更新中...**

