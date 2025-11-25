# LeetCode 解题经验与技巧总结

本文档总结了23道LeetCode题目的核心算法思想、常见技巧和易错点。

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
10. [C语言常见陷阱](#10-c语言常见陷阱)
11. [面试技巧](#11-面试技巧)

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

## 12. 数据结构设计

### 12.1 Insert Delete GetRandom O(1)（380）

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

## 14. C语言常见陷阱

### 14.1 指针相关

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

### 14.2 数组相关

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

### 14.3 排序相关

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

### 14.4 全局变量陷阱

```c
bool rowUsed[9][10] = {false};  // 全局变量

void function() {
    // LeetCode多次调用时，全局变量保留上次的值！
    memset(rowUsed, false, sizeof(rowUsed));  // ✅ 必须重置
}
```

### 14.5 strcat 的性能陷阱

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

### 14.2 Integer to Roman（12）

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

### 14.2 BFS的隐式实现

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

### 14.1 数组越界

| 问题 | 原因 | 解决 |
|-----|------|------|
| 数独数组 | 数字1-9，但数组只到8 | `[9][10]`，索引0不用 |
| 动态分配不够 | Three Sum结果可能O(n²) | 分配 `n²/2` 空间 |

### 14.2 边界条件

| 场景 | 检查 | 原因 |
|-----|------|------|
| 螺旋矩阵方向3 | `if (top <= bottom)` | 避免单行重复 |
| 滑动窗口 | `charLastPos[ch] >= left` | 防止left后退 |
| Jump Game II | `i < n-1` | 不要遍历到终点 |

### 14.3 变量命名

| 不好 | 更好 | 为什么 |
|-----|------|--------|
| `sum` | `currentSum` | 更明确 |
| `i, j` | `row, col` | 更清晰 |
| `hash` | `charLastPos` | 说明用途 |
| `cmp` | `compareIntervals` | 说明比较什么 |

---

## 18. 面试技巧

### 14.1 思路讲解

**推荐顺序**：
1. 重述问题（确保理解）
2. 举例说明（画图辅助）
3. 提出思路（从暴力到优化）
4. 分析复杂度
5. 讨论边界情况
6. 开始编码

### 14.2 代码规范

**变量命名**：
- ✅ 用完整单词：`maxLength` 而不是 `maxLen`
- ✅ 驼峰命名：`currentSum` 而不是 `current_sum`
- ✅ 语义明确：`liveNeighbors` 而不是 `count`

**注释规范**：
- ✅ 算法思想和复杂度
- ✅ 关键步骤说明
- ✅ 边界情况注释

### 14.3 常见优化思路

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

