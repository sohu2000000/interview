/**
 * @file deep_copy_random_list.c
 * @brief 带随机指针的链表深拷贝实现（O(1)空间复杂度）
 * 
 * 题目：给定一个链表，每个节点除了有next指针外，还有一个random指针指向链表中的任意节点或NULL。
 *       请实现该链表的深拷贝，要求使用O(1)的额外空间复杂度。
 * 
 * 这是一个经典的链表操作面试题，考察：
 * 1. 链表操作的深度理解
 * 2. 指针操作的熟练程度
 * 3. 空间复杂度优化思维
 * 4. 算法设计的创新性
 * 5. 复杂数据结构的处理能力
 * 6. 边界条件和异常处理
 * 
 * @author feliu@nvidia.com
 * @date 2025-10-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/**
 * @brief 带随机指针的链表节点结构
 */
typedef struct RandomListNode {
    int val;                           // 节点值
    struct RandomListNode *next;       // 指向下一个节点
    struct RandomListNode *random;     // 指向链表中任意节点或NULL
} RandomListNode;

/**
 * @brief 创建新节点
 * @param val 节点值
 * @return 新创建的节点指针
 */
RandomListNode* create_node(int val) {
    RandomListNode *node = (RandomListNode*)malloc(sizeof(RandomListNode));
    if (!node) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    node->val = val;
    node->next = NULL;
    node->random = NULL;
    
    return node;
}

/**
 * @brief 打印链表（显示next和random指针信息）
 * @param head 链表头指针
 * @param description 描述信息
 */
void print_random_list(RandomListNode *head, const char *description) {
    printf("\n=== %s ===\n", description);
    
    if (!head) {
        printf("空链表\n");
        return;
    }
    
    // 首先给每个节点分配一个索引，便于显示random指针
    RandomListNode *current = head;
    int index = 0;
    
    printf("节点信息：\n");
    while (current) {
        printf("节点[%d]: 值=%d, 地址=%p", index, current->val, (void*)current);
        
        if (current->next) {
            printf(", next->值=%d", current->next->val);
        } else {
            printf(", next=NULL");
        }
        
        if (current->random) {
            // 找到random指向的节点索引
            RandomListNode *temp = head;
            int random_index = 0;
            while (temp && temp != current->random) {
                temp = temp->next;
                random_index++;
            }
            if (temp) {
                printf(", random->节点[%d](值=%d)", random_index, current->random->val);
            } else {
                printf(", random->外部节点(值=%d)", current->random->val);
            }
        } else {
            printf(", random=NULL");
        }
        
        printf("\n");
        current = current->next;
        index++;
    }
}

/**
 * @brief 方法1：使用哈希表的深拷贝（O(n)空间复杂度）
 * 
 * 算法思路：
 * 1. 第一遍遍历：创建所有新节点，建立原节点到新节点的映射
 * 2. 第二遍遍历：根据映射关系设置next和random指针
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(n) - 需要哈希表存储映射关系
 * 
 * @param head 原链表头指针
 * @return 新链表头指针
 */

// 简单哈希表实现（用于演示）
#define HASH_SIZE 1000

typedef struct HashEntry {
    RandomListNode *original;
    RandomListNode *copy;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry *buckets[HASH_SIZE];
} HashMap;

// 哈希函数
int hash_function(RandomListNode *node) {
    return ((uintptr_t)node / sizeof(RandomListNode)) % HASH_SIZE;
}

// 初始化哈希表
void init_hashmap(HashMap *map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        map->buckets[i] = NULL;
    }
}

// 向哈希表添加映射
void hashmap_put(HashMap *map, RandomListNode *original, RandomListNode *copy) {
    int index = hash_function(original);
    
    HashEntry *entry = (HashEntry*)malloc(sizeof(HashEntry));
    entry->original = original;
    entry->copy = copy;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
}

// 从哈希表获取映射
RandomListNode* hashmap_get(HashMap *map, RandomListNode *original) {
    if (!original) return NULL;
    
    int index = hash_function(original);
    HashEntry *entry = map->buckets[index];
    
    while (entry) {
        if (entry->original == original) {
            return entry->copy;
        }
        entry = entry->next;
    }
    
    return NULL;
}

// 释放哈希表
void free_hashmap(HashMap *map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = map->buckets[i];
        while (entry) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
}

