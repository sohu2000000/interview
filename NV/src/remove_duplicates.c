/**
 * @file remove_duplicates.c
 * @brief 链表去重算法实现的面试题
 * 
 * 题目：给定一个链表，编写函数移除重复元素
 * 
 * 这是一个经典的数据结构面试题，考察：
 * 1. 链表操作的熟练程度
 * 2. 指针操作和内存管理
 * 3. 算法设计和优化
 * 4. 边界条件处理
 * 5. 时间和空间复杂度分析
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
 * @brief 链表节点结构体
 */
typedef struct ListNode {
    int data;                   // 节点数据
    struct ListNode *next;      // 指向下一个节点的指针
} ListNode;

/**
 * @brief 哈希表节点（用于O(n)解法）
 */
typedef struct HashNode {
    int key;                    // 键值
    struct HashNode *next;      // 处理哈希冲突的链表
} HashNode;

/**
 * @brief 简单哈希表结构
 */
#define HASH_SIZE 1000
typedef struct {
    HashNode *buckets[HASH_SIZE];  // 哈希桶数组
} HashTable;

/**
 * @brief 创建新的链表节点
 * 
 * @param data 节点数据
 * @return 新创建的节点指针
 */
ListNode* create_node(int data) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (!node) {
        printf("内存分配失败\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

/**
 * @brief 在链表末尾添加节点
 * 
 * @param head 链表头指针的指针
 * @param data 要添加的数据
 */
void append_node(ListNode **head, int data) {
    ListNode *new_node = create_node(data);
    if (!new_node) return;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    ListNode *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

/**
 * @brief 打印链表
 * 
 * @param head 链表头指针
 * @param description 描述信息
 */
void print_list(ListNode *head, const char *description) {
    printf("%s: ", description);
    
    if (head == NULL) {
        printf("空链表\n");
        return;
    }
    
    ListNode *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}

/**
 * @brief 释放链表内存
 * 
 * @param head 链表头指针的指针
 */
void free_list(ListNode **head) {
    while (*head != NULL) {
        ListNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

/**
 * @brief 方法1：暴力解法 - 对每个节点检查后续节点是否有重复
 * 
 * 算法思路：
 * 1. 遍历每个节点
 * 2. 对于当前节点，检查后续所有节点
 * 3. 如果发现重复，删除重复节点
 * 
 * 时间复杂度：O(n²)
 * 空间复杂度：O(1)
 * 
 * @param head 链表头指针
 * @return 处理后的链表头指针
 */
ListNode* remove_duplicates_brute_force(ListNode *head) {
    printf("\n=== 方法1：暴力解法 ===\n");
    printf("算法：对每个节点，检查后续节点并删除重复项\n");
    printf("时间复杂度：O(n²), 空间复杂度：O(1)\n");
    
    if (head == NULL) {
        printf("空链表，无需处理\n");
        return head;
    }
    
    ListNode *current = head;
    
    // 遍历每个节点
    while (current != NULL && current->next != NULL) {
        ListNode *runner = current;
        
        printf("检查节点 %d 的重复项\n", current->data);
        
        // 检查当前节点后面的所有节点
        while (runner->next != NULL) {
            if (runner->next->data == current->data) {
                // 发现重复，删除节点
                ListNode *duplicate = runner->next;
                runner->next = runner->next->next;
                printf("  删除重复节点：%d\n", duplicate->data);
                free(duplicate);
            } else {
                runner = runner->next;
            }
        }
        
        current = current->next;
    }
    
    return head;
}

/**
 * @brief 方法2：哈希表解法 - 使用哈希表记录已见过的元素
 * 
 * 算法思路：
 * 1. 使用哈希表记录已经出现过的元素
 * 2. 遍历链表，如果元素已存在则删除
 * 3. 否则将元素加入哈希表
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 * 
 * @param head 链表头指针
 * @return 处理后的链表头指针
 */

// 哈希函数
int hash_function(int key) {
    return abs(key) % HASH_SIZE;
}

// 初始化哈希表
void init_hash_table(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
}

// 检查元素是否在哈希表中
bool hash_contains(HashTable *table, int key) {
    int index = hash_function(key);
    HashNode *current = table->buckets[index];
    
    while (current != NULL) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// 向哈希表中添加元素
void hash_insert(HashTable *table, int key) {
    if (hash_contains(table, key)) {
        return; // 已存在
    }
    
    int index = hash_function(key);
    HashNode *new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
}

// 释放哈希表内存
void free_hash_table(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = table->buckets[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
        table->buckets[i] = NULL;
    }
}

ListNode* remove_duplicates_hash(ListNode *head) {
    printf("\n=== 方法2：哈希表解法 ===\n");
    printf("算法：使用哈希表记录已见元素，一次遍历完成去重\n");
    printf("时间复杂度：O(n), 空间复杂度：O(n)\n");
    
    if (head == NULL) {
        printf("空链表，无需处理\n");
        return head;
    }
    
    HashTable table;
    init_hash_table(&table);
    
    ListNode *current = head;
    ListNode *prev = NULL;
    
    while (current != NULL) {
        printf("检查节点：%d\n", current->data);
        
        if (hash_contains(&table, current->data)) {
            // 发现重复，删除当前节点
            printf("  发现重复，删除节点：%d\n", current->data);
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            // 新元素，添加到哈希表
            printf("  新元素，添加到哈希表：%d\n", current->data);
            hash_insert(&table, current->data);
            prev = current;
            current = current->next;
        }
    }
    
    free_hash_table(&table);
    return head;
}

/**
 * @brief 方法3：排序后去重（修改原链表结构）
 * 
 * 算法思路：
 * 1. 先对链表进行排序
 * 2. 遍历排序后的链表，相邻重复元素只保留一个
 * 
 * 时间复杂度：O(n log n) - 排序开销
 * 空间复杂度：O(1)
 * 
 * @param head 链表头指针
 * @return 处理后的链表头指针
 */

// 链表归并排序
ListNode* merge_sorted_lists(ListNode *l1, ListNode *l2) {
    ListNode dummy;
    ListNode *tail = &dummy;
    dummy.next = NULL;
    
    while (l1 && l2) {
        if (l1->data <= l2->data) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* merge_sort_list(ListNode *head) {
    if (!head || !head->next) {
        return head;
    }
    
    // 找到中点
    ListNode *slow = head, *fast = head, *prev = NULL;
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 分割链表
    prev->next = NULL;
    
    // 递归排序
    ListNode *left = merge_sort_list(head);
    ListNode *right = merge_sort_list(slow);
    
    // 合并
    return merge_sorted_lists(left, right);
}

ListNode* remove_duplicates_sorted(ListNode *head) {
    printf("\n=== 方法3：排序后去重 ===\n");
    printf("算法：先排序链表，再移除相邻重复元素\n");
    printf("时间复杂度：O(n log n), 空间复杂度：O(1)\n");
    
    if (head == NULL) {
        printf("空链表，无需处理\n");
        return head;
    }
    
    // 1. 排序链表
    printf("步骤1：对链表进行归并排序\n");
    head = merge_sort_list(head);
    print_list(head, "排序后的链表");
    
    // 2. 移除相邻重复元素
    printf("步骤2：移除相邻重复元素\n");
    ListNode *current = head;
    
    while (current && current->next) {
        if (current->data == current->next->data) {
            ListNode *duplicate = current->next;
            current->next = duplicate->next;
            printf("  删除重复节点：%d\n", duplicate->data);
            free(duplicate);
        } else {
            current = current->next;
        }
    }
    
    return head;
}

/**
 * @brief 方法4：双指针解法（适用于已排序链表）
 * 
 * 算法思路：
 * 1. 假设链表已排序
 * 2. 使用双指针，一个指向当前节点，一个指向下一个不同的节点
 * 3. 跳过所有重复节点
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 
 * @param head 已排序链表头指针
 * @return 处理后的链表头指针
 */
ListNode* remove_duplicates_sorted_list(ListNode *head) {
    printf("\n=== 方法4：双指针解法（适用于已排序链表）===\n");
    printf("算法：双指针遍历，跳过重复元素\n");
    printf("时间复杂度：O(n), 空间复杂度：O(1)\n");
    printf("前提：链表已排序\n");
    
    if (head == NULL) {
        printf("空链表，无需处理\n");
        return head;
    }
    
    ListNode *current = head;
    
    while (current && current->next) {
        if (current->data == current->next->data) {
            ListNode *duplicate = current->next;
            current->next = duplicate->next;
            printf("删除重复节点：%d\n", duplicate->data);
            free(duplicate);
        } else {
            current = current->next;
        }
    }
    
    return head;
}

/**
 * @brief 创建测试链表
 */
ListNode* create_test_list(int *data, int size) {
    ListNode *head = NULL;
    for (int i = 0; i < size; i++) {
        append_node(&head, data[i]);
    }
    return head;
}

/**
 * @brief 复制链表（用于测试不同方法）
 */
ListNode* copy_list(ListNode *head) {
    if (head == NULL) return NULL;
    
    ListNode *new_head = NULL;
    ListNode *current = head;
    
    while (current) {
        append_node(&new_head, current->data);
        current = current->next;
    }
    
    return new_head;
}

/**
 * @brief 验证去重结果
 */
bool verify_no_duplicates(ListNode *head) {
    if (head == NULL) return true;
    
    ListNode *current = head;
    while (current && current->next) {
        ListNode *check = current->next;
        while (check) {
            if (current->data == check->data) {
                return false;
            }
            check = check->next;
        }
        current = current->next;
    }
    return true;
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
    printf("│ 1. 暴力解法     │    O(n²)     │    O(1)      │ 简单直接       │\n");
    printf("│ 2. 哈希表解法   │    O(n)      │    O(n)      │ 时间最优       │\n");
    printf("│ 3. 排序后去重   │  O(n log n)  │    O(1)      │ 改变原序       │\n");
    printf("│ 4. 双指针(已排序)│    O(n)      │    O(1)      │ 需预排序       │\n");
    printf("└─────────────────┴──────────────┴──────────────┴────────────────┘\n");
    
    printf("\n选择建议：\n");
    printf("- 小数据集或内存受限：选择暴力解法\n");
    printf("- 大数据集且内存充足：选择哈希表解法\n");
    printf("- 不要求保持原序：选择排序后去重\n");
    printf("- 已知链表有序：选择双指针解法\n");
}

/**
 * @brief 边界情况测试
 */
void test_edge_cases() {
    printf("\n=== 边界情况测试 ===\n");
    
    // 测试1：空链表
    printf("测试1：空链表\n");
    ListNode *empty = NULL;
    empty = remove_duplicates_hash(empty);
    print_list(empty, "结果");
    
    // 测试2：单节点
    printf("\n测试2：单节点链表\n");
    ListNode *single = create_node(42);
    print_list(single, "原链表");
    single = remove_duplicates_hash(single);
    print_list(single, "结果");
    free_list(&single);
    
    // 测试3：全部相同
    printf("\n测试3：全部节点相同\n");
    int same_data[] = {5, 5, 5, 5, 5};
    ListNode *same = create_test_list(same_data, 5);
    print_list(same, "原链表");
    same = remove_duplicates_hash(same);
    print_list(same, "结果");
    free_list(&same);
    
    // 测试4：无重复
    printf("\n测试4：无重复元素\n");
    int unique_data[] = {1, 2, 3, 4, 5};
    ListNode *unique = create_test_list(unique_data, 5);
    print_list(unique, "原链表");
    unique = remove_duplicates_hash(unique);
    print_list(unique, "结果");
    free_list(&unique);
}

/**
 * @brief 性能测试
 */
void performance_test() {
    printf("\n=== 性能测试 ===\n");
    
    // 创建大链表进行性能测试
    printf("创建包含1000个元素的链表（有重复）\n");
    
    ListNode *large_list = NULL;
    for (int i = 0; i < 1000; i++) {
        append_node(&large_list, i % 100); // 创建重复元素
    }
    
    printf("原链表长度：1000，预期去重后长度：100\n");
    
    // 测试哈希表方法
    ListNode *test_list = copy_list(large_list);
    test_list = remove_duplicates_hash(test_list);
    
    // 计算结果长度
    int count = 0;
    ListNode *current = test_list;
    while (current) {
        count++;
        current = current->next;
    }
    
    printf("去重后实际长度：%d\n", count);
    printf("验证无重复：%s\n", verify_no_duplicates(test_list) ? "通过" : "失败");
    
    free_list(&large_list);
    free_list(&test_list);
}

/**
 * @brief 主测试函数
 */
void test_remove_duplicates() {
    printf("=== 链表去重算法测试 ===\n");
    
    // 测试数据
    int test_data[] = {1, 2, 3, 2, 4, 1, 5, 3, 6};
    int size = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("测试数据：");
    for (int i = 0; i < size; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\n");
    
    // 测试方法1：暴力解法
    ListNode *list1 = create_test_list(test_data, size);
    print_list(list1, "原链表");
    list1 = remove_duplicates_brute_force(list1);
    print_list(list1, "暴力解法结果");
    printf("验证：%s\n", verify_no_duplicates(list1) ? "通过" : "失败");
    
    // 测试方法2：哈希表解法
    ListNode *list2 = create_test_list(test_data, size);
    list2 = remove_duplicates_hash(list2);
    print_list(list2, "哈希表解法结果");
    printf("验证：%s\n", verify_no_duplicates(list2) ? "通过" : "失败");
    
    // 测试方法3：排序后去重
    ListNode *list3 = create_test_list(test_data, size);
    list3 = remove_duplicates_sorted(list3);
    print_list(list3, "排序后去重结果");
    printf("验证：%s\n", verify_no_duplicates(list3) ? "通过" : "失败");
    
    // 测试已排序链表的双指针方法
    int sorted_data[] = {1, 1, 2, 3, 3, 4, 5, 5, 6};
    int sorted_size = sizeof(sorted_data) / sizeof(sorted_data[0]);
    ListNode *list4 = create_test_list(sorted_data, sorted_size);
    print_list(list4, "已排序原链表");
    list4 = remove_duplicates_sorted_list(list4);
    print_list(list4, "双指针解法结果");
    printf("验证：%s\n", verify_no_duplicates(list4) ? "通过" : "失败");
    
    // 清理内存
    free_list(&list1);
    free_list(&list2);
    free_list(&list3);
    free_list(&list4);
}

/**
 * @brief 主函数
 */
int main() {
    printf("题目：给定一个链表，编写函数移除重复元素\n");
    printf("=============================================\n");
    
    // 复杂度分析
    complexity_analysis();
    
    // 主要测试
    test_remove_duplicates();
    
    // 边界情况测试
    test_edge_cases();
    
    // 性能测试
    performance_test();
    
    printf("\n=== 面试要点总结 ===\n");
    printf("1. 理解问题：是否保持原序？是否已排序？\n");
    printf("2. 算法选择：根据数据规模和内存限制选择合适算法\n");
    printf("3. 边界处理：空链表、单节点、全重复、无重复\n");
    printf("4. 内存管理：正确释放删除节点的内存\n");
    printf("5. 复杂度分析：时间和空间复杂度的权衡\n");
    printf("6. 代码质量：清晰的逻辑、适当的注释、错误处理\n");
    
    return 0;
}

/**
 * @brief 编译和运行说明
 * 
 * 编译：
 * gcc -o remove_duplicates remove_duplicates.c -g
 * 
 * 运行：
 * ./remove_duplicates
 * 
 * 调试：
 * gdb ./remove_duplicates
 * 
 * 内存检查：
 * valgrind --leak-check=full ./remove_duplicates
 * 
 * 这个实现涵盖了：
 * 1. 多种算法实现和比较
 * 2. 完整的测试用例
 * 3. 边界情况处理
 * 4. 性能分析
 * 5. 内存管理
 * 
 * 面试准备：
 * - 熟练掌握链表基本操作
 * - 理解不同算法的优缺点
 * - 能够分析时间空间复杂度
 * - 考虑实际应用场景
 */
