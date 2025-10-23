/**
 * @file find_common_node.c
 * @brief 寻找两个链表的公共节点
 * 
 * 题目：给定两个链表，找到它们的第一个公共节点。如果没有公共节点，返回NULL。
 * 
 * 这是一个经典的链表操作面试题，考察：
 * 1. 链表操作和指针处理
 * 2. 算法设计和优化思维
 * 3. 空间复杂度优化
 * 4. 边界条件处理
 * 5. 多种解法的比较分析
 * 6. 数学思维的应用
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/**
 * @brief 链表节点结构体
 */
typedef struct ListNode {
    int val;                    // 节点值
    struct ListNode *next;      // 指向下一个节点的指针
} ListNode;

/**
 * @brief 创建新的链表节点
 * @param val 节点值
 * @return 新创建的节点指针
 */
ListNode* create_node(int val) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (!node) {
        printf("内存分配失败\n");
        return NULL;
    }
    node->val = val;
    node->next = NULL;
    return node;
}

/**
 * @brief 打印链表
 * @param head 链表头指针
 * @param name 链表名称
 */
void print_list(ListNode *head, const char *name) {
    printf("%s: ", name);
    ListNode *current = head;
    while (current) {
        printf("%d", current->val);
        if (current->next) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}

/**
 * @brief 计算链表长度
 * @param head 链表头指针
 * @return 链表长度
 */
int get_list_length(ListNode *head) {
    int length = 0;
    while (head) {
        length++;
        head = head->next;
    }
    return length;
}

/**
 * @brief 方法1：暴力解法 - 双重循环
 * 
 * 算法思路：
 * 1. 对于链表A的每个节点，遍历链表B的所有节点
 * 2. 比较节点地址（不是值）是否相同
 * 3. 找到第一个相同的节点即为公共节点
 * 
 * 时间复杂度：O(m*n) - m,n分别为两个链表的长度
 * 空间复杂度：O(1)
 * 
 * @param headA 链表A的头节点
 * @param headB 链表B的头节点
 * @return 公共节点指针，如果没有则返回NULL
 */
ListNode* find_intersection_brute_force(ListNode *headA, ListNode *headB) {
    printf("\n=== 方法1：暴力解法 ===\n");
    printf("算法：双重循环，对每个A节点遍历B链表\n");
    printf("时间复杂度：O(m*n), 空间复杂度：O(1)\n");
    
    if (!headA || !headB) {
        printf("其中一个链表为空，无公共节点\n");
        return NULL;
    }
    
    ListNode *currentA = headA;
    
    // 遍历链表A的每个节点
    while (currentA) {
        ListNode *currentB = headB;
        printf("检查节点A(%d)与链表B的所有节点\n", currentA->val);
        
        // 对当前A节点，遍历链表B的所有节点
        while (currentB) {
            // 注意：比较的是节点地址，不是节点值
            if (currentA == currentB) {
                printf("找到公共节点：值=%d, 地址=%p\n", currentA->val, (void*)currentA);
                return currentA;
            }
            currentB = currentB->next;
        }
        
        currentA = currentA->next;
    }
    
    printf("未找到公共节点\n");
    return NULL;
}

/**
 * @brief 方法2：哈希表解法
 * 
 * 算法思路：
 * 1. 遍历链表A，将所有节点地址存入哈希表
 * 2. 遍历链表B，检查每个节点是否在哈希表中
 * 3. 第一个在哈希表中的节点即为公共节点
 * 
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(m) - 需要存储链表A的所有节点
 * 
 * @param headA 链表A的头节点
 * @param headB 链表B的头节点
 * @return 公共节点指针，如果没有则返回NULL
 */

// 简单哈希表实现
#define HASH_SIZE 1000

typedef struct HashEntry {
    ListNode *node;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry *buckets[HASH_SIZE];
} HashTable;

// 哈希函数
int hash_function(ListNode *node) {
    return ((uintptr_t)node / sizeof(ListNode)) % HASH_SIZE;
}

// 初始化哈希表
void init_hash_table(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
}

// 向哈希表添加节点
void hash_insert(HashTable *table, ListNode *node) {
    int index = hash_function(node);
    
    HashEntry *entry = (HashEntry*)malloc(sizeof(HashEntry));
    entry->node = node;
    entry->next = table->buckets[index];
    table->buckets[index] = entry;
}

// 检查节点是否在哈希表中
bool hash_contains(HashTable *table, ListNode *node) {
    int index = hash_function(node);
    HashEntry *entry = table->buckets[index];
    
    while (entry) {
        if (entry->node == node) {
            return true;
        }
        entry = entry->next;
    }
    
    return false;
}

// 释放哈希表
void free_hash_table(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = table->buckets[i];
        while (entry) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
}

ListNode* find_intersection_hash_table(ListNode *headA, ListNode *headB) {
    printf("\n=== 方法2：哈希表解法 ===\n");
    printf("算法：用哈希表存储链表A的节点，然后检查链表B\n");
    printf("时间复杂度：O(m+n), 空间复杂度：O(m)\n");
    
    if (!headA || !headB) {
        printf("其中一个链表为空，无公共节点\n");
        return NULL;
    }
    
    HashTable table;
    init_hash_table(&table);
    
    // 第一步：将链表A的所有节点存入哈希表
    printf("第一步：将链表A的节点存入哈希表\n");
    ListNode *current = headA;
    while (current) {
        hash_insert(&table, current);
        printf("  存储节点：值=%d, 地址=%p\n", current->val, (void*)current);
        current = current->next;
    }
    
    // 第二步：遍历链表B，检查是否在哈希表中
    printf("第二步：检查链表B的节点是否在哈希表中\n");
    current = headB;
    while (current) {
        printf("  检查节点：值=%d, 地址=%p\n", current->val, (void*)current);
        if (hash_contains(&table, current)) {
            printf("找到公共节点：值=%d, 地址=%p\n", current->val, (void*)current);
            free_hash_table(&table);
            return current;
        }
        current = current->next;
    }
    
    printf("未找到公共节点\n");
    free_hash_table(&table);
    return NULL;
}

/**
 * @brief 方法3：双指针解法（最优解）
 * 
 * 算法思路（核心洞察）：
 * 1. 设链表A长度为m，链表B长度为n
 * 2. 如果有公共节点，设公共部分长度为c
 * 3. 那么A的非公共部分长度为(m-c)，B的非公共部分长度为(n-c)
 * 4. 关键洞察：(m-c) + n = (n-c) + m
 * 5. 两个指针分别遍历A+B和B+A，会在公共节点相遇
 * 
 * 具体过程：
 * - 指针pA从headA开始，走完A后继续走B
 * - 指针pB从headB开始，走完B后继续走A
 * - 如果有公共节点，两指针会在公共节点相遇
 * - 如果没有公共节点，两指针会同时走到NULL
 * 
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 * 
 * @param headA 链表A的头节点
 * @param headB 链表B的头节点
 * @return 公共节点指针，如果没有则返回NULL
 */
ListNode* find_intersection_two_pointers(ListNode *headA, ListNode *headB) {
    printf("\n=== 方法3：双指针解法（最优解）===\n");
    printf("算法：两指针分别走A+B和B+A路径，在公共节点相遇\n");
    printf("时间复杂度：O(m+n), 空间复杂度：O(1)\n");
    
    if (!headA || !headB) {
        printf("其中一个链表为空，无公共节点\n");
        return NULL;
    }
    
    ListNode *pA = headA;
    ListNode *pB = headB;
    int step = 0;
    
    printf("开始双指针遍历：\n");
    
    while (pA != pB) {
        step++;
        
        // 指针A：走完链表A后继续走链表B
        if (pA == NULL) {
            pA = headB;
            printf("步骤%d: 指针A到达链表A末尾，切换到链表B头部\n", step);
        } else {
            printf("步骤%d: 指针A在节点%d\n", step, pA->val);
            pA = pA->next;
        }
        
        // 指针B：走完链表B后继续走链表A
        if (pB == NULL) {
            pB = headA;
            printf("        指针B到达链表B末尾，切换到链表A头部\n");
        } else {
            printf("        指针B在节点%d\n", pB->val);
            pB = pB->next;
        }
        
        // 防止无限循环（理论上不会发生）
        if (step > 1000) {
            printf("步骤过多，可能存在问题\n");
            break;
        }
    }
    
    if (pA == pB && pA != NULL) {
        printf("找到公共节点：值=%d, 地址=%p, 总步数=%d\n", 
               pA->val, (void*)pA, step);
        return pA;
    } else {
        printf("未找到公共节点，两指针都到达NULL, 总步数=%d\n", step);
        return NULL;
    }
}

/**
 * @brief 方法4：长度差值法
 * 
 * 算法思路：
 * 1. 先计算两个链表的长度
 * 2. 让长链表的指针先走|lenA - lenB|步
 * 3. 然后两个指针同时走，第一个相等的节点就是公共节点
 * 
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 * 
 * @param headA 链表A的头节点
 * @param headB 链表B的头节点
 * @return 公共节点指针，如果没有则返回NULL
 */
ListNode* find_intersection_length_diff(ListNode *headA, ListNode *headB) {
    printf("\n=== 方法4：长度差值法 ===\n");
    printf("算法：计算长度差，让长链表先走差值步数，然后同步前进\n");
    printf("时间复杂度：O(m+n), 空间复杂度：O(1)\n");
    
    if (!headA || !headB) {
        printf("其中一个链表为空，无公共节点\n");
        return NULL;
    }
    
    // 计算两个链表的长度
    int lenA = get_list_length(headA);
    int lenB = get_list_length(headB);
    
    printf("链表A长度：%d\n", lenA);
    printf("链表B长度：%d\n", lenB);
    
    ListNode *pA = headA;
    ListNode *pB = headB;
    
    // 让长链表的指针先走差值步数
    int diff = abs(lenA - lenB);
    printf("长度差值：%d\n", diff);
    
    if (lenA > lenB) {
        printf("链表A更长，指针A先走%d步\n", diff);
        for (int i = 0; i < diff; i++) {
            printf("  指针A从节点%d前进", pA->val);
            pA = pA->next;
            if (pA) {
                printf("到节点%d\n", pA->val);
            } else {
                printf("到NULL\n");
            }
        }
    } else {
        printf("链表B更长，指针B先走%d步\n", diff);
        for (int i = 0; i < diff; i++) {
            printf("  指针B从节点%d前进", pB->val);
            pB = pB->next;
            if (pB) {
                printf("到节点%d\n", pB->val);
            } else {
                printf("到NULL\n");
            }
        }
    }
    
    // 现在两个指针距离链表末尾的距离相同，同步前进
    printf("开始同步前进：\n");
    int step = 0;
    while (pA && pB) {
        step++;
        printf("步骤%d: 指针A在节点%d，指针B在节点%d\n", 
               step, pA->val, pB->val);
        
        if (pA == pB) {
            printf("找到公共节点：值=%d, 地址=%p\n", pA->val, (void*)pA);
            return pA;
        }
        
        pA = pA->next;
        pB = pB->next;
    }
    
    printf("未找到公共节点\n");
    return NULL;
}

/**
 * @brief 创建测试链表
 * 
 * 创建如下结构的链表：
 * 
 * ListA: 1 -> 2 -> 3 ----\
 *                         \
 *                          6 -> 7 -> 8 -> NULL
 *                         /
 * ListB:      4 -> 5 ----/
 * 
 * 公共节点从值为6的节点开始
 */
void create_test_lists(ListNode **headA, ListNode **headB, ListNode **common_start) {
    printf("\n=== 创建测试链表 ===\n");
    
    // 创建链表A的独有部分：1 -> 2 -> 3
    ListNode *nodeA1 = create_node(1);
    ListNode *nodeA2 = create_node(2);
    ListNode *nodeA3 = create_node(3);
    
    nodeA1->next = nodeA2;
    nodeA2->next = nodeA3;
    
    // 创建链表B的独有部分：4 -> 5
    ListNode *nodeB1 = create_node(4);
    ListNode *nodeB2 = create_node(5);
    
    nodeB1->next = nodeB2;
    
    // 创建公共部分：6 -> 7 -> 8
    ListNode *common1 = create_node(6);
    ListNode *common2 = create_node(7);
    ListNode *common3 = create_node(8);
    
    common1->next = common2;
    common2->next = common3;
    common3->next = NULL;
    
    // 连接链表
    nodeA3->next = common1;  // 链表A连接到公共部分
    nodeB2->next = common1;  // 链表B连接到公共部分
    
    *headA = nodeA1;
    *headB = nodeB1;
    *common_start = common1;
    
    printf("测试链表创建完成：\n");
    printf("链表A: 1 -> 2 -> 3 -> 6 -> 7 -> 8 -> NULL\n");
    printf("链表B: 4 -> 5 -> 6 -> 7 -> 8 -> NULL\n");
    printf("公共部分从节点6开始，地址=%p\n", (void*)common1);
}

/**
 * @brief 创建无公共节点的测试链表
 */
void create_no_intersection_lists(ListNode **headA, ListNode **headB) {
    printf("\n=== 创建无公共节点的测试链表 ===\n");
    
    // 创建链表A：1 -> 2 -> 3 -> NULL
    ListNode *nodeA1 = create_node(1);
    ListNode *nodeA2 = create_node(2);
    ListNode *nodeA3 = create_node(3);
    
    nodeA1->next = nodeA2;
    nodeA2->next = nodeA3;
    nodeA3->next = NULL;
    
    // 创建链表B：4 -> 5 -> 6 -> NULL
    ListNode *nodeB1 = create_node(4);
    ListNode *nodeB2 = create_node(5);
    ListNode *nodeB3 = create_node(6);
    
    nodeB1->next = nodeB2;
    nodeB2->next = nodeB3;
    nodeB3->next = NULL;
    
    *headA = nodeA1;
    *headB = nodeB1;
    
    printf("无公共节点的链表创建完成：\n");
    printf("链表A: 1 -> 2 -> 3 -> NULL\n");
    printf("链表B: 4 -> 5 -> 6 -> NULL\n");
}

/**
 * @brief 释放链表内存（注意处理公共部分）
 */
void free_test_lists(ListNode *headA, ListNode *headB, ListNode *common_start) {
    // 释放链表A的独有部分
    ListNode *current = headA;
    while (current && current != common_start) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
    
    // 释放链表B的独有部分
    current = headB;
    while (current && current != common_start) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
    
    // 释放公共部分
    current = common_start;
    while (current) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
}

/**
 * @brief 释放无公共节点的链表
 */
void free_no_intersection_lists(ListNode *headA, ListNode *headB) {
    // 释放链表A
    while (headA) {
        ListNode *temp = headA;
        headA = headA->next;
        free(temp);
    }
    
    // 释放链表B
    while (headB) {
        ListNode *temp = headB;
        headB = headB->next;
        free(temp);
    }
}

/**
 * @brief 算法复杂度分析
 */
void complexity_analysis() {
    printf("\n=== 算法复杂度分析 ===\n");
    
    printf("方法对比：\n");
    printf("┌─────────────────┬──────────────┬──────────────┬────────────────┐\n");
    printf("│      方法       │  时间复杂度  │  空间复杂度  │      特点      │\n");
    printf("├─────────────────┼──────────────┼──────────────┼────────────────┤\n");
    printf("│ 1. 暴力解法     │   O(m*n)     │    O(1)      │ 简单直接       │\n");
    printf("│ 2. 哈希表法     │   O(m+n)     │    O(m)      │ 空间换时间     │\n");
    printf("│ 3. 双指针法     │   O(m+n)     │    O(1)      │ 最优解法       │\n");
    printf("│ 4. 长度差值法   │   O(m+n)     │    O(1)      │ 直观易懂       │\n");
    printf("└─────────────────┴──────────────┴──────────────┴────────────────┘\n");
    
    printf("\n核心思想分析：\n");
    printf("1. 暴力解法：\n");
    printf("   - 对每个A节点遍历整个B链表\n");
    printf("   - 实现简单但效率低\n");
    printf("   - 适合链表很短的情况\n\n");
    
    printf("2. 哈希表法：\n");
    printf("   - 空间换时间的经典应用\n");
    printf("   - 第一遍存储，第二遍查找\n");
    printf("   - 时间最优但需要额外空间\n\n");
    
    printf("3. 双指针法（最优）：\n");
    printf("   - 核心洞察：(m-c)+n = (n-c)+m\n");
    printf("   - 两指针走不同路径但总长度相同\n");
    printf("   - 时间空间都最优\n");
    printf("   - 数学思维的巧妙应用\n\n");
    
    printf("4. 长度差值法：\n");
    printf("   - 先对齐起点，再同步前进\n");
    printf("   - 逻辑清晰，易于理解\n");
    printf("   - 需要两次遍历计算长度\n");
}

/**
 * @brief 边界情况测试
 */
void test_edge_cases() {
    printf("\n=== 边界情况测试 ===\n");
    
    // 测试1：空链表
    printf("测试1：一个链表为空\n");
    ListNode *node = create_node(1);
    ListNode *result = find_intersection_two_pointers(NULL, node);
    printf("结果：%s\n", result == NULL ? "NULL（正确）" : "非NULL（错误）");
    free(node);
    
    // 测试2：两个空链表
    printf("\n测试2：两个链表都为空\n");
    result = find_intersection_two_pointers(NULL, NULL);
    printf("结果：%s\n", result == NULL ? "NULL（正确）" : "非NULL（错误）");
    
    // 测试3：单节点链表
    printf("\n测试3：两个单节点链表（相同节点）\n");
    ListNode *single_node = create_node(42);
    result = find_intersection_two_pointers(single_node, single_node);
    printf("结果：%s\n", result == single_node ? "找到公共节点（正确）" : "未找到（错误）");
    free(single_node);
    
    // 测试4：单节点链表（不同节点）
    printf("\n测试4：两个单节点链表（不同节点）\n");
    ListNode *node1 = create_node(1);
    ListNode *node2 = create_node(2);
    result = find_intersection_two_pointers(node1, node2);
    printf("结果：%s\n", result == NULL ? "NULL（正确）" : "非NULL（错误）");
    free(node1);
    free(node2);
    
    // 测试5：完全相同的链表
    printf("\n测试5：完全相同的链表（从头开始就是公共的）\n");
    ListNode *shared_head = create_node(1);
    shared_head->next = create_node(2);
    shared_head->next->next = create_node(3);
    
    result = find_intersection_two_pointers(shared_head, shared_head);
    printf("结果：%s\n", result == shared_head ? "找到公共节点（正确）" : "未找到（错误）");
    
    // 释放内存
    while (shared_head) {
        ListNode *temp = shared_head;
        shared_head = shared_head->next;
        free(temp);
    }
}

/**
 * @brief 性能测试
 */
void performance_test() {
    printf("\n=== 性能测试 ===\n");
    
    const int list_size = 1000;
    printf("创建长度为%d的链表进行性能测试\n", list_size);
    
    // 创建长链表A
    ListNode *headA = create_node(0);
    ListNode *currentA = headA;
    for (int i = 1; i < list_size; i++) {
        currentA->next = create_node(i);
        currentA = currentA->next;
    }
    
    // 创建长链表B
    ListNode *headB = create_node(list_size);
    ListNode *currentB = headB;
    for (int i = 1; i < list_size / 2; i++) {
        currentB->next = create_node(list_size + i);
        currentB = currentB->next;
    }
    
    // 创建公共部分
    ListNode *common = create_node(9999);
    currentA->next = common;
    currentB->next = common;
    
    printf("链表A长度：%d\n", list_size + 1);
    printf("链表B长度：%d\n", list_size / 2 + 1);
    
    // 测试双指针方法
    clock_t start = clock();
    ListNode *result = find_intersection_two_pointers(headA, headB);
    clock_t end = clock();
    
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("双指针方法耗时：%.6f 秒\n", time_used);
    printf("找到公共节点：%s\n", result ? "是" : "否");
    if (result) {
        printf("公共节点值：%d\n", result->val);
    }
    
    // 清理内存
    // 释放链表A的独有部分
    ListNode *current = headA;
    while (current && current != common) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
    
    // 释放链表B的独有部分
    current = headB;
    while (current && current != common) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
    
    // 释放公共部分
    free(common);
}

/**
 * @brief 主测试函数
 */
void test_find_intersection() {
    printf("=== 寻找两个链表的公共节点测试 ===\n");
    
    // 测试有公共节点的情况
    ListNode *headA, *headB, *common_start;
    create_test_lists(&headA, &headB, &common_start);
    
    print_list(headA, "链表A");
    print_list(headB, "链表B");
    
    // 测试所有方法
    ListNode *result1 = find_intersection_brute_force(headA, headB);
    ListNode *result2 = find_intersection_hash_table(headA, headB);
    ListNode *result3 = find_intersection_two_pointers(headA, headB);
    ListNode *result4 = find_intersection_length_diff(headA, headB);
    
    // 验证结果
    printf("\n=== 结果验证 ===\n");
    printf("暴力解法结果：%s\n", result1 == common_start ? "正确" : "错误");
    printf("哈希表法结果：%s\n", result2 == common_start ? "正确" : "错误");
    printf("双指针法结果：%s\n", result3 == common_start ? "正确" : "错误");
    printf("长度差值法结果：%s\n", result4 == common_start ? "正确" : "错误");
    
    free_test_lists(headA, headB, common_start);
    
    // 测试无公共节点的情况
    printf("\n--- 测试无公共节点的情况 ---\n");
    create_no_intersection_lists(&headA, &headB);
    
    print_list(headA, "链表A");
    print_list(headB, "链表B");
    
    ListNode *no_result = find_intersection_two_pointers(headA, headB);
    printf("双指针法结果：%s\n", no_result == NULL ? "正确（无公共节点）" : "错误");
    
    free_no_intersection_lists(headA, headB);
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：寻找两个链表的第一个公共节点\n");
    printf("=====================================\n");
    
    // 算法复杂度分析
    complexity_analysis();
    
    // 主要功能测试
    test_find_intersection();
    
    // 边界情况测试
    test_edge_cases();
    
    // 性能测试
    performance_test();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 问题理解：寻找第一个公共节点（不是值相同）\n");
    printf("2. 算法进化：暴力 -> 哈希表 -> 双指针优化\n");
    printf("3. 核心洞察：双指针走不同路径但总长度相同\n");
    printf("4. 复杂度分析：时间空间复杂度的权衡\n");
    printf("5. 边界处理：空链表、单节点、无公共节点\n");
    printf("6. 代码质量：清晰的逻辑和完善的测试\n");
    
    printf("\n=== 扩展思考 ===\n");
    printf("1. 如果链表有环怎么办？\n");
    printf("2. 如何找到所有的公共节点？\n");
    printf("3. 如何处理三个或更多链表的公共节点？\n");
    printf("4. 在分布式系统中如何解决这个问题？\n");
    printf("5. 如何优化内存使用？\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o find_common_node find_common_node.c -g
 * 
 * 运行：
 * ./find_common_node
 * 
 * 调试：
 * gdb ./find_common_node
 * 
 * 内存检查：
 * valgrind --leak-check=full ./find_common_node
 * 
 * 这个实现涵盖了：
 * 1. 四种不同的算法实现
 * 2. 详细的算法思路解释
 * 3. 完整的测试框架
 * 4. 边界情况处理
 * 5. 性能测试和验证
 * 6. 复杂度分析和比较
 * 
 * 面试重点：
 * - 理解双指针法的数学原理
 * - 掌握链表操作的技巧
 * - 能够分析不同算法的优缺点
 * - 考虑边界条件和异常处理
 * - 注重代码质量和测试覆盖
 * 
 * 常见变种：
 * - 带环链表的公共节点
 * - 多个链表的公共节点
 * - 公共子序列问题
 * - 树的公共祖先问题
 */