RandomListNode* copy_random_list_hashmap(RandomListNode *head) {
    printf("\n=== 方法1：哈希表解法 ===\n");
    printf("算法：使用哈希表建立原节点到新节点的映射\n");
    printf("时间复杂度：O(n), 空间复杂度：O(n)\n");
    
    if (!head) {
        printf("空链表，返回NULL\n");
        return NULL;
    }
    
    HashMap map;
    init_hashmap(&map);
    
    // 第一遍：创建所有新节点并建立映射
    printf("第一遍遍历：创建新节点并建立映射\n");
    RandomListNode *current = head;
    while (current) {
        RandomListNode *new_node = create_node(current->val);
        hashmap_put(&map, current, new_node);
        printf("  创建节点：原节点值=%d -> 新节点值=%d\n", current->val, new_node->val);
        current = current->next;
    }
    
    // 第二遍：设置next和random指针
    printf("第二遍遍历：设置指针关系\n");
    current = head;
    while (current) {
        RandomListNode *new_node = hashmap_get(&map, current);
        
        // 设置next指针
        new_node->next = hashmap_get(&map, current->next);
        
        // 设置random指针
        new_node->random = hashmap_get(&map, current->random);
        
        printf("  设置节点值=%d的指针关系\n", current->val);
        current = current->next;
    }
    
    RandomListNode *new_head = hashmap_get(&map, head);
    free_hashmap(&map);
    
    return new_head;
}

/**
 * @brief 方法2：O(1)空间复杂度的深拷贝（三步法）
 * 
 * 算法思路（核心创新）：
 * 1. 第一步：在每个原节点后面插入对应的新节点
 *    原链表：A -> B -> C
 *    变成：  A -> A' -> B -> B' -> C -> C'
 * 
 * 2. 第二步：设置新节点的random指针
 *    如果A.random指向C，那么A'.random应该指向C'
 *    由于C'就在C的后面，所以A'.random = A.random.next
 * 
 * 3. 第三步：分离原链表和新链表
 *    恢复原链表：A -> B -> C
 *    得到新链表：A' -> B' -> C'
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1) - 不使用额外的数据结构
 * 
 * @param head 原链表头指针
 * @return 新链表头指针
 */
RandomListNode* copy_random_list_o1_space(RandomListNode *head) {
    printf("\n=== 方法2：O(1)空间复杂度解法（三步法）===\n");
    printf("算法：在原节点后插入新节点，利用位置关系设置random指针\n");
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    
    if (!head) {
        printf("空链表，返回NULL\n");
        return NULL;
    }
    
    // 第一步：在每个原节点后面插入新节点
    printf("\n第一步：在每个原节点后插入对应的新节点\n");
    RandomListNode *current = head;
    while (current) {
        RandomListNode *new_node = create_node(current->val);
        if (!new_node) {
            printf("节点创建失败\n");
            return NULL;
        }
        
        // 插入新节点到当前节点后面
        new_node->next = current->next;
        current->next = new_node;
        
        printf("  在节点%d后插入新节点%d\n", current->val, new_node->val);
        
        // 移动到下一个原节点（跳过刚插入的新节点）
        current = new_node->next;
    }
    
    // 第二步：设置新节点的random指针
    printf("\n第二步：设置新节点的random指针\n");
    current = head;
    while (current) {
        RandomListNode *new_node = current->next;  // 新节点就在原节点后面
        
        if (current->random) {
            // 关键：如果原节点的random指向某个节点，
            // 那么新节点的random应该指向那个节点的拷贝
            // 而拷贝节点就在原节点的next位置
            new_node->random = current->random->next;
            printf("  设置新节点%d的random指向新节点%d\n", 
                   new_node->val, new_node->random->val);
        } else {
            new_node->random = NULL;
            printf("  设置新节点%d的random为NULL\n", new_node->val);
        }
        
        // 移动到下一个原节点
        current = new_node->next;
    }
    
    // 第三步：分离原链表和新链表
    printf("\n第三步：分离原链表和新链表\n");
    RandomListNode *new_head = head->next;  // 新链表的头节点
    RandomListNode *current_old = head;
    RandomListNode *current_new = new_head;
    
    while (current_old) {
        // 恢复原链表的next指针
        current_old->next = current_new->next;
        
        // 设置新链表的next指针
        if (current_new->next) {
            current_new->next = current_new->next->next;
        }
        
        printf("  分离节点：原节点%d，新节点%d\n", 
               current_old->val, current_new->val);
        
        // 移动到下一组节点
        current_old = current_old->next;
        current_new = current_new->next;
    }
    
    printf("分离完成，原链表和新链表都已恢复正确结构\n");
    
    return new_head;
}

/**
 * @brief 验证两个链表是否为深拷贝关系
 * @param original 原链表
 * @param copy 拷贝链表
 * @return true表示是正确的深拷贝，false表示不是
 */
bool verify_deep_copy(RandomListNode *original, RandomListNode *copy) {
    printf("\n=== 验证深拷贝正确性 ===\n");
    
    RandomListNode *orig_curr = original;
    RandomListNode *copy_curr = copy;
    int index = 0;
    
    while (orig_curr && copy_curr) {
        // 检查节点值是否相同
        if (orig_curr->val != copy_curr->val) {
            printf("节点[%d]值不匹配：原=%d, 拷贝=%d\n", 
                   index, orig_curr->val, copy_curr->val);
            return false;
        }
        
        // 检查节点地址是否不同（确保是深拷贝）
        if (orig_curr == copy_curr) {
            printf("节点[%d]地址相同，不是深拷贝\n", index);
            return false;
        }
        
        // 检查random指针的正确性
        if (orig_curr->random == NULL && copy_curr->random != NULL) {
            printf("节点[%d]的random指针不匹配：原=NULL, 拷贝!=NULL\n", index);
            return false;
        }
        
        if (orig_curr->random != NULL && copy_curr->random == NULL) {
            printf("节点[%d]的random指针不匹配：原!=NULL, 拷贝=NULL\n", index);
            return false;
        }
        
        if (orig_curr->random != NULL && copy_curr->random != NULL) {
            if (orig_curr->random->val != copy_curr->random->val) {
                printf("节点[%d]的random指向值不匹配：原=%d, 拷贝=%d\n", 
                       index, orig_curr->random->val, copy_curr->random->val);
                return false;
            }
            
            // 确保random指向的也是拷贝的节点，而不是原节点
            if (copy_curr->random == orig_curr->random) {
                printf("节点[%d]的random指向原链表节点，不是深拷贝\n", index);
                return false;
            }
        }
        
        printf("节点[%d]验证通过：值=%d\n", index, orig_curr->val);
        
        orig_curr = orig_curr->next;
        copy_curr = copy_curr->next;
        index++;
    }
    
    // 检查长度是否一致
    if (orig_curr != NULL || copy_curr != NULL) {
        printf("链表长度不一致\n");
        return false;
    }
    
    printf("深拷贝验证通过！\n");
    return true;
}

/**
 * @brief 释放链表内存
 * @param head 链表头指针的指针
 */
void free_random_list(RandomListNode **head) {
    while (*head) {
        RandomListNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

/**
 * @brief 创建测试链表
 * 创建链表：1 -> 2 -> 3 -> 4 -> 5
 * random指针：
 * - 节点1的random指向节点3
 * - 节点2的random指向节点5
 * - 节点3的random指向节点2
 * - 节点4的random指向节点1
 * - 节点5的random指向NULL
 */
RandomListNode* create_test_list() {
    printf("\n=== 创建测试链表 ===\n");
    
    // 创建节点
    RandomListNode *node1 = create_node(1);
    RandomListNode *node2 = create_node(2);
    RandomListNode *node3 = create_node(3);
    RandomListNode *node4 = create_node(4);
    RandomListNode *node5 = create_node(5);
    
    if (!node1 || !node2 || !node3 || !node4 || !node5) {
        printf("节点创建失败\n");
        return NULL;
    }
    
    // 设置next指针
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;
    node5->next = NULL;
    
    // 设置random指针
    node1->random = node3;  // 1 -> 3
    node2->random = node5;  // 2 -> 5
    node3->random = node2;  // 3 -> 2
    node4->random = node1;  // 4 -> 1
    node5->random = NULL;   // 5 -> NULL
    
    printf("测试链表创建完成\n");
    printf("链表结构：1 -> 2 -> 3 -> 4 -> 5\n");
    printf("random关系：1->3, 2->5, 3->2, 4->1, 5->NULL\n");
    
    return node1;
}

/**
 * @brief 创建复杂测试链表（包含自环和交叉引用）
 */
RandomListNode* create_complex_test_list() {
    printf("\n=== 创建复杂测试链表 ===\n");
    
    RandomListNode *nodeA = create_node(10);
    RandomListNode *nodeB = create_node(20);
    RandomListNode *nodeC = create_node(30);
    
    if (!nodeA || !nodeB || !nodeC) {
        return NULL;
    }
    
    // 设置next指针
    nodeA->next = nodeB;
    nodeB->next = nodeC;
    nodeC->next = NULL;
    
    // 设置复杂的random指针关系
    nodeA->random = nodeA;  // 自环
    nodeB->random = nodeC;  // 指向后面
    nodeC->random = nodeA;  // 指向前面
    
    printf("复杂链表创建完成\n");
    printf("链表结构：10 -> 20 -> 30\n");
    printf("random关系：10->10(自环), 20->30, 30->10\n");
    
    return nodeA;
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
    printf("│ 1. 哈希表方法   │    O(n)      │    O(n)      │ 直观易懂       │\n");
    printf("│ 2. 三步法       │    O(n)      │    O(1)      │ 空间最优       │\n");
    printf("└─────────────────┴──────────────┴──────────────┴────────────────┘\n");
    
    printf("\n核心思想分析：\n");
    printf("1. 哈希表方法：\n");
    printf("   - 使用额外空间存储原节点到新节点的映射\n");
    printf("   - 两遍遍历：第一遍创建节点，第二遍设置指针\n");
    printf("   - 空间换时间的典型应用\n\n");
    
    printf("2. 三步法（O(1)空间）：\n");
    printf("   - 巧妙利用链表结构本身存储映射关系\n");
    printf("   - 在原节点后插入新节点，利用位置关系\n");
    printf("   - 关键洞察：A.random.next 就是 A'.random\n");
    printf("   - 三个阶段：插入、设置random、分离\n\n");
    
    printf("算法创新点：\n");
    printf("- 将映射关系隐式存储在链表结构中\n");
    printf("- 利用相对位置关系解决指针设置问题\n");
    printf("- 临时修改原链表结构，最后恢复\n");
}

/**
 * @brief 边界情况测试
 */
void test_edge_cases() {
    printf("\n=== 边界情况测试 ===\n");
    
    // 测试1：空链表
    printf("测试1：空链表\n");
    RandomListNode *empty_copy1 = copy_random_list_hashmap(NULL);
    RandomListNode *empty_copy2 = copy_random_list_o1_space(NULL);
    printf("哈希表方法结果：%s\n", empty_copy1 == NULL ? "NULL" : "非NULL");
    printf("O(1)空间方法结果：%s\n", empty_copy2 == NULL ? "NULL" : "非NULL");
    
    // 测试2：单节点链表
    printf("\n测试2：单节点链表\n");
    RandomListNode *single = create_node(42);
    single->random = single;  // 自环
    
    print_random_list(single, "原单节点链表");
    
    RandomListNode *single_copy1 = copy_random_list_hashmap(single);
    RandomListNode *single_copy2 = copy_random_list_o1_space(single);
    
    print_random_list(single_copy1, "哈希表方法拷贝结果");
    print_random_list(single_copy2, "O(1)空间方法拷贝结果");
    
    printf("单节点验证结果：%s\n", 
           verify_deep_copy(single, single_copy1) ? "哈希表方法正确" : "哈希表方法错误");
    printf("单节点验证结果：%s\n", 
           verify_deep_copy(single, single_copy2) ? "O(1)空间方法正确" : "O(1)空间方法错误");
    
    // 清理内存
    free_random_list(&single);
    free_random_list(&single_copy1);
    free_random_list(&single_copy2);
    
    // 测试3：所有random都为NULL
    printf("\n测试3：所有random指针都为NULL\n");
    RandomListNode *no_random = create_node(1);
    no_random->next = create_node(2);
    no_random->next->next = create_node(3);
    
    print_random_list(no_random, "无random指针链表");
    
    RandomListNode *no_random_copy = copy_random_list_o1_space(no_random);
    print_random_list(no_random_copy, "拷贝结果");
    
    printf("验证结果：%s\n", 
           verify_deep_copy(no_random, no_random_copy) ? "正确" : "错误");
    
    // 清理内存
    free_random_list(&no_random);
    free_random_list(&no_random_copy);
}

/**
 * @brief 性能测试
 */
void performance_test() {
    printf("\n=== 性能测试 ===\n");
    
    const int list_size = 1000;
    printf("创建长度为%d的链表进行性能测试\n", list_size);
    
    // 创建大链表
    RandomListNode *head = create_node(0);
    RandomListNode *current = head;
    RandomListNode **nodes = (RandomListNode**)malloc(list_size * sizeof(RandomListNode*));
    nodes[0] = head;
    
    for (int i = 1; i < list_size; i++) {
        current->next = create_node(i);
        current = current->next;
        nodes[i] = current;
    }
    
    // 设置随机的random指针
    for (int i = 0; i < list_size; i++) {
        if (i % 3 == 0) {  // 1/3的节点有random指针
            nodes[i]->random = nodes[rand() % list_size];
        }
    }
    
    printf("大链表创建完成，开始性能测试\n");
    
    // 测试O(1)空间方法
    clock_t start = clock();
    RandomListNode *copy = copy_random_list_o1_space(head);
    clock_t end = clock();
    
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("O(1)空间方法耗时：%.6f 秒\n", time_used);
    
    // 验证正确性
    bool is_correct = verify_deep_copy(head, copy);
    printf("大链表验证结果：%s\n", is_correct ? "正确" : "错误");
    
    // 清理内存
    free_random_list(&head);
    free_random_list(&copy);
    free(nodes);
}

/**
 * @brief 主测试函数
 */
void test_deep_copy() {
    printf("=== 带随机指针链表深拷贝测试 ===\n");
    
    // 创建测试链表
    RandomListNode *original = create_test_list();
    if (!original) {
        printf("测试链表创建失败\n");
        return;
    }
    
    print_random_list(original, "原始链表");
    
    // 测试方法1：哈希表方法
    RandomListNode *copy1 = copy_random_list_hashmap(original);
    print_random_list(copy1, "哈希表方法拷贝结果");
    
    bool verify1 = verify_deep_copy(original, copy1);
    printf("哈希表方法验证：%s\n", verify1 ? "通过" : "失败");
    
    // 测试方法2：O(1)空间方法
    RandomListNode *copy2 = copy_random_list_o1_space(original);
    print_random_list(copy2, "O(1)空间方法拷贝结果");
    print_random_list(original, "原链表（应该保持不变）");
    
    bool verify2 = verify_deep_copy(original, copy2);
    printf("O(1)空间方法验证：%s\n", verify2 ? "通过" : "失败");
    
    // 测试复杂情况
    printf("\n--- 复杂情况测试 ---\n");
    RandomListNode *complex = create_complex_test_list();
    print_random_list(complex, "复杂原链表");
    
    RandomListNode *complex_copy = copy_random_list_o1_space(complex);
    print_random_list(complex_copy, "复杂链表拷贝结果");
    
    bool verify_complex = verify_deep_copy(complex, complex_copy);
    printf("复杂链表验证：%s\n", verify_complex ? "通过" : "失败");
    
    // 清理内存
    free_random_list(&original);
    free_random_list(&copy1);
    free_random_list(&copy2);
    free_random_list(&complex);
    free_random_list(&complex_copy);
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：带随机指针的链表深拷贝（O(1)空间复杂度）\n");
    printf("========================================\n");
    
    // 算法复杂度分析
    complexity_analysis();
    
    // 主要功能测试
    test_deep_copy();
    
    // 边界情况测试
    test_edge_cases();
    
    // 性能测试
    performance_test();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 问题理解：深拷贝 vs 浅拷贝的区别\n");
    printf("2. 数据结构：带随机指针的链表结构\n");
    printf("3. 算法创新：O(1)空间的三步法\n");
    printf("4. 实现技巧：临时修改原链表结构\n");
    printf("5. 边界处理：空链表、单节点、自环等\n");
    printf("6. 正确性验证：深拷贝验证方法\n");
    printf("7. 复杂度分析：时间空间复杂度权衡\n");
    
    printf("\n=== 扩展思考 ===\n");
    printf("1. 如果链表有多个随机指针怎么办？\n");
    printf("2. 如何处理循环链表的深拷贝？\n");
    printf("3. 在多线程环境下如何保证安全？\n");
    printf("4. 如何优化大链表的拷贝性能？\n");
    printf("5. 能否用递归方法实现？\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o deep_copy_random_list deep_copy_random_list.c -g
 * 
 * 运行：
 * ./deep_copy_random_list
 * 
 * 调试：
 * gdb ./deep_copy_random_list
 * 
 * 内存检查：
 * valgrind --leak-check=full ./deep_copy_random_list
 * 
 * 这个实现涵盖了：
 * 1. 两种不同复杂度的算法实现
 * 2. 详细的算法步骤解释
 * 3. 完整的测试框架
 * 4. 边界情况处理
 * 5. 性能测试和验证
 * 6. 深拷贝正确性验证
 * 
 * 面试重点：
 * - 理解O(1)空间算法的核心思想
 * - 掌握链表指针操作技巧
 * - 能够处理复杂的数据结构
 * - 具备算法创新思维
 * - 注重代码质量和测试
 * 
 * 常见变种：
 * - 带多个随机指针的链表
 * - 双向链表的深拷贝
 * - 树结构的深拷贝
 * - 图结构的深拷贝
 */
